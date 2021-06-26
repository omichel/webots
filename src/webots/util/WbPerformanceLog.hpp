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

#ifndef WB_PERFORMANCE_LOG_HPP
#define WB_PERFORMANCE_LOG_HPP

//
// Description: class used to debug the performance of Webots:
//              - physics step
//              - graphics rendering
//              - controllers
//

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QVector>

class QElapsedTimer;

class Measurement;

class WbPerformanceLog {
public:
  enum InfoType {
    LOADING = 0,
    PRE_PHYSICS_STEP,
    PHYSICS_STEP,
    POST_PHYSICS_STEP,
    MAIN_RENDERING,
    VIRTUAL_REALITY_HEADSET_RENDERING,
    GPU_MEMORY_TRANSFER,
    MAIN_TRIANGLES_COUNT,
    DEVICE_RENDERING,
    DEVICE_WINDOW_RENDERING,
    CONTROLLER,
    SPEED_FACTOR,
    INFO_COUNT
  };

  static WbPerformanceLog *instance() { return cInstance; }
  static void createInstance(const QString &fileName, int stepsCount = 0);
  static void deleteInstance();
  static void enableSystemInfoLog(bool enabled);

  void worldClosed(const QString &worldName, const QString &worldMetrics);
  void writeLog(const QString &text);
  void stepChanged();
  void startMeasure(InfoType type, const QString &object = QString());
  void stopMeasure(InfoType type, const QString &object = QString());
  void startControllerMeasure(const QString &controllerName);
  void stopControllerMeasure(const QString &controllerName);
  void setTimeStep(double value) { mTimeStep = value; }
  void setAvgFPS(double value) { mAverageFPS = value; }

  void reportStepRenderingStats(int trianglesCount);

private:
  static WbPerformanceLog *cInstance;
  WbPerformanceLog(const QString &fileName, int stepsCount);
  WbPerformanceLog(const WbPerformanceLog &);  // non constructor-copyable
  ~WbPerformanceLog();

  bool openFile();
  void closeFile();
  void writeTotalValues();
  static QString justifiedNumber(double value, int size);

  QString mFileName;
  QFile *mFile;
  int mStepsCountToBeLogged;
  int mStepsCount;
  QVector<qint64> mValues;
  QVector<int> mValuesCount;
  QVector<QElapsedTimer *> mTimers;
  QTextStream mOutStream;
  double mAverageFPS;
  double mTimeStep;
  bool mIsLogCompleted;

  QHash<QString, Measurement *> mRenderingDevicesValues;
  QHash<QString, Measurement *> mControllersValues;
};

#endif
