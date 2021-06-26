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

#include "WbBasicJoint.hpp"

#include "WbBoundingSphere.hpp"
#include "WbJointParameters.hpp"
#include "WbSlot.hpp"
#include "WbSolid.hpp"
#include "WbSolidReference.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/config.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include <ode/ode.h>
#include <cassert>

void WbBasicJoint::init() {
  mJoint = NULL;
  mIsReverseJoint = false;
  mSpringAndDamperMotor = NULL;
  mIsEndPointPositionChangedByJoint = false;

  mTransform = NULL;
  mRenderable = NULL;
  mMesh = NULL;
  mMaterial = NULL;

  mParameters = findSFNode("jointParameters");
  mEndPoint = findSFNode("endPoint");
}
// Constructors

WbBasicJoint::WbBasicJoint(const QString &modelName, WbTokenizer *tokenizer) : WbBaseNode(modelName, tokenizer) {
  init();
}

WbBasicJoint::WbBasicJoint(const WbBasicJoint &other) : WbBaseNode(other) {
  init();
}

WbBasicJoint::WbBasicJoint(const WbNode &other) : WbBaseNode(other) {
  init();
}

WbBasicJoint::~WbBasicJoint() {
  WbSolid *const s = solidEndPoint();
  if (s && !s->isBeingDeleted())
    s->removeJointParent(this);

  if (mJoint)
    dJointDestroy(mJoint);
  mJoint = NULL;

  if (mSpringAndDamperMotor)
    dJointDestroy(mSpringAndDamperMotor);
  mSpringAndDamperMotor = NULL;

  if (areWrenObjectsInitialized()) {
    wr_static_mesh_delete(mMesh);
    wr_material_delete(mMaterial);
    wr_node_delete(WR_NODE(mRenderable));
    wr_node_delete(WR_NODE(mTransform));
  }
}

void WbBasicJoint::downloadAssets() {
  WbBaseNode *const e = dynamic_cast<WbBaseNode *>(mEndPoint->value());
  if (e)
    e->downloadAssets();
}

void WbBasicJoint::preFinalize() {
  WbBaseNode::preFinalize();

  // set endPoint initial position
  updateParameters();
  updateEndPointZeroTranslationAndRotation();

  WbBaseNode *const p = dynamic_cast<WbBaseNode *>(mParameters->value());
  WbBaseNode *const e = dynamic_cast<WbBaseNode *>(mEndPoint->value());
  if (p && !p->isPreFinalizedCalled())
    p->preFinalize();
  if (e && !e->isPreFinalizedCalled())
    e->preFinalize();
}

void WbBasicJoint::setMatrixNeedUpdate() {
  WbSolid *const s = solidEndPoint();
  if (s && solidReference() == NULL)
    s->setMatrixNeedUpdate();
}

void WbBasicJoint::postFinalize() {
  WbBaseNode::postFinalize();

  WbBaseNode *const p = dynamic_cast<WbBaseNode *>(mParameters->value());
  WbBaseNode *const e = dynamic_cast<WbBaseNode *>(mEndPoint->value());
  if (p && !p->isPostFinalizedCalled())
    p->postFinalize();
  if (e && !e->isPostFinalizedCalled())
    e->postFinalize();

  connect(mEndPoint, &WbSFNode::changed, this, &WbBasicJoint::updateEndPoint);
  const WbGroup *pg = dynamic_cast<WbGroup *>(parentNode());
  if (pg)
    connect(this, &WbBasicJoint::endPointChanged, pg, &WbGroup::insertChildFromSlotOrJoint);
  else {
    const WbSlot *slot = dynamic_cast<WbSlot *>(parentNode());
    if (slot)
      connect(this, &WbBasicJoint::endPointChanged, slot, &WbSlot::endPointInserted);
  }
  connect(mParameters, &WbSFNode::changed, this, &WbBasicJoint::updateParameters);

  WbSolid *const s = solidEndPoint();
  if (s) {
    connect(s, &WbSolid::positionChangedArtificially, this, &WbBasicJoint::updateEndPointPosition);
    updateEndPointPosition();
  }

  if (protoParameterNode()) {
    const QVector<WbNode *> nodes = protoParameterNode()->protoParameterNodeInstances();
    if (nodes.size() > 1 && nodes.at(0) == this)
      parsingWarn(tr("Joint node defined in PROTO field is used multiple times. "
                     "Webots doesn't fully support this because the multiple node instances cannot be identical."));
  }
}

void WbBasicJoint::createOdeObjects() {
  WbBaseNode::createOdeObjects();
  WbSolid *const s = solidEndPoint();
  if (s && solidReference() == NULL)
    s->createOdeObjects();
}

bool WbBasicJoint::setJoint() {
  WbSolidReference *const sr = solidReference();
  if (sr)
    sr->updateName();
  const WbSolid *const s = solidEndPoint();
  const bool invalidEndPoint = s == NULL && (sr == NULL || !sr->pointsToStaticEnvironment());
  if (invalidEndPoint || upperSolid() == NULL || (s && s->physics() == NULL) || (s && s->solidMerger().isNull())) {
    if (mJoint) {
      dJointAttach(mJoint, NULL, NULL);
      dJointDisable(mJoint);
    }
    if (mSpringAndDamperMotor) {
      dJointAttach(mSpringAndDamperMotor, NULL, NULL);
      dJointDisable(mSpringAndDamperMotor);
    }
    return false;
  }

  return true;
}

void WbBasicJoint::setOdeJoint(dBodyID body, dBodyID parentBody) {
  assert(mJoint && upperSolid() && (solidEndPoint() || (solidReference() && solidReference()->pointsToStaticEnvironment())));
  // linked to static environment: ODE internally requires the first body to be not NULL and switches the two bodies
  mIsReverseJoint = parentBody == NULL;
  dJointAttach(mJoint, parentBody, body);
  if (parentBody == NULL && body == NULL)
    dJointDisable(mJoint);
  else
    dJointEnable(mJoint);

  applyToOdeSpringAndDampingConstants(body, parentBody);
}

void WbBasicJoint::reset(const QString &id) {
  WbBaseNode::reset(id);
  WbNode *const p = mParameters->value();
  WbNode *const e = mEndPoint->value();
  if (p)
    p->reset(id);
  if (e)
    e->reset(id);
}

void WbBasicJoint::save(const QString &id) {
  WbBaseNode::save(id);
  WbNode *const p = mParameters->value();
  WbNode *const e = mEndPoint->value();
  if (p)
    p->save(id);
  if (e)
    e->save(id);
}

// Update methods: they check validity and correct if necessary

void WbBasicJoint::updateAfterParentPhysicsChanged() {
  WbSolid *const s = solidEndPoint();
  if (s) {
    s->appendJointParent(this);
    if (s->isKinematic())
      updateEndPointZeroTranslationAndRotation();
  }
}

void WbBasicJoint::updateEndPoint() {
  WbSolidReference *const r = solidReference();
  if (r)
    r->updateName();

  WbSolid *const s = solidEndPoint();
  if (s) {
    connect(s, &WbSolid::positionChangedArtificially, this, &WbBasicJoint::updateEndPointPosition, Qt::UniqueConnection);
    s->appendJointParent(this);
  }

  updateEndPointPosition();

  if (r)
    connect(r, &WbSolidReference::changed, this, &WbBasicJoint::setJoint, Qt::UniqueConnection);

  if (s == NULL || s->isPostFinalizedCalled()) {
    emit endPointChanged(s);
    if (s != NULL && isPostFinalizedCalled())
      WbBoundingSphere::addSubBoundingSphereToParentNode(this);
  } else {
    connect(s, &WbBaseNode::finalizationCompleted, this, &WbBasicJoint::endPointChanged);
    connect(s, &WbBaseNode::finalizationCompleted, this, &WbBasicJoint::updateBoundingSphere);
  }
}

void WbBasicJoint::updateBoundingSphere(WbBaseNode *subNode) {
  disconnect(subNode, &WbBaseNode::finalizationCompleted, this, &WbBasicJoint::updateBoundingSphere);
  WbBoundingSphere::addSubBoundingSphereToParentNode(this);
}

void WbBasicJoint::updateEndPointPosition() {
  if (mIsEndPointPositionChangedByJoint)
    return;

  WbSolid *const s = solidEndPoint();
  if (s)
    updateEndPointZeroTranslationAndRotation();

  if (areOdeObjectsCreated())
    setJoint();
}

void WbBasicJoint::updateSpringAndDampingConstants() {
  const WbSolid *const s = solidEndPoint();
  const WbSolid *const us = upperSolid();
  if (areOdeObjectsCreated() && s && us)
    applyToOdeSpringAndDampingConstants(s->body(), us->bodyMerger());
}

// Utility functions

void WbBasicJoint::setSolidEndPoint(WbSolid *solid) {
  mEndPoint->removeValue();
  mEndPoint->setValue(solid);
  updateEndPoint();
}

void WbBasicJoint::setSolidEndPoint(WbSolidReference *solid) {
  mEndPoint->removeValue();
  mEndPoint->setValue(solid);
  updateEndPoint();
}

void WbBasicJoint::setSolidEndPoint(WbSlot *slot) {
  mEndPoint->removeValue();
  mEndPoint->setValue(slot);
  updateEndPoint();
}

WbSolid *WbBasicJoint::solidEndPoint() const {
  WbSlot *slot = dynamic_cast<WbSlot *>(mEndPoint->value());
  if (slot) {
    WbSlot *childrenSlot = slot->slotEndPoint();
    if (childrenSlot) {
      WbSolid *solid = childrenSlot->solidEndPoint();
      if (solid)
        return solid;

      WbSolidReference *solidReference = childrenSlot->solidReferenceEndPoint();
      if (solidReference)
        return solidReference->solid();
    }
  } else {
    WbSolid *solid = dynamic_cast<WbSolid *>(mEndPoint->value());
    if (solid)
      return solid;

    const WbSolidReference *const solidReference = dynamic_cast<WbSolidReference *>(mEndPoint->value());
    if (solidReference)
      return solidReference->solid();
  }

  return NULL;
}

WbSolidReference *WbBasicJoint::solidReference() const {
  WbSlot *slot = dynamic_cast<WbSlot *>(mEndPoint->value());
  if (slot) {
    WbSlot *childrenSlot = slot->slotEndPoint();
    if (childrenSlot)
      return childrenSlot->solidReferenceEndPoint();
    else
      return NULL;
  } else
    return dynamic_cast<WbSolidReference *>(mEndPoint->value());
}

WbSolid *WbBasicJoint::solidParent() const {
  return dynamic_cast<WbSolid *>(parentNode());
}

WbVector3 WbBasicJoint::anchor() const {
  static const WbVector3 ZERO(0.0, 0.0, 0.0);
  return ZERO;
}

bool WbBasicJoint::isEnabled() const {
  return mJoint && dJointIsEnabled(mJoint);
}

//////////
// WREN //
//////////

void WbBasicJoint::createWrenObjects() {
  WbBaseNode::createWrenObjects();

  const float color[3] = {0.0f, 0.0f, 0.0f};
  mMaterial = wr_phong_material_new();
  wr_phong_material_set_color(mMaterial, color);
  wr_material_set_default_program(mMaterial, WbWrenShaders::lineSetShader());

  mRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mRenderable, false);
  wr_renderable_set_receive_shadows(mRenderable, false);
  wr_renderable_set_material(mRenderable, mMaterial, NULL);
  wr_renderable_set_visibility_flags(mRenderable, WbWrenRenderingContext::VF_JOINT_AXES);
  wr_renderable_set_drawing_mode(mRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);

  mTransform = wr_transform_new();
  wr_node_set_visible(WR_NODE(mTransform), false);
  wr_transform_attach_child(mTransform, WR_NODE(mRenderable));
  wr_transform_attach_child(wrenNode(), WR_NODE(mTransform));

  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
          &WbBasicJoint::updateOptionalRendering);

  if (solidReference())  // don't create twice
    return;

  WbSlot *slot = dynamic_cast<WbSlot *>(mEndPoint->value());
  if (slot) {
    slot->createWrenObjects();
    return;
  }

  WbSolid *const s = solidEndPoint();
  if (s)
    s->createWrenObjects();
}

void WbBasicJoint::updateOptionalRendering(int option) {
  if (option == WbWrenRenderingContext::VF_JOINT_AXES) {
    if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(option)) {
      updateJointAxisRepresentation();
      wr_node_set_visible(WR_NODE(mTransform), true);
    } else
      wr_node_set_visible(WR_NODE(mTransform), false);
  }
}
/////////////////
// Handle jerk //
/////////////////

bool WbBasicJoint::resetJointPositions() {
  WbSolid *const s = solidEndPoint();
  if (s == NULL)
    return false;

  const WbVector3 t(s->translation());
  const WbRotation r(s->rotation());

  const WbSolidReference *const sr = solidReference();
  // set ODE joint in initial position to avoid offset
  if (sr == NULL) {
    s->setTranslationAndRotation(mEndPointZeroTranslation, mEndPointZeroRotation);
    s->resetPhysics();
  }
  setJoint();

  // back to current position
  if (sr == NULL) {
    s->setTranslationAndRotation(t, r);
    s->resetPhysics();
  }

  return true;
}

void WbBasicJoint::retrieveEndPointSolidTranslationAndRotation(WbVector3 &it, WbRotation &ir) const {
  const WbSolid *const s = solidEndPoint();
  assert(s);

  if (solidReference()) {
    const WbTransform *const ut = upperTransform();
    WbMatrix4 m = ut->matrix().pseudoInversed() * s->matrix();
    double scale = 1.0 / ut->absoluteScale().x();
    scale *= scale;
    m *= scale;
    ir = WbRotation(m.extracted3x3Matrix());
    it = m.translation();
  } else {
    ir = s->rotation();
    it = s->translation();
  }
}

void WbBasicJoint::write(WbVrmlWriter &writer) const {
  WbSolid *const s = solidEndPoint();
  WbVector3 translation;
  WbRotation rotation;

  if (s && nodeType() != WB_NODE_BALL_JOINT) {
    // remove unquantified ODE effects on the endPoint Solid translation and rotation fields
    translation = s->translation();
    rotation = s->rotation();
    WbVector3 computedTranslation;
    WbRotation computedRotation;
    const WbBasicJoint *instance = NULL;
    if (isProtoParameterNode())
      instance = dynamic_cast<WbBasicJoint *>(getFirstFinalizedProtoInstance());
    if (instance == NULL)
      instance = this;
    instance->computeEndPointSolidPositionFromParameters(computedTranslation, computedRotation);
    s->blockSignals(true);
    if (!translation.almostEquals(computedTranslation))
      s->setTranslationFromOde(computedTranslation);
    if (!rotation.almostEquals(computedRotation))
      s->setRotationFromOde(computedRotation);
    s->blockSignals(false);
  }

  if (writer.isWebots() || writer.isUrdf())
    WbBaseNode::write(writer);
  else {
    // we should not export any SolidReference Solid here,
    // otherwise they will appear duplicate in the X3D/VRML file,
    // this is why we don't use the solidEndPoint() method
    const WbSolid *solid = dynamic_cast<const WbSolid *>(mEndPoint->value());
    if (solid)
      solid->write(writer);
    else {
      const WbSlot *slot = dynamic_cast<const WbSlot *>(mEndPoint->value());
      if (slot) {
        WbSlot *childrenSlot = slot->slotEndPoint();
        if (childrenSlot) {
          const WbSolid *solidInSlot = childrenSlot->solidEndPoint();
          if (solidInSlot)
            solidInSlot->write(writer);
        }
      }
    }
  }

  if (s && nodeType() != WB_NODE_BALL_JOINT) {
    // restore previous endPoint Solid status
    s->blockSignals(true);
    s->setTranslationFromOde(translation);
    s->setRotationFromOde(rotation);
    s->blockSignals(false);
  }
}

WbBoundingSphere *WbBasicJoint::boundingSphere() const {
  if (solidReference())
    return NULL;
  const WbSolid *const solid = solidEndPoint();
  if (solid)
    return solid->boundingSphere();
  return NULL;
}
