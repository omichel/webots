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

// ***************************************************************************
// this file contains the API code for the Pen device
// ***************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <webots/nodes.h>
#include <webots/pen.h>
#include <webots/types.h>
#include "device_private.h"
#include "messages.h"
#include "robot_private.h"

// Static functions

typedef struct {
  bool set_write;
  bool write;
  bool set_color;
  unsigned char red, green, blue;
  double density;
} Pen;

static Pen *pen_create() {
  Pen *pen = malloc(sizeof(Pen));
  pen->set_write = false;
  pen->write = false;
  pen->set_color = false;
  pen->red = pen->green = pen->blue = 0;
  pen->density = 0.0;
  return pen;
}

// Static functions

static Pen *pen_get_struct(WbDeviceTag t) {
  WbDevice *d = robot_get_device_with_node(t, WB_NODE_PEN, true);
  return d ? d->pdata : NULL;
}

static void pen_write_request(WbDevice *d, WbRequest *r) {
  Pen *pen = d->pdata;
  if (pen->set_write) {
    request_write_uchar(r, pen->write ? C_PEN_WRITE : C_PEN_DONT_WRITE);
    pen->set_write = false;
  }
  if (pen->set_color) {
    request_write_uchar(r, C_PEN_SET_INK_COLOR);
    request_write_uchar(r, pen->red);
    request_write_uchar(r, pen->green);
    request_write_uchar(r, pen->blue);
    request_write_double(r, pen->density);
    pen->set_color = false;
  }
}

static void pen_cleanup(WbDevice *d) {
  free(d->pdata);
}

static void pen_toggle_remote(WbDevice *d, WbRequest *r) {
  Pen *pen = d->pdata;
  if (pen->write)
    pen->set_write = true;
  if (pen->red != 0 || pen->green != 0 || pen->blue != 0)
    pen->set_color = true;
}

// Exported functions

void wb_pen_init(WbDevice *d) {
  d->read_answer = NULL;
  d->write_request = pen_write_request;
  d->cleanup = pen_cleanup;
  d->pdata = pen_create();
  d->toggle_remote = pen_toggle_remote;
}

// Public functions (available from the user API)

void wb_pen_write(WbDeviceTag tag, bool write) {
  robot_mutex_lock_step();
  Pen *pen = pen_get_struct(tag);
  if (pen) {
    pen->write = write;
    pen->set_write = true;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
}

void wb_pen_set_ink_color(WbDeviceTag tag, int color, double density) {
  robot_mutex_lock_step();
  Pen *pen = pen_get_struct(tag);
  if (pen) {
    pen->set_color = true;
    pen->red = (color >> 16) & 0xff;
    pen->green = (color >> 8) & 0xff;
    pen->blue = color & 0xff;
    pen->density = density;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
}
