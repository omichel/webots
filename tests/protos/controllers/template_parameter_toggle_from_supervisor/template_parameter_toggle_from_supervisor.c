#include <webots/robot.h>
#include <webots/supervisor.h>
#include <webots/touch_sensor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);

  WbNodeRef template = wb_supervisor_node_get_from_def("TEMPLATE");
  ts_assert_pointer_not_null(template, "Unable to retrieve template node");

  WbFieldRef toggle = wb_supervisor_node_get_field(template, "physicsToggle");
  ts_assert_pointer_not_null(toggle, "Unable to retrieve physicsToggle field");

  wb_supervisor_field_set_sf_bool(toggle, true);
  ts_assert_boolean_equal(wb_supervisor_field_get_sf_bool(toggle), "Problem when setting physicsToggle");

  WbDeviceTag ts = wb_robot_get_device("touch sensor");
  wb_touch_sensor_enable(ts, TIME_STEP);
  wb_robot_step(TIME_STEP);

  bool success = false;
  int i;
  for (i = 0; i < 3000 / TIME_STEP; ++i) {
    wb_robot_step(TIME_STEP);
    if (wb_touch_sensor_get_value(ts) != 0) {
      success = true;
      break;
    }
  }

  ts_assert_boolean_equal(success, "No collision detected");

  // check the node id is still accessible
  // by reseting the node at its initial state
  wb_supervisor_field_set_sf_bool(toggle, false);
  WbFieldRef translation_field = wb_supervisor_node_get_field(template, "translation");
  ts_assert_pointer_not_null(translation_field, "Unable to retrieve the translation field after the node regeneration.");
  const double initial_translation[3] = {0.0, 1.0, 0.0};
  wb_supervisor_field_set_sf_vec3f(translation_field, initial_translation);

  wb_robot_step(TIME_STEP);

  const double *current_translation = wb_supervisor_field_get_sf_vec3f(translation_field);
  ts_assert_vec3_equal(current_translation[0], current_translation[1], current_translation[2], initial_translation[0],
                       initial_translation[1], initial_translation[2],
                       "Impossible to reset the procedural node to its initial set after a successful regeneration.");

  ts_send_success();
  return EXIT_SUCCESS;
}
