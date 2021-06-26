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
 * Description:  Basic controller for JetBot robot.
 */

#include <webots/camera.h>
#include <webots/motor.h>
#include <webots/robot.h>

#define SPEED_FACTOR 10.0

static WbDeviceTag left_motor = 0;
static WbDeviceTag right_motor = 0;

static double convert_speed(double speed) {
  if (speed < 0.2)
    return 0.0;

  const double converted_values[9] = {0, 0.12, 0.2, 0.3, 0.4, 0.45, 0.55, 0.6, 0.65};
  double max_speed = 0.3;
  for (int i = 1; i < 9; ++i, max_speed += 0.1) {
    if (speed <= max_speed)
      return SPEED_FACTOR *
             ((converted_values[i] - converted_values[i - 1]) / 0.1 * (speed - max_speed + 0.1) + converted_values[i - 1]);
  }
  return converted_values[8] * SPEED_FACTOR;
}

static void left(double speed) {
  const double s = convert_speed(speed);
  wb_motor_set_velocity(left_motor, -s);
  wb_motor_set_velocity(right_motor, s);
}

static void right(double speed) {
  const double s = convert_speed(speed);
  wb_motor_set_velocity(left_motor, s);
  wb_motor_set_velocity(right_motor, -s);
}

static void forward(double speed) {
  const double s = convert_speed(speed);
  wb_motor_set_velocity(left_motor, s);
  wb_motor_set_velocity(right_motor, s);
}

static void backward(double speed) {
  const double s = convert_speed(speed);
  wb_motor_set_velocity(left_motor, -s);
  wb_motor_set_velocity(right_motor, -s);
}

static void set_motors(double speed_left, double speed_right) {
  wb_motor_set_velocity(left_motor, convert_speed(speed_left));
  wb_motor_set_velocity(right_motor, convert_speed(speed_right));
}

static void stop() {
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
}

static void step_forward() {
  forward(0.4);
  wb_robot_step(512);
  stop();
}

static void step_backward() {
  backward(0.4);
  wb_robot_step(512);
  stop();
}

static void step_left() {
  left(0.3);
  wb_robot_step(512);
  stop();
}

static void step_right() {
  right(0.3);
  wb_robot_step(512);
  stop();
}

int main(int argc, char **argv) {
  wb_robot_init();

  left_motor = wb_robot_get_device("left_wheel_hinge");
  right_motor = wb_robot_get_device("right_wheel_hinge");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  WbDeviceTag camera = wb_robot_get_device("camera");
  // WbDeviceTag display = wb_robot_get_device("display");

  int timestep = wb_robot_get_basic_time_step();
  wb_camera_enable(camera, timestep);
  set_motors(0.0, 0.0);
  wb_robot_step(480);

  /* main loop */
  while (wb_robot_step(timestep) != -1) {
    step_forward();
    wb_robot_step(960);
    step_backward();
    wb_robot_step(960);
    step_left();
    wb_robot_step(960);
    step_right();
    wb_robot_step(960);
  };

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();

  return 0;
}
