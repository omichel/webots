// Copyright 1996-2021 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#define WB_ALLOW_MIXING_C_AND_CPP_API
#include <webots/device.h>
#include <webots/motor.h>
#include <webots/Brake.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Robot.hpp>

using namespace webots;

void Motor::setAcceleration(double acceleration) {
  wb_motor_set_acceleration(getTag(), acceleration);
}

void Motor::setVelocity(double vel) {
  wb_motor_set_velocity(getTag(), vel);
}

void Motor::setControlPID(double p, double i, double d) {
  wb_motor_set_control_pid(getTag(), p, i, d);
}

// Force
void Motor::setForce(double force) {
  wb_motor_set_force(getTag(), force);
}

void Motor::setAvailableForce(double availableForce) {
  wb_motor_set_available_force(getTag(), availableForce);
}

void Motor::enableForceFeedback(int sampling_period) {
  wb_motor_enable_force_feedback(getTag(), sampling_period);
}

void Motor::disableForceFeedback() {
  wb_motor_disable_force_feedback(getTag());
}

int Motor::getForceFeedbackSamplingPeriod() const {
  return wb_motor_get_force_feedback_sampling_period(getTag());
}

double Motor::getForceFeedback() const {
  return wb_motor_get_force_feedback(getTag());
}

// Torque
void Motor::setTorque(double torque) {
  wb_motor_set_force(getTag(), torque);
}
void Motor::setAvailableTorque(double availableTorque) {
  wb_motor_set_available_force(getTag(), availableTorque);
}

double Motor::getTorqueFeedback() const {
  return wb_motor_get_force_feedback(getTag());
}

void Motor::enableTorqueFeedback(int sampling_period) {
  wb_motor_enable_force_feedback(getTag(), sampling_period);
}

void Motor::disableTorqueFeedback() {
  wb_motor_disable_force_feedback(getTag());
}

int Motor::getTorqueFeedbackSamplingPeriod() const {
  return wb_motor_get_force_feedback_sampling_period(getTag());
}

void Motor::setPosition(double position) {
  wb_motor_set_position(getTag(), position);
}

Motor::Type Motor::getType() const {
  return Type(wb_motor_get_type(getTag()));
}

double Motor::getTargetPosition() const {
  return wb_motor_get_target_position(getTag());
}

double Motor::getMinPosition() const {
  return wb_motor_get_min_position(getTag());
}

double Motor::getMaxPosition() const {
  return wb_motor_get_max_position(getTag());
}

double Motor::getVelocity() const {
  return wb_motor_get_velocity(getTag());
}

double Motor::getMaxVelocity() const {
  return wb_motor_get_max_velocity(getTag());
}

double Motor::getAcceleration() const {
  return wb_motor_get_acceleration(getTag());
}

double Motor::getAvailableForce() const {
  return wb_motor_get_available_force(getTag());
}

double Motor::getMaxForce() const {
  return wb_motor_get_max_force(getTag());
}

double Motor::getAvailableTorque() const {
  return wb_motor_get_available_torque(getTag());
}

double Motor::getMaxTorque() const {
  return wb_motor_get_max_torque(getTag());
}

double Motor::getMultiplier() const {
  return wb_motor_get_multiplier(getTag());
}

Brake *Motor::getBrake() {
  if (brake == NULL)
    brake = dynamic_cast<Brake *>(Robot::getDeviceFromTag(getBrakeTag()));
  return brake;
}

int Motor::getBrakeTag() const {
  return wb_motor_get_brake(getTag());
}

PositionSensor *Motor::getPositionSensor() {
  if (positionSensor == NULL)
    positionSensor = dynamic_cast<PositionSensor *>(Robot::getDeviceFromTag(getPositionSensorTag()));
  return positionSensor;
}

int Motor::getPositionSensorTag() const {
  return wb_motor_get_position_sensor(getTag());
}
