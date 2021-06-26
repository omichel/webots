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

/*
 * Description:   Implement the functions defined in arm.h
 */

#include "arm.h"

#include <webots/motor.h>
#include <webots/robot.h>

#include <math.h>
#include <stdio.h>

static WbDeviceTag arm_elements[5];

static enum Height current_height = ARM_RESET;
static enum Orientation current_orientation = ARM_FRONT;

enum Height new_height;
enum Orientation new_orientation;

void arm_init() {
  arm_elements[ARM1] = wb_robot_get_device("arm1");
  arm_elements[ARM2] = wb_robot_get_device("arm2");
  arm_elements[ARM3] = wb_robot_get_device("arm3");
  arm_elements[ARM4] = wb_robot_get_device("arm4");
  arm_elements[ARM5] = wb_robot_get_device("arm5");

  wb_motor_set_velocity(arm_elements[ARM2], 0.5);

  arm_set_height(ARM_RESET);
  arm_set_orientation(ARM_FRONT);
}

void arm_reset() {
  arm_set_height(ARM_RESET);
  arm_set_orientation(ARM_FRONT);
}

void arm_set_height(enum Height height) {
  switch (height) {
    case ARM_FRONT_FLOOR:
      wb_motor_set_position(arm_elements[ARM2], -0.97);
      wb_motor_set_position(arm_elements[ARM3], -1.55);
      wb_motor_set_position(arm_elements[ARM4], -0.61);
      wb_motor_set_position(arm_elements[ARM5], 0.0);
      break;
    case ARM_FRONT_PLATE:
      wb_motor_set_position(arm_elements[ARM2], -0.62);
      wb_motor_set_position(arm_elements[ARM3], -0.98);
      wb_motor_set_position(arm_elements[ARM4], -1.53);
      wb_motor_set_position(arm_elements[ARM5], 0.0);
      break;
    case ARM_FRONT_CARDBOARD_BOX:
      wb_motor_set_position(arm_elements[ARM2], 0.0);
      wb_motor_set_position(arm_elements[ARM3], -0.77);
      wb_motor_set_position(arm_elements[ARM4], -1.21);
      wb_motor_set_position(arm_elements[ARM5], 0.0);
      break;
    case ARM_RESET:
      wb_motor_set_position(arm_elements[ARM2], 1.57);
      wb_motor_set_position(arm_elements[ARM3], -2.635);
      wb_motor_set_position(arm_elements[ARM4], 1.78);
      wb_motor_set_position(arm_elements[ARM5], 0.0);
      break;
    case ARM_BACK_PLATE_HIGH:
      wb_motor_set_position(arm_elements[ARM2], 0.678);
      wb_motor_set_position(arm_elements[ARM3], 0.682);
      wb_motor_set_position(arm_elements[ARM4], 1.74);
      wb_motor_set_position(arm_elements[ARM5], 0.0);
      break;
    case ARM_BACK_PLATE_LOW:
      wb_motor_set_position(arm_elements[ARM2], 0.92);
      wb_motor_set_position(arm_elements[ARM3], 0.42);
      wb_motor_set_position(arm_elements[ARM4], 1.78);
      wb_motor_set_position(arm_elements[ARM5], 0.0);
      break;
    case ARM_HANOI_PREPARE:
      wb_motor_set_position(arm_elements[ARM2], -0.4);
      wb_motor_set_position(arm_elements[ARM3], -1.2);
      wb_motor_set_position(arm_elements[ARM4], -M_PI_2);
      wb_motor_set_position(arm_elements[ARM5], M_PI_2);
      break;
    default:
      fprintf(stderr, "arm_height() called with a wrong argument\n");
      return;
  }
  current_height = height;
}

void arm_set_orientation(enum Orientation orientation) {
  switch (orientation) {
    case ARM_BACK_LEFT:
      wb_motor_set_position(arm_elements[ARM1], -2.949);
      break;
    case ARM_LEFT:
      wb_motor_set_position(arm_elements[ARM1], -M_PI_2);
      break;
    case ARM_FRONT_LEFT:
      wb_motor_set_position(arm_elements[ARM1], -0.2);
      break;
    case ARM_FRONT:
      wb_motor_set_position(arm_elements[ARM1], 0.0);
      break;
    case ARM_FRONT_RIGHT:
      wb_motor_set_position(arm_elements[ARM1], 0.2);
      break;
    case ARM_RIGHT:
      wb_motor_set_position(arm_elements[ARM1], M_PI_2);
      break;
    case ARM_BACK_RIGHT:
      wb_motor_set_position(arm_elements[ARM1], 2.949);
      break;
    default:
      fprintf(stderr, "arm_set_side() called with a wrong argument\n");
      return;
  }
  current_orientation = orientation;
}

void arm_increase_height() {
  new_height = current_height + 1;

  // Prevents from going beyond index.
  if (new_height >= ARM_MAX_HEIGHT)
    new_height = ARM_MAX_HEIGHT - 1;

  // Prevents self-colliding poses.
  if (new_height == ARM_FRONT_FLOOR) {
    if (current_orientation == ARM_BACK_LEFT || current_orientation == ARM_BACK_RIGHT)
      new_height = current_height;
  }

  arm_set_height(new_height);
}

void arm_decrease_height() {
  new_height = current_height - 1;
  if ((int)new_height < 0)
    new_height = 0;
  arm_set_height(new_height);
}

void arm_increase_orientation() {
  new_orientation = current_orientation + 1;

  // Prevents from going beyond index.
  if (new_orientation >= ARM_MAX_SIDE)
    new_orientation = ARM_MAX_SIDE - 1;

  // Prevents self-colliding poses.
  if (new_orientation == ARM_BACK_LEFT) {
    if (current_height == ARM_FRONT_FLOOR)
      new_orientation = current_orientation;
  }

  arm_set_orientation(new_orientation);
}

void arm_decrease_orientation() {
  new_orientation = current_orientation - 1;

  // Prevents from going beyond index.
  if ((int)new_orientation < 0)
    new_orientation = 0;

  // Prevents self-colliding poses.
  if (new_orientation == ARM_BACK_RIGHT) {
    if (current_height == ARM_FRONT_FLOOR)
      new_orientation = current_orientation;
  }

  arm_set_orientation(new_orientation);
}

void arm_set_sub_arm_rotation(enum Arm arm, double radian) {
  wb_motor_set_position(arm_elements[arm], radian);
}

double arm_get_sub_arm_length(enum Arm arm) {
  switch (arm) {
    case ARM1:
      return 0.253;
    case ARM2:
      return 0.155;
    case ARM3:
      return 0.135;
    case ARM4:
      return 0.081;
    case ARM5:
      return 0.105;
  }
  return 0.0;
}

void arm_ik(double x, double y, double z) {
  double x1 = sqrt(x * x + z * z);
  double y1 = y + arm_get_sub_arm_length(ARM4) + arm_get_sub_arm_length(ARM5) - arm_get_sub_arm_length(ARM1);

  double a = arm_get_sub_arm_length(ARM2);
  double b = arm_get_sub_arm_length(ARM3);
  double c = sqrt(x1 * x1 + y1 * y1);

  double alpha = -asin(z / x1);
  double beta = -(M_PI_2 - acos((a * a + c * c - b * b) / (2.0 * a * c)) - atan(y1 / x1));
  double gamma = -(M_PI - acos((a * a + b * b - c * c) / (2.0 * a * b)));
  double delta = -(M_PI + (beta + gamma));
  double epsilon = M_PI_2 + alpha;

  wb_motor_set_position(arm_elements[ARM1], alpha);
  wb_motor_set_position(arm_elements[ARM2], beta);
  wb_motor_set_position(arm_elements[ARM3], gamma);
  wb_motor_set_position(arm_elements[ARM4], delta);
  wb_motor_set_position(arm_elements[ARM5], epsilon);
}
