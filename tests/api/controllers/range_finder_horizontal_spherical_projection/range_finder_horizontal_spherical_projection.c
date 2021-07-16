#include <webots/range_finder.h>
#include <webots/robot.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#include <string.h>

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[0]);

  WbDeviceTag range_finder = wb_robot_get_device("range-finder");
  int width = wb_range_finder_get_width(range_finder);
  int height = wb_range_finder_get_height(range_finder);

  wb_range_finder_enable(range_finder, TIME_STEP);

  wb_robot_step(TIME_STEP);

  const float *image = wb_range_finder_get_range_image(range_finder);
  int i = 0;
  for (i = 0; i < 4; ++i) {
    int x1 = width * ((i * 2.0 + 1.0) / 8.0) - 1;
    int x2 = width * ((i * 2.0 + 2.0) / 8.0) - 1;
    float sub_cameras_junction_depth = wb_range_finder_image_get_depth(image, width, x1, height - 10);
    float sub_camera_center_depth = wb_range_finder_image_get_depth(image, width, x2, height - 10);
    ts_assert_double_in_delta(
      sub_cameras_junction_depth, sub_camera_center_depth, 0.05,
      "Depths at the sub-camera center and sub-cameras junction are not equal (%lf (%d,%d) != %lf (%d,%d)).",
      sub_cameras_junction_depth, x1, height - 10, sub_camera_center_depth, x2, height - 10);
  }

  ts_send_success();
  return EXIT_SUCCESS;
}
