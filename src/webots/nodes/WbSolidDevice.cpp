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

#include "WbSolidDevice.hpp"
#include "WbNodeUtilities.hpp"
#include "WbOdeGeomData.hpp"
#include "WbRobot.hpp"
#include "WbSolidMerger.hpp"

#include <ode/ode.h>

QList<QPair<WbSolidDevice *, dGeomID>> WbSolidDevice::cDirtySensors;

void WbSolidDevice::updateDirtySensors(const QList<dSpaceID> &spacesInCluster) {
  if (cDirtySensors.isEmpty())
    return;
  int size = cDirtySensors.size();
  WbSolidDevice *s = NULL;
  dGeomID ray = NULL;
  for (int i = 0; i < size; ++i) {
    s = cDirtySensors[i].first;
    ray = cDirtySensors[i].second;
    if (spacesInCluster.contains(dGeomGetSpace(ray)))
      s->updateRaysSetupIfNeeded();
  }
}

WbSolidDevice::WbSolidDevice(const QString &modelName, WbTokenizer *tokenizer) : WbDevice(), WbSolid(modelName, tokenizer) {
}

WbSolidDevice::WbSolidDevice(const WbSolidDevice &other) : WbDevice(other), WbSolid(other) {
}

WbSolidDevice::WbSolidDevice(const WbNode &other) : WbDevice(), WbSolid(other) {
}

WbSolidDevice::~WbSolidDevice() {
}

void WbSolidDevice::subscribeToRaysUpdate(dGeomID ray) {
  cDirtySensors.append(QPair<WbSolidDevice *, dGeomID>(this, ray));
}

QString WbSolidDevice::computeShortUniqueName() const {
  WbSolid *robot = WbNodeUtilities::findRobotAncestor(this);
  if (robot)
    return QString("%1:%2").arg(robot->computeUniqueName()).arg(WbSolid::name());
  return computeUniqueName();
}
