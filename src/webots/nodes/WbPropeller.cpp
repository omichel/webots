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

#include "WbPropeller.hpp"

#include "WbRobot.hpp"
#include "WbRotationalMotor.hpp"
#include "WbSFNode.hpp"
#include "WbSimulationState.hpp"
#include "WbSolid.hpp"
#include "WbSolidMerger.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/config.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include <ode/ode.h>

void WbPropeller::init() {
  mShaftAxis = findSFVector3("shaftAxis");
  mCenterOfThrust = findSFVector3("centerOfThrust");
  mThrustConstants = findSFVector2("thrustConstants");
  mTorqueConstants = findSFVector2("torqueConstants");
  mFastHelixThreshold = findSFDouble("fastHelixThreshold");
  mDevice = findSFNode("device");
  mFastHelix = findSFNode("fastHelix");
  mSlowHelix = findSFNode("slowHelix");

  mPosition = 0.0;
  mHelixType = SLOW_HELIX;
  mHelix = NULL;

  mCurrentThrust = 0.0;
  mCurrentTorque = 0.0;

  // WREN
  mTransform = NULL;
  mRenderable = NULL;
  mMesh = NULL;
  mMaterial = NULL;
}

// Constructors

WbPropeller::WbPropeller(WbTokenizer *tokenizer) : WbBaseNode("Propeller", tokenizer) {
  init();
}

WbPropeller::WbPropeller(const WbPropeller &other) : WbBaseNode(other) {
  init();
}

WbPropeller::WbPropeller(const WbNode &other) : WbBaseNode(other) {
  init();
}

WbPropeller::~WbPropeller() {
  if (areWrenObjectsInitialized()) {
    wr_static_mesh_delete(mMesh);
    wr_material_delete(mMaterial);
    wr_node_delete(WR_NODE(mRenderable));
    wr_node_delete(WR_NODE(mTransform));
  }
}

void WbPropeller::preFinalize() {
  WbBaseNode::preFinalize();

  WbLogicalDevice *const d = device();
  if (d && !d->isPreFinalizedCalled())
    d->preFinalize();

  WbBaseNode *helix = static_cast<WbBaseNode *>(mFastHelix->value());
  if (helix && !helix->isPreFinalizedCalled())
    helix->preFinalize();

  helix = static_cast<WbBaseNode *>(mSlowHelix->value());
  if (helix && !helix->isPreFinalizedCalled())
    helix->preFinalize();

  updateShaftAxis();
  updateDevice();
}

void WbPropeller::postFinalize() {
  WbBaseNode::postFinalize();

  WbLogicalDevice *const d = device();
  if (d && !d->isPostFinalizedCalled())
    d->postFinalize();

  WbBaseNode *helix = static_cast<WbBaseNode *>(mFastHelix->value());
  if (helix && !helix->isPostFinalizedCalled())
    helix->postFinalize();

  helix = static_cast<WbBaseNode *>(mSlowHelix->value());
  if (helix && !helix->isPostFinalizedCalled())
    helix->postFinalize();

  updateHelix(0.0);

  connect(mShaftAxis, &WbSFVector3::changed, this, &WbPropeller::updateShaftAxis);
  connect(mCenterOfThrust, &WbSFVector3::changed, this, &WbPropeller::updateShaftAxis);
  connect(mDevice, &WbSFNode::changed, this, &WbPropeller::updateDevice);
  connect(WbSimulationState::instance(), &WbSimulationState::modeChanged, this, &WbPropeller::updateHelixRepresentation);
}

void WbPropeller::createOdeObjects() {
  WbSolid *const fastHelix = helix(FAST_HELIX);
  WbSolid *const slowHelix = helix(SLOW_HELIX);

  if (fastHelix)
    fastHelix->createOdeObjects();
  if (slowHelix)
    slowHelix->createOdeObjects();
}

WbRotationalMotor *WbPropeller::motor() const {
  return dynamic_cast<WbRotationalMotor *>(mDevice->value());
}

WbLogicalDevice *WbPropeller::device() const {
  return dynamic_cast<WbLogicalDevice *>(mDevice->value());
}

// Update methods: they check validity and correct if necessary
void WbPropeller::updateHelix(double angularSpeed, bool ode) {
  const bool fast = fabs(angularSpeed) > mFastHelixThreshold->value();
  mHelixType = fast ? FAST_HELIX : SLOW_HELIX;
  WbSolid *const fastHelix = helix(FAST_HELIX);
  WbSolid *const slowHelix = helix(SLOW_HELIX);
  mHelix = fast ? fastHelix : slowHelix;
  WbSolid *const hiddenHelix = fast ? slowHelix : fastHelix;
  if (mHelix)
    mHelix->enable(true, ode);

  const WbSimulationState *const state = WbSimulationState::instance();
  const bool pause = state->isPaused();
  if (hiddenHelix)
    hiddenHelix->enable(pause && hiddenHelix->isSelected(), ode);
}

void WbPropeller::updateHelixRepresentation() {
  const WbRotationalMotor *const m = motor();
  const double speed = m ? m->currentVelocity() : 0.0;
  updateHelix(speed, false);
}

void WbPropeller::updateDevice() {
  if (mDevice->value()) {
    const WbSolid *const s = upperSolid();
    if (s) {
      WbRobot *const r = s->robot();
      assert(r);
      r->descendantNodeInserted(NULL);
    }
  }
}

void WbPropeller::updateShaftAxis() {
  static const WbVector3 DEFAULT_AXIS(0.0, 1.0, 0.0);
  if (mShaftAxis->value().isNull()) {
    parsingWarn(
      tr("'shaftAxis'cannot be zero. Defaults to %1 %2 %3").arg(DEFAULT_AXIS.x()).arg(DEFAULT_AXIS.y()).arg(DEFAULT_AXIS.z()));
    mShaftAxis->setValue(DEFAULT_AXIS);
  }

  mNormalizedAxis = mShaftAxis->value().normalized();

  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_JOINT_AXES))
    updateShaftAxisRepresentation();
}

void WbPropeller::prePhysicsStep(double ms) {
  mCurrentThrust = 0.0;
  mCurrentTorque = 0.0;

  WbRotationalMotor *const m = motor();
  if (m == NULL)
    return;

  const bool run = m->runKinematicControl(ms, mPosition);
  if (!run)
    return;

  const double velocity = m->currentVelocity();
  const double absoluteVelocity = fabs(velocity);
  if (absoluteVelocity > 0.0) {
    const WbSolid *const us = upperSolid();
    const WbSolidMerger *const sm = us->solidMerger();
    dBodyID b = sm ? sm->body() : NULL;
    if (b == NULL) {
      parsingWarn(tr("Adds a Physics node to Solid ancestors to enable thrust and torque effect."));
      return;
    }

    // Computes thrust and torque
    const WbTransform *const ut = upperTransform();
    const WbVector3 &cot = ut->matrix() * mCenterOfThrust->value();
    double vp[4];
    dBodyGetPointVel(b, cot.x(), cot.y(), cot.z(), vp);
    const double V = dCalcVectorDot3(vp, mNormalizedAxis.ptr());

    const WbVector2 &tcs = mTorqueConstants->value();
    mCurrentTorque = tcs.x() * velocity * absoluteVelocity - tcs.y() * absoluteVelocity * V;
    const double mt = m->maxForceOrTorque();
    if (fabs(mCurrentTorque) > mt)
      mCurrentTorque = mCurrentTorque > 0.0 ? mt : -mt;

    const WbVector2 &fcs = mThrustConstants->value();
    mCurrentThrust = fcs.x() * velocity * absoluteVelocity - fcs.y() * absoluteVelocity * V;

    // Applies thrust and torque
    const WbMatrix3 &m3 = ut->rotationMatrix();
    const WbVector3 &axis = m3 * mNormalizedAxis;
    const WbVector3 &thrustVector = mCurrentThrust * axis;
    const WbVector3 &torqueVector = -mCurrentTorque * axis;
    if (sm && !sm->isBodyArtificiallyDisabled())
      dBodyEnable(b);
    dBodyAddForceAtPos(b, thrustVector.x(), thrustVector.y(), thrustVector.z(), cot.x(), cot.y(), cot.z());
    dBodyAddTorque(b, torqueVector.x(), torqueVector.y(), torqueVector.z());

    updateHelix(absoluteVelocity);
    if (mHelix == NULL)
      return;

    // Moves the slow helix
    const WbQuaternion q(mNormalizedAxis, mPosition);
    const WbQuaternion iq(mHelix->rotationFromFile().toQuaternion());
    WbQuaternion qp(q * iq);
    if (qp.w() != 1.0)
      qp.normalize();
    WbRotation r(qp);
    if (r.angle() == 0.0)
      r = WbRotation(mNormalizedAxis.x(), mNormalizedAxis.y(), mNormalizedAxis.z(), 0.0);
    const WbVector3 &c = mCenterOfThrust->value();
    mHelix->setTranslationAndRotation(q * (mHelix->translationFromFile() - c) + c, r);
  }
}

void WbPropeller::createWrenObjects() {
  WbBaseNode::createWrenObjects();

  WbSolid *const fastHelix = helix(FAST_HELIX);
  WbSolid *const slowHelix = helix(SLOW_HELIX);

  if (fastHelix)
    fastHelix->createWrenObjects();
  if (slowHelix)
    slowHelix->createWrenObjects();

  mTransform = wr_transform_new();
  wr_node_set_visible(WR_NODE(mTransform), false);
  wr_transform_attach_child(wrenNode(), WR_NODE(mTransform));

  const float color[3] = {0.0f, 0.0f, 0.0f};
  mMaterial = wr_phong_material_new();
  wr_phong_material_set_color(mMaterial, color);
  wr_material_set_default_program(mMaterial, WbWrenShaders::lineSetShader());

  mRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mRenderable, false);
  wr_renderable_set_receive_shadows(mRenderable, false);
  wr_renderable_set_drawing_mode(mRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_material(mRenderable, mMaterial, NULL);

  wr_transform_attach_child(mTransform, WR_NODE(mRenderable));

  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_JOINT_AXES))
    wr_node_set_visible(WR_NODE(mTransform), true);

  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
          &WbPropeller::updateOptionalRendering);
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this,
          &WbPropeller::updateShaftAxisRepresentation);

  updateShaftAxisRepresentation();
}

void WbPropeller::updateOptionalRendering(int option) {
  if (option == WbWrenRenderingContext::VF_JOINT_AXES) {
    if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(option)) {
      wr_node_set_visible(WR_NODE(mTransform), true);
      updateShaftAxisRepresentation();
    } else
      wr_node_set_visible(WR_NODE(mTransform), false);
  }
}

void WbPropeller::updateShaftAxisRepresentation() {
  if (!areWrenObjectsInitialized())
    return;

  wr_static_mesh_delete(mMesh);

  const float scaling = 0.5f * wr_config_get_line_scale();
  const WbVector3 &centerOfThrust = mCenterOfThrust->value();
  const WbVector3 axis(scaling * mNormalizedAxis);
  const WbVector3 vMinus(centerOfThrust - axis);
  const WbVector3 vPlus(centerOfThrust + axis);

  float vertices[6];
  vMinus.toFloatArray(vertices);
  vPlus.toFloatArray(vertices + 3);

  mMesh = wr_static_mesh_line_set_new(2, vertices, NULL);
  wr_renderable_set_mesh(mRenderable, WR_MESH(mMesh));
}

void WbPropeller::setMatrixNeedUpdate() {
  WbSolid *const fastHelix = helix(FAST_HELIX);
  WbSolid *const slowHelix = helix(SLOW_HELIX);

  if (fastHelix)
    fastHelix->setMatrixNeedUpdate();
  if (slowHelix)
    slowHelix->setMatrixNeedUpdate();
}
WbSolid *WbPropeller::helix(HelixType type) const {
  return type == FAST_HELIX ? static_cast<WbSolid *>(mFastHelix->value()) : static_cast<WbSolid *>(mSlowHelix->value());
}

void WbPropeller::propagateSelection(bool selected) {
  const WbSimulationState *const state = WbSimulationState::instance();
  const bool pause = state->isPaused();
  WbSolid *const fastHelix = helix(FAST_HELIX);
  WbSolid *const slowHelix = helix(SLOW_HELIX);
  const bool fast = mHelixType == FAST_HELIX;

  if (fastHelix && (fast || pause))
    fastHelix->propagateSelection(selected);

  const bool slow = !fast;
  if (slowHelix && (slow || pause))
    slowHelix->propagateSelection(selected);
}

void WbPropeller::write(WbVrmlWriter &writer) const {
  if (writer.isWebots())
    WbBaseNode::write(writer);
  else {
    WbSolid *const fastHelix = helix(FAST_HELIX);
    WbSolid *const slowHelix = helix(SLOW_HELIX);
    if (writer.isX3d())
      writer << "<Group isPropeller='true'>";
    else {
      writer << "Group {\n";
      writer.increaseIndent();
      writer.indent();
      writer << "children ";
    }
    writer.writeMFStart();
    if (fastHelix) {
      writer.writeMFSeparator(true, false);
      fastHelix->write(writer);
    }
    if (slowHelix) {
      writer.writeMFSeparator(!fastHelix, false);
      slowHelix->write(writer);
    }
    writer.writeMFEnd(!fastHelix && !slowHelix);
    if (writer.isX3d())
      writer << "</Group>";
    else {
      writer << "\n";
      writer.decreaseIndent();
      writer.indent();
      writer << "}";
    }
  }
}

void WbPropeller::reset(const QString &id) {
  WbBaseNode::reset(id);

  WbNode *const device = mDevice->value();
  if (device)
    device->reset(id);
  WbNode *const fastHelix = mFastHelix->value();
  if (fastHelix)
    fastHelix->reset(id);
  WbNode *const slowHelix = mSlowHelix->value();
  if (slowHelix)
    slowHelix->reset(id);

  updateHelix(0.0);
}
