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
#include "WbHinge2Joint.hpp"

#include "WbBrake.hpp"
#include "WbJointParameters.hpp"
#include "WbMathsUtilities.hpp"
#include "WbOdeContext.hpp"
#include "WbOdeUtilities.hpp"
#include "WbPositionSensor.hpp"
#include "WbRobot.hpp"
#include "WbRotationalMotor.hpp"
#include "WbSolid.hpp"
#include "WbWorld.hpp"
#include "WbWrenRenderingContext.hpp"

#include <wren/config.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

// Constructors

void WbHinge2Joint::init() {
  mParameters2 = findSFNode("jointParameters2");
  mDevice2 = findMFNode("device2");

  // spring and dampingConstant
  mSpringAndDampingConstantsAxis1On = false;
  mSpringAndDampingConstantsAxis2On = false;

  // hidden field
  mPosition2 = findSFDouble("position2")->value();
  mOdePositionOffset2 = mPosition2;
  mSavedPositions2[stateId()] = mPosition2;
}

WbHinge2Joint::WbHinge2Joint(const QString &modelName, WbTokenizer *tokenizer) : WbHingeJoint(modelName, tokenizer) {
  init();
}

WbHinge2Joint::WbHinge2Joint(WbTokenizer *tokenizer) : WbHingeJoint("Hinge2Joint", tokenizer) {
  init();
}

WbHinge2Joint::WbHinge2Joint(const WbHinge2Joint &other) : WbHingeJoint(other) {
  init();
}

WbHinge2Joint::WbHinge2Joint(const WbNode &other) : WbHingeJoint(other) {
  init();
}

WbHinge2Joint::~WbHinge2Joint() {
}

void WbHinge2Joint::preFinalize() {
  WbHingeJoint::preFinalize();

  for (int i = 0; i < devices2Number(); ++i) {
    if (device2(i) && !device2(i)->isPreFinalizedCalled())
      device2(i)->preFinalize();
  }

  WbBaseNode *const p2 = dynamic_cast<WbBaseNode *>(mParameters2->value());
  if (p2 && !p2->isPreFinalizedCalled())
    p2->preFinalize();

  updateParameters2();

  mSavedPositions2[stateId()] = mPosition2;
}

void WbHinge2Joint::postFinalize() {
  WbHingeJoint::postFinalize();

  for (int i = 0; i < devices2Number(); ++i) {
    if (device2(i) && !device2(i)->isPostFinalizedCalled())
      device2(i)->postFinalize();
  }

  WbBaseNode *const p2 = dynamic_cast<WbBaseNode *>(mParameters2->value());
  if (p2 && !p2->isPostFinalizedCalled())
    p2->postFinalize();

  connect(mDevice2, &WbMFNode::itemInserted, this, &WbHinge2Joint::addDevice2);
  connect(mParameters2, &WbSFNode::changed, this, &WbHinge2Joint::updateParameters);
  if (brake2())
    connect(brake2(), &WbBrake::brakingChanged, this, &WbHinge2Joint::updateSpringAndDampingConstants, Qt::UniqueConnection);
}

bool WbHinge2Joint::setJoint() {
  if (!WbBasicJoint::setJoint())
    return false;

  if (mJoint == NULL)
    mJoint = dJointCreateHinge2(WbOdeContext::instance()->world(), 0);

  const WbSolid *const s = solidEndPoint();
  dBodyID body = s ? s->body() : NULL;
  dBodyID parentBody = upperSolid()->bodyMerger();
  if (body && parentBody)
    setOdeJoint(body, parentBody);
  else {
    parsingWarn(tr("Hinge2Joint nodes can only connect Solid nodes that have a Physics node."));
    return false;
  }

  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_JOINT_AXES))
    updateJointAxisRepresentation();

  return true;
}

double WbHinge2Joint::position(int index) const {
  switch (index) {
    case 1:
      return mPosition;
    case 2:
      return mPosition2;
    default:
      return NAN;
  }
}

double WbHinge2Joint::initialPosition(int index) const {
  switch (index) {
    case 1:
      return mSavedPositions[stateId()];
    case 2:
      return mSavedPositions2[stateId()];
    default:
      return NAN;
  }
}

void WbHinge2Joint::setPosition(double position, int index) {
  WbJoint::setPosition(position, index);

  if (index != 2)
    return;

  mPosition2 = position;
  mOdePositionOffset2 = position;
  WbJointParameters *const p2 = parameters2();
  if (p2)
    p2->setPosition(mPosition2);
  WbMotor *const m2 = motor2();
  if (m2)
    m2->setTargetPosition(position);
}

bool WbHinge2Joint::resetJointPositions() {
  mOdePositionOffset2 = 0.0;
  return WbJoint::resetJointPositions();
}

void WbHinge2Joint::updateOdePositionOffset() {
  WbJoint::updateOdePositionOffset();
  mOdePositionOffset2 = position(2);
}

void WbHinge2Joint::applyToOdeAxis() {
  assert(mJoint);

  updateOdePositionOffset();

  const WbMatrix4 &m4 = upperTransform()->matrix();
  // compute orientation of rotation axis
  const WbVector3 &a1 = m4.sub3x3MatrixDot(axis());
  WbVector3 a2;
  if (mPosition == 0.0)
    a2 = m4.sub3x3MatrixDot(axis2());
  else {
    // compute axis2 based on axis1 rotation
    WbMatrix3 a1Matrix(axis(), mPosition);
    a2 = (m4.extracted3x3Matrix() * a1Matrix) * axis2();
  }
  const WbVector3 &c = a1.cross(a2);
  if (!c.isNull()) {
    dJointSetHinge2Axis1(mJoint, a1.x(), a1.y(), a1.z());
    dJointSetHinge2Axis2(mJoint, a2.x(), a2.y(), a2.z());
    if (mSpringAndDamperMotor) {
      if (mSpringAndDampingConstantsAxis1On && mSpringAndDampingConstantsAxis2On) {
        // axes 0 and 1 of the AMotorAngle are enabled
        dJointSetAMotorAxis(mSpringAndDamperMotor, 0, 1, a1.x(), a1.y(), a1.z());
        dJointSetAMotorAxis(mSpringAndDamperMotor, 1, 1, a2.x(), a2.y(), a2.z());
      } else if (mSpringAndDampingConstantsAxis1On) {
        // only axis 0 of the AMotorAngle is enabled
        dJointSetAMotorAxis(mSpringAndDamperMotor, 0, 1, a1.x(), a1.y(), a1.z());
      } else
        dJointSetAMotorAxis(mSpringAndDamperMotor, 0, 1, a2.x(), a2.y(), a2.z());
    }
  } else {
    parsingWarn(tr("Hinge axes are aligned: using x and z axes instead."));
    dJointSetHinge2Axis1(mJoint, 1.0, 0.0, 0.0);
    dJointSetHinge2Axis2(mJoint, 0.0, 0.0, 1.0);
    if (mSpringAndDamperMotor) {
      dJointSetAMotorAxis(mSpringAndDamperMotor, 0, 1, 1.0, 0.0, 0.0);
      dJointSetAMotorAxis(mSpringAndDamperMotor, 1, 1, 0.0, 0.0, 1.0);
    }
  }
}

void WbHinge2Joint::applyToOdeMinAndMaxStop() {
  assert(mJoint);
  // place hard stops if defined
  const WbJointParameters *const p = parameters();
  double m = p ? p->minStop() : 0.0;
  double M = p ? p->maxStop() : 0.0;
  if (m != M) {
    double min = -M + mOdePositionOffset;
    double max = -m + mOdePositionOffset;
    WbMathsUtilities::clampAngles(min, max);
    dJointSetHinge2Param(mJoint, dParamLoStop, min);
    dJointSetHinge2Param(mJoint, dParamHiStop, max);
  }

  const WbJointParameters *const p2 = parameters2();
  m = p2 ? p2->minStop() : 0.0;
  M = p2 ? p2->maxStop() : 0.0;
  if (m != M) {
    double min = -M + mOdePositionOffset2;
    double max = -m + mOdePositionOffset2;
    WbMathsUtilities::clampAngles(min, max);
    dJointSetHinge2Param(mJoint, dParamLoStop2, min);
    dJointSetHinge2Param(mJoint, dParamHiStop2, max);
  }
}

void WbHinge2Joint::applyToOdeSpringAndDampingConstants(dBodyID body, dBodyID parentBody) {
  const WbJointParameters *const p = parameters();
  const WbJointParameters *const p2 = parameters2();

  const double brakingDampingConstant = brake() ? brake()->getBrakingDampingConstant() : 0.0;
  const double brakingDampingConstant2 = brake2() ? brake2()->getBrakingDampingConstant() : 0.0;

  if ((p == NULL && p2 == NULL && brakingDampingConstant == 0.0 && brakingDampingConstant2 == 0.0) ||
      (body == NULL && parentBody == NULL)) {
    if (mSpringAndDamperMotor) {
      dJointDestroy(mSpringAndDamperMotor);
      mSpringAndDamperMotor = NULL;
    }
    return;
  }
  assert((body || parentBody) && (p || p2 || brake() || brake2()));

  double s = p ? p->springConstant() : 0.0;
  double d = p ? p->dampingConstant() : 0.0;
  double s2 = p2 ? p2->springConstant() : 0.0;
  double d2 = p2 ? p2->dampingConstant() : 0.0;

  d += brakingDampingConstant;
  d2 += brakingDampingConstant2;

  mSpringAndDampingConstantsAxis1On = s != 0.0 || d != 0.0;
  mSpringAndDampingConstantsAxis2On = s2 != 0.0 || d2 != 0.0;

  if (!mSpringAndDampingConstantsAxis1On && !mSpringAndDampingConstantsAxis2On) {
    if (mSpringAndDamperMotor) {
      dJointDestroy(mSpringAndDamperMotor);
      mSpringAndDamperMotor = NULL;
    }
    return;
  }

  // Handles scale
  const double scale = upperTransform()->absoluteScale().x();
  double s4 = scale * scale;
  s4 *= scale;
  s *= s4;
  d *= s4;
  s2 *= s4;
  d2 *= s4;

  double cfm, erp, cfm2, erp2;
  const WbWorldInfo *const wi = WbWorld::instance()->worldInfo();
  const double t = wi->basicTimeStep() * 0.001;
  WbOdeUtilities::convertSpringAndDampingConstants(s, d, t, cfm, erp);
  WbOdeUtilities::convertSpringAndDampingConstants(s2, d2, t, cfm2, erp2);

  const dWorldID world = parentBody ? dBodyGetWorld(parentBody) : dBodyGetWorld(body);
  if (mSpringAndDamperMotor == NULL)
    mSpringAndDamperMotor = dJointCreateAMotor(world, 0);

  dJointAttach(mSpringAndDamperMotor, parentBody, body);

  const bool bothAxes = mSpringAndDampingConstantsAxis1On && mSpringAndDampingConstantsAxis2On;
  const int numberOfAxes = bothAxes ? 2 : 1;

  dJointSetAMotorNumAxes(mSpringAndDamperMotor, numberOfAxes);
  dJointSetAMotorMode(mSpringAndDamperMotor, dAMotorUser);

  // Axis dependent settings
  const WbMatrix4 &m4 = upperTransform()->matrix();
  if (mSpringAndDampingConstantsAxis1On) {
    const double clamped = WbMathsUtilities::normalizeAngle(mOdePositionOffset);
    const WbVector3 &a1 = m4.sub3x3MatrixDot(axis());
    dJointSetAMotorAxis(mSpringAndDamperMotor, 0, 1, a1.x(), a1.y(), a1.z());
    dJointSetAMotorAngle(mSpringAndDamperMotor, 0, 0.0);
    dJointSetAMotorParam(mSpringAndDamperMotor, dParamLoStop, clamped);
    dJointSetAMotorParam(mSpringAndDamperMotor, dParamHiStop, clamped);
    dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopCFM, cfm);
    dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopERP, erp);
  }

  if (mSpringAndDampingConstantsAxis2On) {
    const double clamped2 = WbMathsUtilities::normalizeAngle(mOdePositionOffset2);
    const WbVector3 &a2 = m4.sub3x3MatrixDot(axis2());
    if (bothAxes) {  // axes 0 and 1 of the AMotorAngle are enabled
      dJointSetAMotorAxis(mSpringAndDamperMotor, 1, 1, a2.x(), a2.y(), a2.z());
      dJointSetAMotorAngle(mSpringAndDamperMotor, 1, 0.0);
      dJointSetAMotorParam(mSpringAndDamperMotor, dParamLoStop2, clamped2);
      dJointSetAMotorParam(mSpringAndDamperMotor, dParamHiStop2, clamped2);
      dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopCFM2, cfm2);
      dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopERP2, erp2);
    } else {  // only axis 0 of the AMotorAngle is enabled
      dJointSetAMotorAxis(mSpringAndDamperMotor, 0, 1, a2.x(), a2.y(), a2.z());
      dJointSetAMotorAngle(mSpringAndDamperMotor, 0, 0.0);
      dJointSetAMotorParam(mSpringAndDamperMotor, dParamLoStop, clamped2);
      dJointSetAMotorParam(mSpringAndDamperMotor, dParamHiStop, clamped2);
      dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopCFM, cfm2);
      dJointSetAMotorParam(mSpringAndDamperMotor, dParamStopERP, erp2);
    }
  }
}

void WbHinge2Joint::prePhysicsStep(double ms) {
  assert(solidEndPoint());
  WbRotationalMotor *const rm = rotationalMotor();
  WbRotationalMotor *const rm2 = rotationalMotor2();
  WbJointParameters *const p = parameters();
  WbJointParameters *const p2 = parameters2();

  if (isEnabled()) {
    const double s = upperTransform()->absoluteScale().x();
    double s5 = s * s;
    s5 *= s5 * s;

    if (rm && rm->userControl()) {
      // user-defined torque
      dJointAddHinge2Torques(mJoint, -rm->rawInput(), 0.0);
      if (rm->hasMuscles())
        // force is directly applied to the bodies and not included in joint motor feedback
        emit updateMuscleStretch(rm->rawInput() / rm->maxForceOrTorque(), false, 1);
    } else {
      // ODE motor torque (user velocity/position control)
      const double currentVelocity = rm ? rm->computeCurrentDynamicVelocity(ms, mPosition) : 0.0;
      const double fMax = qMax(p ? p->staticFriction() : 0.0, rm ? rm->torque() : 0.0);
      dJointSetHinge2Param(mJoint, dParamFMax, s5 * fMax);
      dJointSetHinge2Param(mJoint, dParamVel, currentVelocity);
    }

    if (rm2 && rm2->userControl()) {
      // user-defined torque
      dJointAddHinge2Torques(mJoint, 0.0, -rm2->rawInput());
      if (rm2->hasMuscles())
        // force is directly applied to the bodies and not included in joint motor feedback
        emit updateMuscleStretch(rm2->rawInput() / rm2->maxForceOrTorque(), false, 2);
    } else {
      // ODE motor torque (user velocity/position control)
      const double currentVelocity2 = rm2 ? rm2->computeCurrentDynamicVelocity(ms, mPosition2) : 0.0;
      const double fMax2 = qMax(p2 ? p2->staticFriction() : 0.0, rm2 ? rm2->torque() : 0.0);
      dJointSetHinge2Param(mJoint, dParamFMax2, s5 * fMax2);
      dJointSetHinge2Param(mJoint, dParamVel2, currentVelocity2);
    }
    // eventually add spring and damping forces
    if (mSpringAndDamperMotor) {
      if (mSpringAndDampingConstantsAxis1On && mSpringAndDampingConstantsAxis2On) {
        // axes 0 and 1 of the AMotorAngle are enabled
        dJointSetAMotorAngle(mSpringAndDamperMotor, 0, dJointGetHinge2Angle1(mJoint));
        dJointSetAMotorAngle(mSpringAndDamperMotor, 1, -dJointGetHinge2Angle2(mJoint));
      } else if (mSpringAndDampingConstantsAxis1On) {
        // only axis 0 of the AMotorAngle is enabled
        dJointSetAMotorAngle(mSpringAndDamperMotor, 0, dJointGetHinge2Angle1(mJoint));
      } else
        dJointSetAMotorAngle(mSpringAndDamperMotor, 0, -dJointGetHinge2Angle2(mJoint));
    }
  } else {
    const bool run1 = rm && rm->runKinematicControl(ms, mPosition);
    if (run1) {
      if (p)
        p->setPosition(mPosition);
      if (rm->hasMuscles()) {
        double velocityPercentage = rm->currentVelocity() / rm->maxVelocity();
        if (rm->kinematicVelocitySign() == -1)
          velocityPercentage = -velocityPercentage;
        emit updateMuscleStretch(velocityPercentage, true, 1);
      }
    }

    const bool run2 = rm2 && rm2->runKinematicControl(ms, mPosition2);
    if (run2) {
      if (p2)
        p2->setPosition(mPosition2);
      if (rm2->hasMuscles()) {
        double velocityPercentage = rm2->currentVelocity() / rm2->maxVelocity();
        if (rm2->kinematicVelocitySign() == -1)
          velocityPercentage = -velocityPercentage;
        emit updateMuscleStretch(velocityPercentage, true, 2);
      }
    }

    if (run1 || run2)
      updatePositions(mPosition, mPosition2);
  }
  mTimeStep = ms;
}

void WbHinge2Joint::postPhysicsStep() {
  assert(mJoint);
  WbRotationalMotor *const rm = rotationalMotor();
  WbRotationalMotor *const rm2 = rotationalMotor2();
  if (rm && rm->isPIDPositionControl()) {
    // if controlling in position we update position using directly the angle feedback
    mPosition = WbMathsUtilities::normalizeAngle(-dJointGetHinge2Angle1(mJoint) + mOdePositionOffset, mPosition);
  } else {
    // if not controlling in position we use the angle rate feedback to update position (because at high speed angle feedback is
    // under-estimated)
    mPosition -= dJointGetHinge2Angle1Rate(mJoint) * mTimeStep / 1000.0;
  }
  WbJointParameters *const p = parameters();
  if (p)
    p->setPositionFromOde(mPosition);
  if (isEnabled() && rm && rm->hasMuscles() && !rm->userControl())
    // dynamic position or velocity control
    emit updateMuscleStretch(rm->computeFeedback() / rm->maxForceOrTorque(), false, 1);

  if (rm2 && rm2->isPIDPositionControl())
    mPosition2 = WbMathsUtilities::normalizeAngle(dJointGetHinge2Angle2(mJoint) + mOdePositionOffset2, mPosition2);
  else
    mPosition2 -= dJointGetHinge2Angle2Rate(mJoint) * mTimeStep / 1000.0;
  WbJointParameters *const p2 = parameters2();
  if (p2)
    p2->setPositionFromOde(mPosition2);
  if (isEnabled() && rm2 && rm2->hasMuscles() && !rm2->userControl())
    // dynamic position or velocity control
    emit updateMuscleStretch(rm2->computeFeedback() / rm2->maxForceOrTorque(), false, 2);
}

void WbHinge2Joint::reset(const QString &id) {
  WbJoint::reset(id);

  for (int i = 0; i < mDevice2->size(); ++i)
    mDevice2->item(i)->reset(id);

  WbNode *const p = mParameters2->value();
  if (p)
    p->reset(id);

  setPosition(mSavedPositions2[id], 2);
}

void WbHinge2Joint::resetPhysics() {
  WbJoint::resetPhysics();

  WbMotor *const m = motor2();
  if (m)
    m->resetPhysics();
}

void WbHinge2Joint::save(const QString &id) {
  WbJoint::save(id);

  for (int i = 0; i < mDevice2->size(); ++i)
    mDevice2->item(i)->save(id);

  WbNode *const p = mParameters2->value();
  if (p)
    p->save(id);

  mSavedPositions2[id] = mPosition2;
}

void WbHinge2Joint::updateEndPointZeroTranslationAndRotation() {
  if (solidEndPoint() == NULL)
    return;

  WbRotation ir;
  WbVector3 it;
  retrieveEndPointSolidTranslationAndRotation(it, ir);

  WbQuaternion qp;
  if (WbMathsUtilities::isZeroAngle(mPosition) && WbMathsUtilities::isZeroAngle(mPosition2))
    mEndPointZeroRotation = ir;  // Keeps track of the original axis if the angle is zero as it defines the second DoF axis
  else {
    const WbQuaternion q(axis(), -mPosition);
    const WbQuaternion q2(axis2(), -mPosition2);
    qp = q2 * q;
    const WbQuaternion &iq = ir.toQuaternion();
    WbQuaternion qr = qp * iq;
    qr.normalize();
    mEndPointZeroRotation = WbRotation(qr);
  }
  const WbVector3 &a = anchor();
  const WbVector3 t(it - a);
  mEndPointZeroTranslation = qp * t + a;
}

void WbHinge2Joint::computeEndPointSolidPositionFromParameters(WbVector3 &translation, WbRotation &rotation) const {
  WbQuaternion qp;
  const WbQuaternion q(axis(), mPosition);
  const WbQuaternion q2(axis2(), mPosition2);
  WbQuaternion qi = mEndPointZeroRotation.toQuaternion();
  qp = q * q2;
  const WbVector3 &a = anchor();
  const WbVector3 t(mEndPointZeroTranslation - a);
  translation = qp * t + a;
  qp = qp * qi;
  qp.normalize();
  rotation.fromQuaternion(qp);
}

void WbHinge2Joint::updatePosition() {
  const WbJointParameters *const p = parameters();
  const WbJointParameters *const p2 = parameters2();

  if (solidReference() == NULL && solidEndPoint())
    updatePositions(p ? p->position() : mPosition, p2 ? p2->position() : mPosition2);
  emit updateMuscleStretch(0.0, true, 1);
  emit updateMuscleStretch(0.0, true, 2);
}

void WbHinge2Joint::updatePositions(double position, double position2) {
  WbSolid *const s = solidEndPoint();
  assert(s);
  // called after an artificial move (user or Supervisor move) or in kinematic mode
  mPosition = position;
  mPosition2 = position2;
  WbMotor *m1 = motor();
  WbMotor *m2 = motor2();
  if (m1 && !m1->isConfigureDone())
    m1->setTargetPosition(position);
  if (m2 && !m2->isConfigureDone())
    m2->setTargetPosition(position2);
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

void WbHinge2Joint::updatePosition(double position) {
  updatePositions(mPosition, mPosition2);
}

void WbHinge2Joint::updateMinAndMaxStop(double min, double max) {
  const WbJointParameters *const p = dynamic_cast<WbJointParameters *>(sender());

  const WbRotationalMotor *rm = NULL;
  if (p == parameters2())
    rm = rotationalMotor2();
  else if (p == parameters())
    rm = rotationalMotor();

  if (rm) {
    const double minPos = rm->minPosition();
    const double maxPos = rm->maxPosition();
    if (min != max && minPos != maxPos) {
      if (minPos < min)
        p->parsingWarn(tr("HingeJoint 'minStop' must be less or equal to RotationalMotor 'minPosition'."));

      if (maxPos > max)
        p->parsingWarn(tr("HingeJoint 'maxStop' must be greater or equal to RotationalMotor 'maxPosition'."));
    }
  }

  if (mJoint)
    applyToOdeMinAndMaxStop();
}

void WbHinge2Joint::updateParameters() {
  WbHingeJoint::updateParameters();
  updateParameters2();
}

// Update methods

void WbHinge2Joint::addDevice2(int index) {
  const WbSolid *const s = upperSolid();
  if (s) {
    WbRobot *const r = s->robot();
    assert(r);
    WbBaseNode *decendant = dynamic_cast<WbBaseNode *>(mDevice2->item(index));
    r->descendantNodeInserted(decendant);
  }
  WbBrake *brake = dynamic_cast<WbBrake *>(mDevice2->item(index));
  if (brake)
    connect(brake, &WbBrake::brakingChanged, this, &WbHinge2Joint::updateSpringAndDampingConstants, Qt::UniqueConnection);
}

void WbHinge2Joint::updateParameters2() {
  const WbJointParameters *const p2 = parameters2();
  if (p2) {
    mOdePositionOffset2 = p2->position();
    mPosition2 = mOdePositionOffset2;
    connect(p2, &WbJointParameters::minAndMaxStopChanged, this, &WbHinge2Joint::updateMinAndMaxStop, Qt::UniqueConnection);
    connect(p2, &WbJointParameters::springAndDampingConstantsChanged, this, &WbHinge2Joint::updateSpringAndDampingConstants,
            Qt::UniqueConnection);
    connect(p2, &WbJointParameters::axisChanged, this, &WbHinge2Joint::updateAxis, Qt::UniqueConnection);
    connect(p2, SIGNAL(positionChanged()), this, SLOT(updatePosition()), Qt::UniqueConnection);
  }
}

WbJointParameters *WbHinge2Joint::parameters2() const {
  return dynamic_cast<WbJointParameters *>(mParameters2->value());
}

WbMotor *WbHinge2Joint::motor2() const {
  WbMotor *motor = NULL;
  for (int i = 0; i < mDevice2->size(); ++i) {
    motor = dynamic_cast<WbMotor *>(mDevice2->item(i));
    if (motor)
      return motor;
  }

  return NULL;
}

WbPositionSensor *WbHinge2Joint::positionSensor2() const {
  WbPositionSensor *sensor = NULL;
  for (int i = 0; i < mDevice2->size(); ++i) {
    sensor = dynamic_cast<WbPositionSensor *>(mDevice2->item(i));
    if (sensor)
      return sensor;
  }

  return NULL;
}

WbBrake *WbHinge2Joint::brake2() const {
  WbBrake *brake = NULL;
  for (int i = 0; i < mDevice2->size(); ++i) {
    brake = dynamic_cast<WbBrake *>(mDevice2->item(i));
    if (brake)
      return brake;
  }

  return NULL;
}

WbRotationalMotor *WbHinge2Joint::rotationalMotor2() const {
  WbRotationalMotor *motor = NULL;
  for (int i = 0; i < mDevice2->size(); ++i) {
    motor = dynamic_cast<WbRotationalMotor *>(mDevice2->item(i));
    if (motor)
      return motor;
  }

  return NULL;
}

WbVector3 WbHinge2Joint::axis2() const {
  static const WbVector3 DEFAULT_AXIS_2(0.0, 0.0, 1.0);
  const WbJointParameters *const p2 = parameters2();
  return p2 ? p2->axis() : DEFAULT_AXIS_2;
}

WbJointDevice *WbHinge2Joint::device2(int index) const {
  if (index >= 0 && mDevice2->size() > index)
    return dynamic_cast<WbJointDevice *>(mDevice2->item(index));
  else
    return NULL;
}

int WbHinge2Joint::devices2Number() const {
  return mDevice2->size();
}

QVector<WbLogicalDevice *> WbHinge2Joint::devices() const {
  QVector<WbLogicalDevice *> devices;
  int i = 0;
  for (i = 0; i < devicesNumber(); ++i)
    devices.append(device(i));
  for (i = 0; i < devices2Number(); ++i)
    devices.append(device2(i));

  return devices;
}

//////////
// WREN //
//////////

void WbHinge2Joint::createWrenObjects() {
  WbJoint::createWrenObjects();

  // create Wren objects for Muscle devices
  for (int i = 0; i < devices2Number(); ++i) {
    if (device2(i))
      device2(i)->createWrenObjects();
  }
}

void WbHinge2Joint::updateJointAxisRepresentation() {
  if (!areWrenObjectsInitialized())
    return;

  wr_static_mesh_delete(mMesh);

  const float scaling = 0.5f * wr_config_get_line_scale();

  float vertices[12];
  const WbVector3 &anchorVector = anchor();
  const WbVector3 &axisVector = scaling * axis();

  WbVector3 vertex(anchorVector - axisVector);
  vertex.toFloatArray(vertices);

  vertex = anchorVector + axisVector;
  vertex.toFloatArray(vertices + 3);

  const WbVector3 &axisVector2 = scaling * axis2();
  vertex = anchorVector - axisVector2;
  vertex.toFloatArray(vertices + 6);

  vertex = anchorVector + axisVector2;
  vertex.toFloatArray(vertices + 9);

  mMesh = wr_static_mesh_line_set_new(4, vertices, NULL);
  wr_renderable_set_mesh(mRenderable, WR_MESH(mMesh));
}

void WbHinge2Joint::writeExport(WbVrmlWriter &writer) const {
  if (writer.isUrdf() && solidEndPoint()) {
    warn(tr("Exporting 'Hinge2Joint' nodes to URDF is currently not supported"));
    return;
  }
  WbBasicJoint::writeExport(writer);
}
