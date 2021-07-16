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

#include "WbGuiApplication.hpp"

#include "WbApplication.hpp"
#include "WbApplicationInfo.hpp"
#include "WbConsole.hpp"
#include "WbMainWindow.hpp"
#include "WbMessageBox.hpp"
#include "WbMultimediaStreamingServer.hpp"
#include "WbNewVersionDialog.hpp"
#include "WbPerformanceLog.hpp"
#include "WbPreferences.hpp"
#include "WbProject.hpp"
#include "WbSingleTaskApplication.hpp"
#include "WbSplashScreen.hpp"
#include "WbStandardPaths.hpp"
#include "WbSysInfo.hpp"
#include "WbTranslator.hpp"
#include "WbVersion.hpp"
#include "WbWorld.hpp"
#include "WbWrenOpenGlContext.hpp"
#include "WbX3dStreamingServer.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtGui/QFontDatabase>
#include <QtGui/QScreen>

#ifdef __APPLE__
#include <QtGui/QFileOpenEvent>
#endif

#include <iostream>

using namespace std;

// QApplication needs the reference to the original argc directly to run properly.
// Otherwise, bugs related with threads (splashscreen, tooltips, ...) can appear.
// This was observed on Linux 64
// cf:
// - http://lists-archives.org/kde-devel/20232-qt-4-5-related-crash-on-kdm-startup.html
// - http://www.qtcentre.org/archive/index.php/t-28785.html
WbGuiApplication::WbGuiApplication(int &argc, char **argv) :
  QApplication(argc, argv),
  mMainWindow(NULL),
  mTask(NORMAL),
  mStreamingServer(NULL) {
  setApplicationName("Webots");
  setApplicationVersion(WbApplicationInfo::version().toString(true, false, true));
  setOrganizationName("Cyberbotics");
  setOrganizationDomain("cyberbotics.com");
#ifdef _WIN32
  QProcess process;
  process.start("cygpath", QStringList{QString("-w"), QString("/")});
  process.waitForFinished(-1);
  QString MSYS2_HOME = process.readAllStandardOutput().trimmed();
  MSYS2_HOME.chop(1);                          // remove final backslash
  qputenv("MSYS2_HOME", MSYS2_HOME.toUtf8());  // useful to Python 3.8 controllers
  const QString webotsQtPlugins = MSYS2_HOME.replace('\\', '/') + "/mingw64/share/qt5/plugins";
  QCoreApplication::setLibraryPaths(QStringList(webotsQtPlugins));
  QApplication::setStyle("windowsvista");
#endif

  mApplication = new WbApplication();  // creates WbApplication singleton
  connect(mApplication, &WbApplication::createWorldLoadingProgressDialog, this, &WbGuiApplication::closeSplashScreenIfNeeded);
  // translation settings is the first thing to do in order
  // to have all the messages in the right language
  // (even the first failure messages)
  // most recently installed translation file is searched for translations first
  installTranslator(WbTranslator::instance()->basicTranslator());
  installTranslator(WbTranslator::instance()->translator());

  QDir::addSearchPath("icons", WbStandardPaths::resourcesPath() + "nodes/icons");
  QDir::addSearchPath("images", WbStandardPaths::resourcesPath() + "images");

  QFontDatabase::addApplicationFont(WbStandardPaths::fontsPath() + "Raleway-Light.ttf");

  // setup the stylesheet for the application
  udpateStyleSheet();

  // Qt has its own arguments, see Qt doc
  mShouldMinimize = false;
  mShouldStartFullscreen = false;
  mStartupMode = WbSimulationState::NONE;
  mShouldDoRendering = true;

  parseArguments();
}

WbGuiApplication::~WbGuiApplication() {
  delete mMainWindow;
}

void WbGuiApplication::restart() {
  if (mMainWindow)
    mMainWindow->close();
  else
    qApp->quit();
  QStringList nonProgramArgs = qApp->arguments();
  nonProgramArgs.removeFirst();
#ifdef __linux__
  QProcess::startDetached("./webots", nonProgramArgs);
#elif defined(_WIN32)
  exit(3030);  // this special code tells the launcher to restart Webots, see launcher.c
#else  // macOS
  QProcess::startDetached(qApp->arguments()[0], nonProgramArgs);
#endif
}

void WbGuiApplication::parseStreamArguments(const QString &streamArguments) {
  bool monitorActivity = false;
  bool disableTextStreams = false;
  bool ssl = false;
  bool controllerEdit = false;
  int port = 1234;
  QString mode = "x3d";

  const QStringList &options = streamArguments.split(';', Qt::SkipEmptyParts);
  foreach (QString option, options) {
    option = option.trimmed();
    const QRegExp rx("(\\w+)\\s*=\\s*([A-Za-z0-9:/.\\-,]+)?");
    rx.indexIn(option);
    const QStringList &capture = rx.capturedTexts();
    // "key" without value case
    if (option == "monitorActivity")
      monitorActivity = true;
    else if (option == "disableTextStreams")
      disableTextStreams = true;
    else if (option == "ssl")
      ssl = true;
    else if (option == "controllerEdit")
      controllerEdit = true;
    else if (capture.size() == 3) {
      const QString &key = capture[1];
      const QString &value = capture[2];
      if (key == "port") {
        bool ok;
        const int tmpPort = value.toInt(&ok);
        if (ok)
          port = tmpPort;
        else {
          cout << tr("webots: invalid 'port' option: '%1' in --stream").arg(value).toUtf8().constData() << endl;
          cout << tr("webots: stream port has to be integer").toUtf8().constData() << endl;
          mTask = FAILURE;
        }
      } else if (key == "mode") {
        if (value != "x3d" && value != "mjpeg") {
          cout << tr("webots: invalid 'mode' option: '%1' in --stream").arg(value).toUtf8().constData() << endl;
          cout << tr("webots: stream mode can only be x3d or mjpeg").toUtf8().constData() << endl;
          mTask = FAILURE;
        } else if (value == "mjpeg")
          mode = "mjpeg";
      } else {
        cout << tr("webots: unknown option: '%1' in --stream").arg(option).toUtf8().constData() << endl;
        mTask = FAILURE;
      }
    } else {
      cout << tr("webots: unknown option: '%1' in --stream").arg(option).toUtf8().constData() << endl;
      mTask = FAILURE;
    }
  }
  if (mTask == FAILURE) {
    cout << tr("Try 'webots --help' for more information.").toUtf8().constData() << endl;
    return;
  }
  if (mode == "mjpeg") {
    mStreamingServer = new WbMultimediaStreamingServer(monitorActivity, disableTextStreams, ssl, controllerEdit);
    mStreamingServer->start(port);
    return;
  }
  mStreamingServer = new WbX3dStreamingServer(monitorActivity, disableTextStreams, ssl, controllerEdit);
  mStreamingServer->start(port);
  WbWorld::enableX3DStreaming();
}

void WbGuiApplication::parseArguments() {
  // faster when copied according to Qt's doc
  QStringList args = arguments();
  bool logPerformanceMode = false;
  bool batch = false, stream = false;

  const int size = args.size();
  for (int i = 1; i < size; ++i) {
    const QString &arg = args[i];
    if (arg == "--minimize")
      mShouldMinimize = true;
    else if (arg == "--fullscreen")
      mShouldStartFullscreen = true;
    else if (arg == "--mode=stop") {
      cout << tr("The '--mode=stop' option is deprecated. Please use '--mode=pause' instead.").toUtf8().constData() << endl;
      mStartupMode = WbSimulationState::PAUSE;
    } else if (arg == "--mode=pause")
      mStartupMode = WbSimulationState::PAUSE;
    else if (arg == "--mode=realtime")
      mStartupMode = WbSimulationState::REALTIME;
    else if (arg == "--mode=fast")
      mStartupMode = WbSimulationState::FAST;
    else if (arg == "--mode=run") {
      cout << "Warning: `run` mode is deprecated, falling back to `fast` mode" << endl;
      mStartupMode = WbSimulationState::FAST;
    } else if (arg == "--no-rendering")
      mShouldDoRendering = false;
    else if (arg == "convert") {
      mTask = CONVERT;
      mTaskArguments = args.mid(i);
      break;
    } else if (arg == "--help")
      mTask = HELP;
    else if (arg == "--sysinfo")
      mTask = SYSINFO;
    else if (arg == "--version")
      mTask = VERSION;
    else if (arg == "--batch") {
      batch = true;
      WbMessageBox::disable();
    } else if (arg.startsWith("--update-proto-cache")) {
      QStringList items = arg.split('=');
      if (items.size() > 1)
        mTaskArguments.append(items[1]);
      else
        mTaskArguments.clear();
      mTask = UPDATE_PROTO_CACHE;
    } else if (arg.startsWith("--update-world"))
      mTask = UPDATE_WORLD;
    else if (arg == "--enable-x3d-meta-file-export")
      WbWorld::enableX3DMetaFileExport();
    else if (arg.startsWith("--stream")) {
      stream = true;
      QString serverArgument;
      int equalCharacterIndex = arg.indexOf('=');
      if (equalCharacterIndex != -1) {
        serverArgument = arg.mid(equalCharacterIndex + 1);
        // remove starting/trailing double quotes
        if (serverArgument.startsWith('"'))
          serverArgument = serverArgument.right(serverArgument.size() - 1);
        if (serverArgument.endsWith('"'))
          serverArgument = serverArgument.left(serverArgument.size() - 1);
      }
      parseStreamArguments(serverArgument);
    } else if (arg == "--stdout")
      WbConsole::enableStdOutRedirectToTerminal();
    else if (arg == "--stderr")
      WbConsole::enableStdErrRedirectToTerminal();
    else if (arg.startsWith("--log-performance")) {
      int equalCharacterIndex = arg.indexOf('=');
      if (equalCharacterIndex != -1) {
        QString logArgument = arg.mid(equalCharacterIndex + 1);
        // remove starting/trailing double quotes
        if (logArgument.startsWith('"'))
          logArgument = logArgument.right(logArgument.size() - 1);
        if (logArgument.endsWith('"'))
          logArgument = logArgument.left(logArgument.size() - 1);

        if (logArgument.contains(",")) {
          QStringList argumentsList = logArgument.split(",");
          WbPerformanceLog::createInstance(argumentsList[0], argumentsList[1].trimmed().toInt());
        } else
          WbPerformanceLog::createInstance(logArgument);

        logPerformanceMode = true;
      } else
        cout << tr("webots: invalid option : '--log-performance': log file path is missing.").toUtf8().constData() << endl;
    }
#ifndef _WIN32
    else if (arg == "--disable-gpu" || arg == "--disable-logging" || arg == "--enable-logging" ||
             arg.startsWith("--log-level=") || arg == "--no-sandbox" || arg == "--single-process" ||
             arg.startsWith("--remote-debugging-port=")) {
      // Silently ignore the awesome QWebEngine debugging tools:
      // cf. https://doc.qt.io/qt-5/qtwebengine-debugging.html
    }
#endif
    else if (arg.startsWith("-")) {
      cout << tr("webots: invalid option: '%1'").arg(arg).toUtf8().constData() << endl;
      cout << tr("Try 'webots --help' for more information.").toUtf8().constData() << endl;
      mTask = FAILURE;
    } else {
      if (mStartWorldName.isEmpty())
        mStartWorldName = QDir::fromNativeSeparators(arg);
      else {
        cout << tr("webots: too many arguments.").toUtf8().constData() << endl;
        cout << tr("Try 'webots --help' for more information.").toUtf8().constData() << endl;
        mTask = FAILURE;
      }
    }
  }

  if (stream && !batch)
    cout << "Warning: you should also use --batch (in addition to --stream) for production." << endl;

  if (logPerformanceMode) {
    WbPerformanceLog::enableSystemInfoLog(mTask == SYSINFO);
    mTask = NORMAL;
  }

  if (!qgetenv("WEBOTS_SAFE_MODE").isEmpty()) {
    WbPreferences::instance()->setValue("OpenGL/disableShadows", true);
    WbPreferences::instance()->setValue("OpenGL/disableAntiAliasing", true);
    WbPreferences::instance()->setValue("OpenGL/GTAO", 0);
    WbPreferences::instance()->setValue("OpenGL/textureQuality", 0);
    WbPreferences::instance()->setValue("OpenGL/textureFiltering", 0);
    mStartupMode = WbSimulationState::PAUSE;
    mStartWorldName = WbStandardPaths::resourcesPath() + "projects/worlds/empty.wbt";
  }
}

int WbGuiApplication::exec() {
  if (mTask == NORMAL || mTask == UPDATE_WORLD) {
    if (setup()) {
      QApplication::processEvents();
      loadInitialWorld();
    }
  }

  WbSingleTaskApplication *task = NULL;
  if (mTask != NORMAL) {
    task = new WbSingleTaskApplication(mTask, mTaskArguments, this, mApplication->startupPath());
    if (mMainWindow)
      connect(task, &WbSingleTaskApplication::finished, mMainWindow, &WbMainWindow::close);
    else
      connect(task, &WbSingleTaskApplication::finished, this, &QApplication::exit);
    // run the task from the application event loop
    QTimer::singleShot(0, task, SLOT(run()));
  }

  int ret = QApplication::exec();
  delete task;
  return ret;
}

bool WbGuiApplication::setup() {
  WbPreferences *const prefs = WbPreferences::instance();
  if (mStartupMode == WbSimulationState::NONE)
    mStartupMode = startupModeFromPreferences();
  if (mShouldDoRendering)
    mShouldDoRendering = renderingFromPreferences();

  WbSimulationState::instance()->setMode(mStartupMode);
  WbSimulationState::instance()->setRendering(mShouldDoRendering);

  // check specified world file if any
  if (!mStartWorldName.isEmpty()) {
    // if relative, make absolute
    if (QDir::isRelativePath(mStartWorldName))
      mStartWorldName = mApplication->startupPath() + '/' + mStartWorldName;

    QFileInfo info(mStartWorldName);
    if (!info.isReadable()) {
      cerr << tr("Could not open file: '%1'.").arg(mStartWorldName).toUtf8().constData() << endl;
      mTask = FAILURE;
    }
  }

  if (WbMessageBox::enabled() &&
      (!WbPreferences::instance()->contains("General/theme") || !WbPreferences::instance()->contains("General/telemetry"))) {
    if (WbNewVersionDialog::run() != QDialog::Accepted) {
      mTask = QUIT;
      return false;
    } else if (WbPreferences::instance()->value("General/theme").toString() != "webots_classic.qss")
      udpateStyleSheet();
  }

  // Show guided tour if first ever launch and no command line world argument is given
  bool showGuidedTour =
    prefs->value("Internal/firstLaunch", true).toBool() && mStartWorldName.isEmpty() && WbMessageBox::enabled();

#ifndef _WIN32
  // create main window on Linux and macOS before the splash screen otherwise, the
  // image in the splash screen is empty...
  // Doing the same on Windows slows down the popup of the SplashScreen, therefore
  // the main window is created later on Windows.
  mMainWindow = new WbMainWindow(mShouldMinimize, mStreamingServer);
#endif

  if (!mShouldMinimize) {
    // splash screen
    // Warning: using heap allocated splash screen and/or pixmap cause crash while
    // showing tooltips in the main window under Linux.
    const QDir screenshotLocation = QDir("images:splash_images/", "*.jpg");
    const QString webotsLogoName("webots.png");
    mSplash = new WbSplashScreen(screenshotLocation.entryList(), webotsLogoName);
    if (WbPreferences::instance()->value("MainWindow/maximized", false).toBool()) {
      // we need to center the splash screen on the same window as the mainWindow,
      // which is positioned wherever the mouse is on launch
      const QScreen *mainWindowScreen = QGuiApplication::screenAt(QCursor::pos());
      const QRect mainWindowScreenRect = mainWindowScreen->geometry();
      QPoint targetPosition = mainWindowScreenRect.center();
      targetPosition.setX(targetPosition.x() - mSplash->width() / 2);
      targetPosition.setY(targetPosition.y() - mSplash->height() / 2);
      mSplash->move(targetPosition);
    }

    // now we can safely show the splash screen, knowing it will be in the right place
    mSplash->show();
#ifdef __APPLE__
    // On macOS, when the WbSplashScreen is shown, Qt calls a resize event on the QMainWindow (not shown yet) with the size of
    // the splash screen. This overrides the WbMainWindow size preferences. This sounds like a Qt bug.
    mMainWindow->restorePreferredGeometry(mShouldMinimize);
#endif
    connect(WbLog::instance(), &WbLog::popupOpen, mSplash, &QSplashScreen::hide);
    connect(WbLog::instance(), &WbLog::popupClosed, mSplash, &QSplashScreen::show);
    processEvents();
    setSplashMessage(tr("Starting up..."));
  } else
    mSplash = NULL;
  // otherwise get it from the list of recent files
  if (mStartWorldName.isEmpty() || showGuidedTour) {
    const QString defaultFileName = WbStandardPaths::emptyProjectPath() + "worlds/" + WbProject::newWorldFileName();
    mStartWorldName = prefs->value("RecentFiles/file0", defaultFileName).toString();
  }
  setSplashMessage(tr("Loading world..."));

#ifdef __APPLE__
  /**
   * Fixed the floating docks which are not rendered (gray panel)
   * This is a know issue between Qt 5.0.0 and 5.0.2
   * Hopefully this can be removed later.
   * cf: https://bugreports.qt-project.org/browse/QTBUG-30655
   **/
  setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
#endif

  /**
   * Hopefully improved the icon resolution
   * http://blog.qt.digia.com/blog/2013/04/25/retina-display-support-for-mac-os-ios-and-x11/
   **/
  setAttribute(Qt::AA_UseHighDpiPixmaps);

#ifdef _WIN32
  // create main window
  mMainWindow = new WbMainWindow(mShouldMinimize, mStreamingServer);
#endif

  if (mShouldMinimize)
    mMainWindow->showMinimized();
  else {
    if (prefs->value("MainWindow/maximized", false).toBool())
      mMainWindow->showMaximized();
    else
      mMainWindow->showNormal();
  }

  connect(mMainWindow, &WbMainWindow::restartRequested, this, &WbGuiApplication::restart);
  connect(mMainWindow, &WbMainWindow::splashScreenCloseRequested, this, &WbGuiApplication::closeSplashScreenIfNeeded);
  mApplication->setup();

#ifdef __linux__
  // popup a warning message if the preferences file is not writable
  prefs->checkIsWritable();
  if (WbSysInfo::isRootUser())
    WbLog::warning("It is not recommended to run Webots as root.");

  if (prefs->value("Internal/firstLaunch", true).toBool()) {
    // delete previous desktop application info file in order to update it to the current installation data
    const QString desktopFilePath = QDir::homePath() + "/.local/share/applications/webots.desktop";
    if (QFile::exists(desktopFilePath))
      QFile::remove(desktopFilePath);
  }
#endif

  WbWrenOpenGlContext::makeWrenCurrent();
  WbSysInfo::initializeOpenGlInfo();
  WbWrenOpenGlContext::doneWren();

  if (showGuidedTour)
    mMainWindow->showGuidedTour();

  return true;
}

WbSimulationState::Mode WbGuiApplication::startupModeFromPreferences() const {
  WbPreferences *const prefs = WbPreferences::instance();
  const QString startupMode(prefs->value("General/startupMode").toString());

  if (startupMode == "Real-time")
    return WbSimulationState::REALTIME;
  if (startupMode == "Fast")
    return WbSimulationState::FAST;
  return WbSimulationState::PAUSE;
}

bool WbGuiApplication::renderingFromPreferences() const {
  WbPreferences *const prefs = WbPreferences::instance();
  return prefs->value("General/rendering", true).toBool();
}

#ifdef __APPLE__
// Fixed the Webots opening by double-clicking on .wbt file in the Finder
bool WbGuiApplication::event(QEvent *event) {
  switch (event->type()) {
    case QEvent::FileOpen:
      mStartWorldName = static_cast<QFileOpenEvent *>(event)->file();
      return true;
    default:
      return QApplication::event(event);
  }
}
#endif

void WbGuiApplication::setSplashMessage(const QString &message) {
  if (!mSplash)
    return;

  QString copyright = tr("Copyright \u00A9 1998 - %1 Cyberbotics Ltd.").arg(WbApplicationInfo::version().majorNumber());
  mSplash->setLiveMessage(copyright + "\n" + message);
  mSplash->repaint();
}

void WbGuiApplication::closeSplashScreenIfNeeded() {
  if (mSplash) {
    mSplash->finish(mMainWindow);
    delete mSplash;
    mSplash = NULL;
  }
}

void WbGuiApplication::loadInitialWorld() {
  if (!mMainWindow->loadWorld(mStartWorldName))
    // this file should always exists
    mMainWindow->loadWorld(WbStandardPaths::emptyProjectPath() + "worlds/" + WbProject::newWorldFileName());

  if (!mShouldMinimize && mShouldStartFullscreen)
    mMainWindow->setFullScreen(true, false, false, true);
}

void WbGuiApplication::udpateStyleSheet() {
  QString themeToLoad = WbPreferences::instance()->value("General/theme", "webots_classic.qss").toString();
  QFile qssFile(WbStandardPaths::resourcesPath() + themeToLoad);
  qssFile.open(QFile::ReadOnly);
  QString styleSheet = QString::fromUtf8(qssFile.readAll());

#ifdef __APPLE__
  QFile macOSQssFile(WbStandardPaths::resourcesPath() + "stylesheet.macos.qss");
  macOSQssFile.open(QFile::ReadOnly);
  styleSheet += QString::fromUtf8(macOSQssFile.readAll());

#elif defined(__linux__)
  QFile linuxQssFile(WbStandardPaths::resourcesPath() + "stylesheet.linux.qss");
  linuxQssFile.open(QFile::ReadOnly);
  styleSheet += QString::fromUtf8(linuxQssFile.readAll());

#elif _WIN32
  QFile windowsQssFile(WbStandardPaths::resourcesPath() + "stylesheet.windows.qss");
  windowsQssFile.open(QFile::ReadOnly);
  styleSheet += QString::fromUtf8(windowsQssFile.readAll());
#endif

  qApp->setStyleSheet(styleSheet);
}
