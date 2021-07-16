/* Test regeneration of template PROTO in parameter:
 * 1. trigger regeneration by changing the box size
 * 2. test that 'translation' parameter connected using 'IS' is correctly redirected
 */

#include <webots/camera.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

static void check_object_status(WbDeviceTag camera, bool visible, const char *message) {
  const int expected = visible ? 0 : 255;
  const unsigned char *values = wb_camera_get_image(camera);
  ts_assert_color_in_delta(wb_camera_image_get_red(values, 1, 0, 0), wb_camera_image_get_green(values, 1, 0, 0),
                           wb_camera_image_get_blue(values, 1, 0, 0), expected, expected, expected, 1, message);
}

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  WbDeviceTag cameraX = wb_robot_get_device("cameraX");
  WbDeviceTag cameraY = wb_robot_get_device("cameraY");
  wb_camera_enable(cameraX, TIME_STEP);
  wb_camera_enable(cameraY, TIME_STEP);
  ts_assert_int_equal(wb_camera_get_width(cameraX), 1, "Wrong cameraX width");
  ts_assert_int_equal(wb_camera_get_height(cameraX), 1, "Wrong cameraX height");
  ts_assert_int_equal(wb_camera_get_width(cameraY), 1, "Wrong cameraY width");
  ts_assert_int_equal(wb_camera_get_height(cameraY), 1, "Wrong cameraY height");

  WbNodeRef protoNode = wb_supervisor_node_get_from_def("TEST_PROTO");
  WbFieldRef tField = wb_supervisor_node_get_field(protoNode, "translation");
  WbFieldRef xField = wb_supervisor_node_get_field(protoNode, "xSize");

  wb_robot_step(TIME_STEP);

  if (strncmp(argv[1], "nested_mixed_template_proto", 27) == 0) {
    check_object_status(cameraX, false, "Unexpected cameraX color. The box should not be visible.");
    check_object_status(cameraY, false, "Unexpected cameraY color. The box should not be visible.");
    wb_supervisor_field_set_sf_float(xField, 0.5);

    wb_robot_step(TIME_STEP);

    check_object_status(cameraX, true, "Unexpected cameraX color after changing x size. The box should be visible.");
    check_object_status(cameraY, false, "Unexpected cameraY color after changing x size. The box not should be visible.");

    const double newPosition[3] = {0.0, 0.2, 0.0};
    wb_supervisor_field_set_sf_vec3f(tField, newPosition);

    wb_robot_step(TIME_STEP);

    check_object_status(cameraX, false, "Unexpected cameraX color after changing translation. The box should not be visible.");
    check_object_status(cameraY, true, "Unexpected cameraY color after changing translation. The box should be visible.");

  } else {  // argv[1] == "proto_nested_2"
    check_object_status(cameraX, true, "Unexpected cameraX color. The cylinder should be visible.");
    check_object_status(cameraY, false, "Unexpected cameraY color. The cylinder should not be visible.");

    const double newPosition[3] = {0.0, 0.2, 0.0};
    wb_supervisor_field_set_sf_vec3f(tField, newPosition);

    wb_robot_step(TIME_STEP);

    check_object_status(cameraX, false,
                        "Unexpected cameraX color after changing translation. The cylinder should not be visible.");
    check_object_status(cameraY, true, "Unexpected cameraY color after changing translation. The cylinder should be visible.");
  }

  ts_send_success();
  return EXIT_SUCCESS;
}
