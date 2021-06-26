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

#include "WbDistanceSensor.hpp"

#include "WbFieldChecker.hpp"
#include "WbGeometry.hpp"
#include "WbLookupTable.hpp"
#include "WbMFVector3.hpp"
#include "WbMathsUtilities.hpp"
#include "WbNodeUtilities.hpp"
#include "WbOdeContext.hpp"
#include "WbOdeGeomData.hpp"
#include "WbRay.hpp"
#include "WbRgb.hpp"
#include "WbSFDouble.hpp"
#include "WbSFInt.hpp"
#include "WbSensor.hpp"
#include "WbShape.hpp"
#include "WbSimulationState.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include "../../controller/c/messages.h"

#include <wren/config.h>
#include <wren/dynamic_mesh.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/scene.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include <QtCore/QDataStream>

#include <limits>

// circle parts
static const double HALF = M_PI;
static const double THIRD = 2 * M_PI / 3;
static const double QUARTER = M_PI_2;
static const double FIFTH = 2 * M_PI / 5;
static const double SIXTH = M_PI / 3;
static const double SEVENTH = 2 * M_PI / 7;

// number of predifined configurations
static const int NUM_PREDEFINED = 10;

// definition of predifined combinations
static const double POLAR[NUM_PREDEFINED][NUM_PREDEFINED][2] = {
  {{0, 0}},
  {{QUARTER, 1}, {-QUARTER, 1}},
  {{0, 1}, {THIRD, 1}, {-THIRD, 1}},
  {{0, 0}, {0, 1}, {THIRD, 1}, {-THIRD, 1}},
  {{0, 0}, {0, 1}, {QUARTER, 1}, {HALF, 1}, {-QUARTER, 1}},
  {{0, 0}, {0, 1}, {FIFTH, 1}, {2 * FIFTH, 1}, {3 * FIFTH, 1}, {4 * FIFTH, 1}},
  {{0, 0}, {0, 1}, {SIXTH, 1}, {2 * SIXTH, 1}, {3 * SIXTH, 1}, {4 * SIXTH, 1}, {5 * SIXTH, 1}},
  {{0, 0}, {0, 1}, {SEVENTH, 1}, {2 * SEVENTH, 1}, {3 * SEVENTH, 1}, {4 * SEVENTH, 1}, {5 * SEVENTH, 1}, {6 * SEVENTH, 1}},
  {{0, 0.3}, {THIRD, 0.3}, {-THIRD, 0.3}, {0, 1}, {SIXTH, 1}, {2 * SIXTH, 1}, {3 * SIXTH, 1}, {4 * SIXTH, 1}, {5 * SIXTH, 1}},
  {{0, 0},
   {0, 0.5},
   {THIRD, 0.5},
   {-THIRD, 0.5},
   {0, 1},
   {SIXTH, 1},
   {2 * SIXTH, 1},
   {3 * SIXTH, 1},
   {4 * SIXTH, 1},
   {5 * SIXTH, 1}}};

class SensorRay {
public:
  SensorRay() {
    mDistance = std::numeric_limits<double>::infinity();
    mGeom = NULL;
    mCollidedGeometry = NULL;
    mWeight = 1.0;
    mContactPosition[0] = 0.0;
    mContactPosition[1] = 0.0;
    mContactPosition[2] = 0.0;
    mContactNormal[0] = 0.0;
    mContactNormal[1] = 0.0;
    mContactNormal[2] = 0.0;
  };

  ~SensorRay() {
    if (mGeom) {
      WbOdeGeomData *odeGeomData = static_cast<WbOdeGeomData *>(dGeomGetData(mGeom));
      delete odeGeomData;
      dGeomDestroy(mGeom);
    }
  }

  // for GENERIC
  void setCollision(WbGeometry *geometry, double depth) {
    if (depth < mDistance) {
      mCollidedGeometry = geometry;
      mDistance = depth;
    }
  }

  // for LASER and SONAR
  void setCollision(WbGeometry *geometry, const dContactGeom *contact) {
    if (contact->depth < mDistance) {
      mCollidedGeometry = geometry;
      // the meaning of ODE contact depth for a ray collision is "distance from ray's origin to the contact point"
      mDistance = contact->depth;
      memcpy(mContactPosition, contact->pos, sizeof(dVector3));
      memcpy(mContactNormal, contact->normal, sizeof(dVector3));

      // according to ODE, in a dContactGeom, the normal vector points "in" g1
      // this means that it point "out of" g2, this is fine if g2 is the primitive
      // however if g2 is the sensor's ray then we must invert the normal
      if (dGeomGetClass(contact->g2) == dRayClass) {
        mContactNormal[0] = -mContactNormal[0];
        mContactNormal[1] = -mContactNormal[1];
        mContactNormal[2] = -mContactNormal[2];
      }
    }
  }
  void resetCollision() {
    mCollidedGeometry = NULL;
    mDistance = std::numeric_limits<double>::infinity();
  }

  // getters
  double distance() const { return mDistance; }
  double weight() const { return mWeight; }
  dGeomID geom() const { return mGeom; }
  WbGeometry *collidedGeometry() const { return mCollidedGeometry; }
  const WbVector3 &direction() const { return mDirection; }
  const dReal *contactPosition() const { return mContactPosition; }
  const dReal *contactNormal() const { return mContactNormal; }

  // setters
  void setGeom(dGeomID geom) {
    if (mGeom)
      dGeomDestroy(mGeom);
    mGeom = geom;
  }
  void setDirection(double x, double y, double z) { mDirection.setXyz(x, y, z); }
  void setDistance(double distance) { mDistance = distance; }
  void setWeight(double weight) { mWeight = weight; }

protected:
  WbVector3 mDirection;
  double mWeight;
  double mDistance;

  // ODE ray tracing
  dGeomID mGeom;
  WbGeometry *mCollidedGeometry;
  dVector3 mContactPosition;
  dVector3 mContactNormal;
};

void WbDistanceSensor::init() {
  mValue = 0.0;
  mDistance = std::numeric_limits<double>::infinity();
  mRayType = SONAR;
  mNRays = 1;
  mSensor = NULL;
  mRays = NULL;
  mLut = NULL;
  mIsSubscribedToRayTracing = false;
  mNeedToReconfigure = false;

  mLookupTable = findMFVector3("lookupTable");
  mType = findSFString("type");
  mAperture = findSFDouble("aperture");
  mNumberOfRays = findSFInt("numberOfRays");
  mGaussianWidth = findSFDouble("gaussianWidth");
  mResolution = findSFDouble("resolution");
  mRedColorSensitivity = findSFDouble("redColorSensitivity");

  mTransform = NULL;
  mMesh = NULL;
  mRenderable = NULL;
  mMaterial = NULL;

  mLaserBeamTransform = NULL;
  mLaserBeamMesh = NULL;
  mLaserBeamRenderable = NULL;
  mLaserBeamMaterial = NULL;
}

WbDistanceSensor::WbDistanceSensor(WbTokenizer *tokenizer) : WbSolidDevice("DistanceSensor", tokenizer) {
  init();
}

WbDistanceSensor::WbDistanceSensor(const WbDistanceSensor &other) : WbSolidDevice(other) {
  init();
}

WbDistanceSensor::WbDistanceSensor(const WbNode &other) : WbSolidDevice(other) {
  init();
}

WbDistanceSensor::~WbDistanceSensor() {
  delete mSensor;
  delete mLut;
  delete[] mRays;
  if (areWrenObjectsInitialized()) {
    // Optional rendering
    wr_node_delete(WR_NODE(mRenderable));
    wr_node_delete(WR_NODE(mTransform));
    wr_material_delete(mMaterial);
    wr_dynamic_mesh_delete(mMesh);

    // Laser beam
    wr_node_delete(WR_NODE(mLaserBeamRenderable));
    wr_node_delete(WR_NODE(mLaserBeamTransform));
    wr_material_delete(mLaserBeamMaterial);
    wr_static_mesh_delete(mLaserBeamMesh);
  }

  if (mIsSubscribedToRayTracing) {
    mIsSubscribedToRayTracing = false;
    WbSimulationState::instance()->unsubscribeToRayTracing();
  }
}

void WbDistanceSensor::preFinalize() {
  WbSolidDevice::preFinalize();

  mSensor = new WbSensor();
  updateRaySetup();
}

void WbDistanceSensor::postFinalize() {
  WbSolidDevice::postFinalize();

  connect(mLookupTable, &WbMFVector3::changed, this, &WbDistanceSensor::updateRaySetup);
  connect(mType, &WbSFString::changed, this, &WbDistanceSensor::updateRaySetup);
  connect(mAperture, &WbSFDouble::changed, this, &WbDistanceSensor::updateRaySetup);
  connect(mNumberOfRays, &WbSFInt::changed, this, &WbDistanceSensor::updateRaySetup);
  connect(mGaussianWidth, &WbSFDouble::changed, this, &WbDistanceSensor::updateRaySetup);
  connect(mResolution, &WbSFDouble::changed, this, &WbDistanceSensor::updateRaySetup);
  connect(mRedColorSensitivity, &WbSFDouble::changed, this, &WbDistanceSensor::updateRaySetup);
}

void WbDistanceSensor::updateRaySetup() {
  // update type
  if (mType->value().compare("generic", Qt::CaseInsensitive) == 0)
    mRayType = GENERIC;
  else if (mType->value().compare("infra-red", Qt::CaseInsensitive) == 0 ||
           mType->value().compare("infrared", Qt::CaseInsensitive) == 0)
    mRayType = INFRA_RED;
  else if (mType->value().compare("laser", Qt::CaseInsensitive) == 0)
    mRayType = LASER;
  else
    mRayType = SONAR;

  // correct invalid input values
  if (WbFieldChecker::resetDoubleIfNegative(this, mAperture, -mAperture->value()))
    return;  // in order to avoiding passing twice in this function
  if (WbFieldChecker::resetIntIfLess(this, mNumberOfRays, 1, 1))
    return;  // in order to avoiding passing twice in this function
  if (WbFieldChecker::resetDoubleIfNonPositive(this, mGaussianWidth, 1.0))
    return;  // in order to avoiding passing twice in this function
  if (WbFieldChecker::resetDoubleIfNonPositiveAndNotDisabled(this, mResolution, -1, -1))
    return;  // in order to avoiding passing twice in this function
  if (WbFieldChecker::resetDoubleIfNegative(this, mRedColorSensitivity, -mRedColorSensitivity->value()))
    return;  // in order to avoiding passing twice in this function
  if (mRayType == LASER && mNumberOfRays->value() > 1) {
    parsingWarn(tr("'type' \"laser\" must have one single ray."));
    mNumberOfRays->setValue(1);
    return;  // in order to avoiding passing twice in this function
  }

  // rebuild the lookup table
  delete mLut;
  mLut = new WbLookupTable(*mLookupTable);

  // if a null aperture is specified together with multiple rays
  // then improve performance by casting one single ray only
  mNRays = (mNumberOfRays->value() > 1 && mAperture->isZero()) ? 1 : mNumberOfRays->value();

  // alloc/realloc buffers
  delete[] mRays;
  mRays = new SensorRay[mNRays];

  // compute the rays directions and weights
  setupRayDirs();
  setupRayWeights();

  // notify wren (if already created)
  if (areWrenObjectsInitialized()) {
    applyLaserBeamToWren();
    applyOptionalRenderingToWren();
  }

  // notify ode (if already created)
  if (areOdeObjectsCreated() && mRayType != INFRA_RED)
    createOdeRays();

  // subscribe for ray tracing if needed
  if (mRayType == INFRA_RED) {
    if (!mIsSubscribedToRayTracing) {
      mIsSubscribedToRayTracing = true;
      WbSimulationState::instance()->subscribeToRayTracing();
    }
  } else {
    if (mIsSubscribedToRayTracing) {
      mIsSubscribedToRayTracing = false;
      WbSimulationState::instance()->unsubscribeToRayTracing();
    }
  }

  mNeedToReconfigure = true;
}

void WbDistanceSensor::polarTo3d(double alpha, double theta, int i) {
  // rotate the cone so that its initial angle
  // looks upwards, that ways we obtain a left/right symmetry
  alpha += QUARTER;

  // rescale the cone tip angle to fit all rays
  // in the user-defined aperture angle
  theta *= mAperture->value() / 2;

  // first rotate around x-axis which is the sensors central ray axis
  const double x = cos(theta);
  double z = -sin(theta);

  // then rotate around y-axis
  const double y = -z * sin(alpha);
  z *= cos(alpha);

  mRays[i].setDirection(x, y, z);
}

void WbDistanceSensor::setupRayDirs() {
  if (mNRays == 1)
    mRays[0].setDirection(1, 0, 0);
  else {
    if (mNRays > NUM_PREDEFINED) {
      // Not a predefined configuration: arrange rays in a 3d
      // cone oriented towards x. The cone is further divided in a
      // number of thinner cones that will accomodate some rays
      int left = mNRays;  // number of rays left to be assigned
      int ncone;          // number of orbits
      int s = 0;          // number of so far assigned rays

      double *const alphas = new double[mNRays];
      double *const thetas = new double[mNRays];

      // as long as there are rays left to be assigned
      for (ncone = 0; left > 0; ncone++) {
        const int capacity = 3 * ncone + 1;
        const int m = (left > capacity) ? capacity : left;

        const double twoPiOverM = 2.0 * M_PI / m;
        // within a cone arrange the rays in a circle
        for (int i = 0; i < m; i++) {
          alphas[s] = s * twoPiOverM;
          thetas[s] = ncone;
          s++;
        }

        left -= capacity;
      }

      // for each ray ...
      for (int i = 0; i < mNRays; i++) {
        // rescale the cones to fit all rays
        thetas[i] /= (ncone - 1);

        // convert from polar to 3d coordinate system
        polarTo3d(alphas[i], thetas[i], i);
      }

      delete[] alphas;
      delete[] thetas;
    } else {
      // for each ray ...
      for (int i = 0; i < mNRays; i++)
        // convert from polar to 3d coordinate system
        polarTo3d(POLAR[mNRays - 1][i][0], POLAR[mNRays - 1][i][1], i);
    }
  }
}

void WbDistanceSensor::setupRayWeights() {
  // avoid calculations for the most common case: mNRays == 1
  if (mNRays == 1) {
    mRays[0].setWeight(1.0);
    return;
  }
  // create gaussian distribution
  double sum = 0;
  for (int i = 0; i < mNRays; i++) {
    const WbVector3 &dir = mRays[i].direction();
    const double theta = acos(dir.x() / dir.length());
    assert(!std::isnan(theta));
    const double temp = theta / (mAperture->value() * mGaussianWidth->value());
    const double w = exp(-(temp * temp));
    mRays[i].setWeight(w);
    sum += w;
  }

  // normalize such that the sum of all rays equals 1.0
  for (int i = 0; i < mNRays; i++) {
    double w = mRays[i].weight();
    mRays[i].setWeight(w / sum);
  }
}

void WbDistanceSensor::createOdeObjects() {
  WbSolidDevice::createOdeObjects();

  if (mRayType != INFRA_RED) {
    createOdeRays();
  }
}

void WbDistanceSensor::createOdeRays() {
  const double lutMaxRange = absoluteScale().x() * mLut->maxMetricsRange();
  for (int i = 0; i < mNRays; i++) {
    dGeomID rayGeom = dCreateRay(WbOdeContext::instance()->space(), lutMaxRange);
    dGeomSetDynamicFlag(rayGeom);
    dGeomSetData(rayGeom, new WbOdeGeomData(this));
    mRays[i].setGeom(rayGeom);
  }
}

void WbDistanceSensor::setSensorRays() {
  const WbMatrix4 &m = matrix();
  const WbVector3 &trans = m.translation();
  for (int i = 0; i < mNRays; i++)
    if (mRays[i].geom()) {  // NOT INFRA_RED
      // get ray direction
      const WbVector3 &dir = mRays[i].direction();
      assert(dir != WbVector3(0, 0, 0));

      // apply sensor's coordinate system transformation to rays
      WbVector3 r = m.sub3x3MatrixDot(dir);
      if (r.isNull())  // Prevent ODE from crashing on zero direction vector
        r.setXyz(1.0, 0.0, 0.0);
      // setup ray position and direction for ODE collision detection
      dGeomRaySet(mRays[i].geom(), trans.x(), trans.y(), trans.z(), r.x(), r.y(), r.z());  // r is normalized by ODE
    }
}

void WbDistanceSensor::updateRaysSetupIfNeeded() {
  updateTransformForPhysicsStep();
  setSensorRays();
}

void WbDistanceSensor::prePhysicsStep(double ms) {
  WbSolidDevice::prePhysicsStep(ms);

  if (isPowerOn() && mSensor->needToRefreshInMs(ms) && mNRays > 0) {
    // reset collisions
    for (int i = 0; i < mNRays; ++i)
      mRays[i].resetCollision();

    if (mRays[0].geom() != NULL)  // not infra-red
      // sensor or target objects could move during physics step
      subscribeToRaysUpdate(mRays[0].geom());
  }
}

bool WbDistanceSensor::refreshSensorIfNeeded() {
  if (isPowerOn() && mSensor->needToRefresh()) {
    computeValue();
    mSensor->updateTimer();
    return true;
  }
  return false;
}

void WbDistanceSensor::reset(const QString &id) {
  WbSolidDevice::reset(id);
  updateOptionalRendering(WbWrenRenderingContext::VF_DISTANCE_SENSORS_RAYS);
}

void WbDistanceSensor::computeValue() {
  static const double PI_OVER_8 = 0.392699082;  // used for sonar type

  // value when no object collide
  if (mLut->isEmpty()) {
    mValue = 0.0;
    mDistance = std::numeric_limits<double>::infinity();
    return;
  }

  const double s = absoluteScale().x();
  const double lutMaxRange = s * mLut->maxMetricsRange();

  if (mRayType == GENERIC) {
    // average all ray collision distances using ray weights
    mDistance = 0.0;
    for (int i = 0; i < mNRays; i++) {
      const double dist = (mRays[i].collidedGeometry()) ? mRays[i].distance() : lutMaxRange;
      mDistance += dist * mRays[i].weight();
    }
  } else if (mRayType == INFRA_RED) {
    double averageInfraRedFactor = 0.0;
    mDistance = 0.0;
    for (int i = 0; i < mNRays; i++) {
      double distance = 0.0;
      // apply sensor's coordinate system transformation to rays
      const WbMatrix4 &m = matrix();
      const WbVector3 &trans = m.translation();
      WbVector3 r = m.sub3x3MatrixDot(mRays[i].direction());
      r.normalize();
      const WbShape *const shape = WbNodeUtilities::findIntersectingShape(WbRay(trans, r), lutMaxRange, distance);

      if (shape) {
        mRays[i].setDistance(distance);

        WbRgb pickedColor;
        double roughness, occlusion;
        shape->pickColor(WbRay(trans, r), pickedColor, &roughness, &occlusion);

        const double infraRedFactor = 0.8 * pickedColor.red() * (1 - 0.5 * roughness) * (1 - 0.5 * occlusion) + 0.2;
        averageInfraRedFactor += infraRedFactor * mRays[i].weight();
      } else
        averageInfraRedFactor += mRays[i].weight();

      mDistance += distance * mRays[i].weight();
    }

    // apply infrared reflection factor and red color sensitivity
    // before adding of red color sensitivity factor it was calculated with mDistance = mDistance / averageInfraRedFactor
    mDistance = mDistance + (mDistance / averageInfraRedFactor - mDistance) * mRedColorSensitivity->value();
  } else if (mRayType == SONAR) {
    // use only the nearest ray collision, ignore ray weight
    mDistance = lutMaxRange;
    for (int i = 0; i < mNRays; ++i)
      if (mRays[i].collidedGeometry() && mRays[i].distance() < mDistance) {
        // compute angle between ray and contact normal
        dVector3 start, dir;
        dGeomRayGet(mRays[i].geom(), start, dir);
        WbVector3 direction(dir);                    // the ray direction is expressed in global coordinates
        WbVector3 normal(mRays[i].contactNormal());  // the contact normal is expressed in global coordinates
        const double angle = direction.angle(-normal);
        // ignore contact outside of a reflection cone with 45 degrees aperture (experimental value)
        if (angle < PI_OVER_8)
          mDistance = mRays[i].distance();
      }
  } else if (mRayType == LASER)
    // consider only one ray (there should be only one)
    mDistance = (mRays[0].collidedGeometry()) ? mRays[0].distance() : lutMaxRange;

  mValue = mLut->lookup(mDistance / s);
  if (mResolution->value() != -1.0)
    mValue = WbMathsUtilities::discretize(mValue, mResolution->value());

  applyOptionalRenderingToWren();
}

void WbDistanceSensor::postPhysicsStep() {
  WbSolidDevice::postPhysicsStep();
  if (areWrenObjectsInitialized())
    applyLaserBeamToWren();
}

void WbDistanceSensor::rayCollisionCallback(WbGeometry *object, dGeomID rayGeom, const dContactGeom *contact) {
  if (!mSensor->isEnabled())
    return;

  for (int i = 0; i < mNRays; i++)
    if (rayGeom == mRays[i].geom()) {
      if (mRayType == GENERIC)
        mRays[i].setCollision(object, contact->depth);
      else  // SONAR and LASER
        mRays[i].setCollision(object, contact);
      return;
    }

  assert(0);  // should never be reached
}

void WbDistanceSensor::handleMessage(QDataStream &stream) {
  unsigned char command;
  short refreshRate;
  stream >> command;

  switch (command) {
    case C_SET_SAMPLING_PERIOD:
      stream >> refreshRate;
      mSensor->setRefreshRate(refreshRate);
      if (refreshRate == 0) {  // sensor disabled
        // update rays appearance
        applyOptionalRenderingToWren();
      }
      break;
    default:
      assert(0);
  }
}

void WbDistanceSensor::writeAnswer(QDataStream &stream) {
  if (refreshSensorIfNeeded() || mSensor->hasPendingValue()) {
    stream << tag();
    stream << (unsigned char)C_DISTANCE_SENSOR_DATA;
    stream << mValue;

    mSensor->resetPendingValue();
  }

  if (mNeedToReconfigure)
    addConfigure(stream);
}

void WbDistanceSensor::addConfigure(QDataStream &stream) {
  stream << (short unsigned int)tag();
  stream << (unsigned char)C_CONFIGURE;
  stream << (int)mRayType;
  stream << (double)mLut->minValue();
  stream << (double)mLut->maxValue();
  stream << (double)mAperture->value();
  stream << (int)mLookupTable->size();
  for (int i = 0; i < mLookupTable->size(); i++) {
    stream << (double)mLookupTable->item(i).x();
    stream << (double)mLookupTable->item(i).y();
    stream << (double)mLookupTable->item(i).z();
  }
  mNeedToReconfigure = false;
}

void WbDistanceSensor::writeConfigure(QDataStream &stream) {
  mSensor->connectToRobotSignal(robot());
  addConfigure(stream);
}

void WbDistanceSensor::createWrenObjects() {
  // Optional rendering
  mMaterial = wr_phong_material_new();
  wr_phong_material_set_color_per_vertex(mMaterial, true);
  wr_material_set_default_program(mMaterial, WbWrenShaders::lineSetShader());
  mMesh = wr_dynamic_mesh_new(false, false, true);
  mRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mRenderable, false);
  wr_renderable_set_receive_shadows(mRenderable, false);
  wr_renderable_set_mesh(mRenderable, WR_MESH(mMesh));
  wr_renderable_set_visibility_flags(mRenderable, WbWrenRenderingContext::VF_DISTANCE_SENSORS_RAYS);
  wr_renderable_set_drawing_mode(mRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_material(mRenderable, mMaterial, NULL);
  mTransform = wr_transform_new();
  wr_transform_attach_child(mTransform, WR_NODE(mRenderable));

  // Laser beam
  const float vertices[36] = {0.0f,    0.0f,    0.0f, 0.0f,    1.0f,    0.0f, -0.588f, 0.809f,  0.0f, -0.951f, 0.309f,  0.0f,
                              -0.951f, -0.309f, 0.0f, -0.588f, -0.809f, 0.0f, 0.000f,  -1.000f, 0.0f, 0.588f,  -0.809f, 0.0f,
                              0.951f,  -0.309f, 0.0f, 0.951f,  0.309f,  0.0f, 0.588f,  0.809f,  0.0f, 0.0f,    1.0f,    0.0f};
  const float normals[36] = {0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                             0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                             0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f};
  const float textureCoordinates[24] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  const unsigned int indices[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  mLaserBeamMesh = wr_static_mesh_new(12, 12, vertices, normals, textureCoordinates, textureCoordinates, indices, false);

  const float red[3] = {1.0f, 0.0f, 0.0f};
  mLaserBeamMaterial = wr_phong_material_new();
  wr_phong_material_set_color(mLaserBeamMaterial, red);
  wr_phong_material_set_emissive(mLaserBeamMaterial, red);
  wr_material_set_default_program(mLaserBeamMaterial, WbWrenShaders::phongShader());

  mLaserBeamRenderable = wr_renderable_new();
  wr_renderable_set_material(mLaserBeamRenderable, mLaserBeamMaterial, NULL);
  wr_renderable_set_mesh(mLaserBeamRenderable, WR_MESH(mLaserBeamMesh));
  wr_renderable_set_drawing_mode(mLaserBeamRenderable, WR_RENDERABLE_DRAWING_MODE_TRIANGLE_FAN);
  wr_renderable_set_visibility_flags(mLaserBeamRenderable, WbWrenRenderingContext::VF_LASER_BEAM);
  wr_renderable_set_cast_shadows(mLaserBeamRenderable, false);
  wr_renderable_set_receive_shadows(mLaserBeamRenderable, false);

  mLaserBeamTransform = wr_transform_new();
  wr_transform_attach_child(mLaserBeamTransform, WR_NODE(mLaserBeamRenderable));

  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this,
          &WbDistanceSensor::updateLineScale);
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
          &WbDistanceSensor::updateOptionalRendering);

  wr_node_set_visible(WR_NODE(mLaserBeamTransform), mRayType == LASER);

  if (!WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_DISTANCE_SENSORS_RAYS))
    wr_node_set_visible(WR_NODE(mTransform), false);

  WbSolidDevice::createWrenObjects();
  WrTransform *root = wr_scene_get_root(wr_scene_get_instance());
  wr_transform_attach_child(root, WR_NODE(mLaserBeamTransform));
  wr_transform_attach_child(wrenNode(), WR_NODE(mTransform));

  applyOptionalRenderingToWren();
  applyLaserBeamToWren();
}

void WbDistanceSensor::updateOptionalRendering(int option) {
  if (option == WbWrenRenderingContext::VF_DISTANCE_SENSORS_RAYS) {
    if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(option)) {
      wr_node_set_visible(WR_NODE(mTransform), true);
      applyOptionalRenderingToWren();
    } else
      wr_node_set_visible(WR_NODE(mTransform), false);
  }
}

void WbDistanceSensor::applyOptionalRenderingToWren() {
  if (!areWrenObjectsInitialized())
    return;

  wr_dynamic_mesh_clear(mMesh);

  if (mRays) {
    const float scale = absoluteScale().x();
    const float minValue = mLut->minMetricsRange();
    const float maxValue = mLut->maxMetricsRange();

    const float redColor[3] = {1.0f, 0.0f, 0.0f};
    const float greenColor[3] = {0.0f, 1.0f, 0.0f};
    const float greyColor[3] = {0.5f, 0.5f, 0.5f};
    const float orangeColor[3] = {1.0f, 0.5f, 0.0f};

    int vertexIndex = 0;
    float vertex[3];
    for (int i = 0; i < mNRays; ++i) {
      const WbVector3 &direction = mRays[i].direction();
      const float computedDistance = mDistance;
      const float actualDistance = mRays[i].distance();
      const float *color = mSensor->isEnabled() ? redColor : greyColor;

      // start with a red/grey line segment
      (direction * minValue / scale).toFloatArray(vertex);
      wr_dynamic_mesh_add_vertex(mMesh, vertex);
      wr_dynamic_mesh_add_index(mMesh, vertexIndex++);
      wr_dynamic_mesh_add_color(mMesh, color);

      if (mSensor->isEnabled() && mSensor->isFirstValueReady()) {
        // if collision
        if (actualDistance < maxValue) {
          // draw a green line segment
          (direction * actualDistance).toFloatArray(vertex);

          wr_dynamic_mesh_add_vertex(mMesh, vertex);
          wr_dynamic_mesh_add_index(mMesh, vertexIndex++);
          wr_dynamic_mesh_add_color(mMesh, redColor);

          wr_dynamic_mesh_add_vertex(mMesh, vertex);
          wr_dynamic_mesh_add_index(mMesh, vertexIndex++);

          if (mRayType == INFRA_RED && computedDistance - actualDistance > 1e-6) {
            wr_dynamic_mesh_add_color(mMesh, orangeColor);
            // draw an orange line segment
            const float distance = computedDistance > maxValue ? maxValue : computedDistance;
            (direction * distance).toFloatArray(vertex);

            wr_dynamic_mesh_add_vertex(mMesh, vertex);
            wr_dynamic_mesh_add_index(mMesh, vertexIndex++);
            wr_dynamic_mesh_add_color(mMesh, orangeColor);

            wr_dynamic_mesh_add_vertex(mMesh, vertex);
            wr_dynamic_mesh_add_index(mMesh, vertexIndex++);
          }
          wr_dynamic_mesh_add_color(mMesh, greenColor);

          color = greenColor;
        }
      }

      // finish line segment
      (direction * maxValue / scale).toFloatArray(vertex);
      wr_dynamic_mesh_add_vertex(mMesh, vertex);
      wr_dynamic_mesh_add_index(mMesh, vertexIndex++);
      wr_dynamic_mesh_add_color(mMesh, color);
    }
  }
}

void WbDistanceSensor::updateLineScale() {
  if (areWrenObjectsInitialized()) {
    WbSolid::updateLineScale();
    applyLaserBeamToWren();
  }
}

void WbDistanceSensor::applyLaserBeamToWren() {
  if (mRayType == LASER && mAperture->value() > 0.0 && mRays[0].distance() < absoluteScale().x() * mLut->maxValue()) {
    const dReal *contactPosition = mRays[0].contactPosition();
    const dReal *contactNormal = mRays[0].contactNormal();

    const WbVector3 &trans = matrix().translation();
    const WbVector3 sensorPosition(trans.x(), trans.y(), trans.z());
    WbVector3 normal(contactNormal[0], contactNormal[1], contactNormal[2]);
    WbVector3 position(contactPosition[0], contactPosition[1], contactPosition[2]);

    // direction of laser source (global coords)
    const WbVector3 laser = sensorPosition - position;

    // angle between normal and laser beam
    double phi = normal.angle(laser);

    // sometimes the normal vector returned by ODE points towards the inside of the surface
    // we can figure this out thanks to the phi angle and fix it
    if (phi > M_PI_2) {
      phi = M_PI - phi;
      normal = -normal;
    }

    // incidence angle of laser beam on the surface must be more than 10 degrees
    if (phi < 1.3963) {
      // add an offset relative to the lineScale to avoid depth fighting
      position += laser.normalized() * wr_config_get_line_scale() / 200.0;
      float positionArray[3];
      position.toFloatArray(positionArray);
      wr_transform_set_position(mLaserBeamTransform, positionArray);

      const WbVector3 UNIT_Z = WbVector3(0.0, 0.0, 1.0);
      WbVector3 axis = UNIT_Z.cross(normal).normalized();
      float angle = static_cast<float>(UNIT_Z.angle(normal));
      if (axis.isNan()) {  // normal colinear to Z axis
        axis = WbVector3(0, 1, 0);
        if (normal[2] < 0)  // Flip beam
          angle = M_PI;
      }

      float orientation[4];
      orientation[0] = angle;
      axis.toFloatArray(&orientation[1]);
      wr_transform_set_orientation(mLaserBeamTransform, orientation);
      const float aperture = static_cast<float>(mAperture->value());
      const float scale[3] = {aperture, aperture, 1.0f};
      wr_transform_set_scale(mLaserBeamTransform, scale);
      wr_node_set_visible(WR_NODE(mLaserBeamTransform), true);
      return;
    }
  }
  wr_node_set_visible(WR_NODE(mLaserBeamTransform), false);
}

void WbDistanceSensor::propagateScale() {
  WbSolid::propagateScale();
  updateRaySetup();
}
