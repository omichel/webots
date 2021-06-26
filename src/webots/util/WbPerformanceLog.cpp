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

#include "WbPerformanceLog.hpp"

#include "WbOdeContext.hpp"
#include "WbPreferences.hpp"
#include "WbSysInfo.hpp"

#include <QtCore/QElapsedTimer>
#include <cassert>

class Measurement {
public:
  explicit Measurement();
  ~Measurement();

  void start();
  void stop();
  double averageValue() const;  // in ms
  double totalValue() const;    // in ms

private:
  Measurement(const Measurement &);  // non constructor-copyable
  QElapsedTimer *mTimer;
  quint64 mTotalValue;
  unsigned int mValuesCount;
};

Measurement::Measurement() : mTotalValue(0), mValuesCount(0) {
  mTimer = new QElapsedTimer();
}

Measurement::~Measurement() {
  delete mTimer;
}

void Measurement::start() {
  if (mTimer->isValid())
    return;
  mTimer->start();
}

void Measurement::stop() {
  if (!mTimer->isValid())
    return;
  mTotalValue += (unsigned int)(mTimer->nsecsElapsed() * 1e-3);  // micro seconds precision
  mValuesCount++;
  mTimer->invalidate();
}

double Measurement::averageValue() const {
  if (mValuesCount == 0)
    return 0.0;
  return 1e-3 * (mTotalValue / mValuesCount);
}

double Measurement::totalValue() const {
  return 1e-3 * mTotalValue;
}

static bool gLogSystemInfo = false;
static const char *const gInfoLabels[] = {"loading",           "prePhysics",
                                          "physics",           "postPhysics",
                                          "mainRendering",     "virtualRealityHeadsetRendering",
                                          "gpuMemoryTransfer", "trianglesCount",
                                          "deviceRendering",   "deviceWindowRendering",
                                          "controller"};

WbPerformanceLog *WbPerformanceLog::cInstance = NULL;

void WbPerformanceLog::createInstance(const QString &fileName, int stepsCount) {
  if (cInstance)
    return;

  cInstance = new WbPerformanceLog(fileName, stepsCount);
}

void WbPerformanceLog::deleteInstance() {
  delete cInstance;
  cInstance = NULL;
}

void WbPerformanceLog::enableSystemInfoLog(bool enabled) {
  gLogSystemInfo = enabled;
}

WbPerformanceLog::WbPerformanceLog(const QString &fileName, int stepsCount) :
  mFileName(fileName),
  mFile(NULL),
  mStepsCountToBeLogged(stepsCount),
  mStepsCount(0),
  mValues(INFO_COUNT, 0.0),
  mValuesCount(INFO_COUNT, 0),
  mTimers(INFO_COUNT),
  mAverageFPS(0.0),
  mTimeStep(0),
  mIsLogCompleted(false) {
  mFile = new QFile(mFileName);
  for (int i = 0; i < INFO_COUNT; ++i)
    mTimers[i] = new QElapsedTimer();
}

WbPerformanceLog::~WbPerformanceLog() {
  for (int i = 0; i < INFO_COUNT; ++i) {
    delete mTimers[i];
    mTimers[i] = NULL;
  }
}

void WbPerformanceLog::worldClosed(const QString &worldName, const QString &worldMetrics) {
  if (mStepsCount == 0)
    return;

  // log the system info only once per file if requested
  bool logSysInfo = gLogSystemInfo && !mFile->exists();
  if (!openFile())
    return;

  QTextStream out(mFile);
  if (logSysInfo) {
    // system info
    out << "System: " << WbSysInfo::sysInfo() << "\n";
    out << "Processor: " << WbSysInfo::processor() << "\n";
    out << "Number of cores: " << WbSysInfo::coreCount() << "\n";
    out << "OpenGL vendor: " << WbSysInfo::openGLVendor() << "\n";
    out << "OpenGL renderer: " << WbSysInfo::openGLRenderer() << "\n";
    out << "OpenGL version: " << WbSysInfo::openGLVersion() << "\n";
    out << "\n";
  }

  out << "World: " << worldName << "\n";
  out << "World metrics: " << worldMetrics << "\n";

  closeFile();

  writeTotalValues();

  // reset values
  mIsLogCompleted = false;
  mStepsCount = 0;
  mTimeStep = 0;
  for (int i = 0; i < INFO_COUNT; ++i) {
    mValues[i] = 0;
    mValuesCount[i] = 0;
  }

  foreach (const QString &key, mRenderingDevicesValues.keys())
    delete mRenderingDevicesValues[key];
  mRenderingDevicesValues.clear();
  foreach (const QString &key, mControllersValues.keys())
    delete mControllersValues[key];
  mControllersValues.clear();
}

void WbPerformanceLog::writeTotalValues() {
  if (!openFile())
    return;
  QTextStream out(mFile);

  out << "Threads count: " << WbOdeContext::instance()->numberOfThreads() << "\n";

  const double averageSpeed = (double)mValuesCount[SPEED_FACTOR] * mTimeStep * 1e3 / ((double)mValues[SPEED_FACTOR]);

  WbPreferences *prefs = WbPreferences::instance();
  out << "Shadows disabled: " << (prefs->value("OpenGL/disableShadows").toBool() ? "true" : "false") << "\n";
  out << "Anti-aliasing disabled: " << (prefs->value("OpenGL/disableAntiAliasing").toBool() ? "true" : "false") << "\n";
  out << "Average speed factor: " << averageSpeed << "x\n";

  QList<QString> devicesKeys = mRenderingDevicesValues.keys();
  QList<QString> controllersKeys = mControllersValues.keys();

  QStringList headers;
  for (int i = 0; i < INFO_COUNT - 4; ++i) {
    QString s = QString("<") + gInfoLabels[i];
    if (i == MAIN_TRIANGLES_COUNT)
      s += ">";
    else
      s += "(ms)>";
    headers.append(s);
  }
  headers.append("<mainFPS>");
  foreach (QString key, devicesKeys)
    headers.append("<device:" + key + "(ms)>");
  foreach (QString key, controllersKeys)
    headers.append("<controller:" + key + "(ms)>");
  out << "<mode> <stepsCount> " << headers.join(" ");

  out << "\n" << QString("AVG").leftJustified(6, ' ') << " " << QString::number(mStepsCount).rightJustified(12, ' ') << " ";
  int i = 0;
  for (; i < INFO_COUNT - 4; ++i) {
    double value = 0.0;
    if (i == MAIN_TRIANGLES_COUNT)
      value = ((double)mValues[i]) / ((double)mValuesCount[i]);
    else if (mValuesCount[i] == 0)
      value = 0;
    else
      value = 1e-3 * mValues[i] / ((double)mValuesCount[i]);

    out << justifiedNumber(value, headers[i].size()) << " ";
  }
  out << justifiedNumber(mAverageFPS, headers[i++].size()) << " ";
  foreach (QString key, devicesKeys)
    out << justifiedNumber(mRenderingDevicesValues.value(key)->averageValue(), headers[i++].size()) << " ";
  foreach (QString key, controllersKeys)
    out << justifiedNumber(mControllersValues.value(key)->averageValue(), headers[i++].size()) << " ";

  // total
  out << "\n" << QString("TOT").leftJustified(6, ' ') << " " << QString::number(mStepsCount).rightJustified(12, ' ') << " ";
  for (i = 0; i < INFO_COUNT - 4; ++i) {
    double value = 0.0;
    if (i == MAIN_TRIANGLES_COUNT)
      value = mValues[i];
    else
      value = 1e-3 * mValues[i];
    out << justifiedNumber(value, headers[i].size()) << " ";
  }
  out << justifiedNumber(mAverageFPS, headers[i++].size()) << " ";
  foreach (QString key, devicesKeys)
    out << justifiedNumber(mRenderingDevicesValues.value(key)->totalValue(), headers[i++].size()) << " ";
  foreach (QString key, controllersKeys)
    out << justifiedNumber(mControllersValues.value(key)->totalValue(), headers[i++].size()) << " ";
  out << "\n";
  out << "\n";

  closeFile();
}

void WbPerformanceLog::stepChanged() {
  if (mIsLogCompleted)
    return;

  if (mStepsCountToBeLogged > 0 && mStepsCount == mStepsCountToBeLogged)
    mIsLogCompleted = true;
  else
    mStepsCount++;

  if (mValuesCount[SPEED_FACTOR] == 0 && !mTimers[SPEED_FACTOR]->isValid()) {
    startMeasure(SPEED_FACTOR);
    return;
  }

  stopMeasure(SPEED_FACTOR);
  startMeasure(SPEED_FACTOR);
}

void WbPerformanceLog::startMeasure(InfoType type, const QString &object) {
  if (mIsLogCompleted)
    return;

  if (!object.isEmpty()) {
    Measurement *measurement = NULL;
    if (type == DEVICE_RENDERING || type == DEVICE_WINDOW_RENDERING) {
      measurement = mRenderingDevicesValues.value(object);
      if (!measurement) {
        measurement = new Measurement();
        mRenderingDevicesValues.insert(object, measurement);
      }
    } else if (type == CONTROLLER) {
      measurement = mControllersValues.value(object);
      if (!measurement) {
        measurement = new Measurement();
        mControllersValues.insert(object, measurement);
      }
    } else
      assert(false);
    measurement->start();
  } else
    mTimers[type]->start();
}

void WbPerformanceLog::stopMeasure(InfoType type, const QString &object) {
  if (mIsLogCompleted)
    return;

  if (!object.isEmpty()) {
    Measurement *measurement = NULL;
    if (type == DEVICE_RENDERING || type == DEVICE_WINDOW_RENDERING)
      measurement = mRenderingDevicesValues.value(object);
    else if (type == CONTROLLER)
      measurement = mControllersValues.value(object);
    if (measurement)
      measurement->stop();
  } else if (mTimers[type]->isValid()) {
    mValues[type] += mTimers[type]->nsecsElapsed() * 1e-3;
    mValuesCount[type] += 1;
    mTimers[type]->invalidate();
  }
}

void WbPerformanceLog::reportStepRenderingStats(int trianglesCount) {
  mValues[MAIN_TRIANGLES_COUNT] += trianglesCount;
  mValuesCount[MAIN_TRIANGLES_COUNT] += 1;
}

void WbPerformanceLog::writeLog(const QString &text) {
  if (!openFile())
    return;
  QTextStream out(mFile);
  out << text;
  closeFile();
}

bool WbPerformanceLog::openFile() {
  return mFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

void WbPerformanceLog::closeFile() {
  mFile->close();
}

QString WbPerformanceLog::justifiedNumber(double value, int size) {
  return QString::number(value, 'f', 3).rightJustified(size, ' ');
}
