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

#include "WbTouchSensor.hpp"

#include "WbBasicJoint.hpp"
#include "WbFieldChecker.hpp"
#include "WbLookupTable.hpp"
#include "WbMFVector3.hpp"
#include "WbMathsUtilities.hpp"
#include "WbMatrix3.hpp"
#include "WbSFDouble.hpp"
#include "WbSensor.hpp"
#include "WbSolidMerger.hpp"

#include <ode/ode.h>
#include <QtCore/QDataStream>
#include <cassert>
#include "../../controller/c/messages.h"

void WbTouchSensor::init() {
  mIsTouching = false;
  mIsGuiTouch = false;
  mFeedback = NULL;
  mLut = NULL;
  mSensor = NULL;
  mDeviceType = BUMPER;
  mValues[0] = 0.0;
  mValues[1] = 0.0;
  mValues[2] = 0.0;

  mLookupTable = findMFVector3("lookupTable");
  mType = findSFString("type");
  mResolution = findSFDouble("resolution");

  mNeedToReconfigure = false;
}

WbTouchSensor::WbTouchSensor(WbTokenizer *tokenizer) : WbSolidDevice("TouchSensor", tokenizer) {
  init();
}

WbTouchSensor::WbTouchSensor(const WbTouchSensor &other) : WbSolidDevice(other) {
  init();
}

WbTouchSensor::WbTouchSensor(const WbNode &other) : WbSolidDevice(other) {
  init();
}

WbTouchSensor::~WbTouchSensor() {
  delete mLut;
  delete mSensor;
  delete mFeedback;
}

void WbTouchSensor::preFinalize() {
  WbSolidDevice::preFinalize();

  mSensor = new WbSensor();

  updateLookupTable();
  updateType();
}

void WbTouchSensor::postFinalize() {
  WbSolidDevice::postFinalize();

  connect(mLookupTable, &WbMFVector3::changed, this, &WbTouchSensor::updateLookupTable);
  connect(mType, &WbSFString::changed, this, &WbTouchSensor::updateType);
  connect(mResolution, &WbSFDouble::changed, this, &WbTouchSensor::updateResolution);
}

void WbTouchSensor::updateLookupTable() {
  // rebuild lookup table
  delete mLut;
  mLut = new WbLookupTable(*mLookupTable);

  mValues[0] = mLut->minValue();
  mValues[1] = mLut->minValue();
  mValues[2] = mLut->minValue();

  mNeedToReconfigure = true;
}

void WbTouchSensor::updateType() {
  if (mType->value() == "bumper")
    mDeviceType = BUMPER;
  else if (mType->value() == "force")
    mDeviceType = FORCE;
  else if (mType->value() == "force-3d")
    mDeviceType = FORCE3D;
  else
    mDeviceType = BUMPER;

  if (mDeviceType == BUMPER && mType->value() != "bumper")
    parsingWarn(tr("Unknown 'type': \"%1\". Set to \"bumper\"").arg(mType->value()));

  if ((mDeviceType == FORCE || mDeviceType == FORCE3D) && !physics())
    parsingWarn(tr("\"force\" and \"force-3d\" 'type' requires 'physics' to be functional."));
}

void WbTouchSensor::updateResolution() {
  WbFieldChecker::resetDoubleIfNonPositiveAndNotDisabled(this, mResolution, -1.0, -1.0);
}

void WbTouchSensor::handleMessage(QDataStream &stream) {
  unsigned char command;
  short refreshRate;
  stream >> command;

  switch (command) {
    case C_SET_SAMPLING_PERIOD:
      stream >> refreshRate;
      mSensor->setRefreshRate(refreshRate);
      return;
    default:
      assert(0);
  }
}

void WbTouchSensor::writeAnswer(QDataStream &stream) {
  if (refreshSensorIfNeeded() || mSensor->hasPendingValue()) {
    stream << tag();
    if (mDeviceType != FORCE3D) {  // BUMPER or FORCE
      stream << (unsigned char)C_TOUCH_SENSOR_DATA;
      stream << (double)mValues[0];
    } else {  // FORCE_3D
      stream << (unsigned char)C_TOUCH_SENSOR_DATA_3D;
      stream << (double)mValues[0] << (double)mValues[1] << (double)mValues[2];
    }
    mSensor->resetPendingValue();
  }

  if (mNeedToReconfigure)
    addConfigure(stream);
}

void WbTouchSensor::addConfigure(QDataStream &stream) {
  stream << (short unsigned int)tag();
  stream << (unsigned char)C_CONFIGURE;
  stream << (int)mDeviceType;
  stream << (int)mLookupTable->size();
  for (int i = 0; i < mLookupTable->size(); i++) {
    stream << (double)mLookupTable->item(i).x();
    stream << (double)mLookupTable->item(i).y();
    stream << (double)mLookupTable->item(i).z();
  }
  mNeedToReconfigure = false;
}

bool WbTouchSensor::refreshSensorIfNeeded() {
  if (isPowerOn() && mSensor->needToRefresh()) {
    computeValue();
    mSensor->updateTimer();
    return true;
  }
  return false;
}

void WbTouchSensor::computeValue() {
  if (mDeviceType == FORCE) {  // "force" sensors
    // TODO: check why mFeedback is NULL
    if (!mFeedback) {
      mValues[0] = mLut->lookup(0.0);
      return;
    }

    WbVector3 f1(&mFeedback->f1[0]);  // create WbVector3 from ODE vector

    // by convention, TouchSensor's z-axis is the sensitive axis
    // the orientation of the z-axis can be read directly from the rotation matrix
    WbVector3 zaxis = matrix().zAxis();

    // compute how much of the force is aligned with the TouchSensors's z-axis
    // use dot product: |force| * cos(theta) = dot(zaxis, sum) / |zaxis|
    // we know that: |zaxis| == 1.0 (approximatively), therefore it can be ignored
    double force = zaxis.dot(f1);

    // ignore negative forces because they would represent a pull rather than a push on the sensor
    force = force < 0.0 ? -force : 0.0;

    // lookup and add noise
    mValues[0] = mLut->lookup(force);

    // apply resolution
    if (mResolution->value() != -1.0)
      mValues[0] = WbMathsUtilities::discretize(mValues[0], mResolution->value());
  } else if (mDeviceType == FORCE3D) {
    // TODO: check why mFeedback is NULL
    if (!mFeedback) {
      mValues[0] = mLut->lookup(0.0);
      mValues[1] = mLut->lookup(0.0);
      mValues[2] = mLut->lookup(0.0);
      return;
    }

    WbVector3 f1(&mFeedback->f1[0]);  // create WbVector3 from ODE vector

    // take into account the sensor's current orientation
    WbVector3 vec = f1 * matrix();

    // lookup
    mValues[0] = mLut->lookup(vec[0]);
    mValues[1] = mLut->lookup(vec[1]);
    mValues[2] = mLut->lookup(vec[2]);

    // apply resolution
    if (mResolution->value() != -1.0) {
      mValues[0] = WbMathsUtilities::discretize(mValues[0], mResolution->value());
      mValues[1] = WbMathsUtilities::discretize(mValues[1], mResolution->value());
      mValues[2] = WbMathsUtilities::discretize(mValues[2], mResolution->value());
    }
  } else {  // BUMPER
    // for "bumpers" we don't need to do the math, we return a binary value
    mValues[0] = (mIsTouching != mIsGuiTouch) ? 1 : 0;
    mIsTouching = false;  // reset once it was read
  }
}

void WbTouchSensor::setODEDynamicFlag(WbBaseNode *_node) {
  WbGeometry *geom = dynamic_cast<WbGeometry *>(_node);

  if (!geom) {
    WbShape *shape = dynamic_cast<WbShape *>(_node);
    if (shape)
      geom = shape->geometry();
  }
  if (geom)
    dGeomSetDynamicFlag(geom->odeGeom());
  else {
    WbGroup *group = dynamic_cast<WbGroup *>(_node);
    if (group) {
      for (int i = 0; i < group->childCount(); i++)
        setODEDynamicFlag(group->child(i));
    }
  }
}

void WbTouchSensor::createOdeObjects() {
  WbSolidDevice::createOdeObjects();
  WbBaseNode *node = WbSolidDevice::boundingObject();
  setODEDynamicFlag(node);
}

dJointID WbTouchSensor::createJoint(dBodyID body, dBodyID parentBody, dWorldID world) const {
  dJointID joint = dJointCreateFixed(world, 0);
  dJointAttach(joint, parentBody, body);
  dJointSetFixed(joint);

  if (forceBehavior()) {
    if (!mFeedback)
      mFeedback = new dJointFeedback;
    dJointSetFeedback(joint, mFeedback);
  } else {
    delete mFeedback;
    mFeedback = NULL;
  }

  return joint;
}

void WbTouchSensor::writeConfigure(QDataStream &stream) {
  mSensor->connectToRobotSignal(robot());
  addConfigure(stream);
}

bool WbTouchSensor::forceBehavior() const {
  return (mDeviceType == FORCE || mDeviceType == FORCE3D);
}

void WbTouchSensor::setSolidMerger() {
  mSolidMerger = physics() ? new WbSolidMerger(this) : NULL;
}

void WbTouchSensor::jerk(bool resetVelocities, bool rootJerk) {
  const WbSolid *const us = jointParent() ? NULL : upperSolid();
  const bool inherit = us && us->physics();
  WbSolidMerger *tempSolidMerger = inherit ? us->solidMerger() : QPointer<WbSolidMerger>(new WbSolidMerger(this));
  bool isTempSolidMerger = physics() && tempSolidMerger && tempSolidMerger->solid() == this;

  if (isTempSolidMerger) {
    tempSolidMerger->setGeomAndBodyPositions(resetVelocities, jointParents().size() == 0 && !isTopSolid());
  } else
    tempSolidMerger->solid()->updateOdeGeomPosition();

  foreach (WbSolid *const solid, solidChildren())
    solid->jerk(resetVelocities, false);

  foreach (WbBasicJoint *const joint, jointChildren())
    joint->updateOdeWorldCoordinates();

  if (isDynamic() && jointParents().size() > 0 && rootJerk)
    emit positionChangedArtificially();
}
