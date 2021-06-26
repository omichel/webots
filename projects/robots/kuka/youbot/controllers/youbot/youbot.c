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
 * Description:   Starts with a predefined behaviors and then
 *                read the user keyboard inputs to actuate the
 *                robot
 */

#include <webots/keyboard.h>
#include <webots/robot.h>

#include <arm.h>
#include <base.h>
#include <gripper.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME_STEP 32

static void step() {
  if (wb_robot_step(TIME_STEP) == -1) {
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

static void automatic_behavior() {
  passive_wait(2.0);
  gripper_release();
  arm_set_height(ARM_FRONT_CARDBOARD_BOX);
  passive_wait(4.0);
  gripper_grip();
  passive_wait(1.0);
  arm_set_height(ARM_BACK_PLATE_LOW);
  passive_wait(3.0);
  gripper_release();
  passive_wait(1.0);
  arm_reset();
  base_strafe_left();
  passive_wait(5.0);
  gripper_grip();
  base_reset();
  passive_wait(1.0);
  base_turn_left();
  passive_wait(1.0);
  base_reset();
  gripper_release();
  arm_set_height(ARM_BACK_PLATE_LOW);
  passive_wait(3.0);
  gripper_grip();
  passive_wait(1.0);
  arm_set_height(ARM_RESET);
  passive_wait(2.0);
  arm_set_height(ARM_FRONT_PLATE);
  arm_set_orientation(ARM_RIGHT);
  passive_wait(4.0);
  arm_set_height(ARM_FRONT_FLOOR);
  passive_wait(2.0);
  gripper_release();
  passive_wait(1.0);
  arm_set_height(ARM_FRONT_PLATE);
  passive_wait(2.0);
  arm_set_height(ARM_RESET);
  passive_wait(2.0);
  arm_reset();
  gripper_grip();
  passive_wait(2.0);
}

static void display_helper_message() {
  printf("\n \nControl commands:\n");
  printf(" Arrows:         Move the robot\n");
  printf(" Page Up/Down:   Rotate the robot\n");
  printf(" +/-:            (Un)grip\n");
  printf(" Shift + arrows: Handle the arm\n");
  printf(" Space:          Reset\n");
}

int main(int argc, char **argv) {
  wb_robot_init();

  base_init();
  arm_init();
  gripper_init();
  passive_wait(2.0);

  if (argc > 1 && strcmp(argv[1], "demo") == 0)
    automatic_behavior();

  display_helper_message();

  int pc = 0;
  wb_keyboard_enable(TIME_STEP);

  while (true) {
    step();

    int c = wb_keyboard_get_key();
    if ((c >= 0) && c != pc) {
      switch (c) {
        case WB_KEYBOARD_UP:
          base_forwards_increment();
          break;
        case WB_KEYBOARD_DOWN:
          base_backwards_increment();
          break;
        case WB_KEYBOARD_LEFT:
          base_strafe_left_increment();
          break;
        case WB_KEYBOARD_RIGHT:
          base_strafe_right_increment();
          break;
        case WB_KEYBOARD_PAGEUP:
          base_turn_left_increment();
          break;
        case WB_KEYBOARD_PAGEDOWN:
          base_turn_right_increment();
          break;
        case WB_KEYBOARD_END:
        case ' ':
          printf("Reset\n");
          base_reset();
          arm_reset();
          break;
        case '+':
        case 388:
        case 65585:
          printf("Grip\n");
          gripper_grip();
          break;
        case '-':
        case 390:
          printf("Ungrip\n");
          gripper_release();
          break;
        case 332:
        case WB_KEYBOARD_UP | WB_KEYBOARD_SHIFT:
          printf("Increase arm height\n");
          arm_increase_height();
          break;
        case 326:
        case WB_KEYBOARD_DOWN | WB_KEYBOARD_SHIFT:
          printf("Decrease arm height\n");
          arm_decrease_height();
          break;
        case 330:
        case WB_KEYBOARD_RIGHT | WB_KEYBOARD_SHIFT:
          printf("Increase arm orientation\n");
          arm_increase_orientation();
          break;
        case 328:
        case WB_KEYBOARD_LEFT | WB_KEYBOARD_SHIFT:
          printf("Decrease arm orientation\n");
          arm_decrease_orientation();
          break;
        default:
          fprintf(stderr, "Wrong keyboard input\n");
          break;
      }
    }
    pc = c;
  }

  wb_robot_cleanup();

  return 0;
}
