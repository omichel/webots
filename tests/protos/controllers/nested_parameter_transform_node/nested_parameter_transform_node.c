#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 64

int main(int argc, char **argv) {
  ts_setup(argv[0]);  // give the controller args

  WbNodeRef parent_node = wb_supervisor_node_get_from_def("PARENT");
  WbFieldRef translation = wb_supervisor_node_get_field(parent_node, "translation");

  // Test position update of node defined in nested PROTO parameter
  WbNodeRef parameter_node = wb_supervisor_node_get_from_def("PARAMETER");
  const double *parameter_position = wb_supervisor_node_get_position(parameter_node);
  ts_assert_vec3_in_delta(parameter_position[0], parameter_position[1], parameter_position[2], 0.45, 0.5, 0.03, 0.0001,
                          "Position of visible nested PROTO parameter is wrong.");

  // Test position update of node defined in a PROTO definition
  WbNodeRef test_node = wb_supervisor_node_get_from_proto_def(parent_node, "TEST_PROTO");
  wb_robot_step(TIME_STEP);
  const double *position_before = wb_supervisor_node_get_position(test_node);
  ts_assert_vec3_in_delta(position_before[0], position_before[1], position_before[2], 3.0, 0.0, 1.0, 0.0001,
                          "Initial position of deeply nested PROTO parameter is wrong.");

  const double new_position[3] = {0.0, 1.0, 0.0};
  wb_supervisor_field_set_sf_vec3f(translation, new_position);

  wb_robot_step(TIME_STEP);

  const double *position_after = wb_supervisor_node_get_position(test_node);
  ts_assert_vec3_in_delta(position_after[0], position_after[1], position_after[2], 2.0, 1.0, 1.0, 0.0001,
                          "Position of deeply nested PROTO parameter is wrong after top node position change.");

  ts_send_success();
  return EXIT_SUCCESS;
}
