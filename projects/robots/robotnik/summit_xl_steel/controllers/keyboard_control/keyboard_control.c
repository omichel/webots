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
 * Description:  This controller is used to move the mecanum-wheeled robot Summit-XL Steel in an industrial environment
 *               using the keyboard. The keys are the following:
 *
 *               vx: ↑/↓
 *               vy: ←/→
 *               ω: Page Up/Page Down
 *               Reset: Space bar
 */

#include <math.h>
#include <stdio.h>
#include <webots/keyboard.h>
#include <webots/motor.h>
#include <webots/robot.h>

#define TIME_STEP 32
#define WHEEL_RADIUS 0.127
#define LX 0.2045  // lateral distance from robot's COM to wheel [m].
#define LY 0.2225  // longitudinal distance from robot's COM to wheel [m].
#define SPEED_INCREMENT 0.1
#define MAX_SPEED 1.5

int main() {
  wb_robot_init();

  WbDeviceTag motor_fl = wb_robot_get_device("front_left_wheel_joint");
  WbDeviceTag motor_fr = wb_robot_get_device("front_right_wheel_joint");
  WbDeviceTag motor_bl = wb_robot_get_device("back_left_wheel_joint");
  WbDeviceTag motor_br = wb_robot_get_device("back_right_wheel_joint");

  // Velocity control, so position must be set to infinity.
  wb_motor_set_position(motor_fl, INFINITY);
  wb_motor_set_position(motor_fr, INFINITY);
  wb_motor_set_position(motor_bl, INFINITY);
  wb_motor_set_position(motor_br, INFINITY);

  wb_motor_set_velocity(motor_fl, 0.0);
  wb_motor_set_velocity(motor_fr, 0.0);
  wb_motor_set_velocity(motor_bl, 0.0);
  wb_motor_set_velocity(motor_br, 0.0);

  double target_speed[3] = {0.0, 0.0, 0.0};      // vx [m/s], vy [m/s], ω [rad/s].
  int speed_id = 0;                              // index to select either vx, vy, ω.
  int sign;                                      // sign of the increment (decrement if -1).
  double motor_speed[4] = {0.0, 0.0, 0.0, 0.0};  // wheels speed in [m/s], computed from vx, vy and ω.

  wb_keyboard_enable(TIME_STEP);

  printf("To move the Summit-XL Steel with your keyboard, click first inside the simulation window and press:\n \
  vx   : ↑/↓               \n \
  vy   : ←/→               \n \
  ω    : Page Up/Page Down \n \
  Reset: Space bar         \n");

  while (wb_robot_step(TIME_STEP) != -1) {
    int key = wb_keyboard_get_key();
    bool is_key_valid = 1;
    switch (key) {
      case WB_KEYBOARD_UP:
        speed_id = 0;
        sign = 1;
        break;

      case WB_KEYBOARD_DOWN:
        speed_id = 0;
        sign = -1;
        break;

      case WB_KEYBOARD_LEFT:
        speed_id = 1;
        sign = 1;
        break;

      case WB_KEYBOARD_RIGHT:
        speed_id = 1;
        sign = -1;
        break;

      case WB_KEYBOARD_PAGEUP:
        speed_id = 2;
        sign = 1;
        break;

      case WB_KEYBOARD_PAGEDOWN:
        speed_id = 2;
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
      if (sign > 0) {
        target_speed[speed_id] += SPEED_INCREMENT;
        if (target_speed[speed_id] > MAX_SPEED)
          target_speed[speed_id] = MAX_SPEED;
      } else if (sign < 0) {
        target_speed[speed_id] -= SPEED_INCREMENT;
        if (target_speed[speed_id] < -MAX_SPEED)
          target_speed[speed_id] = -MAX_SPEED;
      } else {
        for (int i = 0; i < 3; ++i)
          target_speed[i] = 0;
      }
      printf("vx:%.2f[m/s] vy:%.2f[m/s] ω:%.2f[rad/s]\n", target_speed[0], target_speed[1], target_speed[2]);

      // Computes the wheel motors speeds from vx, vy and ω.
      motor_speed[0] = 1 / WHEEL_RADIUS * (target_speed[0] - target_speed[1] - (LX + LY) * target_speed[2]);
      motor_speed[1] = 1 / WHEEL_RADIUS * (target_speed[0] + target_speed[1] + (LX + LY) * target_speed[2]);
      motor_speed[2] = 1 / WHEEL_RADIUS * (target_speed[0] + target_speed[1] - (LX + LY) * target_speed[2]);
      motor_speed[3] = 1 / WHEEL_RADIUS * (target_speed[0] - target_speed[1] + (LX + LY) * target_speed[2]);

      wb_motor_set_velocity(motor_fl, motor_speed[0]);
      wb_motor_set_velocity(motor_fr, motor_speed[1]);
      wb_motor_set_velocity(motor_bl, motor_speed[2]);
      wb_motor_set_velocity(motor_br, motor_speed[3]);
    }
  }

  wb_robot_cleanup();
  return 0;
}
