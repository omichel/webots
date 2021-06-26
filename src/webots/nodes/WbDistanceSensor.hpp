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

#ifndef WB_DISTANCE_SENSOR_HPP
#define WB_DISTANCE_SENSOR_HPP

#include "WbSolidDevice.hpp"

class WbSFDouble;
class WbSFInt;
class WbSFString;
class WbSensor;
class WbLookupTable;

class QDataStream;
class SensorRay;
typedef struct dxGeom *dGeomID;
struct dContactGeom;

struct WrTransform;
struct WrDynamicMesh;
struct WrStaticMesh;
struct WrMaterial;
struct WrRenderable;

class WbDistanceSensor : public WbSolidDevice {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbDistanceSensor(WbTokenizer *tokenizer = NULL);
  WbDistanceSensor(const WbDistanceSensor &other);
  explicit WbDistanceSensor(const WbNode &other);
  virtual ~WbDistanceSensor();

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_DISTANCE_SENSOR; }
  void preFinalize() override;
  void postFinalize() override;
  void handleMessage(QDataStream &) override;
  void writeAnswer(QDataStream &) override;
  void writeConfigure(QDataStream &) override;
  void createOdeObjects() override;
  void createWrenObjects() override;
  void prePhysicsStep(double ms) override;
  void postPhysicsStep() override;
  bool refreshSensorIfNeeded() override;
  void reset(const QString &id) override;

  // other functions
  void rayCollisionCallback(WbGeometry *object, dGeomID rayGeom, const dContactGeom *);

protected:
  void propagateScale() override;
  void updateLineScale() override;

private slots:
  void updateRaySetup();
  void updateOptionalRendering(int option);

private:
  enum { GENERIC, INFRA_RED, SONAR, LASER };

  // private functions
  WbDistanceSensor &operator=(const WbDistanceSensor &);  // non copyable
  WbNode *clone() const override { return new WbDistanceSensor(*this); }
  void init();
  void polarTo3d(double alpha, double theta, int i);
  void setupRayDirs();
  void setupRayWeights();
  void setSensorRays();
  void createOdeRays();
  void computeValue();
  void applyLaserBeamToWren();
  void applyOptionalRenderingToWren();
  void addConfigure(QDataStream &stream);
  void updateRaysSetupIfNeeded() override;

  // user accessible fields
  WbMFVector3 *mLookupTable;
  WbSFString *mType;
  WbSFDouble *mAperture;
  WbSFInt *mNumberOfRays;
  WbSFDouble *mGaussianWidth;
  WbSFDouble *mResolution;
  WbSFDouble *mRedColorSensitivity;

  // other fields
  WbLookupTable *mLut;
  WbSensor *mSensor;
  SensorRay *mRays;  // rays
  bool mIsSubscribedToRayTracing;

  // WREN
  WrTransform *mTransform;
  WrDynamicMesh *mMesh;
  WrRenderable *mRenderable;
  WrMaterial *mMaterial;

  // Laser beam
  WrTransform *mLaserBeamTransform;
  WrStaticMesh *mLaserBeamMesh;
  WrRenderable *mLaserBeamRenderable;
  WrMaterial *mLaserBeamMaterial;

  double mValue;     // current sensor value according to lookup table
  double mDistance;  // current averaged measured distance
  int mRayType;      // GENERIC, INFRA_RED, SONAR or LASER
  int mNRays;
  bool mNeedToReconfigure;
};

#endif
