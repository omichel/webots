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

#ifndef WB_MAIN_WINDOW_HPP
#define WB_MAIN_WINDOW_HPP

//
// Description: Webots main window
//

#include <QtCore/QMap>
#include <QtWidgets/QMainWindow>

#include "WbLog.hpp"

class WbBuildEditor;
class WbConsole;
class WbNode;
class WbOdeDebugger;
class WbRecentFilesList;
class WbRobot;
class WbSimulationView;
class WbStreamingServer;

class QMenu;
class QMenuBar;
class QProgressDialog;
class QTimer;

// cppcheck-suppress noConstructor
class WbMainWindow : public QMainWindow {
  Q_OBJECT
  Q_PROPERTY(QString enabledIconPath MEMBER mEnabledIconPath READ enabledIconPath WRITE setEnabledIconPath)
  Q_PROPERTY(QString disabledIconPath MEMBER mDisabledIconPath READ disabledIconPath WRITE setDisabledIconPath)
  Q_PROPERTY(QString coreIconPath MEMBER mCoreIconPath READ coreIconPath WRITE setCoreIconPath)
  Q_PROPERTY(QString toolBarAlign MEMBER mToolBarAlign READ toolBarAlign WRITE setToolBarAlign)

public:
  explicit WbMainWindow(bool minimizedOnStart, WbStreamingServer *streamingServer, QWidget *parent = NULL);
  virtual ~WbMainWindow();

  void lockFullScreen(bool isLocked);
  void savePerspective(bool reloading, bool saveToFile);
  void restorePerspective(bool reloading, bool firstLoad, bool loadingFromMemory);

  const QString &enabledIconPath() const { return mEnabledIconPath; }
  const QString &disabledIconPath() const { return mDisabledIconPath; }
  const QString &coreIconPath() const { return mCoreIconPath; }
  const QString &toolBarAlign() const { return mToolBarAlign; }

  void setEnabledIconPath(const QString &path) { mEnabledIconPath = path; }
  void setDisabledIconPath(const QString &path) { mDisabledIconPath = path; }
  void setCoreIconPath(const QString &path) { mCoreIconPath = path; }
  void setToolBarAlign(const QString &align) { mToolBarAlign = align; }

  void restorePreferredGeometry(bool minimizedOnStart = false);

signals:
  void restartRequested();
  void splashScreenCloseRequested();

public slots:
  bool loadDifferentWorld(const QString &fileName);
  bool loadWorld(const QString &fileName, bool reloading = false);
  bool setFullScreen(bool isEnabled, bool isRecording = false, bool showDialog = true, bool startup = false);
  void showGuidedTour();
  void setView3DSize(const QSize &size);
  void restoreRenderingDevicesPerspective();
  void resetWorldFromGui();

protected:
  bool event(QEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

private slots:
  void updateBeforeWorldLoading(bool reloading);
  void updateAfterWorldLoading(bool reloading, bool firstLoad);
  void newWorld();
  void openWorld();
  void openSampleWorld();
  void saveWorld();
  void saveWorldAs(bool skipSimulationHasRunWarning = false);
  void reloadWorld();
  void resetGui(bool restartControllers);
  void importVrml();
  void exportVrml();
  void exportHtml();
  void showAboutBox();
  void show3DViewingInfo();
  void show3DMovingInfo();
  void show3DForceInfo();
  void showOpenGlInfo();
  void showUserGuide();
  void showReferenceManual();
  void showAutomobileDocumentation();

  void openGithubRepository();
  void openCyberboticsWebsite();
  void openBugReport();
  void openNewsletterSubscription();
  void openDiscord();
  void openTwitter();
  void openYouTube();
  void openLinkedIn();

  void newProjectDirectory();
  void newRobotController();
  void newPhysicsPlugin();
  void openPreferencesDialog();
  void openWebotsUpdateDialogFromStartup();
  void openWebotsUpdateDialogFromMenu();
  void restoreLayout();
  void editPhysicsPlugin();
  void simulationEnabledChanged(bool);
  void showStatusBarMessage(WbLog::Level level, const QString &message);
  void editRobotController();
  void showRobotWindow();
  void updateOverlayMenu();
  void createWorldLoadingProgressDialog();
  void deleteWorldLoadingProgressDialog();
  void setWorldLoadingProgress(const int progress);
  void setWorldLoadingStatus(const QString &status);
  void startAnimationRecording();
  void stopAnimationRecording();
  void toggleAnimationIcon();
  void toggleAnimationAction(bool isRecording);
  void enableAnimationAction();
  void disableAnimationAction();

private:
  void showHtmlRobotWindow(WbRobot *);
  int mExitStatus;
  QList<WbConsole *> mConsoles;
  WbBuildEditor *mTextEditor;
  WbSimulationView *mSimulationView;
  WbRecentFilesList *mRecentFiles;
  WbOdeDebugger *mOdeDebugger;
  QMenu *mRecentFilesSubMenu;
  QByteArray *mFactoryLayout;
  QMenu *mSimulationMenu;
  QMenuBar *mMenuBar;
  QMenu *mOverlayMenu;
  QMenu *mRobotCameraMenu;
  QMenu *mRobotRangeFinderMenu;
  QMenu *mRobotDisplayMenu;
  QAction *mToggleFullScreenAction;
  QAction *mExitFullScreenAction;
  QProgressDialog *mWorldLoadingProgressDialog;
  QTimer *mAnimationRecordingTimer;
  bool mIsFullScreenLocked;
  bool mWorldIsBeingDeleted;

  void createMainTools();
  void createMenus();

  QMenu *createFileMenu();
  QMenu *createEditMenu();
  QMenu *createViewMenu();
  QMenu *createSimulationMenu();
  QMenu *createBuildMenu();
  QMenu *createOverlayMenu();
  QMenu *createToolsMenu();
  QMenu *createWizardsMenu();
  QMenu *createHelpMenu();
  bool proposeToSaveWorld(bool reloading = false);
  QString findHtmlFileName(const char *title);
  void enableToolsWidgetItems(bool enabled);
  void updateWindowTitle();
  void updateGui();
  void updateSimulationMenu();
  void writePreferences() const;
  void showDocument(const QString &url);
  bool runSimulationHasRunWarningMessage();
  void logActiveControllersTermination();
  void addDock(QWidget *);

  // maximized/minimize dock widgets
  QList<QWidget *> mDockWidgets;
  QWidget *mMaximizedWidget;
  QByteArray mMinimizedDockState;

  // temporarily save devices perspective during PROTO template regeneration
  QHash<QString, QStringList> mTemporaryProtoPerspectives;

  // QSS properties
  QString mEnabledIconPath, mDisabledIconPath, mCoreIconPath, mToolBarAlign;

  WbStreamingServer *mStreamingServer;

private slots:
  void showOnlineDocumentation(const QString &book, const QString &page = "index");
  void updateProjectPath(const QString &oldPath, const QString &newPath);
  void simulationQuit(int exitStatus);
  void openFileInTextEditor(const QString &);

  void maximizeDock();
  void minimizeDock();
  void setWidgetMaximized(QWidget *widget, bool maximized);
  void removeHtmlRobotWindow(WbNode *node);
  void handleNewRobotInsertion(WbRobot *robot);

  void toggleFullScreen(bool enabled);
  void exitFullScreen();

  void openNewConsole(const QString &name = QString("Console"));
  void handleConsoleClosure();

  void openUrl(const QString &fileName, const QString &message, const QString &title);

  void prepareNodeRegeneration(WbNode *node);
  void discardNodeRegeneration() { finalizeNodeRegeneration(NULL); }
  void finalizeNodeRegeneration(WbNode *node);
};

#endif
