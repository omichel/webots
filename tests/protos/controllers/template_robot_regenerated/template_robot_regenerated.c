#include <webots/robot.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#include <stdio.h>

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[1]);  // give the controller args

  wb_robot_step(TIME_STEP);

  WbNodeRef node = wb_supervisor_node_get_from_def("ROBOT");
  WbFieldRef field = wb_supervisor_node_get_field(node, "proceduralField");

  int i = 0;
  char buffer[32];
  while (wb_robot_step(10 * TIME_STEP) != -1 && i < 5) {
    sprintf(buffer, "counter: %d", i);
    wb_supervisor_field_set_sf_string(field, buffer);
    ++i;
  };

  ts_send_success();
  return EXIT_SUCCESS;
}
