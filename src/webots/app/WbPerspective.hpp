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

#ifndef WB_PERSPECTIVE_HPP
#define WB_PERSPECTIVE_HPP

//
// Description: user interface configuration associated to each world file
//

#include "WbAction.hpp"
#include "WbVersion.hpp"

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

class QTextStream;

struct ConsoleSettings {
  QStringList enabledFilters;
  QStringList enabledLevels;
  QString name;
};

class WbPerspective {
public:
  // create perspective for this world file
  explicit WbPerspective(const QString &worldPath);

  // destructor
  ~WbPerspective();

  // dock/toolbar state of the main window
  void setMainWindowState(const QByteArray &state) { mState = state; }
  const QByteArray &mainWindowState() const { return mState; }

  // dock/toolbar state when the main window is minimized
  void setMinimizedState(const QByteArray &state) { mMinimizedState = state; }
  const QByteArray &minimizedState() const { return mMinimizedState; }

  // index of the maximized widget (or -1 if there is no maximization)
  void setMaximizedDockId(int id) { mMaximizedDockId = id; }
  int maximizedDockId() const { return mMaximizedDockId; }

  // should the main window's central widget be shown
  void setCentralWidgetVisible(bool visible) { mCentralWidgetVisible = visible; }
  bool centralWidgetVisible() const { return mCentralWidgetVisible; }

  // splitter state of the simulation view
  void setSimulationViewState(QList<QByteArray> state);
  QList<QByteArray> simulationViewState() const;

  // index of the selected tab in the text editor
  void setSelectedTab(int tab) { mSelectedTab = tab; }
  int selectedTab() const { return mSelectedTab; }

  // orthographic view height of the viewpoint
  void setOrthographicViewHeight(double ovh) { mOrthographicViewHeight = (ovh <= 0) ? 1 : ovh; }
  double orthographicViewHeight() const { return mOrthographicViewHeight; }

  // list of opened files in the text editor
  void setFilesList(const QStringList &list) { mFilesList = list; }
  QStringList filesList() const { return mFilesList; }

  void setRobotWindowNodeNames(const QStringList &robotWindowNodeNames) { mRobotWindowNodeNames = robotWindowNodeNames; }
  const QStringList &enabledRobotWindowNodeNames() const { return mRobotWindowNodeNames; }

  // consoles
  void setConsolesSettings(const QVector<ConsoleSettings> &settings) { mConsolesSettings = settings; }
  const QVector<ConsoleSettings> &consoleList() const { return mConsolesSettings; }

  // global optional renderings
  void enableGlobalOptionalRendering(const QString &optionalRenderingName, bool enable);
  bool isGlobalOptionalRenderingEnabled(const QString &optionalRenderingName) const {
    return mEnabledOptionalRenderingList.contains(optionalRenderingName);
  }

  // list of nodes with enabled optional renderings
  void clearEnabledOptionalRenderings();
  void setEnabledOptionalRendering(const QStringList &centerOfMassNodeNames, const QStringList &centerOfBuoyancyNodeNames,
                                   const QStringList &supportPolygonNodeNames) {
    mCenterOfMassNodeNames = centerOfMassNodeNames;
    mCenterOfBuoyancyNodeNames = centerOfBuoyancyNodeNames;
    mSupportPolygonNodeNames = supportPolygonNodeNames;
  }
  const QStringList &enabledCenterOfMassNodeNames() const { return mCenterOfMassNodeNames; }
  const QStringList &enabledCenterOfBuoyancyNodeNames() const { return mCenterOfBuoyancyNodeNames; }
  const QStringList &enabledSupportPolygonNodeNames() const { return mSupportPolygonNodeNames; }

  // selection and viewpoint lock mechanism
  void setUserInteractionDisabled(WbAction::WbActionKind action, bool disabled) {
    mDisabledUserInteractionsMap[action] = disabled;
  }
  QMap<WbAction::WbActionKind, bool> disabledUserInteractionsMap() const { return mDisabledUserInteractionsMap; }
  bool isUserInteractionDisabled(WbAction::WbActionKind action) const {
    return mDisabledUserInteractionsMap.value(action, false);
  }

  // projection and rendering mode
  void setProjectionMode(const QString &mode) { mProjectionMode = mode; }
  void setRenderingMode(const QString &mode) { mRenderingMode = mode; }
  const QString &projectionMode() const { return mProjectionMode; }
  const QString &renderingMode() const { return mRenderingMode; }

  // rendering devices perspective
  void clearRenderingDevicesPerspectiveList();
  void setRenderingDevicePerspective(const QString &deviceUniqueName, const QStringList &perspective);
  QStringList renderingDevicePerspective(const QString &deviceUniqueName) const;

  // load/save perspective
  bool load(bool reloading = false);
  bool save() const;

private:
  QString mBaseName;
  WbVersion mVersion;
  QByteArray mState;
  QByteArray mMinimizedState;
  QByteArray mSimulationViewState;
  QByteArray mSceneTreeState;
  int mMaximizedDockId;
  bool mCentralWidgetVisible;
  int mSelectedTab;
  QStringList mFilesList;
  double mOrthographicViewHeight;
  QMap<WbAction::WbActionKind, bool> mDisabledUserInteractionsMap;
  QString mProjectionMode;
  QString mRenderingMode;
  QStringList mEnabledOptionalRenderingList;
  QStringList mRobotWindowNodeNames;
  QStringList mCenterOfMassNodeNames;
  QStringList mCenterOfBuoyancyNodeNames;
  QStringList mSupportPolygonNodeNames;
  QVector<ConsoleSettings> mConsolesSettings;
  QMap<QString, QStringList> mRenderingDevicesPerspectiveList;

  bool readContent(QTextStream &in, bool reloading);
  void addDefaultConsole();
  static QString joinUniqueNameList(const QStringList &nameList);
  static void splitUniqueNameList(const QString &text, QStringList &targetList);
  static QString getActionName(WbAction::WbActionKind action);
  static WbAction::WbActionKind getActionFromString(const QString &actionString);
};

#endif
