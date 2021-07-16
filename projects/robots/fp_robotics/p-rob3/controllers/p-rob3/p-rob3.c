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

#include <webots/motor.h>
#include <webots/robot.h>

#include <stdlib.h>

#define MOTOR_NUMBER 8

static WbDeviceTag motors[MOTOR_NUMBER];

static const char *motor_names[MOTOR_NUMBER] = {"motor 1", "motor 2", "motor 3", "motor 4",
                                                "motor 5", "motor 6", "motor 7", "motor 7 left"};

static int get_time_step() {
  static int time_step = -1;
  if (time_step == -1)
    time_step = (int)wb_robot_get_basic_time_step();
  return time_step;
}

static void step() {
  if (wb_robot_step(get_time_step()) == -1) {
    wb_robot_cleanup();
    exit(EXIT_SUCCESS);
  }
}

static void passive_wait(double sec) {
  double start_time = wb_robot_get_time();
  do {
    step();
  } while (start_time + sec > wb_robot_get_time());
}

void open_gripper() {
  wb_motor_set_position(motors[6], 0.5);
  wb_motor_set_position(motors[7], 0.5);
}

void close_gripper() {
  wb_motor_set_torque(motors[6], -0.2);
  wb_motor_set_torque(motors[7], -0.2);
}

int main(int argc, char **argv) {
  wb_robot_init();

  for (int i = 0; i < MOTOR_NUMBER; ++i)
    motors[i] = wb_robot_get_device(motor_names[i]);

  while (true) {
    wb_motor_set_position(motors[4], -1.95);
    passive_wait(0.5);

    // prepare for grasping the can
    wb_motor_set_position(motors[1], 1.55);
    wb_motor_set_position(motors[2], 1.12);
    open_gripper();
    passive_wait(2.0);

    // align gripper with the can
    wb_motor_set_position(motors[4], -1.09);
    passive_wait(2.0);

    // grasp the can
    close_gripper();
    passive_wait(0.5);

    // move the arm up
    wb_motor_set_position(motors[1], -0.92);
    passive_wait(0.3);
    wb_motor_set_position(motors[2], 1.88);
    wb_motor_set_position(motors[4], 1.5);
    passive_wait(2.0);

    // rotate the arm
    wb_motor_set_position(motors[0], -1.5708);
    passive_wait(1.0);

    // prepare for releasing the can
    wb_motor_set_position(motors[4], -1.04);
    passive_wait(1.0);
    wb_motor_set_position(motors[2], 1.12);
    wb_motor_set_position(motors[1], 1.53);
    passive_wait(2.0);

    // release the can
    open_gripper();
    passive_wait(0.5);

    // remove the arm
    wb_motor_set_position(motors[4], -1.95);
    passive_wait(1.0);

    // move the arm back to the origin position
    wb_motor_set_position(motors[1], 0.0);
    passive_wait(2.0);
    wb_motor_set_position(motors[0], 0.0);
    passive_wait(2.0);
  };

  wb_robot_cleanup();

  return 0;
}
