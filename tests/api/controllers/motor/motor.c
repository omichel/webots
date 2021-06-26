#include <webots/motor.h>
#include <webots/position_sensor.h>
#include <webots/robot.h>

#include "../../../lib/ts_assertion.h"
#include "../../../lib/ts_utils.h"

#define ACCELERATION 4.8
#define CONTROL_P 15
#define CONTROL_I 5
#define CONTROL_D 1
#define TORQUE 50.0
#define VELOCITY 18.0
#define NUMBER_OF_STEPS 20
// copied from the .wbt file
#define TIME_STEP 32
#define MAX_POS 1.51
#define MIN_POS -1.63
#define MAX_ACCELERATION 5.0
#define MAX_VELOCITY 20.0
#define MAX_TORQUE 100.0
// saved from prior simulation
#define REFERENCE_POSITION 1.18212
#define REFERENCE_TORQUE 0.0268543

int main(int argc, char **argv) {
  ts_setup(argv[0]);

  const WbDeviceTag motor = wb_robot_get_device("rotational motor");
  const WbDeviceTag position_sensor = wb_motor_get_position_sensor(motor);
  wb_robot_step(TIME_STEP);

  double position = wb_position_sensor_get_value(position_sensor);
  ts_assert_double_equal(
    position, NAN, "The position value measured by the position sensor should be NaN and not %g before the device is enabled",
    position);

  const double torque_feedback = wb_motor_get_torque_feedback(motor);
  ts_assert_double_equal(
    torque_feedback, NAN,
    "The torque feedback value measured by the motor should be NaN and not %g before the device is enabled", torque_feedback);

  const double feedback_sampling_period = wb_motor_get_torque_feedback_sampling_period(motor);
  ts_assert_double_equal(
    feedback_sampling_period, 0,
    "The torque feedback sampling period value measured by the motor should be 0 and not %g before the device is enabled",
    feedback_sampling_period);

  const double max_position = wb_motor_get_max_position(motor);
  ts_assert_double_equal(max_position, MAX_POS, "The max position value of the motor should be %g and not %g", MAX_POS,
                         max_position);

  const double min_position = wb_motor_get_min_position(motor);
  ts_assert_double_equal(min_position, MIN_POS, "The min position value of the motor should be %g and not %g", MIN_POS,
                         min_position);

  double velocity = wb_motor_get_velocity(motor);
  double max_velocity = wb_motor_get_max_velocity(motor);
  ts_assert_double_equal(max_velocity, MAX_VELOCITY, "The max velocity value of the motor should be %g and not %g",
                         MAX_VELOCITY, max_velocity);
  ts_assert_double_equal(velocity, max_velocity, "Velocity %g and max velocity %g values have to be equal", velocity,
                         max_velocity);

  double acceleration = wb_motor_get_acceleration(motor);
  ts_assert_double_equal(acceleration, MAX_ACCELERATION, "The acceleration value of the motor should be %g and not %g",
                         MAX_ACCELERATION, acceleration);

  const WbDeviceTag uncoupled_motor = wb_robot_get_device_by_index(0);
  const WbDeviceTag coupled_motor = wb_robot_get_device_by_index(1);
  const WbDeviceTag coupled_motor2 = wb_robot_get_device_by_index(2);
  double uncoupled_multiplier = wb_motor_get_multiplier(uncoupled_motor);
  double coupled_multiplier = wb_motor_get_multiplier(coupled_motor);
  double coupled_multiplier2 = wb_motor_get_multiplier(coupled_motor2);
  ts_assert_double_equal(uncoupled_multiplier, 0.2, "The multiplier value of the motor should be %g and not %g", 0.2,
                         uncoupled_multiplier);
  ts_assert_double_equal(coupled_multiplier, 5, "The multiplier value of the motor should be 5 and not %g", coupled_multiplier);
  ts_assert_double_equal(coupled_multiplier2, -5, "The multiplier value of the motor should be -5 and not %g",
                         coupled_multiplier2);

  double torque = wb_motor_get_available_torque(motor);
  double max_torque = wb_motor_get_max_torque(motor);
  ts_assert_double_equal(max_torque, MAX_TORQUE, "The max torque value of the motor should be %g and not %g", MAX_TORQUE,
                         max_torque);
  ts_assert_double_equal(torque, max_torque, "Torque %g and max torque %g values have to be equal", torque, max_torque);

  wb_position_sensor_enable(position_sensor, TIME_STEP);
  wb_motor_enable_torque_feedback(motor, TIME_STEP);
  wb_motor_set_control_pid(motor, CONTROL_P, CONTROL_I, CONTROL_D);
  wb_motor_set_available_torque(motor, TORQUE);
  wb_motor_set_position(motor, 100);

  wb_robot_step(TIME_STEP);

  const double target_position = wb_motor_get_target_position(motor);
  ts_assert_double_equal(target_position, max_position,
                         "The target position value measured by the motor should be %g and not %g", max_position,
                         target_position);
  int i;
  for (i = 0; i < NUMBER_OF_STEPS; ++i)
    wb_robot_step(TIME_STEP);

  position = wb_position_sensor_get_value(position_sensor);
  ts_assert_double_in_delta(position, REFERENCE_POSITION, 0.001,
                            "The position value measured by the position sensor should be %g and not %g", REFERENCE_POSITION,
                            position);

  torque = wb_motor_get_torque_feedback(motor);
  ts_assert_double_in_delta(torque, REFERENCE_TORQUE, 0.001,
                            "The torque feedback value measured by the motor should be %g and not %g", REFERENCE_TORQUE,
                            torque);

  velocity = wb_motor_get_velocity(motor);
  max_velocity = wb_motor_get_max_velocity(motor);
  ts_assert_double_equal(max_velocity, MAX_VELOCITY,
                         "The max velocity value of the motor should be %g and not %g after parameters changed", MAX_VELOCITY,
                         max_velocity);
  ts_assert_double_equal(velocity, MAX_VELOCITY, "Velocity should be %g and not %g after parameters changed", MAX_VELOCITY,
                         velocity);

  torque = wb_motor_get_available_torque(motor);
  max_torque = wb_motor_get_max_torque(motor);
  ts_assert_double_equal(max_torque, MAX_TORQUE,
                         "The max torque value of the motor should be %g and not %g after torque changed", MAX_TORQUE,
                         max_torque);
  ts_assert_double_equal(torque, TORQUE, "Torque should be %g and not %g after torque changed", TORQUE, torque);

  wb_motor_set_velocity(motor, VELOCITY);
  wb_motor_set_acceleration(motor, ACCELERATION);

  wb_robot_step(TIME_STEP);

  acceleration = wb_motor_get_acceleration(motor);
  ts_assert_double_equal(acceleration, ACCELERATION, "Acceleration should be %g and not %g after acceleration changed",
                         ACCELERATION, acceleration);

  velocity = wb_motor_get_velocity(motor);
  max_velocity = wb_motor_get_max_velocity(motor);
  ts_assert_double_equal(max_velocity, MAX_VELOCITY,
                         "The max velocity value of the motor should be %g and not %g after velocity changed", MAX_VELOCITY,
                         max_velocity);
  ts_assert_double_equal(velocity, VELOCITY, "Velocity should be %g and not %g after velocity changed", VELOCITY, velocity);

  ts_send_success();
  return EXIT_SUCCESS;
}
