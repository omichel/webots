#include <webots/camera.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

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
  WbFieldRef xField = wb_supervisor_node_get_field(protoNode, "dxSize");
  WbFieldRef yField = wb_supervisor_node_get_field(protoNode, "dySize");

  wb_robot_step(TIME_STEP);

  const unsigned char *valuesXA = wb_camera_get_image(cameraX);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesXA, 1, 0, 0), wb_camera_image_get_green(valuesXA, 1, 0, 0),
                           wb_camera_image_get_blue(valuesXA, 1, 0, 0), 255, 255, 255,  // background
                           1, "Unexpected cameraX color. The box should not be visible.");

  const unsigned char *valuesYA = wb_camera_get_image(cameraY);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesYA, 1, 0, 0), wb_camera_image_get_green(valuesYA, 1, 0, 0),
                           wb_camera_image_get_blue(valuesYA, 1, 0, 0), 255, 255, 255,  // background
                           1, "Unexpected cameraY color. The box should not be visible.");

  wb_supervisor_field_set_sf_float(yField, 0.5);

  wb_robot_step(TIME_STEP);

  const unsigned char *valuesXB = wb_camera_get_image(cameraX);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesXB, 1, 0, 0), wb_camera_image_get_green(valuesXB, 1, 0, 0),
                           wb_camera_image_get_blue(valuesXB, 1, 0, 0), 255, 255, 255,  // background
                           1, "Unexpected cameraX after changing y size. The box should not be visible.");

  const unsigned char *valuesYB = wb_camera_get_image(cameraY);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesYB, 1, 0, 0), wb_camera_image_get_green(valuesYB, 1, 0, 0),
                           wb_camera_image_get_blue(valuesYB, 1, 0, 0), 0, 0, 0,  // box
                           1, "Unexpected cameraY color after changing y size. The box should be visible.");

  wb_supervisor_field_set_sf_float(xField, 0.5);

  wb_robot_step(TIME_STEP);

  const unsigned char *valuesXC = wb_camera_get_image(cameraX);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesXC, 1, 0, 0), wb_camera_image_get_green(valuesXC, 1, 0, 0),
                           wb_camera_image_get_blue(valuesXC, 1, 0, 0), 0, 0, 0,  // box
                           1, "Unexpected cameraX after changing x size. The box should be visible.");

  const unsigned char *valuesYC = wb_camera_get_image(cameraY);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesYC, 1, 0, 0), wb_camera_image_get_green(valuesYC, 1, 0, 0),
                           wb_camera_image_get_blue(valuesYC, 1, 0, 0), 0, 0, 0,  // box
                           1, "Unexpected cameraY color after changing x size. The box should be visible.");

  wb_robot_step(TIME_STEP);

  double newTranslation[] = {-2, -2, -2};
  wb_supervisor_field_set_sf_vec3f(tField, newTranslation);

  wb_robot_step(TIME_STEP);

  const unsigned char *valuesXD = wb_camera_get_image(cameraX);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesXD, 1, 0, 0), wb_camera_image_get_green(valuesXD, 1, 0, 0),
                           wb_camera_image_get_blue(valuesXD, 1, 0, 0), 255, 255, 255,  // background
                           1, "Unexpected cameraX after changing translation. The box should not be visible.");

  const unsigned char *valuesYD = wb_camera_get_image(cameraY);
  ts_assert_color_in_delta(wb_camera_image_get_red(valuesYD, 1, 0, 0), wb_camera_image_get_green(valuesYD, 1, 0, 0),
                           wb_camera_image_get_blue(valuesYD, 1, 0, 0), 255, 255, 255,  // background
                           1, "Unexpected cameraY color after changing translation. The box should not be visible.");

  ts_send_success();
  return EXIT_SUCCESS;
}
