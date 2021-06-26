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

#include "WbPrecision.hpp"

#include <QtCore/QRegExp>

#include <cassert>
#include <limits>

QString WbPrecision::doubleToString(double value, Level level) {
  switch (level) {
    case DOUBLE_MAX: {
      // Returning the maximimum precision causes noise digits like: "0.10000000000000001"
      //     return QString::number(value, 'g', std::numeric_limits<double>::max_digits10);
      // Looping from digits10 to max_digits10 avoids this issue:
      // cf. https://stackoverflow.com/questions/4738768/printing-double-without-losing-precision/4742599#4742599
      QString r;
      for (int k = std::numeric_limits<double>::digits10; k <= std::numeric_limits<double>::max_digits10; ++k) {
        r.setNum(value, 'g', k);
        const double v = r.toDouble();
        if (v == value)
          return r;
      }
      assert(0);
      return r;
    }
    case FLOAT_MAX: {
      QString r;
      float fValue = value;
      for (int k = std::numeric_limits<float>::digits10; k <= std::numeric_limits<float>::max_digits10; ++k) {
        r.setNum(fValue, 'g', k);
        const float v = r.toFloat();
        if (v == fValue)
          return r;
      }
      assert(0);
      return r;
    }
    case FLOAT_ROUND_6: {
      QString str = QString::number(value, 'f', 6);
      static QRegExp r("0+$|\\.0+$");  // Remove any number of trailing 0's including '.' if needed
      str.remove(r);
      if (str == "-0")
        return "0";
      return str;
    }
    case GUI_LOW:
      return QString::number(value, 'g', 3);
    default:  // GUI_MEDIUM
      return QString::number(value, 'g', 6);
  }
}

double WbPrecision::roundValue(double value, Level level) {
  return doubleToString(value, level).toDouble();
}
