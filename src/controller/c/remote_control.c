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

#include "remote_control_private.h"

#include "abstract_camera.h"
#include "display_private.h"
#include "dynamic_library.h"
#include "keyboard_private.h"
#include "messages.h"
#include "robot_private.h"

#include <webots/display.h>  // used for WB_IMAGE_RGB constant
#include <webots/remote_control.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool (*_wbr_init)(WbrInterface *) = NULL;
static void (*_wbr_cleanup)() = NULL;

static void handleMessage(WbRequest *r, WbDeviceTag tag, WbNodeType type);
static void handleRobotMessage(WbRequest *r, unsigned char c);
static void handleDisplayMessage(WbRequest *r, WbDeviceTag tag, unsigned char c);

static WbrInterface remoteInterface = {};
static DYNAMIC_LIBRARY_HANDLE library_handle = NULL;
static bool initialized = false;

static inline void foo(int useless, ...) {
}
// do-while(true): C trick to make a statement.
// foo(0, __VA_ARGS__): force arg evaluation for side effect.
#define CALL_INTERFACE_FUNCTION(f, ...)                \
  do {                                                 \
    if (remoteInterface.f == NULL) {                   \
      foo(0, __VA_ARGS__);                             \
      fprintf(stderr, #f " was called but not set\n"); \
    } else                                             \
      remoteInterface.f(__VA_ARGS__);                  \
  } while (0)

#if defined(__OPTIMIZE__) && __GNUC__ <= 4 && __GNUC_MINOR__ < 5
// #pragma GCC diagnostic push
// TODO gcc 4.4.3 emit a wrong warning... strict-aliasing should not be break...
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

static bool check_remote_interface(void) {
  void (**ptr)() = (void *)&remoteInterface.mandatory;
  int i;

  for (i = 0; i < sizeof(remoteInterface.mandatory) / sizeof(void *); i++) {
    // cppcheck-suppress objectIndex
    if (ptr[i] == NULL)
      return false;
  }
  return true;
}

void remote_control_init(const char *library_name) {
  if (initialized) {
    fprintf(stderr, "Error: %s remote control library already initialized\n", library_name);
    return;
  }

  initialized = false;

  // copy name to libname
  if (!library_name || strlen(library_name) < 1) {
    fprintf(stderr, "Error: invalid remote control library name\n");
    return;
  }

  // load the library
  library_handle = dynamic_library_init(library_name);
  if (library_handle == NULL) {
    fprintf(stderr, "Error: %s remote control library initialisation failed\n", library_name);
    remote_control_cleanup();
    return;
  }

  // load the required functions
  _wbr_init = dynamic_library_get_symbol(library_handle, "wbr_init");
  _wbr_cleanup = dynamic_library_get_symbol(library_handle, "wbr_cleanup");

  // check the existence of the required functions
  if (!_wbr_init || !_wbr_cleanup) {
    fprintf(stderr, "Error: %s remote control library entry points badly defined\n", library_name);
    remote_control_cleanup();
    return;
  }

  if (!_wbr_init(&remoteInterface)) {
    fprintf(stderr, "Error: %s remote control library _wbr_init failed\n", library_name);
    remote_control_cleanup();
    return;
  }

  if (!check_remote_interface()) {
    fprintf(stderr, "Error: a mandatory function was not set in the WbrInterface of the %s remote control library\n",
            library_name);
    remote_control_cleanup();
    return;
  }

  // ok
  initialized = true;
}

bool remote_control_is_initialized() {
  return initialized;
}

bool remote_control_is_function_defined(const char *function_name) {
  return initialized && dynamic_library_get_symbol(library_handle, function_name);
}

void remote_control_cleanup() {
  if (_wbr_cleanup)
    _wbr_cleanup();

  dynamic_library_cleanup(library_handle);
  library_handle = NULL;

  _wbr_init = NULL;
  _wbr_cleanup = NULL;

  memset(&remoteInterface, 1, sizeof(remoteInterface));
  initialized = false;
}

bool remote_control_start(const char *args) {
  return remoteInterface.mandatory.wbr_start(args);
}

bool remote_control_has_failed() {
  return remoteInterface.mandatory.wbr_has_failed();
}

void remote_control_stop() {
  remoteInterface.mandatory.wbr_stop();
}

void remote_control_stop_actuators() {
  remoteInterface.mandatory.wbr_stop_actuators();
}

void remote_control_handle_one_message(WbRequest *r, WbDeviceTag tag) {
  WbNodeType type = robot_get_device_type(tag);
  ROBOT_ASSERT(type != WB_NODE_NO_NODE);
  handleMessage(r, tag, type);
}

WbRequest *remote_control_handle_messages(WbRequest *r) {
  WbRequest *copy = request_new_empty();

  // skip request size
  r->pointer = sizeof(int);

  request_write_uint32(copy, request_read_uint32(r));

  while (request_is_over(r) == 0) {
    WbDeviceTag tag = request_read_tag(r);
    WbNodeType type = robot_get_device_type(tag);
    ROBOT_ASSERT(type != WB_NODE_NO_NODE);

    int size = request_read_int32(r);
    int end = request_get_position(r) + size;
    ROBOT_ASSERT(end <= request_get_size(r));

    // copy the request for all device types we still want to send to webots.
    bool robot_wwi_message = false;
    if (tag == 0) {
      if (request_read_uchar(r) == C_ROBOT_WWI_MESSAGE)
        robot_wwi_message = true;
      r->pointer -= sizeof(unsigned char);  // rewind
    }
    if (robot_wwi_message || type == WB_NODE_DISPLAY || type == WB_NODE_EMITTER || type == WB_NODE_RECEIVER) {
      request_write_tag(copy, tag);
      request_write_int32(copy, size);
      request_write_data(copy, request_read_data(r, size), size);
      request_set_position(r, request_get_position(r) - size);
    }

    do  // handle all requests for this device
      handleMessage(r, tag, type);
    while (request_get_position(r) < end);
  }
  request_write_size(copy);
  return copy;
}

void remote_control_step(int duration) {
  remoteInterface.mandatory.wbr_robot_step(duration);
}

#define REQUEST_ASSERT(cond, tag, type, c)                                                                        \
  {                                                                                                               \
    if (!(cond))                                                                                                  \
      robot_abort("%s:%d: Error bad request : tag %d, type %d, command %d \n", __FILE__, __LINE__, tag, type, c); \
  }

static void handleMessage(WbRequest *r, WbDeviceTag tag, WbNodeType type) {
  // motor is an exception because the command is sent differently
  if (type == WB_NODE_ROTATIONAL_MOTOR || type == WB_NODE_LINEAR_MOTOR) {
    int state = request_read_uint16(r);

    switch (state) {
      case C_MOTOR_SET_POSITION:
        CALL_INTERFACE_FUNCTION(wbr_motor_set_position, tag, request_read_double(r));
        break;
      case C_MOTOR_SET_VELOCITY:
        CALL_INTERFACE_FUNCTION(wbr_motor_set_velocity, tag, request_read_double(r));
        break;
      case C_MOTOR_SET_ACCELERATION:
        CALL_INTERFACE_FUNCTION(wbr_motor_set_acceleration, tag, request_read_double(r));
        break;
      case C_MOTOR_SET_FORCE: {
        if (type == WB_NODE_LINEAR_MOTOR)
          CALL_INTERFACE_FUNCTION(wbr_motor_set_force, tag, request_read_double(r));
        else
          CALL_INTERFACE_FUNCTION(wbr_motor_set_torque, tag, request_read_double(r));
        break;
      }
      case C_MOTOR_SET_AVAILABLE_FORCE: {
        if (type == WB_NODE_LINEAR_MOTOR)
          CALL_INTERFACE_FUNCTION(wbr_motor_set_available_force, tag, request_read_double(r));
        else
          CALL_INTERFACE_FUNCTION(wbr_motor_set_available_torque, tag, request_read_double(r));
        break;
      }
      case C_MOTOR_SET_CONTROL_PID: {
        const double p = request_read_double(r);
        const double i = request_read_double(r);
        const double d = request_read_double(r);
        CALL_INTERFACE_FUNCTION(wbr_motor_set_control_pid, tag, p, i, d);
        break;
      }
      case C_MOTOR_FEEDBACK: {
        if (type == WB_NODE_LINEAR_MOTOR)
          CALL_INTERFACE_FUNCTION(wbr_motor_set_force_sampling_period, tag, request_read_uint16(r));
        else
          CALL_INTERFACE_FUNCTION(wbr_motor_set_torque_sampling_period, tag, request_read_uint16(r));
        break;
      }
      case C_MOTOR_GET_ASSOCIATED_DEVICE:
        request_read_uint16(r);  // Do nothing.
        break;
      default:
        ROBOT_ASSERT(0);
    }
    return;
  }

  unsigned char c = request_read_uchar(r);
  int size;
  switch (type) {
    case WB_NODE_EMITTER:
    case WB_NODE_RECEIVER:
      // dealt in Webots only
      break;
    case WB_NODE_ACCELEROMETER:
    case WB_NODE_CAMERA:
    case WB_NODE_COMPASS:
    case WB_NODE_DISTANCE_SENSOR:
    case WB_NODE_GPS:
    case WB_NODE_GYRO:
    case WB_NODE_LIDAR:
    case WB_NODE_LIGHT_SENSOR:
    case WB_NODE_MICROPHONE:
    case WB_NODE_POSITION_SENSOR:
    case WB_NODE_RADAR:
    case WB_NODE_RADIO:
    case WB_NODE_TOUCH_SENSOR:
      if (c == C_SET_SAMPLING_PERIOD)
        CALL_INTERFACE_FUNCTION(wbr_set_sampling_period, tag, request_read_uint16(r));
      else {
        // devices that have C_SET_SAMPLING_PERIOD and also other command
        switch (type) {
          case WB_NODE_LIDAR:
            switch (c){
            case C_LIDAR_ENABLE_POINT_CLOUD:
              //What should I do here?
              break;
            case C_LIDAR_SET_FREQUENCY:
              CALL_INTERFACE_FUNCTION(wbr_lidar_set_frequency, tag, request_read_double(r));
              break;
            case C_CAMERA_GET_IMAGE:
              wb_abstract_camera_update_timestamp(robot_get_device_with_node(tag, WB_NODE_LIDAR, true));
              break;
            default:
              REQUEST_ASSERT(0, tag, type, c);
              break;
            }
            break;
          case WB_NODE_CAMERA:
            switch (c) {
              case C_CAMERA_SET_FOV:
                CALL_INTERFACE_FUNCTION(wbr_camera_set_fov, tag, request_read_double(r));
                break;
              case C_CAMERA_SET_EXPOSURE:
                CALL_INTERFACE_FUNCTION(wbr_camera_set_exposure, tag, request_read_double(r));
                break;
              case C_CAMERA_SET_FOCAL:
                CALL_INTERFACE_FUNCTION(wbr_camera_set_focal_distance, tag, request_read_double(r));
                break;
              case C_CAMERA_GET_IMAGE:
                wb_abstract_camera_update_timestamp(robot_get_device_with_node(tag, WB_NODE_CAMERA, true));
                break;
              default:
                REQUEST_ASSERT(0, tag, type, c);
            }
            break;
          case WB_NODE_RADIO:
            ROBOT_ASSERT(!"radio unimplemented");
            break;
          default:
            REQUEST_ASSERT(0, tag, type, c);
        }
      }
      break;
    case WB_NODE_ROBOT:
      handleRobotMessage(r, c);
      break;
    case WB_NODE_CONNECTOR:
      ROBOT_ASSERT(!"connector unimplemented");
      break;
    case WB_NODE_DISPLAY:
      handleDisplayMessage(r, tag, c);
      break;
    case WB_NODE_LED:
      REQUEST_ASSERT(c == C_LED_SET, tag, type, c);
      CALL_INTERFACE_FUNCTION(wbr_led_set, tag, request_read_int32(r));
      break;
    case WB_NODE_PEN:
      if (c == C_PEN_SET_INK_COLOR) {
        unsigned char red = request_read_uchar(r);
        unsigned char green = request_read_uchar(r);
        unsigned char blue = request_read_uchar(r);
        double density = request_read_uchar(r) / 255.0;
        unsigned int color = red << 16 | green << 8 | blue;
        CALL_INTERFACE_FUNCTION(wbr_pen_set_ink_color, tag, color, density);
      } else {
        REQUEST_ASSERT(c == C_PEN_WRITE || c == C_PEN_DONT_WRITE, tag, type, c);
        CALL_INTERFACE_FUNCTION(wbr_pen_write, tag, c == C_PEN_WRITE);
      }
      break;
    case WB_NODE_SPEAKER:
      REQUEST_ASSERT(c == C_SPEAKER_PLAY_SOUND, tag, type, c);
      size = request_read_int32(r);
      CALL_INTERFACE_FUNCTION(wbr_speaker_emit_sample, tag, request_read_data(r, size), size);
      break;
    case WB_NODE_ROTATIONAL_MOTOR:
    case WB_NODE_LINEAR_MOTOR:
      REQUEST_ASSERT(!"Impossible fallback", tag, type, c);
    default:
      REQUEST_ASSERT(0, tag, type, c);
  }
}

static void handleRobotMessage(WbRequest *r, unsigned char c) {
  switch (c) {
    case C_ROBOT_WWI_MESSAGE: {
      int size = request_read_int32(r);
      request_read_data(r, size);  // skip it
      break;
    }
    default:
      REQUEST_ASSERT(0, 0, -1, c);
  }
}

static void handleDisplayMessage(WbRequest *r, WbDeviceTag tag, unsigned char c) {
  if (c == 0)
    return;
  switch (c) {
    case C_DISPLAY_SET_COLOR:
      CALL_INTERFACE_FUNCTION(wbr_display_set_color, tag, request_read_int32(r));
      break;
    case C_DISPLAY_SET_ALPHA:
      CALL_INTERFACE_FUNCTION(wbr_display_set_alpha, tag, request_read_uchar(r) / 255.0);
      break;
    case C_DISPLAY_SET_OPACITY:
      CALL_INTERFACE_FUNCTION(wbr_display_set_opacity, tag, request_read_uchar(r) / 255.0);
      break;
    default:
      if (c & 0x20) {  // draw message
        int size = request_read_int16(r);
        int *x = (int *)request_read_data(r, size * sizeof(int));
        int *y = (int *)request_read_data(r, size * sizeof(int));
        switch (c) {
          case C_DISPLAY_DRAW_PIXEL:
            ROBOT_ASSERT(size == 1);
            CALL_INTERFACE_FUNCTION(wbr_display_draw_pixel, tag, x[0], y[0]);
            break;
          case C_DISPLAY_DRAW_LINE:
            ROBOT_ASSERT(size == 2);
            CALL_INTERFACE_FUNCTION(wbr_display_draw_line, tag, x[0], y[0], x[1], y[1]);
            break;
          case C_DISPLAY_DRAW_TEXT: {
            ROBOT_ASSERT(size == 1);
            char *str = request_read_string(r);
            CALL_INTERFACE_FUNCTION(wbr_display_draw_text, tag, str, x[0], y[0]);
            free(str);
            break;
          }
          case C_DISPLAY_DRAW_RECTANGLE:
            ROBOT_ASSERT(size == 2);
            if (request_read_uchar(r))
              CALL_INTERFACE_FUNCTION(wbr_display_fill_rectangle, tag, x[0], y[0], x[1], y[1]);
            else
              CALL_INTERFACE_FUNCTION(wbr_display_draw_rectangle, tag, x[0], y[0], x[1], y[1]);
            break;
          case C_DISPLAY_DRAW_OVAL:
            ROBOT_ASSERT(size == 2);
            if (request_read_uchar(r))
              CALL_INTERFACE_FUNCTION(wbr_display_fill_oval, tag, x[0], y[0], x[1], y[1]);
            else
              CALL_INTERFACE_FUNCTION(wbr_display_draw_oval, tag, x[0], y[0], x[1], y[1]);
            break;
          case C_DISPLAY_DRAW_POLYGON:
            if (request_read_uchar(r))
              CALL_INTERFACE_FUNCTION(wbr_display_fill_polygon, tag, x, y, size);
            else
              CALL_INTERFACE_FUNCTION(wbr_display_draw_polygon, tag, x, y, size);
            break;
          default:
            REQUEST_ASSERT(0, -1, WB_NODE_DISPLAY, c);
        }
      } else if (c & 0x40) {  // Image message
        int id = request_read_int32(r);
        int x, y, width, height;
        switch (c) {
          case C_DISPLAY_IMAGE_COPY:
            x = request_read_int16(r);
            y = request_read_int16(r);
            width = request_read_int16(r);
            height = request_read_int16(r);
            CALL_INTERFACE_FUNCTION(wbr_display_image_copy, tag, id, x, y, width, height);
            break;
          case C_DISPLAY_IMAGE_PASTE:
            x = request_read_int16(r);
            y = request_read_int16(r);
            CALL_INTERFACE_FUNCTION(wbr_display_image_paste, tag, id, x, y);
            break;
          case C_DISPLAY_IMAGE_DELETE:
            CALL_INTERFACE_FUNCTION(wbr_display_image_delete, tag, id);
            break;
          case C_DISPLAY_IMAGE_LOAD:
            width = request_read_int16(r);
            height = request_read_int16(r);
            x = request_read_uchar(r);  // format
            CALL_INTERFACE_FUNCTION(wbr_display_image_new, tag, id, width, height,
                                    request_read_data(r, width * height * display_get_channel_number(x)), x);
            break;
          case C_DISPLAY_IMAGE_SAVE:
            // CALL_INTERFACE_FUNCTION(wbr_display_image_save, tag, id);
            break;
          case C_DISPLAY_IMAGE_GET_ALL: {
            width = request_read_int16(r);
            height = request_read_int16(r);
            unsigned char *im = request_read_data(r, 4 * width * height);
            CALL_INTERFACE_FUNCTION(wbr_display_image_new, tag, -1, width, height, im, WB_IMAGE_RGBA);
            CALL_INTERFACE_FUNCTION(wbr_display_image_paste, tag, -1, 0, 0);
            CALL_INTERFACE_FUNCTION(wbr_display_image_delete, tag, -1);
            int i;
            int number = id;  // in this message the first "id" is the number of imageRef
            for (i = 0; i < number; i++) {
              id = request_read_int32(r);
              width = request_read_int16(r);
              height = request_read_int16(r);
              im = request_read_data(r, 4 * width * height);
              CALL_INTERFACE_FUNCTION(wbr_display_image_new, tag, id, width, height, im, WB_IMAGE_RGBA);
            }
            CALL_INTERFACE_FUNCTION(wbr_display_set_color, tag, request_read_int32(r));
            CALL_INTERFACE_FUNCTION(wbr_display_set_alpha, tag, request_read_uchar(r) / 255.0);
            CALL_INTERFACE_FUNCTION(wbr_display_set_opacity, tag, request_read_uchar(r) / 255.0);
          } break;
          default:
            REQUEST_ASSERT(0, -1, WB_NODE_DISPLAY, c);
        }
      } else
        REQUEST_ASSERT(0, -1, WB_NODE_DISPLAY, c);
  }
}

void *wb_remote_control_custom_function(void *arg) {
  if (remoteInterface.wbr_custom_function == NULL)
    return NULL;
  return remoteInterface.wbr_custom_function(arg);
}
