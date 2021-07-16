#include <webots/distance_sensor.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#include <stdio.h>

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  int time_step = wb_robot_get_basic_time_step();

  WbDeviceTag ds_def = wb_robot_get_device("ds def");
  WbDeviceTag ds_use = wb_robot_get_device("ds use");
  wb_distance_sensor_enable(ds_def, time_step);
  wb_distance_sensor_enable(ds_use, time_step);

  // Regenerate procedural PROTO by modifying template field
  WbNodeRef template_node = wb_supervisor_node_get_from_def("TEST_PROTO");
  WbFieldRef template_field = wb_supervisor_node_get_field(template_node, "regenerate");
  wb_supervisor_field_set_sf_bool(template_field, true);

  wb_robot_step(time_step);

  // Check initial size of boxes
  ts_assert_double_in_delta(wb_distance_sensor_get_value(ds_def), 1000.0, 0.1, "Wrong initial size of DEF TEST_BOX.");
  ts_assert_double_in_delta(wb_distance_sensor_get_value(ds_use), 1000.0, 0.1, "Wrong initial size of USE TEST_BOX.");

  // Increase boxes size
  WbNodeRef box_node = wb_supervisor_node_get_from_def("TEST_BOX");

  WbFieldRef size_field = wb_supervisor_node_get_field(box_node, "size");
  const double new_size[3] = {0.1, 0.1, 0.5};
  wb_supervisor_field_set_sf_vec3f(size_field, new_size);

  wb_robot_step(time_step);

  // Check new boxes size
  ts_assert_double_in_delta(wb_distance_sensor_get_value(ds_def), 500.0, 0.1,
                            "Size of DEF TEST_BOX not updated after template regeneration.");
  ts_assert_double_in_delta(wb_distance_sensor_get_value(ds_use), 500.0, 0.1,
                            "Size of USE TEST_BOX not updated after template regeneration.");

  ts_send_success();
  return EXIT_SUCCESS;
}
