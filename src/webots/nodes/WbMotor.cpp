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

//
//  WbMotor.cpp
//

#include "WbMotor.hpp"

#include "WbDownloader.hpp"
#include "WbField.hpp"
#include "WbFieldChecker.hpp"
#include "WbJoint.hpp"
#include "WbJointParameters.hpp"
#include "WbMuscle.hpp"
#include "WbPropeller.hpp"
#include "WbRobot.hpp"
#include "WbSensor.hpp"
#include "WbSolid.hpp"
#include "WbSoundEngine.hpp"
#include "WbTrack.hpp"
#include "WbUrl.hpp"

#include <ode/ode.h>

#include <QtCore/QDataStream>
#include <QtCore/QUrl>

#include <cassert>
#include <cmath>

#include "../../controller/c/messages.h"  // contains the definitions for the macros C_SET_SAMPLING_PERIOD, C_MOTOR_SET_POSITION, C_MOTOR_SET_VELOCITY ...

QList<const WbMotor *> WbMotor::cMotors;

void WbMotor::init() {
  mHasAllocatedJointFeedback = false;
  mForceOrTorqueSensor = NULL;
  mNeedToConfigure = false;
  mSoundClip = NULL;
  mForceOrTorqueLastValue = 0.0;
  mKinematicVelocitySign = 0;
  mRequestedDeviceTag = NULL;
  mCoupledMotors.clear();

  mMotorForceOrTorque = 0.0;
  mTargetVelocity = 0.0;
  mTargetPosition = 0.0;
  mCurrentVelocity = 0.0;
  mRawInput = 0.0;
  mUserControl = false;
  mErrorIntegral = 0.0;
  mPreviousError = 0.0;

  mMaxForceOrTorque = NULL;
  mAcceleration = findSFDouble("acceleration");
  mConsumptionFactor = findSFDouble("consumptionFactor");
  mControlPID = findSFVector3("controlPID");
  mMinPosition = findSFDouble("minPosition");
  mMaxPosition = findSFDouble("maxPosition");
  mMaxVelocity = findSFDouble("maxVelocity");
  mMultiplier = findSFDouble("multiplier");
  mSound = findSFString("sound");
  mMuscles = findMFNode("muscles");
  mDownloader = NULL;
}

WbMotor::WbMotor(const QString &modelName, WbTokenizer *tokenizer) : WbJointDevice(modelName, tokenizer) {
  init();
}

WbMotor::WbMotor(const WbMotor &other) : WbJointDevice(other) {
  init();
}

WbMotor::WbMotor(const WbNode &other) : WbJointDevice(other) {
  init();
}

WbMotor::~WbMotor() {
  delete mForceOrTorqueSensor;
  // notify siblings about removal
  for (int i = 0; i < mCoupledMotors.size(); ++i)
    mCoupledMotors[i]->removeFromCoupledMotors(this);
  cMotors.removeAll(this);
}

void WbMotor::downloadAssets() {
  const QString &sound = mSound->value();
  if (WbUrl::isWeb(sound)) {
    mDownloader = new WbDownloader(this);
    if (isPostFinalizedCalled())
      connect(mDownloader, &WbDownloader::complete, this, &WbMotor::updateSound);
    mDownloader->download(QUrl(sound));
  }
}

void WbMotor::preFinalize() {
  WbJointDevice::preFinalize();

  cMotors << this;

  // note: only parameter validity check has to occur in preFinalize, validity across couplings must be done in postFinalize
  updateMaxVelocity();
  updateMaxAcceleration();
  updateControlPID();
  updateMinAndMaxPosition();
  updateMultiplier();
  updateSound();

  mForceOrTorqueSensor = new WbSensor();

  const WbJoint *const j = joint();
  mTargetPosition = j ? position() : 0.0;
  mMotorForceOrTorque = mMaxForceOrTorque->value();
  updateMaxForceOrTorque();
}

void WbMotor::postFinalize() {
  WbJointDevice::postFinalize();
  assert(robot());
  if (!mMuscles->isEmpty() || robot()->maxEnergy() > 0)
    setupJointFeedback();

  inferMotorCouplings();  // it also checks consistency across couplings

  // must be done in the postFinalize step as the coupled motor consistency check could change the values
  mTargetVelocity = mMaxVelocity->value();
  mNeedToConfigure = true;  // notify libController about the changes done by the check

  WbMFIterator<WbMFNode, WbNode *> it(mMuscles);
  while (it.hasNext())
    dynamic_cast<WbMuscle *>(it.next())->postFinalize();

  connect(mMultiplier, &WbSFDouble::changed, this, &WbMotor::updateMultiplier);
  connect(mMaxVelocity, &WbSFDouble::changed, this, &WbMotor::updateMaxVelocity);
  connect(mAcceleration, &WbSFDouble::changed, this, &WbMotor::updateMaxAcceleration);
  connect(mControlPID, &WbSFVector3::changed, this, &WbMotor::updateControlPID);
  connect(mMinPosition, &WbSFDouble::changed, this, &WbMotor::updateMinAndMaxPosition);
  connect(mMaxPosition, &WbSFDouble::changed, this, &WbMotor::updateMinAndMaxPosition);
  connect(mMinPosition, &WbSFDouble::changed, this, &WbMotor::minPositionChanged);
  connect(mMaxPosition, &WbSFDouble::changed, this, &WbMotor::maxPositionChanged);
  connect(mSound, &WbSFString::changed, this, &WbMotor::updateSound);
  connect(mMuscles, &WbSFNode::changed, this, &WbMotor::updateMuscles);
  connect(mMaxForceOrTorque, &WbSFDouble::changed, this, &WbMotor::updateMaxForceOrTorque);
  connect(mDeviceName, &WbSFString::changed, this, &WbMotor::inferMotorCouplings);
}

void WbMotor::createWrenObjects() {
  WbJointDevice::createWrenObjects();
  WbMFIterator<WbMFNode, WbNode *> it(mMuscles);
  while (it.hasNext())
    dynamic_cast<WbMuscle *>(it.next())->createWrenObjects();
}

void WbMotor::setupJointFeedback() {
  const WbJoint *const j = joint();
  if (!j)
    return;
  dJointID jID = j->jointID();
  if (!jID)
    return;
  // check whether a feedback structure was already registered by a physics plugin
  // or a wb_motor_enable_force_feedback/wb_motor_enable_torque_feedback call,
  // otherwise create own structure
  dJointFeedback *fb = dJointGetFeedback(jID);
  if (fb)  // already set
    return;
  fb = new dJointFeedback;
  dJointSetFeedback(jID, fb);
}

double WbMotor::energyConsumption() const {
  if (dynamic_cast<WbTrack *>(parentNode()))
    return 0.0;

  return fabs(computeFeedback()) * mConsumptionFactor->value();
}

void WbMotor::inferMotorCouplings() {
  // remove the reference to this motor from any of the siblings (necessary in the event of name changes from the interface)
  for (int i = 0; i < mCoupledMotors.size(); ++i)
    mCoupledMotors[i]->removeFromCoupledMotors(this);
  // clear references to my siblings
  mCoupledMotors.clear();
  // rebuild the sibling list
  const QStringList nameChunks = deviceName().split("::");
  if (nameChunks.size() != 2)  // name structure: "motor name::specifier name". Coupling is determined by the motor name part
    return;

  for (int i = 0; i < cMotors.size(); ++i) {
    QStringList otherNameChunks = cMotors[i]->deviceName().split("::");
    if (otherNameChunks.size() != 2)
      continue;

    if (robot() == cMotors[i]->robot() && cMotors[i]->tag() != tag() && nameChunks[0] == otherNameChunks[0] &&
        !mCoupledMotors.contains(const_cast<WbMotor *>(cMotors[i]))) {
      mCoupledMotors.append(const_cast<WbMotor *>(cMotors[i]));
      mCoupledMotors.last()->addToCoupledMotors(this);  // add myself to the newly added sibling
    }
  }

  if (deviceName().contains("::") && mCoupledMotors.size() == 0)
    parsingWarn(tr("Motor '%1' uses the coupled motor name structure but does not have any siblings.").arg(deviceName()));

  checkMultiplierAcrossCoupledMotors();  // it calls all the other checks implicitly
}

/////////////
// Updates //
/////////////

void WbMotor::updateMinAndMaxPosition() {
  enforceMotorLimitsInsideJointLimits();

  if (isPreFinalizedCalled())
    checkMinAndMaxPositionAcrossCoupledMotors();

  mNeedToConfigure = true;
}

void WbMotor::updateMaxForceOrTorque() {
  WbFieldChecker::resetDoubleIfNegative(this, mMaxForceOrTorque, 10.0);
  mNeedToConfigure = true;
}

void WbMotor::updateMaxVelocity() {
  WbFieldChecker::resetDoubleIfNegative(this, mMaxVelocity, -mMaxVelocity->value());

  if (mCoupledMotors.size() > 0 && isPreFinalizedCalled())
    checkMaxVelocityAcrossCoupledMotors();

  mNeedToConfigure = true;
}

void WbMotor::updateMultiplier() {
  if (multiplier() == 0.0) {
    parsingWarn(tr("The value of 'multiplier' cannot be 0. Value reverted to 1."));
    mMultiplier->setValue(1.0);
  }

  if (isPreFinalizedCalled()) {
    checkMultiplierAcrossCoupledMotors();
  }

  mNeedToConfigure = true;
}

void WbMotor::updateControlPID() {
  const WbVector3 &pid = mControlPID->value();
  const double p = pid.x();
  if (p <= 0.0)
    parsingWarn(tr("'controlP' (currently %1) must be positive.").arg(p));

  const double i = pid.y();
  if (i < 0.0)
    parsingWarn(tr("'controlI' (currently %1) must be non-negative.").arg(i));

  const double d = pid.z();
  if (d < 0.0)
    parsingWarn(tr("'controlD' (currently %1) must be non-negative.").arg(d));

  mErrorIntegral = 0.0;
  mPreviousError = 0.0;

  mNeedToConfigure = true;
}

void WbMotor::updateSound() {
  const QString &sound = mSound->value();
  if (sound.isEmpty())
    mSoundClip = NULL;
  else if (isPostFinalizedCalled() && WbUrl::isWeb(sound) && mDownloader == NULL) {
    downloadAssets();
    return;
  } else if (!mDownloader)
    mSoundClip = WbSoundEngine::sound(WbUrl::computePath(this, "sound", sound));
  else {
    if (mDownloader->error().isEmpty())
      mSoundClip = WbSoundEngine::sound(sound, mDownloader->device());
    else {
      mSoundClip = NULL;
      warn(mDownloader->error());
    }
    delete mDownloader;
    mDownloader = NULL;
  }
  WbSoundEngine::clearAllMotorSoundSources();
}

void WbMotor::updateMuscles() {
  setupJointFeedback();
}

void WbMotor::updateMaxAcceleration() {
  WbFieldChecker::resetDoubleIfNegativeAndNotDisabled(this, mAcceleration, -1, -1);
  mNeedToConfigure = true;
}

///////////////////
// Plain setters //
///////////////////

void WbMotor::setMaxVelocity(double v) {
  mMaxVelocity->setValue(v);
  awake();
}

void WbMotor::setMaxAcceleration(double acc) {
  mAcceleration->setValue(acc);
  awake();
}

/////////////////
// API setters //
/////////////////

void WbMotor::setTargetPosition(double position) {
  const double maxp = mMaxPosition->value();
  const double minp = mMinPosition->value();
  const bool velocityControl = std::isinf(position);

  // negative multipliers could turn the +inf into a -inf
  mTargetPosition = velocityControl ? position : position * multiplier();

  if (maxp != minp && !velocityControl) {
    if (mTargetPosition > maxp) {
      mTargetPosition = maxp;
      warn(QString("too big requested position: %1 > %2").arg(position).arg(maxp));
    } else if (mTargetPosition < minp) {
      mTargetPosition = minp;
      warn(QString("too low requested position: %1 < %2").arg(position).arg(minp));
    }
  }

  mUserControl = false;
  mNeedToConfigure = true;  // each sibling has to notify libcontroller about velocityControl/positionControl
  awake();
}

void WbMotor::setVelocity(double velocity) {
  mTargetVelocity = velocity * multiplier();

  const double m = mMaxVelocity->value();
  const bool isNegative = mTargetVelocity < 0.0;
  if ((isNegative ? -mTargetVelocity : mTargetVelocity) > m) {
    warn(tr("The requested velocity %1 exceeds 'maxVelocity' = %2.").arg(mTargetVelocity).arg(m));
    mTargetVelocity = isNegative ? -m : m;
  }

  mNeedToConfigure = true;  // each sibling has to notify libcontroller about velocityControl/positionControl
  awake();
}

void WbMotor::setAcceleration(double acceleration) {
  // note: a check is performed on libController side for negative values
  mAcceleration->setValue(acceleration);  // mAcceleration specifies maximal acceleration, hence it isn't multiplied
  awake();
}

void WbMotor::setForceOrTorque(double forceOrTorque) {
  if (!mUserControl)  // we were previously using motor force
    turnOffMotor();
  mUserControl = true;

  mRawInput = forceOrTorque * multiplier();
  if (fabs(mRawInput) > mMotorForceOrTorque) {
    if (mCoupledMotors.size() == 0) {  // silence warning for coupled motors
      if (nodeType() == WB_NODE_ROTATIONAL_MOTOR)
        warn(tr("The requested motor torque %1 exceeds 'maxTorque' = %2").arg(mRawInput).arg(mMotorForceOrTorque));
      else
        warn(tr("The requested motor force %1 exceeds 'maxForce' = %2").arg(mRawInput).arg(mMotorForceOrTorque));
    }

    mRawInput = mRawInput >= 0.0 ? mMotorForceOrTorque : -mMotorForceOrTorque;
  }

  awake();
}

void WbMotor::setAvailableForceOrTorque(double availableForceOrTorque) {
  // note: a check is performed on libController side for negative values
  mMotorForceOrTorque = availableForceOrTorque;

  const double m = mMaxForceOrTorque->value();
  if (mMotorForceOrTorque > m) {
    if (mCoupledMotors.size() == 0) {  // silence warning for coupled motors
      if (nodeType() == WB_NODE_ROTATIONAL_MOTOR)
        warn(tr("The requested available motor torque %1 exceeds 'maxTorque' = %2").arg(mMotorForceOrTorque).arg(m));
      else
        warn(tr("The requested available motor force %1 exceeds 'maxForce' = %2").arg(mMotorForceOrTorque).arg(m));
    }
    mMotorForceOrTorque = m;
  }
  awake();
}

double WbMotor::computeCurrentDynamicVelocity(double ms, double position) {
  if (mMotorForceOrTorque == 0.0) {  // no control
    mCurrentVelocity = 0.0;
    return mCurrentVelocity;
  }

  // compute required velocity
  double velocity = -mTargetVelocity;
  if (!std::isinf(mTargetPosition)) {  // PID-position control
    const double error = mTargetPosition - position;
    const double ts = ms * 0.001;
    mErrorIntegral += error * ts;
    const double errorDerivative = (error - mPreviousError) / ts;
    const double outputValue =
      mControlPID->value().x() * error + mControlPID->value().y() * mErrorIntegral + mControlPID->value().z() * errorDerivative;
    mPreviousError = error;

    if (fabs(outputValue) < fabs(mTargetVelocity))
      velocity = -outputValue;
    else
      velocity = outputValue > 0.0 ? -fabs(mTargetVelocity) : fabs(mTargetVelocity);
  }

  // try to get closer to velocity
  double acc = mAcceleration->value();
  if (acc == -1.0)
    mCurrentVelocity = velocity;  // use maximum force/torque
  else {
    const double ts = ms * 0.001;
    double d = fabs(velocity - mCurrentVelocity) / ts;  // requested acceleration
    if (d < acc)
      acc = d;
    if (velocity > mCurrentVelocity)
      mCurrentVelocity += acc * ts;
    else
      mCurrentVelocity -= acc * ts;
  }

  return mCurrentVelocity;
}

// run control without physics simulation
bool WbMotor::runKinematicControl(double ms, double &position) {
  static const double TARGET_POSITION_THRESHOLD = 1e-6;
  bool doNothing = false;
  if (std::isinf(mTargetPosition)) {  // velocity control
    const double maxp = mMaxPosition->value();
    const double minp = mMinPosition->value();
    doNothing = maxp != minp && ((position >= maxp && mTargetVelocity >= 0.0) || (position <= minp && mTargetVelocity <= 0.0));
  } else
    doNothing = fabs(mTargetPosition - position) < TARGET_POSITION_THRESHOLD;

  if (doNothing) {
    mCurrentVelocity = 0.0;
    mKinematicVelocitySign = 0;
    return false;
  }

  const double ts = ms * 0.001;
  double acc = mAcceleration->value();
  if (acc == -1.0 || acc > mMotorForceOrTorque)
    acc = mMotorForceOrTorque;
  static const double TARGET_VELOCITY_THRESHOLD = 1e-6;
  if (fabs(mTargetVelocity - mCurrentVelocity) > TARGET_VELOCITY_THRESHOLD) {
    // didn't reach the target velocity yet
    if (mTargetVelocity > mCurrentVelocity) {
      mCurrentVelocity += acc * ts;
      if (mCurrentVelocity > mTargetVelocity)
        mCurrentVelocity = mTargetVelocity;
    } else {
      mCurrentVelocity -= acc * ts;
      if (mCurrentVelocity < mTargetVelocity)
        mCurrentVelocity = mTargetVelocity;
    }
  }

  if (mTargetPosition > position) {
    mKinematicVelocitySign = 1;
    position += mCurrentVelocity * ts;
    if (position > mTargetPosition)
      position = mTargetPosition;
  } else {
    mKinematicVelocitySign = -1;
    position -= mCurrentVelocity * ts;
    if (position < mTargetPosition)
      position = mTargetPosition;
  }

  return true;
}

void WbMotor::powerOn(bool e) {  // called when running out of energy with e=false
  WbDevice::powerOn(e);
  if (!e)
    mMotorForceOrTorque = 0.0;
  else
    mMotorForceOrTorque = mMaxForceOrTorque->value();
}

bool WbMotor::isConfigureDone() const {
  return robot()->isConfigureDone();
}

void WbMotor::checkMinAndMaxPositionAcrossCoupledMotors() {
  // if the position is unlimited for this motor, the coupled ones must also be
  for (int i = 0; i < mCoupledMotors.size(); ++i) {
    if (isPositionUnlimited() && !mCoupledMotors[i]->isPositionUnlimited()) {
      parsingWarn(
        tr("For coupled motors, if one has unlimited position, its siblings must have unlimited position as well. Adjusting "
           "'minPosition' and 'maxPosition' to 0 for motor '%1'.")
          .arg(deviceName()));
      mCoupledMotors[i]->setMinPosition(0);
      mCoupledMotors[i]->setMaxPosition(0);
    }
  }

  // if the position is limited for this motor, adjust the limits of the siblings accordingly
  if (!isPositionUnlimited()) {
    for (int i = 0; i < mCoupledMotors.size(); ++i) {
      double potentialMinimalPosition = minPosition() * mCoupledMotors[i]->multiplier() / multiplier();
      double potentialMaximalPosition = maxPosition() * mCoupledMotors[i]->multiplier() / multiplier();

      if (potentialMaximalPosition < potentialMinimalPosition) {  // swap values for consistency
        const double tmp = potentialMaximalPosition;
        potentialMaximalPosition = potentialMinimalPosition;
        potentialMinimalPosition = tmp;
      }

      if (mCoupledMotors[i]->minPosition() != potentialMinimalPosition) {
        parsingWarn(tr("When using coupled motors, 'minPosition' must be consistent across devices. Adjusting 'minPosition' "
                       "from %1 to %2 for motor '%3'.")
                      .arg(mCoupledMotors[i]->minPosition())
                      .arg(potentialMinimalPosition)
                      .arg(deviceName()));
        mCoupledMotors[i]->setMinPosition(potentialMinimalPosition);
      }

      if (mCoupledMotors[i]->maxPosition() != potentialMaximalPosition) {
        parsingWarn(tr("When using coupled motors, 'maxPosition' must be consistent across devices. Adjusting 'maxPosition' "
                       "from %1 to %2 for motor '%3'.")
                      .arg(mCoupledMotors[i]->maxPosition())
                      .arg(potentialMaximalPosition)
                      .arg(deviceName()));
        mCoupledMotors[i]->setMaxPosition(potentialMaximalPosition);
      }
    }
  }
}

void WbMotor::enforceMotorLimitsInsideJointLimits() {
  if (isPositionUnlimited())
    return;

  WbJoint *parentJoint = dynamic_cast<WbJoint *>(parentNode());
  double p = 0.0;
  if (parentJoint) {
    if (positionIndex() == 1 && parentJoint->parameters())
      p = parentJoint->parameters()->position();
    if (positionIndex() == 2 && parentJoint->parameters2())
      p = parentJoint->parameters2()->position();
    if (positionIndex() == 3 && parentJoint->parameters3())
      p = parentJoint->parameters3()->position();
  }

  // current joint position should lie between min and max position
  WbFieldChecker::resetDoubleIfLess(this, mMaxPosition, p, p);
  WbFieldChecker::resetDoubleIfGreater(this, mMinPosition, p, p);
}

void WbMotor::checkMaxVelocityAcrossCoupledMotors() {
  // assume this motor is pushed to its limit, ensure the sibling limits aren't broken
  for (int i = 0; i < mCoupledMotors.size(); ++i) {
    const double potentialMaximalVelocity = maxVelocity() * fabs(mCoupledMotors[i]->multiplier()) / fabs(multiplier());

    if (mCoupledMotors[i]->maxVelocity() != potentialMaximalVelocity) {
      parsingWarn(tr("When using coupled motors, velocity limits must be consistent across devices. Adjusted 'maxVelocity' "
                     "from %1 to %2 for motor '%3'.")
                    .arg(mCoupledMotors[i]->maxVelocity())
                    .arg(potentialMaximalVelocity)
                    .arg(deviceName()));
      mCoupledMotors[i]->setMaxVelocity(potentialMaximalVelocity);
    }
  }
}

void WbMotor::checkMultiplierAcrossCoupledMotors() {
  if (mCoupledMotors.size() == 0)
    return;

  checkMinAndMaxPositionAcrossCoupledMotors();
  checkMaxVelocityAcrossCoupledMotors();
}

/////////////
// Control //
/////////////

void WbMotor::addConfigureToStream(QDataStream &stream) {
  stream << (unsigned short)tag();
  stream << (unsigned char)C_CONFIGURE;
  stream << (int)type();
  stream << (double)mMinPosition->value();
  stream << (double)mMaxPosition->value();
  stream << (double)mMaxVelocity->value();
  stream << (double)mAcceleration->value();
  stream << (double)mMaxForceOrTorque->value();
  stream << (double)mControlPID->value().x();
  stream << (double)mControlPID->value().y();
  stream << (double)mControlPID->value().z();
  stream << (double)mTargetPosition;
  stream << (double)mTargetVelocity;
  stream << (double)mMultiplier->value();
  mNeedToConfigure = false;
}

void WbMotor::writeConfigure(QDataStream &stream) {
  if (mForceOrTorqueSensor)
    mForceOrTorqueSensor->connectToRobotSignal(robot());
  addConfigureToStream(stream);
}

void WbMotor::enableMotorFeedback(int rate) {
  const WbJoint *const j = joint();

  if (j == NULL) {
    warn(tr("Feedback is available for motorized joints only"));
    return;
  }

  const WbSolid *const s = j->solidEndPoint();

  if (s == NULL) {
    warn(nodeType() == WB_NODE_ROTATIONAL_MOTOR ?
           tr("wb_motor_enable_torque_feedback(): cannot be invoked for a Joint with no end point.") :
           tr("wb_motor_enable_force_feedback(): cannot be invoked for a Joint with no end point."));
    return;
  }

  if (s->physics() == NULL) {
    warn(nodeType() == WB_NODE_ROTATIONAL_MOTOR ?
           tr("wb_motor_enable_torque_feedback(): cannot be invoked for a Joint whose end point has no Physics node.") :
           tr("wb_motor_enable_force_feedback(): cannot be invoked for a Joint whose end point has no Physics node."));
    return;
  }

  const WbSolid *const us = j->upperSolid();
  if (us->physics() == NULL) {
    warn(nodeType() == WB_NODE_ROTATIONAL_MOTOR ?
           tr("wb_motor_enable_torque_feedback(): cannot be invoked because the parent Solid has no Physics node.") :
           tr("wb_motor_enable_force_feedback(): cannot be invoked because the parent Solid has no Physics node."));
    return;
  }

  dJointID jID = j->jointID();
  if (!mForceOrTorqueSensor->isEnabled() && rate) {  // switch on feedback
    // check if a feedback structure was already registered by the physics plugin
    // or some energy measurement requirement, otherwise create own structure
    dJointFeedback *fb = dJointGetFeedback(jID);
    if (!fb) {
      fb = new dJointFeedback;
      dJointSetFeedback(jID, fb);
      mHasAllocatedJointFeedback = true;
    }
  } else if (mForceOrTorqueSensor->isEnabled() && !rate) {  // switching off feedback
    if (mHasAllocatedJointFeedback) {
      const dJointFeedback *const fb = dJointGetFeedback(jID);
      delete fb;
      dJointSetFeedback(jID, NULL);
      mHasAllocatedJointFeedback = false;
    }
  }

  mForceOrTorqueSensor->setRefreshRate(rate);
}

void WbMotor::handleMessage(QDataStream &stream) {
  short command;
  stream >> command;

  switch (command) {
    case C_MOTOR_SET_POSITION: {
      double position;
      stream >> position;
      setTargetPosition(position);
      // relay target position to coupled motors, if any
      for (int i = 0; i < mCoupledMotors.size(); ++i)
        mCoupledMotors[i]->setTargetPosition(position);
      break;
    }
    case C_MOTOR_SET_VELOCITY: {
      double velocity;
      stream >> velocity;
      setVelocity(velocity);
      // relay target velocity to coupled motors, if any
      for (int i = 0; i < mCoupledMotors.size(); ++i)
        mCoupledMotors[i]->setVelocity(velocity);
      break;
    }
    case C_MOTOR_SET_ACCELERATION: {
      double acceleration;
      stream >> acceleration;
      setAcceleration(acceleration);
      break;
    }
    case C_MOTOR_SET_FORCE: {
      double forceOrTorque;
      stream >> forceOrTorque;
      setForceOrTorque(forceOrTorque);
      for (int i = 0; i < mCoupledMotors.size(); ++i)
        mCoupledMotors[i]->setForceOrTorque(forceOrTorque);
      break;
    }
    case C_MOTOR_SET_AVAILABLE_FORCE: {
      double availableForceOrTorque;
      stream >> availableForceOrTorque;
      setAvailableForceOrTorque(availableForceOrTorque);
      break;
    }
    case C_MOTOR_SET_CONTROL_PID: {
      double controlP, controlI, controlD;
      stream >> controlP;
      stream >> controlI;
      stream >> controlD;
      mControlPID->setValue(controlP, controlI, controlD);
      awake();
      break;
    }
    case C_MOTOR_FEEDBACK: {
      short rate;
      stream >> rate;
      enableMotorFeedback(rate);
      break;
    }
    case C_MOTOR_GET_ASSOCIATED_DEVICE: {
      short deviceType;
      stream >> deviceType;
      assert(mRequestedDeviceTag == NULL);
      mRequestedDeviceTag = new WbDeviceTag[1];
      WbLogicalDevice *device = getSiblingDeviceByType(deviceType);
      mRequestedDeviceTag[0] = device ? device->tag() : 0;
      break;
    }
    default:
      assert(0);  // Invalid command.
  }
}

void WbMotor::writeAnswer(QDataStream &stream) {
  if (mForceOrTorqueSensor && (refreshSensorIfNeeded() || mForceOrTorqueSensor->hasPendingValue())) {
    stream << tag();
    stream << (unsigned char)C_MOTOR_FEEDBACK;
    stream << (double)mForceOrTorqueLastValue;
    mForceOrTorqueSensor->resetPendingValue();
  }
  if (mRequestedDeviceTag != NULL) {
    stream << tag();
    stream << (unsigned char)C_MOTOR_GET_ASSOCIATED_DEVICE;
    stream << (unsigned short)mRequestedDeviceTag[0];
    delete[] mRequestedDeviceTag;
    mRequestedDeviceTag = NULL;
  }
  if (mNeedToConfigure)
    addConfigureToStream(stream);
}

bool WbMotor::refreshSensorIfNeeded() {
  if (isPowerOn() && mForceOrTorqueSensor && mForceOrTorqueSensor->needToRefresh()) {
    mForceOrTorqueLastValue = computeFeedback();
    mForceOrTorqueSensor->updateTimer();
    return true;
  }
  return false;
}

void WbMotor::reset(const QString &id) {
  WbJointDevice::reset(id);

  turnOffMotor();

  mTargetVelocity = mMaxVelocity->value();
  mCurrentVelocity = 0.0;
  mUserControl = false;
  mRawInput = 0.0;
  const WbJoint *const j = joint();
  mTargetPosition = j ? position() : 0.0;
  mErrorIntegral = 0.0;
  mPreviousError = 0.0;
  mMotorForceOrTorque = mMaxForceOrTorque->value();
}

void WbMotor::awake() const {
  const WbJoint *const j = joint();
  if (j) {
    WbSolid *const s = j->solidEndPoint();
    if (s)
      s->awake();
    return;
  }

  const WbPropeller *const p = propeller();
  const WbTrack *const t = dynamic_cast<WbTrack *>(parentNode());
  if (p || t) {
    WbSolid *const s = upperSolid();
    assert(s);
    s->awake();
  }
}

void WbMotor::addToCoupledMotors(WbMotor *motor) {
  if (!mCoupledMotors.contains(motor))
    mCoupledMotors.append(motor);
}

void WbMotor::resetPhysics() {
  mCurrentVelocity = 0;
}
