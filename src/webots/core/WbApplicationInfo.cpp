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

#include "WbApplicationInfo.hpp"
#include "WbLog.hpp"
#include "WbStandardPaths.hpp"
#include "WbVersion.hpp"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <stdio.h>
#include <time.h>

const WbVersion &WbApplicationInfo::version() {
  static WbVersion webotsVersion;
  static bool firstCall = true;

  if (firstCall) {
    static QString webotsVersionString = "R2021b";  // updated by script
    bool success = webotsVersion.fromString(webotsVersionString);
    if (!success)
      WbLog::fatal(QObject::tr("Internal error: the Webots version is not computable."));
    firstCall = false;
  }
  return webotsVersion;
}

const QString &WbApplicationInfo::branch() {
  static QString branchName;
  static bool firstCall = true;

  if (firstCall) {
    QFile file(WbStandardPaths::webotsHomePath() + "resources/branch.txt");
    if (file.open(QIODevice::ReadOnly)) {
      QTextStream in(&file);
      const QString line = in.readLine();
      if (!line.isNull())
        branchName = line;

      file.close();
    }
  }
  return branchName;
}

unsigned int WbApplicationInfo::releaseDate() {
  char s_month[4];
  int month, day, year;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  sscanf(__DATE__, "%3s %d %d", s_month, &day, &year);
  month = (strstr(month_names, s_month) - month_names) / 3;
  struct tm t;
  t.tm_sec = 0;
  t.tm_min = 0;
  t.tm_hour = 0;
  t.tm_mday = day;
  t.tm_wday = 0;
  t.tm_yday = 0;
  t.tm_mon = month;
  t.tm_year = year - 1900;
  t.tm_isdst = -1;
  return mktime(&t);
}
