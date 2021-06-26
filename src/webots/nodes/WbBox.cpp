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

#include "WbBox.hpp"

#include "WbAffinePlane.hpp"
#include "WbBoundingSphere.hpp"
#include "WbField.hpp"
#include "WbFieldChecker.hpp"
#include "WbMatter.hpp"
#include "WbNodeUtilities.hpp"
#include "WbOdeGeomData.hpp"
#include "WbRay.hpp"
#include "WbResizeManipulator.hpp"
#include "WbSimulationState.hpp"
#include "WbTransform.hpp"
#include "WbWorld.hpp"
#include "WbWrenAbstractResizeManipulator.hpp"
#include "WbWrenRenderingContext.hpp"

#include <wren/config.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include <ode/ode.h>

void WbBox::init() {
  mSize = findSFVector3("size");
}

WbBox::WbBox(WbTokenizer *tokenizer) : WbGeometry("Box", tokenizer) {
  init();
  if (tokenizer == NULL)
    mSize->setValueNoSignal(0.1, 0.1, 0.1);
}

WbBox::WbBox(const WbBox &other) : WbGeometry(other) {
  init();
}

WbBox::WbBox(const WbNode &other) : WbGeometry(other) {
  init();
}

WbBox::~WbBox() {
  wr_static_mesh_delete(mWrenMesh);
}

void WbBox::postFinalize() {
  WbGeometry::postFinalize();

  connect(mSize, &WbSFVector3::changed, this, &WbBox::updateSize);
}

const WbVector3 &WbBox::size() const {
  return mSize->value();
}

void WbBox::createWrenObjects() {
  WbGeometry::createWrenObjects();
  WbGeometry::computeWrenRenderable();

  sanitizeFields();

  const bool createOutlineMesh = isInBoundingObject();
  mWrenMesh = wr_static_mesh_unit_box_new(createOutlineMesh);

  wr_renderable_set_mesh(mWrenRenderable, WR_MESH(mWrenMesh));

  updateSize();

  if (createOutlineMesh)
    connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this, &WbBox::updateLineScale);

  emit wrenObjectsCreated();
}

void WbBox::setResizeManipulatorDimensions() {
  WbVector3 scale = size().abs();
  WbTransform *transform = upperTransform();
  if (transform)
    scale *= transform->matrix().scale();

  if (isAValidBoundingObject())
    scale *= 1.0f + (wr_config_get_line_scale() / LINE_SCALE_FACTOR);

  resizeManipulator()->updateHandleScale(scale.ptr());
  updateResizeHandlesSize();
}

void WbBox::createResizeManipulator() {
  mResizeManipulator = new WbRegularResizeManipulator(uniqueId());
}

bool WbBox::areSizeFieldsVisibleAndNotRegenerator() const {
  const WbField *const size = findField("size", true);
  return WbNodeUtilities::isVisible(size) && !WbNodeUtilities::isTemplateRegeneratorField(size);
}

void WbBox::setSize(const WbVector3 &size) {
  mSize->setValue(size);
}

void WbBox::setSize(double x, double y, double z) {
  mSize->setValue(x, y, z);
}

void WbBox::setX(double x) {
  mSize->setX(x);
}

void WbBox::setY(double y) {
  mSize->setY(y);
}

void WbBox::setZ(double z) {
  mSize->setZ(z);
}

void WbBox::rescale(const WbVector3 &scale) {
  WbVector3 resizedSize = size();
  resizedSize *= scale;
  setSize(resizedSize);
}

bool WbBox::sanitizeFields() {
  if (WbFieldChecker::resetVector3IfNonPositive(this, mSize, WbVector3(1.0, 1.0, 1.0)))
    return false;

  return true;
}

void WbBox::updateSize() {
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

void WbBox::updateLineScale() {
  if (!isAValidBoundingObject())
    return;

  const float x = static_cast<float>(mSize->value().x());
  const float y = static_cast<float>(mSize->value().y());
  const float z = static_cast<float>(mSize->value().z());
  const float offset = std::min({x, y, z}) * wr_config_get_line_scale() / LINE_SCALE_FACTOR;
  const float scale[] = {x + offset, y + offset, z + offset};
  wr_transform_set_scale(wrenNode(), scale);
}

void WbBox::updateScale() {
  float scale[] = {static_cast<float>(mSize->value().x()), static_cast<float>(mSize->value().y()),
                   static_cast<float>(mSize->value().z())};
  wr_transform_set_scale(wrenNode(), scale);
}

/////////////////
// ODE objects //
/////////////////

void WbBox::checkFluidBoundingObjectOrientation() {
  const WbMatrix3 &m = upperTransform()->rotationMatrix();
  const WbVector3 &yAxis = m.column(1);
  const WbVector3 &g = WbWorld::instance()->worldInfo()->gravityVector();
  const double alpha = yAxis.angle(-g);

  static const double BOX_THRESHOLD = M_PI_2;

  if (fabs(alpha) >= BOX_THRESHOLD)
    parsingWarn(
      "The normal to the immersion plane defined by this Box has a large defect angle with the gravity vector."
      "This may yield unexpected behaviors when immersing solids. (Please consult the Reference Manual for the definition "
      "of immersion planes.)");
}

dGeomID WbBox::createOdeGeom(dSpaceID space) {
  const WbVector3 &size = mSize->value();
  if (size.x() <= 0.0 || size.y() <= 0.0 || size.z() <= 0.0) {
    parsingWarn(tr("'size' must be positive: construction of the Box in 'boundingObject' failed."));
    return NULL;
  }

  const WbVector3 s = scaledSize();
  return dCreateBox(space, s.x(), s.y(), s.z());
}

void WbBox::applyToOdeData(bool correctSolidMass) {
  if (mOdeGeom == NULL)
    return;

  WbVector3 size = mSize->value();
  size *= absoluteScale().abs();
  assert(dGeomGetClass(mOdeGeom) == dBoxClass);
  dGeomBoxSetLengths(mOdeGeom, size.x(), size.y(), size.z());

  WbOdeGeomData *const odeGeomData = static_cast<WbOdeGeomData *>(dGeomGetData(mOdeGeom));
  assert(odeGeomData);
  odeGeomData->setLastChangeTime(WbSimulationState::instance()->time());

  if (correctSolidMass)
    applyToOdeMass();
}

const WbVector3 WbBox::scaledSize() const {
  return (mSize->value() * absoluteScale()).abs();
}

bool WbBox::isSuitableForInsertionInBoundingObject(bool warning) const {
  const bool invalidDimensions = (mSize->x() <= 0.0 || mSize->y() <= 0.0 || mSize->z() <= 0.0);
  if (warning && invalidDimensions)
    parsingWarn(tr("All 'size' components must be positive for a Box used in a 'boundingObject'."));

  return !invalidDimensions;
}

bool WbBox::isAValidBoundingObject(bool checkOde, bool warning) const {
  const bool admissible = WbGeometry::isAValidBoundingObject(checkOde, warning);
  return admissible && isSuitableForInsertionInBoundingObject(admissible && warning);
}

/////////////////
// Ray Tracing //
/////////////////
enum IntersectedFace { FRONT_FACE, BACK_FACE, LEFT_FACE, RIGHT_FACE, TOP_FACE, BOTTOM_FACE };

int WbBox::findIntersectedFace(const WbVector3 &minBound, const WbVector3 &maxBound, const WbVector3 &intersectionPoint) {
  static const double TOLERANCE = 1e-9;

  // determine intersected face
  if (fabs(intersectionPoint.x() - maxBound.x()) < TOLERANCE)
    return RIGHT_FACE;
  else if (fabs(intersectionPoint.x() - minBound.x()) < TOLERANCE)
    return LEFT_FACE;
  else if (fabs(intersectionPoint.z() - minBound.z()) < TOLERANCE)
    return BACK_FACE;
  else if (fabs(intersectionPoint.z() - maxBound.z()) < TOLERANCE)
    return FRONT_FACE;
  else if (fabs(intersectionPoint.y() - maxBound.y()) < TOLERANCE)
    return TOP_FACE;
  else if (fabs(intersectionPoint.y() - minBound.y()) < TOLERANCE)
    return BOTTOM_FACE;

  return -1;
}

WbVector2 WbBox::computeTextureCoordinate(const WbVector3 &minBound, const WbVector3 &maxBound, const WbVector3 &point,
                                          bool nonRecursive, int intersectedFace) {
  double u, v;
  if (intersectedFace < 0)
    intersectedFace = findIntersectedFace(minBound, maxBound, point);

  WbVector3 vertex = point - minBound;
  WbVector3 size = maxBound - minBound;
  switch (intersectedFace) {
    case FRONT_FACE:
      u = vertex.x() / size.x();
      v = 1 - vertex.y() / size.y();
      if (nonRecursive) {
        u = 0.25 * u + 0.50;
        v = 0.50 * v + 0.50;
      }
      break;
    case BACK_FACE:
      u = 1 - vertex.x() / size.x();
      v = 1 - vertex.y() / size.y();
      if (nonRecursive) {
        u = 0.25 * u;
        v = 0.50 * v + 0.50;
      }
      break;
    case LEFT_FACE:
      u = vertex.z() / size.z();
      v = 1 - vertex.y() / size.y();
      if (nonRecursive) {
        u = 0.25 * u + 0.25;
        v = 0.50 * v + 0.50;
      }
      break;
    case RIGHT_FACE:
      u = 1 - vertex.z() / size.z();
      v = 1 - vertex.y() / size.y();
      if (nonRecursive) {
        u = 0.25 * u + 0.75;
        v = 0.50 * v + 0.50;
      }
      break;
    case TOP_FACE:
      u = vertex.x() / size.x();
      v = vertex.z() / size.z();
      if (nonRecursive) {
        u = 0.25 * u + 0.50;
        v = 0.50 * v;
      }
      break;
    case BOTTOM_FACE:
      u = vertex.x() / size.x();
      v = 1 - vertex.z() / size.z();
      if (nonRecursive) {
        u = 0.25 * u;
        v = 0.50 * v;
      }
      break;
    default:
      v = 0;
      u = 0;
      assert(false);
      break;
  }

  return WbVector2(u, v);
}

bool WbBox::pickUVCoordinate(WbVector2 &uv, const WbRay &ray, int textureCoordSet) const {
  WbVector3 localCollisionPoint;
  int faceIndex;
  double collisionDistance = computeLocalCollisionPoint(localCollisionPoint, faceIndex, ray);
  if (collisionDistance < 0)
    // no valid collision
    return false;

  // transform point into texture coordinates in range [0..1]
  WbVector3 halfSize = scaledSize() * 0.5;
  uv = computeTextureCoordinate(-halfSize, +halfSize, localCollisionPoint, textureCoordSet == 1);
  return true;
}

double WbBox::computeDistance(const WbRay &ray) const {
  WbVector3 collisionPoint;
  int faceIndex;
  return computeLocalCollisionPoint(collisionPoint, faceIndex, ray);
}

double WbBox::computeLocalCollisionPoint(WbVector3 &point, int &faceIndex, const WbRay &ray) const {
  WbRay localRay(ray);
  WbTransform *transform = upperTransform();
  if (transform) {
    localRay.setDirection(ray.direction() * transform->matrix());
    WbVector3 origin = transform->matrix().pseudoInversed(ray.origin());
    origin /= absoluteScale();
    localRay.setOrigin(origin);
    localRay.normalize();
  }

  WbVector3 halfSize = scaledSize() * 0.5;
  double tmin, tmax;
  std::pair<bool, double> result = localRay.intersects(-halfSize, halfSize, tmin, tmax);

  if (result.first && tmin >= 0) {
    // collision detected and ray origin not inside the box
    point = localRay.origin() + result.second * localRay.direction();
    return result.second;
  }

  return -1;
}

void WbBox::recomputeBoundingSphere() const {
  assert(mBoundingSphere);
  mBoundingSphere->set(WbVector3(), mSize->value().length() / 2.0);
}

////////////////////////
// Friction Direction //
////////////////////////

WbVector3 WbBox::computeFrictionDirection(const WbVector3 &normal) const {
  WbVector3 localNormal = normal * matrix().extracted3x3Matrix();
  // Find most probable face and return first friction direction in the local coordinate system
  if ((fabs(localNormal[1]) > fabs(localNormal[0])) && (fabs(localNormal[1]) > fabs(localNormal[2])))
    return WbVector3(0, 0, 1);
  else
    return WbVector3(0, 1, 0);
}
