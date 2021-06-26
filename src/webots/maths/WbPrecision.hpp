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

#ifndef WB_PRECISION_HPP
#define WB_PRECISION_HPP

#include <math.h>
#include <QtCore/QString>
#include <limits>

namespace WbPrecision {

  // Defines the minimum tolerance when comparing doubles.
  // This value assumes that few mathematical operations have been applied on the double.
  // `R` programming language set this to k * epsilon, where k is an arbitrary constant set to 1000.
  // cf. https://www.rdocumentation.org/packages/scales/versions/0.4.1/topics/zero_range
  // In Webots, it turns out we need to set this constant to 10000 to fix issue
  // https://github.com/omichel/webots-dev/issues/6519

  const double DOUBLE_EQUALITY_TOLERANCE = 10000.0 * std::numeric_limits<double>::epsilon();

  enum Level { DOUBLE_MAX, FLOAT_MAX, FLOAT_ROUND_6, GUI_MEDIUM, GUI_LOW };

  // - GUI_MEDIUM and GUI_LOW case (reserved for the GUI):
  //     - a string with an aribtrary precision is returned.
  // - DOUBLE_MAX and FLOAT_MAX case: this function ensures that:
  //     - a string <-> floating point conversion without loosing precision.
  //     - the result is the shortest possible string.
  QString doubleToString(double value, Level level);

  double roundValue(double value, Level level);

};  // namespace WbPrecision

#endif
