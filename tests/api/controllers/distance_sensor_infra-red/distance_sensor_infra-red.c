#include <webots/distance_sensor.h>
#include <webots/pen.h>
#include <webots/robot.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[0]);
  WbDeviceTag pen0, pen1, ds0, ds1;
  double value = 0.0;

  ds0 = wb_robot_get_device("ds0");
  ds1 = wb_robot_get_device("ds1");
  wb_distance_sensor_enable(ds0, TIME_STEP);
  wb_distance_sensor_enable(ds1, TIME_STEP);

  pen0 = wb_robot_get_device("pen0");
  pen1 = wb_robot_get_device("pen1");
  wb_pen_set_ink_color(pen0, 0x00FF00, 0.3);
  wb_pen_set_ink_color(pen1, 0x00FF00, 0.3);
  wb_pen_write(pen0, false);
  wb_pen_write(pen1, false);

  if (ts_webots_major_version() < 7)
    wb_robot_step(TIME_STEP);

  // check ds0 type
  int type = wb_distance_sensor_get_type(ds0);
  ts_assert_int_equal(type, WB_DISTANCE_SENSOR_INFRA_RED, "Wrong type returned.");

  // check ds0 range
  double min_range = wb_distance_sensor_get_min_value(ds0) / 10000.0;  // cf. DistanceSensor.lookupTable
  double max_range = wb_distance_sensor_get_max_value(ds0) / 10000.0;  // cf. DistanceSensor.lookupTable
  ts_assert_double_equal(min_range, 0.0, "Minumum range returned is wrong (%lf instead of 0.0)", min_range);
  ts_assert_double_equal(max_range, 0.2, "Maximum range returned is wrong (%lf instead of 0.2)", max_range);

  // check ds0 lookup table
  int lookup_table_size = wb_distance_sensor_get_lookup_table_size(ds0);
  ts_assert_double_equal(lookup_table_size, 2, "Lookup table size returned is wrong (%d instead of 2)", lookup_table_size);
  const double *lookup_table = wb_distance_sensor_get_lookup_table(ds0);
  ts_assert_double_equal(lookup_table[3], 0.2, "Lookup table (index 3) returned is wrong (%lf instead of 0.2)",
                         lookup_table[3]);
  ts_assert_double_equal(lookup_table[4], 2000, "Lookup table (index 4) returned is wrong (%lf instead of 2000)",
                         lookup_table[4]);
  ts_assert_double_equal(lookup_table[5], 0, "Lookup table (index 5) returned is wrong (%lf instead of 0)", lookup_table[5]);

  // FIRST STEP - distance to texture
  wb_robot_step(TIME_STEP);
  wb_robot_step(TIME_STEP);

  // check ds0
  value = wb_distance_sensor_get_value(ds0);  // no texture
  ts_assert_double_in_delta(value, 1019.3725, 0.001,
                            "Distance sensor 'ds0' doesn't return the right distance when hitting an object without texture "
                            "(expected = %f, received = %f)",
                            1019.3725, value);

  // check ds1
  value = wb_distance_sensor_get_value(ds1);  // green texture
  ts_assert_double_in_delta(value, 1172.206506, 0.001,
                            "Distance sensor 'ds1' doesn't return the right distance when hitting an object with green texture "
                            " (expected = %f, received = %f)",
                            1172.206506, value);

  wb_pen_write(pen0, true);
  wb_pen_write(pen1, true);

  // SECOND STEP - distance to texture painted in green
  wb_robot_step(TIME_STEP);

  // check ds0
  value = wb_distance_sensor_get_value(ds0);
  ts_assert_double_in_delta(value, 1283.6893, 0.001,
                            "Distance sensor 'ds0' doesn't return the right distance when hitting a object without texture "
                            "painted green (expected = %f, received = %f)",
                            1283.6893, value);

  // check ds1
  value = wb_distance_sensor_get_value(ds1);
  ts_assert_double_in_delta(value, 1450.385, 0.001,
                            "Distance sensor 'ds1' doesn't return the right distance when hitting a object with green texture "
                            "painted green (expected = %f, received = %f)",
                            1450.385, value);

  wb_pen_set_ink_color(pen0, 0xCC33FF, 0.3);
  wb_pen_set_ink_color(pen1, 0xCC33FF, 0.3);

  // THIRD STEP - distance to texture painted in green and violet
  wb_robot_step(TIME_STEP);

  // check ds0
  value = wb_distance_sensor_get_value(ds0);
  ts_assert_double_in_delta(value, 1252.266, 0.001,
                            "Distance sensor 'ds0' doesn't return the right distance when hitting an object without texture "
                            "painted in violet (expected = %f, received = %f)",
                            1252.266, value);

  // check ds1
  value = wb_distance_sensor_get_value(ds1);
  ts_assert_double_in_delta(value, 1217.675, 0.001,
                            "Distance sensor 'ds1' doesn't return the right distance when hitting an object with green texture "
                            "painted in violet (expected = %f, received = %f)",
                            1217.675, value);

  wb_pen_set_ink_color(pen0, 0xFF0000, 0.7);
  wb_pen_set_ink_color(pen1, 0xFF0000, 0.7);

  // FORTH STEP - distance to texture painted in green, violet and red
  wb_robot_step(TIME_STEP);

  // check ds0
  value = wb_distance_sensor_get_value(ds0);
  ts_assert_double_in_delta(value, 1025.485, 0.001,
                            "Distance sensor 'ds0' doesn't return the right distance when hitting an object without texture "
                            "painted in red (expected = %f, received = %f)",
                            1025.485, value);

  // check ds1
  value = wb_distance_sensor_get_value(ds1);
  ts_assert_double_in_delta(value, 834.98, 0.001,
                            "Distance sensor 'ds1' doesn't return the right distance when hitting an object with green texture "
                            "painted in red (expected = %f, received = %f)",
                            834.98, value);

  ts_send_success();
  return EXIT_SUCCESS;
}
