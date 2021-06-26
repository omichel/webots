#include <webots/gps.h>
#include <webots/motor.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#include <stdlib.h>

#define MAX_SPEED 10.0
#define TIME_STEP 16

static WbDeviceTag left_motor;
static WbDeviceTag right_motor;

static double double_rand(double min, double max) {
  const double scale = rand() / (double)RAND_MAX;
  return min + scale * (max - min);
}

static void move_randomly(double time) {
  const double start_time = wb_robot_get_time();
  while (start_time + 1.0 > wb_robot_get_time()) {
    wb_robot_step(TIME_STEP);
    wb_motor_set_velocity(left_motor, double_rand(-MAX_SPEED, MAX_SPEED));
    wb_motor_set_velocity(right_motor, double_rand(-MAX_SPEED, MAX_SPEED));
  }
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);
  wb_robot_step(TIME_STEP);
}

int main(int argc, char **argv) {
  ts_setup(argv[0]);

  // Get motors in velocity mode.
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);

  // Get devices
  WbDeviceTag body_gps = wb_robot_get_device("body_gps");
  wb_gps_enable(body_gps, TIME_STEP);
  WbDeviceTag right_wheel_gps = wb_robot_get_device("right_wheel_gps");
  wb_gps_enable(right_wheel_gps, TIME_STEP);
  WbDeviceTag left_wheel_gps = wb_robot_get_device("left_wheel_gps");
  wb_gps_enable(left_wheel_gps, TIME_STEP);

  // Get fields.
  WbNodeRef robot_node = wb_supervisor_node_get_self();
  WbFieldRef translation_field = wb_supervisor_node_get_field(robot_node, "translation");
  WbFieldRef rotation_field = wb_supervisor_node_get_field(robot_node, "rotation");

  double translation[] = {0.0, 0.0, 0.0};
  double rotation[] = {0.0, 1.0, 0.0, 0.0};

  // Move the robot at multiple locations while moving it.
  int i;
  for (i = 0; i < 50; ++i) {
    translation[0] = double_rand(-0.4, 0.4);
    translation[2] = double_rand(-0.4, 0.4);
    rotation[3] = double_rand(-M_PI, M_PI);
    wb_supervisor_field_set_sf_vec3f(translation_field, translation);
    wb_supervisor_field_set_sf_rotation(rotation_field, rotation);
    wb_supervisor_node_reset_physics(robot_node);
    move_randomly(1.0);
  }

  // Reset the robot at the origin.
  translation[0] = 0.0;
  translation[2] = 0.0;
  rotation[3] = 0.0;
  wb_supervisor_field_set_sf_vec3f(translation_field, translation);
  wb_supervisor_field_set_sf_rotation(rotation_field, rotation);
  wb_supervisor_node_reset_physics(robot_node);

  wb_robot_step(TIME_STEP);      // Apply the reset
  wb_robot_step(2 * TIME_STEP);  // Run some steps to the stabilize the robot.

  // Check GPS feedback is correct.
  const double *body_position = wb_gps_get_values(body_gps);
  const double *left_wheel_position = wb_gps_get_values(left_wheel_gps);
  const double *right_wheel_position = wb_gps_get_values(right_wheel_gps);

  const double body_expectations[] = {0.0, 0.0, 0.0};
  const double left_wheel_expectations[] = {-0.045, 0.025, 0.0};
  const double right_wheel_expectations[] = {0.045, 0.025, 0.0};

  ts_assert_doubles_in_delta(3, body_position, body_expectations, 0.002,
                             "Body position is not at the expected location (found %f %f %f)", body_position[0],
                             body_position[1], body_position[2]);
  ts_assert_doubles_in_delta(3, left_wheel_position, left_wheel_expectations, 0.002,
                             "Left wheel position is not at the expected location (found %f %f %f)", left_wheel_position[0],
                             left_wheel_position[1], left_wheel_position[2]);
  ts_assert_doubles_in_delta(3, right_wheel_position, right_wheel_expectations, 0.002,
                             "Right wheel position is not at the expected location (found %f %f %f)", right_wheel_position[0],
                             right_wheel_position[1], right_wheel_position[2]);

  ts_send_success();
  return EXIT_SUCCESS;
}
