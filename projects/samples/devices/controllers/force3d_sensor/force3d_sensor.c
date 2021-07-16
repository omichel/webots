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
 * Description:   Example of use of a "force-3d" TouchSensor

Instructions:

This demo is intentionally very simple:
The transparent box is a "force-3d" TouchSensor. The opaque box in the
center of the transparent one is a Robot node. The TouchSensor is the
child of the Robot node (Scene tree). This simple setup allows to measure
the force on all (six) sides of the TouchSensor.

To do:

Rotate the box to see how the measured force vector changes with the
orientation of the box. To rotate the box use the right mouse button
and the shift key. Release and re-press the shift key to change the
rotation axis.

Results:

The "force-3d" TouchSensor measures the force in its own coordinate system.
When it is selected, the TouchSensor shows its coordinate system as
small red/green/blue lines, that correspond respectively to the x/y/z-axes.
For example in the initial configuration the box's y-axis (green line) is
oriented upwards. When the simulation runs, the vector of the force applied
by the floor to the TouchSensor is also oriented upwards. Hence in the initial
orientation the TouchSensor measures a force on its y-axis.

Magnitude:

The magnitude of the measured force is 78.48 Newtons,
this matches very accurately the theory:

78.48 [N] = (0.2)^3 [m^3] * 1000 [kg/m^3] * 9.81 [m/s^2]

Where 0.2 is the dimension of one side of the robot's box, and hence the
robot's box is 0.008 m^3, 1000 is the density of the robot (Physics.density)
in kg/m^3 and 9.81 is the gravitation acceleration (WorldInfo.gravity) in m/s^2.
Note that a TouchSensor measures the force applied between its body and the
its parent's body. Hence in this case only the weight of the Robot is measured,
not the weight of the TouchSensor itself.

*/

#include <stdio.h>
#include <webots/robot.h>
#include <webots/touch_sensor.h>

#define TIME_STEP 8

int main(int argc, char **argv) {
  // necessary to initialize webots
  wb_robot_init();

  // find and enable the TouchSensor
  WbDeviceTag touch = wb_robot_get_device("touch");
  wb_touch_sensor_enable(touch, TIME_STEP);

  // main loop
  while (wb_robot_step(TIME_STEP) != -1) {
    // read the value (in Newtons)
    const double *f = wb_touch_sensor_get_values(touch);
    printf("force vector: %8.2f %8.2f %8.2f\n", f[0], f[1], f[2]);
  }

  // Necessary to cleanup webots stuff
  wb_robot_cleanup();

  return 0;
}
