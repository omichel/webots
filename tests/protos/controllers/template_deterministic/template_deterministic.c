#include <webots/distance_sensor.h>
#include <webots/robot.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);

  // get and enable the acc
  WbDeviceTag ds_deterministic1 = wb_robot_get_device("deterministic 1");
  WbDeviceTag ds_deterministic2 = wb_robot_get_device("deterministic 2");
  WbDeviceTag ds_non_deterministic1 = wb_robot_get_device("non deterministic 1");
  WbDeviceTag ds_non_deterministic2 = wb_robot_get_device("non deterministic 2");

  wb_distance_sensor_enable(ds_deterministic1, TIME_STEP);
  wb_distance_sensor_enable(ds_deterministic2, TIME_STEP);
  wb_distance_sensor_enable(ds_non_deterministic1, TIME_STEP);
  wb_distance_sensor_enable(ds_non_deterministic2, TIME_STEP);

  wb_robot_step(TIME_STEP);

  // check that both 'deterministic' PROTO nodes are equivalent
  ts_assert_double_equal(wb_distance_sensor_get_value(ds_deterministic1), wb_distance_sensor_get_value(ds_deterministic2),
                         "Deterministic procedural PROTO nodes should have the same size (%lf != %lf)",
                         wb_distance_sensor_get_value(ds_deterministic1), wb_distance_sensor_get_value(ds_deterministic2));

  // check that both 'non-deterministic' PROTO nodes are not equivalent
  ts_assert_double_not_equal(
    wb_distance_sensor_get_value(ds_non_deterministic1), wb_distance_sensor_get_value(ds_non_deterministic2),
    "Non-deterministic procedural PROTO nodes should have different sizes (%lf = %lf)",
    wb_distance_sensor_get_value(ds_non_deterministic1), wb_distance_sensor_get_value(ds_non_deterministic2));

  // Make sure deterministic and non-deterministic PROTO nodes are not equivalent
  ts_assert_double_not_equal(
    wb_distance_sensor_get_value(ds_deterministic1), wb_distance_sensor_get_value(ds_non_deterministic2),
    "Non-deterministic and deterministic procedural PROTO nodes should have different sizes (%lf = %lf)",
    wb_distance_sensor_get_value(ds_deterministic1), wb_distance_sensor_get_value(ds_non_deterministic2));

  ts_send_success();
  return EXIT_SUCCESS;
}
