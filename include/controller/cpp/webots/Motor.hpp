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

#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <webots/Device.hpp>

#include <math.h>  // for 'INFINITY'

namespace webots {
  class Brake;
  class PositionSensor;

  class Motor : public Device {
  public:
    typedef enum { ROTATIONAL = 0, LINEAR } Type;

    explicit Motor(const std::string &name) :
      Device(name),
      brake(NULL),
      positionSensor(NULL) {}  // Use Robot::getMotor() instead
    virtual ~Motor() {}

    virtual void setPosition(double position);
    virtual void setVelocity(double vel);
    virtual void setAcceleration(double acceleration);
    virtual void setAvailableForce(double availableForce);
    virtual void setAvailableTorque(double availableTorque);
    virtual void setControlPID(double p, double i, double d);

    double getTargetPosition() const;
    double getMinPosition() const;
    double getMaxPosition() const;
    double getVelocity() const;
    double getMaxVelocity() const;
    double getAcceleration() const;
    double getAvailableForce() const;
    double getMaxForce() const;
    double getAvailableTorque() const;
    double getMaxTorque() const;
    double getMultiplier() const;

    virtual void enableForceFeedback(int samplingPeriod);
    virtual void disableForceFeedback();
    int getForceFeedbackSamplingPeriod() const;
    double getForceFeedback() const;

    virtual void enableTorqueFeedback(int samplingPeriod);
    virtual void disableTorqueFeedback();
    int getTorqueFeedbackSamplingPeriod() const;
    double getTorqueFeedback() const;

    virtual void setForce(double force);
    virtual void setTorque(double torque);

    Type getType() const;

    Brake *getBrake();
    PositionSensor *getPositionSensor();

    // internal functions
    int getBrakeTag() const;
    int getPositionSensorTag() const;

  private:
    Brake *brake;
    PositionSensor *positionSensor;
  };
}  // namespace webots

#endif  // MOTOR_HPP
