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
   Description:   Demo for InertialUnit node
*/

#include <stdio.h>
#include <stdlib.h>
#include <webots/inertial_unit.h>
#include <webots/motor.h>
#include <webots/robot.h>

int main(int argc, const char *argv[]) {
  // initialize webots API
  wb_robot_init();

  int step = wb_robot_get_basic_time_step();

  WbDeviceTag imu = wb_robot_get_device("inertial unit");
  wb_inertial_unit_enable(imu, step);

  WbDeviceTag yaw_motor = wb_robot_get_device("yaw motor");
  WbDeviceTag pitch_motor = wb_robot_get_device("pitch motor");
  WbDeviceTag roll_motor = wb_robot_get_device("roll motor");

  int i;
  for (i = 0; true; i++) {
    // choose a random target
    double yaw = rand() / (double)RAND_MAX * 2.0 * M_PI - M_PI;
    double pitch = - (rand() / (double)RAND_MAX * 2.3 - 0.8);
    double roll = rand() / (double)RAND_MAX * 2.0 * M_PI - M_PI;

    printf("new target #%d: roll/pitch/yaw=%f %f %f\n", i, roll, pitch, yaw);

    // start moving arm to target
    wb_motor_set_position(yaw_motor, yaw);
    wb_motor_set_position(pitch_motor, pitch);
    wb_motor_set_position(roll_motor, roll);

    int j;
    for (j = 0; true; j++) {
      // execute a simulation step
      if (wb_robot_step(step) == -1)
        break;

      // read inertial unit values
      const double *rpy = wb_inertial_unit_get_roll_pitch_yaw(imu);

      // see if target position was reached
      if (fabs(rpy[0] - roll) < 0.01 && fabs(rpy[1] - pitch) < 0.01 && fabs(rpy[2] - yaw) < 0.01) {
        printf("reached target after %d simulation steps\n", j);
        break;
      }
    }
  }

  // cleanup webots resources
  wb_robot_cleanup();

  return 0;
}
