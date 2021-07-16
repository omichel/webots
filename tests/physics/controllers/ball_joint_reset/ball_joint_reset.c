#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

int main(int argc, char **argv) {
  ts_setup(argv[0]);

  const int time_step = wb_robot_get_basic_time_step();

  WbNodeRef end_point_node = wb_supervisor_node_get_from_def("SOLID_ENDPOINT");
  WbFieldRef translation_field = wb_supervisor_node_get_field(end_point_node, "translation");
  WbFieldRef rotation_field = wb_supervisor_node_get_field(end_point_node, "rotation");

  wb_robot_step(time_step);

  const double *translation = wb_supervisor_field_get_sf_vec3f(translation_field);
  const double *rotation = wb_supervisor_field_get_sf_rotation(rotation_field);

  // save initial position to compare afterwards
  double expected_translation[3];
  double expected_rotation[4];
  memcpy(&expected_translation, translation, 3 * sizeof(double));
  memcpy(&expected_rotation, rotation, 4 * sizeof(double));

  wb_robot_step(2 * time_step);

  wb_supervisor_simulation_reset();

  wb_robot_step(2 * time_step);

  const double *translation_after = wb_supervisor_field_get_sf_vec3f(translation_field);
  const double *rotation_after = wb_supervisor_field_get_sf_rotation(rotation_field);

  ts_assert_doubles_in_delta(3, translation_after, expected_translation, 1e-10,
                             "Translation of the endpoint changed after the reset but shouldn't.");
  ts_assert_doubles_in_delta(4, rotation_after, expected_rotation, 1e-10,
                             "Rotation of the endpoint changed after the reset but shouldn't.");

  wb_robot_step(2 * time_step);

  ts_send_success();
  return EXIT_SUCCESS;
}
