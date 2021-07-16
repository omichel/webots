/*
 * Regression test for issue 908
 */

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
  WbFieldRef translationField = wb_supervisor_node_get_field(protoNode, "translation");

  WbNodeRef parameterNode = wb_supervisor_node_get_from_def("TEST_PARAMETER");
  WbFieldRef parameterTranslationField = wb_supervisor_node_get_field(parameterNode, "translation");
  WbFieldRef sizeField = wb_supervisor_node_get_field(parameterNode, "boxSize");
  wb_robot_step(TIME_STEP);

  const unsigned char *valuesA = wb_camera_get_image(camera);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesA, 1, 0, 0), wb_camera_image_get_green(valuesA, 1, 0, 0),
                           wb_camera_image_get_blue(valuesA, 1, 0, 0), 255, 255, 255,  // background
                           1, "Unexpected camera color. Box should be located at position (-1, -1, -1).");

  double parentTranslation[] = {0, 0, 0};
  wb_supervisor_field_set_sf_vec3f(translationField, parentTranslation);

  wb_robot_step(TIME_STEP);

  const unsigned char *valuesB = wb_camera_get_image(camera);
  ts_assert_color_in_delta(
    wb_camera_image_get_red(valuesB, 1, 0, 0), wb_camera_image_get_green(valuesB, 1, 0, 0),
    wb_camera_image_get_blue(valuesB, 1, 0, 0), 0, 0, 0,  // box
    1, "Unexpected camera color after changing parent translation. Box should be located at position (0, 0, 0).");

  double size[] = {0.5, 0.5, 1};
  wb_supervisor_field_set_sf_vec3f(sizeField, size);

  wb_robot_step(TIME_STEP);

  const unsigned char *valuesC = wb_camera_get_image(camera);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesC, 1, 0, 0), wb_camera_image_get_green(valuesC, 1, 0, 0),
                           wb_camera_image_get_blue(valuesC, 1, 0, 0), 255, 255, 255,  // background
                           1, "Unexpected camera color after resizing the Box. Box should not be visible in the camera image.");

  double parameterTranslation[] = {-0.25, 0.25, 0};
  wb_supervisor_field_set_sf_vec3f(parameterTranslationField, parameterTranslation);

  wb_robot_step(TIME_STEP);

  const unsigned char *valuesD = wb_camera_get_image(camera);
  ts_assert_color_in_delta(
    wb_camera_image_get_red(valuesD, 1, 0, 0), wb_camera_image_get_green(valuesD, 1, 0, 0),
    wb_camera_image_get_blue(valuesD, 1, 0, 0), 0, 0, 0,  // box
    1, "Unexpected camera color after changing parameter translation. Box should be visible in the camera image.");

  ts_send_success();
  return EXIT_SUCCESS;
}
