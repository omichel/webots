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
#include <string.h>
#include <webots/range_finder.h>
#include <webots/robot.h>
#include "abstract_camera.h"
#include "g_image.h"
#include "messages.h"
#include "robot_private.h"

typedef struct {
  double max_range;
} RangeFinder;

static WbDevice *range_finder_get_device(WbDeviceTag t) {
  WbDevice *d = robot_get_device_with_node(t, WB_NODE_RANGE_FINDER, true);
  return d;
}

static AbstractCamera *range_finder_get_abstract_camera_struct(WbDeviceTag t) {
  WbDevice *d = range_finder_get_device(t);
  return d ? d->pdata : NULL;
}

static RangeFinder *range_finder_get_struct(WbDeviceTag t) {
  AbstractCamera *ac = range_finder_get_abstract_camera_struct(t);
  return ac ? ac->pdata : NULL;
}

static void wb_range_finder_cleanup(WbDevice *d) {
  AbstractCamera *ac = d->pdata;
  if (ac == NULL)
    return;
  RangeFinder *rf = ac->pdata;
  if (rf == NULL)
    return;
  free(rf);
  ac->pdata = NULL;
  wb_abstract_camera_cleanup(d);
}

static void wb_range_finder_new(WbDevice *d, unsigned int id, int w, int h, double fov, double camnear, double max_range,
                                bool spherical) {
  RangeFinder *rf;
  wb_range_finder_cleanup(d);
  wb_abstract_camera_new(d, id, w, h, fov, camnear, spherical);

  rf = malloc(sizeof(RangeFinder));
  rf->max_range = max_range;

  AbstractCamera *ac = d->pdata;
  ac->pdata = rf;
}

static void wb_range_finder_write_request(WbDevice *d, WbRequest *r) {
  wb_abstract_camera_write_request(d, r);
}

static void wb_range_finder_read_answer(WbDevice *d, WbRequest *r) {
  unsigned char command = request_read_uchar(r);
  if (wb_abstract_camera_handle_command(d, r, command))
    return;
  unsigned int uid;
  int width, height;
  double fov, camnear, max_range;
  bool spherical;

  AbstractCamera *ac = d->pdata;
  RangeFinder *rf = NULL;

  switch (command) {
    case C_CONFIGURE:
      uid = request_read_uint32(r);
      width = request_read_uint16(r);
      height = request_read_uint16(r);
      fov = request_read_double(r);
      camnear = request_read_double(r);
      spherical = request_read_uchar(r);
      max_range = request_read_double(r);

      // printf("new range_finder %u %d %d %lf %lf %lf %d\n", uid, width, height, fov, camnear, max_range, spherical);
      wb_range_finder_new(d, uid, width, height, fov, camnear, max_range, spherical);
      break;
    case C_CAMERA_RECONFIGURE:
      rf = ac->pdata;
      ac->fov = request_read_double(r);
      ac->camnear = request_read_double(r);
      ac->spherical = request_read_uchar(r);
      rf->max_range = request_read_double(r);
      break;

    default:
      ROBOT_ASSERT(0);
      break;
  }
}

static void range_finder_toggle_remote(WbDevice *d, WbRequest *r) {
  abstract_camera_toggle_remote(d, r);
}

// Protected functions available from other source files

void wb_range_finder_init(WbDevice *d) {
  d->read_answer = wb_range_finder_read_answer;
  d->write_request = wb_range_finder_write_request;
  d->cleanup = wb_range_finder_cleanup;
  d->pdata = NULL;
  d->toggle_remote = range_finder_toggle_remote;
  // g_print("range_finder init done\n");
}

int wb_range_finder_get_unique_id(WbDevice *d) {
  AbstractCamera *ac = d->pdata;
  return ac->unique_id;
}

void wbr_range_finder_set_image(WbDeviceTag t, const unsigned char *image) {
  RangeFinder *rf = range_finder_get_struct(t);
  if (rf)
    wbr_abstract_camera_set_image(range_finder_get_device(t), image);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

unsigned char *wbr_range_finder_get_image_buffer(WbDeviceTag t) {
  RangeFinder *rf = range_finder_get_struct(t);
  if (rf)
    return wbr_abstract_camera_get_image_buffer(range_finder_get_device(t));

  fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return (unsigned char *)"";  // can't return NULL, swig needs the empty string to make python strings
}

// Public functions available from the range_finder API

void wb_range_finder_enable(WbDeviceTag tag, int sampling_period) {
  if (sampling_period < 0) {
    fprintf(stderr, "Error: %s() called with negative sampling period.\n", __FUNCTION__);
    return;
  }

  WbDevice *d = range_finder_get_device(tag);
  if (!d) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    return;
  }

  wb_abstract_camera_enable(d, sampling_period);
}

void wb_range_finder_disable(WbDeviceTag tag) {
  RangeFinder *rf = range_finder_get_struct(tag);
  if (rf)
    wb_range_finder_enable(tag, 0);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

int wb_range_finder_get_sampling_period(WbDeviceTag tag) {
  RangeFinder *rf = range_finder_get_struct(tag);
  if (!rf)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  return wb_abstract_camera_get_sampling_period(range_finder_get_device(tag));
}

int wb_range_finder_get_height(WbDeviceTag tag) {
  RangeFinder *rf = range_finder_get_struct(tag);
  if (!rf)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  return wb_abstract_camera_get_height(range_finder_get_device(tag));
}

int wb_range_finder_get_width(WbDeviceTag tag) {
  RangeFinder *rf = range_finder_get_struct(tag);
  if (!rf)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  return wb_abstract_camera_get_width(range_finder_get_device(tag));
}

double wb_range_finder_get_fov(WbDeviceTag tag) {
  RangeFinder *rf = range_finder_get_struct(tag);
  if (!rf)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  return wb_abstract_camera_get_fov(range_finder_get_device(tag));
}

double wb_range_finder_get_min_range(WbDeviceTag tag) {
  RangeFinder *rf = range_finder_get_struct(tag);
  if (!rf)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  return wb_abstract_camera_get_near(range_finder_get_device(tag));
}

double wb_range_finder_get_max_range(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  RangeFinder *rf = range_finder_get_struct(tag);
  if (rf)
    result = rf->max_range;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

const float *wb_range_finder_get_range_image(WbDeviceTag tag) {
  robot_mutex_lock_step();
  AbstractCamera *ac = range_finder_get_abstract_camera_struct(tag);

  if (!ac) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return NULL;
  }

  if (wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL) {
    robot_mutex_unlock_step();
    return (const float *)(void *)ac->image->data;
  }

  if (ac->sampling_period <= 0)
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_range_finder_enable().\n", __FUNCTION__);
  robot_mutex_unlock_step();

  return (const float *)(void *)ac->image->data;
}

int wb_range_finder_save_image(WbDeviceTag tag, const char *filename, int quality) {
  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'filename' argument.\n", __FUNCTION__);
    return -1;
  }
  if (quality < 1 || quality > 100) {
    fprintf(stderr, "Error: %s() called with invalid 'quality' argument.\n", __FUNCTION__);
    return -1;
  }

  int i, j, x, y, ret, size;
  double v;
  unsigned char g;

  robot_mutex_lock_step();
  AbstractCamera *ac = range_finder_get_abstract_camera_struct(tag);
  RangeFinder *rf = range_finder_get_struct(tag);

  if (!ac) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return -1;
  }

  // make sure image is up to date before saving it
  if (!ac->image->data) {
    robot_mutex_unlock_step();
    return -1;
  }

  GImage img;
  img.width = ac->width;
  img.height = ac->height;
  img.data = NULL;
  img.float_data = NULL;
  size = img.width * img.height;
  if (g_image_get_type(filename) == G_IMAGE_TIFF) {
    fprintf(stderr, "Error: %s(): .tiff image not supported anymore, use .hdr instead.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return -1;
  } else if (g_image_get_type(filename) == G_IMAGE_HDR) {
    img.data_format = G_IMAGE_DATA_FORMAT_F;
    size *= sizeof(float);
    img.float_data = malloc(size);
  } else {
    img.data_format = G_IMAGE_DATA_FORMAT_RGB;
    size *= 3;
    img.data = malloc(size);
  }

  if (!img.data && !img.float_data) {
    fprintf(stderr, "Error: %s(): malloc failed.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return -1;
  }

  for (y = 0; y < ac->height; y++) {
    for (x = 0; x < ac->width; x++) {
      j = y * ac->width + x;
      v = ((float *)(void *)ac->image->data)[j];
      v -= ac->camnear;
      v /= rf->max_range;
      // Bound v to the range 0 <= v <= 1.0
      v = (v > 1.0) ? 1.0 : (v < 0.0) ? 0.0 : v;
      // RGB images have each channel in the range 0 - 255 so we
      // scale up v to that range to get our channel values
      g = (unsigned char)(v * 255.0);
      if (img.data_format == G_IMAGE_DATA_FORMAT_RGB) {
        i = y * ac->width * 3 + 3 * x;
        img.data[i] = g;
        img.data[i + 1] = g;
        img.data[i + 2] = g;
      } else
        img.float_data[j] = v;
    }
  }
  ret = g_image_save(&img, filename, quality);

  if (img.data_format == G_IMAGE_DATA_FORMAT_RGB)
    free(img.data);
  else
    free(img.float_data);

  robot_mutex_unlock_step();
  return ret;
}
