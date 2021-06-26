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

#ifndef DEVICE_PRIVATE_H
#define DEVICE_PRIVATE_H

#include <webots/nodes.h>
#include "request.h"

#ifndef NAN
#define NAN (0.0 / 0.0)
#endif

typedef struct _WbDevice WbDevice;

struct _WbDevice {
  WbNodeType node;
  char *name;
  char *model;
  void *pdata;
  void (*read_answer)(WbDevice *, WbRequest *);
  void (*write_request)(WbDevice *, WbRequest *);
  void (*cleanup)(WbDevice *);
  void (*toggle_remote)(WbDevice *, WbRequest *);
};

void wb_device_cleanup(WbDevice *);
void wb_device_init(WbDevice *);

#endif  // DEVICE_PRIVATE_H
