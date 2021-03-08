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

// Description: Webots integration with for `ros_control`.

#include "RosControl.hpp"

namespace highlevel {

  RosControl::RosControl(webots::Robot *robot, ros::NodeHandle *nodeHandle) {
    mWebotsHw = new WebotsHw(robot);
    mControllerManager = new controller_manager::ControllerManager(mWebotsHw, *nodeHandle);
    mLastUpdate = ros::Time::now();
  }

  void RosControl::read() {
    mWebotsHw->read();
    mControllerManager->update(ros::Time::now(), ros::Time::now() - mLastUpdate);
    mLastUpdate = ros::Time::now();
  }

  void RosControl::write() { mWebotsHw->write(); }
}  // namespace highlevel
