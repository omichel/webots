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

#include "WbHiddenKinematicParameters.hpp"

#include "WbField.hpp"
#include "WbSFDouble.hpp"
#include "WbSFRotation.hpp"
#include "WbSFVector3.hpp"

#include <assert.h>

void WbHiddenKinematicParameters::createHiddenKinematicParameter(
  WbField *field, WbHiddenKinematicParameters::HiddenKinematicParametersMap &map) {
  // Extract solid and joint indices
  static const QRegExp rx1("(_\\d+)+$");  // looks for a substring of the form _7 or _13_1 at the end of the parameter name,
                                          // e.g. as in rotation_7, position2_13_1
  const QString parameterName(field->name());

  rx1.indexIn(parameterName);
  const QString str1(rx1.cap(0));

  int pos = 0;
  static const QRegExp rx2("(\\d+)");
  QStringList indices;
  while ((pos = rx2.indexIn(str1, pos)) != -1) {
    indices << rx2.cap(1);
    pos += rx2.matchedLength();
  }

  assert(indices.size() > 0);
  const int solidIndex = indices[0].toInt();
  HiddenKinematicParameters *const hkp = map.value(solidIndex, NULL);
  HiddenKinematicParameters *const data = (hkp == NULL) ? new HiddenKinematicParameters() : hkp;

  const WbSFVector3 *const sfvec3f = dynamic_cast<WbSFVector3 *>(field->value());
  if (sfvec3f) {
    const double x = sfvec3f->x();
    const double y = sfvec3f->y();
    const double z = sfvec3f->z();

    if (parameterName.startsWith("translation"))
      data->createTranslation(x, y, z);
    else if (parameterName.startsWith("linearVelocity"))
      data->createLinearVelocity(x, y, z);
    else if (parameterName.startsWith("angularVelocity"))
      data->createAngularVelocity(x, y, z);

  } else if (parameterName.startsWith("rotation")) {
    const WbSFRotation *const sfrotation = static_cast<WbSFRotation *>(field->value());
    data->createRotation(sfrotation->x(), sfrotation->y(), sfrotation->z(), sfrotation->angle());
  } else if (parameterName.startsWith("position")) {
    assert(indices.size() > 1);
    const int jointIndex = indices[1].toInt();
    const int j = (parameterName.at(8) == QChar('3')) ? 2 : (parameterName.at(8) == QChar('2')) ? 1 : 0;
    WbVector3 *v = data->positions(jointIndex);
    if (v == NULL)
      v = new WbVector3(NAN, NAN, NAN);
    const WbSFDouble *const sfdouble = static_cast<WbSFDouble *>(field->value());
    (*v)[j] = sfdouble->value();
    data->insertPositions(jointIndex, v);
  }

  if (hkp == NULL)
    map.insert(solidIndex, data);
}
