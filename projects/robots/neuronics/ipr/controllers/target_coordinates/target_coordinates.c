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
 * Description:  Example to demonstrate the wb_supervisor_get_position()
 *               and wb_supervisor_get_orientation() functions.
 *               This Supervisor controller prints the coordinates
 *               of the gripper and of a target object (red box)
 *               and the distance between them.
 */

#include <math.h>
#include <stdio.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#define TIME_STEP 100

static void vec_show(int label, const char *msg, const double v[3]) {
  char str[128];
  sprintf(str, "%s %7.3f %7.3f %7.3f", msg, v[0], v[1], v[2]);
  wb_supervisor_set_label(label, str, 0.01, 0.01 + 0.05 * label, 0.1, 0x0000ff, 0.0, "Arial");
}

static void val_show(int label, const char *msg, double v) {
  char str[128];
  sprintf(str, "%s %7.3f", msg, v);
  wb_supervisor_set_label(label, str, 0.01, 0.01 + 0.05 * label, 0.1, 0xffffff, 0.0, "Arial");
}

// distance between 2 vectors
static double vec_dist(const double a[3], const double b[3]) {
  double d[3] = {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
  return sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
}

// vector addition
static void vec_add(double result[3], const double b[3]) {
  result[0] += b[0];
  result[1] += b[1];
  result[2] += b[2];
}

// matrix * vector multiplicarion: result = m * v
void vec_rotate(double result[3], const double m[9], const double v[3]) {
  result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
  result[1] = m[3] * v[0] + m[4] * v[1] + m[5] * v[2];
  result[2] = m[6] * v[0] + m[7] * v[1] + m[8] * v[2];
}

int main() {
  // initialize Webots
  wb_robot_init();

  // get handles to objects in the Scene Tree
  WbNodeRef target = wb_supervisor_node_get_from_def("TARGET");
  WbNodeRef gripper = wb_supervisor_node_get_from_def("WRIST");

  while (wb_robot_step(TIME_STEP) != -1) {
    // get current gripper pose in world coordinates
    const double *gripper_pose = wb_supervisor_node_get_pose(gripper, NULL);

    // transformation matrices are composable as is
    // however, we extract rotation and translation for demonstration purposes
    double gripper_rotation[9] = {gripper_pose[0], gripper_pose[1], gripper_pose[2], gripper_pose[4], gripper_pose[5],
                                  gripper_pose[6], gripper_pose[8], gripper_pose[9], gripper_pose[10]};
    double gripper_translation[3] = {gripper_pose[3], gripper_pose[7], gripper_pose[11]};

    // get current target pose in world coordinates
    const double *target_pose = wb_supervisor_node_get_pose(target, NULL);
    double target_translation[3] = {target_pose[3], target_pose[7], target_pose[11]};

    // center point of the gripper in local (WRIST) coordinates
    const double center[3] = {0, 0.18, 0};

    // change center point from WRIST to world coordinates

    double cpos[3];
    vec_rotate(cpos, gripper_rotation, center);
    vec_add(cpos, gripper_translation);

    // compute distance between gripper center and target
    const double distance = vec_dist(target_translation, cpos);

    // display info
    vec_show(0, "target:  ", target_translation);
    vec_show(1, "gripper: ", gripper_translation);
    val_show(2, "distance:", distance);
  }

  wb_robot_cleanup();
  return 0;
}
