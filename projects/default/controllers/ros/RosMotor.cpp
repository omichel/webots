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

#include "RosMotor.hpp"

#include <webots/Brake.hpp>
#include <webots/PositionSensor.hpp>

RosMotor::RosMotor(Motor *motor, Ros *ros) : RosDevice(motor, ros) {
  std::string fixedDeviceName = RosDevice::fixedDeviceName();
  mSetAccelerationServer = RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/set_acceleration",
                                                          &RosMotor::setAccelerationCallback);
  mSetVelocityServer =
    RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/set_velocity", &RosMotor::setVelocityCallback);
  mSetControlPServer =
    RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/set_control_pid", &RosMotor::setControlPIDCallback);
  mSetPositionServer =
    RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/set_position", &RosMotor::setPositionCallback);
  mGetTargetPositionServer = RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_target_position",
                                                            &RosMotor::getTargetPositionCallback);
  mGetMinPositionServer = RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_min_position",
                                                         &RosMotor::getMinPositionCallback);
  mGetMaxPositionServer = RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_max_position",
                                                         &RosMotor::getMaxPositionCallback);
  mGetVelocityServer =
    RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_velocity", &RosMotor::getVelocityCallback);
  mGetMaxVelocityServer = RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_max_velocity",
                                                         &RosMotor::getMaxVelocityCallback);
  mGetAccelerationServer = RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_acceleration",
                                                          &RosMotor::getAccelerationCallback);
  mGetMultiplierServer =
    RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_multiplier", &RosMotor::getMultiplierCallback);
  mGetTypeServer =
    RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_type", &RosMotor::getTypeCallback);
  mGetBrakeNameServer =
    RosDevice::rosAdvertiseService(ros->name() + '/' + fixedDeviceName + "/get_brake_name", &RosMotor::getBrakeNameCallback);
  mGetPositionSensorNameServer = RosDevice::rosAdvertiseService(
    ros->name() + '/' + fixedDeviceName + "/get_position_sensor_name", &RosMotor::getPositionSensorNameCallback);

  mMotor = motor;
  if (mMotor->getType() == Motor::ROTATIONAL) {
    mSetTorqueServer =
      RosDevice::rosAdvertiseService((ros->name()) + '/' + fixedDeviceName + "/set_torque", &RosMotor::setTorqueCallback);
    mSetAvailableTorqueServer = RosDevice::rosAdvertiseService((ros->name()) + '/' + fixedDeviceName + "/set_available_torque",
                                                               &RosMotor::setAvailableTorqueCallback);
    mGetAvailableTorqueServer = RosDevice::rosAdvertiseService((ros->name()) + '/' + fixedDeviceName + "/get_available_torque",
                                                               &RosMotor::getAvailableTorqueCallback);
    mGetMaxTorqueServer = RosDevice::rosAdvertiseService((ros->name()) + '/' + fixedDeviceName + "/get_max_torque",
                                                         &RosMotor::getMaxTorqueCallback);
    mTorqueFeedbackSensor = new RosMotorSensor(mMotor, fixedDeviceName + "/torque_feedback_sensor", ros);
  } else {
    mSetForceServer =
      RosDevice::rosAdvertiseService((ros->name()) + '/' + fixedDeviceName + "/set_force", &RosMotor::setForceCallback);
    mSetAvailableForceServer = RosDevice::rosAdvertiseService((ros->name()) + '/' + fixedDeviceName + "/set_available_force",
                                                              &RosMotor::setAvailableForceCallback);
    mGetAvailableForceServer = RosDevice::rosAdvertiseService((ros->name()) + '/' + fixedDeviceName + "/get_available_force",
                                                              &RosMotor::getAvailableForceCallback);
    mGetMaxForceServer =
      RosDevice::rosAdvertiseService((ros->name()) + '/' + fixedDeviceName + "/get_max_force", &RosMotor::getMaxForceCallback);
    mForceFeedbackSensor = new RosMotorSensor(mMotor, fixedDeviceName + "/force_feedback_sensor", ros);
  }
}

RosMotor::~RosMotor() {
  if (mMotor->getType() == Motor::ROTATIONAL)
    delete mForceFeedbackSensor;
  else
    delete mTorqueFeedbackSensor;
  mSetAccelerationServer.shutdown();
  mSetVelocityServer.shutdown();
  mSetControlPServer.shutdown();
  mSetPositionServer.shutdown();
  mGetTargetPositionServer.shutdown();
  mGetMinPositionServer.shutdown();
  mGetMaxPositionServer.shutdown();
  mGetVelocityServer.shutdown();
  mGetMaxVelocityServer.shutdown();
  mGetAccelerationServer.shutdown();
  mGetMultiplierServer.shutdown();
  mGetTypeServer.shutdown();
  if (mMotor->getType() == Motor::ROTATIONAL) {
    mSetTorqueServer.shutdown();
    mSetAvailableTorqueServer.shutdown();
    mGetAvailableTorqueServer.shutdown();
    mGetMaxTorqueServer.shutdown();
  } else {
    mSetForceServer.shutdown();
    mSetAvailableForceServer.shutdown();
    mGetAvailableForceServer.shutdown();
    mGetMaxForceServer.shutdown();
  }
}

bool RosMotor::setAccelerationCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res) {
  mMotor->setAcceleration(req.value);
  res.success = true;
  return true;
}

bool RosMotor::setVelocityCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res) {
  mMotor->setVelocity(req.value);
  res.success = true;
  return true;
}

bool RosMotor::setForceCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res) {
  mMotor->setForce(req.value);
  res.success = true;
  return true;
}

bool RosMotor::setTorqueCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res) {
  mMotor->setTorque(req.value);
  res.success = true;
  return true;
}

bool RosMotor::setAvailableForceCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res) {
  mMotor->setAvailableForce(req.value);
  res.success = true;
  return true;
}

bool RosMotor::setAvailableTorqueCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res) {
  mMotor->setAvailableTorque(req.value);
  res.success = true;
  return true;
}

bool RosMotor::setControlPIDCallback(webots_ros::motor_set_control_pid::Request &req,
                                     webots_ros::motor_set_control_pid::Response &res) {
  mMotor->setControlPID(req.controlp, req.controli, req.controld);
  res.success = 1;
  return true;
}

bool RosMotor::setPositionCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res) {
  mMotor->setPosition(req.value);
  res.success = true;
  return true;
}

bool RosMotor::getTargetPositionCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getTargetPosition();
  return true;
}

bool RosMotor::getMinPositionCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getMinPosition();
  return true;
}

bool RosMotor::getMaxPositionCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getMaxPosition();
  return true;
}

bool RosMotor::getVelocityCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getVelocity();
  return true;
}

bool RosMotor::getMaxVelocityCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getMaxVelocity();
  return true;
}

bool RosMotor::getAccelerationCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getAcceleration();
  return true;
}

bool RosMotor::getAvailableForceCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getAvailableForce();
  return true;
}

bool RosMotor::getMaxForceCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getMaxForce();
  return true;
}

bool RosMotor::getAvailableTorqueCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getAvailableTorque();
  return true;
}

bool RosMotor::getMaxTorqueCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getMaxTorque();
  return true;
}

bool RosMotor::getMultiplierCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mMotor);
  res.value = mMotor->getMultiplier();
  return true;
}

bool RosMotor::getTypeCallback(webots_ros::get_int::Request &req, webots_ros::get_int::Response &res) {
  assert(mMotor);
  res.value = mMotor->getType();
  return true;
}

bool RosMotor::getBrakeNameCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res) {
  Brake *brake = mMotor->getBrake();
  if (brake)
    res.value = brake->getName();
  else
    res.value = "";
  return true;
}

bool RosMotor::getPositionSensorNameCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res) {
  PositionSensor *positionSensor = mMotor->getPositionSensor();
  if (positionSensor)
    res.value = positionSensor->getName();
  else
    res.value = "";
  return true;
}
