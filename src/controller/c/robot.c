/*
 * Copyright 1996-2021 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// generic robot controller API
//
// this module includes:
// (1) robot initialization
// (2) management of devices (device list, requests, answers)
// (3) handling basic robot requests
// (4) initialization of the remote scene if any (textures, download)

#include <locale.h>  // LC_NUMERIC
#include <signal.h>  // signal
#include <stdarg.h>
#include <stdio.h>   // snprintf
#include <stdlib.h>  // exit
#include <string.h>  // strlen
#include <unistd.h>  // sleep, pipe, dup2, STDOUT_FILENO, STDERR_FILENO

#include <webots/joystick.h>
#include <webots/keyboard.h>
#include <webots/mouse.h>
#include <webots/robot.h>
#include <webots/supervisor.h>
#include <webots/types.h>
#include <webots/utils/system.h>
#include "device_private.h"
#include "joystick_private.h"
#include "keyboard_private.h"
#include "messages.h"
#include "motion_private.h"
#include "mouse_private.h"
#include "request.h"
#include "robot_private.h"
#include "scheduler.h"
#include "supervisor_private.h"

#include "default_robot_window_private.h"
#include "html_robot_window_private.h"
#include "remote_control_private.h"
#include "robot_window_private.h"

#ifdef _WIN32
#include <windows.h>  // GetCommandLine
#else
#include <pthread.h>
#endif

#define WEBOTS_EXIT_FALSE 0
#define WEBOTS_EXIT_NOW 1
#define WEBOTS_EXIT_LATER 2

typedef struct {
  WbDevice **device;  // array of devices
  double battery_value;
  void (*real_robot_cleanup)(void);
  int battery_sampling_period;
  int n_device;  // number of devices, including the robot itself
  bool is_supervisor;
  unsigned char synchronization;
  WbRobotMode mode;
  unsigned char configure;  // 1 if just configured: reset need to be called
  unsigned char client_exit;
  unsigned char webots_exit;  // WEBOTS_EXIT_FALSE, WEBOTS_EXIT_NOW or WEBOTS_EXIT_LATER
  double basic_time_step;
  char console_stream;
  char *console_text;
  char *project_path;
  char *world_path;
  char *model;
  char *window_filename;
  char *remote_control_filename;
  char *controller_name;
  char *urdf;
  bool need_urdf;
  char *urdf_prefix;
  char *custom_data;
  bool is_immediate_message;
  bool is_waiting_for_user_input_event;
  WbUserInputEvent user_input_event_type;
  int user_input_event_timeout;
  bool dataNeedToWriteRequest;
  bool show_window;
  bool has_html_robot_window;
  bool update_window;
  bool toggle_remote_first_step;  // true in the first step after a switch between remote/simulation
  bool send_remote_mode_off;      // tell to send the REMOTE_OFF message in the next robot_write_request()
  int pin;
  int wwi_message_to_send_size;
  const char *wwi_message_to_send;
  int wwi_message_received_size;
  char *wwi_message_received;
  WbSimulationMode simulation_mode;  // WB_SUPERVISOR_SIMULATION_MODE_FAST, etc.
} WbRobot;

static bool robot_init_was_done = false;
static WbRobot robot;
static WbMutexRef robot_step_mutex;
static double simulation_time = 0.0;
static unsigned int current_step_duration = 0;
static bool should_abort_simulation_waiting = false;

// Static functions
static void init_robot_window_library() {
  if (robot_window_is_initialized())
    return;

  robot_window_init(robot.window_filename);
  if (!robot_window_is_initialized())
    fprintf(stderr, "Error: Cannot load the \"%s\" robot window library.\n", robot.window_filename);
}

static void quit_controller(int signal_number) {
  should_abort_simulation_waiting = true;
  signal(signal_number, SIG_DFL);
  raise(signal_number);
}

static void init_remote_control_library() {
  if (remote_control_is_initialized())
    return;

  if (strlen(robot.remote_control_filename) > 0) {
    remote_control_init(robot.remote_control_filename);
    if (!remote_control_is_initialized())
      fprintf(stderr, "Error: Cannot load the \"%s\" remote control library.\n", robot.remote_control_filename);
  }
}

static void init_devices_from_tag(WbRequest *r, int firstTag) {
  for (int tag = firstTag; tag < robot.n_device; tag++) {
    robot.device[tag] = malloc(sizeof(WbDevice));
    robot.device[tag]->node = request_read_uint16(r);
    robot.device[tag]->name = request_read_string(r);
    robot.device[tag]->model = request_read_string(r);
    // printf("reading %s (%d) (%s)\n", robot.device[tag]->name, robot.device[tag]->node, robot.device[tag]->model);
    wb_device_init(robot.device[tag]);  // set device specific fields (read_answer and device_data)
  }
}

static void robot_quit() {  // called when Webots kills a controller
  WbDeviceTag tag;
  for (tag = 0; tag < robot.n_device; tag++)
    wb_device_cleanup(robot.device[tag]);
  free(robot.device);
  robot.device = NULL;
  robot.n_device = 0;
  if (robot.real_robot_cleanup)
    (*(robot.real_robot_cleanup))();
  scheduler_cleanup();
  motion_cleanup();
  free(robot.project_path);
  robot.project_path = NULL;
  free(robot.world_path);
  robot.world_path = NULL;
  free(robot.model);
  robot.model = NULL;
  free(robot.window_filename);
  robot.window_filename = NULL;
  free(robot.controller_name);
  robot.controller_name = NULL;
  free(robot.custom_data);
  robot.custom_data = NULL;
  free(robot.console_text);
  robot.console_text = NULL;
  free(robot.wwi_message_received);
  robot.wwi_message_received = NULL;
  robot_window_cleanup();
  remote_control_cleanup();
  free(robot.urdf);
  free(robot.urdf_prefix);
}

// this function is also called from supervisor_write_request()
void robot_write_request(WbDevice *dev, WbRequest *req) {
  keyboard_write_request(req);
  joystick_write_request(req);
  mouse_write_request(req);
  if (robot.battery_value < 0.0) {  // need to enable or disable
    request_write_uchar(req, C_ROBOT_SET_BATTERY_SAMPLING_PERIOD);
    request_write_uint16(req, robot.battery_sampling_period);
    robot.battery_value = 0.0;
  }
  if (robot.dataNeedToWriteRequest) {  // need to send new robot data
    request_write_uchar(req, C_ROBOT_SET_DATA);
    request_write_uint16(req, strlen(robot.custom_data) + 1);
    request_write_string(req, robot.custom_data);
    robot.dataNeedToWriteRequest = false;
  }
  if (robot.console_text) {
    request_write_uchar(req, C_CONSOLE_MESSAGE);
    request_write_uchar(req, robot.console_stream);
    request_write_uint32(req, strlen(robot.console_text) + 1);
    request_write_string(req, robot.console_text);
    free(robot.console_text);
    robot.console_text = NULL;
  }
  if (robot.client_exit) {
    request_write_uchar(req, C_ROBOT_CLIENT_EXIT_NOTIFY);
  }
  if (robot.send_remote_mode_off == true) {
    robot.send_remote_mode_off = false;
    request_write_uchar(req, C_ROBOT_REMOTE_OFF);
  }
  if (robot.pin >= 0) {
    request_write_uchar(req, C_ROBOT_PIN);
    request_write_uchar(req, robot.pin != 0);
    robot.pin = -1;
  }
  if (robot.wwi_message_to_send_size) {
    request_write_uchar(req, C_ROBOT_WWI_MESSAGE);
    request_write_int32(req, robot.wwi_message_to_send_size);
    request_write_data(req, robot.wwi_message_to_send, robot.wwi_message_to_send_size);
    robot.wwi_message_to_send_size = 0;
  }
  if (robot.is_waiting_for_user_input_event) {
    request_write_uchar(req, C_ROBOT_WAIT_FOR_USER_INPUT_EVENT);
    request_write_int32(req, robot.user_input_event_type);
    request_write_int32(req, robot.user_input_event_timeout);
  }
  if (robot.need_urdf) {
    request_write_uchar(req, C_ROBOT_URDF);
    request_write_uint16(req, strlen(robot.urdf_prefix) + 1);
    request_write_string(req, robot.urdf_prefix);
  }
}

static WbRequest *generate_request(unsigned int step_duration, bool toggle_remote) {
  WbRequest *req = request_new_empty();
  request_write_uint32(req, step_duration);

  WbDeviceTag tag;
  for (tag = 0; tag < robot.n_device; tag++) {
    int begin = request_get_position(req);
    request_write_tag(req, tag);
    request_write_int32(req, 0);  // reserve 1 int for the size
    int before = request_get_position(req);
    if (toggle_remote && robot.device[tag]->toggle_remote)
      robot.device[tag]->toggle_remote(robot.device[tag], req);
    else
      robot.device[tag]->write_request(robot.device[tag], req);

    int after = request_get_position(req);
    int size = after - before;  // calc number of bytes in this request
    if (size > 0) {
      // write only if not empty
      request_set_position(req, before - sizeof(int));
      request_write_int32(req, size);  // now write size in reserved int
      request_set_position(req, after);
    } else
      // reset to beginning -> 0 bytes will be sent
      request_set_position(req, begin);
  }
  request_write_size(req);
  return req;
}

static void robot_send_request(unsigned int step_duration) {
  WbRequest *req = generate_request(step_duration, false);

  if (wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL) {
    // it returns a partial request copy to be sent to Webots.
    WbRequest *copy = remote_control_handle_messages(req);
    request_delete(req);
    req = copy;
    remote_control_step(step_duration);
  }

  // scheduler_print_request(req);
  if (scheduler_is_local())
    scheduler_send_request(req);
  else if (request_get_size(req) != 8)
    scheduler_send_request(req);
  request_delete(req);
}

// rebuild the device list
static void robot_configure(WbRequest *r) {
  // delete all the devices except the robot
  WbDeviceTag tag;
  for (tag = 1; tag < robot.n_device; tag++)
    wb_device_cleanup(robot.device[tag]);

  // read the supervisor
  robot.is_supervisor = request_read_uchar(r);
  // read the synchronization
  robot.synchronization = request_read_uchar(r);
  // reading number of devices
  robot.n_device = request_read_uint16(r);
  ROBOT_ASSERT(robot.n_device > 0);
  WbDevice *d = robot.device[0];  // save pointer to the root device
  free(robot.device);
  robot.device = malloc(sizeof(WbDevice *) * robot.n_device);
  robot.device[0] = d;  // restore pointer to root device
  robot.device[0]->node = request_read_uint16(r);
  simulation_time = request_read_double(r);
  free(robot.device[0]->name);
  robot.device[0]->name = request_read_string(r);

  // printf("robot.is_supervisor = %d\n", robot.is_supervisor);
  // printf("robot.synchronization = %d\n", robot.synchronization);
  // printf("robo.n_device = %d\n", robot.n_device);
  // printf("robot.device[0]->node = %d\n", robot.device[0]->node);
  // printf("robot.device[0]->name = %s\n", robot.device[0]->name);

  switch (robot.device[0]->node) {
    case WB_NODE_ROBOT:
      if (robot.is_supervisor)
        wb_supervisor_init(robot.device[0]);
      break;
    default:
      ROBOT_ASSERT(0);
      break;
  }
  // reading device names
  init_devices_from_tag(r, 1);

  robot.configure = 1;
  robot.basic_time_step = request_read_double(r);
  robot.project_path = request_read_string(r);
  robot.world_path = request_read_string(r);
  robot.model = request_read_string(r);
  robot.window_filename = request_read_string(r);
  robot.remote_control_filename = request_read_string(r);
  robot.controller_name = request_read_string(r);
  robot.custom_data = request_read_string(r);
  robot.show_window = request_read_uchar(r);
  robot.has_html_robot_window = request_read_uchar(r);
  wb_robot_window_load_library(robot.window_filename);
  robot.simulation_mode = request_read_int32(r);
  robot.is_immediate_message = false;
  // printf("configure done\n");
}

static char robot_read_data() {
  bool immediate = false;

  do {
    robot_mutex_unlock_step();
    robot_window_update_gui();
    robot_mutex_lock_step();

    WbRequest *r = scheduler_read_data();
    while (r == NULL) {
      fprintf(stderr, "Warning: %s(): received empty data request!\n", __FUNCTION__);
      r = scheduler_read_data();
    }
    while (request_is_over(r) == false) {
      WbDeviceTag tag = request_read_tag(r);
      // fprintf(stderr, "robot_read_data: tag=%d\n", tag);
      ROBOT_ASSERT(tag < robot.n_device);
      WbDevice *dev = robot.device[tag];
      dev->read_answer(dev, r);
    }

    immediate = request_is_immediate(r);

    request_delete(r);

    if (immediate) {
      if (robot.show_window && !robot.has_html_robot_window) {  // Qt-based robot window
        robot_mutex_unlock_step();
        // initialize first the remote control library in order
        // to have access to the real robot at the window creation
        // to get custom data
        init_robot_window_library();
        robot_window_show();
        robot_mutex_lock_step();
        robot.show_window = false;
      }
      if (robot.update_window) {  // HTML robot window
        robot_mutex_unlock_step();
        html_robot_window_step(0);
        robot_mutex_lock_step();
        robot.update_window = false;
      }
      robot_window_pre_update_gui();
    }

  } while (immediate);

  return true;
}

void robot_read_answer(WbDevice *d, WbRequest *r) {
  int n;
  int message = request_read_uchar(r);

  if (keyboard_read_answer(message, r))
    return;

  if (joystick_read_answer(message, r))
    return;

  if (mouse_read_answer(message, r))
    return;

  switch (message) {
    case C_ROBOT_TIME:
      simulation_time = request_read_double(r);
      break;
    case C_CONFIGURE:
      robot_configure(r);
      break;
    case C_ROBOT_BATTERY_VALUE:
      // printf("received robot battery value\n");
      robot.battery_value = request_read_double(r);
      break;
    case C_ROBOT_DATA:
      free(robot.custom_data);
      robot.custom_data = request_read_string(r);
      break;
    case C_ROBOT_SUPERVISOR:
      robot.is_supervisor = request_read_uchar(r);
      break;
    case C_ROBOT_MODEL:
      free(robot.model);
      robot.model = request_read_string(r);
      break;
    case C_ROBOT_NEW_DEVICE:
      n = request_read_uint16(r);
      robot.device = realloc(robot.device, sizeof(WbDevice *) * (robot.n_device + n));
      if (!robot.device) {
        fprintf(stderr, "Error initializing the new device: not enough memory.\n");
        exit(EXIT_FAILURE);
      }
      const int firstTag = robot.n_device;
      robot.n_device += n;
      init_devices_from_tag(r, firstTag);
    case C_ROBOT_WINDOW_SHOW:
      robot.show_window = true;
      break;
    case C_ROBOT_WINDOW_UPDATE:
      robot.update_window = true;
      break;
    case C_ROBOT_WWI_MESSAGE:
      n = request_read_int32(r);
      if (n > robot.wwi_message_received_size)
        robot.wwi_message_received = realloc(robot.wwi_message_received, n);
      robot.wwi_message_received_size = n;
      memcpy(robot.wwi_message_received, request_read_data(r, n), n);
      break;
    case C_ROBOT_SIMULATION_CHANGE_MODE:
      robot.simulation_mode = request_read_int32(r);
      robot_mutex_unlock_step();
      if (robot.simulation_mode == WB_SUPERVISOR_SIMULATION_MODE_PAUSE && wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL)
        remote_control_stop_actuators();
      robot_mutex_lock_step();
      break;
    case C_ROBOT_QUIT:
      robot.webots_exit = WEBOTS_EXIT_NOW;
      break;
    case C_ROBOT_WAIT_FOR_USER_INPUT_EVENT:
      robot.is_waiting_for_user_input_event = false;
      robot.user_input_event_type = request_read_int32(r);
      break;
    case C_ROBOT_URDF:
      free(robot.urdf);
      robot.urdf = request_read_string(r);
      break;
    default:
      r->pointer--;  // unread the char from the request
      break;
  }
}

// Protected funtions available from other files of the client library

const char *robot_get_device_name(WbDeviceTag tag) {
  if (tag < robot.n_device)
    return robot.device[tag]->name;

  return NULL;
}

const char *robot_get_device_model(WbDeviceTag tag) {
  if (tag < robot.n_device)
    return robot.device[tag]->model;

  return NULL;
}

int robot_get_number_of_devices() {
  return robot.n_device;
}

WbDevice *robot_get_robot_device() {
  return robot.device[0];
}

static const char *robot_get_type_name() {
  switch (robot.device[0]->node) {
    case WB_NODE_ROBOT:
      return "Robot";
    default:
      ROBOT_ASSERT(0);
  }

  return NULL;
}

int robot_check_supervisor(const char *func_name) {
  if (robot.is_supervisor)
    return 1;  // OK

  fprintf(stderr, "Error: ignoring illegal call to %s() in a '%s' controller.\n", func_name, robot_get_type_name());
  fprintf(stderr, "Error: this function can only be used in a 'Supervisor' controller.\n");
  return 0;
}

WbDevice *robot_get_device_with_node(WbDeviceTag tag, WbNodeType node, bool warning) {
  if (tag < robot.n_device) {  // exists
    WbDevice *d = robot.device[tag];
    if (d->node == node)
      return d;
    else
      return NULL;
  }
  if (warning)
    fprintf(stderr, "Error: device with tag=%d not found.\n", (int)tag);
  return NULL;
}

void wb_robot_cleanup() {  // called when the client quits
  html_robot_window_cleanup();
  default_robot_window_cleanup();

  if (robot.n_device == 0)
    return;

  if (wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL)
    wb_robot_set_mode(0, NULL);

  robot.client_exit = true;
  wb_robot_step(0);
  robot_quit();
}

void robot_mutex_lock_step() {
  wb_robot_mutex_lock(robot_step_mutex);
}

void robot_mutex_unlock_step() {
  wb_robot_mutex_unlock(robot_step_mutex);
}

void robot_step_begin(int duration) {
  motion_step_all(duration);
  robot_send_request(duration);
}

int robot_step_end() {
  keyboard_step_end();
  joystick_step_end();
  robot_read_data();
  if (robot.webots_exit == WEBOTS_EXIT_FALSE)
    return scheduler_actual_step;

  return -1;
}

WbDeviceTag robot_get_device_tag(WbDevice *d) {
  WbDeviceTag tag;
  for (tag = 0; tag < robot.n_device; tag++) {
    if (robot.device[tag] == d)
      return tag;
  }
  return 0;
}

void robot_abort(const char *format, ...) {
  va_list args;
  char message[1024];
  va_start(args, format);
  vsprintf(message, format, args);
  va_end(args);
  fprintf(stderr, "abort: %s\n", message);
  robot_send_request(0);
  robot_read_data();
  exit(EXIT_FAILURE);
}

WbNodeType robot_get_device_type(WbDeviceTag tag) {
  int ti = (int)tag;
  if (ti >= 0 && ti < robot.n_device)
    return robot.device[ti]->node;
  return WB_NODE_NO_NODE;
}

int robot_is_quitting() {
  return robot.webots_exit;
}

void robot_toggle_remote(WbDevice *d, WbRequest *r) {
  if (robot.battery_sampling_period != 0)
    robot.battery_value = -1;

  if (wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL)
    request_write_uchar(r, C_ROBOT_REMOTE_ON);
  else if (wb_robot_get_mode() == WB_MODE_SIMULATION)
    // in that case the request is not sent to webots
    // do it in the next robot_write_request
    robot.send_remote_mode_off = true;
}

void robot_console_print(const char *text, int stream) {
  const int n = strlen(text) + 1;
  robot.console_text = malloc(n);
  memcpy(robot.console_text, text, n);
  robot.console_stream = stream;
  if (wb_robot_step(0) == -1) {
    robot_quit();
    exit(EXIT_SUCCESS);
  }
}

bool robot_is_immediate_message() {
  return robot.is_immediate_message;
}

// Public functions available from the robot API

// multi-thread API

void wb_robot_task_new(void (*task)(void *), void *param) {  // create a task
#ifdef _WIN32
  DWORD thread_id;
  HANDLE thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)task, param, 0, &thread_id);
  if (!thread_handle) {
    fprintf(stderr, "Error: %s() failed to create new thread.\n", __FUNCTION__);
    exit(EXIT_FAILURE);
  }
#else
  pthread_t thread;
  pthread_create(&thread, NULL, (void *(*)(void *))task, param);
#endif
  // cppcheck-suppress resourceLeak ; for thread_handle (which we don't need anymore)
}

WbMutexRef wb_robot_mutex_new() {
#ifdef _WIN32
  HANDLE m = CreateMutex(NULL, false, NULL);
#else
  pthread_mutex_t *m = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(m, NULL);
#endif
  // cppcheck-suppress resourceLeak
  return (WbMutexRef)m;
}

void wb_robot_mutex_lock(WbMutexRef m) {
// printf("lock\n");
#ifdef _WIN32
  WaitForSingleObject((HANDLE)m, INFINITE);
#else
  pthread_mutex_lock((pthread_mutex_t *)m);
#endif
}

void wb_robot_mutex_unlock(WbMutexRef m) {
// printf("unlock\n");
#ifdef _WIN32
  ReleaseMutex((HANDLE)m);
#else
  pthread_mutex_unlock((pthread_mutex_t *)m);
#endif
}

void wb_robot_mutex_delete(WbMutexRef m) {
#ifdef _WIN32
  CloseHandle((HANDLE)m);
#else
  pthread_mutex_destroy((pthread_mutex_t *)m);
  free(m);
#endif
}

// end of multi-task API

WbNodeType wb_robot_get_type() {
  return robot.device[0]->node;
}

void wb_robot_set_mode(WbRobotMode mode, const char *arg) {
  if (mode != WB_MODE_SIMULATION && mode != WB_MODE_REMOTE_CONTROL) {
    fprintf(stderr, "Error: %s() cannot set mode to %d.\n", __FUNCTION__, mode);
    return;
  }
  if (robot.mode == WB_MODE_REMOTE_CONTROL && mode == WB_MODE_SIMULATION && remote_control_is_initialized()) {
    remote_control_stop();  // deactivate the remote control
    robot.toggle_remote_first_step = true;
  } else if (robot.mode == WB_MODE_SIMULATION && mode == WB_MODE_REMOTE_CONTROL &&
             remote_control_is_initialized()) {  // activate the remote control
    if (remote_control_start(arg)) {
      // remote control
      robot.mode = mode;
      robot.toggle_remote_first_step = true;
      return;
    } else
      fprintf(stderr, "Error: %s(): starting the remote control library (wbr_start) failed\n", __FUNCTION__);
  }
  robot.mode = WB_MODE_SIMULATION;
}

const char *wb_robot_get_model() {
  return robot.model;
}

const char *wb_robot_get_custom_data() {
  return robot.custom_data;
}

void wb_robot_set_custom_data(const char *data) {
  free(robot.custom_data);
  const int size = strlen(data) + 1;
  robot.custom_data = malloc(size);
  memcpy(robot.custom_data, data, size);
  robot.dataNeedToWriteRequest = true;
}

const char *wb_robot_get_data() {
  return wb_robot_get_custom_data();
}

void wb_robot_set_data(const char *data) {
  wb_robot_set_custom_data(data);
}

int wb_robot_get_number_of_devices() {
  return robot.n_device - 1;  // the first item is the robot and not a device
}

WbDeviceTag wb_robot_get_device_by_index(int index) {
  if (index >= 0 && index < wb_robot_get_number_of_devices())
    return (WbDeviceTag)index + 1;  // the first item is the robot and not a device
  else {
    fprintf(stderr, "Error: The index of %s() is out of the bounds.\n", __FUNCTION__);
    return 0;
  }
}

WbDeviceTag wb_robot_get_device(const char *name) {
  if (!name || !name[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty argument.\n", __FUNCTION__);
    return 0;
  }

  if (!robot_init_was_done) {
    // we need to redirect the streams to make this message appear in the console
    wb_robot_init();
    robot_abort("wb_robot_init() must be called before any other Webots function.\n");
  }

  WbDeviceTag tag;
  for (tag = 0; tag < robot.n_device; tag++) {
    if (strcmp(robot.device[tag]->name, name) == 0)
      return tag;
  }

  fprintf(stderr, "Warning: \"%s\" device not found.\n", name);
  return 0;  // error: device not found (can not be the root device)
}

const char *wb_robot_get_name() {
  return robot.device[0]->name;
}

void wb_robot_battery_sensor_enable(int sampling_period) {
  robot_mutex_lock_step();
  robot.battery_value = -1.0;  // need to enable or disable
  robot.battery_sampling_period = sampling_period;
  robot_mutex_unlock_step();
}

void wb_robot_battery_sensor_disable() {
  wb_robot_battery_sensor_enable(0);
}

double wb_robot_battery_sensor_get_value() {
  if (robot.battery_sampling_period <= 0)
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_robot_battery_sensor_enable().\n", __FUNCTION__);
  double result;
  robot_mutex_lock_step();
  result = robot.battery_value;
  robot_mutex_unlock_step();
  return result;
}

int wb_robot_battery_sensor_get_sampling_period() {
  int sampling_period = 0;
  robot_mutex_lock_step();
  sampling_period = robot.battery_sampling_period;
  robot_mutex_unlock_step();
  return sampling_period;
}

void wbr_robot_battery_sensor_set_value(double value) {
  if (value < 0)
    fprintf(stderr, "Error: %s() received negative value, new value ignored.\n", __FUNCTION__);
  else
    robot.battery_value = value;
}

int wb_robot_step(int duration) {
  if (!robot.client_exit)
    html_robot_window_step(duration);

  robot_mutex_lock_step();
  // transfer state to/from remote (but not when client exit)
  if (robot.toggle_remote_first_step && !robot.client_exit) {
    robot.toggle_remote_first_step = false;
    WbRequest *req = generate_request(0, true);
    if (wb_robot_get_mode() == WB_MODE_SIMULATION) {  // remote desactivated
      // nothing to do since the display is still simulated in remote mode.
      // remote_control_handle_messages(r);
    } else if (wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL) {  // remote activated
      scheduler_send_request(req);
      robot_read_data();
    }
    request_delete(req);
  }

  current_step_duration = duration;

  if (robot.webots_exit == WEBOTS_EXIT_NOW) {
    robot_quit();
    robot_mutex_unlock_step();
    exit(EXIT_SUCCESS);
  } else if (robot.webots_exit == WEBOTS_EXIT_LATER) {
    robot.webots_exit = WEBOTS_EXIT_NOW;
    robot_mutex_unlock_step();
    return -1;
  }

  robot_mutex_unlock_step();
  robot_window_write_actuators();
  robot_window_pre_update_gui();
  robot_mutex_lock_step();

  robot_step_begin(duration);
  int e = robot_step_end();

  if (e != -1 && wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL && remote_control_has_failed())
    wb_robot_set_mode(0, NULL);

  robot_mutex_unlock_step();
  robot_window_read_sensors();

  return e;
}

WbUserInputEvent wb_robot_wait_for_user_input_event(WbUserInputEvent event_type, int timeout) {
  // check that the devices associated with the event type are enabled
  bool valid = event_type == WB_EVENT_NO_EVENT;
  if (event_type & (WB_EVENT_MOUSE_CLICK | WB_EVENT_MOUSE_MOVE)) {
    if (wb_mouse_get_sampling_period() <= 0)
      fprintf(stderr,
              "Error: %s() called with an event type including the mouse, but the mouse is disabled, please enable it with "
              "wb_mouse_enable().\n",
              __FUNCTION__);
    else
      valid = true;
  }
  if (event_type & WB_EVENT_KEYBOARD) {
    if (wb_keyboard_get_sampling_period() <= 0)
      fprintf(stderr,
              "Error: %s() called with an event type including the keyboard, but the keyboard is disabled, please enable it "
              "with wb_keyboard_enable().\n",
              __FUNCTION__);
    else
      valid = true;
  }
  if (event_type & (WB_EVENT_JOYSTICK_BUTTON | WB_EVENT_JOYSTICK_AXIS | WB_EVENT_JOYSTICK_POV)) {
    if (wb_joystick_get_sampling_period() <= 0)
      fprintf(stderr,
              "Error: %s() called with an event type including a joystick, but no joystick is enabled, please enable it with "
              "wb_joystick_enable().\n",
              __FUNCTION__);
    else
      valid = true;
  }

  if (!valid)
    return WB_EVENT_NO_EVENT;

  robot_mutex_lock_step();
  robot.is_waiting_for_user_input_event = true;
  robot.user_input_event_type = event_type;
  robot.user_input_event_timeout = timeout;
  wb_robot_flush_unlocked();
  while (robot.is_waiting_for_user_input_event && !robot_is_quitting())
    robot_read_data();

  if (robot.webots_exit == WEBOTS_EXIT_NOW) {
    robot_quit();
    robot_mutex_unlock_step();
    exit(EXIT_SUCCESS);
  }

  if (robot.webots_exit == WEBOTS_EXIT_LATER) {
    robot.webots_exit = WEBOTS_EXIT_NOW;
    robot_mutex_unlock_step();
    return WB_EVENT_QUIT;
  }

  robot_mutex_unlock_step();
  return robot.user_input_event_type;
}

void wb_robot_flush_unlocked() {
  if (robot.webots_exit == WEBOTS_EXIT_NOW) {
    robot_quit();
    robot_mutex_unlock_step();
    exit(EXIT_SUCCESS);
  }
  if (robot.webots_exit == WEBOTS_EXIT_LATER)
    return;
  robot.is_immediate_message = true;
  robot_send_request(0);
  robot_read_data();
  if (robot.webots_exit == WEBOTS_EXIT_NOW)
    robot.webots_exit = WEBOTS_EXIT_LATER;
  robot.is_immediate_message = false;
}

int wb_robot_init_msvc() {
  return wb_robot_init();
}

static void wb_robot_cleanup_shm() {
  WbDeviceTag tag;
  for (tag = 1; tag < robot.n_device; tag++) {
    WbDevice *d = robot.device[tag];
    if (d->node != WB_NODE_CAMERA)
      continue;
    wb_device_cleanup(d);
    robot.device[tag] = NULL;
  }
}

int wb_robot_init() {  // API initialization
// do not use any buffer for the standard streams
#ifdef _WIN32  // the line buffered option doesn't to work under Windows, so use unbuffered streams
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
#else
  setvbuf(stdout, NULL, _IOLBF, 4096);
  setvbuf(stderr, NULL, _IOLBF, 4096);
#endif
  // flush stdout and stderr to display messages printed before the robot initialization in the Webots console
  fflush(stdout);
  fflush(stderr);
  static bool already_done = false;
  if (already_done)
    return true;
  setlocale(LC_NUMERIC, "C");
  // the robot.configuration points to a data structure is made up of the following:
  // one uint8 saying if the robot is synchronized (1) or not (0)
  // one uint8 giving the number of devices n
  // n \0-terminated strings giving the names of the devices 0 .. n-1

  signal(SIGINT, quit_controller);  // this signal is working on Windows when Ctrl+C from cmd.exe.
#ifndef _WIN32
  signal(SIGTERM, quit_controller);
  signal(SIGQUIT, quit_controller);
  signal(SIGHUP, quit_controller);
#endif

  robot.configure = 0;
  robot.real_robot_cleanup = NULL;
  robot.is_supervisor = false;
  robot.synchronization = 0;
  robot.mode = 0;  // simulation
  robot.client_exit = false;
  robot.webots_exit = WEBOTS_EXIT_FALSE;
  robot.battery_value = NAN;
  robot.battery_sampling_period = 0;  // initially disabled
  robot.console_text = NULL;
  robot.urdf = NULL;
  robot.urdf_prefix = NULL;
  robot.need_urdf = false;
  robot.pin = -1;
  robot.is_waiting_for_user_input_event = false;
  robot.dataNeedToWriteRequest = false;
  wb_keyboard_init();
  wb_joystick_init();
  wb_mouse_init();

  const char *WEBOTS_SERVER = getenv("WEBOTS_SERVER");
  char *pipe;
  int success = 0;
  if (WEBOTS_SERVER && WEBOTS_SERVER[0]) {
    pipe = strdup(WEBOTS_SERVER);
    success = scheduler_init(pipe);
  } else {
    pipe = NULL;
    int trial = 0;
    while (!should_abort_simulation_waiting) {
      trial++;
      const char *WEBOTS_TMP_PATH = wbu_system_webots_tmp_path(true);
      char retry[256];
      snprintf(retry, sizeof(retry), "Retrying in %d second%s.", trial, trial > 1 ? "s" : "");
      if (!WEBOTS_TMP_PATH) {
        fprintf(stderr, "Webots doesn't seems to be ready yet: (retry count %d)\n", trial);
        sleep(1);
      } else {
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "%s/WEBOTS_SERVER", WEBOTS_TMP_PATH);
        FILE *fd = fopen(buffer, "r");
        if (fd) {
          if (!fscanf(fd, "%1023s", buffer))
            fprintf(stderr, "Cannot read %s/WEBOTS_SERVER content. %s\n", WEBOTS_TMP_PATH, retry);
          else {
            success = scheduler_init(buffer);
            if (success) {
              pipe = strdup(buffer);
              break;
            } else
              fprintf(stderr, "Cannot open %s. %s\nDelete %s to clear this warning.\n", buffer, retry, WEBOTS_TMP_PATH);
          }
          fclose(fd);
        } else
          fprintf(stderr, "Cannot open file: %s (retry count %d)\n", buffer, trial);
        sleep(1);
      }
    }
  }
  if (!success) {
    if (!pipe)
      fprintf(stderr, "Cannot connect to Webots: no valid pipe found.\n");
    free(pipe);
    exit(EXIT_FAILURE);
  }
  free(pipe);

  // robot device
  robot.n_device = 1;
  robot.device = malloc(sizeof(WbDevice *));
  robot.device[0] = malloc(sizeof(WbDevice));
  robot.device[0]->name = NULL;
  robot.device[0]->node = WB_NODE_ROBOT;
  robot.device[0]->read_answer = robot_read_answer;
  robot.device[0]->write_request = robot_write_request;
  robot.device[0]->cleanup = NULL;
  robot_init_was_done = true;
  robot_step_mutex = wb_robot_mutex_new();
  robot.device[0]->toggle_remote = robot_toggle_remote;
  robot.toggle_remote_first_step = false;
  robot.send_remote_mode_off = false;
  robot.show_window = false;
  robot.update_window = false;
  robot.has_html_robot_window = false;
  robot.wwi_message_to_send = NULL;
  robot.wwi_message_to_send_size = 0;
  robot.wwi_message_received = NULL;
  robot.wwi_message_received_size = 0;
  robot.simulation_mode = -1;

  // receive a configure message for the robot and devices
  // need to check return value to avoid crash when the
  // user reverts before having run the simulation
  if (wb_robot_step(0) == -1) {
    robot_quit();
    exit(EXIT_SUCCESS);
  }

  init_remote_control_library();

  // show the robot window as early as possible if required
  if (robot.show_window && !robot.has_html_robot_window) {
    init_robot_window_library();
    robot_window_show();
    robot.show_window = false;
  }

  robot.configure = 0;

  atexit(wb_robot_cleanup_shm);

  already_done = true;

  html_robot_window_init();
  return true;
}

WbRobotMode wb_robot_get_mode() {
  return robot.mode;
}

double wb_robot_get_basic_time_step() {
  return robot.basic_time_step;
}

double wb_robot_get_time() {
  return simulation_time;
}

bool wb_robot_get_synchronization() {
  return robot.synchronization;
}

bool wb_robot_get_supervisor() {
  return robot.is_supervisor;
}

int wb_robot_get_step_duration() {
  return current_step_duration;
}

const char *wb_robot_get_project_path() {
  return robot.project_path;
}

const char *wb_robot_get_world_path() {
  return robot.world_path;
}

const char *wb_robot_get_controller_name() {
  return robot.controller_name;
}

void wb_robot_pin_to_static_environment(bool pin) {
  robot.pin = pin ? 1 : 0;
}

void wb_robot_wwi_send(const char *data, int size) {
  robot_mutex_lock_step();
  robot.wwi_message_to_send_size = size;
  robot.wwi_message_to_send = data;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

const char *wb_robot_wwi_receive(int *size) {
  if (robot.wwi_message_received_size) {
    if (size)
      *size = robot.wwi_message_received_size;
    robot.wwi_message_received_size = 0;
    return robot.wwi_message_received;
  } else {
    if (size)
      *size = 0;
    return NULL;
  }
}

WbSimulationMode robot_get_simulation_mode() {
  return robot.simulation_mode;
}

void robot_set_simulation_mode(WbSimulationMode mode) {
  robot.simulation_mode = mode;
}

const char *wb_robot_get_urdf(const char *prefix) {
  robot_mutex_lock_step();

  robot.need_urdf = true;
  free(robot.urdf_prefix);
  robot.urdf_prefix = malloc(strlen(prefix) + 1);
  strcpy(robot.urdf_prefix, prefix);

  wb_robot_flush_unlocked();
  robot.need_urdf = false;

  robot_mutex_unlock_step();
  return robot.urdf;
}
