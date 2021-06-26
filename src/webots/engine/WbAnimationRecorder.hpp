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

#ifndef WB_ANIMATION_RECORDER_HPP
#define WB_ANIMATION_RECORDER_HPP

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QSet>

#include "WbRotation.hpp"
#include "WbVector3.hpp"

class QFile;

class WbField;
class WbNode;

class WbAnimationCommand : public QObject {
  Q_OBJECT

public:
  WbAnimationCommand(const WbNode *n, const QStringList &fields, bool saveInitialValue);

  const WbNode *node() const { return mNode; }
  QList<QString> fields() const { return mChangedValues.keys(); }
  QString fieldValue(const QString &field) const { return mChangedValues[field]; }

  // Keep track of initial state that will be written to the animation file if the command changes during the animation
  const QString &initialState() const { return mInitialState; }
  bool isChangedFromStart() const { return mChangedFromStart; }

  // the addArtificialFieldChange method should be used to add a X3D field change that has no matching WBT field,
  // e.g., for example the "render" field.
  void addArtificialFieldChange(const QString &fieldName, const QString &value);
  void updateAllFieldValues();
  void resetChanges();

signals:
  void changed(WbAnimationCommand *command);

private:
  void updateFieldValue(const WbField *field);

  const WbNode *mNode;
  QList<WbField *> mFields;
  QHash<QString, QString> mChangedValues;
  WbVector3 mLastTranslation;
  WbRotation mLastRotation;
  QString mInitialState;
  bool mChangedFromStart;

private slots:
  void updateValue();
};

class WbAnimationRecorder : public QObject {
  Q_OBJECT

public:
  static WbAnimationRecorder *instance();
  static bool isInstantiated() { return (cInstance != NULL); }

  void setStartFromGuiFlag(bool flag) { mStartedFromGui = flag; }
  void initFromStreamingServer();
  QString computeUpdateData(bool force = false);
  void cleanupFromStreamingServer();

signals:
  void animationStartStatusChanged(int status);
  void animationStopStatusChanged(int status);
  void initalizedFromStreamingServer();
  void cleanedUpFromStreamingServer();
  void requestOpenUrl(const QString &fileName, const QString &message, const QString &title);

public slots:
  void start(const QString &fileName);
  void stop();
  void propagateNodeAddition(WbNode *node);

private slots:
  void update();
  void updateCommandsAfterNodeDeletion(QObject *);
  void addChangedCommandToList(WbAnimationCommand *command);
  void addChangedLabelToList(const QString &label);
  void handleNodeVisibilityChange(WbNode *node, bool visibility);

private:
  static WbAnimationRecorder *cInstance;
  static void cleanup();

  WbAnimationRecorder();
  virtual ~WbAnimationRecorder();

  void startRecording(const QString &targetFile);
  void stopRecording();

  void populateCommands();
  void cleanCommands();

  QString mResults;
  bool mIsRecording;
  bool mStartedFromGui;

  double mLastUpdateTime;

  QString mAnimationFilename;
  QFile *mFile;
  bool mFirstFrame;
  bool mStreamingServer;

  QList<WbAnimationCommand *> mCommands;
  QList<WbAnimationCommand *> mChangedCommands;
  QList<WbAnimationCommand *> mArtificialCommands;
  QList<QString> mChangedLabels;
  QSet<QString> mLabelsIds;
};

#endif
