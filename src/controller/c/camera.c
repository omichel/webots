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

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/camera.h>
#include <webots/robot.h>
#include "abstract_camera.h"
#include "g_image.h"
#include "messages.h"
#include "remote_control_private.h"
#include "robot_private.h"

typedef struct {
  double min_fov;
  double max_fov;
  double exposure;
  double focal_length;
  double focal_distance;
  double min_focal_distance;
  double max_focal_distance;
  bool set_exposure;
  bool set_focal_distance;
  bool set_fov;
  bool has_recognition;
  bool enable_recognition;
  int recognition_sampling_period;
  int recognized_object_number;                   // number of object currrently recognized
  WbCameraRecognitionObject *recognized_objects;  // list of objects
  bool segmentation;
  bool segmentation_enabled;
  bool segmentation_changed;
  Image *segmentation_image;
} Camera;

static WbDevice *camera_get_device(WbDeviceTag t) {
  WbDevice *d = robot_get_device_with_node(t, WB_NODE_CAMERA, true);
  return d;
}

static AbstractCamera *camera_get_abstract_camera_struct(WbDeviceTag t) {
  WbDevice *d = camera_get_device(t);
  return d ? d->pdata : NULL;
}

static Camera *camera_get_struct(WbDeviceTag tag) {
  AbstractCamera *ac = camera_get_abstract_camera_struct(tag);
  return ac ? ac->pdata : NULL;
}

static void camera_clear_recognized_objects_list(Camera *c) {
  int i;
  for (i = 0; i < c->recognized_object_number; ++i) {
    free(c->recognized_objects[i].colors);
    free(c->recognized_objects[i].model);
  }
  free(c->recognized_objects);
}

static void wb_camera_cleanup(WbDevice *d) {
  AbstractCamera *ac = d->pdata;
  if (!ac)
    return;
  Camera *c = ac->pdata;
  if (!c)
    return;
  camera_clear_recognized_objects_list(c);
  if (c->segmentation_image) {
    image_cleanup_shm(c->segmentation_image);
    free(c->segmentation_image);
  }
  free(c);
  ac->pdata = NULL;
  wb_abstract_camera_cleanup(d);
}

static void wb_camera_new(WbDevice *d, unsigned int id, int w, int h, double fov, double min_fov, double max_fov,
                          double exposure, double focal_length, double focal_distance, double min_focal_distance,
                          double max_focal_distance, double camnear, bool spherical, bool has_recognition, bool segmentation) {
  Camera *c;
  wb_camera_cleanup(d);
  wb_abstract_camera_new(d, id, w, h, fov, camnear, spherical);

  c = malloc(sizeof(Camera));
  c->min_fov = min_fov;
  c->max_fov = max_fov;
  c->exposure = exposure;
  c->focal_length = focal_length;
  c->focal_distance = focal_distance;
  c->min_focal_distance = min_focal_distance;
  c->max_focal_distance = max_focal_distance;
  c->has_recognition = has_recognition;
  c->set_focal_distance = false;
  c->set_fov = false;
  c->enable_recognition = false;
  c->recognition_sampling_period = 0;
  c->recognized_object_number = 0;
  c->recognized_objects = NULL;
  c->segmentation = segmentation;
  c->segmentation_enabled = false;
  c->segmentation_changed = false;
  if (c->segmentation)
    c->segmentation_image = image_new();
  else
    c->segmentation_image = NULL;

  AbstractCamera *ac = d->pdata;
  ac->pdata = c;
}

static void wb_camera_write_request(WbDevice *d, WbRequest *r) {
  wb_abstract_camera_write_request(d, r);
  AbstractCamera *ac = d->pdata;
  Camera *c = ac->pdata;
  if (c->set_fov) {
    request_write_uchar(r, C_CAMERA_SET_FOV);
    request_write_double(r, ac->fov);
    c->set_fov = false;  // done
  }
  if (c->set_exposure) {
    request_write_uchar(r, C_CAMERA_SET_EXPOSURE);
    request_write_double(r, c->exposure);
    c->set_exposure = false;  // done
  }
  if (c->set_focal_distance) {
    request_write_uchar(r, C_CAMERA_SET_FOCAL);
    request_write_double(r, c->focal_distance);
    c->set_focal_distance = false;  // done
  }
  if (c->enable_recognition) {
    request_write_uchar(r, C_CAMERA_SET_RECOGNITION_SAMPLING_PERIOD);
    request_write_uint16(r, c->recognition_sampling_period);
    c->enable_recognition = false;  // done
  }
  if (c->segmentation_changed) {
    request_write_uchar(r, C_CAMERA_ENABLE_SEGMENTATION);
    request_write_uchar(r, c->segmentation_enabled ? 1 : 0);
    if (c->segmentation_enabled && !c->segmentation_image)
      c->segmentation_image = image_new();
    c->segmentation_changed = false;  // done
  }
}

static void wb_camera_read_answer(WbDevice *d, WbRequest *r) {
  unsigned char command = request_read_uchar(r);
  if (wb_abstract_camera_handle_command(d, r, command))
    return;
  unsigned int uid;
  int width, height;
  double fov, min_fov, max_fov, camnear, exposure, focal_length, focal_distance, min_focal_distance, max_focal_distance;
  bool spherical, has_recognition, segmentation;

  AbstractCamera *ac = d->pdata;
  Camera *c = NULL;

  switch (command) {
    case C_CONFIGURE:
      uid = request_read_uint32(r);
      width = request_read_uint16(r);
      height = request_read_uint16(r);
      fov = request_read_double(r);
      camnear = request_read_double(r);
      spherical = request_read_uchar(r);
      min_fov = request_read_double(r);
      max_fov = request_read_double(r);
      has_recognition = request_read_uchar(r) != 0;
      segmentation = request_read_uchar(r) != 0;
      exposure = request_read_double(r);
      focal_length = request_read_double(r);
      focal_distance = request_read_double(r);
      min_focal_distance = request_read_double(r);
      max_focal_distance = request_read_double(r);

      // printf("new camera %u %d %d %lf %lf %d\n", uid, width, height, fov, camnear, spherical);
      wb_camera_new(d, uid, width, height, fov, min_fov, max_fov, exposure, focal_length, focal_distance, min_focal_distance,
                    max_focal_distance, camnear, spherical, has_recognition, segmentation);
      break;
    case C_CAMERA_RECONFIGURE:
      c = ac->pdata;
      ac->fov = request_read_double(r);
      ac->camnear = request_read_double(r);
      ac->spherical = request_read_uchar(r);
      c->min_fov = request_read_double(r);
      c->max_fov = request_read_double(r);
      c->has_recognition = request_read_uchar(r) != 0;
      c->segmentation = request_read_uchar(r) != 0;
      c->exposure = request_read_double(r);
      c->focal_length = request_read_double(r);
      c->focal_distance = request_read_double(r);
      c->min_focal_distance = request_read_double(r);
      c->max_focal_distance = request_read_double(r);
      break;
    case C_CAMERA_OBJECTS: {
      c = ac->pdata;
      int i, j;

      // clean previous list
      camera_clear_recognized_objects_list(c);
      // get number of recognized objects
      c->recognized_object_number = request_read_int32(r);
      c->recognized_objects =
        (WbCameraRecognitionObject *)malloc(c->recognized_object_number * sizeof(WbCameraRecognitionObject));

      for (i = 0; i < c->recognized_object_number; ++i) {
        // get id of the object
        c->recognized_objects[i].id = request_read_int32(r);
        // get relative position of the object
        c->recognized_objects[i].position[0] = request_read_double(r);
        c->recognized_objects[i].position[1] = request_read_double(r);
        c->recognized_objects[i].position[2] = request_read_double(r);
        // get relative orientation of the object
        c->recognized_objects[i].orientation[0] = request_read_double(r);
        c->recognized_objects[i].orientation[1] = request_read_double(r);
        c->recognized_objects[i].orientation[2] = request_read_double(r);
        c->recognized_objects[i].orientation[3] = request_read_double(r);
        // get size of the object
        c->recognized_objects[i].size[0] = request_read_double(r);
        c->recognized_objects[i].size[1] = request_read_double(r);
        // get position of the object on the camera image
        c->recognized_objects[i].position_on_image[0] = request_read_int32(r);
        c->recognized_objects[i].position_on_image[1] = request_read_int32(r);
        // get size of the object on the camera image
        c->recognized_objects[i].size_on_image[0] = request_read_int32(r);
        c->recognized_objects[i].size_on_image[1] = request_read_int32(r);
        // get number of colors of the object
        c->recognized_objects[i].number_of_colors = request_read_int32(r);
        const int size = 3 * c->recognized_objects[i].number_of_colors * sizeof(double *);
        c->recognized_objects[i].colors = (double *)malloc(size);
        for (j = 0; j < c->recognized_objects[i].number_of_colors; j++) {
          // get each color of the object
          c->recognized_objects[i].colors[3 * j] = request_read_double(r);
          c->recognized_objects[i].colors[3 * j + 1] = request_read_double(r);
          c->recognized_objects[i].colors[3 * j + 2] = request_read_double(r);
        }
        // get the model of the object
        c->recognized_objects[i].model = request_read_string(r);
      }
      break;
    }
    case C_CAMERA_SET_SEGMENTATION:
      c = ac->pdata;
      c->segmentation = request_read_uchar(r);
      if (!c->segmentation)
        c->segmentation_enabled = false;
      break;
    case C_CAMERA_SEGMENTATION_SHARED_MEMORY:
      // Cleanup the previous shared memory if any.
      c = ac->pdata;
      assert(c->segmentation);
      if (!c->segmentation_image)
        c->segmentation_image = image_new();  // prevent controller crash
      image_cleanup_shm(c->segmentation_image);
      image_setup_shm(c->segmentation_image, r);
      break;
    default:
      ROBOT_ASSERT(0);
      break;
  }
}

static void camera_toggle_remote(WbDevice *d, WbRequest *r) {
  abstract_camera_toggle_remote(d, r);
  AbstractCamera *ac = d->pdata;
  Camera *c = ac->pdata;
  if (ac->sampling_period != 0) {
    ac->enable = true;
    ac->image->requested = true;
    if (remote_control_is_function_defined("wbr_camera_set_fov"))
      c->set_fov = true;
    if (remote_control_is_function_defined("wbr_camera_set_exposure"))
      c->set_exposure = true;
    if (remote_control_is_function_defined("wbr_camera_set_focal_distance"))
      c->set_focal_distance = true;
  }
  if (c->recognition_sampling_period != 0)
    c->enable_recognition = true;
}

// Protected functions available from other source files

void wb_camera_init(WbDevice *d) {
  d->read_answer = wb_camera_read_answer;
  d->write_request = wb_camera_write_request;
  d->cleanup = wb_camera_cleanup;
  d->pdata = NULL;
  d->toggle_remote = camera_toggle_remote;
  // g_print("camera init done\n");
}

void wbr_camera_set_image(WbDeviceTag tag, const unsigned char *image) {
  WbDevice *d = camera_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  wbr_abstract_camera_set_image(d, image);
}

unsigned char *wbr_camera_get_image_buffer(WbDeviceTag tag) {
  WbDevice *d = camera_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return wbr_abstract_camera_get_image_buffer(d);
}

void wbr_camera_recognition_set_object(WbDeviceTag tag, const WbCameraRecognitionObject *objects, int object_number) {
  Camera *c = camera_get_struct(tag);
  if (c) {
    camera_clear_recognized_objects_list(c);
    // get number of recognized objects
    c->recognized_object_number = object_number;
    c->recognized_objects =
      (WbCameraRecognitionObject *)malloc(c->recognized_object_number * sizeof(WbCameraRecognitionObject));
    int i, j;
    for (i = 0; i < c->recognized_object_number; ++i) {
      // set id of the object
      c->recognized_objects[i].id = objects[i].id;
      // set relative position of the object
      c->recognized_objects[i].position[0] = objects[i].position[0];
      c->recognized_objects[i].position[1] = objects[i].position[1];
      c->recognized_objects[i].position[2] = objects[i].position[2];
      // set relative orientation of the object
      c->recognized_objects[i].orientation[0] = objects[i].orientation[0];
      c->recognized_objects[i].orientation[1] = objects[i].orientation[1];
      c->recognized_objects[i].orientation[2] = objects[i].orientation[2];
      c->recognized_objects[i].orientation[3] = objects[i].orientation[3];
      // set size of the object
      c->recognized_objects[i].size[0] = objects[i].size[0];
      c->recognized_objects[i].size[1] = objects[i].size[1];
      // set position of the object on the camera image
      c->recognized_objects[i].position_on_image[0] = objects[i].position_on_image[0];
      c->recognized_objects[i].position_on_image[1] = objects[i].position_on_image[1];
      // set size of the object on the camera image
      c->recognized_objects[i].size_on_image[0] = objects[i].size_on_image[0];
      c->recognized_objects[i].size_on_image[1] = objects[i].size_on_image[1];
      // set number of colors of the object
      c->recognized_objects[i].number_of_colors = objects[i].number_of_colors;
      const int size = 3 * c->recognized_objects[i].number_of_colors * sizeof(double *);
      c->recognized_objects[i].colors = (double *)malloc(size);
      for (j = 0; j < c->recognized_objects[i].number_of_colors; j++) {
        // set each color of the object
        c->recognized_objects[i].colors[3 * j] = objects[i].colors[0];
        c->recognized_objects[i].colors[3 * j + 1] = objects[i].colors[1];
        c->recognized_objects[i].colors[3 * j + 2] = objects[i].colors[2];
      }
      // set the model of the object
      c->recognized_objects[i].model = (char *)malloc(sizeof(objects[i].model));
      strcpy(c->recognized_objects[i].model, objects[i].model);
    }
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

// Public functions available from the camera API

void wb_camera_enable(WbDeviceTag tag, int sampling_period) {
  if (sampling_period < 0) {
    fprintf(stderr, "Error: %s() called with negative sampling period.\n", __FUNCTION__);
    return;
  }

  WbDevice *d = camera_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  wb_abstract_camera_enable(d, sampling_period);
}

void wb_camera_disable(WbDeviceTag tag) {
  Camera *c = camera_get_struct(tag);
  if (c)
    wb_camera_enable(tag, 0);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

int wb_camera_get_sampling_period(WbDeviceTag tag) {
  WbDevice *d = camera_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s: invalid device tag.\n", __FUNCTION__);
  return wb_abstract_camera_get_sampling_period(d);
}

int wb_camera_get_height(WbDeviceTag tag) {
  WbDevice *d = camera_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return wb_abstract_camera_get_height(d);
}

int wb_camera_get_width(WbDeviceTag tag) {
  WbDevice *d = camera_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return wb_abstract_camera_get_width(d);
}

double wb_camera_get_fov(WbDeviceTag tag) {
  WbDevice *d = camera_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return wb_abstract_camera_get_fov(d);
}

double wb_camera_get_min_fov(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    result = c->min_fov;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_camera_get_max_fov(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    result = c->max_fov;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

void wb_camera_set_fov(WbDeviceTag tag, double fov) {
  bool in_range = true;
  robot_mutex_lock_step();
  AbstractCamera *ac = camera_get_abstract_camera_struct(tag);
  Camera *c = camera_get_struct(tag);
  if (!ac || !c) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return;
  }
  if (ac->spherical && (fov < 0.0 || fov > 2.0 * M_PI)) {
    fprintf(stderr, "Error: %s() called with 'fov' argument outside of the [0, 2.0*pi] range.\n", __FUNCTION__);
    in_range = false;
  } else if (!ac->spherical && (fov < 0.0 || fov > M_PI)) {
    fprintf(stderr, "Error: %s() called with 'fov' argument outside of the [0, pi] range.\n", __FUNCTION__);
    in_range = false;
  } else if (fov < c->min_fov || fov > c->max_fov) {
    fprintf(stderr, "Error: %s() out of zoom range [%f, %f].\n", __FUNCTION__, c->min_fov, c->max_fov);
    in_range = false;
  }
  if (in_range) {
    ac->fov = fov;
    c->set_fov = true;
  }
  robot_mutex_unlock_step();
}

double wb_camera_get_focal_length(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    result = c->focal_length;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_camera_get_exposure(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    result = c->exposure;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

void wb_camera_set_exposure(WbDeviceTag tag, double exposure) {
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (!c)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  else if (exposure < 0)
    fprintf(stderr, "Error: 'exposure' argument of %s() can't be negative.\n", __FUNCTION__);
  else {
    c->exposure = exposure;
    c->set_exposure = true;
  }
  robot_mutex_unlock_step();
}

double wb_camera_get_focal_distance(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    result = c->focal_distance;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_camera_get_min_focal_distance(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    result = c->min_focal_distance;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

double wb_camera_get_max_focal_distance(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    result = c->max_focal_distance;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

void wb_camera_set_focal_distance(WbDeviceTag tag, double focal_distance) {
  bool in_range = true;
  robot_mutex_lock_step();
  AbstractCamera *ac = camera_get_abstract_camera_struct(tag);
  Camera *c = camera_get_struct(tag);
  if (!c || !ac) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return;
  } else if (ac->spherical) {
    fprintf(stderr, "Error: %s() can't be called on a spherical camera.\n", __FUNCTION__);
    in_range = false;
  } else if (focal_distance < c->min_focal_distance || focal_distance > c->max_focal_distance) {
    fprintf(stderr, "Error: %s() out of focus range [%f, %f].\n", __FUNCTION__, c->min_focal_distance, c->max_focal_distance);
    in_range = false;
  }
  if (in_range) {
    c->focal_distance = focal_distance;
    c->set_focal_distance = true;
  }
  robot_mutex_unlock_step();
}

double wb_camera_get_near(WbDeviceTag tag) {
  WbDevice *d = camera_get_device(tag);
  if (!d)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  return wb_abstract_camera_get_near(camera_get_device(tag));
}

void wb_camera_recognition_enable(WbDeviceTag tag, int sampling_period) {
  if (sampling_period < 0) {
    fprintf(stderr, "Error: %s() called with negative sampling period.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);

  if (c) {
    if (!c->has_recognition)
      fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    else {
      c->enable_recognition = true;
      c->recognition_sampling_period = sampling_period;
    }
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);

  robot_mutex_unlock_step();
}

void wb_camera_recognition_disable(WbDeviceTag tag) {
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  bool should_return = false;
  if (!c) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    should_return = true;
  } else if (!c->has_recognition) {
    fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    should_return = true;
  }
  robot_mutex_unlock_step();
  if (!should_return)
    wb_camera_recognition_enable(tag, 0);
}

int wb_camera_recognition_get_sampling_period(WbDeviceTag tag) {
  int sampling_period = 0;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c) {
    if (!c->has_recognition)
      fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    else
      sampling_period = c->recognition_sampling_period;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return sampling_period;
}

int wb_camera_recognition_get_number_of_objects(WbDeviceTag tag) {
  int result = 0;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c) {
    if (!c->has_recognition)
      fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    else if (c->recognition_sampling_period == 0)
      fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_camera_recognition_enable().\n", __FUNCTION__);
    else
      result = c->recognized_object_number;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

bool wb_camera_has_recognition(WbDeviceTag tag) {
  bool has_recognition = false;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    has_recognition = c->has_recognition;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return has_recognition;
}

const WbCameraRecognitionObject *wb_camera_recognition_get_objects(WbDeviceTag tag) {
  const WbCameraRecognitionObject *result = 0;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c) {
    if (!c->has_recognition)
      fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    else if (c->recognition_sampling_period == 0)
      fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_camera_recognition_enable().\n", __FUNCTION__);
    else
      result = c->recognized_objects;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

const unsigned char *wb_camera_get_image(WbDeviceTag tag) {
  robot_mutex_lock_step();
  AbstractCamera *ac = camera_get_abstract_camera_struct(tag);

  if (!ac) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return NULL;
  }

  if (ac->sampling_period <= 0) {
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_camera_enable().\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return NULL;
  }

  if (wb_robot_get_mode() == WB_MODE_REMOTE_CONTROL) {
    robot_mutex_unlock_step();
    return ac->image->data;
  }

  robot_mutex_unlock_step();
  return ac->image->data;
}

int wb_camera_save_image(WbDeviceTag tag, const char *filename, int quality) {
  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'filename' argument.\n", __FUNCTION__);
    return -1;
  }
  unsigned char type = g_image_get_type(filename);
  if (type != G_IMAGE_PNG && type != G_IMAGE_JPEG) {
    fprintf(stderr, "Error: %s() called with unsupported image format (should be PNG or JPEG).\n", __FUNCTION__);
    return -1;
  }
  if (type == G_IMAGE_JPEG && (quality < 1 || quality > 100)) {
    fprintf(stderr, "Error: %s() called with invalid 'quality' argument.\n", __FUNCTION__);
    return -1;
  }

  robot_mutex_lock_step();
  AbstractCamera *ac = camera_get_abstract_camera_struct(tag);

  if (!ac) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return -1;
  }

  if (!ac->image->data) {
    robot_mutex_unlock_step();
    return -1;
  }
  GImage img;
  img.width = ac->width;
  img.height = ac->height;

  img.data_format = G_IMAGE_DATA_FORMAT_BGRA;
  img.data = ac->image->data;
  int ret = g_image_save(&img, filename, quality);

  robot_mutex_unlock_step();
  return ret;
}

const WbCameraRecognitionObject *wb_camera_recognition_get_object(WbDeviceTag tag, int index) {
  Camera *c = camera_get_struct(tag);
  if (!c) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    return NULL;
  }
  return (wb_camera_recognition_get_objects(tag) + index);
}

bool wb_camera_recognition_has_segmentation(WbDeviceTag tag) {
  bool has_segmentation;
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (c)
    has_segmentation = c->segmentation;
  else {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    has_segmentation = false;
  }
  robot_mutex_unlock_step();
  return has_segmentation;
}

void wb_camera_recognition_enable_segmentation(WbDeviceTag tag) {
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (!c) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return;
  }
  if (!c->has_recognition) {
    fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return;
  }
  if (c->recognition_sampling_period == 0) {
    fprintf(stderr, "Error: %s() called for a disabled device! Please use: wb_camera_recognition_enable().\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return;
  }
  if (!c->segmentation) {
    fprintf(stderr, "Error: %s(): segmentation is disabled in Recognition node.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return;
  }
  if (!c->segmentation_enabled) {
    c->segmentation_enabled = true;
    c->segmentation_changed = true;
  }
  robot_mutex_unlock_step();
}

void wb_camera_recognition_disable_segmentation(WbDeviceTag tag) {
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (!c) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return;
  }
  if (!c->has_recognition) {
    fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return;
  }
  if (c->segmentation_enabled) {
    c->segmentation_enabled = false;
    c->segmentation_changed = true;
  }
  robot_mutex_unlock_step();
}

bool wb_camera_recognition_is_segmentation_enabled(WbDeviceTag tag) {
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (!c) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return false;
  }
  if (!c->has_recognition) {
    fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return false;
  }
  const bool is_segmentation_enabled = c->segmentation_enabled;
  robot_mutex_unlock_step();
  return is_segmentation_enabled;
}

const unsigned char *wb_camera_recognition_get_segmentation_image(WbDeviceTag tag) {
  robot_mutex_lock_step();
  Camera *c = camera_get_struct(tag);
  if (!c) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return NULL;
  }
  if (!c->has_recognition) {
    fprintf(stderr, "Error: %s() called on a Camera without Recognition node.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return NULL;
  }
  if (!c->segmentation) {
    robot_mutex_unlock_step();
    return NULL;
  }
  if (!c->segmentation_enabled) {
    fprintf(stderr, "Error: %s(): segmentation is disabled! Please use: wb_camera_recognition_enable_segmentation().\n",
            __FUNCTION__);
    robot_mutex_unlock_step();
    return NULL;
  }
  if (!c->segmentation_image->data) {
    robot_mutex_unlock_step();
    return NULL;
  }
  robot_mutex_unlock_step();
  return c->segmentation_image->data;
}

int wb_camera_recognition_save_segmentation_image(WbDeviceTag tag, const char *filename, int quality) {
  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'filename' argument.\n", __FUNCTION__);
    return -1;
  }
  unsigned char type = g_image_get_type(filename);
  if (type != G_IMAGE_PNG && type != G_IMAGE_JPEG) {
    fprintf(stderr, "Error: %s() called with unsupported image format (should be PNG or JPEG).\n", __FUNCTION__);
    return -1;
  }
  if (type == G_IMAGE_JPEG && (quality < 1 || quality > 100)) {
    fprintf(stderr, "Error: %s() called with invalid 'quality' argument.\n", __FUNCTION__);
    return -1;
  }

  robot_mutex_lock_step();
  AbstractCamera *ac = camera_get_abstract_camera_struct(tag);
  Camera *c = camera_get_struct(tag);
  if (!c) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return -1;
  }
  if (!c->segmentation_image) {
    fprintf(stderr, "Error: %s() called before rendering a valid segmentation image.\n", __FUNCTION__);
    robot_mutex_unlock_step();
    return -1;
  }

  GImage img;
  img.width = ac->width;
  img.height = ac->height;

  img.data_format = G_IMAGE_DATA_FORMAT_BGRA;
  img.data = c->segmentation_image->data;
  int ret = g_image_save(&img, filename, quality);

  robot_mutex_unlock_step();
  return ret;
}
