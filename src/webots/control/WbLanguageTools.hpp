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

#ifndef WB_LANGUAGE_TOOLS_HPP
#define WB_LANGUAGE_TOOLS_HPP

#include <QtCore/QString>

class QProcessEnvironment;

class WbLanguageTools {
public:
  // get platform dependent commands
  static QString pythonCommand(QString &shortVersion, const QString &command, QProcessEnvironment &env);
  static const QStringList pythonArguments();
  static const QString &javaCommand();
  static const QStringList javaArguments();
  static const QString &matlabCommand();
  static const QStringList matlabArguments();

  // add dir in front of path, in a platform independent way
  static void prependToPath(const QString &dir, QString &path);

private:
  WbLanguageTools() {}
  ~WbLanguageTools() {}
#if defined __APPLE__ || defined __linux__
  static const QString checkIfPythonCommandExist(const QString &pythonCommand, QProcessEnvironment &env, bool log);
#endif
#ifdef __APPLE__
  static QString findWorkingPythonPath(const QString &pythonVersion, QProcessEnvironment &env, bool log);
#endif
};

#endif
