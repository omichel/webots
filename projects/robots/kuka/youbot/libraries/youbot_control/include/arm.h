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
 * Description:   Allows to handle the arm
 */

#ifndef ARM_H
#define ARM_H

#ifdef __cplusplus
extern "C" {
#endif

void arm_init();

void arm_reset();

enum Height {
  ARM_BACK_PLATE_LOW,
  ARM_BACK_PLATE_HIGH,
  ARM_RESET,
  ARM_FRONT_CARDBOARD_BOX,
  ARM_HANOI_PREPARE,
  ARM_FRONT_PLATE,
  ARM_FRONT_FLOOR,
  ARM_MAX_HEIGHT
};
void arm_set_height(enum Height height);
void arm_increase_height();
void arm_decrease_height();

enum Orientation {
  ARM_BACK_RIGHT,
  ARM_RIGHT,
  ARM_FRONT_RIGHT,
  ARM_FRONT,
  ARM_FRONT_LEFT,
  ARM_LEFT,
  ARM_BACK_LEFT,
  ARM_MAX_SIDE
};
void arm_set_orientation(enum Orientation orientation);
void arm_increase_orientation();
void arm_decrease_orientation();

enum Arm { ARM1, ARM2, ARM3, ARM4, ARM5 };
void arm_set_sub_arm_rotation(enum Arm arm, double radian);
double arm_get_sub_arm_length(enum Arm arm);

void arm_ik(double x, double y, double z);

#ifdef __cplusplus
}
#endif

#endif
