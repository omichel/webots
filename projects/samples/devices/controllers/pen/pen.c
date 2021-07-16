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
 * Description:  An example of a controller using a pen device which you can
 *               turn on and off using the defined keys.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <webots/distance_sensor.h>
#include <webots/keyboard.h>
#include <webots/motor.h>
#include <webots/pen.h>
#include <webots/robot.h>

#define SPEED 10
#define TIME_STEP 64

static double clamp(double value, double min, double max) {
  if (min > max) {
    assert(0);
    return value;
  }
  return value < min ? min : value > max ? max : value;
}

int main() {
  WbDeviceTag ds0, ds1, pen, left_motor, right_motor;
  int color_counter;
  int left_speed, right_speed;
  double max_speed;
  double ink_intensity;
  int color, red, green, blue;

  wb_robot_init();

  srand(123456789);  // arbitrary value that results in a pretty looking trail

  /* get a handler to the distance sensors. */
  ds0 = wb_robot_get_device("ds0");
  ds1 = wb_robot_get_device("ds1");

  /* and to the pen too. */
  pen = wb_robot_get_device("pen");

  wb_distance_sensor_enable(ds0, TIME_STEP);
  wb_distance_sensor_enable(ds1, TIME_STEP);

  wb_keyboard_enable(TIME_STEP);

  /* get a handler to the motors and set target position to infinity (speed control). */
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
  max_speed = wb_motor_get_max_velocity(left_motor);

  color_counter = 0;

  printf("You can switch the pen OFF by pressing the X key "
         "and ON by pressing the Y key\n");

  while (wb_robot_step(TIME_STEP) != -1) {
    switch (wb_keyboard_get_key()) {
      case 'Y':
        wb_pen_write(pen, 1);
        color_counter = 0;
        break;
      case 'X':
        wb_pen_write(pen, 0);
        color_counter = -1;
        break;
      default:
        break;
    }

    /*
     * We simply select randomly all the parameters of the ink and change them
     * after that the counter has come back to 0.
     */
    if (color_counter == 0) {
      ink_intensity = ((double)rand() / RAND_MAX);

      red = rand() & 0xff;
      green = rand() & 0xff;
      blue = rand() & 0xff;
      color = (((red << 8) | green) << 8) | blue;

      wb_pen_set_ink_color(pen, color, ink_intensity);
      color_counter = 10;
    } else {
      color_counter--;
    }

    const double ds0_value = wb_distance_sensor_get_value(ds0);
    const double ds1_value = wb_distance_sensor_get_value(ds1);

    if (ds1_value > 500) {
      /*
       * If both distance sensors are detecting something, this means that
       * we are facing a wall. In this case we need to move backwards.
       */
      if (ds0_value > 200) {
        left_speed = -SPEED;
        right_speed = -SPEED / 2;
      } else {
        /*
         * We turn proportionally to the sensors value because the
         * closer we are from the wall, the more we need to turn.
         */
        left_speed = -ds1_value / 50;
        right_speed = ds0_value / 50;
        left_speed = clamp(left_speed, -max_speed, max_speed);
        right_speed = clamp(right_speed, -max_speed, max_speed);
      }
    } else if (ds0_value > 500) {
      left_speed = ds1_value / 50;
      right_speed = -ds0_value / 50;
      left_speed = clamp(left_speed, -max_speed, max_speed);
      right_speed = clamp(right_speed, -max_speed, max_speed);
    } else {
      /*
       * If nothing was detected we can move forward at maximal speed.
       */
      left_speed = SPEED;
      right_speed = SPEED;
    }

    /* Set the motor speeds. */
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
  }

  wb_robot_cleanup();

  return 0;
}
