#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  WbNodeRef solid_node = wb_supervisor_node_get_from_def("SOLID");
  WbNodeRef viewpoint_node = wb_supervisor_node_get_from_def("VIEWPOINT");

  ts_assert_pointer_not_null(solid_node, "Solid node reference not found");
  ts_assert_pointer_not_null(viewpoint_node, "Viewpoint node reference not found");

  WbFieldRef viewpoint_field_of_view_field = wb_supervisor_node_get_field(viewpoint_node, "fieldOfView");
  WbFieldRef viewpoint_position_field = wb_supervisor_node_get_field(viewpoint_node, "position");
  WbFieldRef solid_translation = wb_supervisor_node_get_field(solid_node, "translation");

  ts_assert_pointer_not_null(viewpoint_field_of_view_field, "Viewpoint 'fieldOfView' field not found");
  ts_assert_pointer_not_null(viewpoint_position_field, "Viewpoint 'position' field not found");
  ts_assert_pointer_not_null(solid_translation, "Solid 'translation' field not found");

  double viewpoint_initial_height = wb_supervisor_field_get_sf_vec3f(viewpoint_position_field)[1];
  double solid_initial_height = wb_supervisor_field_get_sf_vec3f(solid_translation)[1];

  ts_assert_double_equal(viewpoint_initial_height, solid_initial_height,
                         "Viewpoint and Solid should have the same initial height.");

  wb_robot_step(10 * TIME_STEP);

  double viewpoint_height = wb_supervisor_field_get_sf_vec3f(viewpoint_position_field)[1];
  double solid_height = wb_supervisor_field_get_sf_vec3f(solid_translation)[1];

  ts_assert_double_equal(viewpoint_height, solid_height,
                         "Viewpoint and Solid should still have the same height after a few steps.");

  wb_supervisor_field_set_sf_float(viewpoint_field_of_view_field, 0.78);

  wb_robot_step(10 * TIME_STEP);

  viewpoint_height = wb_supervisor_field_get_sf_vec3f(viewpoint_position_field)[1];
  solid_height = wb_supervisor_field_get_sf_vec3f(solid_translation)[1];
  ts_assert_double_equal(viewpoint_height, solid_height,
                         "Viewpoint and Solid should still have the same height after viewpoint regeneration.");

  ts_send_success();

  return EXIT_SUCCESS;
}
