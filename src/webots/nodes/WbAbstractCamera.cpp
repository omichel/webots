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

#include "WbAbstractCamera.hpp"

#include "WbBackground.hpp"
#include "WbFieldChecker.hpp"
#include "WbLens.hpp"
#include "WbLight.hpp"
#include "WbLog.hpp"
#include "WbPerformanceLog.hpp"
#include "WbPreferences.hpp"
#include "WbProtoModel.hpp"
#include "WbRgb.hpp"
#include "WbSFNode.hpp"
#include "WbSimulationState.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"
#include "WbWrenCamera.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"
#include "WbWrenTextureOverlay.hpp"

#include "../../controller/c/messages.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#ifndef _WIN32
#include "WbPosixSharedMemory.hpp"
#else
#include <QtCore/QSharedMemory>
#endif
#include <QtCore/QVector>

#include <wren/config.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

int WbAbstractCamera::cCameraNumber = 0;
int WbAbstractCamera::cCameraCounter = 0;

void WbAbstractCamera::init() {
  mImageShm = NULL;
  mImageData = NULL;
  mWrenCamera = NULL;
  mSensor = NULL;
  mRefreshRate = 0;
  mNeedToConfigure = false;
  mHasSharedMemoryChanged = false;
  mNeedToCheckShaderErrors = false;
  mSharedMemoryReset = false;
  mExternalWindowEnabled = false;
  mCharType = 0;

  mTransform = NULL;
  mRenderable = NULL;
  mMesh = NULL;
  mMaterial = NULL;

  mFrustumDisplayTransform = NULL;
  mFrustumDisplayRenderable = NULL;
  mFrustumDisplayMesh = NULL;
  mFrustumDisplayMaterial = NULL;

  // Fields initialization
  mFieldOfView = findSFDouble("fieldOfView");
  mSpherical = findSFBool("spherical");
  mNear = findSFDouble("near");
  mMotionBlur = findSFDouble("motionBlur");
  mNoise = findSFDouble("noise");
  mLens = findSFNode("lens");
  mImageChanged = false;
}

WbAbstractCamera::WbAbstractCamera(const QString &modelName, WbTokenizer *tokenizer) : WbRenderingDevice(modelName, tokenizer) {
  init();
}

WbAbstractCamera::WbAbstractCamera(const WbAbstractCamera &other) : WbRenderingDevice(other) {
  init();
}

WbAbstractCamera::WbAbstractCamera(const WbNode &other) : WbRenderingDevice(other) {
  init();
}

WbAbstractCamera::~WbAbstractCamera() {
  delete mImageShm;
  delete mSensor;

  if (areWrenObjectsInitialized())
    deleteWren();
}

void WbAbstractCamera::preFinalize() {
  WbRenderingDevice::preFinalize();

  if (lens())
    lens()->preFinalize();

  mSensor = new WbSensor();

  updateFieldOfView();
  updateSpherical();
  updateNoise();
}

void WbAbstractCamera::postFinalize() {
  WbRenderingDevice::postFinalize();

  if (lens()) {
    lens()->postFinalize();
    updateLens();
  }

  connect(mFieldOfView, &WbSFDouble::changed, this, &WbAbstractCamera::updateFieldOfView);
  connect(mSpherical, &WbSFBool::changed, this, &WbAbstractCamera::updateSpherical);
  connect(mNoise, &WbSFDouble::changed, this, &WbAbstractCamera::updateNoise);
  if (mLens)
    connect(mLens, &WbSFNode::changed, this, &WbAbstractCamera::updateLens);
  if (mMotionBlur)
    connect(mMotionBlur, &WbSFDouble::changed, this, &WbAbstractCamera::updateMotionBlur);
  connect(mSensor, &WbSensor::stateChanged, this, &WbAbstractCamera::applyFrustumToWren);
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
          &WbAbstractCamera::updateOptionalRendering);
}

void WbAbstractCamera::updateOptionalRendering(int option) {
  if (areWrenObjectsInitialized() && ((isRangeFinder() && option == WbWrenRenderingContext::VF_RANGE_FINDER_FRUSTUMS) ||
                                      (!isRangeFinder() && option == WbWrenRenderingContext::VF_CAMERA_FRUSTUMS)))
    applyFrustumToWren();
}

WbLens *WbAbstractCamera::lens() const {
  if (mLens)
    return dynamic_cast<WbLens *>(mLens->value());
  else
    return NULL;
}

void WbAbstractCamera::deleteWren() {
  if (mWrenCamera) {
    disconnect(mWrenCamera, &WbWrenCamera::cameraInitialized, this, &WbAbstractCamera::applyCameraSettingsToWren);
    delete mWrenCamera;
    mWrenCamera = NULL;
  }

  if (areWrenObjectsInitialized()) {
    wr_static_mesh_delete(mMesh);
    wr_node_delete(WR_NODE(mRenderable));
    wr_node_delete(WR_NODE(mTransform));
    wr_material_delete(mMaterial);

    wr_static_mesh_delete(mFrustumDisplayMesh);
    wr_node_delete(WR_NODE(mFrustumDisplayRenderable));
    wr_node_delete(WR_NODE(mFrustumDisplayTransform));
    wr_material_delete(mFrustumDisplayMaterial);
  }

  cCameraCounter--;
  if (cCameraCounter == 0)
    resetStaticCounters();
}

void WbAbstractCamera::initializeImageSharedMemory() {
  mHasSharedMemoryChanged = true;
  delete mImageShm;
  mImageShm = initializeSharedMemory();
  if (mImageShm)
    mImageData = (unsigned char *)mImageShm->data();
}

WbSharedMemory *WbAbstractCamera::initializeSharedMemory() {
  QString sharedMemoryName =
    QString("Webots_Camera_Image_%1_%2").arg((long)QCoreApplication::applicationPid()).arg(cCameraNumber);
  WbSharedMemory *imageShm = new WbSharedMemory(sharedMemoryName);
  // A controller of the previous simulation may have not released cleanly the shared memory (e.g. when the controller crashes).
  // This can be detected by trying to attach, and the shared memory may be cleaned by detaching.
  if (imageShm->attach())
    imageShm->detach();
  if (!imageShm->create(size())) {
    QString message = tr("Cannot allocate shared memory. The shared memory is required for the cameras. The shared memory of "
                         "your OS is probably full. Please check your shared memory setup.");
    warn(message);
    delete imageShm;
    return NULL;
  }
  return imageShm;
}

void WbAbstractCamera::setup() {
  cCameraNumber++;
  cCameraCounter++;

  initializeImageSharedMemory();
  if (!mImageShm)
    return;

  WbRenderingDevice::setup();

  createWrenCamera();
  createWrenOverlay();

  if (!spherical()) {
    updateFrustumDisplay();
    connect(mWrenCamera, &WbWrenCamera::cameraInitialized, this, &WbAbstractCamera::updateFrustumDisplay);
    connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
            &WbAbstractCamera::updateFrustumDisplayIfNeeded);
  }
}

bool WbAbstractCamera::needToRender() const {
  return mSensor->isEnabled() && mSensor->needToRefresh();
}

void WbAbstractCamera::updateCameraTexture() {
  if (isPowerOn() && needToRender()) {
    // update camera overlay before main rendering
    computeValue();
    if (WbAbstractCamera::needToRender()) {
      mSensor->updateTimer();
      mImageChanged = true;
    }
  } else if (mOverlay && mSensor->isEnabled() && mSensor->isRemoteModeEnabled())
    // keep updating the overlay in remote mode
    mOverlay->requestUpdateTexture();
}

// Generally, computeValue() acquires the data from the RTT
// handle and copies the resulting value in the shared memory
void WbAbstractCamera::computeValue() {
  if (!hasBeenSetup())
    return;

  WbPerformanceLog *log = WbPerformanceLog::instance();
  if (log)
    log->startMeasure(WbPerformanceLog::DEVICE_RENDERING, deviceName());

  // enable viewpoint's invisible nodes
  // they will be re-disabled after the controller step
  WbViewpoint *viewpoint = WbWorld::instance()->viewpoint();
  viewpoint->enableNodeVisibility(true);

  const int invisibleNodesCount = mInvisibleNodes.size();
  for (int i = 0; i < invisibleNodesCount; ++i)
    wr_node_set_visible(WR_NODE(mInvisibleNodes.at(i)->wrenNode()), false);

  if (WbAbstractCamera::needToRender())
    mWrenCamera->render();
  render();

  for (int i = 0; i < invisibleNodesCount; ++i)
    wr_node_set_visible(WR_NODE(mInvisibleNodes.at(i)->wrenNode()), true);

  if (log)
    log->stopMeasure(WbPerformanceLog::DEVICE_RENDERING, deviceName());
}

void WbAbstractCamera::copyImageToSharedMemory(WbWrenCamera *camera, unsigned char *data) {
  if (camera) {
    camera->enableCopying(true);
    camera->copyContentsToMemory(data);
  }
}

void WbAbstractCamera::reset(const QString &id) {
  WbRenderingDevice::reset(id);

  if (mLens) {
    WbNode *const l = mLens->value();
    if (l)
      l->reset(id);
  }

  for (int i = 0; i < mInvisibleNodes.size(); ++i)
    disconnect(mInvisibleNodes.at(i), &QObject::destroyed, this, &WbAbstractCamera::removeInvisibleNodeFromList);
  mInvisibleNodes.clear();
}

void WbAbstractCamera::resetSharedMemory() {
  if (hasBeenSetup()) {
    // the previous shared memory will be released by the new controller start
    cCameraNumber++;
    initializeImageSharedMemory();
    mSharedMemoryReset = true;
  }
}

void WbAbstractCamera::writeConfigure(QDataStream &stream) {
  mSensor->connectToRobotSignal(robot());
  addConfigureToStream(stream);
}

void WbAbstractCamera::writeAnswer(QDataStream &stream) {
  if (mImageChanged) {
    copyImageToSharedMemory(mWrenCamera, image());
    mSensor->resetPendingValue();
    mImageChanged = false;
  }

  if (mNeedToConfigure)
    addConfigureToStream(stream, true);

  if (mHasSharedMemoryChanged && mImageShm) {
    stream << (short unsigned int)tag();
    stream << (unsigned char)C_CAMERA_SHARED_MEMORY;
    if (mImageShm) {
      stream << (int)(mImageShm->size());
      QByteArray n = QFile::encodeName(mImageShm->nativeKey());
      stream.writeRawData(n.constData(), n.size() + 1);
    } else
      stream << (int)(0);
    mHasSharedMemoryChanged = false;
  }
}

void WbAbstractCamera::addConfigureToStream(QDataStream &stream, bool reconfigure) {
  stream << (short unsigned int)tag();
  if (reconfigure)
    stream << (unsigned char)C_CAMERA_RECONFIGURE;
  else {
    stream << (unsigned char)C_CONFIGURE;
    stream << (unsigned int)uniqueId();
    stream << (unsigned short)width();
    stream << (unsigned short)height();
  }
  stream << (double)fieldOfView();
  stream << (double)minRange();
  stream << (unsigned char)mSpherical->value();

  mNeedToConfigure = false;
  if (!reconfigure && !mSharedMemoryReset)
    mHasSharedMemoryChanged = false;
  mSharedMemoryReset = false;
}

bool WbAbstractCamera::handleCommand(QDataStream &stream, unsigned char command) {
  bool commandHandled = true;
  switch (command) {
    case C_SET_SAMPLING_PERIOD:
      stream >> mRefreshRate;
      mSensor->setRefreshRate(mRefreshRate);

      // update motion blur factor
      applyMotionBlurToWren();

      emit enabled(this, isEnabled());
      copyImageToSharedMemory(mWrenCamera, image());

      if (!hasBeenSetup()) {
        setup();
        mHasSharedMemoryChanged = true;
      }
      break;
    default:
      commandHandled = false;
  }
  return commandHandled;
}

void WbAbstractCamera::setNodeVisibility(WbBaseNode *node, bool visible) {
  if (visible)
    mInvisibleNodes.removeAll(node);
  else if (!mInvisibleNodes.contains(node)) {
    mInvisibleNodes.append(node);
    connect(node, &QObject::destroyed, this, &WbAbstractCamera::removeInvisibleNodeFromList, Qt::UniqueConnection);
  }
}

void WbAbstractCamera::removeInvisibleNodeFromList(QObject *node) {
  WbBaseNode *const baseNode = static_cast<WbBaseNode *>(node);
  mInvisibleNodes.removeAll(baseNode);
}

void WbAbstractCamera::createWrenObjects() {
  WbRenderingDevice::createWrenObjects();

  // Frustum
  mMaterial = wr_phong_material_new();
  wr_material_set_default_program(mMaterial, WbWrenShaders::lineSetShader());
  wr_phong_material_set_color_per_vertex(mMaterial, true);

  mRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mRenderable, false);
  wr_renderable_set_receive_shadows(mRenderable, false);
  wr_renderable_set_material(mRenderable, mMaterial, NULL);
  wr_renderable_set_drawing_mode(mRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);

  mTransform = wr_transform_new();
  wr_transform_attach_child(mTransform, WR_NODE(mRenderable));
  wr_transform_attach_child(wrenNode(), WR_NODE(mTransform));

  applyFrustumToWren();

  // Frustum display
  mFrustumDisplayTransform = wr_transform_new();
  mFrustumDisplayRenderable = wr_renderable_new();
  mFrustumDisplayMesh = wr_static_mesh_unit_rectangle_new(false);
  mFrustumDisplayMaterial = wr_phong_material_new();

  wr_material_set_default_program(mFrustumDisplayMaterial, WbWrenShaders::simpleShader());
  wr_phong_material_set_transparency(mFrustumDisplayMaterial, 0.5f);
  wr_renderable_set_material(mFrustumDisplayRenderable, mFrustumDisplayMaterial, NULL);
  wr_renderable_set_mesh(mFrustumDisplayRenderable, WR_MESH(mFrustumDisplayMesh));
  wr_renderable_set_face_culling(mFrustumDisplayRenderable, false);

  if (isRangeFinder())
    wr_renderable_set_visibility_flags(mFrustumDisplayRenderable, WbWrenRenderingContext::VF_RANGE_FINDER_FRUSTUMS);
  else
    wr_renderable_set_visibility_flags(mFrustumDisplayRenderable, WbWrenRenderingContext::VF_CAMERA_FRUSTUMS);

  wr_node_set_visible(WR_NODE(mFrustumDisplayTransform), false);
  wr_transform_attach_child(mFrustumDisplayTransform, WR_NODE(mFrustumDisplayRenderable));
  wr_transform_attach_child(wrenNode(), WR_NODE(mFrustumDisplayTransform));

  createWrenOverlay();
}

void WbAbstractCamera::createWrenCamera() {
  if (mWrenCamera)
    disconnect(mWrenCamera, &WbWrenCamera::cameraInitialized, this, &WbAbstractCamera::applyCameraSettingsToWren);
  delete mWrenCamera;

  // Assumption: Qt ensure that the following slots are calling sequentially.
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::backgroundColorChanged, this,
          &WbAbstractCamera::updateBackground, Qt::UniqueConnection);
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::backgroundColorChanged, this,
          &WbAbstractCamera::updatePostProcessingEffect, Qt::UniqueConnection);

  // create the camera
  bool enableAntiAliasing = antiAliasing() && !WbPreferences::instance()->value("OpenGL/disableAntiAliasing", true).toBool();
  mWrenCamera = new WbWrenCamera(wrenNode(), width(), height(), nearValue(), minRange(), maxRange(), fieldOfView(), mCharType,
                                 enableAntiAliasing, mSpherical->value());
  updateBackground();

  connect(mWrenCamera, &WbWrenCamera::cameraInitialized, this, &WbAbstractCamera::applyCameraSettingsToWren);

  applyNearToWren();
  applyFieldOfViewToWren();
  applyMotionBlurToWren();
  applyLensToWren();
  applyNoiseToWren();

  if (mExternalWindowEnabled)
    updateTextureUpdateNotifications(mExternalWindowEnabled);
}

void WbAbstractCamera::updateBackground() {
  if (!mWrenCamera)
    return;

  WbBackground *background = WbBackground::firstInstance();
  if (background)
    mWrenCamera->setBackgroundColor(background->skyColor());
  else
    mWrenCamera->setBackgroundColor(WbRgb());
}

void WbAbstractCamera::updatePostProcessingEffect() {
  applyCameraSettingsToWren();
  computeValue();
}

void WbAbstractCamera::applyCameraSettingsToWren() {
  createWrenOverlay();
  applyMotionBlurToWren();
  applyLensToWren();
  applyNoiseToWren();
}

void WbAbstractCamera::createWrenOverlay() {
  QStringList previousSettings;
  if (mOverlay)
    previousSettings = mOverlay->perspective();

  delete mOverlay;
  mOverlay = NULL;

  if (mWrenCamera) {
    mWrenCamera->setSize(width(), height());
    if (mCharType == 'r')
      mOverlay = new WbWrenTextureOverlay(image(), width(), height(), WbWrenTextureOverlay::TEXTURE_TYPE_DEPTH,
                                          WbWrenTextureOverlay::OVERLAY_TYPE_RANGE_FINDER, mWrenCamera->getWrenTexture(),
                                          maxRange(), true, false);
    else if (mCharType == 'c')
      mOverlay =
        new WbWrenTextureOverlay(image(), width(), height(), WbWrenTextureOverlay::TEXTURE_TYPE_BGRA,
                                 WbWrenTextureOverlay::OVERLAY_TYPE_CAMERA, mWrenCamera->getWrenTexture(), 1.0, false, false);
  } else {
    if (mCharType == 'r')
      mOverlay = new WbWrenTextureOverlay(image(), width(), height(), WbWrenTextureOverlay::TEXTURE_TYPE_DEPTH,
                                          WbWrenTextureOverlay::OVERLAY_TYPE_RANGE_FINDER);
    else if (mCharType == 'c')
      mOverlay = new WbWrenTextureOverlay(image(), width(), height(), WbWrenTextureOverlay::TEXTURE_TYPE_BGRA,
                                          WbWrenTextureOverlay::OVERLAY_TYPE_CAMERA, NULL, 1.0, false, false);
  }
  if (!mOverlay)
    return;
  if (mCharType == 'r')
    mOverlay->setMaxRange(maxRange());

  applyWorldSettings();

  if (!previousSettings.isEmpty())
    mOverlay->restorePerspective(previousSettings, areOverlaysEnabled());
  else
    mOverlay->setVisible(true, areOverlaysEnabled());

  emit textureIdUpdated(textureGLId(), MAIN_TEXTURE);
}

/////////////////////
//  Update methods //
/////////////////////

void WbAbstractCamera::updateWidth() {
  WbRenderingDevice::updateWidth();

  if (areWrenObjectsInitialized() && !hasBeenSetup()) {
    createWrenOverlay();
    applyFrustumToWren();
  }
}

void WbAbstractCamera::updateHeight() {
  WbRenderingDevice::updateHeight();

  if (areWrenObjectsInitialized() && !hasBeenSetup()) {
    createWrenOverlay();
    applyFrustumToWren();
  }
}

void WbAbstractCamera::updateLens() {
  if (lens()) {
    connect(lens(), &WbLens::centerChanged, this, &WbAbstractCamera::applyLensToWren);
    connect(lens(), &WbLens::radialCoefficientsChanged, this, &WbAbstractCamera::applyLensToWren);
    connect(lens(), &WbLens::tangentialCoefficientsChanged, this, &WbAbstractCamera::applyLensToWren);
  }
  if (hasBeenSetup())
    applyLensToWren();
}

void WbAbstractCamera::updateFieldOfView() {
  if (WbFieldChecker::resetDoubleIfNonPositive(this, mFieldOfView, 0.7854))
    return;
  if (!mSpherical->value() && fieldOfView() > M_PI) {
    parsingWarn(
      tr("Invalid 'fieldOfView' changed to 0.7854. The field of view is limited to pi if the 'spherical' field is FALSE."));
    mFieldOfView->setValue(0.7854);
    return;
  }

  mNeedToConfigure = true;

  if (hasBeenSetup())
    applyFieldOfViewToWren();

  if (areWrenObjectsInitialized()) {
    applyFrustumToWren();
    if (!spherical() && hasBeenSetup())
      updateFrustumDisplay();
  }
}

void WbAbstractCamera::updateSpherical() {
  mNeedToConfigure = true;

  if (hasBeenSetup()) {
    wr_node_set_visible(WR_NODE(mFrustumDisplayTransform), false);
    setup();
  }

  if (areWrenObjectsInitialized())
    applyFrustumToWren();

  updateFieldOfView();
}

void WbAbstractCamera::updateAntiAliasing() {
  if (hasBeenSetup())
    setup();
}

void WbAbstractCamera::updateMotionBlur() {
  if (!mMotionBlur || WbFieldChecker::resetDoubleIfNegative(this, mMotionBlur, 0.0))
    return;

  if (hasBeenSetup())
    applyMotionBlurToWren();
}

void WbAbstractCamera::updateNoise() {
  if (WbFieldChecker::resetDoubleIfNegative(this, mNoise, 0.0))
    return;

  if (hasBeenSetup())
    applyNoiseToWren();
}

/////////////////////
//  Apply methods  //
/////////////////////

void WbAbstractCamera::applyNearToWren() {
  mWrenCamera->setNear(nearValue());
}

void WbAbstractCamera::applyFieldOfViewToWren() {
  mWrenCamera->setFieldOfView(mFieldOfView->value());
}

void WbAbstractCamera::applyMotionBlurToWren() {
  if (mMotionBlur && hasBeenSetup()) {
    if (mRefreshRate > 0) {
      // 0.005 because the threshold is 99.5%
      double factor = pow(0.005, (double)mRefreshRate / mMotionBlur->value());
      mWrenCamera->setMotionBlur(factor);
    }
  }
}

void WbAbstractCamera::applyLensToWren() {
  if (hasBeenSetup()) {
    if (lens()) {
      mWrenCamera->enableLensDistortion();
      mWrenCamera->setLensDistortionCenter(lens()->center());
      mWrenCamera->setRadialLensDistortionCoefficients(lens()->radialCoefficients());
      mWrenCamera->setTangentialLensDistortionCoefficients(lens()->tangentialCoefficients());
    } else
      mWrenCamera->disableLensDistortion();
  }
}

void WbAbstractCamera::applyNoiseToWren() {
  if (mCharType == 'c')
    mWrenCamera->setColorNoise(mNoise->value());
  else
    mWrenCamera->setRangeNoise(mNoise->value());
}

///////////////////////
//  Drawing methods  //
///////////////////////
static void addVertex(QVector<float> &vertices, QVector<float> &colors, const float *vertex, const float *color) {
  vertices.push_back(vertex[0]);
  vertices.push_back(vertex[1]);
  vertices.push_back(vertex[2]);

  colors.push_back(color[0]);
  colors.push_back(color[1]);
  colors.push_back(color[2]);
}

static void drawCube(QVector<float> &vertices, QVector<float> &colors, float n, const float *color) {
  const float cubeVertices[8][3] = {{n, n, n},  {-n, n, n},  {-n, n, -n},  {n, n, -n},
                                    {n, -n, n}, {-n, -n, n}, {-n, -n, -n}, {n, -n, -n}};

  for (int i = 0; i < 4; ++i) {
    // Top edge
    addVertex(vertices, colors, cubeVertices[i], color);
    addVertex(vertices, colors, cubeVertices[(i + 1) % 4], color);

    // Bottom edge
    addVertex(vertices, colors, cubeVertices[i + 4], color);
    addVertex(vertices, colors, cubeVertices[((i + 1) % 4) + 4], color);

    // Side edge
    addVertex(vertices, colors, cubeVertices[i], color);
    addVertex(vertices, colors, cubeVertices[i + 4], color);
  }
}

static void drawRectangle(QVector<float> &vertices, QVector<float> &colors, const float v[4][3], const float *color) {
  for (int i = 0; i < 4; ++i) {
    addVertex(vertices, colors, v[i], color);
    addVertex(vertices, colors, v[(i + 1) % 4], color);
  }
}

void WbAbstractCamera::applyFrustumToWren() {
  wr_node_set_visible(WR_NODE(mTransform), false);

  wr_static_mesh_delete(mMesh);
  mMesh = NULL;

  if (!isFrustumEnabled())
    return;

  WbRgb frustumColorRgb;
  if (mSensor->isEnabled() && mSensor->isFirstValueReady())
    frustumColorRgb = enabledCameraFrustrumColor();
  else
    frustumColorRgb = disabledCameraFrustrumColor();

  const float frustumColor[] = {static_cast<float>(frustumColorRgb.red()), static_cast<float>(frustumColorRgb.green()),
                                static_cast<float>(frustumColorRgb.blue())};
  wr_phong_material_set_color(mMaterial, frustumColor);

  bool drawFarPlane;
  float f;
  const float n = minRange();
  // if the far is set to 0 it means the far clipping plane is set to infinity
  // so, the far distance of the colored frustum should be set arbitrarily
  if (mCharType == 'c' && maxRange() == 0.0f) {
    f = n + 2.0f * wr_config_get_line_scale();
    drawFarPlane = false;
  } else {
    f = maxRange();
    drawFarPlane = true;
  }

  const float w = width();
  const float h = height();
  const float fovX = mFieldOfView->value();
  const float fovY = WbWrenCamera::computeFieldOfViewY(fovX, w / h);  // fovX -> fovY
  const float t = tanf(fovX / 2.0f);
  const float dw1 = n * t;
  const float dh1 = dw1 * h / w;
  const float n1 = -n;
  const float dw2 = f * t;
  const float dh2 = dw2 * h / w;
  const float n2 = -f;

  QVector<float> vertices;
  QVector<float> colors;
  float vertex[3] = {0.0f, 0.0f, 0.0f};
  addVertex(vertices, colors, vertex, frustumColor);
  vertex[2] = -n;
  addVertex(vertices, colors, vertex, frustumColor);

  // creation of the near plane
  if (hasBeenSetup() && mWrenCamera->isSpherical()) {
    const float cubeColor[3] = {0.0f, 0.0f, 0.0f};
    drawCube(vertices, colors, n, cubeColor);

    const float n95 = 0.95f * n;
    if (mWrenCamera->isSubCameraActive(WbWrenCamera::CAMERA_ORIENTATION_FRONT)) {
      const float pos[4][3] = {{n95, n95, -n}, {n95, -n95, -n}, {-n95, -n95, -n}, {-n95, n95, -n}};
      drawRectangle(vertices, colors, pos, frustumColor);
    }
    if (mWrenCamera->isSubCameraActive(WbWrenCamera::CAMERA_ORIENTATION_DOWN)) {
      const float pos0[4][3] = {{n95, -n, n95}, {n95, -n, -n95}, {-n95, -n, -n95}, {-n95, -n, n95}};
      const float pos1[4][3] = {{n95, n, n95}, {n95, n, -n95}, {-n95, n, -n95}, {-n95, n, n95}};
      drawRectangle(vertices, colors, pos0, frustumColor);
      drawRectangle(vertices, colors, pos1, frustumColor);
    }
    if (mWrenCamera->isSubCameraActive(WbWrenCamera::CAMERA_ORIENTATION_LEFT)) {
      const float pos0[4][3] = {{-n, n95, n95}, {-n, n95, -n95}, {-n, -n95, -n95}, {-n, -n95, n95}};
      const float pos1[4][3] = {{n, n95, n95}, {n, n95, -n95}, {n, -n95, -n95}, {n, -n95, n95}};
      drawRectangle(vertices, colors, pos0, frustumColor);
      drawRectangle(vertices, colors, pos1, frustumColor);
    }
    if (mWrenCamera->isSubCameraActive(WbWrenCamera::CAMERA_ORIENTATION_BACK)) {
      const float pos[4][3] = {{n95, n95, n}, {n95, -n95, n}, {-n95, -n95, n}, {-n95, n95, n}};
      drawRectangle(vertices, colors, pos, frustumColor);
    }
  } else {
    const float pos[4][3] = {{dw1, dh1, n1}, {dw1, -dh1, n1}, {-dw1, -dh1, n1}, {-dw1, dh1, n1}};
    drawRectangle(vertices, colors, pos, frustumColor);
  }

  // Creation of the far plane
  // if the camera is not of the range-finder type, the far is set to infinity
  // so, the far rectangle of the colored frustum shouldn't be set
  if (drawFarPlane && !mSpherical->value()) {
    const float pos[4][3] = {{dw2, dh2, n2}, {dw2, -dh2, n2}, {-dw2, -dh2, n2}, {-dw2, dh2, n2}};
    drawRectangle(vertices, colors, pos, frustumColor);
  }

  const float zero[3] = {0.0f, 0.0f, 0.0f};
  // Creation of the external outline of the frustum (4 lines)
  if (mSpherical->value()) {
    const float angleY[4] = {-fovY / 2.0f, -fovY / 2.0f, fovY / 2.0f, fovY / 2.0f};
    const float angleX[4] = {fovX / 2.0f, -fovX / 2.0f, -fovX / 2.0f, fovX / 2.0f};
    for (int k = 0; k < 4; ++k) {
      const float helper = cosf(angleY[k]);
      // get x, y and z from the spherical coordinates
      float x = 0.0f;
      if (angleY[k] > M_PI_4 || angleY[k] < -M_PI_4)
        x = f * cosf(angleY[k] + M_PI_2) * sinf(angleX[k]);
      else
        x = f * helper * sinf(angleX[k]);
      const float y = f * sinf(angleY[k]);
      const float z = -f * helper * cosf(angleX[k]);
      addVertex(vertices, colors, zero, frustumColor);
      const float outlineVertex[3] = {x, y, z};
      addVertex(vertices, colors, outlineVertex, frustumColor);
    }
  } else {
    const float frustumOutline[8][3] = {{dw1, dh1, n1},  {dw2, dh2, n2},  {-dw1, dh1, n1},  {-dw2, dh2, n2},
                                        {dw1, -dh1, n1}, {dw2, -dh2, n2}, {-dw1, -dh1, n1}, {-dw2, -dh2, n2}};
    for (int i = 0; i < 8; ++i)
      addVertex(vertices, colors, frustumOutline[i], frustumColor);
  }

  mMesh = wr_static_mesh_line_set_new(vertices.size() / 3, &vertices[0], &colors[0]);
  wr_renderable_set_mesh(mRenderable, WR_MESH(mMesh));

  if (isRangeFinder())
    wr_renderable_set_visibility_flags(mRenderable, WbWrenRenderingContext::VF_RANGE_FINDER_FRUSTUMS);
  else
    wr_renderable_set_visibility_flags(mRenderable, WbWrenRenderingContext::VF_CAMERA_FRUSTUMS);

  wr_node_set_visible(WR_NODE(mTransform), true);
}

void WbAbstractCamera::updateFrustumDisplay() {
  wr_node_set_visible(WR_NODE(mFrustumDisplayTransform), false);

  if (!mWrenCamera || !isFrustumEnabled())
    return;

  const float n = minRange();
  const float quadWidth = 2.0f * n * tanf(mFieldOfView->value() / 2.0f);
  const float translation[3] = {0.0f, 0.0f, -n};
  const float orientation[4] = {M_PI / 2.0f, 1.0f, 0.0f, 0.0f};
  const float scale[3] = {quadWidth, 1.0f, (quadWidth * height()) / width()};

  wr_transform_set_position(mFrustumDisplayTransform, translation);
  wr_transform_set_orientation(mFrustumDisplayTransform, orientation);
  wr_transform_set_scale(mFrustumDisplayTransform, scale);
  wr_material_set_texture(mFrustumDisplayMaterial, mWrenCamera->getWrenTexture(), 0);
  wr_node_set_visible(WR_NODE(mFrustumDisplayTransform), true);
}

void WbAbstractCamera::updateTextureUpdateNotifications(bool enabled) {
  assert(mWrenCamera);
  if (enabled)
    connect(mWrenCamera, &WbWrenCamera::textureUpdated, this, &WbRenderingDevice::textureUpdated, Qt::UniqueConnection);
  else
    disconnect(mWrenCamera, &WbWrenCamera::textureUpdated, this, &WbRenderingDevice::textureUpdated);
  mWrenCamera->enableTextureUpdateNotifications(enabled);
}

void WbAbstractCamera::enableExternalWindowForAttachedCamera(bool enabled) {
  if (mExternalWindowEnabled)
    return;
  updateTextureUpdateNotifications(enabled);
}

void WbAbstractCamera::enableExternalWindow(bool enabled) {
  WbRenderingDevice::enableExternalWindow(enabled);
  mExternalWindowEnabled = enabled;
  if (mWrenCamera)
    updateTextureUpdateNotifications(mExternalWindowEnabled);
}
