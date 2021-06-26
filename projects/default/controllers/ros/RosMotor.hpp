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

#ifndef ROS_MOTOR_HPP
#define ROS_MOTOR_HPP

#include <webots/Motor.hpp>
#include "RosDevice.hpp"
#include "RosMotorSensor.hpp"

#include <webots_ros/get_float.h>
#include <webots_ros/get_int.h>
#include <webots_ros/get_string.h>
#include <webots_ros/set_float.h>

#include <webots_ros/motor_set_control_pid.h>

using namespace webots;

class RosMotor : public RosDevice {
public:
  RosMotor(Motor *motor, Ros *ros);
  virtual ~RosMotor();

  bool setAccelerationCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res);
  bool setVelocityCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res);
  bool setForceCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res);
  bool setTorqueCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res);
  bool setAvailableForceCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res);
  bool setAvailableTorqueCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res);
  bool setControlPIDCallback(webots_ros::motor_set_control_pid::Request &req, webots_ros::motor_set_control_pid::Response &res);
  bool setPositionCallback(webots_ros::set_float::Request &req, webots_ros::set_float::Response &res);
  bool getTargetPositionCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getMinPositionCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getMaxPositionCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getVelocityCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getMaxVelocityCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getAccelerationCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getAvailableForceCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getMaxForceCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getAvailableTorqueCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getMaxTorqueCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getMultiplierCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res);
  bool getTypeCallback(webots_ros::get_int::Request &req, webots_ros::get_int::Response &res);
  bool getBrakeNameCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res);
  bool getPositionSensorNameCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res);

  RosMotorSensor *mForceFeedbackSensor;
  RosMotorSensor *mTorqueFeedbackSensor;

private:
  RosMotor(const RosMotor &);             // non constructor-copyable;
  RosMotor &operator=(const RosMotor &);  // non copyable

  Motor *mMotor;
  ros::ServiceServer mSetAccelerationServer;
  ros::ServiceServer mSetVelocityServer;
  ros::ServiceServer mSetForceServer;
  ros::ServiceServer mSetTorqueServer;
  ros::ServiceServer mSetAvailableForceServer;
  ros::ServiceServer mSetAvailableTorqueServer;
  ros::ServiceServer mSetControlPServer;
  ros::ServiceServer mSetPositionServer;
  ros::ServiceServer mGetTargetPositionServer;
  ros::ServiceServer mGetMinPositionServer;
  ros::ServiceServer mGetMaxPositionServer;
  ros::ServiceServer mGetVelocityServer;
  ros::ServiceServer mGetMaxVelocityServer;
  ros::ServiceServer mGetAccelerationServer;
  ros::ServiceServer mGetAvailableForceServer;
  ros::ServiceServer mGetMaxForceServer;
  ros::ServiceServer mGetAvailableTorqueServer;
  ros::ServiceServer mGetMaxTorqueServer;
  ros::ServiceServer mGetMultiplierServer;
  ros::ServiceServer mGetTypeServer;
  ros::ServiceServer mGetBrakeNameServer;
  ros::ServiceServer mGetPositionSensorNameServer;
  ros::Publisher mPublisher;
};

#endif  // ROS_MOTOR_HPP
