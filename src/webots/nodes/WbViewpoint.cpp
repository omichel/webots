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

#include "WbViewpoint.hpp"

#include "WbBackground.hpp"
#include "WbBoundingSphere.hpp"
#include "WbCoordinateSystem.hpp"
#include "WbFieldChecker.hpp"
#include "WbGuiRefreshOracle.hpp"
#include "WbLensFlare.hpp"
#include "WbLight.hpp"
#include "WbLog.hpp"
#include "WbMFNode.hpp"
#include "WbMatrix3.hpp"
#include "WbNodeUtilities.hpp"
#include "WbPreferences.hpp"
#include "WbRay.hpp"
#include "WbRgb.hpp"
#include "WbSFDouble.hpp"
#include "WbSFRotation.hpp"
#include "WbSFVector3.hpp"
#include "WbSelection.hpp"
#include "WbSimulationState.hpp"
#include "WbSolid.hpp"
#include "WbVector4.hpp"
#include "WbWorld.hpp"
#include "WbWrenBloom.hpp"
#include "WbWrenGtao.hpp"
#include "WbWrenHdr.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"
#include "WbWrenSmaa.hpp"

#ifdef _WIN32
#include "WbVirtualRealityHeadset.hpp"
#endif

#include <wren/camera.h>
#include <wren/node.h>
#include <wren/scene.h>
#include <wren/shader_program.h>
#include <wren/transform.h>
#include <wren/viewport.h>

#include <QtCore/QVariantAnimation>

#define ANIMATION_DURATION 1000

const double WbViewpoint::INCREASE_FACTOR = 1.1;
const double WbViewpoint::DECREASE_FACTOR = 0.9;

static const float DEFAULT_FAR = 1000000.0f;

void WbViewpoint::init() {
  static const double TAN_M_PI_8 = tan(M_PI_4 / 2.0);
  mAspectRatio = 1.0;
  mOrthographicViewHeight = 1.0;
  mFollowedSolid = NULL;
  mFollowedSolidPreviousPosition = WbVector3();
  mFollowedSolidReferenceRotation = WbMatrix3();
  mViewPointReferenceRotation = WbRotation();
  mReferenceOffset = WbVector3();
  mIsLocked = false;
  mRotationCenterIsLocked = false;
  mFieldOfViewY = M_PI_4;
  mTanHalfFieldOfViewY = TAN_M_PI_8;
  mFollowChangedBySelection = false;
  mFollowEmptiedByDestroyedSolid = false;
  mFollowChangedBySolidName = false;
  mNeedToUpdateFollowSolidState = false;
  mCoordinateSystem = NULL;
  mVirtualRealityHeadset = NULL;
  mFromOrthographic = false;
  mTranslateAnimation = NULL;
  mRotateAnimation = NULL;
  mOrbitAnimation = NULL;
  mOrbitRadius = 0.0;
  mSavedFieldOfView[stateId()] = 0.0;
  mSavedFar[stateId()] = 0.0;
  mSavedOrthographicHeight[stateId()] = 0.0;
  mSavedNear[stateId()] = 0.0;
  mInitialOrientationQuaternion = WbQuaternion();
  mInitialOrbitQuaternion = WbQuaternion();
  mFinalOrientationQuaternion = WbQuaternion();
  mFinalOrbitQuaternion = WbQuaternion();
  mLookAtInitialQuaternion = WbQuaternion();
  mLookAtFinalQuaternion = WbQuaternion();
  mSpaceQuaternion = WbQuaternion();
  mWrenCamera = NULL;

  mFieldOfView = findSFDouble("fieldOfView");
  mOrientation = findSFRotation("orientation");
  mPosition = findSFVector3("position");
  mDescription = findSFString("description");
  mNear = findSFDouble("near");
  mFar = findSFDouble("far");
  mExposure = findSFDouble("exposure");
  mFollow = findSFString("follow");
  mFollowType = findSFString("followType");
  mFollowSmoothness = findSFDouble("followSmoothness");
  mLensFlare = findSFNode("lensFlare");
  mAmbientOcclusionRadius = findSFDouble("ambientOcclusionRadius");
  mBloomThreshold = findSFDouble("bloomThreshold");
  mProjectionMode = WR_CAMERA_PROJECTION_MODE_PERSPECTIVE;
  mRotationCenter = WbVector3(mPosition->value());

  mWrenViewport = NULL;
  mWrenSmaa = new WbWrenSmaa();
  mWrenHdr = new WbWrenHdr();
  mWrenGtao = new WbWrenGtao();
  mWrenBloom = new WbWrenBloom();
  mInverseViewMatrix = NULL;

  mNodeVisibilityEnabled = false;

  // backward compatibility
  WbSFBool *followOrientation = findSFBool("followOrientation");
  if (followOrientation->value()) {
    parsingWarn("Deprecated 'followOrientation' field, please use the 'followType' field instead.");
    if (mFollowType->value() == "Tracking Shot") {
      mFollowType->setValue("Mounted Shot");
      followOrientation->setValue(false);
    }
  }

#ifdef _WIN32
  if (WbPreferences::instance()->value("VirtualRealityHeadset/enable").toBool()) {
    mVirtualRealityHeadset = WbVirtualRealityHeadset::instance();
    if (!mVirtualRealityHeadset || !mVirtualRealityHeadset->isValid())
      mVirtualRealityHeadset = NULL;
    else
      connect(mVirtualRealityHeadset, &WbVirtualRealityHeadset::renderRequired, this,
              &WbViewpoint::virtualRealityHeadsetRequiresRender);
  }
#endif
}

WbViewpoint::WbViewpoint(WbTokenizer *tokenizer) : WbBaseNode("Viewpoint", tokenizer) {
  init();
}

WbViewpoint::WbViewpoint(const WbViewpoint &other) : WbBaseNode(other) {
  init();
}

WbViewpoint::WbViewpoint(const WbNode &other) : WbBaseNode(other) {
  init();
}

WbViewpoint::~WbViewpoint() {
  if (areWrenObjectsInitialized()) {
    deleteWrenObjects();
  }
  clearCoordinateSystem();

  delete mWrenSmaa;
  delete mWrenHdr;
  delete mWrenGtao;
  delete mWrenBloom;
}

void WbViewpoint::preFinalize() {
  WbBaseNode::preFinalize();

  updateFieldOfView();
  updateNear();
  updateFar();

  if (lensFlare())
    lensFlare()->preFinalize();
}

void WbViewpoint::postFinalize() {
  WbBaseNode::postFinalize();

  connect(mFieldOfView, &WbSFDouble::changed, this, &WbViewpoint::updateFieldOfView);
  connect(mOrientation, &WbSFRotation::changed, this, &WbViewpoint::updateOrientation);
  connect(mPosition, &WbSFVector3::changed, this, &WbViewpoint::updatePosition);
  connect(mNear, &WbSFDouble::changed, this, &WbViewpoint::updateNear);
  connect(mFar, &WbSFDouble::changed, this, &WbViewpoint::updateFar);
  connect(mExposure, &WbSFDouble::changed, this, &WbViewpoint::updateExposure);
  connect(mFollow, &WbSFString::changed, this, &WbViewpoint::updateFollow);
  connect(mFollowType, &WbSFString::changed, this, &WbViewpoint::updateFollowSolidState);
  connect(mFollowType, &WbSFString::changed, this, &WbViewpoint::updateFollowType);
  connect(mLensFlare, &WbSFNode::changed, this, &WbViewpoint::updateLensFlare);
  connect(mAmbientOcclusionRadius, &WbSFDouble::changed, this, &WbViewpoint::updateAmbientOcclusionRadius);
  connect(mBloomThreshold, &WbSFDouble::changed, this, &WbViewpoint::updateBloomThreshold);
  connect(WbPreferences::instance(), &WbPreferences::changedByUser, this, &WbViewpoint::updatePostProcessingEffects);

  save(stateId());

  if (lensFlare())
    lensFlare()->postFinalize();

  startFollowUpFromField();
}

void WbViewpoint::deleteWrenObjects() {
#ifdef _WIN32
  if (mVirtualRealityHeadset)
    mVirtualRealityHeadset->deleteWrenObjects();
#endif

  if (lensFlare())
    lensFlare()->detachFromViewport();

  clearCoordinateSystem();
}

void WbViewpoint::reset(const QString &id) {
  WbBaseNode::reset(id);

  WbNode *const l = mLensFlare->value();
  if (l)
    l->reset(id);

  mOrientation->setValue(mSavedOrientation[id]);
  mPosition->setValue(mSavedPosition[id]);
  resetAnimations();
  // we can't call 'updateFollowSolidState' here because the followed solid will probably moved
  mNeedToUpdateFollowSolidState = true;
  mEquilibriumVector.setXyz(0.0, 0.0, 0.0);
  mVelocity.setXyz(0.0, 0.0, 0.0);

  foreach (WbBaseNode *const node, mInvisibleNodes)
    setNodeVisibility(node, true);

  mInvisibleNodes.clear();
  updatePostProcessingEffects();
}

/////////////
// Cleanup //
/////////////

void WbViewpoint::terminateFollowUp() {
  mFollowedSolid = NULL;
}

void WbViewpoint::emptyFollow() {
  mFollowedSolid = NULL;
  emit followInvalidated(false);          // turn off the follow object action at the WbView3D level
  mFollowEmptiedByDestroyedSolid = true;  // do nothing in mViewpoint when emitting the changed() signal of mFollow
  mFollow->setValue(QString());
  mFollowEmptiedByDestroyedSolid = false;
}

void WbViewpoint::clearCoordinateSystem() {
  if (!mCoordinateSystem)
    return;

  disconnect(mOrientation, &WbSFRotation::changed, this, &WbViewpoint::updateCoordinateSystem);

  delete mCoordinateSystem;
  mCoordinateSystem = NULL;
}

// Getters //
/////////////

bool WbViewpoint::isFollowed(const WbSolid *solid) const {
  return solid == mFollowedSolid;
}

float WbViewpoint::viewDistanceUnscaling(WbVector3 position) const {
  WbVector3 eye = mPosition->value();
  WbVector3 forward = mOrientation->value().direction().normalized();

  float w;
  if (projectionMode() == WbViewpoint::PM_PERSPECTIVE) {
    WbVector4 bottomRow = WbVector4(-forward.x(), -forward.y(), -forward.z(), forward.dot(eye));
    w = bottomRow.dot(WbVector4(position, 1.0f));

    // Remove scaling due to FOV
    w *= tanf(0.5f * mFieldOfView->value());
  } else {
    const float halfHeight = orthographicViewHeight() / 2.0f, width = halfHeight * aspectRatio();
    if (halfHeight > width)
      w = halfHeight;
    else
      w = width;
  }
  return w;
}

// Setters //
/////////////

void WbViewpoint::startFollowUp(WbSolid *solid, bool updateField) {
  if (solid == NULL)
    return;
  if (mFollowedSolid) {
    disconnect(mFollowedSolid, &WbMatter::destroyed, this, &WbViewpoint::emptyFollow);
    disconnect(mFollowedSolid, &WbMatter::matterNameChanged, this, &WbViewpoint::synchronizeFollowWithSolidName);
  }

  // only a node instance can be followed
  WbNode *node = solid;
  if (node->isProtoParameterNode())
    node = static_cast<WbBaseNode *>(node)->getFirstFinalizedProtoInstance();
  WbSolid *solidInstance = solid;
  if (node != solid) {
    solidInstance = dynamic_cast<WbSolid *>(node);
    if (solidInstance == NULL)
      // no valid solid instance found
      return;
  }

  mFollowedSolid = solidInstance;
  mEquilibriumVector.setXyz(0.0, 0.0, 0.0);
  mVelocity.setXyz(0.0, 0.0, 0.0);
  // listens and reacts to solid's name changes and solid's life cycle
  connect(mFollowedSolid, &WbMatter::destroyed, this, &WbViewpoint::emptyFollow);
  connect(mFollowedSolid, &WbMatter::matterNameChanged, this, &WbViewpoint::synchronizeFollowWithSolidName);
  updateFollowSolidState();
  connect(mPosition, &WbSFVector3::changed, this, &WbViewpoint::updateFollowSolidState);
  connect(mOrientation, &WbSFRotation::changed, this, &WbViewpoint::updateFollowSolidState);

  if (updateField)
    recomputeFollowField();
}

void WbViewpoint::updateFollowSolidState() {
  if (mFollowedSolid) {
    mFollowedSolidPreviousPosition = mFollowedSolid->position();
    mFollowedSolidReferenceRotation = mFollowedSolid->rotationMatrix();
    mViewPointReferenceRotation = mOrientation->value();
    mReferenceOffset = mPosition->value() - mFollowedSolid->position();
  }
}

void WbViewpoint::updateFollowType() {
  emit followTypeChanged(followStringToType(mFollowType->value()));
}

void WbViewpoint::updateLensFlare() {
  if (lensFlare() && areWrenObjectsInitialized()) {
    lensFlare()->setup(mWrenViewport);
#ifdef _WIN32
    if (mVirtualRealityHeadset)
      mVirtualRealityHeadset->setupLensFlare(lensFlare());
#endif
  }
}

void WbViewpoint::updateAmbientOcclusionRadius() {
  WbFieldChecker::resetDoubleIfNegative(this, mAmbientOcclusionRadius, 2.0);
}

void WbViewpoint::updateBloomThreshold() {
  WbFieldChecker::resetDoubleIfNegativeAndNotDisabled(this, mBloomThreshold, 21.0, -1.0);
  updatePostProcessingEffects();
}

WbLensFlare *WbViewpoint::lensFlare() const {
  return dynamic_cast<WbLensFlare *>(mLensFlare->value());
}

void WbViewpoint::startFollowUpFromField() {
  WbSolid *followedSolid = WbSolid::findSolidFromUniqueName(mFollow->value());
  if (followedSolid != NULL)
    startFollowUp(followedSolid, false);
}

void WbViewpoint::setFollowType(int followType) {
  disconnect(mFollowType, &WbSFBool::changed, this, &WbViewpoint::updateFollowType);
  mFollowType->setValue(followTypeToString(followType));
  connect(mFollowType, &WbSFBool::changed, this, &WbViewpoint::updateFollowType);
}

void WbViewpoint::recomputeFollowField() {
  if (mFollowedSolid == NULL)
    return;
  mFollowChangedBySelection = true;  // do nothing in mViewpoint when the changed() signal of mFollow is emitted
  mFollow->setValue(mFollowedSolid->computeUniqueName());
  mFollowChangedBySelection = false;
}

void WbViewpoint::synchronizeFollowWithSolidName() {
  mFollowChangedBySolidName = true;  // do nothing in mViewpoint when the changed() signal of mFollow is emitted
  mFollow->setValue(mFollowedSolid->computeUniqueName());
  mFollowChangedBySolidName = false;
}

void WbViewpoint::setOrthographicViewHeight(double ovh) {
  mOrthographicViewHeight = ovh;
  mSavedOrthographicHeight[stateId()] = ovh;
  updateOrthographicViewHeight();
}

void WbViewpoint::incOrthographicViewHeight() {
  mOrthographicViewHeight *= INCREASE_FACTOR;
  updateOrthographicViewHeight();
}

void WbViewpoint::decOrthographicViewHeight() {
  mOrthographicViewHeight *= DECREASE_FACTOR;
  updateOrthographicViewHeight();
}

void WbViewpoint::setNodeVisibility(WbBaseNode *node, bool visible) {
  WrNode *wrenNode = WR_NODE(node->wrenNode());
  if (wrenNode)
    wr_node_set_visible(wrenNode, visible);

  if (visible)
    mInvisibleNodes.removeAll(node);
  else if (!mInvisibleNodes.contains(node))
    mInvisibleNodes.append(node);
  emit nodeVisibilityChanged(node, visible);
}

void WbViewpoint::enableNodeVisibility(bool enabled) {
  // apply action only if needed
  // and avoid enabling/disabling visibility multiple times in the same step in case of multiple cameras
  if (mNodeVisibilityEnabled == enabled)
    return;

  const int size = mInvisibleNodes.size();
  for (int i = 0; i < size; ++i) {
    WrNode *wrenNode = WR_NODE(mInvisibleNodes.at(i)->wrenNode());
    if (wrenNode)
      wr_node_set_visible(wrenNode, enabled);
  }
  mNodeVisibilityEnabled = enabled;
}

void WbViewpoint::save(const QString &id) {
  WbBaseNode::save(id);
  mSavedNear[id] = mNear->value();
  mSavedFar[id] = mFar->value();
  mSavedFieldOfView[id] = mFieldOfView->value();
  mSavedPosition[id] = mPosition->value();
  mSavedOrientation[id] = mOrientation->value();
  mSavedDescription[id] = mDescription->value();
  mSavedFollow[id] = mFollow->value();
  recomputeFollowField();
}

void WbViewpoint::setPosition(const WbVector3 &position) {
  // will update and emit necessary signals
  mPosition->setValue(position);
}

void WbViewpoint::restore() {
  mNear->setValue(mSavedNear[stateId()]);
  mFar->setValue(mSavedFar[stateId()]);
  mFieldOfView->setValue(mSavedFieldOfView[stateId()]);
  mDescription->setValue(mSavedDescription[stateId()]);
  mFollow->setValue(mSavedFollow[stateId()]);

  if (mProjectionMode == WR_CAMERA_PROJECTION_MODE_ORTHOGRAPHIC) {
    mOrthographicViewHeight = mSavedOrthographicHeight[stateId()];
    updateOrthographicViewHeight();
  }
  moveTo(mSavedPosition[stateId()], mSavedOrientation[stateId()]);
}

void WbViewpoint::lookAt(const WbVector3 &target, const WbVector3 &upVector) {
  // compute the forward vector from target to eye
  WbVector3 forward = mPosition->value() - target;
  forward.normalize();

  WbVector3 normalizedUpVector = upVector;
  normalizedUpVector.normalize();

  // don't bother looking if we're already looking at the object
  if (forward.dot(mOrientation->value().direction()) > 0.9999999)
    return;

  // compute the right vector
  WbVector3 right = normalizedUpVector.cross(forward);
  right.normalize();

  // recompute the orthonormal up vector
  WbVector3 up = forward.cross(right);
  WbQuaternion newLookAtQuaternion = WbQuaternion(right, up, forward);
  newLookAtQuaternion.normalize();
  WbRotation newOrientation = WbRotation(newLookAtQuaternion);
  mOrientation->setValue(newOrientation);
}

// Create //
////////////

void WbViewpoint::createWrenObjects() {
  WbBaseNode::createWrenObjects();

#ifdef _WIN32
  if (mVirtualRealityHeadset)
    mVirtualRealityHeadset->createWrenObjects(wrenNode(),
                                              WbPreferences::instance()->value("VirtualRealityHeadset/antiAliasing").toBool());
#endif

  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
          &WbViewpoint::updateOptionalRendering);
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::renderingModeChanged, this,
          &WbViewpoint::updateRenderingMode);

  const bool coordinateSystemIsVisible =
    WbSimulationState::instance()->isRendering() &&
    WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_COORDINATE_SYSTEM);
  if (coordinateSystemIsVisible)
    createCoordinateSystem();

  mWrenViewport = wr_scene_get_viewport(wr_scene_get_instance());
  wr_viewport_set_visibility_mask(mWrenViewport, WbWrenRenderingContext::instance()->optionalRenderingsMask());

  const WbBackground *const background = WbBackground::firstInstance();
  if (background) {
    float color[] = {static_cast<float>(background->skyColor().red()), static_cast<float>(background->skyColor().green()),
                     static_cast<float>(background->skyColor().blue())};
    wr_viewport_set_clear_color_rgb(mWrenViewport, color);
  } else {
    float color[] = {0.0, 0.0, 0.0};
    wr_viewport_set_clear_color_rgb(mWrenViewport, color);
  }

  mWrenCamera = wr_viewport_get_camera(mWrenViewport);

  applyPositionToWren();
  applyOrientationToWren();
  applyNearToWren();
  applyFarToWren();
  applyFieldOfViewToWren();
  applyOrthographicViewHeightToWren();
  updateLensFlare();
  updatePostProcessingEffects();

  mInverseViewMatrix = wr_transform_get_matrix(WR_TRANSFORM(mWrenCamera));

  // once the camera and viewport are created, update everything in the world instance
  WbWorld::instance()->setViewpoint(this);
}

void WbViewpoint::createCoordinateSystem() {
  if (mCoordinateSystem != NULL || mVirtualRealityHeadset)
    return;

  mCoordinateSystem = new WbCoordinateSystem(WbWrenRenderingContext::instance());
  mCoordinateSystem->setVisible(true);
  updateCoordinateSystem();
  connect(mOrientation, &WbSFRotation::changed, this, &WbViewpoint::updateCoordinateSystem);
}

void WbViewpoint::showCoordinateSystem(bool visible) {
  if (visible)
    createCoordinateSystem();
  else
    clearCoordinateSystem();
}
/////////////////////
// updates methods //
/////////////////////

void WbViewpoint::updateFieldOfView() {
  if (WbFieldChecker::resetDoubleIfNotInRangeWithExcludedBounds(this, mFieldOfView, 0.0, M_PI, M_PI_2))
    return;

  updateFieldOfViewY();

  if (areWrenObjectsInitialized())
    applyFieldOfViewToWren();

  emit cameraParametersChanged();
}

void WbViewpoint::updateOrientation() {
  if (areWrenObjectsInitialized()) {
    applyOrientationToWren();
    emit cameraParametersChanged();
  }
}

void WbViewpoint::updatePosition() {
  if (areWrenObjectsInitialized()) {
    applyPositionToWren();
    emit cameraParametersChanged();
  }
}

void WbViewpoint::updateNear() {
  if (WbFieldChecker::resetDoubleIfNonPositive(this, mNear, 0.05))
    return;

  if (mFar->value() > 0.0 and mFar->value() < mNear->value()) {
    mNear->setValue(mFar->value());
    parsingWarn(tr("'near' is greater than 'far'. Setting 'near' to %1.").arg(mNear->value()));
  }

  if (areWrenObjectsInitialized())
    applyNearToWren();
}

void WbViewpoint::updateFar() {
  if (WbFieldChecker::resetDoubleIfNegative(this, mFar, 0.0))
    return;

  if (mFar->value() > 0.0 and mFar->value() < mNear->value()) {
    mFar->setValue(mNear->value() + 1.0);
    parsingWarn(tr("'far' is less than 'near'. Setting 'far' to %1.").arg(mFar->value()));
    return;
  }

  if (areWrenObjectsInitialized())
    applyFarToWren();
}

void WbViewpoint::updateExposure() {
  if (WbFieldChecker::resetDoubleIfNegative(this, mExposure, 1.0))
    return;

#ifdef _WIN32
  if (mVirtualRealityHeadset) {
    if (mVirtualRealityHeadset->leftEyeHdr())
      mVirtualRealityHeadset->leftEyeHdr()->setExposure(mExposure->value());
    if (mVirtualRealityHeadset->rightEyeHdr())
      mVirtualRealityHeadset->rightEyeHdr()->setExposure(mExposure->value());
  }
#endif

  if (areWrenObjectsInitialized() && mWrenHdr)
    mWrenHdr->setExposure(mExposure->value());
}

void WbViewpoint::updateAspectRatio(double renderWindowAspectRatio) {
  if (!areWrenObjectsInitialized())
    return;

  mAspectRatio = renderWindowAspectRatio;
  wr_camera_set_aspect_ratio(mWrenCamera, mAspectRatio);

  updateFieldOfViewY();

  applyFieldOfViewToWren();

  emit cameraParametersChanged();
}

void WbViewpoint::updateRenderingMode() {
  if (areWrenObjectsInitialized())
    applyRenderingModeToWren();
}

void WbViewpoint::updateOrthographicViewHeight() {
  if (!areWrenObjectsInitialized())
    return;

  applyOrthographicViewHeightToWren();

  emit cameraParametersChanged();
}

QString WbViewpoint::followTypeToString(int type) {
  if (type == FOLLOW_MOUNTED)
    return "Mounted Shot";
  else if (type == FOLLOW_PAN_AND_TILT)
    return "Pan and Tilt Shot";
  else if (type == FOLLOW_TRACKING)
    return "Tracking Shot";
  return "None";
}

int WbViewpoint::followStringToType(const QString &type) {
  if (type == "Tracking Shot")
    return FOLLOW_TRACKING;
  else if (type == "Mounted Shot")
    return FOLLOW_MOUNTED;
  else if (type == "Pan and Tilt Shot")
    return FOLLOW_PAN_AND_TILT;
  return FOLLOW_NONE;
}

void WbViewpoint::updateFieldOfViewY() {
  mTanHalfFieldOfViewY = tan(0.5 * mFieldOfView->value());  // stored for reuse in viewpointRay()

  // According to VRML standards, the meaning of mFieldOfView depends on the aspect ratio:
  // the view angle is taken with respect to the largest dimension
  if (mAspectRatio < 1.0)
    mFieldOfViewY = mFieldOfView->value();
  else {
    mTanHalfFieldOfViewY /= mAspectRatio;
    mFieldOfViewY = 2.0 * atan(mTanHalfFieldOfViewY);
  }
}

void WbViewpoint::updateOptionalRendering(int optionalRendering) {
  if (areWrenObjectsInitialized())
    applyOptionalRenderingToWren(optionalRendering);
}

void WbViewpoint::updateFollow() {
  if (mFollowChangedBySelection || mFollowChangedBySolidName || mFollowEmptiedByDestroyedSolid)
    return;

  if (!mFollow->value().isEmpty()) {
    WbSolid *followedSolid = WbSolid::findSolidFromUniqueName(mFollow->value());
    if (followedSolid) {
      startFollowUp(followedSolid, false);
      emit followInvalidated(true);  // checks the follow object action at the WbView3D level
      return;
    }
    parsingWarn(tr("'follow' field is filled with an invalid Solid name."));
  }
  mFollowedSolid = NULL;
  emit followInvalidated(false);  // unchecks the follow object action at the WbView3D level
}

void WbViewpoint::updateFollowUp() {
  if (mNeedToUpdateFollowSolidState) {
    mNeedToUpdateFollowSolidState = false;
    updateFollowSolidState();
  }

  if (!mFollowedSolid)
    return;

  disconnect(mPosition, &WbSFVector3::changed, this, &WbViewpoint::updateFollowSolidState);
  disconnect(mOrientation, &WbSFRotation::changed, this, &WbViewpoint::updateFollowSolidState);

  //  Translates viewpoint according to solid displacement
  const WbVector3 &followedSolidCurrentPosition = mFollowedSolid->position();
  const WbVector3 delta(followedSolidCurrentPosition - mFollowedSolidPreviousPosition);
  mFollowedSolidPreviousPosition = followedSolidCurrentPosition;

  if (!mIsLocked) {
    int type = followStringToType(mFollowType->value());
    if (type == FOLLOW_PAN_AND_TILT)
      lookAt(mFollowedSolid->position(), WbWorld::instance()->worldInfo()->upVector());
    else if (type == FOLLOW_MOUNTED) {
      // Update Orientation
      WbMatrix3 solidRotation = mFollowedSolid->rotationMatrix() * mFollowedSolidReferenceRotation.transposed();
      WbRotation newOrientation = WbRotation(solidRotation * mViewPointReferenceRotation.toMatrix3());
      newOrientation.normalize();
      mOrientation->setValue(newOrientation);
      // Update Position (position is computed relatively to the solid)
      mPosition->setValue(mFollowedSolid->position() + solidRotation * mReferenceOffset);
    } else if (type == FOLLOW_TRACKING) {
      mEquilibriumVector += delta;
      // clang-format off
      // clang-format 11.0.0 is not compatible with previous versions with respect to nested conditional operators
      const double mass = ((mFollowSmoothness->value() < 0.05) ? 0.0 :
                           (mFollowSmoothness->value() > 1.0)  ? 1.0 :
                                                                 mFollowSmoothness->value());
      // clang-format on
      // If mass is 0, we instantly move the viewpoint to its equilibrium position.
      if (mass == 0.0) {
        // Moves the rotation point if a drag rotating the viewpoint is active
        if (!mRotationCenterIsLocked)
          mRotationCenter += mEquilibriumVector;

        mPosition->setValue(mPosition->value() + mEquilibriumVector);
        mVelocity.setXyz(0.0, 0.0, 0.0);
        mEquilibriumVector.setXyz(0.0, 0.0, 0.0);
      } else {  // Otherwise we apply a force and let physics do the rest.
        const double timeStep = WbWorld::instance()->worldInfo()->basicTimeStep() / 1000.0;
        const WbVector3 acceleration = mEquilibriumVector / mass;
        mVelocity += acceleration * timeStep;

        const double viewPointScalarVelocity = mVelocity.length();
        double followedObjectScalarVelocity;
        WbVector3 followedObjectVelocity;
        if (delta.length() > 0.0) {
          followedObjectVelocity = (delta / timeStep);
          followedObjectScalarVelocity = followedObjectVelocity.dot(mVelocity) / viewPointScalarVelocity;
        } else {
          followedObjectVelocity.setXyz(0.0, 0.0, 0.0);
          followedObjectScalarVelocity = 0.0;
        }

        // If the viewpoint is going faster than the followed object, we slow it down to avoid oscillations
        if (viewPointScalarVelocity > followedObjectScalarVelocity) {
          const double relativeSpeed = viewPointScalarVelocity - followedObjectScalarVelocity;
          if (relativeSpeed < 0.0001)
            mVelocity *= followedObjectScalarVelocity / viewPointScalarVelocity;
          else {
            double friction = 0.05 / mass;
            if (friction > 1.0)
              friction = 1.0;
            mVelocity *= (viewPointScalarVelocity - relativeSpeed * friction) / viewPointScalarVelocity;
          }
        }

        const WbVector3 deltaPosition(mVelocity * timeStep);
        mPosition->setValue(mPosition->value() + deltaPosition);
        // Moves the rotation point if a drag rotating the viewpoint is active
        if (!mRotationCenterIsLocked)
          mRotationCenter += deltaPosition;
        mEquilibriumVector -= deltaPosition;
      }
    }
  }

  connect(mPosition, &WbSFVector3::changed, this, &WbViewpoint::updateFollowSolidState);
  connect(mOrientation, &WbSFRotation::changed, this, &WbViewpoint::updateFollowSolidState);
}

const double WbViewpoint::X_OFFSET = 0.075;    // 7.5 percents of the width from the right side of the screen
const double WbViewpoint::Z_THRESHOLD = 0.05;  // rate of the near clipping distance from the 3D-immersed camera screen
const double WbViewpoint::X_REL_ORTHOGRAPHIC = 0.5 - X_OFFSET;

void WbViewpoint::updateCoordinateSystem() {
  if (!areWrenObjectsInitialized())
    return;

  // Sets orientation
  mCoordinateSystem->setOrientation(mOrientation->value().toQuaternion().conjugated());
}

void WbViewpoint::setCoordinateSystemVisibility(bool visible) {
  visible =
    visible && WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_COORDINATE_SYSTEM);
  // create or clear coordinate system entity
  showCoordinateSystem(visible);
  if (visible)
    // make sure that if already exists the visibility is correct
    mCoordinateSystem->setVisible(visible);
}
///////////////////
// Apply methods //
///////////////////

void WbViewpoint::applyFieldOfViewToWren() {
  wr_camera_set_fovy(mWrenCamera, mFieldOfViewY);

  if (mWrenGtao)
    mWrenGtao->setFov(mFieldOfViewY);
}

void WbViewpoint::applyOrientationToWren() {
#ifdef _WIN32
  if (mVirtualRealityHeadset)
    mVirtualRealityHeadset->setOrientation(mOrientation->value());
#endif

  float angleAxis[] = {static_cast<float>(mOrientation->angle()), static_cast<float>(mOrientation->x()),
                       static_cast<float>(mOrientation->y()), static_cast<float>(mOrientation->z())};
  wr_camera_set_orientation(mWrenCamera, angleAxis);
}

void WbViewpoint::applyPositionToWren() {
#ifdef _WIN32
  if (mVirtualRealityHeadset)
    mVirtualRealityHeadset->setPosition(mPosition->value());
#endif

  float position[] = {static_cast<float>(mPosition->x()), static_cast<float>(mPosition->y()),
                      static_cast<float>(mPosition->z())};
  wr_camera_set_position(mWrenCamera, position);
}

void WbViewpoint::applyNearToWren() {
#ifdef _WIN32
  if (mVirtualRealityHeadset)
    mVirtualRealityHeadset->setNear(mNear->value());
#endif

  wr_camera_set_near(mWrenCamera, mNear->value());
}

void WbViewpoint::applyFarToWren() {
#ifdef _WIN32
  if (mVirtualRealityHeadset) {
    if (mFar->value() > 0.0)
      mVirtualRealityHeadset->setFar(mFar->value());
    else
      mVirtualRealityHeadset->setFar(DEFAULT_FAR);
  }
#endif

  if (mFar->value() > 0.0)
    wr_camera_set_far(mWrenCamera, mFar->value());
  else
    wr_camera_set_far(mWrenCamera, DEFAULT_FAR);
}

// Called when the optional rendering selection changed
void WbViewpoint::applyOptionalRenderingToWren(int optionalRendering) {
  switch (optionalRendering) {
    case WbWrenRenderingContext::VF_ALL_BOUNDING_OBJECTS:
      applyRenderingModeToWren();
      break;
    case WbWrenRenderingContext::VF_COORDINATE_SYSTEM: {
      const bool visible =
        WbSimulationState::instance()->isRendering() &&
        WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_COORDINATE_SYSTEM);
      showCoordinateSystem(visible);
      wr_viewport_set_visibility_mask(mWrenViewport, WbWrenRenderingContext::instance()->visibilityMask());
      break;
    }
    default:
      wr_viewport_set_visibility_mask(mWrenViewport, WbWrenRenderingContext::instance()->visibilityMask());
      break;
  }
}
// Called in the constructor
void WbViewpoint::applyOptionalRenderingToWren() {
  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_ALL_BOUNDING_OBJECTS))
    applyRenderingModeToWren();

  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_COORDINATE_SYSTEM)) {
    const bool visible =
      WbSimulationState::instance()->isRendering() &&
      WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_COORDINATE_SYSTEM);
    showCoordinateSystem(visible);
  }

  wr_viewport_set_visibility_mask(mWrenViewport, WbWrenRenderingContext::instance()->optionalRenderingsMask());
}

void WbViewpoint::applyRenderingModeToWren() {
  int wireframeRendering;
  if (WbWrenRenderingContext::instance()->renderingMode() == WbWrenRenderingContext::RM_WIREFRAME) {
    wr_viewport_set_polygon_mode(mWrenViewport, WR_VIEWPORT_POLYGON_MODE_LINE);
    wireframeRendering = 1;
  } else {
    wr_viewport_set_polygon_mode(mWrenViewport, WR_VIEWPORT_POLYGON_MODE_FILL);
    wireframeRendering = 0;
  }
  wr_shader_program_set_custom_uniform_value(WbWrenShaders::pbrStencilAmbientEmissiveShader(), "wireframeRendering",
                                             WR_SHADER_PROGRAM_UNIFORM_TYPE_INT,
                                             reinterpret_cast<const char *>(&wireframeRendering));

  wr_viewport_set_visibility_mask(mWrenViewport, WbWrenRenderingContext::instance()->visibilityMask());
}

void WbViewpoint::applyOrthographicViewHeightToWren() {
  if (mProjectionMode == WR_CAMERA_PROJECTION_MODE_ORTHOGRAPHIC)
    wr_camera_set_height(mWrenCamera, mOrthographicViewHeight);
}

// Ray picking //
/////////////////

void WbViewpoint::viewpointRay(int x, int y, WbRay &ray) const {
  // Relative position of the picked pixel in the selected viewport with respect to viewport's center
  double w = ((double)x) / wr_viewport_get_width(mWrenViewport) - 0.5;
  double h = ((double)y) / wr_viewport_get_height(mWrenViewport) - 0.5;
  WbVector3 origin(mPosition->value());
  WbVector3 direction;
  const double nearValue = mNear->value();
  const WbMatrix3 &viewpointMatrix = mOrientation->value().toMatrix3();
  const bool &projectionModeIsPerspective = mProjectionMode == WR_CAMERA_PROJECTION_MODE_PERSPECTIVE;
  if (projectionModeIsPerspective) {
    const double scaleFactor = 2.0 * nearValue * mTanHalfFieldOfViewY;
    // World position on camera's screen (we refer here to the world dimensions of camera's screen)
    w *= scaleFactor * mAspectRatio;  // right - left in openGL terms
    h *= scaleFactor;                 // top  - bottom in openGL terms
    // Origin and direction of the mouse ray intersecting with camera's screen
    direction = viewpointMatrix * WbVector3(w, -h, -nearValue);
  } else {
    w *= mOrthographicViewHeight * mAspectRatio;
    h *= mOrthographicViewHeight;
    origin += viewpointMatrix * WbVector3(w, -h, 0.0);
    direction = viewpointMatrix.column(2);
  }
  ray.redefine(origin, direction);
}
// Returns the intersection point of a casted (x, y)-pixel ray with the plane of equation z = z0 in within viewpoint's
// coordinate frame
WbVector3 WbViewpoint::pick(int x, int y, double z0) const {
  // Relative position of the picked pixel in the selected viewport with respect to viewport's center
  double w = ((double)x) / wr_viewport_get_width(mWrenViewport) - 0.5;
  double h = ((double)y) / wr_viewport_get_height(mWrenViewport) - 0.5;
  WbVector3 rayOrigin(mPosition->value());
  WbVector3 rayDirection;
  const double nearValue = mNear->value();
  const WbMatrix3 &viewpointMatrix = mOrientation->value().toMatrix3();
  const WbVector3 &cameraDirection = viewpointMatrix.column(2);
  const bool &perspective = mProjectionMode == WR_CAMERA_PROJECTION_MODE_PERSPECTIVE;
  if (perspective) {
    const double scaleFactor = 2.0 * nearValue * mTanHalfFieldOfViewY;
    // World position on camera's screen (we refer here to the world dimensions of camera's screen)
    w *= scaleFactor * mAspectRatio;  // right - left in openGL terms
    h *= scaleFactor;                 // top  - bottom in openGL terms
    // Origin and direction of the mouse ray intersecting with camera's screen
    rayDirection = viewpointMatrix * WbVector3(w, -h, -nearValue);
    const double factor = -z0 / nearValue;
    rayDirection *= factor;
  } else {
    w *= mOrthographicViewHeight * mAspectRatio;
    h *= mOrthographicViewHeight;
    rayOrigin += viewpointMatrix * WbVector3(w, -h, 0.0);
    rayDirection = z0 * cameraDirection;
  }

  return rayOrigin + rayDirection;
}

// Converts absolute world coordinates of a 3D-point into screen pixel coordinates
void WbViewpoint::toPixels(const WbVector3 &pos, WbVector2 &P) const {
  const WbMatrix3 &viewpointMatrix = mOrientation->value().toMatrix3();
  WbVector3 eyePosition((pos - mPosition->value()) * viewpointMatrix);

  const double z = eyePosition.z();
  if (z == 0.0) {
    P.setX(0.0);
    P.setY(0.0);
    return;
  }

  double w, h;
  if (mProjectionMode == WR_CAMERA_PROJECTION_MODE_PERSPECTIVE) {
    const double factor = 0.5 / (z * mTanHalfFieldOfViewY);
    h = factor * eyePosition.y();
    w = mAspectRatio ? -factor * eyePosition.x() / mAspectRatio : 0.0;
  } else {  // PM_ORTHOGRAPHIC
    w = eyePosition.x() / (mAspectRatio * mOrthographicViewHeight);
    h = -eyePosition.y() / mOrthographicViewHeight;
  }

  P.setX((w + 0.5) * wr_viewport_get_width(mWrenViewport));
  P.setY((h + 0.5) * wr_viewport_get_height(mWrenViewport));
}

// Converts absolute world coordinates of a two 3D-points into screen pixel coordinates
void WbViewpoint::toPixels(const WbVector3 &pos1, WbVector2 &P1, const WbVector3 &pos2, WbVector2 &P2) const {
  const WbMatrix3 &viewpointMatrix = mOrientation->value().toMatrix3();

  const WbVector3 &eyePosition1 = (pos1 - mPosition->value()) * viewpointMatrix;
  const WbVector3 &eyePosition2 = (pos2 - mPosition->value()) * viewpointMatrix;
  eyeToPixels(eyePosition1, P1);
  eyeToPixels(eyePosition2, P2);
}

// Converts screen coordinates to world coordinates
void WbViewpoint::toWorld(const WbVector3 &pos, WbVector3 &P) const {
  double zFar = mFar->value();
  if (zFar == 0)
    zFar = DEFAULT_FAR;

  double zNear = mNear->value();
  WbMatrix4 projection;
  if (mProjectionMode == WR_CAMERA_PROJECTION_MODE_PERSPECTIVE) {
    WbMatrix4 perspective(1.0 / (mAspectRatio * mTanHalfFieldOfViewY), 0, 0, 0, 0, 1.0 / mTanHalfFieldOfViewY, 0, 0, 0, 0,
                          zFar / (zNear - zFar), -(zFar * zNear) / (zFar - zNear), 0, 0, -1, 0);
    projection = perspective;
  } else {
    double halfHeight = mOrthographicViewHeight * 0.5;
    double left = -halfHeight * mAspectRatio, right = halfHeight * mAspectRatio, top = halfHeight, bottom = -halfHeight;
    WbMatrix4 orthographic(2.0 / (right - left), 0, 0, -(right + left) / (right - left), 0, 2.0 / (top - bottom), 0,
                           -(top + bottom) / (top - bottom), 0, 0, -1.0 / (zFar - zNear), -zNear / (zFar - zNear), 0, 0, 0, 1);
    projection = orthographic;
  }

  WbVector3 eye = mPosition->value(), center = eye + mOrientation->value().direction(), up = mOrientation->value().up();

  WbVector3 f = (center - eye).normalized(), s = f.cross(up).normalized(), u = s.cross(f);

  WbMatrix4 view(-s.x(), -s.y(), -s.z(), s.dot(eye), u.x(), u.y(), u.z(), -u.dot(eye), f.x(), f.y(), f.z(), -f.dot(eye), 0, 0,
                 0, 1);

  WbMatrix4 inverse = projection * view;
  if (!inverse.inverse())
    return;

  WbVector4 screen(pos.x(), pos.y(), pos.z(), 1.0);
  screen = inverse * screen;
  screen /= screen.w();
  P.setXyz(screen.ptr());
}

// Converts eye coordinates of a 3D-point into screen pixel coordinates
void WbViewpoint::eyeToPixels(const WbVector3 &eyePosition, WbVector2 &P) const {
  const double z = eyePosition.z();
  if (z == 0.0) {
    P.setX(0.0);
    P.setY(0.0);
    return;
  }

  double w, h;
  if (mProjectionMode == WR_CAMERA_PROJECTION_MODE_PERSPECTIVE) {
    const double factor = 0.5 / (z * mTanHalfFieldOfViewY);
    h = factor * eyePosition.y();
    w = mAspectRatio != 0.0 ? -factor * eyePosition.x() / mAspectRatio : 0.0;
  } else {  // PM_ORTHOGRAPHIC
    w = eyePosition.x() / (mAspectRatio * mOrthographicViewHeight);
    h = -eyePosition.y() / mOrthographicViewHeight;
  }

  P.setX((w + 0.5) * wr_viewport_get_width(mWrenViewport));
  P.setY((h + 0.5) * wr_viewport_get_height(mWrenViewport));
}

// Retrieves the z-eye coordinate of a 3D-point defined by its world coordinates
double WbViewpoint::zEye(const WbVector3 &pos) const {
  const WbVector3 &direction = mOrientation->value().direction();
  return direction.dot(pos - mPosition->value());
}

bool WbViewpoint::enableVirtualRealityHeadset(bool enable) {
#ifdef _WIN32
  if (enable && !mVirtualRealityHeadset) {
    mVirtualRealityHeadset = WbVirtualRealityHeadset::instance();
    if (!mVirtualRealityHeadset || !mVirtualRealityHeadset->isValid()) {
      mVirtualRealityHeadset = NULL;
      return false;
    } else {
      mVirtualRealityHeadset->createWrenObjects(
        wrenNode(), WbPreferences::instance()->value("VirtualRealityHeadset/antiAliasing").toBool());
      applyPositionToWren();
      applyOrientationToWren();
      applyNearToWren();
      applyFarToWren();
      updateLensFlare();
      connect(mVirtualRealityHeadset, &WbVirtualRealityHeadset::renderRequired, this,
              &WbViewpoint::virtualRealityHeadsetRequiresRender);
    }
  } else if (mVirtualRealityHeadset) {
    WbVirtualRealityHeadset::cleanup();
    mVirtualRealityHeadset = NULL;
  }
  return true;
#else
  return false;
#endif
}

void WbViewpoint::setVirtualRealityHeadsetAntiAliasing(bool enable) {
#ifdef _WIN32
  if (mVirtualRealityHeadset) {
    deleteWrenObjects();
    mVirtualRealityHeadset->createWrenObjects(wrenNode(), enable);
    applyPositionToWren();
    applyOrientationToWren();
    applyNearToWren();
    applyFarToWren();
    updateLensFlare();
    connect(mVirtualRealityHeadset, &WbVirtualRealityHeadset::renderRequired, this,
            &WbViewpoint::virtualRealityHeadsetRequiresRender);
  }
#endif
}

void WbViewpoint::updatePostProcessingEffects() {
  if (!areWrenObjectsInitialized())
    return;

  if (lensFlare())
    lensFlare()->setup(mWrenViewport);

  if (mWrenSmaa) {
    if (WbPreferences::instance()->value("OpenGL/disableAntiAliasing", true).toBool())
      mWrenSmaa->detachFromViewport();
    else
      mWrenSmaa->setup(mWrenViewport);
  }

  if (mWrenHdr) {
    mWrenHdr->setup(mWrenViewport);
    updateExposure();
  }

  if (mWrenGtao) {
    const int qualityLevel = WbPreferences::instance()->value("OpenGL/GTAO", 2).toInt();
    if (qualityLevel == 0)
      mWrenGtao->detachFromViewport();
    else {
      mWrenGtao->setHalfResolution(qualityLevel <= 2);
      mWrenGtao->setup(mWrenViewport);
      updateNear();
      updateFar();
      updateFieldOfViewY();
    }
  }

  if (mWrenBloom) {
    if (mBloomThreshold->value() == -1.0)
      mWrenBloom->detachFromViewport();
    else
      mWrenBloom->setup(mWrenViewport);

    mWrenBloom->setThreshold(mBloomThreshold->value());
  }

  emit refreshRequired();
}

void WbViewpoint::updatePostProcessingParameters() {
  if (!areWrenObjectsInitialized())
    return;

  if (mWrenHdr)
    updateExposure();

  if (mWrenGtao) {
    if (mAmbientOcclusionRadius->value() == 0.0 || !WbPreferences::instance()->value("OpenGL/GTAO", 2).toInt()) {
      mWrenGtao->detachFromViewport();
      return;
    } else if (!mWrenGtao->hasBeenSetup())
      mWrenGtao->setup(mWrenViewport);

    int qualityLevel = WbPreferences::instance()->value("OpenGL/GTAO", 2).toInt();
    updateNear();
    mWrenGtao->setRadius(mAmbientOcclusionRadius->value());
    mWrenGtao->setQualityLevel(qualityLevel);
    mWrenGtao->applyOldInverseViewMatrixToWren();
    mWrenGtao->copyNewInverseViewMatrix(mInverseViewMatrix);
  }

  if (mWrenBloom) {
    if (mBloomThreshold->value() == -1.0) {
      mWrenBloom->detachFromViewport();
      return;
    } else if (!mWrenBloom->hasBeenSetup())
      mWrenBloom->setup(mWrenViewport);

    mWrenBloom->setThreshold(mBloomThreshold->value());
  }
}

bool WbViewpoint::moveViewpointToObject(WbBaseNode *node) {
  if (!node)
    return false;

  WbBoundingSphere *boundingSphere = WbNodeUtilities::boundingSphereAncestor(reinterpret_cast<WbNode *>(node));

  boundingSphere->recomputeIfNeeded(false);
  if (boundingSphere->isEmpty())
    // empty world
    return false;

  WbVector3 absoluteCenter;
  double radius;
  boundingSphere->computeSphereInGlobalCoordinates(absoluteCenter, radius);
  const WbVector3 boundingSphereCenter(absoluteCenter.x(), absoluteCenter.y(), absoluteCenter.z());

  // Compute direction vector where the viewpoint is looking at.
  // For all orientation and a zero angle, the viewpoint is looking at the z-axis opposite.
  const WbVector3 viewpointDirection = mOrientation->value().toQuaternion() * WbVector3(0, 0, -1);

  // Compute a distance coefficient between the object and future viewpoint.
  // The bounding sphere will be entirely contained in the 3D view.
  // Use a slightly larger sphere to keep some space between the object and the 3D view borders
  radius *= 1.05;
  double distance = radius / (sin(mFieldOfView->value() / 2.0) * ((mAspectRatio <= 1.0) ? mAspectRatio : (1.0 / mAspectRatio)));

  // set a minimum distance
  if (distance < mNear->value() + radius)
    distance = mNear->value() + radius;

  // Compute new position. From the center of the object, move back the viewpoint along
  // its direction axis.
  const WbVector3 newViewpointPosition = boundingSphereCenter + viewpointDirection * (-distance);

  if (newViewpointPosition != mPosition->value()) {
    // move to target using eased animation
    WbWorld::instance()->setModified();
    moveTo(WbVector3(newViewpointPosition.x(), newViewpointPosition.y(), newViewpointPosition.z()), mOrientation->value());
    return true;
  }

  return false;
}

void WbViewpoint::frontView() {
  orbitTo(WbVector3(0, 0, 1), WbRotation(0, 1, 0, 0));
}

void WbViewpoint::backView() {
  orbitTo(WbVector3(0, 0, -1), WbRotation(0, 1, 0, -M_PI));
}

void WbViewpoint::leftView() {
  orbitTo(WbVector3(-1, 0, 0), WbRotation(0, 1, 0, -M_PI / 2));
}

void WbViewpoint::rightView() {
  orbitTo(WbVector3(1, 0, 0), WbRotation(0, 1, 0, M_PI / 2));
}

void WbViewpoint::topView() {
  orbitTo(WbVector3(0, 1, 0), WbRotation(1, 0, 0, -M_PI / 2));
}

void WbViewpoint::bottomView() {
  orbitTo(WbVector3(0, -1, 0), WbRotation(0, -0.707107, 0.707107, M_PI));
}

void WbViewpoint::orbitTo(const WbVector3 &targetUnitVector, const WbRotation &targetRotation) {
  resetAnimations();
  lock();

  WbWorld::instance()->setModified();

  // first, we need to calculate the orientation of the world as this will be applied to all orbits
  const WbVector3 &defaultUpVector = WbVector3(0, 1, 0);
  const WbVector3 &gravityUpVector = -WbWorld::instance()->worldInfo()->gravityUnitVector();
  if (gravityUpVector.dot(defaultUpVector) > 0.9999)
    // In the case of the gravity vector being the default create the identity quaternion
    mSpaceQuaternion = WbQuaternion();
  else if (gravityUpVector.dot(defaultUpVector) < -0.9999)
    // The gravity vector is the opposite of the default, so our transform is a vertical flip
    mSpaceQuaternion = WbQuaternion(WbVector3(0, 0, 1), M_PI);
  else {  // otherwise we can safely get a rotation axis using the cross product of both vectors
    mSpaceQuaternion = WbQuaternion(defaultUpVector.cross(gravityUpVector), gravityUpVector.angle(defaultUpVector));
    mSpaceQuaternion.normalize();
  }

  const WbNode *selectedNode = reinterpret_cast<WbNode *>(WbSelection::instance()->selectedNode());
  // for UX reasons, we want the default rotation height just above the floor,
  // meaning any horizontal view can see a floor at height 0
  WbVector3 centerToViewpoint;
  WbBoundingSphere *const boundingSphere = WbNodeUtilities::boundingSphereAncestor(selectedNode);
  // if an object is selected use its bounding sphere center to orbit around
  if (boundingSphere) {
    WbVector3 absoluteCenter;
    double unused;  // passed to computeSphereInGlobalCoordinates but not needed
    boundingSphere->computeSphereInGlobalCoordinates(absoluteCenter, unused);
    mRotationCenter = absoluteCenter;
    centerToViewpoint = mPosition->value() - mRotationCenter;
  } else {
    mRotationCenter = WbVector3();
    centerToViewpoint = mPosition->value();
  }
  // preserve the original distance to object / world center
  double newOrbitRadius = centerToViewpoint.length();

  // the orbit radius is only updated if the last animation completed successfully
  if (mOrbitRadius == 0.0)
    mOrbitRadius = newOrbitRadius;

  mCenterToViewpointUnitVector = centerToViewpoint / mOrbitRadius;
  mOrbitTargetUnitVector = mSpaceQuaternion * targetUnitVector;
  mInitialOrientationQuaternion = mSpaceQuaternion * WbQuaternion(mOrientation->value().axis(), mOrientation->value().angle());
  mFinalOrientationQuaternion = mSpaceQuaternion * WbQuaternion(targetRotation.axis(), targetRotation.angle());
  mInitialOrientationQuaternion.normalize();
  mFinalOrientationQuaternion.normalize();

  animateLookAtIfNeeded();
}

void WbViewpoint::animateLookAtIfNeeded() {
  lock();
  lockRotationCenter();

  mLookAtInitialQuaternion = WbQuaternion(mOrientation->value().axis(), mOrientation->value().angle());
  // find out where we're going to be looking
  lookAt(mRotationCenter, mOrientation->value().up());
  // get this as a quaternion
  mLookAtFinalQuaternion = WbQuaternion(mOrientation->value().axis(), mOrientation->value().angle());
  // reset viewpoint to where it was just before
  mLookAtInitialQuaternion.normalize();
  mLookAtFinalQuaternion.normalize();
  mOrientation->setValue(WbRotation(mLookAtInitialQuaternion));

  if (mLookAtInitialQuaternion != mLookAtFinalQuaternion) {
    mRotateAnimation = new QVariantAnimation(this);
    mRotateAnimation->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
    mRotateAnimation->setDuration(ANIMATION_DURATION / 2);
    mRotateAnimation->setStartValue(0.0);
    mRotateAnimation->setEndValue(1.0);
    connect(mRotateAnimation, &QVariantAnimation::valueChanged, this, &WbViewpoint::lookAtAnimationStep);
    connect(mRotateAnimation, &QVariantAnimation::finished, this, &WbViewpoint::firstOrbitStep);
    // we can safely delete this animation when stopped
    mRotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
  } else {
    firstOrbitStep();
  }
}

void WbViewpoint::firstOrbitStep() {
  // no need to lock the viewpoint or its rotation center, they're already locked
  double angleBetweenStartAndFinish = mCenterToViewpointUnitVector.angle(mOrbitTargetUnitVector);
  WbVector3 orbitAxis;
  // choose prefereable axes for axis-to-axis rotations
  if (mCenterToViewpointUnitVector.dot(mOrbitTargetUnitVector) < -0.99) {
    if ((mSpaceQuaternion.conjugated() * mOrbitTargetUnitVector).y() == 0.0)
      orbitAxis = mSpaceQuaternion * WbVector3(0, 1, 0);
    else
      orbitAxis = mSpaceQuaternion * WbVector3(0, 0, 1);
  } else {
    orbitAxis = mOrbitTargetUnitVector.cross(mCenterToViewpointUnitVector).normalized();
  }

  mInitialOrbitQuaternion = WbQuaternion(orbitAxis, 0.0);
  mFinalOrbitQuaternion = WbQuaternion(orbitAxis, angleBetweenStartAndFinish);

  mOrbitAnimation = new QVariantAnimation(this);
  mOrbitAnimation->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
  mOrbitAnimation->setDuration(ANIMATION_DURATION);
  mOrbitAnimation->setStartValue(0.0);
  mOrbitAnimation->setEndValue(1.0);
  connect(mOrbitAnimation, &QVariantAnimation::valueChanged, this, &WbViewpoint::translateOrbitAnimationStep);
  connect(mOrbitAnimation, &QVariantAnimation::finished, this, &WbViewpoint::secondOrbitStep);
  mOrbitAnimation->start();

  mRotateAnimation = new QVariantAnimation(this);
  mRotateAnimation->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
  mRotateAnimation->setDuration(ANIMATION_DURATION);
  mRotateAnimation->setStartValue(0.0);
  mRotateAnimation->setEndValue(0.5);
  connect(mRotateAnimation, &QVariantAnimation::valueChanged, this, &WbViewpoint::rotateOrbitAnimationStep);
  mRotateAnimation->start();
}

void WbViewpoint::secondOrbitStep() {
  resetAnimations();
  lock();
  mInitialOrientationQuaternion = WbQuaternion(mOrientation->value().axis(), mOrientation->value().angle());
  if (mInitialOrientationQuaternion != mFinalOrientationQuaternion) {
    mRotateAnimation = new QVariantAnimation(this);
    mRotateAnimation->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
    mRotateAnimation->setDuration(ANIMATION_DURATION);
    mRotateAnimation->setStartValue(0.0);
    mRotateAnimation->setEndValue(1.0);
    connect(mRotateAnimation, &QVariantAnimation::valueChanged, this, &WbViewpoint::rotateAnimationStep);
    connect(mRotateAnimation, &QVariantAnimation::finished, this, &WbViewpoint::resetAnimations);
    mRotateAnimation->start();
  } else {
    mOrientation->setValue(WbRotation(mFinalOrientationQuaternion));
    emit refreshRequired();
    resetAnimations();
  }
}

void WbViewpoint::moveTo(const WbVector3 &targetPosition, const WbRotation &targetRotation, bool movingToAxis) {
  resetAnimations();
  lock();
  WbVector3 differenceVector = targetPosition - mPosition->value();
  double distance = differenceVector.length();
  // don't animate if the target position is very close to avoid numerical errors
  if (distance > 0.00001) {
    mInitialMoveToPosition = mPosition->value();
    mMoveToDirection = differenceVector / distance;
    mTranslateAnimation = new QVariantAnimation(this);
    mTranslateAnimation->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
    mTranslateAnimation->setDuration(ANIMATION_DURATION);
    mTranslateAnimation->setStartValue(0.0);
    mTranslateAnimation->setEndValue(distance);
    connect(mTranslateAnimation, &QVariantAnimation::valueChanged, this, &WbViewpoint::translateAnimationStep);
    connect(mTranslateAnimation, &QVariantAnimation::finished, this, &WbViewpoint::resetAnimations);
    mTranslateAnimation->start();
  } else {
    unlock();
    mPosition->setValue(targetPosition);
    emit refreshRequired();
  }

  if (mOrientation->value().direction().dot(targetRotation.direction()) < 0.99994) {
    // build the start and end quaternions for camera orientation
    mInitialOrientationQuaternion = WbQuaternion(mOrientation->value().axis(), mOrientation->value().angle());
    mFinalOrientationQuaternion = WbQuaternion(targetRotation.axis(), targetRotation.angle());
    mFinalOrientationQuaternion.normalize();
    mRotateAnimation = new QVariantAnimation(this);
    mRotateAnimation->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
    mRotateAnimation->setDuration(ANIMATION_DURATION);
    mRotateAnimation->setStartValue(0.0);
    mRotateAnimation->setEndValue(1.0);
    connect(mRotateAnimation, &QVariantAnimation::valueChanged, this, &WbViewpoint::rotateAnimationStep);
    connect(mRotateAnimation, &QVariantAnimation::finished, this, &WbViewpoint::resetAnimations);
    mRotateAnimation->start();
  } else {
    unlock();
    mOrientation->setValue(targetRotation);
    emit refreshRequired();
  }
}

void WbViewpoint::resetAnimations() {
  delete mTranslateAnimation;
  mTranslateAnimation = NULL;

  delete mRotateAnimation;
  mRotateAnimation = NULL;

  delete mOrbitAnimation;
  mOrbitAnimation = NULL;

  mOrbitRadius = 0.0;
  unlock();
  unlockRotationCenter();
}

void WbViewpoint::translateAnimationStep(const QVariant &value) {
  mPosition->setValue(mInitialMoveToPosition + value.toDouble() * mMoveToDirection);
  emit refreshRequired();
}

void WbViewpoint::rotateAnimationStep(const QVariant &value) {
  WbQuaternion slerpedQuaternion(
    WbQuaternion::slerp(mInitialOrientationQuaternion, mFinalOrientationQuaternion, value.toDouble()));
  slerpedQuaternion.normalize();
  // deal with numerical errors when slerping to identity quaternion
  if (WbRotation(slerpedQuaternion).direction().isNan())
    mOrientation->setValue(WbRotation(mFinalOrientationQuaternion));
  else
    mOrientation->setValue(WbRotation(slerpedQuaternion));
  emit refreshRequired();
}

void WbViewpoint::translateOrbitAnimationStep(const QVariant &value) {
  WbQuaternion slerpedQuaternion(WbQuaternion::slerp(mInitialOrbitQuaternion, mFinalOrbitQuaternion, value.toDouble()));
  mPosition->setValue(mRotationCenter + (mCenterToViewpointUnitVector * WbMatrix3(slerpedQuaternion)) * mOrbitRadius);
}

void WbViewpoint::rotateOrbitAnimationStep(const QVariant &value) {
  WbRotation normalisedRotation = mOrientation->value();
  normalisedRotation.normalizeAxis();
  mOrientation->setValue(normalisedRotation);
  lookAt(mRotationCenter, mOrientation->value().up());
  emit refreshRequired();
}

void WbViewpoint::lookAtAnimationStep(const QVariant &value) {
  WbQuaternion slerpedQuaternion(WbQuaternion::slerp(mLookAtInitialQuaternion, mLookAtFinalQuaternion, value.toDouble()));
  slerpedQuaternion.normalize();
  // deal with numerical errors when slerping to identity quaternion
  if (WbRotation(slerpedQuaternion).direction().isNan())
    mOrientation->setValue(WbRotation(mLookAtFinalQuaternion));
  else
    mOrientation->setValue(WbRotation(slerpedQuaternion));
  emit refreshRequired();
}

void WbViewpoint::exportNodeFields(WbVrmlWriter &writer) const {
  WbBaseNode::exportNodeFields(writer);

  if (writer.isX3d()) {
    writer << " exposure=\'" << mExposure->value() << "\'";
    writer << " bloomThreshold=\'" << mBloomThreshold->value() << "\'";
    writer << " zNear=\'" << mNear->value() << "\'";
    writer << " zFar=\'" << mFar->value() << "\'";
    writer << " followSmoothness=\'" << mFollowSmoothness->value() << "\'";
    writer << " ambientOcclusionRadius=\'" << mAmbientOcclusionRadius->value() << "\'";
    if (mFollowedSolid)
      writer << " followedId=\'n" << QString::number(mFollowedSolid->uniqueId()) << "\'";
  }
}
