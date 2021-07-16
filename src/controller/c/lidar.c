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
#include <webots/lidar.h>
#include <webots/robot.h>
#include "abstract_camera.h"
#include "g_image.h"
#include "messages.h"
#include "remote_control_private.h"
#include "robot_private.h"

typedef struct {
  double max_range;
  int number_of_layers;
  int horizontal_resolution;
  double frequency;
  double min_frequency;
  double max_frequency;
  double vertical_fov;
  bool point_cloud_enabled;
  bool set_frequency;
  bool set_enable_point_cloud;
  bool set_disable_point_cloud;
} Lidar;

static WbDevice *lidar_get_device(WbDeviceTag t) {
  WbDevice *d = robot_get_device_with_node(t, WB_NODE_LIDAR, true);
  return d;
}

static AbstractCamera *lidar_get_abstract_camera_struct(WbDeviceTag t) {
  WbDevice *d = lidar_get_device(t);
  return d ? d->pdata : NULL;
}

static Lidar *lidar_get_struct(WbDeviceTag t) {
  AbstractCamera *ac = lidar_get_abstract_camera_struct(t);
  return ac ? ac->pdata : NULL;
}

static void wb_lidar_cleanup(WbDevice *d) {
  AbstractCamera *ac = d->pdata;
  if (ac == NULL)
    return;
  Lidar *l = ac->pdata;
  if (l == NULL)
    return;
  free(l);
  ac->pdata = NULL;
  wb_abstract_camera_cleanup(d);
}

static void wb_lidar_new(WbDevice *d, unsigned int id, int w, int h, double fov, double camnear, double max_range,
                         bool spherical, int number_of_layers, double frequency, double min_frequency, double max_frequency,
                         double vertical_fov, int horizontal_resolution) {
  Lidar *l;
  wb_lidar_cleanup(d);
  wb_abstract_camera_new(d, id, w, h, fov, camnear, spherical);

  l = malloc(sizeof(Lidar));
  l->max_range = max_range;
  l->number_of_layers = number_of_layers;
  l->horizontal_resolution = horizontal_resolution;
  l->frequency = frequency;
  l->min_frequency = min_frequency;
  l->max_frequency = max_frequency;
  l->vertical_fov = vertical_fov;
  l->point_cloud_enabled = false;
  l->set_frequency = false;
  l->set_enable_point_cloud = false;
  l->set_disable_point_cloud = false;

  AbstractCamera *ac = d->pdata;
  ac->pdata = l;
}

static void wb_lidar_write_request(WbDevice *d, WbRequest *r) {
  wb_abstract_camera_write_request(d, r);
  AbstractCamera *ac = d->pdata;
  Lidar *l = ac->pdata;
  if (l->set_frequency) {
    request_write_uchar(r, C_LIDAR_SET_FREQUENCY);
    request_write_double(r, l->frequency);
    l->set_frequency = false;  // done
  }
  if (l->set_enable_point_cloud) {
    request_write_uchar(r, C_LIDAR_ENABLE_POINT_CLOUD);
    l->set_enable_point_cloud = false;  // done
  }
  if (l->set_disable_point_cloud) {
    request_write_uchar(r, C_LIDAR_DISABLE_POINT_CLOUD);
    l->set_disable_point_cloud = false;  // done
  }
}

static void wb_lidar_read_answer(WbDevice *d, WbRequest *r) {
  unsigned char command = request_read_uchar(r);
  if (wb_abstract_camera_handle_command(d, r, command))
    return;
  unsigned int uid;
  int width, height, number_of_layers, horizontal_resolution;
  double fov, camnear, max_range, frequency, min_frequency, max_frequency, vertical_fov;
  bool spherical;

  AbstractCamera *ac = d->pdata;
  Lidar *l = NULL;

  switch (command) {
    case C_CONFIGURE:
      uid = request_read_uint32(r);
      width = request_read_uint16(r);
      height = request_read_uint16(r);
      fov = request_read_double(r);
      camnear = request_read_double(r);
      spherical = request_read_uchar(r);
      max_range = request_read_double(r);
      number_of_layers = request_read_uint16(r);
      frequency = request_read_double(r);
      min_frequency = request_read_double(r);
      max_frequency = request_read_double(r);
      vertical_fov = request_read_double(r);
      horizontal_resolution = request_read_double(r);

      // printf("new lidar %u %d %d %lf %lf %lf %d\n", uid, width, height, fov, camnear, max_range, spherical);
      wb_lidar_new(d, uid, width, height, fov, camnear, max_range, spherical, number_of_layers, frequency, min_frequency,
                   max_frequency, vertical_fov, horizontal_resolution);
      break;
    case C_CAMERA_RECONFIGURE:
      l = ac->pdata;
      ac->fov = request_read_double(r);
      ac->camnear = request_read_double(r);
      ac->spherical = request_read_uchar(r);
      l->max_range = request_read_double(r);
      l->number_of_layers = request_read_uint16(r);
      l->frequency = request_read_double(r);
      l->min_frequency = request_read_double(r);
      l->max_frequency = request_read_double(r);
      l->vertical_fov = request_read_double(r);
      l->horizontal_resolution = request_read_double(r);
      break;

    default:
      ROBOT_ASSERT(0);
      break;
  }
}

static void lidar_toggle_remote(WbDevice *d, WbRequest *r) {
  abstract_camera_toggle_remote(d, r);
  AbstractCamera *ac = d->pdata;
  Lidar *l = ac->pdata;
  if (ac->sampling_period != 0) {
    ac->enable = true;
    ac->image->requested = true;
    if (remote_control_is_function_defined("wbr_lidar_set_frequency"))
      l->set_frequency = true;
  }
}

// Protected functions available from other source files

void wb_lidar_init(WbDevice *d) {
  d->read_answer = wb_lidar_read_answer;
  d->write_request = wb_lidar_write_request;
  d->cleanup = wb_lidar_cleanup;
  d->pdata = NULL;
  d->toggle_remote = lidar_toggle_remote;
  // g_print("lidar init done\n");
}

int wb_lidar_get_unique_id(WbDevice *d) {
  AbstractCamera *ac = d->pdata;
  return ac->unique_id;
}

void wbr_lidar_set_image(WbDeviceTag t, const unsigned char *image) {
  WbDevice *d = lidar_get_device(t);
  if (d)
    wbr_abstract_camera_set_image(d, image);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

unsigned char *wbr_lidar_get_image_buffer(WbDeviceTag t) {
  WbDevice *d = lidar_get_device(t);
  if (d)
    return wbr_abstract_camera_get_image_buffer(d);

  fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return (unsigned char *)"";  // don't return NULL, swig can't create string objects from NULL
}

// Public functions available from the lidar API

void wb_lidar_enable(WbDeviceTag tag, int sampling_period) {
  if (sampling_period < 0) {
    fprintf(stderr, "Error: %s() called with negative sampling period.\n", __FUNCTION__);
    return;
  }
  WbDevice *d = lidar_get_device(tag);
  if (d)
    wb_abstract_camera_enable(d, sampling_period);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

void wb_lidar_enable_point_cloud(WbDeviceTag tag) {
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (!l)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  else {
    l->point_cloud_enabled = true;
    l->set_enable_point_cloud = true;
    l->set_disable_point_cloud = false;
  }
  robot_mutex_unlock_step();
}

void wb_lidar_disable(WbDeviceTag tag) {
  Lidar *l = lidar_get_struct(tag);
  if (!l)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  else
    wb_lidar_enable(tag, 0);
}

void wb_lidar_disable_point_cloud(WbDeviceTag tag) {
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (!l)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  else {
    l->point_cloud_enabled = false;
    l->set_disable_point_cloud = true;
    l->set_enable_point_cloud = false;
  }
  robot_mutex_unlock_step();
}

int wb_lidar_get_sampling_period(WbDeviceTag tag) {
  Lidar *l = lidar_get_struct(tag);
  if (!l)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  return wb_abstract_camera_get_sampling_period(lidar_get_device(tag));
}

bool wb_lidar_is_point_cloud_enabled(WbDeviceTag tag) {
  bool result = false;
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (l)
    result = l->point_cloud_enabled;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

int wb_lidar_get_number_of_layers(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (l)
    result = l->number_of_layers;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_lidar_get_min_frequency(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (l)
    result = l->min_frequency;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_lidar_get_max_frequency(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (l)
    result = l->max_frequency;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_lidar_get_frequency(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (l)
    result = l->frequency;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

void wb_lidar_set_frequency(WbDeviceTag tag, double frequency) {
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (!l)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  else {
    if (frequency < l->min_frequency || frequency > l->max_frequency)
      fprintf(stderr, "Error: %s() out of frequency range [%f, %f].\n", __FUNCTION__, l->min_frequency, l->max_frequency);
    else {
      l->frequency = frequency;
      l->set_frequency = true;
    }
  }
  robot_mutex_unlock_step();
}

int wb_lidar_get_horizontal_resolution(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (l)
    result = l->horizontal_resolution;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_lidar_get_fov(WbDeviceTag tag) {
  WbDevice *d = lidar_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return wb_abstract_camera_get_fov(d);
}

double wb_lidar_get_vertical_fov(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (l)
    result = l->vertical_fov;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_lidar_get_min_range(WbDeviceTag tag) {
  WbDevice *d = lidar_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return wb_abstract_camera_get_near(d);
}

double wb_lidar_get_max_range(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Lidar *l = lidar_get_struct(tag);
  if (l)
    result = l->max_range;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

const float *wb_lidar_get_range_image(WbDeviceTag tag) {
  robot_mutex_lock_step();
  AbstractCamera *ac = lidar_get_abstract_camera_struct(tag);

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
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_lidar_enable().\n", __FUNCTION__);

  robot_mutex_unlock_step();

  return (const float *)(void *)ac->image->data;
}

const float *wb_lidar_get_layer_range_image(WbDeviceTag tag, int layer) {
  Lidar *l = lidar_get_struct(tag);
  if (!l) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    return NULL;
  }

  if (wb_abstract_camera_get_sampling_period(lidar_get_device(tag)) <= 0) {
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_lidar_enable().\n", __FUNCTION__);
    return NULL;
  }

  if (layer >= l->number_of_layers) {
    fprintf(stderr,
            "Error: %s() called with a 'layer' argument (%d) bigger or equal to the number of layers of this lidar (%d).\n",
            __FUNCTION__, layer, l->number_of_layers);
    return NULL;
  } else if (layer < 0) {
    fprintf(stderr, "Error: %s() called with a negative 'layer' argument.\n", __FUNCTION__);
    return NULL;
  }
  const float *image = wb_lidar_get_range_image(tag);
  if (image == NULL)
    return NULL;
  return image + layer * l->horizontal_resolution;
}

const WbLidarPoint *wb_lidar_get_point_cloud(WbDeviceTag tag) {
  Lidar *l = lidar_get_struct(tag);
  if (!l) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    return NULL;
  }
  if (!l->point_cloud_enabled) {
    fprintf(stderr, "Error: %s() called for a lidar with point cloud disabled. Please use: wb_lidar_enable_point_cloud().\n",
            __FUNCTION__);
    return NULL;
  }
  if (wb_abstract_camera_get_sampling_period(lidar_get_device(tag)) <= 0) {
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_lidar_enable().\n", __FUNCTION__);
    return NULL;
  }
  const float *image = wb_lidar_get_range_image(tag);
  if (image == NULL)
    return NULL;
  return (WbLidarPoint *)(image + l->number_of_layers * l->horizontal_resolution);
}

const WbLidarPoint *wb_lidar_get_layer_point_cloud(WbDeviceTag tag, int layer) {
  Lidar *l = lidar_get_struct(tag);
  if (!l) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    return NULL;
  }
  if (!l->point_cloud_enabled) {
    fprintf(stderr, "Error: %s() called for a lidar with point cloud disabled. Please use: wb_lidar_enable_point_cloud().\n",
            __FUNCTION__);
    return NULL;
  }
  if (wb_abstract_camera_get_sampling_period(lidar_get_device(tag)) <= 0) {
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_lidar_enable().\n", __FUNCTION__);
    return 0;
  }
  if (layer >= l->number_of_layers) {
    fprintf(stderr,
            "Error: %s() called with a 'layer' argument (%d) bigger or equal to the number of layers of this lidar (%d).\n",
            __FUNCTION__, layer, l->number_of_layers);
    return NULL;
  } else if (layer < 0) {
    fprintf(stderr, "Error: %s() called with a negative 'layer' argument.\n", __FUNCTION__);
    return NULL;
  }
  const WbLidarPoint *point_cloud = wb_lidar_get_point_cloud(tag);
  if (point_cloud == NULL)
    return NULL;
  return point_cloud + layer * l->horizontal_resolution;
}

int wb_lidar_get_number_of_points(WbDeviceTag tag) {
  Lidar *l = lidar_get_struct(tag);
  if (!l) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    return 0;
  }
  if (!l->point_cloud_enabled) {
    fprintf(stderr, "Error: %s() called for a lidar with point cloud disabled! Please use: wb_lidar_enable_point_cloud().\n",
            __FUNCTION__);
    return 0;
  }
  if (wb_abstract_camera_get_sampling_period(lidar_get_device(tag)) <= 0) {
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_lidar_enable().\n", __FUNCTION__);
    return 0;
  }
  return l->horizontal_resolution * l->number_of_layers;
}

const WbLidarPoint *wb_lidar_get_point(WbDeviceTag tag, int index) {
  Lidar *l = lidar_get_struct(tag);
  if (l)
    return (wb_lidar_get_point_cloud(tag) + index);

  fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return NULL;
}
