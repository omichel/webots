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

#include "WbSliderJoint.hpp"

#include "WbBrake.hpp"
#include "WbJointParameters.hpp"
#include "WbLinearMotor.hpp"
#include "WbOdeContext.hpp"
#include "WbOdeUtilities.hpp"
#include "WbSolid.hpp"
#include "WbSolidMerger.hpp"
#include "WbWorld.hpp"

#include <ode/ode.h>
#include <cassert>

// Constructors

WbSliderJoint::WbSliderJoint(WbTokenizer *tokenizer) : WbJoint("SliderJoint", tokenizer) {
}

WbSliderJoint::WbSliderJoint(const WbSliderJoint &other) : WbJoint(other) {
}

WbSliderJoint::WbSliderJoint(const WbNode &other) : WbJoint(other) {
}

WbSliderJoint::~WbSliderJoint() {
}

WbLinearMotor *WbSliderJoint::linearMotor() const {
  WbLinearMotor *motor = NULL;
  for (int i = 0; i < mDevice->size(); ++i) {
    motor = dynamic_cast<WbLinearMotor *>(mDevice->item(i));
    if (motor)
      return motor;
  }

  return NULL;
}

void WbSliderJoint::updateEndPointZeroTranslationAndRotation() {
  if (solidEndPoint() == NULL)
    return;

  WbRotation ir;
  WbVector3 it;

  retrieveEndPointSolidTranslationAndRotation(it, ir);

  mEndPointZeroRotation = ir;
  mEndPointZeroTranslation = it - mPosition * axis();
}

void WbSliderJoint::computeEndPointSolidPositionFromParameters(WbVector3 &translation, WbRotation &rotation) const {
  translation = mEndPointZeroTranslation + mPosition * axis();
  rotation = mEndPointZeroRotation;
}

// Update methods: they check validity and correct if necessary

bool WbSliderJoint::setJoint() {
  if (!WbBasicJoint::setJoint())
    return false;

  if (mJoint == NULL)
    mJoint = dJointCreateSlider(WbOdeContext::instance()->world(), 0);

  const WbSolid *const s = solidEndPoint();
  setOdeJoint(s ? s->body() : NULL, upperSolid()->bodyMerger());

  return true;
}

void WbSliderJoint::applyToOdeMinAndMaxStop() {
  assert(mJoint);
  // place hard stops if defined
  const WbJointParameters *const p = parameters();
  const double m = p ? p->minStop() : 0.0;
  const double M = p ? p->maxStop() : 0.0;
  if (m != M) {
    const double s = upperTransform()->absoluteScale().x();
    dJointSetSliderParam(mJoint, dParamLoStop, s * (m - mOdePositionOffset));
    dJointSetSliderParam(mJoint, dParamHiStop, s * (M - mOdePositionOffset));
  }
}

void WbSliderJoint::applyToOdeAxis() {
  updateOdePositionOffset();

  assert(mJoint);
  const WbMatrix4 &m4 = upperTransform()->matrix();
  const WbVector3 &a = -m4.sub3x3MatrixDot(axis());  // ODE convention reverses the axis
  dJointSetSliderAxis(mJoint, a.x(), a.y(), a.z());
  if (mSpringAndDamperMotor)
    dJointSetLMotorAxis(mSpringAndDamperMotor, 0, 0, a.x(), a.y(), a.z());
}

void WbSliderJoint::updatePosition() {
  // Update triggered by an artificial move, i.e. a move caused by the user or a Supervisor
  const WbJointParameters *const p = parameters();

  if (solidReference() == NULL && solidEndPoint())
    updatePosition(p ? p->position() : mPosition);

  emit updateMuscleStretch(0.0, true, 1);
}

void WbSliderJoint::updatePosition(double position) {
  WbSolid *const s = solidEndPoint();
  assert(s);
  // called after a special artificial move, i.e. a statically based robot was moved
  mPosition = position;
  WbMotor *m = motor();
  if (m && !m->isConfigureDone())
    m->setTargetPosition(position);
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

void WbSliderJoint::updateMinAndMaxStop(double min, double max) {
  const WbJointParameters *const p = dynamic_cast<WbJointParameters *>(sender());
  WbLinearMotor *const lm = linearMotor();
  if (lm) {
    const double minPos = lm->minPosition();
    const double maxPos = lm->maxPosition();
    if (min != max && minPos != maxPos) {
      if (minPos < min)
        p->parsingWarn(tr("SliderJoint 'minStop' must be less or equal to LinearMotor 'minPosition'."));

      if (maxPos > max)
        p->parsingWarn(tr("SliderJoint 'maxStop' must be greater or equal to LinearMotor 'maxPosition'."));
    }
  }

  if (mJoint)
    applyToOdeMinAndMaxStop();
}

void WbSliderJoint::applyToOdeSpringAndDampingConstants(dBodyID body, dBodyID parentBody) {
  const WbJointParameters *const p = parameters();
  const double brakingDampingConstant = brake() ? brake()->getBrakingDampingConstant() : 0.0;

  if ((p == NULL && brakingDampingConstant == 0.0) || body == NULL) {
    if (mSpringAndDamperMotor) {
      dJointDestroy(mSpringAndDamperMotor);
      mSpringAndDamperMotor = NULL;
    }
    return;
  }

  assert(body && (p || brake()));

  double d = brakingDampingConstant;
  double s = 0.0;
  if (p) {
    d += p->dampingConstant();
    s += p->springConstant();
  }

  if (s == 0.0 && d == 0.0) {
    if (mSpringAndDamperMotor) {
      dJointDestroy(mSpringAndDamperMotor);
      mSpringAndDamperMotor = NULL;
    }
    return;
  }

  // Handles scale
  const double scale = upperTransform()->absoluteScale().x();
  const double s2 = scale * scale;
  s *= s2;
  d *= s2;

  double cfm, erp;
  const WbWorldInfo *const wi = WbWorld::instance()->worldInfo();
  WbOdeUtilities::convertSpringAndDampingConstants(s, d, wi->basicTimeStep() * 0.001, cfm, erp);

  const dWorldID world = parentBody ? dBodyGetWorld(parentBody) : dBodyGetWorld(body);
  if (mSpringAndDamperMotor == NULL)
    mSpringAndDamperMotor = dJointCreateLMotor(world, 0);

  dJointAttach(mSpringAndDamperMotor, parentBody, body);

  dJointSetLMotorNumAxes(mSpringAndDamperMotor, 1);

  // Axis setting
  const WbMatrix4 &m4 = upperTransform()->matrix();
  const WbVector3 &a = m4.sub3x3MatrixDot(axis());
  dJointSetLMotorAxis(mSpringAndDamperMotor, 0, 0, a.x(), a.y(), a.z());

  // Stops
  const double stop = -scale * mOdePositionOffset;
  dJointSetLMotorParam(mSpringAndDamperMotor, dParamLoStop, stop);
  dJointSetLMotorParam(mSpringAndDamperMotor, dParamHiStop, stop);

  // Spring and damper through LCP
  dJointSetLMotorParam(mSpringAndDamperMotor, dParamStopCFM, cfm);
  dJointSetLMotorParam(mSpringAndDamperMotor, dParamStopERP, erp);

  // Initial position
  dJointSetLMotorPosition(mSpringAndDamperMotor, 0, 0.0);
}

void WbSliderJoint::updateParameters() {
  WbJoint::updateParameters();
}

void WbSliderJoint::prePhysicsStep(double ms) {
  WbSolid *const solid = solidEndPoint();
  assert(solid);
  WbJointParameters *const p = parameters();
  WbLinearMotor *const lm = linearMotor();

  if (isEnabled()) {
    if (lm && lm->userControl()) {
      // user-defined force
      const double force = lm->rawInput();
      dJointAddSliderForce(mJoint, force);
      if (lm->hasMuscles())
        // force is directly applied to the bodies and not included in joint motor feedback
        emit updateMuscleStretch(force / lm->maxForceOrTorque(), false, 1);
    } else {
      // ODE motor force (user velocity/position control)
      const double currentVelocity = lm ? lm->computeCurrentDynamicVelocity(ms, mPosition) : 0.0;
      const double fMax = qMax(p ? p->staticFriction() : 0.0, lm ? lm->force() : 0.0);
      const double s = solid->absoluteScale().x();
      double s4 = s * s;
      s4 *= s4;
      dJointSetSliderParam(mJoint, dParamFMax, s4 * fMax);
      dJointSetSliderParam(mJoint, dParamVel, -s * currentVelocity);  // ODE convention reverses the axis
    }
    if (mSpringAndDamperMotor) {
      double position = dJointGetSliderPosition(mJoint);
      if (mIsReverseJoint)
        position = -position;
      dJointSetLMotorPosition(mSpringAndDamperMotor, 0, position);
    }
  } else if (lm) {
    if (lm->runKinematicControl(ms, mPosition)) {
      if (p)
        p->setPosition(mPosition);
      else
        updatePosition(mPosition);
    }
    if (lm->hasMuscles()) {
      double velocityPercentage = lm->currentVelocity() / lm->maxVelocity();
      if (lm->kinematicVelocitySign() == -1)
        velocityPercentage = -velocityPercentage;
      emit updateMuscleStretch(velocityPercentage, true, 1);
    }
  }
}

void WbSliderJoint::postPhysicsStep() {
  const WbSolid *const solid = solidEndPoint();
  assert(mJoint && solid);
  mPosition = dJointGetSliderPosition(mJoint) / solid->absoluteScale().x() + mOdePositionOffset;
  WbJointParameters *const p = parameters();
  if (p)
    p->setPositionFromOde(mPosition);

  if (isEnabled()) {
    WbLinearMotor *const lm = linearMotor();
    if (lm && lm->hasMuscles() && !lm->userControl())
      // dynamic position or velocity control
      emit updateMuscleStretch(-lm->computeFeedback() / lm->maxForceOrTorque(), false, 1);
  }
}

WbVector3 WbSliderJoint::axis() const {
  static const WbVector3 DEFAULT_AXIS(0.0, 0.0, 1.0);
  const WbJointParameters *const p = parameters();
  return p ? p->axis().normalized() : DEFAULT_AXIS;
}

WbVector3 WbSliderJoint::anchor() const {
  const WbSolid *const s = solidEndPoint();
  const WbSolidReference *const sr = solidReference();
  if (s && !sr)
    return mEndPointZeroTranslation;
  else if (s) {
    const WbVector3 &a = s->position();
    const WbTransform *const ut = upperTransform();
    WbVector3 an = ut->matrix().pseudoInversed(a);
    double scale = ut->absoluteScale().x();
    an /= scale * scale;
    return an;
  }

  return WbBasicJoint::anchor();
}

void WbSliderJoint::writeExport(WbVrmlWriter &writer) const {
  if (writer.isUrdf() && solidEndPoint()) {
    const WbNode *const parentRoot = findUrdfLinkRoot();
    const WbVector3 currentOffset = solidEndPoint()->translation() - anchor();
    const WbVector3 translation = solidEndPoint()->translationFrom(parentRoot) - currentOffset + writer.jointOffset();
    writer.setJointOffset(solidEndPoint()->rotationMatrixFrom(parentRoot).transposed() * currentOffset);
    const WbVector3 rotationEuler = solidEndPoint()->rotationMatrixFrom(parentRoot).toEulerAnglesZYX();
    const WbVector3 rotationAxis = axis() * solidEndPoint()->rotationMatrixFrom(parentRoot);

    writer.increaseIndent();
    writer.indent();
    writer << QString("<joint name=\"%1\" type=\"prismatic\">\n").arg(urdfName());

    writer.increaseIndent();
    writer.indent();
    writer << QString("<parent link=\"%1\"/>\n").arg(parentNode()->urdfName());
    writer.indent();
    writer << QString("<child link=\"%1\"/>\n").arg(solidEndPoint()->urdfName());
    writer.indent();
    writer << QString("<axis xyz=\"%1\"/>\n").arg(rotationAxis.toString(WbPrecision::FLOAT_ROUND_6));
    writer.indent();
    writer << QString("<origin xyz=\"%1\" rpy=\"%2\"/>\n")
                .arg(translation.toString(WbPrecision::FLOAT_ROUND_6))
                .arg(rotationEuler.toString(WbPrecision::FLOAT_ROUND_6));
    writer.indent();
    const WbMotor *m = motor();
    if (m) {
      if (m->minPosition() != 0.0 || m->maxPosition() != 0.0)
        writer << QString("<limit effort=\"%1\" lower=\"%2\" upper=\"%3\" velocity=\"%4\"/>\n")
                    .arg(m->maxForceOrTorque())
                    .arg(m->minPosition())
                    .arg(m->maxPosition())
                    .arg(m->maxVelocity());
      else
        writer << QString("<limit effort=\"%1\" velocity=\"%2\"/>\n").arg(m->maxForceOrTorque()).arg(m->maxVelocity());
    }
    writer.decreaseIndent();

    writer.indent();
    writer << QString("</joint>\n");
    writer.decreaseIndent();

    WbNode::exportNodeSubNodes(writer);
    return;
  }

  WbNode::writeExport(writer);
}
