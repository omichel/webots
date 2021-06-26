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
 * Description: This controller shows how to employ coupled motors to control a gripper.
 */

#include <webots/motor.h>
#include <webots/robot.h>

#define TIME_STEP 16

int main() {
  wb_robot_init();

  const WbDeviceTag linear_motor = wb_robot_get_device("linear motor");
  // only the left finger of the gripper is controlled directly
  const WbDeviceTag motor = wb_robot_get_device("motor::left finger");

  while (1) {
    // delay
    for (int i = 0; i < 50; i++)
      wb_robot_step(TIME_STEP);

    // close the gripper, both sides will receive the command as the motors have the same name structure
    wb_motor_set_position(motor, 0.42);

    for (int i = 0; i < 50; i++)
      wb_robot_step(TIME_STEP);

    // climb the rod
    wb_motor_set_position(linear_motor, 0.14);
    wb_motor_set_velocity(linear_motor, 0.1);

    for (int i = 0; i < 100; i++)
      wb_robot_step(TIME_STEP);

    // open the gripper
    wb_motor_set_position(motor, 0);

    for (int i = 0; i < 50; i++)
      wb_robot_step(TIME_STEP);

    // descend the rod
    wb_motor_set_position(linear_motor, 0);

    for (int i = 0; i < 100; i++)
      wb_robot_step(TIME_STEP);
  }

  wb_robot_cleanup();
  return 0;
}
