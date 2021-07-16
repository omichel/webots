#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  WbDeviceTag motor = wb_robot_get_device("linear motor");
  wb_motor_set_position(motor, INFINITY);
  wb_motor_set_velocity(motor, 0.1);

  WbNodeRef node = wb_supervisor_node_get_self();
  WbFieldRef reverted_field = wb_supervisor_node_get_field(node, "isRegenerated");
  bool is_regenerated = wb_supervisor_field_get_sf_bool(reverted_field);

  node = wb_supervisor_node_get_from_def("VIEWPOINT");
  WbFieldRef position_field = wb_supervisor_node_get_field(node, "position");
  const double *positionBefore = wb_supervisor_field_get_sf_vec3f(position_field);
  double expected_z = positionBefore[2];

  wb_robot_step(4 * TIME_STEP);

  // check if viewpoint moved
  const char *message;
  if (is_regenerated) {
    message = "viewpoint doesn't follow the solid after PROTO regeneration.";
  } else {
    message = "viewpoint doesn't follow the solid.";
  }

  const double *positionAfter = wb_supervisor_field_get_sf_vec3f(position_field);
  ts_assert_double_is_bigger(positionAfter[2], expected_z + 0.01, message);

  if (!is_regenerated) {
    ts_notify_controller_status(false);
    wb_robot_step(TIME_STEP);
    // trigger template PROTO regeneration
    wb_supervisor_field_set_sf_bool(reverted_field, true);
  } else
    ts_send_success();

  return EXIT_SUCCESS;
}
