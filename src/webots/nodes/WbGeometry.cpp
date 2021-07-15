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

#include "WbGeometry.hpp"

#include "WbBoundingSphere.hpp"
#include "WbFluid.hpp"
#include "WbNode.hpp"
#include "WbNodeUtilities.hpp"
#include "WbOdeContext.hpp"
#include "WbOdeGeomData.hpp"
#include "WbPrecision.hpp"
#include "WbResizeManipulator.hpp"
#include "WbSimulationState.hpp"
#include "WbSlot.hpp"
#include "WbSolid.hpp"
#include "WbVector4.hpp"
#include "WbWorld.hpp"
#include "WbWrenMeshBuffers.hpp"
#include "WbWrenOpenGlContext.hpp"
#include "WbWrenPicker.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/texture.h>
#include <wren/transform.h>

#include <ode/ode.h>

// Constant used to scale down the line scale property
const float WbGeometry::LINE_SCALE_FACTOR = 250.0f;

const int gMaxIndexNumberToCastShadows = (1 << 16) - 1;  // 2^16 - 1 (16-bit resolution)

int WbGeometry::maxIndexNumberToCastShadows() {
  return gMaxIndexNumberToCastShadows;
}

void WbGeometry::init() {
  mWrenMaterial = NULL;
  mWrenEncodeDepthMaterial = NULL;
  mWrenSegmentationMaterial = NULL;
  mWrenMesh = NULL;
  mWrenRenderable = NULL;
  mWrenScaleTransform = NULL;
  mCollisionTime = -std::numeric_limits<float>::infinity();
  mPreviousCollisionTime = -std::numeric_limits<float>::infinity();
  mOdeGeom = NULL;
  mIs90DegreesRotated = false;
  mOdeMass = NULL;
  mResizeManipulator = NULL;
  mResizeManipulatorInitialized = false;
  mResizeConstraint = WbWrenAbstractResizeManipulator::NO_CONSTRAINT;
  mBoundingSphere = NULL;
  mPickable = false;
  mIsTransparent = false;
}

WbGeometry::WbGeometry(const QString &modelName, WbTokenizer *tokenizer) : WbBaseNode(modelName, tokenizer) {
  init();
}

WbGeometry::WbGeometry(const WbGeometry &other) : WbBaseNode(other) {
  init();
}

WbGeometry::WbGeometry(const WbNode &other) : WbBaseNode(other) {
  init();
}

WbGeometry::~WbGeometry() {
  delete mResizeManipulator;
  if (mOdeGeom)
    destroyOdeObjects();  // for WbGeometries lying in a boundinObject
  delete mOdeMass;
  delete mBoundingSphere;

  destroyWrenObjects();
}

void WbGeometry::postFinalize() {
  WbBaseNode::postFinalize();

  mBoundingSphere = new WbBoundingSphere(this);
  recomputeBoundingSphere();

  if (isInBoundingObject())
    connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
            &WbGeometry::updateBoundingObjectVisibility);
  else {
    const WbSolid *solid = WbNodeUtilities::findUpperSolid(this);
    while (solid) {
      if (solid->recognitionColorSize() > 0) {
        setSegmentationColor(solid->recognitionColor(0));
        break;
      }
      solid = WbNodeUtilities::findUpperSolid(solid);
    }

    if (!solid)
      setSegmentationColor(WbRgb(0.0, 0.0, 0.0));
  }
}

void WbGeometry::destroyOdeObjects() {
  assert(mOdeGeom);

  WbOdeGeomData *const data = static_cast<WbOdeGeomData *>(dGeomGetData(mOdeGeom));
  delete data;

  dGeomDestroy(mOdeGeom);
  mOdeGeom = NULL;

  // Destroys all the physical objects attached to the geometry
  // if signal is emitted before destroying the ODE geom, then it could happen that an already
  // deleted Webots bounding object node is accessed during WbMatter::updateOdeGeomPosition
  emit boundingGeometryRemoved();
}

WbWrenMeshBuffers *WbGeometry::createMeshBuffers(int verticesCount, int indicesCount) const {
  if (verticesCount <= 0 || indicesCount <= 0)
    return NULL;

  return new WbWrenMeshBuffers(verticesCount, indicesCount, isInBoundingObject() ? 0 : 2, 0);
}

////////////////////////
// Create ODE Objects //
////////////////////////

void WbGeometry::createOdeObjects() {
  WbBaseNode::createOdeObjects();

  if (!mOdeMass) {
    mOdeMass = new dMass;
    dMassSetZero(mOdeMass);
  }
}

// Default creation method (overriden in every WbGeometry inherited class)
dGeomID WbGeometry::createOdeGeom(dSpaceID space) {
  parsingWarn(tr("This type of geometry node cannot be placed in 'boundingObject'."));
  return NULL;
}

void WbGeometry::checkFluidBoundingObjectOrientation() {
  const WbMatrix3 &m = upperTransform()->rotationMatrix();
  const WbVector3 &yAxis = m.column(1);
  const WbVector3 &g = WbWorld::instance()->worldInfo()->gravityVector();
  const double alpha = yAxis.angle(-g);

  static const double ZERO_THRESHOLD = 1e-3;

  if (fabs(alpha) > ZERO_THRESHOLD)
    parsingWarn(
      "The normal to this geometry's immersion plane is not opposed to the gravity vector. "
      "This may yield unexpected behaviors when immersing solids. (Please consult the Reference Manual for the definition "
      "of immersion planes.)");
}

/////////////////////////
// Create WREN Objects //
/////////////////////////

void WbGeometry::checkForResizeManipulator() {
  if (!mResizeManipulator && hasResizeManipulator()) {
    createResizeManipulator();
    if (mResizeManipulator)
      mResizeManipulator->attachTo(wrenNode());
  }
}

void WbGeometry::updateContextDependentObjects() {
  checkForResizeManipulator();
  WbBaseNode::updateContextDependentObjects();
}

void WbGeometry::setPickable(bool pickable) {
  if (!mWrenRenderable || isInBoundingObject())
    return;

  mPickable = pickable && isShadedGeometryPickable();
  WbWrenPicker::setPickable(mWrenRenderable, uniqueId(), pickable);
}

void WbGeometry::setSegmentationColor(const WbRgb &color) {
  if (!mWrenRenderable || !mWrenSegmentationMaterial || isInBoundingObject())
    return;

  const float segmentationColor[3] = {(float)color.red(), (float)color.green(), (float)color.blue()};
  wr_phong_material_set_linear_diffuse(mWrenSegmentationMaterial, segmentationColor);
}

///////////////////
// Apply Methods //
///////////////////

void WbGeometry::applyVisibilityFlagToWren(bool selected) {
  if (!mWrenScaleTransform)
    return;

  if (isInBoundingObject()) {
    if (selected) {
      wr_renderable_set_visibility_flags(mWrenRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
      wr_node_set_visible(WR_NODE(mWrenScaleTransform), true);
    } else if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(
                 WbWrenRenderingContext::VF_ALL_BOUNDING_OBJECTS)) {
      wr_renderable_set_visibility_flags(mWrenRenderable, WbWrenRenderingContext::VF_ALL_BOUNDING_OBJECTS);
      wr_node_set_visible(WR_NODE(mWrenScaleTransform), true);
    } else if (wr_node_get_parent(WR_NODE(mWrenScaleTransform)))
      wr_node_set_visible(WR_NODE(mWrenScaleTransform), false);
  } else if (mIsTransparent) {
    wr_renderable_set_visibility_flags(mWrenRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
    wr_node_set_visible(WR_NODE(mWrenScaleTransform), false);
  } else if (WbNodeUtilities::isDescendantOfBillboard(this)) {
    wr_renderable_set_visibility_flags(mWrenRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
    wr_node_set_visible(WR_NODE(mWrenScaleTransform), true);
  } else {
    wr_renderable_set_visibility_flags(mWrenRenderable, WbWrenRenderingContext::VM_REGULAR);
    wr_node_set_visible(WR_NODE(mWrenScaleTransform), true);
  }
}

void WbGeometry::updateBoundingObjectVisibility(int optionalRendering) {
  if (optionalRendering == WbWrenRenderingContext::VF_ALL_BOUNDING_OBJECTS)
    applyVisibilityFlagToWren(isSelected());
}

// Apply to ODE
void WbGeometry::applyToOdeMass() {
  if (mOdeGeom == NULL)
    return;

  const WbOdeGeomData *const odeGeomData = static_cast<WbOdeGeomData *>(dGeomGetData(mOdeGeom));
  assert(odeGeomData);
  if (mOdeMass->mass > 0.0) {
    WbSolid *const solid = odeGeomData->solid();
    solid->correctOdeMass(mOdeMass, transformedGeometry());
  }
}

////////////
// Others //
////////////

void WbGeometry::updateCollisionMaterial(bool triggerChange, bool onSelection) {
  if (!mWrenMaterial || !isInBoundingObject())
    return;

  bool isColliding = mCollisionTime >= WbSimulationState::instance()->time();
  if (onSelection && !isColliding)
    isColliding = mCollisionTime == WbSimulationState::instance()->time() - WbWorld::instance()->basicTimeStep();
  const bool wasColliding =
    mCollisionTime == WbSimulationState::instance()->time() - WbWorld::instance()->basicTimeStep() ||
    mPreviousCollisionTime >= WbSimulationState::instance()->time() - 2 * WbWorld::instance()->basicTimeStep();
  const bool changeBoundingObjectMaterial = isColliding != wasColliding || triggerChange;

  if (changeBoundingObjectMaterial) {
    if (isColliding) {
      const float outlineCollidedColor[3] = {0.8f, 0.4f, 0.4f};
      wr_phong_material_set_emissive(mWrenMaterial, outlineCollidedColor);
    } else {
      const float outlineColor[3] = {1.0f, 1.0f, 1.0f};
      wr_phong_material_set_emissive(mWrenMaterial, outlineColor);
    }
  }
}

void WbGeometry::setColliding() {
  if (mCollisionTime != WbSimulationState::instance()->time()) {
    mPreviousCollisionTime = mCollisionTime;
    mCollisionTime = WbSimulationState::instance()->time();
  }
}

void WbGeometry::setSleepMaterial() {
  if (!isInBoundingObject())
    return;
  const float sleepColor[3] = {0.4f, 0.4f, 0.8f};
  wr_phong_material_set_emissive(mWrenMaterial, sleepColor);
}

// Selection management
void WbGeometry::propagateSelection(bool selected) {
  applyVisibilityFlagToWren(selected);
}

bool WbGeometry::isSelected() {
  const WbMatter *const matter = WbNodeUtilities::findUpperMatter(this);
  if (matter)
    return matter->isSelected();
  return false;
}

//////////////
// Setters  //
//////////////

void WbGeometry::computeWrenRenderable() {
  // for USE nodes, it can happen that WbBaseNode::createWrenObjects hasn't been called when constructing their mesh
  if (!areWrenObjectsInitialized())
    WbBaseNode::createWrenObjects();

  assert(mWrenScaleTransform == NULL);
  assert(mWrenRenderable == NULL);

  mWrenScaleTransform = wr_transform_new();
  wr_transform_attach_child(wrenNode(), WR_NODE(mWrenScaleTransform));
  setWrenNode(mWrenScaleTransform);

  mWrenRenderable = wr_renderable_new();

  // since USE nodes aren't contained in a WbShape, no material will be created for them
  if (isInBoundingObject()) {
    if (!mWrenMaterial) {
      mWrenMaterial = wr_phong_material_new();
      wr_material_set_default_program(mWrenMaterial, WbWrenShaders::lineSetShader());
    }

    wr_renderable_set_cast_shadows(mWrenRenderable, false);
    wr_renderable_set_receive_shadows(mWrenRenderable, false);
    wr_renderable_set_drawing_mode(mWrenRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);

    setWrenMaterial(mWrenMaterial, false);
  }

  // used for rendering range finder camera
  if (!mWrenEncodeDepthMaterial) {
    mWrenEncodeDepthMaterial = wr_phong_material_new();
    wr_material_set_default_program(mWrenEncodeDepthMaterial, WbWrenShaders::encodeDepthShader());
  }
  wr_renderable_set_material(mWrenRenderable, mWrenEncodeDepthMaterial, "encodeDepth");

  // used for rendering segmentation camera
  if (!mWrenSegmentationMaterial) {
    mWrenSegmentationMaterial = wr_phong_material_new();
    wr_material_set_default_program(mWrenSegmentationMaterial, WbWrenShaders::segmentationShader());
  }
  wr_renderable_set_material(mWrenRenderable, mWrenSegmentationMaterial, "segmentation");

  wr_transform_attach_child(mWrenScaleTransform, WR_NODE(mWrenRenderable));

  applyVisibilityFlagToWren(isSelected());

  computeCastShadows(true);
}

void WbGeometry::deleteWrenRenderable() {
  if (mWrenRenderable) {
    if (mWrenMaterial)
      setWrenMaterial(NULL, false);

    // Delete outline material
    wr_material_delete(mWrenMaterial);
    mWrenMaterial = NULL;

    // Delete encode depth material
    wr_material_delete(mWrenEncodeDepthMaterial);
    mWrenEncodeDepthMaterial = NULL;

    // Delete camera segmentation material
    wr_material_delete(mWrenSegmentationMaterial);
    mWrenSegmentationMaterial = NULL;

    // Delete picking material
    wr_material_delete(wr_renderable_get_material(mWrenRenderable, "picking"));

    wr_node_delete(WR_NODE(mWrenRenderable));
    mWrenRenderable = NULL;

    setWrenNode(wr_node_get_parent(WR_NODE(mWrenScaleTransform)));
    wr_node_delete(WR_NODE(mWrenScaleTransform));
    mWrenScaleTransform = NULL;
  }
}

void WbGeometry::setWrenMaterial(WrMaterial *material, bool castShadows) {
  if (mWrenRenderable) {
    wr_renderable_set_material(mWrenRenderable, material, NULL);
    computeCastShadows(castShadows);
  }
}

void WbGeometry::setTransparent(bool isTransparent) {
  if (mIsTransparent != isTransparent) {
    mIsTransparent = isTransparent;
    applyVisibilityFlagToWren(isSelected());
  }
}

void WbGeometry::destroyWrenObjects() {
  if (areWrenObjectsInitialized()) {
    WbWrenOpenGlContext::makeWrenCurrent();
    deleteWrenRenderable();
    WbWrenOpenGlContext::doneWren();
  }
}

void WbGeometry::showResizeManipulator(bool enabled) {
  if (enabled)
    attachResizeManipulator();
  else
    detachResizeManipulator();

  emit visibleHandlesChanged(enabled);
}

void WbGeometry::updateResizeHandlesSize() {
  createResizeManipulatorIfNeeded();
  if (mResizeManipulator && !WbNodeUtilities::isNodeOrAncestorLocked(this))
    mResizeManipulator->computeHandleScaleFromViewportSize();
}

void WbGeometry::createResizeManipulatorIfNeeded() {
  if (!mResizeManipulatorInitialized) {
    mResizeManipulatorInitialized = true;
    checkForResizeManipulator();
  }
}

WbWrenAbstractResizeManipulator *WbGeometry::resizeManipulator() {
  createResizeManipulatorIfNeeded();
  return mResizeManipulator;
}

void WbGeometry::attachResizeManipulator() {
  createResizeManipulatorIfNeeded();
  if (mResizeManipulator && !mResizeManipulator->isAttached()) {
    setResizeManipulatorDimensions();
    mResizeManipulator->show();
  }
}

void WbGeometry::detachResizeManipulator() const {
  if (mResizeManipulator && mResizeManipulator->isAttached())
    mResizeManipulator->hide();
}

void WbGeometry::setUniformConstraintForResizeHandles(bool enabled) {
  createResizeManipulatorIfNeeded();
  if (!mResizeManipulator || !mResizeManipulator->isAttached())
    return;

  if (enabled)
    mResizeManipulator->setResizeConstraint(WbScaleManipulator::UNIFORM);
  else
    mResizeManipulator->setResizeConstraint((WbWrenAbstractResizeManipulator::ResizeConstraint)mResizeConstraint);
}

// ODE setters

void WbGeometry::setOdeMass(const dMass *mass) {
  if (mOdeMass)
    memcpy(mOdeMass, mass, sizeof(dMass));
}

void WbGeometry::setOdeData(dGeomID geom, WbMatter *matterAncestor) {
  assert(geom && matterAncestor);

  if (!areOdeObjectsCreated())
    createOdeObjects();

  mOdeGeom = geom;
  WbSolid *s = dynamic_cast<WbSolid *>(matterAncestor);
  if (s)
    dGeomSetData(geom, new WbOdeGeomData(s, this));
  else
    dGeomSetData(geom, new WbOdeGeomData(dynamic_cast<WbFluid *>(matterAncestor), this));
}

// Utility functions

WbBaseNode *WbGeometry::transformedGeometry() {  // returns an upper WbTransform lying in the same boundingObject if it does
                                                 // exist, otherwise the WbGeometry itself
  WbTransform *const ut = upperTransform();
  return ut->isInBoundingObject() ? static_cast<WbBaseNode *>(ut) : static_cast<WbBaseNode *>(this);
}

const WbVector3 WbGeometry::absoluteScale() const {
  const WbTransform *const ut = upperTransform();
  return ut ? ut->absoluteScale() : WbVector3(1.0, 1.0, 1.0);
}

WbVector3 WbGeometry::absolutePosition() const {
  const WbTransform *const ut = upperTransform();
  return ut ? ut->position() : WbVector3();
}

void WbGeometry::computeCastShadows(bool enabled) {
  if (!mWrenRenderable)
    return;

  if (isInBoundingObject() || WbNodeUtilities::isDescendantOfBillboard(this)) {
    wr_renderable_set_cast_shadows(mWrenRenderable, false);
    wr_renderable_set_receive_shadows(mWrenRenderable, false);
  } else
    wr_renderable_set_cast_shadows(mWrenRenderable, enabled);
}

void WbGeometry::setOdePosition(const WbVector3 &translation) {
  mOdePositionSet = translation;
  mOdeOffsetTranslation = translation + mOdeOffsetRotation * mLocalOdeGeomOffsetPosition;

  if (mOdeGeom == NULL)
    return;

  if (dGeomGetBody(mOdeGeom) == NULL)
    dGeomSetPosition(mOdeGeom, mOdeOffsetTranslation.x(), mOdeOffsetTranslation.y(), mOdeOffsetTranslation.z());
  else
    dGeomSetOffsetWorldPosition(mOdeGeom, mOdeOffsetTranslation.x(), mOdeOffsetTranslation.y(), mOdeOffsetTranslation.z());
}

void WbGeometry::setOdeRotation(const WbMatrix3 &rotation) {
  mOdeOffsetRotation = rotation;

  if (!mLocalOdeGeomOffsetPosition.isNull())
    // the position should be recomputed because the local offset is influenced by the global rotation
    setOdePosition(mOdePositionSet);

  if (mIs90DegreesRotated) {
    // append 90 deg rotation
    static const WbMatrix3 localRotation = WbRotation(1.0, 0.0, 0.0, M_PI_2).toMatrix3();
    mOdeOffsetRotation *= localRotation;
  }

  if (mOdeGeom == NULL)
    return;

  dMatrix3 m;
  m[0] = mOdeOffsetRotation(0, 0);
  m[1] = mOdeOffsetRotation(0, 1);
  m[2] = mOdeOffsetRotation(0, 2);
  m[4] = mOdeOffsetRotation(1, 0);
  m[5] = mOdeOffsetRotation(1, 1);
  m[6] = mOdeOffsetRotation(1, 2);
  m[8] = mOdeOffsetRotation(2, 0);
  m[9] = mOdeOffsetRotation(2, 1);
  m[10] = mOdeOffsetRotation(2, 2);

  if (dGeomGetBody(mOdeGeom) == NULL)
    dGeomSetRotation(mOdeGeom, m);
  else
    dGeomSetOffsetWorldRotation(mOdeGeom, m);
}

bool WbGeometry::isAValidBoundingObject(bool checkOde, bool warning) const {
  if (!isInBoundingObject())
    return false;

  const WbTransform *const ut = upperTransform();
  if (ut && ut->isInBoundingObject() && ut->geometry() != this)
    return false;

  if (checkOde && mOdeGeom == NULL)
    return false;

  return true;
}

int WbGeometry::triangleCount() const {
  if (areWrenObjectsInitialized() && this->wrenMesh())
    return wr_static_mesh_get_index_count(this->wrenMesh()) / 3;
  else
    return 0;
}

bool WbGeometry::exportNodeHeader(WbVrmlWriter &writer) const {
  if (writer.isUrdf())
    return true;
  return WbBaseNode::exportNodeHeader(writer);
}

////////////////////////////////
//  Position and orientation  //
////////////////////////////////

WbMatrix4 WbGeometry::matrix() const {
  const WbTransform *ut = upperTransform();
  if (!ut)
    return WbMatrix4();
  if (!ut->isInBoundingObject())
    return ut->matrix();
  else {
    const WbMatrix4 &matrix = ut->vrmlMatrix();
    ut = ut->upperTransform();
    return ut->matrix() * matrix;
  }
}

///////////////////////////////
// Scale Handles Constraints //
///////////////////////////////

int WbGeometry::constraintType() const {
  const int geometryType = nodeType();
  int constraint = WbWrenAbstractResizeManipulator::NO_CONSTRAINT;
  if (geometryType == WB_NODE_SPHERE || geometryType == WB_NODE_CAPSULE)
    constraint = WbWrenAbstractResizeManipulator::UNIFORM;
  else if (geometryType == WB_NODE_CYLINDER)
    constraint = WbWrenAbstractResizeManipulator::X_EQUAL_Z;

  return constraint;
}

////////////
// Export //
////////////

void WbGeometry::exportBoundingObjectToX3D(WbVrmlWriter &writer) const {
  assert(writer.isX3d());
  assert(isInBoundingObject());
  assert(mWrenMesh);

  const int vertexCount = wr_static_mesh_get_vertex_count(mWrenMesh);
  const int indexCount = wr_static_mesh_get_index_count(mWrenMesh);
  float vertices[3 * vertexCount];
  unsigned int indices[indexCount];
  wr_static_mesh_read_data(mWrenMesh, vertices, NULL, NULL, indices);

  writer << "<Appearance sortType='transparent'><Material emissiveColor='1 1 1'></Material></Appearance>";
  writer << "<IndexedLineSet coordIndex='";

  for (int i = 0; i < indexCount / 2; ++i)
    writer << indices[2 * i] << " " << indices[2 * i + 1] << " -1 ";
  writer << "'>";

  writer << "<Coordinate point='";
  const float *floatMatrix = wr_transform_get_matrix(mWrenScaleTransform);
  double doubleMatrix[16];
  for (int i = 0; i < 16; ++i)
    doubleMatrix[i] = static_cast<double>(floatMatrix[i]);

  // Extract WREN scaling factors
  WbMatrix4 meshMatrix;
  meshMatrix.fromOpenGlMatrix(doubleMatrix);
  WbVector3 scale = meshMatrix.scale();
  scale /= absoluteScale();

  for (int i = 0; i < vertexCount; ++i) {
    const int index = 3 * i;
    WbVector3 coord = WbVector3(vertices[index], vertices[index + 1], vertices[index + 2]) * scale;
    writer << coord.toString(WbPrecision::DOUBLE_MAX) << " ";
  }
  writer << "'></Coordinate>";

  writer << "</IndexedLineSet>";
}
