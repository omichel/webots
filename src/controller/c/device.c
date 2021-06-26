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

#include <stdio.h>
#include <stdlib.h>
#include <webots/device.h>
#include <webots/nodes.h>
#include <webots/robot.h>
#include <webots/types.h>
#include "device_private.h"
#include "robot_private.h"

const char *wb_device_get_name(WbDeviceTag dt) {
  return robot_get_device_name(dt);
}

const char *wb_device_get_model(WbDeviceTag dt) {
  return robot_get_device_model(dt);
}

WbNodeType wb_device_get_node_type(WbDeviceTag dt) {
  return robot_get_device_type(dt);
}

void wb_device_cleanup(WbDevice *d) {
  free(d->name);
  if (d->cleanup)
    d->cleanup(d);
  free(d);
}

extern void wb_accelerometer_init(WbDevice *);
extern void wb_brake_init(WbDevice *);
extern void wb_camera_init(WbDevice *);
extern void wb_compass_init(WbDevice *);
extern void wb_connector_init(WbDevice *);
extern void wb_display_init(WbDevice *);
extern void wb_distance_sensor_init(WbDevice *);
extern void wb_emitter_init(WbDevice *);
extern void wb_gps_init(WbDevice *);
extern void wb_gyro_init(WbDevice *);
extern void wb_inertial_unit_init(WbDevice *);
extern void wb_led_init(WbDevice *);
extern void wb_lidar_init(WbDevice *);
extern void wb_light_sensor_init(WbDevice *);
extern void wb_microphone_init(WbDevice *);
extern void wb_motor_init(WbDevice *);
extern void wb_pen_init(WbDevice *);
extern void wb_position_sensor_init(WbDevice *);
extern void wb_touch_sensor_init(WbDevice *);
extern void wb_radar_init(WbDevice *);
extern void wb_radio_init(WbDevice *);
extern void wb_range_finder_init(WbDevice *);
extern void wb_receiver_init(WbDevice *);
extern void wb_skin_init(WbDevice *);
extern void wb_speaker_init(WbDevice *);

void wb_device_init(WbDevice *d) {
  d->toggle_remote = NULL;

  switch (d->node) {
    case WB_NODE_ACCELEROMETER:
      wb_accelerometer_init(d);
      break;
    case WB_NODE_BRAKE:
      wb_brake_init(d);
      break;
    case WB_NODE_CAMERA:
      wb_camera_init(d);
      break;
    case WB_NODE_COMPASS:
      wb_compass_init(d);
      break;
    case WB_NODE_CONNECTOR:
      wb_connector_init(d);
      break;
    case WB_NODE_DISPLAY:
      wb_display_init(d);
      break;
    case WB_NODE_DISTANCE_SENSOR:
      wb_distance_sensor_init(d);
      break;
    case WB_NODE_EMITTER:
      wb_emitter_init(d);
      break;
    case WB_NODE_GPS:
      wb_gps_init(d);
      break;
    case WB_NODE_GYRO:
      wb_gyro_init(d);
      break;
    case WB_NODE_INERTIAL_UNIT:
      wb_inertial_unit_init(d);
      break;
    case WB_NODE_LED:
      wb_led_init(d);
      break;
    case WB_NODE_LIDAR:
      wb_lidar_init(d);
      break;
    case WB_NODE_LIGHT_SENSOR:
      wb_light_sensor_init(d);
      break;
    case WB_NODE_LINEAR_MOTOR:
      wb_motor_init(d);
      break;
    case WB_NODE_MICROPHONE:
      wb_microphone_init(d);
      break;
    case WB_NODE_PEN:
      wb_pen_init(d);
      break;
    case WB_NODE_POSITION_SENSOR:
      wb_position_sensor_init(d);
      break;
    case WB_NODE_ROTATIONAL_MOTOR:
      wb_motor_init(d);
      break;
    case WB_NODE_TOUCH_SENSOR:
      wb_touch_sensor_init(d);
      break;
    case WB_NODE_RADAR:
      wb_radar_init(d);
      break;
    case WB_NODE_RADIO:
      wb_radio_init(d);
      break;
    case WB_NODE_RANGE_FINDER:
      wb_range_finder_init(d);
      break;
    case WB_NODE_RECEIVER:
      wb_receiver_init(d);
      break;
    case WB_NODE_SKIN:
      wb_skin_init(d);
      break;
    case WB_NODE_SPEAKER:
      wb_speaker_init(d);
      break;
    default:
      fprintf(stderr, "%s(): node not handled\n", __FUNCTION__);
      break;
  }
}

// obsolete function kept for backward compatibility

WbNodeType wb_device_get_type(WbDeviceTag dt) {
  return wb_device_get_node_type(dt);
}
