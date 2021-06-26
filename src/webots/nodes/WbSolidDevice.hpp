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

#ifndef WB_SOLID_DEVICE_HPP
#define WB_SOLID_DEVICE_HPP

//
// Description: abstract base classes for all Webots solid devices
//

#include "WbDevice.hpp"
#include "WbSolid.hpp"

// Generic Webots devices, i.e., all devices but WbMotor and WbPositionSensor
class WbSolidDevice : public WbDevice, public WbSolid {
public:
  // destructor
  virtual ~WbSolidDevice();
  const QString &deviceName() const override { return WbSolid::name(); }
  int deviceNodeType() const override { return nodeType(); }

  // device name is unique in Robot scope so the string can be simplified
  QString computeShortUniqueName() const;

  // methods to update sensors between the ODE physics step and the ODE collision detection
  static bool hasDirtySensors() { return !cDirtySensors.isEmpty(); }
  static void clearDirtySensorsList() { cDirtySensors.clear(); }
  static void updateDirtySensors(const QList<dSpaceID> &spacesInCluster);

protected:
  // all constructors are reserved for derived classes only
  WbSolidDevice(const QString &modelName, WbTokenizer *tokenizer);
  WbSolidDevice(const WbSolidDevice &other);
  explicit WbSolidDevice(const WbNode &other);

  void subscribeToRaysUpdate(dGeomID ray);
  virtual void updateRaysSetupIfNeeded() {}

private:
  static QList<QPair<WbSolidDevice *, dGeomID>> cDirtySensors;
};

#endif
