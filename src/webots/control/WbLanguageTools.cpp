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

#include "WbLanguageTools.hpp"
#include "WbLog.hpp"
#include "WbPreferences.hpp"
#include "WbStandardPaths.hpp"
#include "WbSysInfo.hpp"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

#ifdef _WIN32
static const QChar PATHS_SEPARATOR(';');
#else
static const QChar PATHS_SEPARATOR(':');
#endif

static QString gJavaCommand;
static QString gMatlabCommand;

void WbLanguageTools::prependToPath(const QString &dir, QString &path) {
  if (path.isEmpty())
    path = dir;
  else
    path = dir + PATHS_SEPARATOR + path;
}

const QString &WbLanguageTools::javaCommand() {
  if (gJavaCommand.isEmpty()) {
#ifdef _WIN32
    gJavaCommand = "javaw.exe";
#else
    gJavaCommand = "java";
#endif
  }
  return gJavaCommand;
}

const QStringList WbLanguageTools::javaArguments() {
#ifdef __APPLE__
  // In order to run the robot window on the gui thread (thread 0)
  // which is a requirement of Qt, this option is required
  return QStringList("-XstartOnFirstThread");
#else
  return QStringList();
#endif
}

QString WbLanguageTools::pythonCommand(QString &shortVersion, const QString &command, QProcessEnvironment &env) {
  QString pythonCommand = command;
  const QString advice =
#ifdef __APPLE__
    "To fix the problem, you should set the full path of your python command in "
    "Webots->preferences->python command.\n";
#else
    QObject::tr("Webots requires Python version 3.9, 3.8"
#ifdef __linux__
                ", 3.7 or 3.6"  // we also support 3.6 on ubuntu 18.04
#else
                " or 3.7"
#endif
                " from python.org in your current PATH.\n"
                "To fix the problem, you should:\n"
                "1. Check the Python command set in the Webots preferences.\n"
                "2. Check the COMMAND set in the [python] section of the runtime.ini file of your controller program if any.\n"
                "3. Fix your PATH environment variable to use the required Python 64 bit version (if available).\n"
                "4. Install the required Python 64 bit version and ensure your PATH environment variable points to it.\n");
#endif
#ifdef _WIN32
  if (!command.endsWith(".exe", Qt::CaseInsensitive))
    pythonCommand += ".exe";
  QProcess process;
  process.setProcessEnvironment(env);
  process.start(pythonCommand, QStringList() << "-u"
                                             << "-c"
                                             << "import sys;print(sys.version);print(sys.maxsize > 2**32)");
  process.waitForFinished();
  const QString output = process.readAll();
  // "3.6.3 (v3.6.3:2c5fed8, Oct  3 2017, 18:11:49) [MSC v.1900 64 bit (AMD64)]\nTrue\n" or the like
  const QStringList version = output.split("\n");
  if (!version[0].startsWith("3.9.") && !version[0].startsWith("3.8.") && !version[0].startsWith("3.7.")) {
    WbLog::warning(QObject::tr("\"%1\" was not found.\n").arg(pythonCommand) + advice);
    pythonCommand = "!";
  } else if (version.size() > 1 && version[1].startsWith("False")) {
    WbLog::warning(QObject::tr("\"%1\" 64 bit was not found, but the 32 bit version was found.\n").arg(pythonCommand) + advice);
    pythonCommand = "!";
  } else
    shortVersion = QString(version[0][0]) + version[0][2];
#elif __APPLE__
  if (pythonCommand == "python" || pythonCommand == "python3") {
    pythonCommand = findWorkingPythonPath("3.8", env, false);
    shortVersion = "38";
    if (pythonCommand == "!") {
      pythonCommand = findWorkingPythonPath("3.9", env, false);
      shortVersion = "39";
      if (pythonCommand == "!") {
        pythonCommand = findWorkingPythonPath("3.7", env, true);
        shortVersion = "37";
      }
    }
  } else if (pythonCommand == "python3.7") {
    pythonCommand = findWorkingPythonPath("3.7", env, true);
    shortVersion = "37";
  } else if (pythonCommand == "python3.8") {
    pythonCommand = findWorkingPythonPath("3.8", env, true);
    shortVersion = "38";
  } else if (pythonCommand == "python3.9") {
    pythonCommand = findWorkingPythonPath("3.9", env, true);
    shortVersion = "39";
  } else {
    shortVersion = checkIfPythonCommandExist(pythonCommand, env, true);
    if (shortVersion.isEmpty())
      pythonCommand = "!";
  }

  if (pythonCommand == "!")
    WbLog::warning(QObject::tr("Python was not found.\n") + advice);
#else  // Linux
    shortVersion = checkIfPythonCommandExist(pythonCommand, env, true);
  if (shortVersion.isEmpty()) {
    pythonCommand = "!";
    WbLog::warning(QObject::tr("Python was not found.\n") + advice);
  }

#endif
  return pythonCommand;
}

#if defined __APPLE__ || defined __linux__
const QString WbLanguageTools::checkIfPythonCommandExist(const QString &pythonCommand, QProcessEnvironment &env, bool log) {
  QString shortVersion;
  QProcess process;
  process.setProcessEnvironment(env);
  process.start(pythonCommand, QStringList() << "-c"
                                             << "import sys;print(sys.version);");
  process.waitForFinished();
  const QString output = process.readAll();
  // "3.8.10 (tags/v3.8.10:3d8993a, May  3 2021, 11:48:03) [MSC v.1928 64 bit (AMD64)]" or the like
  const QStringList version = output.split(" ");
  if (!version[0].startsWith("3.")) {
    if (log)
      WbLog::warning(QObject::tr("\"%1\" was not found.\n").arg(pythonCommand));
    shortVersion = QString();
  } else
    shortVersion = QString(version[0][0]) + version[0][2];
  return shortVersion;
}
#endif

#ifdef __APPLE__
QString WbLanguageTools::findWorkingPythonPath(const QString &pythonVersion, QProcessEnvironment &env, bool log) {
  QString shortVersion;

  // look for python from python.org
  QString pythonCommand = "/Library/Frameworks/Python.framework/Versions/" + pythonVersion + "/bin/python" + pythonVersion;
  shortVersion = checkIfPythonCommandExist(pythonCommand, env, false);
  if (shortVersion.isEmpty()) {
    // look first possible path for python from homebrew
    pythonCommand = "/usr/local/opt/python@" + pythonVersion + " /bin/python" + pythonVersion;
    shortVersion = checkIfPythonCommandExist(pythonCommand, env, false);
    if (shortVersion.isEmpty()) {
      // look a second possible path for python from homebrew
      pythonCommand = "/usr/local/bin/python" + pythonVersion;
      shortVersion = checkIfPythonCommandExist(pythonCommand, env, log);
      if (shortVersion.isEmpty())
        pythonCommand = "!";
    }
  }

  return pythonCommand;
}
#endif

const QStringList WbLanguageTools::pythonArguments() {
  return QStringList("-u");
}

const QString &WbLanguageTools::matlabCommand() {
  if (gMatlabCommand.isEmpty()) {
#ifdef _WIN32
    // on Windows there are two MATLAB .exe files, one is located in
    // bin/matlab.exe and the other one in bin/win64/MATLAB.exe.
    // bin/matlab.exe is normally in the PATH, but we must call bin/win64/MATLAB.exe
    // because bin/matlab.exe is just a launcher that causes problem with stdout/stderr
    // and with the termination of the QProcess.
    QString PATH = qgetenv("PATH");
    QStringList dirs = PATH.split(';', Qt::SkipEmptyParts);
    foreach (QString dir, dirs) {
      if (QDir(dir).exists()) {
        QString file = dir + "\\win64\\MATLAB.exe";
        if (QFile::exists(file)) {
          gMatlabCommand = file;
          break;
        }
      }
    }
    if (gMatlabCommand.isEmpty()) {
      WbLog::warning(QObject::tr("To run Matlab controllers, you need to install Matlab 64-bit and ensure it is available "
                                 "from the DOS CMD.EXE console."));
      gMatlabCommand = "!";
    }
#else
    gMatlabCommand = "matlab";
#endif
  }
  return gMatlabCommand;
}

const QStringList WbLanguageTools::matlabArguments() {
  QStringList arguments("-nosplash");
  arguments << "-nodesktop";
#ifdef _WIN32
  // minimize option is only supported on Windows
  // http://www.mathworks.ch/ch/help/matlab/matlab_env/startup-options.html
  arguments << "-minimize";
#endif
#ifdef __linux__
  arguments << (WbSysInfo::isPointerSize64bits() ? "-glnxa64" : "-glnx86");
#endif
  return arguments;
}
