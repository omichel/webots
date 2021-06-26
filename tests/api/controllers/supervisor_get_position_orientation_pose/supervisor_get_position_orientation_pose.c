#include <webots/supervisor.h>
#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[0]);

  WbNodeRef node = wb_supervisor_node_get_from_def("NODE");
  ts_assert_pointer_not_null(node, "wb_supervisor_node_get_from_def(\"NODE\") failed");

  WbNodeRef node2 = wb_supervisor_node_get_from_def("ROBOT.JOINT.SOLID.NODE");
  ts_assert_pointer_not_null(node2, "wb_supervisor_node_get_from_def(\"ROBOT.JOINT.SOLID.NODE\") failed");

  WbNodeRef node_parent = wb_supervisor_node_get_from_def("ROBOT.JOINT.SOLID");
  ts_assert_pointer_not_null(node_parent, "wb_supervisor_node_get_from_def(\"ROBOT.JOINT.SOLID\") failed");

  if (node != node2)
    ts_assert_boolean_equal(
      false,
      "wb_supervisor_node_get_from_def(\"ROBOT.JOINT.SOLID.NODE\") failed, reference to node and node2 should be equal.");

  const double *position = wb_supervisor_node_get_position(node);
  const double POSITION[] = {0.1, -0.2, 0.3};

  ts_assert_doubles_in_delta(3, position, POSITION, 0.000001,
                             "wb_supervisor_node_get_position() did not return the expected values.");

  const double *orientation = wb_supervisor_node_get_orientation(node);
  const double ROTATION[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};

  ts_assert_doubles_in_delta(9, orientation, ROTATION, 0.0001,
                             "wb_supervisor_node_get_orientation() did not return the expected values.");

  // change node scale and check that `wb_supervisor_node_get_orientation` still returns the unscaled matrix
  WbFieldRef scale_field = wb_supervisor_node_get_field(node, "scale");
  const double new_scale[] = {2.0, 3.0, 4.0};
  wb_supervisor_field_set_sf_vec3f(scale_field, new_scale);

  wb_robot_step(TIME_STEP);

  orientation = wb_supervisor_node_get_orientation(node);
  ts_assert_doubles_in_delta(9, orientation, ROTATION, 0.0001,
                             "wb_supervisor_node_get_orientation() did not return the expected values for scaled node.");

  const double *pose = wb_supervisor_node_get_pose(node, node_parent);
  const double POSE[] = {-1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.3, 0.0, 0.0, 0.0, 1.0};

  ts_assert_doubles_in_delta(16, pose, POSE, 0.0001, "wb_supervisor_node_get_pose() did not return the expected values.");

  // supervisor pose tracking
  wb_supervisor_node_enable_pose_tracking(node, TIME_STEP, node_parent);

  for (int i = 0; i < 10; i++) {
    wb_robot_step(TIME_STEP);
    const double *tracked_pose = wb_supervisor_node_get_pose(node, node_parent);
    ts_assert_doubles_in_delta(16, POSE, tracked_pose, 0.0001,
                               "wb_supervisor_node_get_pose() did not return the expected values.");
  }
  wb_supervisor_node_disable_pose_tracking(node, node_parent);

  ts_send_success();
  return EXIT_SUCCESS;
}
