#include <stdio.h>
#include <unistd.h>
#include <webots/distance_sensor.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  WbDeviceTag ds = wb_robot_get_device("ds");
  wb_distance_sensor_enable(ds, TIME_STEP);

  wb_robot_step(TIME_STEP);
  WbNodeRef node = wb_supervisor_node_get_from_def("TEST_NODE");
  WbFieldRef urlField = wb_supervisor_node_get_field(node, "url");
  const char *url = wb_supervisor_field_get_mf_string(urlField, 0);
  const double ds_value_grey = wb_distance_sensor_get_value(ds);
  ts_assert_double_in_delta(ds_value_grey, 277, 20.0,
                            "Wrong distance sensor value with \"%s\" texture: expecting 277, received %g.", url, ds_value_grey);
  wb_supervisor_field_set_mf_string(urlField, 0, "textures/green.jpg");

  wb_robot_step(TIME_STEP);

  // test appearance after regeneration
  url = wb_supervisor_field_get_mf_string(urlField, 0);
  const double ds_value_green = wb_distance_sensor_get_value(ds);
  ts_assert_double_in_delta(ds_value_green, 381, 20.0,
                            "Wrong distance sensor value with \"%s\" texture after regeneration: expecting 381, received %g.",
                            url, ds_value_green);

  wb_robot_step(TIME_STEP);

  // node reference changed due to PROTO regeneration
  WbFieldRef filteringField = wb_supervisor_node_get_field(node, "filtering");
  ts_assert_pointer_not_null(filteringField, "Invalid 'filtering' field after PROTO regeneration.");
  wb_supervisor_field_set_sf_int32(filteringField, -2);
  // webots should not crash after changing the regenerated node again

  wb_robot_step(TIME_STEP);

  // if set "filtering" was executed, the field has been reset to default value 4
  const int filteringValue = wb_supervisor_field_get_sf_int32(filteringField);
  ts_assert_int_equal(filteringValue, 4, "Wrong filtering value after setting to an invalid value.");

  ts_send_success();
  return EXIT_SUCCESS;
}
