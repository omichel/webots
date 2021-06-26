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
// this file contains the API interface for the Motor device
// ***************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/motor.h>
#include <webots/nodes.h>
#include "messages.h"
#include "robot_private.h"

typedef struct {
  char requests[9];
  int force_feedback_sampling_period;
  double position;  // target position
  double velocity;
  double acceleration;
  double force;
  double available_force;
  double control_p;
  double control_i;
  double control_d;
  double force_feedback;
  double min_position;
  double max_position;
  double max_velocity;
  double max_force;
  double previous_velocity;
  double previous_acceleration;
  double previous_available_force;
  double previous_control_p;
  double previous_control_i;
  double previous_control_d;
  double multiplier;
  bool configured;
  WbJointType type;
  int requested_device_type;
  int associated_device_tag;
} Motor;

static Motor *motor_create() {
  Motor *motor = malloc(sizeof(Motor));
  int i;
  for (i = 0; i < sizeof(motor->requests); ++i)
    motor->requests[i] = 0;
  motor->force_feedback_sampling_period = 0;
  motor->position = 0.0;
  motor->velocity = 0.0;
  motor->acceleration = 0.0;
  motor->force = 0.0;
  motor->available_force = 0.0;
  motor->control_p = 0.0;
  motor->control_i = 0.0;
  motor->control_d = 0.0;
  motor->force_feedback = NAN;
  motor->min_position = 0.0;
  motor->max_position = 0.0;
  motor->max_velocity = 10.0;
  motor->max_force = 10.0;
  motor->multiplier = 1.0;
  motor->type = WB_ROTATIONAL;
  // To support user changes during remote control
  motor->previous_velocity = 10.0;
  motor->previous_acceleration = -1.0;
  motor->previous_available_force = 10.0;
  motor->previous_control_p = 10.0;
  motor->previous_control_i = 0.0;
  motor->previous_control_d = 0.0;
  // to be sure available_force is not overwritten after first configure
  motor->configured = false;
  motor->requested_device_type = 0;
  motor->associated_device_tag = 0;
  return motor;
}

// Static functions
static Motor *motor_get_struct(WbDeviceTag t) {
  WbDevice *d = robot_get_device_with_node(t, WB_NODE_ROTATIONAL_MOTOR, false);
  if (d == NULL)
    d = robot_get_device_with_node(t, WB_NODE_LINEAR_MOTOR, true);
  return d ? d->pdata : NULL;
}

static void motor_write_request(WbDevice *d, WbRequest *r) {
  Motor *m = (Motor *)d->pdata;
  int i;
  for (i = 0; i < sizeof(m->requests); ++i) {
    if (m->requests[i] == 0)
      continue;

    request_write_uint16(r, i);
    switch (i) {
      case C_MOTOR_SET_POSITION:
        request_write_double(r, m->position);
        break;
      case C_MOTOR_SET_VELOCITY:
        request_write_double(r, m->velocity);
        break;
      case C_MOTOR_SET_ACCELERATION:
        request_write_double(r, m->acceleration);
        break;
      case C_MOTOR_SET_FORCE:
        request_write_double(r, m->force);
        break;
      case C_MOTOR_SET_AVAILABLE_FORCE:
        request_write_double(r, m->available_force);
        break;
      case C_MOTOR_SET_CONTROL_PID:
        request_write_double(r, m->control_p);
        request_write_double(r, m->control_i);
        request_write_double(r, m->control_d);
        break;
      case C_MOTOR_FEEDBACK:
        request_write_uint16(r, m->force_feedback_sampling_period);
        break;
      case C_MOTOR_GET_ASSOCIATED_DEVICE:
        request_write_uint16(r, m->requested_device_type);
        break;
      default:
        break;
    }
    m->requests[i] = 0;
  }
}

static void motor_read_answer(WbDevice *d, WbRequest *r) {
  Motor *m = (Motor *)d->pdata;
  switch (request_read_uchar(r)) {
    case C_CONFIGURE:
      m->type = request_read_int32(r);
      m->min_position = request_read_double(r);
      m->max_position = request_read_double(r);
      m->max_velocity = request_read_double(r);
      if (!m->configured) {
        m->previous_velocity = m->max_velocity;
        m->velocity = m->max_velocity;
      }
      m->previous_acceleration = request_read_double(r);
      m->acceleration = m->previous_acceleration;
      m->max_force = request_read_double(r);
      if (!m->configured) {
        m->previous_available_force = m->max_force;
        m->available_force = m->max_force;
      } else {
        if (m->available_force == m->previous_available_force) {
          m->previous_available_force = m->max_force;
          m->available_force = m->max_force;
        } else {
          if (m->previous_available_force > m->max_force)
            m->previous_available_force = m->max_force;
          if (m->available_force > m->max_force)
            m->available_force = m->max_force;
        }
      }
      m->previous_control_p = request_read_double(r);
      m->previous_control_i = request_read_double(r);
      m->previous_control_d = request_read_double(r);
      m->control_p = m->previous_control_p;
      m->control_i = m->previous_control_i;
      m->control_d = m->previous_control_d;
      m->position = request_read_double(r);
      m->velocity = request_read_double(r);
      m->multiplier = request_read_double(r);
      m->configured = true;
      break;
    case C_MOTOR_FEEDBACK:
      m->force_feedback = request_read_double(r);
      break;
    case C_MOTOR_GET_ASSOCIATED_DEVICE:
      m->associated_device_tag = request_read_uint16(r);
      break;
    default:
      ROBOT_ASSERT(0);  // should not be reached
      break;
  }
}

static void motor_cleanup(WbDevice *d) {
  Motor *m = (Motor *)d->pdata;
  free(m);
}

static void motor_toggle_remote(WbDevice *d, WbRequest *r) {
  Motor *m = (Motor *)d->pdata;
  m->requests[C_MOTOR_FEEDBACK] = 1;

  // setup motor if changed by user
  if (m->velocity != m->previous_velocity)
    m->requests[C_MOTOR_SET_VELOCITY] = 1;
  if (m->acceleration != m->previous_acceleration)
    m->requests[C_MOTOR_SET_ACCELERATION] = 1;
  if (m->available_force != m->previous_available_force)
    m->requests[C_MOTOR_SET_AVAILABLE_FORCE] = 1;
  if (m->control_p != m->previous_control_p || m->control_i != m->previous_control_i || m->control_d != m->previous_control_d)
    m->requests[C_MOTOR_SET_CONTROL_PID] = 1;
}

// Exported functions

void wb_motor_init(WbDevice *d) {
  d->read_answer = motor_read_answer;
  d->write_request = motor_write_request;
  d->cleanup = motor_cleanup;
  d->toggle_remote = motor_toggle_remote;
  d->pdata = motor_create();
}

// Public functions (available from the user API)

void wb_motor_set_position_no_mutex(WbDeviceTag tag, double pos) {
  Motor *m = motor_get_struct(tag);
  if (m) {
    m->requests[C_MOTOR_SET_POSITION] = 1;
    m->position = pos;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

void wb_motor_set_position(WbDeviceTag tag, double position) {
  if (isnan(position)) {
    fprintf(stderr, "Error: %s() called with an invalid 'position' argument (NaN).\n", __FUNCTION__);
    return;
  }

  Motor *m = motor_get_struct(tag);
  if (!m)
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  else {
    robot_mutex_lock_step();
    wb_motor_set_position_no_mutex(tag, position);
    robot_mutex_unlock_step();
  }
}

void wb_motor_set_velocity(WbDeviceTag tag, double velocity) {
  if (isnan(velocity)) {
    fprintf(stderr, "Error: %s() called with an invalid 'velocity' argument.(NaN)\n", __FUNCTION__);
    return;
  }
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m) {
    if (velocity < 0.0) {
      if (!isinf(m->position)) {
        fprintf(stderr, "Error: %s() called with negative 'velocity' argument in position control mode (%g).\n", __FUNCTION__,
                m->position);
        robot_mutex_unlock_step();
        return;
      }
    }
    m->requests[C_MOTOR_SET_VELOCITY] = 1;
    m->velocity = velocity;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
}

double wb_motor_get_velocity(WbDeviceTag tag) {
  double vel = NAN;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    vel = m->velocity;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return vel;
}

double wb_motor_get_max_velocity(WbDeviceTag tag) {
  double vel = NAN;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    vel = m->max_velocity;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return vel;
}

void wb_motor_set_acceleration(WbDeviceTag tag, double acceleration) {
  if (acceleration < 0.0 && acceleration != -1) {
    fprintf(stderr, "Error: %s() called with negative 'acceleration' argument.\n", __FUNCTION__);
    return;
  }
  if (isnan(acceleration)) {
    fprintf(stderr, "Error: %s() called with an invalid 'acceleration' argument (NaN).\n", __FUNCTION__);
    return;
  }
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m) {
    m->requests[C_MOTOR_SET_ACCELERATION] = 1;
    m->acceleration = acceleration;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
}

double wb_motor_get_acceleration(WbDeviceTag tag) {
  double acc = NAN;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    acc = m->acceleration;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return acc;
}

void wb_motor_set_force(WbDeviceTag tag, double force) {
  if (isnan(force)) {
    fprintf(stderr, "Error: %s() called with an invalid 'force' argument (NaN).\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m) {
    m->requests[C_MOTOR_SET_FORCE] = 1;
    m->force = force;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
}

void wb_motor_set_available_force(WbDeviceTag tag, double force) {
  if (isnan(force)) {
    fprintf(stderr, "Error: %s() called with an invalid 'force' argument (NaN).\n", __FUNCTION__);
    return;
  }

  if (force < 0.0) {
    fprintf(stderr, "Error: %s() called with negative 'force' argument.\n", __FUNCTION__);
    return;
  }
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m) {
    m->requests[C_MOTOR_SET_AVAILABLE_FORCE] = 1;
    m->available_force = force;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
}

double wb_motor_get_available_force(WbDeviceTag tag) {
  double force = NAN;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    force = m->available_force;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return force;
}

double wb_motor_get_max_force(WbDeviceTag tag) {
  double force = NAN;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    force = m->max_force;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return force;
}

void wb_motor_set_control_pid(WbDeviceTag tag, double p, double i, double d) {
  if (p <= 0.0) {
    fprintf(stderr, "Error: %s() called with negative or zero 'p' argument.\n", __FUNCTION__);
    return;
  }

  if (i < 0.0) {
    fprintf(stderr, "Error: %s() called with negative 'i' argument.\n", __FUNCTION__);
    return;
  }

  if (d < 0.0) {
    fprintf(stderr, "Error: %s() called with negative 'd' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m) {
    m->requests[C_MOTOR_SET_CONTROL_PID] = 1;
    m->control_p = p;
    m->control_i = i;
    m->control_d = d;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
}

void wb_motor_enable_force_feedback(WbDeviceTag tag, int sampling_period) {
  if (sampling_period < 0) {
    fprintf(stderr, "Error: %s() called with negative sampling period.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m) {
    m->requests[C_MOTOR_FEEDBACK] = 1;
    m->force_feedback_sampling_period = sampling_period;
  } else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
}

void wb_motor_disable_force_feedback(WbDeviceTag tag) {
  Motor *m = motor_get_struct(tag);
  if (m)
    wb_motor_enable_force_feedback(tag, 0);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

double wb_motor_get_force_feedback(WbDeviceTag tag) {
  double result = NAN;
  robot_mutex_lock_step();
  const Motor *m = motor_get_struct(tag);
  if (m)
    result = m->force_feedback;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return result;
}

int wb_motor_get_force_feedback_sampling_period(WbDeviceTag tag) {
  int sampling_period = 0;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    sampling_period = m->force_feedback_sampling_period;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return sampling_period;
}

double wb_motor_get_min_position(WbDeviceTag tag) {
  double pos = NAN;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    pos = m->min_position;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return pos;
}

double wb_motor_get_max_position(WbDeviceTag tag) {
  double pos = NAN;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    pos = m->max_position;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return pos;
}

double wb_motor_get_target_position(WbDeviceTag tag) {
  double pos = NAN;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    pos = m->position;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return pos;
}

WbJointType wb_motor_get_type(WbDeviceTag tag) {
  WbJointType type = WB_ROTATIONAL;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    type = m->type;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  robot_mutex_unlock_step();
  return type;
}

void wbr_motor_set_force_feedback(WbDeviceTag t, double value) {
  Motor *m = motor_get_struct(t);
  if (m)
    m->force_feedback = value;
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

// Aliases
void wb_motor_set_available_torque(WbDeviceTag tag, double torque) {
  Motor *m = motor_get_struct(tag);
  if (m)
    wb_motor_set_available_force(tag, torque);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

double wb_motor_get_available_torque(WbDeviceTag tag) {
  Motor *m = motor_get_struct(tag);
  if (m)
    return wb_motor_get_available_force(tag);

  fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return NAN;
}

double wb_motor_get_multiplier(WbDeviceTag tag) {
  double multiplier;
  robot_mutex_lock_step();
  Motor *m = motor_get_struct(tag);
  if (m)
    multiplier = m->multiplier;
  else {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
    multiplier = NAN;
  }
  robot_mutex_unlock_step();
  return multiplier;
}

double wb_motor_get_max_torque(WbDeviceTag tag) {
  Motor *m = motor_get_struct(tag);
  if (m)
    return wb_motor_get_max_force(tag);

  fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return NAN;
}

void wb_motor_set_torque(WbDeviceTag tag, double torque) {
  Motor *m = motor_get_struct(tag);
  if (m)
    wb_motor_set_force(tag, torque);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

void wb_motor_enable_torque_feedback(WbDeviceTag tag, int sampling_period) {
  Motor *m = motor_get_struct(tag);
  if (m)
    wb_motor_enable_force_feedback(tag, sampling_period);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

void wb_motor_disable_torque_feedback(WbDeviceTag tag) {
  Motor *m = motor_get_struct(tag);
  if (m)
    wb_motor_disable_force_feedback(tag);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

int wb_motor_get_torque_feedback_sampling_period(WbDeviceTag tag) {
  Motor *m = motor_get_struct(tag);
  if (m)
    return wb_motor_get_force_feedback_sampling_period(tag);

  fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return 0;
}

double wb_motor_get_torque_feedback(WbDeviceTag tag) {
  Motor *m = motor_get_struct(tag);
  if (m)
    return wb_motor_get_force_feedback(tag);

  fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
  return NAN;
}

void wbr_motor_set_torque_feedback(WbDeviceTag t, double value) {
  Motor *m = motor_get_struct(t);
  if (m)
    wbr_motor_set_force_feedback(t, value);
  else
    fprintf(stderr, "Error: %s(): invalid device tag.\n", __FUNCTION__);
}

static WbDeviceTag motor_get_associated_device(WbDeviceTag t, int device_type, const char *function_name) {
  Motor *motor = motor_get_struct(t);
  if (!motor) {
    fprintf(stderr, "Error: %s(): invalid device tag.\n", function_name);
    return 0;
  }

  robot_mutex_lock_step();
  motor->requests[C_MOTOR_GET_ASSOCIATED_DEVICE] = 1;
  motor->requested_device_type = device_type;
  wb_robot_flush_unlocked();
  WbDeviceTag tag = motor->associated_device_tag;
  robot_mutex_unlock_step();
  return tag;
}

WbDeviceTag wb_motor_get_position_sensor(WbDeviceTag tag) {
  return motor_get_associated_device(tag, WB_NODE_POSITION_SENSOR, __FUNCTION__);
}

WbDeviceTag wb_motor_get_brake(WbDeviceTag tag) {
  return motor_get_associated_device(tag, WB_NODE_BRAKE, __FUNCTION__);
}
