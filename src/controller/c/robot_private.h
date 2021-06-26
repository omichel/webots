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

#ifndef ROBOT_PRIVATE_H
#define ROBOT_PRIVATE_H

#include <webots/supervisor.h>
#include <webots/types.h>
#include "device_private.h"
#include "request.h"
#include "webots/nodes.h"

#ifdef NDEBUG
#define ROBOT_ASSERT(condition) \
  {}
#else
#define ROBOT_ASSERT(condition)                                                     \
  {                                                                                 \
    if (!(condition))                                                               \
      robot_abort("%s:%d: assertion failed: %s\n", __FILE__, __LINE__, #condition); \
  }
#endif

int wb_robot_get_step_duration();
void wb_robot_flush_unlocked();
void robot_write_request(WbDevice *, WbRequest *);
void robot_read_answer(WbDevice *, WbRequest *);
WbDevice *robot_get_device_with_node(WbDeviceTag tag, WbNodeType node, bool warning);
int robot_get_number_of_devices();
WbDeviceTag robot_get_device_tag(WbDevice *);
WbDevice *robot_get_robot_device();
int robot_check_supervisor(const char *func_name);
const char *robot_get_device_name(WbDeviceTag tag);
const char *robot_get_device_model(WbDeviceTag tag);
void robot_mutex_lock_step();
void robot_mutex_unlock_step();
void robot_abort(const char *format, ...);
WbNodeType robot_get_device_type(WbDeviceTag tag);
void robot_toggle_remote(WbDevice *, WbRequest *);
int robot_is_quitting();
void robot_console_print(const char *text, int stream);
WbSimulationMode robot_get_simulation_mode();
void robot_set_simulation_mode(WbSimulationMode mode);
bool robot_is_immediate_message();

#endif  // ROBOT_PRIVATE_H
