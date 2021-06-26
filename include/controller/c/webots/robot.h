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

/**********************************************************************************/
/* Description:  Webots C programming interface for the Robot node                */
/**********************************************************************************/

#ifndef WB_ROBOT_H
#define WB_ROBOT_H

#define WB_USING_C_API
#include "types.h"

#ifdef __CYGWIN__
#include <stdio.h>
#endif

#if defined(__VISUALC__) || defined(_MSC_VER)
#include "stdio.h"
#endif

#include "nodes.h"

#ifdef KROS_COMPILATION
#define main() _kros_main()
#endif

typedef void *WbMutexRef;  // identifier of a mutex

typedef enum { WB_MODE_SIMULATION = 0, WB_MODE_CROSS_COMPILATION, WB_MODE_REMOTE_CONTROL } WbRobotMode;

typedef enum {
  WB_EVENT_QUIT = -1,
  WB_EVENT_NO_EVENT = 0,
  WB_EVENT_MOUSE_CLICK = 1,
  WB_EVENT_MOUSE_MOVE = 2,
  WB_EVENT_KEYBOARD = 4,
  WB_EVENT_JOYSTICK_BUTTON = 8,
  WB_EVENT_JOYSTICK_AXIS = 16,
  WB_EVENT_JOYSTICK_POV = 32
} WbUserInputEvent;

// cart function headers
#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__VISUALC__) && !defined(_MSC_VER)
int wb_robot_init();

/* In the visual studio case, the buffer size of the standard output and
 * the standard error cannot be modified from a dll
 */
#else
int wb_robot_init_msvc();  // internally, this function just calls wb_robot_init()
#define wb_robot_init() (setvbuf(stdout, NULL, _IONBF, 0), setvbuf(stderr, NULL, _IONBF, 0), wb_robot_init_msvc())
#endif

int wb_robot_step(int duration);  // milliseconds

#ifdef __CYGWIN__  // In that case, we need to flush explicitly the stdout/stdin streams otherwise they are buffered
// We cannot call fflush from the libController as libController is compiled with gcc8 and won't flush the stdout/stderr
// of a gcc7 (cygwin) compiled binary. Therefore, we need to perform the fflush in a gcc7 compiled code, e.g., in a macro here.
#define wb_robot_step(d) (fflush(NULL), wb_robot_step(d))
#endif

WbUserInputEvent wb_robot_wait_for_user_input_event(WbUserInputEvent event_type, int timeout);  // milliseconds
void wb_robot_cleanup();
double wb_robot_get_time();
const char *wb_robot_get_urdf(const char *prefix);
const char *wb_robot_get_name();
const char *wb_robot_get_model();
const char *wb_robot_get_custom_data();
void wb_robot_set_custom_data(const char *data);
WbRobotMode wb_robot_get_mode();
void wb_robot_set_mode(WbRobotMode mode, const char *arg);
bool wb_robot_get_synchronization();
bool wb_robot_get_supervisor();
const char *wb_robot_get_project_path();
const char *wb_robot_get_world_path();
double wb_robot_get_basic_time_step();
WbDeviceTag wb_robot_get_device(const char *name);

// Introspection API
int wb_robot_get_number_of_devices();
WbDeviceTag wb_robot_get_device_by_index(int index);
WbNodeType wb_robot_get_type();

// robot battery API
void wb_robot_battery_sensor_enable(int sampling_period);
void wb_robot_battery_sensor_disable();
int wb_robot_battery_sensor_get_sampling_period();
double wb_robot_battery_sensor_get_value();

// robot multi-thread API
#ifndef WB_MATLAB_LOADLIBRARY
void wb_robot_task_new(void (*task)(void *), void *param);  // create a task
WbMutexRef wb_robot_mutex_new();
void wb_robot_mutex_lock(WbMutexRef);
void wb_robot_mutex_unlock(WbMutexRef);
void wb_robot_mutex_delete(WbMutexRef);
#endif

// Motion editor specfic function : Please don't use this function outside qt_utils
// This function doesn't work if the robot window has not been shown at lease once
void wb_robot_pin_to_static_environment(bool pin);

// Deprecated functions
// deprecated since Webots 2018a, please use wb_robot_get_custom_data and
// wb_robot_set_custom_data instead
const char *wb_robot_get_controller_name() WB_DEPRECATED;
const char *wb_robot_get_data() WB_DEPRECATED;
void wb_robot_set_data(const char *data) WB_DEPRECATED;

#ifdef __cplusplus
}
#endif

#endif /* WB_ROBOT_H */
