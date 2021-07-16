#include <webots/camera.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  WbDeviceTag camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, TIME_STEP);
  ts_assert_int_equal(wb_camera_get_width(camera), 1, "Wrong camera width");
  ts_assert_int_equal(wb_camera_get_height(camera), 1, "Wrong camera height");

  WbNodeRef protoNode = wb_supervisor_node_get_from_def("TEST_PROTO");
  WbFieldRef sizeField = wb_supervisor_node_get_field(protoNode, "size");

  wb_robot_step(TIME_STEP);

  const unsigned char *values_before = wb_camera_get_image(camera);
  ts_assert_color_in_delta(
    wb_camera_image_get_red(values_before, 1, 0, 0), wb_camera_image_get_green(values_before, 1, 0, 0),
    wb_camera_image_get_blue(values_before, 1, 0, 0), 255, 255, 255,  // background
    1, "Unexpected camera color due to incorrect automatic derived parameter association: the box should be smaller.");

  wb_robot_step(TIME_STEP);

  double new_size[3] = {1.0, 1.0, 0.5};
  wb_supervisor_field_set_sf_vec3f(sizeField, new_size);

  wb_robot_step(TIME_STEP);

  const unsigned char *values_after = wb_camera_get_image(camera);
  ts_assert_color_in_delta(
    wb_camera_image_get_red(values_after, 1, 0, 0), wb_camera_image_get_green(values_after, 1, 0, 0),
    wb_camera_image_get_blue(values_after, 1, 0, 0), 255, 255, 255,  // background
    1, "Unexpected camera color due to incorrect automatic derived parameter association: the box size should not change.");

  ts_send_success();
  return EXIT_SUCCESS;
}
