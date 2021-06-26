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

#include "WbInertialUnit.hpp"

#include "WbFieldChecker.hpp"
#include "WbMFVector3.hpp"
#include "WbMathsUtilities.hpp"
#include "WbMatrix3.hpp"
#include "WbRandom.hpp"
#include "WbSensor.hpp"
#include "WbWorld.hpp"

#include "../../controller/c/messages.h"

#include <ode/ode.h>
#include <QtCore/QDataStream>
#include <cassert>

void WbInertialUnit::init() {
  mSensor = NULL;

  mNoise = findSFDouble("noise");
  mXAxis = findSFBool("xAxis");
  mYAxis = findSFBool("yAxis");
  mZAxis = findSFBool("zAxis");
  mResolution = findSFDouble("resolution");

  mNeedToReconfigure = false;
}

WbInertialUnit::WbInertialUnit(WbTokenizer *tokenizer) : WbSolidDevice("InertialUnit", tokenizer) {
  init();
}

WbInertialUnit::WbInertialUnit(const WbInertialUnit &other) : WbSolidDevice(other) {
  init();
}

WbInertialUnit::WbInertialUnit(const WbNode &other) : WbSolidDevice(other) {
  init();
}

WbInertialUnit::~WbInertialUnit() {
  delete mSensor;
}

void WbInertialUnit::preFinalize() {
  WbSolidDevice::preFinalize();
  mSensor = new WbSensor();
  updateNoise();
}

void WbInertialUnit::postFinalize() {
  WbSolidDevice::postFinalize();
  connect(mResolution, &WbSFDouble::changed, this, &WbInertialUnit::updateResolution);
  connect(mNoise, &WbMFVector3::changed, this, &WbInertialUnit::updateNoise);
}

void WbInertialUnit::updateNoise() {
  mNeedToReconfigure = true;
}

void WbInertialUnit::updateResolution() {
  WbFieldChecker::resetDoubleIfNonPositiveAndNotDisabled(this, mResolution, -1.0, -1.0);
}

void WbInertialUnit::handleMessage(QDataStream &stream) {
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

void WbInertialUnit::writeAnswer(QDataStream &stream) {
  if (refreshSensorIfNeeded() || mSensor->hasPendingValue()) {
    stream << (short unsigned int)tag();
    stream << (unsigned char)C_INERTIAL_UNIT_DATA;
    stream << (double)mQuaternion.x() << (double)mQuaternion.y() << (double)mQuaternion.z() << (double)mQuaternion.w();

    mSensor->resetPendingValue();
  }

  if (mNeedToReconfigure)
    addConfigure(stream);
}

void WbInertialUnit::addConfigure(QDataStream &stream) {
  stream << (short unsigned int)tag();
  stream << (unsigned char)C_CONFIGURE;
  stream << (double)mNoise->value();
  const QByteArray &s = WbWorld::instance()->worldInfo()->coordinateSystem().toUtf8();
  stream.writeRawData(s.constData(), s.size() + 1);
  mNeedToReconfigure = false;
}

void WbInertialUnit::writeConfigure(QDataStream &stream) {
  mSensor->connectToRobotSignal(robot());
  addConfigure(stream);
}

bool WbInertialUnit::refreshSensorIfNeeded() {
  if (isPowerOn() && mSensor->needToRefresh()) {
    computeValue();
    mSensor->updateTimer();
    return true;
  }
  return false;
}

void WbInertialUnit::computeValue() {
  WbMatrix3 e = rotationMatrix();

  if (mNoise->value() != 0.0) {
    const double noise = mNoise->value() * M_PI;
    e *= WbMatrix3(noise * WbRandom::nextGaussian(), noise * WbRandom::nextGaussian(), noise * WbRandom::nextGaussian());
  }

  if (!mXAxis->isTrue() || !mYAxis->isTrue() || !mZAxis->isTrue()) {
    WbAxisAngle aa = e.toAxisAngle();
    if (!mXAxis->isTrue())
      aa.axis().setX(0);
    if (!mYAxis->isTrue())
      aa.axis().setZ(0);
    if (!mZAxis->isTrue())
      aa.axis().setY(0);
    aa.axis().normalize();
    e = WbMatrix3(aa.axis(), aa.angle());
  }

  mQuaternion = e.toQuaternion();

  // apply resolution if needed
  if (mResolution->value() != -1.0) {
    mQuaternion.setX(WbMathsUtilities::discretize(mQuaternion.x(), mResolution->value()));
    mQuaternion.setY(WbMathsUtilities::discretize(mQuaternion.y(), mResolution->value()));
    mQuaternion.setZ(WbMathsUtilities::discretize(mQuaternion.z(), mResolution->value()));
    mQuaternion.setW(WbMathsUtilities::discretize(mQuaternion.w(), mResolution->value()));
  }
}
