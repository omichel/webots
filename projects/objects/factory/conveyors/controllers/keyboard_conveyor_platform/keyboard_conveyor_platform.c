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
 * Description:  This controller is used to move the ConveyorPlatform belt using the keyboard.
 *               The keys are the following:
 *
 *               Belt: +/-
 *               Reset: Space bar
 */

#include <math.h>
#include <stdio.h>
#include <webots/keyboard.h>
#include <webots/motor.h>
#include <webots/robot.h>

#define TIME_STEP 32
#define SPEED_INCREMENT 0.1
#define MAX_SPEED 0.6

int main() {
  wb_robot_init();

  WbDeviceTag motor_belt = wb_robot_get_device("belt_motor");

  // Velocity control, so position must be set to infinity.
  wb_motor_set_position(motor_belt, INFINITY);
  wb_motor_set_velocity(motor_belt, 0.0);

  double target_belt_speed = 0.0;  // in [m/s].
  int sign;                        // sign of the increment (decrement if -1).

  wb_keyboard_enable(TIME_STEP);

  printf("To move the ConveyorPlatform with your keyboard, click first inside the simulation window and press: \n \
  Belt : +/-       \n \
  Reset: Space bar \n");

  while (wb_robot_step(TIME_STEP) != -1) {
    int key = wb_keyboard_get_key();
    bool is_key_valid = 1;
    switch (key) {
      case '+':
        sign = 1;
        break;

      case '-':
        sign = -1;
        break;

      case ' ':
        sign = 0;
        break;

      default:
        is_key_valid = 0;
        sign = 0;
    }

    if (is_key_valid) {
      // Increase or decrease target speed, depending on the sign.
      target_belt_speed += sign * SPEED_INCREMENT;
      if (sign > 0) {
        if (target_belt_speed > MAX_SPEED)
          target_belt_speed = MAX_SPEED;
      } else if (sign < 0) {
        if (target_belt_speed < -MAX_SPEED)
          target_belt_speed = -MAX_SPEED;
      } else
        target_belt_speed = 0.0;
      printf("belt speed: %.1f [m/s]\n", target_belt_speed);

      wb_motor_set_velocity(motor_belt, target_belt_speed);
    }
  }

  wb_robot_cleanup();
  return 0;
}
