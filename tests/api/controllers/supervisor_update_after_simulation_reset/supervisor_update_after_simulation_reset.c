/*
 * Description:  Test Supervisor device API
 *               This file contains tests of reset simulation method
 */

#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[0]);

  WbNodeRef root_node = wb_supervisor_node_get_root();
  WbFieldRef root_children = wb_supervisor_node_get_field(root_node, "children");

  wb_robot_step(TIME_STEP);
  wb_supervisor_field_import_mf_node_from_string(root_children, -1, "DEF IMPORTED Solid {}");
  ts_assert_pointer_not_null(wb_supervisor_node_get_from_def("IMPORTED"), "Solid node not correctly imported.");
  const int field_count_before_reset = wb_supervisor_field_get_count(root_children);

  wb_supervisor_simulation_reset();
  wb_robot_step(TIME_STEP);

  // Simulation just reset
  // Check nodes and fields updated after reset
  ts_assert_pointer_null(wb_supervisor_node_get_from_def("IMPORTED"), "Solid node not correctly removed after reset.");
  ts_assert_int_equal(wb_supervisor_field_get_count(root_children), field_count_before_reset - 1,
                      "Root children field count not correctly updated after reset.");
  ts_assert_pointer_not_null(wb_supervisor_field_get_mf_node(root_children, -1),
                             "Root children field index -1 not correctly translated to a valid index.");

  ts_send_success();
  return EXIT_SUCCESS;
}
