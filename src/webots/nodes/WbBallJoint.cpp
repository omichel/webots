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

#include "WbBallJoint.hpp"
#include "WbBallJointParameters.hpp"
#include "WbBrake.hpp"
#include "WbJointParameters.hpp"
#include "WbMathsUtilities.hpp"
#include "WbMotor.hpp"
#include "WbOdeContext.hpp"
#include "WbOdeUtilities.hpp"
#include "WbPositionSensor.hpp"
#include "WbRobot.hpp"
#include "WbRotationalMotor.hpp"
#include "WbSFNode.hpp"
#include "WbSolid.hpp"
#include "WbWorld.hpp"
#include "WbWrenRenderingContext.hpp"

#include <wren/config.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include <ode/ode.h>

// Constructors

void WbBallJoint::init() {
  mParameters3 = findSFNode("jointParameters3");
  mDevice3 = findMFNode("device3");

  // hidden field
  mPosition3 = findSFDouble("position3")->value();
  mOdePositionOffset3 = mPosition3;
  mSavedPositions3[stateId()] = mPosition3;

  mControlMotor = NULL;
}

WbBallJoint::WbBallJoint(WbTokenizer *tokenizer) : WbHinge2Joint("BallJoint", tokenizer) {
  init();
}

WbBallJoint::WbBallJoint(const WbBallJoint &other) : WbHinge2Joint(other) {
  init();
}

WbBallJoint::WbBallJoint(const WbNode &other) : WbHinge2Joint(other) {
  init();
}

WbBallJoint::~WbBallJoint() {
  if (mControlMotor)
    dJointDestroy(mControlMotor);
  mControlMotor = NULL;
}

WbBallJointParameters *WbBallJoint::ballJointParameters() const {
  return dynamic_cast<WbBallJointParameters *>(mParameters->value());
}

WbJointParameters *WbBallJoint::parameters3() const {
  return dynamic_cast<WbJointParameters *>(mParameters3->value());
}

WbMotor *WbBallJoint::motor3() const {
  WbMotor *motor = NULL;
  for (int i = 0; i < mDevice3->size(); ++i) {
    motor = dynamic_cast<WbMotor *>(mDevice3->item(i));
    if (motor)
      return motor;
  }
  return NULL;
}

WbPositionSensor *WbBallJoint::positionSensor3() const {
  WbPositionSensor *sensor = NULL;
  for (int i = 0; i < mDevice3->size(); ++i) {
    sensor = dynamic_cast<WbPositionSensor *>(mDevice3->item(i));
    if (sensor)
      return sensor;
  }
  return NULL;
}

WbBrake *WbBallJoint::brake3() const {
  WbBrake *brake = NULL;
  for (int i = 0; i < mDevice3->size(); ++i) {
    brake = dynamic_cast<WbBrake *>(mDevice3->item(i));
    if (brake)
      return brake;
  }
  return NULL;
}

WbRotationalMotor *WbBallJoint::rotationalMotor3() const {
  WbRotationalMotor *motor = NULL;
  for (int i = 0; i < mDevice3->size(); ++i) {
    motor = dynamic_cast<WbRotationalMotor *>(mDevice3->item(i));
    if (motor)
      return motor;
  }
  return NULL;
}

WbJointDevice *WbBallJoint::device3(int index) const {
  if (index >= 0 && mDevice3->size() > index)
    return dynamic_cast<WbJointDevice *>(mDevice3->item(index));
  return NULL;
}

int WbBallJoint::devices3Number() const {
  return mDevice3->size();
}

QVector<WbLogicalDevice *> WbBallJoint::devices() const {
  QVector<WbLogicalDevice *> devices;
  int i = 0;
  for (i = 0; i < devicesNumber(); ++i)
    devices.append(device(i));
  for (i = 0; i < devices2Number(); ++i)
    devices.append(device2(i));
  for (i = 0; i < devices3Number(); ++i)
    devices.append(device3(i));

  return devices;
}

WbVector3 WbBallJoint::anchor() const {
  static const WbVector3 ZERO(0.0, 0.0, 0.0);
  WbBallJointParameters *const p = ballJointParameters();
  return p ? p->anchor() : ZERO;
}

WbVector3 WbBallJoint::axis() const {
  const WbJointParameters *const p2 = parameters2();
  const WbJointParameters *const p3 = parameters3();
  if (!p2) {
    if (!p3)
      return WbVector3(1.0, 0.0, 0.0);
    else if (p3->axis().cross(WbVector3(0.0, 0.0, 1.0)).isNull())
      return p3->axis().cross(WbVector3(1.0, 0.0, 0.0));
    else
      return p3->axis().cross(WbVector3(0.0, 0.0, 1.0));
  }
  return p2->axis();
}

WbVector3 WbBallJoint::axis2() const {
  return axis3().cross(axis());
}

WbVector3 WbBallJoint::axis3() const {
  const WbJointParameters *const p2 = parameters2();
  const WbJointParameters *const p3 = parameters3();
  if (!p3) {
    if (!p2)
      return WbVector3(0.0, 0.0, 1.0);
    else if (p2->axis().cross(WbVector3(1.0, 0.0, 0.0)).isNull())
      return p2->axis().cross(WbVector3(0.0, 0.0, 1.0));
    else
      return p2->axis().cross(WbVector3(1.0, 0.0, 0.0));
  }
  return p3->axis();
}

void WbBallJoint::updateEndPointZeroTranslationAndRotation() {
  if (solidEndPoint() == NULL)
    return;

  WbRotation ir;
  WbVector3 it;
  retrieveEndPointSolidTranslationAndRotation(it, ir);

  WbQuaternion qp;
  if (WbMathsUtilities::isZeroAngle(mPosition) && WbMathsUtilities::isZeroAngle(mPosition2) &&
      WbMathsUtilities::isZeroAngle(mPosition3))
    mEndPointZeroRotation = ir;  // Keeps track of the original axis if the angle is zero as it defines the second DoF axis
  else {
    const WbQuaternion q(axis(), -mPosition);
    const WbQuaternion q2(axis2(), -mPosition2);
    const WbQuaternion q3(axis3(), -mPosition3);
    qp = q3 * q2 * q;
    const WbQuaternion &iq = ir.toQuaternion();
    WbQuaternion qr = qp * iq;
    qr.normalize();
    mEndPointZeroRotation = WbRotation(qr);
  }
  const WbVector3 &a = anchor();
  const WbVector3 t(it - a);
  mEndPointZeroTranslation = qp * t + a;
}

void WbBallJoint::computeEndPointSolidPositionFromParameters(WbVector3 &translation, WbRotation &rotation) const {
  WbQuaternion qp;
  const WbQuaternion q(axis(), mPosition);
  const WbQuaternion q2(axis2(), mPosition2);
  const WbQuaternion q3(axis3(), mPosition3);
  WbQuaternion qi = mEndPointZeroRotation.toQuaternion();
  qp = q * q2 * q3;
  const WbVector3 &a = anchor();
  const WbVector3 t(mEndPointZeroTranslation - a);
  translation = qp * t + a;
  qp = qp * qi;
  qp.normalize();
  rotation.fromQuaternion(qp);
}

void WbBallJoint::updatePosition() {
  const WbJointParameters *const p = parameters();
  const WbJointParameters *const p2 = parameters2();
  const WbJointParameters *const p3 = parameters3();

  if (solidReference() == NULL && solidEndPoint())
    updatePositions(p ? p->position() : mPosition, p2 ? p2->position() : mPosition2, p3 ? p3->position() : mPosition3);
}

void WbBallJoint::updatePositions(double position, double position2, double position3) {
  WbSolid *const s = solidEndPoint();
  assert(s);
  // called after an artificial move (user or Supervisor move) or in kinematic mode
  mPosition = position;
  mPosition2 = position2;
  mPosition3 = position3;
  WbMotor *m1 = motor();
  WbMotor *m2 = motor2();
  WbMotor *m3 = motor3();
  if (m1 && !m1->isConfigureDone())
    m1->setTargetPosition(position);
  if (m2 && !m2->isConfigureDone())
    m2->setTargetPosition(position2);
  if (m3 && !m3->isConfigureDone())
    m3->setTargetPosition(position3);
  WbVector3 translation;
  WbRotation rotation;
  computeEndPointSolidPositionFromParameters(translation, rotation);
  if (!translation.almostEquals(s->translation()) || !rotation.almostEquals(s->rotation())) {
    mIsEndPointPositionChangedByJoint = true;
    s->setTranslationAndRotation(translation, rotation);
    s->resetPhysics();
    mIsEndPointPositionChangedByJoint = false;
  }
}

void WbBallJoint::updatePosition(double position) {
  updatePositions(mPosition, mPosition2, mPosition3);
}

void WbBallJoint::updateParameters() {
  WbHinge2Joint::updateParameters();
  updateParameters3();
  if (ballJointParameters())
    connect(ballJointParameters(), &WbBallJointParameters::anchorChanged, this, &WbBallJoint::updateAnchor,
            Qt::UniqueConnection);
}

void WbBallJoint::checkMotorLimit() {
  WbMotor *motor = motor2();

  if (!motor)
    return;

  if (motor->minPosition() == 0.0 && motor->maxPosition() == 0.0) {
    motor->setMinPosition(-M_PI_2);
    motor->setMaxPosition(M_PI_2);
  } else {
    if (motor->minPosition() < -M_PI_2) {
      motor->setMinPosition(-M_PI_2);
      parsingWarn(tr("The lower limit of the motor associated to the second axis shouldn't be smaller than -pi/2."));
    }
    if (motor->maxPosition() > M_PI_2) {
      motor->setMaxPosition(M_PI_2);
      parsingWarn(tr("The upper limit of the motor associated to the second axis shouldn't be greater than pi/2."));
    }
  }
}

void WbBallJoint::addDevice2(int index) {
  WbHinge2Joint::addDevice2(index);
  const WbMotor *const motor = dynamic_cast<WbMotor *>(mDevice2->item(index));
  if (motor) {
    checkMotorLimit();
    connect(motor, &WbMotor::minPositionChanged, this, &WbBallJoint::checkMotorLimit, Qt::UniqueConnection);
    connect(motor, &WbMotor::maxPositionChanged, this, &WbBallJoint::checkMotorLimit, Qt::UniqueConnection);
  }
}

void WbBallJoint::addDevice3(int index) {
  const WbSolid *const s = upperSolid();
  if (s) {
    WbRobot *const r = s->robot();
    assert(r);
    WbBaseNode *decendant = dynamic_cast<WbBaseNode *>(mDevice3->item(index));
    r->descendantNodeInserted(decendant);
  }
  const WbBrake *const brake = dynamic_cast<WbBrake *>(mDevice3->item(index));
  if (brake)
    connect(brake, &WbBrake::brakingChanged, this, &WbBallJoint::updateSpringAndDampingConstants, Qt::UniqueConnection);
}

void WbBallJoint::updateParameters3() {
  const WbJointParameters *const p3 = parameters3();
  if (p3) {
    mOdePositionOffset3 = p3->position();
    mPosition3 = mOdePositionOffset3;
    connect(p3, &WbJointParameters::minAndMaxStopChanged, this, &WbBallJoint::updateMinAndMaxStop, Qt::UniqueConnection);
    connect(p3, &WbJointParameters::springAndDampingConstantsChanged, this, &WbBallJoint::updateSpringAndDampingConstants,
            Qt::UniqueConnection);
    connect(p3, &WbJointParameters::axisChanged, this, &WbBallJoint::updateAxis, Qt::UniqueConnection);
    connect(p3, SIGNAL(positionChanged()), this, SLOT(updatePosition()), Qt::UniqueConnection);
  }
}

bool WbBallJoint::setJoint() {
  if (!WbBasicJoint::setJoint())
    return false;

  if (mJoint == NULL)
    mJoint = dJointCreateBall(WbOdeContext::instance()->world(), 0);

  if (mControlMotor == NULL) {
    mControlMotor = dJointCreateAMotor(WbOdeContext::instance()->world(), 0);
    dJointSetAMotorNumAxes(mControlMotor, 3);
    dJointSetAMotorMode(mControlMotor, dAMotorEuler);
    dJointSetAMotorAngle(mControlMotor, 0, 0.0);
    dJointSetAMotorAngle(mControlMotor, 1, 0.0);
    dJointSetAMotorAngle(mControlMotor, 2, 0.0);
  }

  const WbSolid *const s = solidEndPoint();
  dBodyID body = s ? s->body() : NULL;
  dBodyID parentBody = upperSolid()->bodyMerger();
  dJointAttach(mControlMotor, parentBody, body);
  if (parentBody == NULL && body == NULL)
    dJointDisable(mControlMotor);
  else
    dJointEnable(mControlMotor);
  setOdeJoint(body, parentBody);

  return true;
}

double WbBallJoint::position(int index) const {
  switch (index) {
    case 1:
      return mPosition;
    case 2:
      return mPosition2;
    case 3:
      return mPosition3;
    default:
      return NAN;
  }
}

double WbBallJoint::initialPosition(int index) const {
  switch (index) {
    case 1:
      return mSavedPositions[stateId()];
    case 2:
      return mSavedPositions2[stateId()];
    case 3:
      return mSavedPositions3[stateId()];
    default:
      return NAN;
  }
}

void WbBallJoint::setPosition(double position, int index) {
  WbHinge2Joint::setPosition(position, index);

  if (index != 3)
    return;

  mPosition3 = position;
  mOdePositionOffset3 = position;
  WbJointParameters *const p3 = parameters3();
  if (p3)
    p3->setPosition(mPosition3);

  WbMotor *const m3 = motor3();
  if (m3)
    m3->setTargetPosition(position);
}

bool WbBallJoint::resetJointPositions() {
  mOdePositionOffset3 = 0.0;
  return WbHinge2Joint::resetJointPositions();
}

void WbBallJoint::updateOdePositionOffset() {
  WbHinge2Joint::updateOdePositionOffset();
  mOdePositionOffset3 = position(3);
}

void WbBallJoint::preFinalize() {
  WbHinge2Joint::preFinalize();

  WbBallJointParameters *const p = ballJointParameters();
  if (p && !p->isPreFinalizedCalled())
    p->preFinalize();

  for (int i = 0; i < devices3Number(); ++i) {
    if (device3(i) && !device3(i)->isPreFinalizedCalled())
      device3(i)->preFinalize();
  }

  WbBaseNode *const p3 = dynamic_cast<WbBaseNode *>(mParameters3->value());
  if (p3 && !p3->isPreFinalizedCalled())
    p3->preFinalize();

  updateParameters3();
  checkMotorLimit();

  mSavedPositions3["__init__"] = mPosition3;
}

void WbBallJoint::postFinalize() {
  WbHinge2Joint::postFinalize();

  WbBallJointParameters *const p = ballJointParameters();

  if (p && !p->isPostFinalizedCalled())
    p->postFinalize();
  for (int i = 0; i < devices3Number(); ++i) {
    if (device3(i) && !device3(i)->isPostFinalizedCalled())
      device3(i)->postFinalize();
  }

  WbBaseNode *const p3 = dynamic_cast<WbBaseNode *>(mParameters3->value());
  if (p3 && !p3->isPostFinalizedCalled())
    p3->postFinalize();

  connect(mDevice3, &WbMFNode::itemInserted, this, &WbBallJoint::addDevice3);
  connect(mParameters3, &WbSFNode::changed, this, &WbBallJoint::updateParameters);
  if (p)
    connect(p, &WbBallJointParameters::anchorChanged, this, &WbBallJoint::updateAnchor, Qt::UniqueConnection);
  if (brake3())
    connect(brake3(), &WbBrake::brakingChanged, this, &WbBallJoint::updateSpringAndDampingConstants, Qt::UniqueConnection);
  if (motor2()) {
    connect(motor2(), &WbMotor::minPositionChanged, this, &WbBallJoint::checkMotorLimit, Qt::UniqueConnection);
    connect(motor2(), &WbMotor::maxPositionChanged, this, &WbBallJoint::checkMotorLimit, Qt::UniqueConnection);
  }
}

void WbBallJoint::applyToOdeSpringAndDampingConstants(dBodyID body, dBodyID parentBody) {
  const WbBallJointParameters *const p = ballJointParameters();
  const WbJointParameters *const p2 = parameters2();
  const WbJointParameters *const p3 = parameters3();

  const double brakingDampingConstant = brake() ? brake()->getBrakingDampingConstant() : 0.0;
  const double brakingDampingConstant2 = brake2() ? brake2()->getBrakingDampingConstant() : 0.0;
  const double brakingDampingConstant3 = brake3() ? brake3()->getBrakingDampingConstant() : 0.0;

  if ((p == NULL && p2 == NULL && p3 == NULL && brakingDampingConstant == 0.0 && brakingDampingConstant2 == 0.0 &&
       brakingDampingConstant3 == 0.0) ||
      (body == NULL && parentBody == NULL)) {
    if (mSpringAndDamperMotor) {
      dJointDestroy(mSpringAndDamperMotor);
      mSpringAndDamperMotor = NULL;
    }
    return;
  }
  assert((body || parentBody) && (p || p2 || p3 || brake() || brake2() || brake3()));

  double s = p ? p->springConstant() : 0.0;
  double d = p ? p->dampingConstant() : 0.0;
  double s2 = p2 ? p2->springConstant() : 0.0;
  double d2 = p2 ? p2->dampingConstant() : 0.0;
  double s3 = p3 ? p3->springConstant() : 0.0;
  double d3 = p3 ? p3->dampingConstant() : 0.0;

  if (p) {  // homogeneous case
    if (!p2) {
      s2 = s;
      d2 = d;
    }
    if (!p3) {
      s3 = s;
      d3 = d;
    }
  }

  d += brakingDampingConstant;
  d2 += brakingDampingConstant2;
  d3 += brakingDampingConstant3;

  if (s == 0.0 && d == 0.0 && s2 == 0.0 && d2 == 0.0 && s3 == 0.0 && d3 == 0.0) {
    if (mSpringAndDamperMotor) {
      dJointDestroy(mSpringAndDamperMotor);
      mSpringAndDamperMotor = NULL;
    }
    return;
  }

  // Handles scale
  const WbTransform *const ut = upperTransform();
  const double scale = ut->absoluteScale().x();
  double s4 = scale * scale;
  s4 *= scale;
  s *= s4;
  d *= s4;

  const WbWorldInfo *const wi = WbWorld::instance()->worldInfo();
  double cfm, erp, cfm2, erp2, cfm3, erp3;
  WbOdeUtilities::convertSpringAndDampingConstants(s, d, wi->basicTimeStep() * 0.001, cfm, erp);
  WbOdeUtilities::convertSpringAndDampingConstants(s2, d2, wi->basicTimeStep() * 0.001, cfm2, erp2);
  WbOdeUtilities::convertSpringAndDampingConstants(s3, d3, wi->basicTimeStep() * 0.001, cfm3, erp3);

  const dWorldID world = parentBody ? dBodyGetWorld(parentBody) : dBodyGetWorld(body);
  if (mSpringAndDamperMotor == NULL)
    mSpringAndDamperMotor = dJointCreateAMotor(world, 0);

  dJointAttach(mSpringAndDamperMotor, parentBody, body);
  dJointSetAMotorMode(mSpringAndDamperMotor, dAMotorEuler);

  // Axis dependent settings
  const WbMatrix4 &m4 = upperTransform()->matrix();
  const double clamped = WbMathsUtilities::normalizeAngle(mOdePositionOffset);
  const WbVector3 &a1 = m4.sub3x3MatrixDot(axis());
  dJointSetAMotorAxis(mSpringAndDamperMotor, 0, mIsReverseJoint ? 2 : 1, a1.x(), a1.y(), a1.z());
  dJointSetAMotorAngle(mSpringAndDamperMotor, 0, 0.0);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamLoStop, clamped);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamHiStop, clamped);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopCFM, cfm);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopERP, erp);

  const double clamped2 = WbMathsUtilities::normalizeAngle(mOdePositionOffset2);
  dJointSetAMotorAngle(mSpringAndDamperMotor, 1, 0.0);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamLoStop2, clamped2);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamHiStop2, clamped2);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopCFM2, cfm2);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopERP2, erp2);

  const double clamped3 = WbMathsUtilities::normalizeAngle(mOdePositionOffset3);
  const WbVector3 &a3 = m4.sub3x3MatrixDot(axis3());
  dJointSetAMotorAxis(mSpringAndDamperMotor, 2, mIsReverseJoint ? 1 : 2, a3.x(), a3.y(), a3.z());
  dJointSetAMotorAngle(mSpringAndDamperMotor, 2, 0.0);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamLoStop3, clamped3);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamHiStop3, clamped3);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopCFM3, cfm3);
  dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopERP3, erp3);
}

void WbBallJoint::prePhysicsStep(double ms) {
  assert(solidEndPoint());
  WbRotationalMotor *const rm = rotationalMotor();
  WbRotationalMotor *const rm2 = rotationalMotor2();
  WbRotationalMotor *const rm3 = rotationalMotor3();
  WbJointParameters *const p = parameters();
  WbJointParameters *const p2 = parameters2();
  WbJointParameters *const p3 = parameters3();

  if (isEnabled()) {
    const double s = upperTransform()->absoluteScale().x();
    double s5 = s * s;
    s5 *= s5 * s;

    if (rm && rm->userControl())
      // user-defined torque
      dJointAddAMotorTorques(mJoint, -rm->rawInput(), 0.0, 0.0);
    else {
      // ODE motor torque (user velocity/position control)
      const double currentVelocity = rm ? rm->computeCurrentDynamicVelocity(ms, mPosition) : 0.0;
      const double fMax = qMax(p ? p->staticFriction() : 0.0, rm ? rm->torque() : 0.0);
      dJointSetAMotorParam(mControlMotor, dParamFMax, s5 * fMax);
      dJointSetAMotorParam(mControlMotor, dParamVel, currentVelocity);
    }

    if (rm2 && rm2->userControl())
      // user-defined torque
      dJointAddAMotorTorques(mJoint, 0.0, -rm2->rawInput(), 0.0);
    else {
      // ODE motor torque (user velocity/position control)
      const double currentVelocity2 = rm2 ? rm2->computeCurrentDynamicVelocity(ms, mPosition2) : 0.0;
      const double fMax2 = qMax(p2 ? p2->staticFriction() : 0.0, rm2 ? rm2->torque() : 0.0);
      dJointSetAMotorParam(mControlMotor, dParamFMax2, s5 * fMax2);
      dJointSetAMotorParam(mControlMotor, dParamVel2, currentVelocity2);
    }

    if (rm3 && rm3->userControl())
      // user-defined torque
      dJointAddAMotorTorques(mJoint, 0.0, 0.0, -rm3->rawInput());
    else {
      // ODE motor torque (user velocity/position control)
      const double currentVelocity3 = rm3 ? rm3->computeCurrentDynamicVelocity(ms, mPosition3) : 0.0;
      const double fMax3 = qMax(p3 ? p3->staticFriction() : 0.0, rm3 ? rm3->torque() : 0.0);
      dJointSetAMotorParam(mControlMotor, dParamFMax3, s5 * fMax3);
      dJointSetAMotorParam(mControlMotor, dParamVel3, currentVelocity3);
    }

    // eventually add spring and damping forces
    if (mSpringAndDamperMotor) {
      dJointSetAMotorAngle(mSpringAndDamperMotor, 0, -dJointGetAMotorAngle(mControlMotor, 0));
      dJointSetAMotorAngle(mSpringAndDamperMotor, 1, -dJointGetAMotorAngle(mControlMotor, 1));
      dJointSetAMotorAngle(mSpringAndDamperMotor, 2, -dJointGetAMotorAngle(mControlMotor, 2));
    }
  } else {
    const bool run1 = rm && rm->runKinematicControl(ms, mPosition);
    if (run1 && p)
      p->setPosition(mPosition);

    const bool run2 = rm2 && rm2->runKinematicControl(ms, mPosition2);
    if (run2 && p2)
      p2->setPosition(mPosition2);

    const bool run3 = rm3 && rm3->runKinematicControl(ms, mPosition3);
    if (run3 && p3)
      p3->setPosition(mPosition3);

    if (run1 || run2 || run3)
      updatePositions(mPosition, mPosition2, mPosition3);
  }
  mTimeStep = ms;
}

void WbBallJoint::postPhysicsStep() {
  assert(mJoint);
  const WbMotor *const m1 = motor();
  if (m1 && m1->isPIDPositionControl())
    // if controlling in position we update position using directly the angle feedback
    mPosition = WbMathsUtilities::normalizeAngle(-dJointGetAMotorAngle(mControlMotor, 0) + mOdePositionOffset, mPosition);
  else
    // if not controlling in position we use the angle rate feedback to update position (because at high speed angle feedback is
    // under-estimated)
    mPosition -= dJointGetAMotorAngle(mControlMotor, 0) * mTimeStep / 1000.0;
  WbJointParameters *const p = parameters();
  if (p)
    p->setPositionFromOde(mPosition);

  const WbMotor *const m2 = motor2();
  if (m2 && m2->isPIDPositionControl())
    mPosition2 = WbMathsUtilities::normalizeAngle(-dJointGetAMotorAngle(mControlMotor, 1) + mOdePositionOffset2, mPosition2);
  else
    mPosition2 -= dJointGetAMotorAngle(mControlMotor, 1) * mTimeStep / 1000.0;
  WbJointParameters *const p2 = parameters2();
  if (p2)
    p2->setPositionFromOde(mPosition2);

  const WbMotor *const m3 = motor3();
  if (m3 && m3->isPIDPositionControl())
    mPosition3 = WbMathsUtilities::normalizeAngle(-dJointGetAMotorAngle(mControlMotor, 2) + mOdePositionOffset3, mPosition3);
  else
    mPosition3 -= dJointGetAMotorAngle(mControlMotor, 2) * mTimeStep / 1000.0;
  WbJointParameters *const p3 = parameters3();
  if (p3)
    p3->setPositionFromOde(mPosition3);
}

void WbBallJoint::reset(const QString &id) {
  WbHinge2Joint::reset(id);

  for (int i = 0; i < mDevice3->size(); ++i)
    mDevice3->item(i)->reset(id);

  WbNode *const p = mParameters3->value();
  if (p)
    p->reset(id);

  setPosition(mSavedPositions3[id], 3);
}

void WbBallJoint::resetPhysics() {
  WbHinge2Joint::resetPhysics();

  WbMotor *const m = motor3();
  if (m)
    m->resetPhysics();
}

void WbBallJoint::save(const QString &id) {
  WbHinge2Joint::save(id);

  for (int i = 0; i < mDevice3->size(); ++i)
    mDevice3->item(i)->save(id);

  WbNode *const p = mParameters3->value();
  if (p)
    p->save(id);

  mSavedPositions3[id] = mPosition3;
}

void WbBallJoint::applyToOdeAxis() {
  assert(mJoint);

  WbVector3 referenceAxis = axis();
  WbVector3 referenceAxis3 = axis3();

  if (referenceAxis.cross(referenceAxis3).isNull()) {
    parsingWarn(tr("Axes are aligned: using x and z axes instead."));
    referenceAxis = WbVector3(1.0, 0.0, 0.0);
    referenceAxis3 = WbVector3(0.0, 0.0, 1.0);
  }

  if (mIsReverseJoint)
    referenceAxis = -referenceAxis;

  dJointSetAMotorAxis(mControlMotor, 0, mIsReverseJoint ? 2 : 1, referenceAxis.x(), referenceAxis.y(), referenceAxis.z());
  // axis 1 is computed by ODE
  dJointSetAMotorAxis(mControlMotor, 2, mIsReverseJoint ? 1 : 2, referenceAxis3.x(), referenceAxis3.y(), referenceAxis3.z());

  updateOdePositionOffset();

  if (!mSpringAndDamperMotor)
    return;

  const WbMatrix4 &m4 = upperTransform()->matrix();
  const WbVector3 &a1 = m4.sub3x3MatrixDot(axis());
  const WbVector3 &a3 = m4.sub3x3MatrixDot(axis3());
  if (!a1.cross(a3).isNull()) {
    dJointSetAMotorAxis(mSpringAndDamperMotor, 0, mIsReverseJoint ? 2 : 1, a1.x(), a1.y(), a1.z());
    dJointSetAMotorAxis(mSpringAndDamperMotor, 2, mIsReverseJoint ? 1 : 2, a3.x(), a3.y(), a3.z());
  } else {
    dJointSetAMotorAxis(mSpringAndDamperMotor, 0, mIsReverseJoint ? 2 : 1, 1.0, 0.0, 0.0);
    dJointSetAMotorAxis(mSpringAndDamperMotor, 2, mIsReverseJoint ? 1 : 2, 0.0, 0.0, 1.0);
  }
}

void WbBallJoint::applyToOdeMinAndMaxStop() {
  assert(mControlMotor);
  // place hard stops if defined
  const WbJointParameters *const p = parameters();
  double m = p ? p->minStop() : 0.0;
  double M = p ? p->maxStop() : 0.0;
  if (m != M) {
    double min = -M + mOdePositionOffset;
    double max = -m + mOdePositionOffset;
    WbMathsUtilities::clampAngles(min, max);
    dJointSetAMotorParam(mControlMotor, dParamLoStop, min);
    dJointSetAMotorParam(mControlMotor, dParamHiStop, max);
  }

  const WbJointParameters *const p2 = parameters2();
  m = p2 ? p2->minStop() : 0.0;
  M = p2 ? p2->maxStop() : 0.0;
  if (m != M) {
    double min = -M + mOdePositionOffset2;
    double max = -m + mOdePositionOffset2;
    WbMathsUtilities::clampAngles(min, max);
    dJointSetAMotorParam(mControlMotor, dParamLoStop2, min);
    dJointSetAMotorParam(mControlMotor, dParamHiStop2, max);
  }

  const WbJointParameters *const p3 = parameters3();
  m = p3 ? p3->minStop() : 0.0;
  M = p3 ? p3->maxStop() : 0.0;
  if (m != M) {
    double min = -M + mOdePositionOffset3;
    double max = -m + mOdePositionOffset3;
    WbMathsUtilities::clampAngles(min, max);
    dJointSetAMotorParam(mControlMotor, dParamLoStop3, min);
    dJointSetAMotorParam(mControlMotor, dParamHiStop3, max);
  }
}

//////////
// WREN //
//////////

void WbBallJoint::createWrenObjects() {
  WbHinge2Joint::createWrenObjects();

  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_JOINT_AXES))
    wr_node_set_visible(WR_NODE(mTransform), true);

  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this,
          &WbBallJoint::updateJointAxisRepresentation);

  updateJointAxisRepresentation();
}

void WbBallJoint::updateJointAxisRepresentation() {
  if (!areWrenObjectsInitialized())
    return;

  wr_static_mesh_delete(mMesh);

  float anchorArray[3];
  anchor().toFloatArray(anchorArray);
  const WbVector3 a1 = axis().normalized();
  const WbVector3 a2 = axis2().normalized();
  const WbVector3 a3 = axis3().normalized();

  const float scaling = 0.5f * wr_config_get_line_scale();
  const float vertices[18] = {anchorArray[0] - scaling * (float)a1.x(), anchorArray[1] - scaling * (float)a1.y(),
                              anchorArray[2] - scaling * (float)a1.z(), anchorArray[0] + scaling * (float)a1.x(),
                              anchorArray[1] + scaling * (float)a1.y(), anchorArray[2] + scaling * (float)a1.z(),
                              anchorArray[0] - scaling * (float)a2.x(), anchorArray[1] - scaling * (float)a2.y(),
                              anchorArray[2] - scaling * (float)a2.z(), anchorArray[0] + scaling * (float)a2.x(),
                              anchorArray[1] + scaling * (float)a2.y(), anchorArray[2] + scaling * (float)a2.z(),
                              anchorArray[0] - scaling * (float)a3.x(), anchorArray[1] - scaling * (float)a3.y(),
                              anchorArray[2] - scaling * (float)a3.z(), anchorArray[0] + scaling * (float)a3.x(),
                              anchorArray[1] + scaling * (float)a3.y(), anchorArray[2] + scaling * (float)a3.z()};
  mMesh = wr_static_mesh_line_set_new(6, vertices, NULL);
  wr_renderable_set_mesh(mRenderable, WR_MESH(mMesh));
}

void WbBallJoint::writeExport(WbVrmlWriter &writer) const {
  if (writer.isUrdf() && solidEndPoint()) {
    warn(tr("Exporting 'BallJoint' nodes to URDF is currently not supported"));
    return;
  }
  WbBasicJoint::writeExport(writer);
}
