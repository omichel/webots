#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  // let the time for the robot controller to copy the args in customData field
  wb_robot_step(5 * TIME_STEP);

  WbNodeRef robot = wb_supervisor_node_get_from_def("ROBOT");
  WbFieldRef custom_data_field = wb_supervisor_node_get_field(robot, "customData");
  WbFieldRef procedural_field = wb_supervisor_node_get_field(robot, "proceduralField");

  WbNodeRef texture = wb_supervisor_node_get_from_def("TEXTURE");
  ts_assert_pointer_not_null(texture, "Can't find TEXTURE node.");
  WbFieldRef repeats_field = wb_supervisor_node_get_field(texture, "repeatS");
  ts_assert_pointer_not_null(repeats_field, "Can't find 'repeatS' field of TEXTURE node.");

  WbNodeRef shape = wb_supervisor_node_get_from_def("SHAPE");
  ts_assert_pointer_not_null(shape, "Can't find SHAPE node.");

  WbNodeRef internal_node = wb_supervisor_node_get_from_proto_def(robot, "INTERNAL");
  ts_assert_pointer_not_null(internal_node, "Can't find INTERNAL node.");

  WbFieldRef internal_children_field = wb_supervisor_node_get_field(internal_node, "children");
  ts_assert_pointer_not_null(internal_children_field, "Can't find 'children' field of INTERNAL node.");

  const char *field_content = wb_supervisor_field_get_sf_string(custom_data_field);
  int id;
  sscanf(field_content, "%d", &id);

  ts_assert_int_equal(
    id, wb_supervisor_node_get_id(robot),
    "The 'id' returned by 'wb_supervisor_node_get_id' does not match the one received by the PROTO as 'context.id'.");

  wb_supervisor_field_set_sf_string(custom_data_field, "");
  wb_supervisor_field_set_sf_string(procedural_field, "");
  wb_robot_step(5 * TIME_STEP);

  const char *new_field_content = wb_supervisor_field_get_sf_string(custom_data_field);
  int new_id;
  sscanf(new_field_content, "%d", &new_id);

  ts_assert_int_equal(id, new_id, "The 'id' received by the PROTO as 'context.id' has changed when the PROTO was regenerated.");

  ts_assert_int_equal(
    new_id, wb_supervisor_node_get_id(robot),
    "The 'id' returned by 'wb_supervisor_node_get_id' does not match the one received by the PROTO as 'context.id'.");

  WbNodeRef root_node = wb_supervisor_node_get_root();
  ts_assert_int_equal(0, wb_supervisor_node_get_id(root_node), "The 'id' of the root node should be '0'.");

  // test that fields can be retrieved (for the first time) even after PROTO regeneration
  int field_type = wb_supervisor_field_get_type(internal_children_field);
  ts_assert_int_equal(field_type, WB_NO_FIELD,
                      "'children' field reference of INTERNAL node should be invalid after PROTO regeneration.");
  WbFieldRef internal_translation_field = wb_supervisor_node_get_field(internal_node, "translation");
  ts_assert_pointer_null(internal_translation_field, "INTERNAL node reference should be invalid after PROTO regeneration.");
  internal_node = wb_supervisor_node_get_from_proto_def(robot, "INTERNAL");
  ts_assert_pointer_not_null(internal_node, "Can't find INTERNAL node after PROTO regeneration.");
  internal_translation_field = wb_supervisor_node_get_field(internal_node, "translation");
  ts_assert_pointer_not_null(internal_translation_field, "Can't find 'translation' field of INTERNAL node.");
  field_type = wb_supervisor_field_get_type(internal_translation_field);
  ts_assert_int_equal(field_type, WB_SF_VEC3F, "'translation' field of INTERNAL node is invalid.");

  WbFieldRef root_children_field = wb_supervisor_node_get_field(root_node, "children");
  ts_assert_pointer_not_null(root_children_field, "Can't find 'children' field of root node.");

  WbFieldRef controller_field = wb_supervisor_node_get_field(robot, "controller");
  ts_assert_pointer_not_null(controller_field, "Can't find 'controller' field of ROBOT node.");

  WbFieldRef appearance_field = wb_supervisor_node_get_field(shape, "appearance");
  ts_assert_pointer_not_null(appearance_field, "Can't find 'appearance' field of SHAPE node.");

  WbFieldRef repeatt_field = wb_supervisor_node_get_field(texture, "repeatT");
  ts_assert_pointer_not_null(repeatt_field, "Can't find 'repeatS' field of TEXTURE node.");

  // test that fields can still be modified after PROTO regeneration

  wb_supervisor_field_set_sf_bool(repeats_field, false);
  wb_supervisor_field_set_sf_bool(repeatt_field, false);

  wb_robot_step(TIME_STEP);

  ts_assert_boolean_not_equal(wb_supervisor_field_get_sf_bool(repeats_field), "'repeatS' field has not been set.");
  ts_assert_boolean_not_equal(wb_supervisor_field_get_sf_bool(repeatt_field), "'repeatT' field has not been set.");

  ts_send_success();
  return EXIT_SUCCESS;
}
