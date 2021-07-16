#include <stdio.h>
#include <webots/distance_sensor.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  WbNodeRef protoNode = wb_supervisor_node_get_from_def("TEST_NODE");
  WbFieldRef sizeField = wb_supervisor_node_get_field(protoNode, "size");
  WbDeviceTag ds1 = wb_robot_get_device("ds1");
  WbDeviceTag ds2 = wb_robot_get_device("ds2");
  wb_distance_sensor_enable(ds1, TIME_STEP);
  wb_distance_sensor_enable(ds2, TIME_STEP);

  wb_robot_step(2 * TIME_STEP);
  wb_robot_step(2 * TIME_STEP);

  double value = wb_distance_sensor_get_value(ds1);
  ts_assert_double_is_bigger(500.0, value, "The initial sphere of radius 0.15 is not correctly created.");
  value = wb_distance_sensor_get_value(ds2);
  ts_assert_double_is_bigger(value, 990.0, "The initial sphere has wrong radius.");

  wb_supervisor_field_set_sf_float(sizeField, 0.2);

  wb_robot_step(2 * TIME_STEP);

  value = wb_distance_sensor_get_value(ds1);
  ts_assert_double_is_bigger(500.0, value, "The sphere radius after regeneration is smaller than the initial value.");
  value = wb_distance_sensor_get_value(ds2);
  ts_assert_double_is_bigger(500.0, value, "The sphere radius after regeneration has wrong radius.");

  ts_send_success();
  return EXIT_SUCCESS;
}
