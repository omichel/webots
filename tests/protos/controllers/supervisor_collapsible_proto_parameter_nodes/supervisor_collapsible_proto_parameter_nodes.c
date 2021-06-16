#include <stdio.h>
#include <stdlib.h>
#include <webots/receiver.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  ts_setup(argv[0]);

  WbDeviceTag receiver = wb_robot_get_device("receiver");
  wb_receiver_enable(receiver, TIME_STEP);

  wb_robot_step(TIME_STEP);

  char robot_name[30];
  bool received_invisible = false;
  bool received_partially_visible = false;
  bool received_non_proto = false;

  double position_invisible[5];
  double position_partially_visible[5];
  double position_non_proto[5];

  while (wb_robot_get_time() < 10.0) {
    wb_robot_step(TIME_STEP);

    if (wb_receiver_get_queue_length(receiver) > 0) {
      const char *inbuffer = wb_receiver_get_data(receiver);
      double position[5];
      sscanf(inbuffer, "%lf %lf %lf %lf %lf %s\n", &position[0], &position[1], &position[2], &position[3], &position[4],
             robot_name);

      if (strcmp(robot_name, "non_proto") == 0 && !received_non_proto) {
        memcpy(position_non_proto, position, sizeof(position_non_proto));
        received_non_proto = true;
      } else if (strcmp(robot_name, "partially_visible") == 0 && !received_partially_visible) {
        memcpy(position_partially_visible, position, sizeof(position_partially_visible));
        received_partially_visible = true;
      } else if (strcmp(robot_name, "invisible") == 0 && !received_invisible) {
        memcpy(position_invisible, position, sizeof(position_invisible));
        received_invisible = true;
      } else {
        ts_assert_boolean_equal(0, "Message unknown.");
      }

      wb_receiver_next_packet(receiver);
    }

    if (received_invisible && received_partially_visible && received_non_proto) {
      const float delta = 1e-8;
      for (int i = 0; i < 5; ++i) {
        ts_assert_double_in_delta(position_partially_visible[i], position_non_proto[i], delta,
                                  "Partially visible nested proto not behaving like non-proto version.");
        ts_assert_double_in_delta(position_invisible[i], position_non_proto[i], delta,
                                  "Invisible nested proto not behaving like non-proto version.");
      }

      received_invisible = false;
      received_partially_visible = false;
      received_non_proto = false;
    }
  }

  ts_send_success();
  return EXIT_SUCCESS;
}
