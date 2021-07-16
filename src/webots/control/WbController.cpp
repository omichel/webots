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

#include "WbController.hpp"

#include "WbApplicationInfo.hpp"
#include "WbBinaryIncubator.hpp"
#include "WbControlledWorld.hpp"
#include "WbFileUtil.hpp"
#include "WbIniParser.hpp"
#include "WbLanguage.hpp"
#include "WbLanguageTools.hpp"
#include "WbLog.hpp"
#include "WbPerformanceLog.hpp"
#include "WbPreferences.hpp"
#include "WbProject.hpp"
#include "WbProtoList.hpp"
#include "WbProtoModel.hpp"
#include "WbRobot.hpp"
#include "WbSimulationState.hpp"
#include "WbStandardPaths.hpp"
#include "WbSysInfo.hpp"
#include "WbTemplateManager.hpp"
#include "WbVersion.hpp"

#include <QtCore/QByteArray>
#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QtCore/QProcessEnvironment>
#include <QtNetwork/QLocalSocket>
#include <cassert>
#include "../../controller/c/messages.h"

#ifdef _WIN32
#include <windows.h>
#endif

/*
#include <iomanip>
#include <iostream>

static const int maxNumberOfByteDisplayed = 100;

static void printArray(const QByteArray &buffer, const QString &prefix, int id, bool hex, bool ascii) {
  int bufferSize = buffer.size();
  std::cout << prefix.toUtf8().constData() << " at time " << WbSimulationState::instance()->time();
  std::cout << " (id = " << id << ", size = " << bufferSize << ")" << std::endl;
  int numberOfByteDisplayed = bufferSize < maxNumberOfByteDisplayed ? bufferSize : maxNumberOfByteDisplayed;
  if (hex) {
    std::cout << " (Hex) : \"";
    for (int i = 0; i < numberOfByteDisplayed; i++) {
      std::cout << std::uppercase << std::setfill('0') << std::setw(2) << std::hex;
      std::cout << (int)(buffer[i] & 0xFF);
      std::cout << std::nouppercase << std::setfill(' ') << std::setw(0) << std::dec;
      std::cout << ';';
    }
    if (bufferSize > numberOfByteDisplayed)
      std::cout << "...";
    std::cout << "\"" << std::endl;
  }
  if (ascii) {
    std::cout << " (ASCII) : \"";
    for (int i = 0; i < numberOfByteDisplayed; i++) {
      char c = (int)(buffer[i] & 0xFF);
      if (c >= ' ' && c <= '~')
        std::cout << c;
      else
        std::cout << '?';
    }
    if (bufferSize > numberOfByteDisplayed)
      std::cout << "...";
    std::cout << "\"" << std::endl;
  }
}
*/

WbController::WbController(WbRobot *robot) :
  mStdout(QTextCodec::codecForName("UTF-8")),
  mStderr(QTextCodec::codecForName("UTF-8")),
  mHasPendingImmediateAnswer(false) {
  mRobot = robot;
  mRobot->setConfigureRequest(true);
  mControllerPath = mRobot->controllerDir();
  updateName(mRobot->controllerName());
  connect(mRobot, &WbRobot::appendMessageToConsole, this, &WbController::appendMessageToConsole);
  connect(mRobot, &WbRobot::destroyed, this, &WbController::robotDestroyed);

  mType = WbFileUtil::UNKNOWN;
  mSocket = NULL;
  mRequestTime = 0.0;
  mDeltaTimeRequested = 0;
  mDeltaTimeMeasured = 0.0;
  mHasBeenTerminatedByItself = false;
  mIncompleteRequest = false;
  mRequestPending = false;
  mProcessingRequest = false;
  mStdoutNeedsFlush = false;
  mStderrNeedsFlush = false;

  mProcess = new QProcess();

  connect(mRobot, &WbRobot::controllerExited, this, &WbController::handleControllerExit);
  connect(mProcess, &QProcess::readyReadStandardOutput, this, &WbController::readStdout);
  connect(mProcess, &QProcess::readyReadStandardError, this, &WbController::readStderr);
  connect(mProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
  connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
}

WbController::~WbController() {
  // disconnect everything in order to make sure
  // that this function is the last one to be called
  // exception: don't disconnect readyReadStandard*()
  // signals in order to see the latest log messages
  if (mRobot)
    disconnect(mRobot, &WbRobot::controllerExited, this, &WbController::handleControllerExit);
  mProcess->disconnect(SIGNAL(finished(int, QProcess::ExitStatus)));
  mProcess->disconnect(SIGNAL(error(QProcess::ProcessError)));

  if (mSocket) {
    mSocket->disconnect();
    // eat the latest messages from the controller
    if (!mRequestPending && mSocket->isValid()) {
      mSocket->waitForReadyRead(1000);
      mSocket->readAll();
    }

    // send the termination packet
    if (mHasBeenTerminatedByItself)
      mHasBeenTerminatedByItself = false;
    else {
      const int size = 2 * sizeof(int) + sizeof(WbDeviceTag) + sizeof(unsigned char);
      QByteArray buffer;
      QDataStream stream(&buffer, QIODevice::WriteOnly);
      stream.setByteOrder(QDataStream::LittleEndian);
      stream << size;               // size, to be overwritten afterwards
      stream << (int)0;             // time stamp, ignored
      stream << (unsigned short)0;  // tag of the root device
      stream << (unsigned char)C_ROBOT_QUIT;
      assert(size == buffer.size());
      if (mSocket->isValid()) {
        mSocket->write(buffer.constData(), size);
        mSocket->flush();  // otherwise the temination packet is not sent
      }
      // kill the process
      if (mProcess->state() != QProcess::NotRunning && !mProcess->waitForFinished(1000)) {
        WbLog::warning(tr("%1: Forced termination (because process didn't terminate itself after 1 second).").arg(name()));
#ifdef _WIN32
        // on Windows, we need to kill the process as it may not handle the WM_CLOSE message sent by terminate()
        mProcess->kill();
#else
        mProcess->terminate();  // on Linux and macOS, we assume the controller will quit on receiving the SIGTERM signal
#endif
      }
    }
  } else if (mProcess && mProcess->state() != QProcess::NotRunning)
    mProcess->terminate();

  delete mSocket;
  delete mProcess;
}

void WbController::updateName(const QString &name) {
  mName = name;
}

void WbController::resetRequestTime() {
  mRequestTime = WbSimulationState::instance()->time();
}

bool WbController::isRunning() const {
  return mRobot->isControllerStarted() && !mHasBeenTerminatedByItself;
}

// the start() method  never fails: if the controller name is invalid, then the void controller starts instead.
void WbController::start() {
  mRobot->setControllerStarted(true);

  if (mControllerPath.isEmpty()) {
    warn(tr("Could not find the controller directory.\nStarts the void controller instead."));
    startVoidExecutable();
  }

  mType = findType(mControllerPath);
  setProcessEnvironment();
// on Windows, java is unable to find class in a path including UTF-8 characters (e.g., Chinese)
#ifdef _WIN32
  if ((mType == WbFileUtil::CLASS || mType == WbFileUtil::JAR) &&
      QString(mControllerPath.toUtf8()) != QString::fromLocal8Bit(mControllerPath.toLocal8Bit()))
    WbLog::warning(tr("\'%1\'\nThe path to this Webots project contains non 8-bit characters. "
                      "Webots won't be able to execute any Java controller in this path. "
                      "Please move this Webots project into a folder with only 8-bit characters.")
                     .arg(mControllerPath));
#endif
  switch (mType) {
    case WbFileUtil::EXECUTABLE:
      startExecutable();
      break;
    case WbFileUtil::CLASS:
      startJava();
      break;
    case WbFileUtil::JAR:
      startJava(true);
      break;
    case WbFileUtil::PYTHON:
      startPython();
      break;
    case WbFileUtil::MATLAB:
      startMatlab();
      break;
    case WbFileUtil::BOTSTUDIO:
      startBotstudio();
      break;
    default:
      reportControllerNotFound();
      startVoidExecutable();
      mType = WbFileUtil::EXECUTABLE;
  }
  if (mCommand.isEmpty())  // python has wrong version or Matlab 64 not available
    return;

  info(tr("Starting controller: %1").arg(commandLine()));

#ifdef __linux__
  if (!qgetenv("WEBOTS_FIREJAIL_CONTROLLERS").isEmpty() && mRobot->findField("controller")) {
    mArguments.prepend(mCommand);
    mCommand = "firejail";
    QStringList firejailArguments;
    firejailArguments << "--net=none"
                      << "--nosound"
                      << "--shell=none"
                      << "--quiet";
    // adding a path starting with /tmp/ in a whitelist blocks all other paths starting
    // with /tmp/ (including the local socket used by Webots which would prevent the
    // controller from running)
    if (!mControllerPath.startsWith("/tmp/"))
      firejailArguments << "--whitelist=" + mControllerPath;
    firejailArguments << "--whitelist=" + WbStandardPaths::controllerLibPath();
    firejailArguments << "--read-only=" + WbStandardPaths::controllerLibPath();

    QString ldLibraryPath = WbStandardPaths::controllerLibPath();
    ldLibraryPath.chop(1);

    // extract the controller resources from runtime.ini and add them in the firejail whitelist
    // the runtime.ini itself has to be put in the blacklist of firejail
    const QString &runtimeFilePath = mControllerPath + "runtime.ini";
    if (QFile::exists(runtimeFilePath)) {
      WbIniParser iniParser(runtimeFilePath);
      if (!iniParser.isValid())
        warn(tr("Environment variables from runtime.ini could not be loaded: the file contains illegal definitions."));
      else {
        QProcessEnvironment env = mProcess->processEnvironment();
        for (int i = 0; i < iniParser.size(); ++i) {
          if (iniParser.sectionAt(i) != "environment variables with relative paths" &&
              iniParser.sectionAt(i) != "environment variables with paths" &&
              iniParser.sectionAt(i) != "environment variables for linux" &&
              iniParser.sectionAt(i) != "environment variables for linux 64")
            continue;

          if (iniParser.keyAt(i) == ("WEBOTS_LIBRARY_PATH") || iniParser.keyAt(i) == ("FIREJAIL_PATH")) {
            const QStringList pathsList = iniParser.resolvedValueAt(i, env).split(":");
            foreach (QString path, pathsList) {
              firejailArguments << "--whitelist=" + path;
              firejailArguments << "--read-only=" + path;
              ldLibraryPath += ":" + path;
            }
          } else
            // the variable could be used to define WEBOTS_LIBRARY_PATH or FIREJAIL_PATH
            addToPathEnvironmentVariable(env, iniParser.keyAt(i), iniParser.resolvedValueAt(i, env), true);
        }
        mProcess->setProcessEnvironment(env);
      }
    } else {
      // create runtime.ini file so that it is included it in the blacklist
      // otherwise it can be created and modified from the robot controller
      QFile file(runtimeFilePath);
      if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << '\n';
      } else {
        warn(tr("Could not create the runtime.ini file."));
        return;
      }
    }
    firejailArguments << "--blacklist=" + runtimeFilePath << "--env=LD_LIBRARY_PATH=" + ldLibraryPath;
    mArguments = firejailArguments + mArguments;
  }
#endif

  // for matlab controllers we must change to the lib/matlab directory
  // other controller types are executed in the controller dir
  if (mType == WbFileUtil::MATLAB)
    mProcess->setWorkingDirectory(WbStandardPaths::controllerLibPath() + "matlab");
  else
    mProcess->setWorkingDirectory(mControllerPath);

  mProcess->start(mCommand, mArguments);
}

void WbController::addToPathEnvironmentVariable(QProcessEnvironment &env, const QString &key, const QString &value,
                                                bool override, bool shouldPrepend) {
  const QString nativeValue(QDir::toNativeSeparators(value));
  if (!env.contains(key) || override) {  // key is the name of the environment variable
    env.insert(key, nativeValue);
    return;
  }
  const QString &previousValue = env.value(key);
  if (!previousValue.split(QDir::listSeparator()).contains(nativeValue)) {
    if (shouldPrepend)
      env.insert(key, nativeValue + QDir::listSeparator() + previousValue);
    else
      env.insert(key, previousValue + QDir::listSeparator() + nativeValue);
  }
}

bool WbController::removeFromPathEnvironmentVariable(QProcessEnvironment &env, const QString &key, const QString &value) {
  const QString path = env.value(key);
  QStringList paths = path.split(QDir::listSeparator());
#ifdef _WIN32
  Qt::CaseSensitivity cs = Qt::CaseInsensitive;
#else
  Qt::CaseSensitivity cs = Qt::CaseSensitive;
#endif
  if (!paths.contains(value, cs))
    return false;
  env.remove(key);
  paths.removeAll(QString(""));
  paths.removeDuplicates();
  QMutableStringListIterator i(paths);
  while (i.hasNext()) {
    if (i.next().compare(value, cs) == 0)
      i.remove();
  }
  env.insert(key, paths.join(';'));
  return true;
}

void WbController::setProcessEnvironment() {
#ifdef __linux__
  static const QString ldEnvironmentVariable("LD_LIBRARY_PATH");
#elif defined(__APPLE__)
  static const QString ldEnvironmentVariable("DYLD_LIBRARY_PATH");
#else  // _WIN32
  static const QString ldEnvironmentVariable("PATH");
#endif

  // starts from the parent process environment
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

  // store a unique robot ID for the controller
  env.insert("WEBOTS_ROBOT_ID", QString::number(mRobot->uniqueId()));

  // Add the Webots lib path to be able to load (at least) libController
  QString ldLibraryPath = WbStandardPaths::controllerLibPath();
  ldLibraryPath.chop(1);
  addToPathEnvironmentVariable(env, ldEnvironmentVariable, ldLibraryPath, false);
  // Remove paths needed by Webots only
#ifdef _WIN32
  const QString msys64 = QDir::toNativeSeparators(WbStandardPaths::webotsMsys64Path());
  removeFromPathEnvironmentVariable(env, ldEnvironmentVariable, msys64 + "mingw64\\bin");
  removeFromPathEnvironmentVariable(env, ldEnvironmentVariable, msys64 + "usr\\bin");
#else
  ldLibraryPath = WbStandardPaths::webotsLibPath();
  ldLibraryPath.chop(1);
  removeFromPathEnvironmentVariable(env, ldEnvironmentVariable, ldLibraryPath);
  // add the controller path in the PATH-like environment variable
  // in order to be able to add easily dynamic libraries there
  // Note: on windows, this is the default behavior
  ldLibraryPath = mControllerPath;
  ldLibraryPath.chop(1);
  addToPathEnvironmentVariable(env, ldEnvironmentVariable, ldLibraryPath, false, true);
#endif

  if (QFile::exists(mControllerPath + "runtime.ini")) {
    WbIniParser iniParser(mControllerPath + "runtime.ini");
    if (!iniParser.isValid())
      warn(tr("Environment variables from runtime.ini could not be loaded: the file contains illegal definitions."));
    else {
      for (int i = 0; i < iniParser.size(); ++i) {
        const QString &value = iniParser.resolvedValueAt(i, env);
        iniParser.setValue(i, value);
        if (iniParser.sectionAt(i) == "environment variables with relative paths")
          warn(
            "[environment variables with relative path] is deprecated, please use [environment variables with path] instead");
        if (iniParser.sectionAt(i) == "environment variables with relative paths" ||
            iniParser.sectionAt(i) == "environment variables with paths")
          addToPathEnvironmentVariable(env, iniParser.keyAt(i), iniParser.valueAt(i), true);
        if (iniParser.sectionAt(i) == "environment variables")
          env.insert(iniParser.keyAt(i), iniParser.valueAt(i));
        if (iniParser.sectionAt(i) == "java") {
          if (iniParser.keyAt(i) == "COMMAND")
            mJavaCommand = iniParser.valueAt(i);
          else if (iniParser.keyAt(i) == "OPTIONS")
            mJavaOptions = iniParser.valueAt(i);
          else
            WbLog::warning(tr("Unknown key: %1 in java section").arg(iniParser.keyAt(i)));
        }
        if (iniParser.sectionAt(i) == "python") {
          if (iniParser.keyAt(i) == "COMMAND")
            mPythonCommand = WbLanguageTools::pythonCommand(mPythonShortVersion, iniParser.valueAt(i), env);
          else if (iniParser.keyAt(i) == "OPTIONS")
            mPythonOptions = iniParser.valueAt(i);
          else
            WbLog::warning(tr("Unknown key: %1 in python section").arg(iniParser.keyAt(i)));
        }
        if (iniParser.sectionAt(i) == "matlab") {
          if (iniParser.keyAt(i) == "COMMAND")
            mMatlabCommand = iniParser.valueAt(i);
          else if (iniParser.keyAt(i) == "OPTIONS")
            mMatlabOptions = iniParser.valueAt(i);
          else
            WbLog::warning(tr("Unknown key: %1 in matlab section").arg(iniParser.keyAt(i)));
        }
#ifdef _WIN32
        if (iniParser.sectionAt(i) == "environment variables for windows")
          addToPathEnvironmentVariable(env, iniParser.keyAt(i), iniParser.valueAt(i), true);
#elif defined(__APPLE__)
        if (iniParser.sectionAt(i) == "environment variables for mac os x" ||
            iniParser.sectionAt(i) == "environment variables for macos")
          addToPathEnvironmentVariable(env, iniParser.keyAt(i), iniParser.valueAt(i), true);
#else
        if (iniParser.sectionAt(i) == "environment variables for linux")
          addToPathEnvironmentVariable(env, iniParser.keyAt(i), iniParser.valueAt(i), true);
        if (!WbSysInfo::isPointerSize64bits()) {
          if (iniParser.sectionAt(i) == "environment variables for linux 32")
            addToPathEnvironmentVariable(env, iniParser.keyAt(i), iniParser.valueAt(i), true);
        } else {
          if (iniParser.sectionAt(i) == "environment variables for linux 64")
            addToPathEnvironmentVariable(env, iniParser.keyAt(i), iniParser.valueAt(i), true);
        }
#endif
      }
    }
  }
  // WEBOTS_LIBRARY_PATH is an environment variable of Webots that users can edit to
  // prepend paths to the library path
  if (env.contains("WEBOTS_LIBRARY_PATH"))
    addToPathEnvironmentVariable(env, ldEnvironmentVariable, env.value("WEBOTS_LIBRARY_PATH"), false, true);

  // Add all the libraries subdirectories to the environment
  QStringList librariesSearchPaths;
  if (QFile::exists(WbProject::current()->librariesPath()))
    librariesSearchPaths << WbProject::current()->librariesPath();

  if (mRobot->isProtoInstance()) {
    // search in project folder associated with PROTO instance
    const QString protoLibrariesPath = mRobot->protoModelProjectPath() + "/libraries/";
    if (QDir(protoLibrariesPath).exists())
      librariesSearchPaths << protoLibrariesPath;
  }

  QList<WbNode *> nodes = mRobot->subNodes(true, true, true);
  for (int i = 0; i < nodes.size(); ++i) {
    if (nodes.at(i)->isProtoInstance()) {
      WbProtoModel *protoModel = nodes.at(i)->proto();
      do {
        if (!protoModel->projectPath().isEmpty()) {
          QDir protoProjectDir(protoModel->projectPath());
          const QString protoLibrariesPath = protoProjectDir.absolutePath() + "/libraries/";
          if (QDir(protoLibrariesPath).exists())
            librariesSearchPaths << protoLibrariesPath;
        }
        protoModel = WbProtoList::current()->findModel(protoModel->ancestorProtoName(), "");
      } while (protoModel);
    }
  }

  foreach (const QString &librariesSearchPath, librariesSearchPaths) {
    const QDir dir(librariesSearchPath);
    const QStringList subDirectories = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &subDirectory, subDirectories)
      addToPathEnvironmentVariable(env, ldEnvironmentVariable, librariesSearchPath + subDirectory, false, true);
  }
  if (mType == WbFileUtil::PYTHON) {
    if (mPythonCommand.isEmpty())
      mPythonCommand = WbLanguageTools::pythonCommand(
        mPythonShortVersion, WbPreferences::instance()->value("General/pythonCommand", "python").toString(), env);
    // read the python shebang (first line starting with #!) to possibly override the python command
    QFile pythonSourceFile(mControllerPath + name() + ".py");
    if (pythonSourceFile.open(QIODevice::ReadOnly)) {
      QTextStream in(&pythonSourceFile);
      const QString &line = in.readLine();
      if (line.startsWith("#!")) {
#ifndef _WIN32
        if (line.startsWith("#!/usr/bin/env "))
          mPythonCommand = WbLanguageTools::pythonCommand(mPythonShortVersion, line.mid(15).trimmed(), env);
        else
          mPythonCommand = WbLanguageTools::pythonCommand(mPythonShortVersion, line.mid(2).trimmed(), env);
#else  // Windows: check that the version specified in the shebang corresponds to the version of Python installed
        const QString &expectedVersion = line.mid(line.lastIndexOf("python", -1, Qt::CaseInsensitive) + 6);
        bool mismatch = false;
        int l = expectedVersion.length();
        if (l == 1 && expectedVersion[0] != mPythonShortVersion[0])
          mismatch = true;
        if (l >= 3 && (expectedVersion[0] != mPythonShortVersion[0] || expectedVersion[2] != mPythonShortVersion[1]))
          mismatch = true;
        if (mismatch)
          warn(tr("Python shebang requests python%1, but current path points to Python%2")
                 .arg(expectedVersion, mPythonShortVersion));
#endif
      }
      pythonSourceFile.close();
    }
#ifdef __APPLE__
    QProcess process;
    process.setProcessEnvironment(env);
    process.start(mPythonCommand, QStringList() << "-c"
                                                << "import sys; print(sys.exec_prefix);");
    process.waitForFinished();
    const QString output = process.readAll();
    if (output.startsWith("/usr/local/Cellar"))
      addToPathEnvironmentVariable(
        env, "PYTHONPATH", WbStandardPaths::controllerLibPath() + "python" + mPythonShortVersion + "_brew", false, true);
    else
      addToPathEnvironmentVariable(env, "PYTHONPATH", WbStandardPaths::controllerLibPath() + "python" + mPythonShortVersion,
                                   false, true);
#else
    addToPathEnvironmentVariable(env, "PYTHONPATH", WbStandardPaths::controllerLibPath() + "python" + mPythonShortVersion,
                                 false, true);
#endif
    env.insert("PYTHONIOENCODING", "UTF-8");
  } else if (mType == WbFileUtil::MATLAB) {
    // these variables are read by lib/matlab/launcher.m
    env.insert("WEBOTS_PROJECT", WbProject::current()->current()->path().toUtf8());
    env.insert("WEBOTS_CONTROLLER_NAME", name().toUtf8());
    env.insert("WEBOTS_VERSION", WbApplicationInfo::version().toString().toUtf8());
  }
  env.insert("WEBOTS_TMP_PATH", WbStandardPaths::webotsTmpPath());
  // qDebug() << "Environment:";
  // foreach (const QString &element, env)
  //  qDebug() << element;
  mProcess->setProcessEnvironment(env);
}

void WbController::info(const QString &message) {
  WbLog::info(name() + ": " + message);
}

void WbController::warn(const QString &message) {
  WbLog::warning(name() + ": " + message);
}

void WbController::error(const QString &message) {
  WbLog::error(name() + ": " + message);
}

void WbController::appendMessageToConsole(const QString &message, bool useStdout) {
  appendMessageToBuffer(message, useStdout ? &mStdoutBuffer : &mStderrBuffer);
}

void WbController::readStdout() {
  appendMessageToBuffer(mStdout.toUnicode(mProcess->readAllStandardOutput()), &mStdoutBuffer);
}

void WbController::readStderr() {
  appendMessageToBuffer(mStderr.toUnicode(mProcess->readAllStandardError()), &mStderrBuffer);
}

void WbController::appendMessageToBuffer(const QString &message, QString *buffer) {
#ifdef _WIN32
  // on windows replace CR+LF by LF
  QString text = message;
  text.replace("\r\n", "\n");
#else
  const QString &text = message;
#endif
  buffer->append(text);
  if (buffer == mStdoutBuffer)
    mStdoutNeedsFlush = true;
  else
    mStderrNeedsFlush = true;
}

void WbController::flushBuffer(QString *buffer) {
  // Split string into lines by detecting '\n', then send lines one by one to WbLog.
  // When several streams or several controllers are used, this prevents to mix unrelated lines
  int index = buffer->indexOf('\n');
  while (index != -1) {
    const QString line = buffer->mid(0, index + 1);
    if (buffer == mStdoutBuffer)
      WbLog::appendStdout(line, robot()->name());
    else
      WbLog::appendStderr(line, robot()->name());
    // remove line from buffer
    buffer->remove(0, index + 1);
    index = buffer->indexOf('\n');
  }
  if (buffer == mStdoutBuffer)
    mStdoutNeedsFlush = false;
  else
    mStderrNeedsFlush = false;
}

void WbController::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  mHasBeenTerminatedByItself = true;
  flushBuffers();
  switch (exitStatus) {
    case QProcess::NormalExit:
      if (exitCode == 0)
        WbLog::info(tr("'%1' controller exited successfully.").arg(name()));
      else
        WbLog::warning(tr("'%1' controller exited with status: %2.").arg(name()).arg(exitCode));
      break;
    case QProcess::CrashExit:
      WbLog::warning(tr("'%1' controller crashed.").arg(name()));
      break;
  }
  emit hasTerminatedByItself(this);
}

void WbController::reportControllerNotFound() {
  warn(tr("Could not find controller file:"));
  warn(tr("Expected either: %1, %2, %3, %4, or %5")
         .arg(name() + WbStandardPaths::executableExtension(), name() + ".jar", name() + ".class", name() + ".py",
              name() + ".m "));

  // try to give a smart advice
  QDir dir(mControllerPath);
  if (dir.exists(name() + ".c") || dir.exists(name() + ".cpp"))
    info(tr("Try to compile the C/C++ source code, to get a new executable file."));
  else if (dir.exists(name() + ".java"))
    info(tr("Try to compile the Java source code, to get a new .class or .jar file."));

  warn(tr("Starts the void controller instead."));
}

void WbController::reportMissingCommand(const QString &command) {
  WbLog::warning(tr("Unable to find the '%1' executable in the current PATH. "
                    "Please check your %1 installation. "
                    "It should be possible to launch %1 from a terminal by typing '%1'. "
                    "It may be necessary to add the %1 bin directory to your PATH environment variable. "
                    "More information about the %1 installation is available in Webots' User guide.")
                   .arg(command));
}

void WbController::reportFailedStart() {
  warn(tr("failed to start: %1").arg(commandLine()));

  switch (mType) {
    case WbFileUtil::EXECUTABLE: {
      QFileInfo fi(mCommand);
      if (!fi.isFile()) {
        warn(tr("This is not a valid file, maybe a directory."));
        warn(tr("Webots expects a binary executable file at this location."));
        return;
      }
      if (!fi.isExecutable()) {
        warn(tr("This is not an executable file, try to change its permissions."));
        return;
      }
      warn(tr("This is not a valid executable file."));
      warn(tr("Maybe it has the wrong binary architecture: try to recompile this controller."));
      return;
    }
    case WbFileUtil::CLASS:
    case WbFileUtil::JAR:
      reportMissingCommand("java");
      break;
    case WbFileUtil::PYTHON:
      reportMissingCommand("python");
      break;
    case WbFileUtil::MATLAB:
      reportMissingCommand("matlab");
      break;
    default:
      break;
  }
}

void WbController::processError(QProcess::ProcessError error) {
  switch (error) {
    case QProcess::FailedToStart:
      reportFailedStart();
      break;
    case QProcess::Crashed:
      warn(tr("The process crashed some time after starting successfully."));
      break;
    case QProcess::Timedout:
      warn(tr("The process didn't respond in time."));
      break;
    case QProcess::WriteError:
      warn(tr("An error occurred when attempting to write to the process."));
      break;
    case QProcess::ReadError:
      warn(tr("An error occurred when attempting to read from the process."));
      break;
    default:
      warn(tr("Unknown error."));
      break;
  }
}

WbFileUtil::FileType WbController::findType(const QString &controllerPath) {
  QDir dir(controllerPath);
  if (dir.exists(name() + WbStandardPaths::executableExtension()))
    return WbFileUtil::EXECUTABLE;
  else if (dir.exists(QString("build/release/%1%2").arg(name()).arg(WbStandardPaths::executableExtension())))
    return WbFileUtil::EXECUTABLE;
  else if (dir.exists(name() + ".class"))
    return WbFileUtil::CLASS;
  else if (dir.exists(name() + ".jar"))
    return WbFileUtil::JAR;
  else if (dir.exists(name() + ".py"))
    return WbFileUtil::PYTHON;
  else if (dir.exists(name() + ".m"))
    return WbFileUtil::MATLAB;
  else if (dir.exists(name() + ".bsg"))
    return WbFileUtil::BOTSTUDIO;

  return WbFileUtil::UNKNOWN;
}

void WbController::startVoidExecutable() {
  updateName("void");
  mControllerPath = WbStandardPaths::resourcesControllersPath() + "void/";
  mCommand = mControllerPath + "void" + WbStandardPaths::executableExtension();

  copyBinaryAndDependencies(mCommand);

  mCommand = QDir::toNativeSeparators(mCommand);
  mArguments << mRobot->controllerArgs();
}

void WbController::startExecutable() {
  mCommand = mControllerPath + name() + WbStandardPaths::executableExtension();

  copyBinaryAndDependencies(mCommand);

  mCommand = QDir::toNativeSeparators(mCommand);
  mArguments << mRobot->controllerArgs();
}

void WbController::startJava(bool jar) {
  if (mJavaCommand.isEmpty())
    mCommand = WbLanguageTools::javaCommand();
  else
    mCommand = mJavaCommand;

  mArguments = WbLanguageTools::javaArguments();
  const QProcessEnvironment &env = mProcess->processEnvironment();

  // add -classpath option (which is necessary for load find Controller.jar).
  // Note: the user cannot specify the "-classpath" java option in the runtime.ini
  // file, instead (s)he should define a CLASSPATH environment variable.
  QString classPath(env.value("CLASSPATH"));
  QString extraClassPath;
  if (jar)
    extraClassPath = WbProject::current()->controllersPath() + "/" + name() + "/" + name() + ".jar";
  else
    extraClassPath = mControllerPath;
  WbLanguageTools::prependToPath(WbSysInfo::shortPath(extraClassPath), classPath);
  WbLanguageTools::prependToPath(WbStandardPaths::controllerLibPath() + "java/Controller.jar", classPath);
  mArguments << "-classpath" << classPath;

  // add the java.library.path variable based on the custom JAVA_LIBRARY_PATH
  // environment variable (typically defined in runtime.ini)
  // in order to find the JNI libraries
  QString javaLibraryPath = env.value("JAVA_LIBRARY_PATH");
  WbLanguageTools::prependToPath(WbStandardPaths::controllerLibPath() + "java", javaLibraryPath);
  mArguments << QString("-Djava.library.path=%1").arg(javaLibraryPath);
  if (!mJavaOptions.isEmpty())
    mArguments << mJavaOptions.split(" ");
  mArguments << name();
  mArguments << mRobot->controllerArgs();
}

void WbController::startPython() {
  if (mPythonCommand == "!")  // wrong python version
    return;
  mCommand = mPythonCommand;
  mArguments = WbLanguageTools::pythonArguments();
  if (!mPythonOptions.isEmpty())
    mArguments << mPythonOptions.split(" ");
  mArguments << name() + ".py";
  mArguments << mRobot->controllerArgs();
}

void WbController::startMatlab() {
  if (WbSysInfo::isSnap()) {
    warn(tr("MATLAB controllers should be launched as extern controllers with the snap package of Webots."));
    return;
  }
  if (mMatlabCommand.isEmpty()) {
    mCommand = WbLanguageTools::matlabCommand();
    if (mCommand == "!")  // Matlab 64 bit not available
      return;
  } else
    mCommand = mMatlabCommand;

  mArguments = WbLanguageTools::matlabArguments();
  mArguments << "-r"
             << "launcher";
  mArguments << mRobot->controllerArgs();
}

void WbController::startBotstudio() {
  // display a warning if the robot window is not "botstudio"
  if (mRobot->window() != "botstudio")
    warn(tr("A BotStudio controller was detected, but the 'window' field of the Robot node is not set to \"botstudio\". "
            "The controller probably won't work as expected."));

  // start simply the void controller, but without modifying the controller path
  QString voidContollerPath = WbStandardPaths::resourcesControllersPath() + "void/";
  mCommand = voidContollerPath + "void" + WbStandardPaths::executableExtension();
  copyBinaryAndDependencies(mCommand);
  mCommand = QDir::toNativeSeparators(mCommand);
}

void WbController::copyBinaryAndDependencies(const QString &filename) {
  if (WbBinaryIncubator::copyBinaryAndDependencies(filename) == WbBinaryIncubator::FILE_REMOVE_ERROR) {
    warn(tr("An error occured during the copy of controller '%1'. An older version will be executed.\n"
            "Please close any running instances of the controller and reload the world.")
           .arg(filename));
    return;
  }

#ifdef __APPLE__
  // silently change RPATH before launching controller, if the controller is not in the installation path.
  if (filename.startsWith(WbStandardPaths::webotsHomePath()) || !QFileInfo(filename).isWritable())
    return;

  QProcess process;
  bool success;

  // get current RPATH
  const QString cmd = QString("otool -l %1 | grep LC_RPATH -A 3 | grep path | cut -c15- | cut -d' ' -f1").arg(filename);
  process.start("bash", QStringList() << "-c" << cmd);
  success = process.waitForFinished(500);
  if (!success || !process.readAllStandardError().isEmpty())
    return;
  const QString oldRPath = process.readAllStandardOutput().trimmed();

  // change RPATH
  QStringList args;
  if (oldRPath.isEmpty())
    args << "-add_rpath" << WbStandardPaths::webotsHomePath() << filename;
  else
    args << "-rpath" << oldRPath << WbStandardPaths::webotsHomePath() << filename;
  process.start("install_name_tool", args);
  process.waitForFinished(-1);
#endif
}

void WbController::setSocket(QLocalSocket *socket) {
  // associate controller and socket
  mSocket = socket;

  // wb_robot_init performs a wb_robot_step(0) generating a request which has to be catch.
  // This request is forced because the first packets coming from libController
  // may be splitted (wb_robot_init() sends firstly the robotId and the robot_step(0) package which have to be eaten there)
  while (mSocket->bytesAvailable() == 0)
    mSocket->waitForReadyRead();
  readRequest();

  connect(mSocket, SIGNAL(readyRead()), this, SLOT(readRequest()));
  connect(mRobot, &WbRobot::immediateMessageAdded, this, &WbController::writeImmediateAnswer);
  connect(mRobot, &WbRobot::userInputEventNeedUpdate, this, &WbController::writeUserInputEventAnswer);

  writeAnswer();  // send configure message and immediate answers if any
}

int WbController::robotId() const {
  return mRobot->uniqueId();
}

const QString &WbController::name() const {
  return mName;
}

QString WbController::commandLine() const {  // returns the command line with double quotes if needed
  QString commandLine = mCommand.contains(' ') ? '"' + mCommand + '"' : mCommand;
  foreach (QString argument, mArguments)
    commandLine +=
      ' ' + (argument.contains(' ') || (argument.contains('"')) ? '\"' + argument.replace('"', "\\\"") + '"' : argument);
  return commandLine;
}

void WbController::handleControllerExit() {
  if (mRobot->controllerName() == "<extern>") {
    processFinished(0, QProcess::NormalExit);
    mRobot->setControllerNeedRestart();
  }
}

void WbController::writeUserInputEventAnswer() {
  // prepare stream
  QByteArray buffer;
  QDataStream stream(&buffer, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream.device()->seek(sizeof(int));  // size, to be overwritten afterwards
  stream << (int)0;

  // dispatch the stream to the devices
  mRobot->setNeedToWriteUserInputEventAnswer();
  mRobot->dispatchAnswer(stream, false);

  // size management
  const int size = stream.device()->pos();
  stream.device()->seek(0);
  stream << size;

  // write the request
  mSocket->write(buffer.constData(), size);
  mSocket->flush();  // sometimes packets are simply not sent without flushing
}

void WbController::writeAnswer(bool immediateAnswer) {
  if (mRobot == NULL)
    // controller is being destroyed
    return;

  mHasPendingImmediateAnswer = false;

  // prepare stream
  QByteArray buffer;
  QDataStream stream(&buffer, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream.device()->seek(sizeof(int));  // size, to be overwritten afterwards

  // delay management
  // the time including the controller process time is the
  // time between two answers
  int delay = 0;
  if (!immediateAnswer && mRequestPending) {
    delay = mDeltaTimeMeasured - mDeltaTimeRequested;
    if (delay < 0)
      delay = 0;
  }
  stream << delay;
  // dispatch the stream to the devices
  mRobot->dispatchAnswer(stream);
  if (mRobot->hasImmediateAnswer())
    mRobot->writeImmediateAnswer(stream);

  // size management
  const int size = stream.device()->pos();
  stream.device()->seek(0);
  stream << size;

  // write the request
  mSocket->write(buffer.constData(), size);
  mSocket->flush();  // sometimes packets are simply not sent without flushing

  // reset request time
  if (!immediateAnswer)
    mRequestPending = false;

  // Debug code to see the content of the packet
  // static int id = 0;
  // printArray(buffer, "Answer", id++, true, true);
  WbPerformanceLog *log = WbPerformanceLog::instance();
  if (log)
    log->startMeasure(WbPerformanceLog::CONTROLLER, mName);
}

void WbController::writeImmediateAnswer() {
  if (!isRequestPending() || isIncompleteRequest() || WbControlledWorld::instance()->isExecutingStep()) {
    // mixing immediate messages sent by Webots and the libController could
    // make the simulation hang because of the reception of unexpected messages
    // in order to avoid that the Webots immediate messages are postponed
    mHasPendingImmediateAnswer = true;
    return;
  }

  mHasPendingImmediateAnswer = false;
  if (mRobot == NULL)
    // controller is being destroyed
    return;

  if (!mRobot->hasImmediateAnswer())
    return;

  // prepare stream
  QByteArray buffer;
  QDataStream stream(&buffer, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::LittleEndian);
  stream.device()->seek(sizeof(int));  // size, to be overwritten afterwards

  // immediate message
  const int delay = -1;
  stream << delay;

  // dispatch answer
  mRobot->writeImmediateAnswer(stream);

  // size management
  const int size = stream.device()->pos();

  assert(size > 8);  // the immediate message shouldn't be empty

  stream.device()->seek(0);
  stream << size;
  // write the request
  mSocket->write(buffer.constData(), size);
  mSocket->flush();  // sometimes packets are simply not sent without flushing
}

// this function matches with the reception of a datagram
// Warning: several Webots packets can be into a datagram, and
// a Webots packet can be splitted into several datagrams
void WbController::readRequest() {
  mProcessingRequest = true;
  if (mSocket == NULL || mRobot == NULL)
    return;

  // concat all the data which has not been parsed
  mRequest += mSocket->readAll();

  const bool needToBlockRegeneration = robot()->supervisor();
  if (needToBlockRegeneration)
    WbTemplateManager::instance()->blockRegeneration(true);

  bool immediateMessagesPending = false;
  while (true) {
    const unsigned int requestSize = mRequest.size();
    unsigned int webotsPacketSize = 0;

    // get a webots packet size
    if (requestSize < sizeof(int))
      break;

    QDataStream sizeStream(mRequest);
    sizeStream.setByteOrder(QDataStream::LittleEndian);
    sizeStream >> webotsPacketSize;

    if (webotsPacketSize == 0)
      break;  // could occur when a controller stops itself

    // check if packet is complete
    mIncompleteRequest = (requestSize < webotsPacketSize);
    if (mIncompleteRequest)
      break;

    // create the Webots packet
    const QByteArray webotsPacket = mRequest.left(webotsPacketSize);

    // Debug code to see the content of the packet
    // static int id = 0;
    // printArray(webotsPacket, "Request", id++, true, true);

    // create the stream on the Webots packet
    QDataStream stream(webotsPacket);
    stream.setByteOrder(QDataStream::LittleEndian);

    // read the first int (confirmation of the already readed webotsPacketSize
    unsigned int webotsPacketSizeConfirmation;
    stream >> webotsPacketSizeConfirmation;
    assert(webotsPacketSize == webotsPacketSizeConfirmation);

    // read ms
    immediateMessagesPending = false;  // e.g. supervisor
    if (webotsPacketSize >= 2 * sizeof(int)) {
      unsigned int ms;
      stream >> ms;

      const bool isConfigureMessage = !mRobot->isConfigureDone();

      // dispatch the message
      if (webotsPacketSize > 2 * sizeof(int))
        mRobot->dispatchMessage(stream);

      if (mRobot->isWaitingForUserInputEvent())
        // force the controller buffers flush to make sure eventual instructions are shown
        flushBuffers();

      // store stuff
      immediateMessagesPending = (ms == 0 && !isConfigureMessage);
      if (!immediateMessagesPending)
        mRequestPending = (webotsPacketSize > 2 * sizeof(int)) || ms > 0;

      if (ms > 0) {
        mDeltaTimeMeasured = WbSimulationState::instance()->time() - mRequestTime;
        mDeltaTimeRequested = ms;
        mRequestTime = WbSimulationState::instance()->time();
      }
    }

    // remove the packet from the request
    mRequest.remove(0, webotsPacketSize);

    if (immediateMessagesPending)
      writeAnswer(true);

    WbControlledWorld::instance()->checkIfReadRequestCompleted();
  }

  WbPerformanceLog *log = WbPerformanceLog::instance();
  if (log && !immediateMessagesPending)
    log->stopMeasure(WbPerformanceLog::CONTROLLER, mName);

  if (needToBlockRegeneration)
    WbTemplateManager::instance()->blockRegeneration(false);

  emit requestReceived();
  mProcessingRequest = false;
}

void WbController::robotDestroyed() {
  mRobot = NULL;
  WbControlledWorld::instance()->deleteController(this);
}
