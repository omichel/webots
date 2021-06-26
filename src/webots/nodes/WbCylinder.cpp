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

#include "WbCylinder.hpp"

#include "WbAffinePlane.hpp"
#include "WbBoundingSphere.hpp"
#include "WbField.hpp"
#include "WbFieldChecker.hpp"
#include "WbMatter.hpp"
#include "WbNodeUtilities.hpp"
#include "WbOdeGeomData.hpp"
#include "WbRay.hpp"
#include "WbResizeManipulator.hpp"
#include "WbSFBool.hpp"
#include "WbSFInt.hpp"
#include "WbSimulationState.hpp"
#include "WbTransform.hpp"
#include "WbWrenRenderingContext.hpp"

#include <wren/config.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include <ode/ode.h>

#include <cmath>
#include <limits>

void WbCylinder::init() {
  // rotate cylinder by 90 degrees around the x-axis because ODE cylinders
  // are z-aligned but Webots needs the Cylinders to be y-aligned
  mIs90DegreesRotated = true;

  mBottom = findSFBool("bottom");
  mRadius = findSFDouble("radius");
  mHeight = findSFDouble("height");

  mSide = findSFBool("side");
  mTop = findSFBool("top");
  mSubdivision = findSFInt("subdivision");

  mResizeConstraint = WbWrenAbstractResizeManipulator::X_EQUAL_Z;
}

WbCylinder::WbCylinder(WbTokenizer *tokenizer) : WbGeometry("Cylinder", tokenizer) {
  init();
  if (tokenizer == NULL) {
    mRadius->setValueNoSignal(0.05);
    mHeight->setValueNoSignal(0.1);
  }
}

WbCylinder::WbCylinder(const WbCylinder &other) : WbGeometry(other) {
  init();
}

WbCylinder::WbCylinder(const WbNode &other) : WbGeometry(other) {
  init();
}

WbCylinder::~WbCylinder() {
  wr_static_mesh_delete(mWrenMesh);
}

void WbCylinder::postFinalize() {
  WbGeometry::postFinalize();

  connect(mBottom, &WbSFBool::changed, this, &WbCylinder::updateBottom);
  connect(mRadius, &WbSFDouble::changed, this, &WbCylinder::updateRadius);
  connect(mHeight, &WbSFDouble::changed, this, &WbCylinder::updateHeight);
  connect(mSide, &WbSFBool::changed, this, &WbCylinder::updateSide);
  connect(mTop, &WbSFBool::changed, this, &WbCylinder::updateTop);
  connect(mSubdivision, &WbSFInt::changed, this, &WbCylinder::updateSubdivision);
}

void WbCylinder::createWrenObjects() {
  WbGeometry::createWrenObjects();

  if (isInBoundingObject()) {
    connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this, &WbCylinder::updateLineScale);

    if (mSubdivision->value() < MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION && !WbNodeUtilities::hasAUseNodeAncestor(this))
      // silently reset the subdivision on node initialization
      mSubdivision->setValue(MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION);
  }
  sanitizeFields();
  buildWrenMesh();

  emit wrenObjectsCreated();
}

void WbCylinder::setResizeManipulatorDimensions() {
  WbVector3 scale(mRadius->value(), mHeight->value(), mRadius->value());

  WbTransform *transform = upperTransform();
  if (transform)
    scale *= transform->matrix().scale();

  if (isAValidBoundingObject())
    scale *= 1.0f + (wr_config_get_line_scale() / LINE_SCALE_FACTOR);

  resizeManipulator()->updateHandleScale(scale.ptr());
  updateResizeHandlesSize();
}

void WbCylinder::createResizeManipulator() {
  mResizeManipulator =
    new WbRegularResizeManipulator(uniqueId(), (WbWrenAbstractResizeManipulator::ResizeConstraint)mResizeConstraint);
}

bool WbCylinder::areSizeFieldsVisibleAndNotRegenerator() const {
  const WbField *const height = findField("height", true);
  const WbField *const radius = findField("radius", true);
  return WbNodeUtilities::isVisible(height) && WbNodeUtilities::isVisible(radius) &&
         !WbNodeUtilities::isTemplateRegeneratorField(height) && !WbNodeUtilities::isTemplateRegeneratorField(radius);
}

void WbCylinder::exportNodeFields(WbVrmlWriter &writer) const {
  WbGeometry::exportNodeFields(writer);
  if (writer.isX3d())
    writer << " subdivision=\'" << mSubdivision->value() << "\'";
}

bool WbCylinder::sanitizeFields() {
  if (WbFieldChecker::resetIntIfNotInRangeWithIncludedBounds(this, mSubdivision, 3, 1000, 3))
    return false;
  if (mSubdivision->value() < MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION && isInBoundingObject() &&
      !WbNodeUtilities::hasAUseNodeAncestor(this)) {
    parsingWarn(tr("'subdivision' value has no effect to physical 'boundingObject' geometry. "
                   "A minimum value of %2 is used for the representation.")
                  .arg(MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION));
    mSubdivision->setValue(MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION);
    return false;
  }

  if (WbFieldChecker::resetDoubleIfNonPositive(this, mRadius, 1.0))
    return false;

  if (WbFieldChecker::resetDoubleIfNonPositive(this, mHeight, 1.0))
    return false;

  return true;
}

void WbCylinder::buildWrenMesh() {
  WbGeometry::deleteWrenRenderable();

  wr_static_mesh_delete(mWrenMesh);
  mWrenMesh = NULL;

  if (mBottom->isFalse() && mSide->isFalse() && mTop->isFalse())
    return;

  const bool createOutlineMesh = isInBoundingObject();
  const int subdivision = (createOutlineMesh && mSubdivision->value() < MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION) ?
                            MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION :
                            mSubdivision->value();

  WbGeometry::computeWrenRenderable();

  mWrenMesh =
    wr_static_mesh_unit_cylinder_new(subdivision, mSide->isTrue(), mTop->isTrue(), mBottom->isTrue(), createOutlineMesh);

  // This must be done after WbGeometry::computeWrenRenderable() otherwise
  // the outline scaling is applied to the wrong WREN transform
  if (createOutlineMesh)
    updateLineScale();
  else
    updateScale();

  wr_renderable_set_mesh(mWrenRenderable, WR_MESH(mWrenMesh));
}

void WbCylinder::rescale(const WbVector3 &scale) {
  if (scale.x() != 1.0)
    setRadius(radius() * scale.x());
  else if (scale.z() != 1.0)
    setRadius(radius() * scale.z());

  if (scale.y() != 1.0)
    setHeight(height() * scale.y());
}

void WbCylinder::updateBottom() {
  if (!sanitizeFields())
    return;

  buildWrenMesh();

  if (mBoundingSphere && !isInBoundingObject())
    mBoundingSphere->setOwnerSizeChanged();

  if (resizeManipulator() && resizeManipulator()->isAttached())
    setResizeManipulatorDimensions();

  emit changed();
}

void WbCylinder::updateRadius() {
  if (!sanitizeFields())
    return;

  if (isInBoundingObject())
    updateLineScale();
  else
    updateScale();

  if (isAValidBoundingObject())
    applyToOdeData();

  if (mBoundingSphere && !isInBoundingObject())
    mBoundingSphere->setOwnerSizeChanged();

  if (resizeManipulator() && resizeManipulator()->isAttached())
    setResizeManipulatorDimensions();

  emit changed();
}

void WbCylinder::updateHeight() {
  if (sanitizeFields()) {
    if (isInBoundingObject())
      updateLineScale();
    else
      updateScale();
  }

  if (isAValidBoundingObject())
    applyToOdeData();

  if (mBoundingSphere && !isInBoundingObject())
    mBoundingSphere->setOwnerSizeChanged();

  if (resizeManipulator() && resizeManipulator()->isAttached())
    setResizeManipulatorDimensions();

  emit changed();
}

void WbCylinder::updateSide() {
  if (!sanitizeFields())
    return;

  buildWrenMesh();

  if (mBoundingSphere && !isInBoundingObject())
    mBoundingSphere->setOwnerSizeChanged();

  if (resizeManipulator() && resizeManipulator()->isAttached())
    setResizeManipulatorDimensions();

  emit changed();
}

void WbCylinder::updateTop() {
  if (!sanitizeFields())
    return;

  buildWrenMesh();

  if (mBoundingSphere && !isInBoundingObject())
    mBoundingSphere->setOwnerSizeChanged();

  if (resizeManipulator() && resizeManipulator()->isAttached())
    setResizeManipulatorDimensions();

  emit changed();
}

void WbCylinder::updateSubdivision() {
  if (!sanitizeFields())
    return;

  buildWrenMesh();

  emit changed();
}

void WbCylinder::updateLineScale() {
  if (!isAValidBoundingObject())
    return;

  float offset = wr_config_get_line_scale() / LINE_SCALE_FACTOR;

  float scale[] = {static_cast<float>(mRadius->value() * (1.0f + offset)),
                   static_cast<float>(mHeight->value() * (1.0f + offset)),
                   static_cast<float>(mRadius->value() * (1.0f + offset))};
  wr_transform_set_scale(wrenNode(), scale);
}

void WbCylinder::updateScale() {
  float scale[] = {static_cast<float>(mRadius->value()), static_cast<float>(mHeight->value()),
                   static_cast<float>(mRadius->value())};
  wr_transform_set_scale(wrenNode(), scale);
}

/////////////////
// ODE Objects //
/////////////////

dGeomID WbCylinder::createOdeGeom(dSpaceID space) {
  if (mRadius->value() <= 0.0) {
    parsingWarn(tr("'radius' must be positive when used in a 'boundingObject'."));
    return NULL;
  }

  if (mHeight->value() <= 0.0) {
    parsingWarn(tr("'height' must be positive when used in a 'boundingObject'."));
    return NULL;
  }

  if (WbNodeUtilities::findUpperMatter(this)->nodeType() == WB_NODE_FLUID)
    checkFluidBoundingObjectOrientation();

  return dCreateCylinder(space, scaledRadius(), scaledHeight());
}

void WbCylinder::applyToOdeData(bool correctSolidMass) {
  if (mOdeGeom == NULL)
    return;

  assert(dGeomGetClass(mOdeGeom) == dCylinderClass);
  dGeomCylinderSetParams(mOdeGeom, scaledRadius(), scaledHeight());

  WbOdeGeomData *const odeGeomData = static_cast<WbOdeGeomData *>(dGeomGetData(mOdeGeom));
  assert(odeGeomData);
  odeGeomData->setLastChangeTime(WbSimulationState::instance()->time());

  if (correctSolidMass)
    applyToOdeMass();
}

double WbCylinder::scaledRadius() const {
  const WbVector3 &scale = absoluteScale();
  return fabs(mRadius->value() * std::max(scale.x(), scale.z()));
}

double WbCylinder::scaledHeight() const {
  return fabs(mHeight->value() * absoluteScale().y());
}

bool WbCylinder::isSuitableForInsertionInBoundingObject(bool warning) const {
  const bool invalidRadius = mRadius->value() <= 0.0;
  const bool invalidHeight = mHeight->value() <= 0.0;
  if (warning) {
    if (invalidRadius)
      parsingWarn(tr("'radius' must be positive when used in a 'boundingObject'."));

    if (invalidHeight)
      parsingWarn(tr("'height' must be positive when used in a 'boundingObject'."));
  }

  return (!invalidHeight && !invalidRadius);
}

bool WbCylinder::isAValidBoundingObject(bool checkOde, bool warning) const {
  const bool admissible = WbGeometry::isAValidBoundingObject(checkOde, warning);
  return admissible && isSuitableForInsertionInBoundingObject(admissible && warning);
}
/////////////////
// Ray Tracing //
/////////////////

bool WbCylinder::pickUVCoordinate(WbVector2 &uv, const WbRay &ray, int textureCoordSet) const {
  WbVector3 collisionPoint;
  int faceIndex;
  double collisionDistance = computeLocalCollisionPoint(collisionPoint, faceIndex, ray);
  if (collisionDistance < 0)
    return false;

  double h = scaledHeight();
  double r = scaledRadius();

  double u, v;
  if (faceIndex > 0) {
    // top face or bottom face
    if (collisionPoint.x() * collisionPoint.x() + collisionPoint.z() * collisionPoint.z() > r * r)
      return false;

    u = (collisionPoint.x() + r) / (2 * r);
    v = (collisionPoint.z() + r) / (2 * r);

    if (collisionPoint.y() < 0) {
      v = 1 - v;
    }

    if (textureCoordSet == 1) {
      u = u * 0.5;
      v = v * 0.5;
      if (faceIndex == 1)  // TOP
        u += 0.5;
      else  // BOTTOM
        v += 0.5;
    }
  } else {
    // body
    double theta = asin(-collisionPoint.x() / r);
    assert(!std::isnan(theta));
    if (collisionPoint.z() > 0)
      theta = M_PI - theta;

    theta = theta - floor(theta / (2 * M_PI)) * 2 * M_PI;
    u = theta / (2 * M_PI);
    v = 1 - (collisionPoint.y() + h / 2) / h;

    if (textureCoordSet == 1) {
      u = u * 0.5;
      v = v * 0.5;
    }
  }

  uv.setXy(u, v);
  return true;
}

double WbCylinder::computeDistance(const WbRay &ray) const {
  WbVector3 collisionPoint;
  int faceIndex;
  return computeLocalCollisionPoint(collisionPoint, faceIndex, ray);
}

double WbCylinder::computeLocalCollisionPoint(WbVector3 &point, int &faceIndex, const WbRay &ray) const {
  WbVector3 direction(ray.direction());
  WbVector3 origin(ray.origin());
  WbTransform *transform = upperTransform();
  if (transform) {
    direction = ray.direction() * transform->matrix();
    direction.normalize();
    origin = transform->matrix().pseudoInversed(ray.origin());
    origin /= absoluteScale();
  }

  double radius = scaledRadius();
  double radius2 = radius * radius;
  double h = scaledHeight();
  double d = std::numeric_limits<double>::infinity();
  faceIndex = -1;

  // distance from body
  if (mSide->value()) {
    double a = direction.x() * direction.x() + direction.z() * direction.z();
    double b = 2 * (origin.x() * direction.x() + origin.z() * direction.z());
    double c = origin.x() * origin.x() + origin.z() * origin.z() - radius2;
    double discriminant = b * b - 4 * a * c;

    // if c < 0: ray origin is inside cylinder body
    if (c >= 0 && discriminant > 0) {
      // ray intersects the sphere in two points
      discriminant = sqrt(discriminant);
      double t1 = (-b - discriminant) / (2 * a);
      double t2 = (-b + discriminant) / (2 * a);
      double y1 = origin.y() + t1 * direction.y();
      double y2 = origin.y() + t2 * direction.y();
      if (mSide->value() && t1 > 0 && y1 >= -h / 2 && y1 <= h / 2) {
        d = t1;
        faceIndex = 0;
      } else if (mSide->value() && t2 > 0 && y2 >= -h / 2 && y2 <= h / 2) {
        d = t2;
        faceIndex = 0;
      }
    }
  }

  // distance from top face
  if (mTop->value()) {
    std::pair<bool, double> intersection =
      WbRay(origin, direction).intersects(WbAffinePlane(WbVector3(0, 1, 0), WbVector3(0, h / 2, 0)), true);
    if (mTop->value() && intersection.first && intersection.second > 0 && intersection.second < d) {
      WbVector3 p = origin + intersection.second * direction;
      if (p.x() * p.x() + p.z() * p.z() <= radius2) {
        d = intersection.second;
        faceIndex = 1;
      }
    }
  }

  // distance from bottom face
  if (mBottom->value()) {
    std::pair<bool, double> intersection =
      WbRay(origin, direction).intersects(WbAffinePlane(WbVector3(0, -1, 0), WbVector3(0, -h / 2, 0)), true);
    if (mBottom->value() && intersection.first && intersection.second > 0 && intersection.second < d) {
      WbVector3 p = origin + intersection.second * direction;
      if (p.x() * p.x() + p.z() * p.z() <= radius2) {
        d = intersection.second;
        faceIndex = 2;
      }
    }
  }

  if (d == std::numeric_limits<double>::infinity())
    return -1;

  point = origin + d * direction;
  return d;
}

void WbCylinder::recomputeBoundingSphere() const {
  assert(mBoundingSphere);
  const bool top = mTop->value();
  const bool side = mSide->value();
  const bool bottom = mBottom->value();
  const double halfHeight = scaledHeight() / 2.0;
  const double radius = scaledRadius();

  if ((top + side + bottom) == 0)  // it is empty
    mBoundingSphere->empty();
  else if ((top + side + bottom) == 1 && !side) {  // just one disk
    const double center = top ? halfHeight : -halfHeight;
    mBoundingSphere->set(WbVector3(0, center, 0), radius);
  } else
    mBoundingSphere->set(WbVector3(), WbVector3(radius, halfHeight, 0).length());
}

// if a cylinder has nothing to draw, then it shouldn't be exported to X3D
bool WbCylinder::shallExport() const {
  return mBottom->value() || mTop->value() || mSide->value();
}

////////////////////////
// Friction Direction //
////////////////////////

WbVector3 WbCylinder::computeFrictionDirection(const WbVector3 &normal) const {
  WbVector3 localNormal = normal * matrix().extracted3x3Matrix();
  // Find most probable face and return first friction direction in the local coordinate system
  if ((fabs(localNormal[1]) > fabs(localNormal[0])) && (fabs(localNormal[1]) > fabs(localNormal[2])))  // top or bottom face
    return WbVector3(1, 0, 0);
  else  // side
    return WbVector3(0, 1, 0);
}
