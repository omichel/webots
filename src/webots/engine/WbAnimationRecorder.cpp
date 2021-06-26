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

#include "WbAnimationRecorder.hpp"

#include "WbField.hpp"
#include "WbGroup.hpp"
#include "WbLog.hpp"
#include "WbRobot.hpp"
#include "WbSFRotation.hpp"
#include "WbSimulationState.hpp"
#include "WbSupervisorUtilities.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QMutableListIterator>

// this function is used to round the transform position coordinates
#define ROUND(x, precision) (roundf((x) / precision) * precision)

WbAnimationCommand::WbAnimationCommand(const WbNode *n, const QStringList &fields, bool saveInitialValue) :
  mNode(n),
  mChangedFromStart(false) {
  QString state;
  for (int i = 0; i < fields.size(); ++i) {
    WbField *field = mNode->findField(fields[i], true);
    if (field) {
      connect(field, &WbField::valueChanged, this, &WbAnimationCommand::updateValue);
      connect(field, &WbField::valueChangedByOde, this, &WbAnimationCommand::updateValue);
      connect(field, &WbField::valueChangedByWebots, this, &WbAnimationCommand::updateValue);
      mFields.append(field);

      if (saveInitialValue) {
        const WbSFVector3 *sfVector3 = dynamic_cast<WbSFVector3 *>(field->value());
        const WbSFRotation *sfRotation = dynamic_cast<WbSFRotation *>(field->value());
        const QString &fieldName = field->name();
        if (!state.isEmpty())
          state += ",";
        state += "\"" + fieldName + "\":\"";
        if (sfVector3 && fieldName.compare("translation") == 0) {
          // special translation case
          state += QString("%1 %2 %3")
                     .arg(ROUND(sfVector3->x(), 0.0001))
                     .arg(ROUND(sfVector3->y(), 0.0001))
                     .arg(ROUND(sfVector3->z(), 0.0001));
          mLastTranslation =
            WbVector3(ROUND(sfVector3->x(), 0.001), ROUND(sfVector3->y(), 0.001), ROUND(sfVector3->z(), 0.001));
        } else if (sfRotation && fieldName.compare("rotation") == 0) {
          // special rotation case
          state += QString("%1 %2 %3 %4")
                     .arg(ROUND(sfRotation->x(), 0.0001))
                     .arg(ROUND(sfRotation->y(), 0.0001))
                     .arg(ROUND(sfRotation->z(), 0.0001))
                     .arg(ROUND(sfRotation->angle(), 0.0001));
          mLastRotation = WbRotation(ROUND(sfRotation->x(), 0.001), ROUND(sfRotation->y(), 0.001),
                                     ROUND(sfRotation->z(), 0.001), ROUND(sfRotation->angle(), 0.001));
        } else  // generic case
          state += field->value()->toString(WbPrecision::FLOAT_MAX);
        state += "\"";
      }
    }
  }

  if (!state.isEmpty())
    mInitialState = QString("{\"id\":%1,%2}").arg(n->uniqueId()).arg(state);
}

void WbAnimationCommand::resetChanges() {
  mChangedValues.clear();
}

void WbAnimationCommand::addArtificialFieldChange(const QString &fieldName, const QString &value) {
  mChangedValues[fieldName] = value;
}

void WbAnimationCommand::updateValue() {
  const WbField *field = dynamic_cast<WbField *>(sender());
  if (field)
    updateFieldValue(field);
}

void WbAnimationCommand::updateAllFieldValues() {
  for (int i = 0; i < mFields.size(); ++i) {
    const WbField *field = mFields.at(i);
    if (!mChangedValues.contains(field->name()))
      updateFieldValue(field);
  }
}

void WbAnimationCommand::updateFieldValue(const WbField *field) {
  const WbSFVector3 *sfVector3 = dynamic_cast<WbSFVector3 *>(field->value());
  const WbSFRotation *sfRotation = dynamic_cast<WbSFRotation *>(field->value());
  if (sfVector3 && field->name().compare("translation") == 0) {
    // special translation case
    const WbVector3 translationRounded =
      WbVector3(ROUND(sfVector3->x(), 0.001), ROUND(sfVector3->y(), 0.001), ROUND(sfVector3->z(), 0.001));
    if (translationRounded != mLastTranslation) {
      mChangedValues["translation"] = QString("%1 %2 %3")
                                        .arg(ROUND(sfVector3->x(), 0.0001))
                                        .arg(ROUND(sfVector3->y(), 0.0001))
                                        .arg(ROUND(sfVector3->z(), 0.0001));
      mLastTranslation = translationRounded;
      mChangedFromStart = true;
      emit changed(this);
    }
  } else if (sfRotation && field->name().compare("rotation") == 0) {
    // special rotation case
    const WbRotation rotationRounded = WbRotation(ROUND(sfRotation->x(), 0.001), ROUND(sfRotation->y(), 0.001),
                                                  ROUND(sfRotation->z(), 0.001), ROUND(sfRotation->angle(), 0.001));
    if (rotationRounded != mLastRotation) {
      mChangedValues["rotation"] = QString("%1 %2 %3 %4")
                                     .arg(ROUND(sfRotation->x(), 0.0001))
                                     .arg(ROUND(sfRotation->y(), 0.0001))
                                     .arg(ROUND(sfRotation->z(), 0.0001))
                                     .arg(ROUND(sfRotation->angle(), 0.0001));
      mLastRotation = rotationRounded;
      mChangedFromStart = true;
      emit changed(this);
    }
  } else {
    // generic case
    mChangedValues[field->name()] = field->value()->toString(WbPrecision::FLOAT_MAX);
    mChangedFromStart = true;
    emit changed(this);
  }
}

WbAnimationRecorder *WbAnimationRecorder::cInstance = NULL;

WbAnimationRecorder *WbAnimationRecorder::instance() {
  if (cInstance == NULL) {
    cInstance = new WbAnimationRecorder;
    qAddPostRoutine(WbAnimationRecorder::cleanup);
  }
  return cInstance;
}

void WbAnimationRecorder::cleanup() {
  delete cInstance;
  cInstance = NULL;
}

WbAnimationRecorder::WbAnimationRecorder() :
  mIsRecording(false),
  mStartedFromGui(false),
  mLastUpdateTime(0.0),
  mFile(NULL),
  mFirstFrame(true),
  mStreamingServer(false) {
}

WbAnimationRecorder::~WbAnimationRecorder() {
  try {
    stopRecording();
  } catch (const QString &e) {
    WbLog::warning(tr("Error when stopping the HTML5 animation recording: '%1'").arg(e), true);
  }
}

void WbAnimationRecorder::initFromStreamingServer() {
  if (mFile)
    throw tr("HTML5 animation recorder is enabled.");

  if (mStreamingServer)
    throw tr("Streaming server already initialized.");

  emit initalizedFromStreamingServer();

  populateCommands();

  mStreamingServer = true;
}

void WbAnimationRecorder::cleanupFromStreamingServer() {
  emit cleanedUpFromStreamingServer();

  cleanCommands();

  mStreamingServer = false;
}

void WbAnimationRecorder::propagateNodeAddition(WbNode *node) {
  if (!mStreamingServer)
    return;

  populateCommands();
}

void WbAnimationRecorder::populateCommands() {
  cleanCommands();

  const WbWorld *world = WbWorld::instance();
  if (world) {
    QList<WbNode *> nodes = WbWorld::instance()->root()->subNodes(true);
    for (int i = 0; i < nodes.size(); ++i) {
      const WbNode *node = nodes.at(i);
      if (node->isUseNode())
        // skip updates for USE nodes
        // DEF/USE mechanism is handled in webots.min.js
        continue;
      const QStringList fields = node->fieldsToSynchronizeWithX3D();
      if (fields.size() > 0) {
        WbAnimationCommand *command = new WbAnimationCommand(node, fields, !mStreamingServer);
        mCommands << command;
      }
    }

    const QList<WbRobot *> &robots = WbWorld::instance()->robots();
    foreach (WbRobot *const robot, robots) {
      if (robot->supervisor()) {
        foreach (QString label, robot->supervisorUtilities()->labelsState())
          addChangedLabelToList(label);

        connect(robot->supervisorUtilities(), &WbSupervisorUtilities::labelChanged, this,
                &WbAnimationRecorder::addChangedLabelToList);
      }
    }
  }

  foreach (WbAnimationCommand *command, mCommands) {
    connect(command, &WbAnimationCommand::changed, this, &WbAnimationRecorder::addChangedCommandToList);
    // support node deletions
    connect(command->node(), &WbNode::destroyed, this, &WbAnimationRecorder::updateCommandsAfterNodeDeletion);
  }

  connect(WbWorld::instance()->viewpoint(), &WbViewpoint::nodeVisibilityChanged, this,
          &WbAnimationRecorder::handleNodeVisibilityChange);
}

void WbAnimationRecorder::cleanCommands() {
  foreach (WbAnimationCommand *command, mCommands) {
    disconnect(command, &WbAnimationCommand::changed, this, &WbAnimationRecorder::addChangedCommandToList);
    disconnect(command->node(), &WbNode::destroyed, this, &WbAnimationRecorder::updateCommandsAfterNodeDeletion);
    delete command;
  }
  mCommands.clear();
  mChangedCommands.clear();
  mChangedLabels.clear();
  foreach (WbAnimationCommand *command, mArtificialCommands)
    delete command;
  mArtificialCommands.clear();
}

void WbAnimationRecorder::addChangedCommandToList(WbAnimationCommand *command) {
  if (!mChangedCommands.contains(command))
    mChangedCommands.append(command);
}

void WbAnimationRecorder::addChangedLabelToList(const QString &label) {
  if (!mChangedLabels.contains(label))
    mChangedLabels.append(label);
}

void WbAnimationRecorder::handleNodeVisibilityChange(WbNode *node, bool visibility) {
  WbAnimationCommand *newCommand = NULL;
  foreach (WbAnimationCommand *command, mArtificialCommands) {
    if (command->node() == node) {
      newCommand = command;
      break;
    }
  }
  if (!newCommand) {
    newCommand = new WbAnimationCommand(node, QStringList(), !mStreamingServer);
    mArtificialCommands << newCommand;
  }
  if (visibility)
    newCommand->addArtificialFieldChange("render", "true");
  else
    newCommand->addArtificialFieldChange("render", "false");
}

void WbAnimationRecorder::updateCommandsAfterNodeDeletion(QObject *node) {
  QMutableListIterator<WbAnimationCommand *> it(mCommands);
  while (it.hasNext()) {
    WbAnimationCommand *command = it.next();
    if (command->node() == node) {
      it.remove();
      mChangedCommands.removeAll(command);
      disconnect(command, &WbAnimationCommand::changed, this, &WbAnimationRecorder::addChangedCommandToList);
      disconnect(command->node(), &WbNode::destroyed, this, &WbAnimationRecorder::updateCommandsAfterNodeDeletion);
      delete command;
    }
  }
}

void WbAnimationRecorder::update() {
  double currentTime = WbSimulationState::instance()->time();
  if (mLastUpdateTime < 0.0 || currentTime - mLastUpdateTime >= 1000.0 / WbWorld::instance()->worldInfo()->fps()) {
    const QString data = computeUpdateData();
    if (data.isEmpty())
      return;

    QTextStream out(mFile);

    if (!mFirstFrame)
      out << ",\n";

    out << data;

    mFirstFrame = false;
    mLastUpdateTime = currentTime;
  }
}

QString WbAnimationRecorder::computeUpdateData(bool force) {
  // Note: the copy to json is done in 2 passes in order
  //       to be able to manage correctly the trailing ',' characters
  if (force) {
    foreach (WbAnimationCommand *command, mChangedCommands)
      command->updateAllFieldValues();
  }
  QString result;
  QTextStream out(&result);
  const double time = WbSimulationState::instance()->time();
  out << "{\"time\":" << QString::number(time);
  const QList<WbAnimationCommand *> commands = mChangedCommands + mArtificialCommands;
  if (commands.size() == 0 && mChangedLabels.size() == 0) {
    out << "}";
    return result;
  }
  out << ",\"poses\":[";
  foreach (WbAnimationCommand *command, commands) {
    const QList<QString> keys = command->fields();
    if (keys.isEmpty())
      continue;
    out << "{";
    out << QString("\"id\":%1").arg(command->node()->uniqueId());
    foreach (const QString &fieldName, keys)
      out << QString(",\"%1\":\"%2\"").arg(fieldName).arg(command->fieldValue(fieldName));
    if (command == commands.last())
      out << "}";
    else
      out << "},";
    command->resetChanges();
  }
  out << "]";

  if (mChangedLabels.size() != 0) {
    out << ",\"labels\":[";
    foreach (QString label, mChangedLabels) {
      out << "{";
      out << label;
      mLabelsIds.insert(label.mid(5, label.indexOf("font") - 7));
      if (label == mChangedLabels.last())
        out << "}";
      else
        out << "},";
    }
    out << "]";
  }

  out << "}";

  mChangedCommands.clear();
  foreach (WbAnimationCommand *command, mArtificialCommands)
    delete command;
  mArtificialCommands.clear();
  mChangedLabels.clear();

  return result;
}

void WbAnimationRecorder::startRecording(const QString &targetFile) {
  mFile = new QFile(targetFile);
  if (!mFile->open(QIODevice::WriteOnly))
    throw tr("Cannot open HTML5 animation file '%1'").arg(mFile->fileName());

  populateCommands();

  connect(WbSimulationState::instance(), &WbSimulationState::physicsStepEnded, this, &WbAnimationRecorder::update);

  mLastUpdateTime = -1;
  mIsRecording = true;
  mFirstFrame = true;

  WbLog::info(tr("Start HTML5 animation export\n"));
}

void WbAnimationRecorder::stop() {
  try {
    stopRecording();
  } catch (const QString &e) {
    WbLog::warning(tr("Error when stopping the HTML5 animation recording: '%1'").arg(e), true);
    emit animationStopStatusChanged(false);
  }
}

void WbAnimationRecorder::start(const QString &fileName) {
  const WbWorld *world = WbWorld::instance();
  connect(world, &WbWorld::destroyed, this, &WbAnimationRecorder::stop);

  mAnimationFilename = fileName;
  mAnimationFilename.replace(QRegExp(".html$", Qt::CaseInsensitive), ".json");

  try {
    const bool success = world->exportAsHtml(fileName, true);
    if (!success)
      throw tr("HTML5 export failed");

    startRecording(mAnimationFilename);

    emit animationStartStatusChanged(true);
  } catch (const QString &e) {
    WbLog::error(tr("Error when starting the HTML5 animation recording: '%1'").arg(e), true);
    emit animationStartStatusChanged(false);
  }
}

void WbAnimationRecorder::stopRecording() {
  disconnect(WbSimulationState::instance(), &WbSimulationState::physicsStepEnded, this, &WbAnimationRecorder::update);
  mIsRecording = false;
  if (!mFile)
    return;
  mFile->close();
  const WbWorld *const world = WbWorld::instance();
  if (!world) {  // the world is being reverted, aborting the animation and deleting the incomplete animation file
    mFile->remove();
    delete mFile;
    mFile = NULL;
    return;
  }
  // prepend header and initial state to the file containing updates
  mFile->open(QFile::ReadOnly | QFile::Text);
  const QByteArray updates = mFile->readAll();
  mFile->close();
  mFile->open(QFile::WriteOnly | QFile::Text);

  QTextStream out(mFile);
  out << "{\n";
  // write header
  const WbWorldInfo *const worldInfo = world->worldInfo();
  const double step = worldInfo->basicTimeStep() * ceil((1000.0 / worldInfo->fps()) / worldInfo->basicTimeStep());
  out << QString(" \"basicTimeStep\":%1,\n").arg(step);
  out << " \"ids\":\"";
  bool firstCommand = true;
  QList<WbAnimationCommand *> commandsChangedFromStart;
  foreach (WbAnimationCommand *command, mCommands) {
    // store only ids of nodes that changed during the animation
    if (command->isChangedFromStart()) {
      commandsChangedFromStart << command;
      // cppcheck-suppress knownConditionTrueFalse
      if (!firstCommand)
        out << ";";
      else
        firstCommand = false;
      out << command->node()->uniqueId();
    }
  }
  out << "\",\n";

  out << " \"labelsIds\":\"";
  bool firstLabel = true;
  foreach (QString id, mLabelsIds) {
    // cppcheck-suppress knownConditionTrueFalse
    if (!firstLabel)
      out << ";";
    else
      firstLabel = false;
    out << id;
  }

  out << "\",\n";

  out << " \"frames\":[\n";
  // write initial state
  out << "{\"time\":0,\"poses\":[";
  foreach (WbAnimationCommand *command, commandsChangedFromStart) {
    // store only initial state of nodes that changed during the animation
    if (command != commandsChangedFromStart.first())
      out << ",";
    out << command->initialState();
  }

  cleanCommands();
  out << "]}";
  if (!updates.isEmpty()) {
    out << ",\n";
    out << updates;
  }
  out << "\n ]\n}\n";
  mFile->close();

  const QFileInfo fi(mFile->fileName());
  const QString fileName = fi.absolutePath() + "/" + fi.baseName() + ".html";
  WbLog::info(tr("HTML5 animation successfully exported in '%1'\n").arg(fileName));

  if (mStartedFromGui && !mStreamingServer)
    emit requestOpenUrl(fileName,
                        tr("The animation has been created:<br>%1<br><br>Do you want to view it locally now?<br><br>"
                           "Note: please refer to the "
                           "<a style='color: #5DADE2;' href='https://cyberbotics.com/doc/guide/"
                           "web-scene#remarks-on-the-used-technologies-and-their-limitations'>User Guide</a> "
                           "if your browser prevents local files CORS requests.")
                          .arg(fileName),
                        tr("Make HTML5 Animation"));

  delete mFile;
  mFile = NULL;
}
