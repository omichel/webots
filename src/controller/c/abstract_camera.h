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

#ifndef ABSTRACT_CAMERA_PRIVATE_H
#define ABSTRACT_CAMERA_PRIVATE_H

#include <webots/types.h>

#include "device_private.h"
#include "image_private.h"

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
  bool enable;
  int sampling_period;
  unsigned int unique_id;  // camera id
  int width;
  int height;
  double camnear;
  bool spherical;
  double fov;  // in degrees
  int mode;
  void *pdata;
  Image *image;
} AbstractCamera;

void wb_abstract_camera_cleanup(WbDevice *d);

void wb_abstract_camera_new(WbDevice *d, unsigned int id, int w, int h, double fov, double camnear, bool spherical);

void wb_abstract_camera_write_request(WbDevice *d, WbRequest *r);
bool wb_abstract_camera_handle_command(WbDevice *d, WbRequest *r, unsigned char command);

void abstract_camera_toggle_remote(WbDevice *d, WbRequest *r);

void wbr_abstract_camera_set_image(WbDevice *d, const unsigned char *image);
unsigned char *wbr_abstract_camera_get_image_buffer(WbDevice *d);

void wb_abstract_camera_enable(WbDevice *d, int sampling_period);
int wb_abstract_camera_get_sampling_period(WbDevice *d);
int wb_abstract_camera_get_height(WbDevice *d);
int wb_abstract_camera_get_width(WbDevice *d);
double wb_abstract_camera_get_fov(WbDevice *d);
double wb_abstract_camera_get_near(WbDevice *d);

#endif  // ABSTRACT_CAMERA_PRIVATE_H
