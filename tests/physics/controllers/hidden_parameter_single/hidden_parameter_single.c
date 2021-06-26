#include <webots/receiver.h>
#include <webots/supervisor.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define TIME_STEP 32

int main(int argc, char **argv) {
  double expectedInitPos[3] = {0.1, 0.1, 0.23};
  double angle = 0.47;
  double cosAngle = cos(angle);
  double sinAngle = sin(angle);
  double expectedMatrix[9] = {1.0, 0.0, 0.0, 0.0, cosAngle, -sinAngle, 0.0, sinAngle, cosAngle};
  ts_setup(argv[0]);

  WbNodeRef robot = wb_supervisor_node_get_from_def("ROBOT");

  ts_assert_doubles_equal(3, wb_supervisor_node_get_position(robot), expectedInitPos,
                          "The hidden translation has not been set correctly");

  ts_assert_doubles_equal(9, wb_supervisor_node_get_orientation(robot), expectedMatrix,
                          "The hidden rotation has not been set correctly");

  wb_robot_step(TIME_STEP);

  const double *position = wb_supervisor_node_get_position(robot);
  ts_assert_boolean_equal((expectedInitPos[1] + 0.015) < position[1], "The hidden linear velocity has not been set correctly");

  // test loading of hidden positions for joints
  WbDeviceTag receiver = wb_robot_get_device("receiver");
  wb_receiver_enable(receiver, TIME_STEP);

  const float delta = 1e-10;
  char robot_name[20];
  bool received_message = false;

  while (wb_robot_step(TIME_STEP) != -1.0 && wb_robot_get_time() < 5.0) {
    if (wb_receiver_get_queue_length(receiver) > 0) {
      const char *inbuffer = wb_receiver_get_data(receiver);
      double position, position2, position3;

      sscanf(inbuffer, "%lf %lf %lf %s\n", &position, &position2, &position3, robot_name);
      received_message = true;

      if (strcmp(robot_name, "HingeJoint") == 0)
        ts_assert_double_in_delta(position, 0.111, delta, "HingeJoint position is %.4f instead of %.4f", position, 0.111);
      else if (strcmp(robot_name, "SliderJoint") == 0)
        ts_assert_double_in_delta(position, 0.222, delta, "SliderJoint position is %.4f instead of %.4f", position, 0.222);
      else if (strcmp(robot_name, "Hinge2Joint") == 0) {
        ts_assert_double_in_delta(position, 0.333, delta, "Hinge2Joint position is %.4f instead of %.4f", position, 0.333);
        ts_assert_double_in_delta(position2, 0.444, delta, "Hinge2Joint position2 is %.4f instead of %.4f", position2, 0.444);
      } else if (strcmp(robot_name, "BallJoint") == 0) {
        ts_assert_double_in_delta(position, 0.555, delta, "BallJoint position is %.4f instad of %.4f", position, 0.555);
        ts_assert_double_in_delta(position2, 0.666, delta, "BallJoint position2 is %.4f instead of %.4f", position2, 0.666);
        ts_assert_double_in_delta(position3, 0.777, delta, "BallJoint position3 is %.4f instead of %.4f", position3, 0.777);
      } else
        ts_assert_boolean_equal(0, "Message unknown.");

      wb_receiver_next_packet(receiver);
    }
  }

  ts_assert_boolean_equal(received_message, "Should have received at least one message, but didn't.");

  ts_send_success();
  return EXIT_SUCCESS;
}
