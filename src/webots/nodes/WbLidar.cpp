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

#include "WbLidar.hpp"

#include "WbBoundingSphere.hpp"
#include "WbFieldChecker.hpp"
#include "WbPerspective.hpp"
#include "WbRgb.hpp"
#include "WbSensor.hpp"
#include "WbSimulationState.hpp"
#include "WbWorld.hpp"
#include "WbWrenCamera.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include "../../controller/c/messages.h"

#include <QtCore/QDataStream>

#include <wren/config.h>
#include <wren/dynamic_mesh.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#define POINT_CLOUD_RAY_REPRESENTATION_THRESHOLD 2500

void WbLidar::init() {
  mCharType = 'l';
  mIsPointCloudEnabled = false;
  mCurrentRotatingAngle = 0;
  mPreviousRotatingAngle = 0;
  mCurrentTiltAngle = 0;
  mTemporaryImage = NULL;

  mTiltAngle = findSFDouble("tiltAngle");
  mHorizontalResolution = findSFInt("horizontalResolution");
  mVerticalFieldOfView = findSFDouble("verticalFieldOfView");
  mNumberOfLayers = findSFInt("numberOfLayers");
  mMinRange = findSFDouble("minRange");
  mMaxRange = findSFDouble("maxRange");
  mResolution = findSFDouble("resolution");
  mDefaultFrequency = findSFDouble("defaultFrequency");
  mMinFrequency = findSFDouble("minFrequency");
  mMaxFrequency = findSFDouble("maxFrequency");
  mType = findSFString("type");
  mRotatingHead = findSFNode("rotatingHead");

  mFrustumMesh = NULL;
  mFrustumRenderable = NULL;
  mFrustumMaterial = NULL;

  mLidarPointsRenderable = NULL;
  mLidarPointsMesh = NULL;
  mLidarPointsMaterial = NULL;

  mLidarRaysRenderable = NULL;
  mLidarRaysMesh = NULL;
  mLidarRaysMaterial = NULL;

  mActualNumberOfLayers = mNumberOfLayers->value();
  mActualHorizontalResolution = mHorizontalResolution->value();
  mActualVerticalFieldOfView = mVerticalFieldOfView->value();
  mActualFieldOfView = mFieldOfView->value();
  mActualType = mType->value();
}

WbLidar::WbLidar(WbTokenizer *tokenizer) : WbAbstractCamera("Lidar", tokenizer) {
  init();
}

WbLidar::WbLidar(const WbLidar &other) : WbAbstractCamera(other) {
  init();
}

WbLidar::WbLidar(const WbNode &other) : WbAbstractCamera(other) {
  init();
}

WbLidar::~WbLidar() {
  delete mTemporaryImage;
  if (areWrenObjectsInitialized())
    deleteWren();
}

void WbLidar::preFinalize() {
  WbAbstractCamera::preFinalize();

  WbBaseNode *const e = dynamic_cast<WbBaseNode *>(mRotatingHead->value());
  if (e && !e->isPreFinalizedCalled())
    e->preFinalize();

  updateNear();
  updateMinRange();
  updateMaxRange();
  updateResolution();
  updateType();
  updateMinFrequency();
  updateMaxFrequency();
  updateDefaultFrequency();
  updateHorizontalResolution();
  updateVerticalFieldOfView();
  updateNumberOfLayers();
}

void WbLidar::postFinalize() {
  WbAbstractCamera::postFinalize();

  WbBaseNode *const e = dynamic_cast<WbBaseNode *>(mRotatingHead->value());
  if (e && !e->isPostFinalizedCalled())
    e->postFinalize();

  connect(mNear, &WbSFDouble::changed, this, &WbLidar::updateNear);
  connect(mMinRange, &WbSFDouble::changed, this, &WbLidar::updateMinRange);
  connect(mMaxRange, &WbSFDouble::changed, this, &WbLidar::updateMaxRange);
  connect(mResolution, &WbSFDouble::changed, this, &WbLidar::updateResolution);
  connect(mTiltAngle, &WbSFDouble::changed, this, &WbLidar::updateTiltAngle);
  connect(mType, &WbSFString::changed, this, &WbLidar::updateType);
  connect(mMinFrequency, &WbSFDouble::changed, this, &WbLidar::updateMinFrequency);
  connect(mMaxFrequency, &WbSFDouble::changed, this, &WbLidar::updateMaxFrequency);
  connect(mDefaultFrequency, &WbSFDouble::changed, this, &WbLidar::updateDefaultFrequency);
  connect(mHorizontalResolution, &WbSFDouble::changed, this, &WbLidar::updateHorizontalResolution);
  connect(mVerticalFieldOfView, &WbSFDouble::changed, this, &WbLidar::updateVerticalFieldOfView);
  connect(mNumberOfLayers, &WbSFInt::changed, this, &WbLidar::updateNumberOfLayers);
  connect(mRotatingHead, &WbSFNode::changed, this, &WbLidar::updateRotatingHead);
}

void WbLidar::reset(const QString &id) {
  WbAbstractCamera::reset(id);

  WbNode *const r = mRotatingHead->value();
  if (r)
    r->reset(id);

  if (mWrenCamera)
    mWrenCamera->rotateYaw(-mCurrentRotatingAngle);

  mIsPointCloudEnabled = false;
  mCurrentRotatingAngle = 0;
  mPreviousRotatingAngle = 0;
  if (mTemporaryImage)
    memset(mTemporaryImage, 0, actualHorizontalResolution() * height() * sizeof(float));
  hidePointCloud();
}

void WbLidar::updateOptionalRendering(int option) {
  if (areWrenObjectsInitialized()) {
    if (option == WbWrenRenderingContext::VF_LIDAR_POINT_CLOUD) {
      if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(option))
        displayPointCloud();
      else
        hidePointCloud();
    } else if (option == WbWrenRenderingContext::VF_LIDAR_RAYS_PATHS)
      applyFrustumToWren();
  }
}

void WbLidar::initializeImageSharedMemory() {
  WbAbstractCamera::initializeImageSharedMemory();
  if (mImageShm) {
    // initialize the shared memory with a black image
    float *im = lidarImage();
    const int size = width() * actualNumberOfLayers();
    for (int i = 0; i < size; i++)
      im[i] = 0.0f;
  }
  mTemporaryImage = new float[actualHorizontalResolution() * height()];
}

QString WbLidar::pixelInfo(int x, int y) const {
  WbRgb color;
  if (hasBeenSetup())
    color = mWrenCamera->copyPixelColourValue(x, y);

  return QString::asprintf("depth(%d,%d)=%f", x, y, color.red());
}

void WbLidar::prePhysicsStep(double ms) {
  WbSolid::prePhysicsStep(ms);
  WbSolid *s = solidEndPoint();
  if (isRotating() && mSensor->isEnabled()) {
    double angle = -(ms * 2 * M_PI * mDefaultFrequency->value()) / 1000;
    if (s)
      s->rotate(WbVector3(0.0, angle, 0.0));
    if (hasBeenSetup()) {
      mWrenCamera->rotateYaw(angle);
      mPreviousRotatingAngle = mCurrentRotatingAngle;
      mCurrentRotatingAngle += angle;
    }
  }
  if (s)
    s->prePhysicsStep(ms);
}

void WbLidar::postPhysicsStep() {
  WbSolid::postPhysicsStep();
  if (isRotating() && mSensor->isEnabled())
    copyAllLayersToSharedMemory();
}

void WbLidar::write(WbVrmlWriter &writer) const {
  if (writer.isWebots())
    WbBaseNode::write(writer);
  else {
    WbBaseNode::write(writer);
    WbSolid *s = solidEndPoint();
    if (s)
      s->write(writer);
  }
}

void WbLidar::addConfigureToStream(QDataStream &stream, bool reconfigure) {
  WbAbstractCamera::addConfigureToStream(stream, reconfigure);
  stream << (double)mMaxRange->value();
  stream << (short)mNumberOfLayers->value();
  stream << (double)mDefaultFrequency->value();
  stream << (double)mMinFrequency->value();
  stream << (double)mMaxFrequency->value();
  stream << (double)mVerticalFieldOfView->value();
  stream << (double)actualHorizontalResolution();
}

void WbLidar::writeAnswer(QDataStream &stream) {
  if (isRotating())
    mImageChanged = false;

  WbAbstractCamera::writeAnswer(stream);

  if (!isRotating() && mSensor->isEnabled())
    copyAllLayersToSharedMemory();
}

void WbLidar::handleMessage(QDataStream &stream) {
  unsigned char command;
  stream >> command;
  if (command == C_SET_SAMPLING_PERIOD) {
    stream >> mRefreshRate;
    if (isRotating())
      mRefreshRate = WbWorld::instance()->basicTimeStep();

    mSensor->setRefreshRate(mRefreshRate);

    emit enabled(this, mSensor->isEnabled());

    if (!hasBeenSetup()) {
      setup();
      mHasSharedMemoryChanged = true;
    }

    return;
  } else if (command == C_LIDAR_ENABLE_POINT_CLOUD) {
    mIsPointCloudEnabled = true;
    return;
  } else if (command == C_LIDAR_DISABLE_POINT_CLOUD) {
    mIsPointCloudEnabled = false;
    hidePointCloud();
    return;
  } else if (command == C_LIDAR_SET_FREQUENCY) {
    double frequency;
    stream >> frequency;
    mDefaultFrequency->setValue(frequency);
    return;
  } else if (WbAbstractCamera::handleCommand(stream, command))
    return;

  assert(0);
}

void WbLidar::copyAllLayersToSharedMemory() {
  if (!hasBeenSetup() || !mImageShm)
    return;

  float *data = lidarImage();
  double skip = 1.0;
  if (height() != actualNumberOfLayers() && actualNumberOfLayers() != 1)
    skip = (double)(height() - 1) / (double)(actualNumberOfLayers() - 1);
  double w = width();
  int resolution = actualHorizontalResolution();
  int minWidth = 0;
  int maxWidth = w;
  int widthOffset = 0;

  mWrenCamera->enableCopying(true);
  mWrenCamera->copyContentsToMemory(mTemporaryImage);
  // if rotating compute which part of the image should be updated
  if (isRotating()) {
    double deltaAngle = fabs(mCurrentRotatingAngle - mPreviousRotatingAngle);
    double ratio = deltaAngle / actualFieldOfView();
    if (ratio > 1.0)
      ratio = 1.0;
    minWidth = ((double)w / 2.0) * (1.0 - ratio);
    maxWidth = ((double)w / 2.0) * (1.0 + ratio);

    double tmpAngle =
      (fabs(mPreviousRotatingAngle / (2.0 * M_PI)) - floor(fabs(mPreviousRotatingAngle / (2.0 * M_PI)))) * (2.0 * M_PI);
    if (tmpAngle < 0)
      tmpAngle += 2.0 * M_PI;
    widthOffset = resolution * (tmpAngle / (2.0 * M_PI));
    widthOffset -= w / 2;
  }

  for (int i = 0; i < actualNumberOfLayers(); ++i) {
    if ((maxWidth + widthOffset) <= resolution && (minWidth + widthOffset) >= 0)
      memcpy(data + i * resolution + minWidth + widthOffset, mTemporaryImage + width() * (int)(i * skip) + minWidth,
             sizeof(float) * (maxWidth - minWidth));
    else {  // we need two split into two because the current image is 'across' the lidar image (avoid overflow)
      if ((maxWidth + widthOffset) > resolution) {
        memcpy(data + i * resolution + minWidth + widthOffset, mTemporaryImage + width() * (int)(i * skip) + minWidth,
               sizeof(float) * (resolution - minWidth - widthOffset));
        memcpy(data + i * resolution, mTemporaryImage + width() * (int)(i * skip) + resolution - widthOffset,
               sizeof(float) * (maxWidth + widthOffset - resolution));
      } else {  // (minWidth + widthOffset) < 0
        memcpy(data + (i + 1) * resolution + minWidth + widthOffset, mTemporaryImage + width() * (int)(i * skip) + minWidth,
               sizeof(float) * abs(minWidth + widthOffset));
        memcpy(data + i * resolution, mTemporaryImage + width() * (int)(i * skip) - widthOffset,
               sizeof(float) * (maxWidth + widthOffset));
      }
    }
  }

  if (mIsPointCloudEnabled) {
    if (WbWorld::instance()->perspective()->isGlobalOptionalRenderingEnabled("LidarPointClouds"))
      displayPointCloud();
    if ((maxWidth + widthOffset) <= resolution && (minWidth + widthOffset) >= 0)
      updatePointCloud(minWidth + widthOffset, widthOffset + maxWidth);
    else {  // we need two split into two because the current image is 'across' the lidar image (avoid overflow)
      if ((maxWidth + widthOffset) > resolution) {
        updatePointCloud(minWidth + widthOffset, resolution);
        updatePointCloud(0, maxWidth + widthOffset - resolution);
      } else {  // (minWidth + widthOffset) < 0
        updatePointCloud(resolution + minWidth + widthOffset,
                         resolution + minWidth + widthOffset + abs(minWidth + widthOffset));
        updatePointCloud(0, maxWidth + widthOffset);
      }
    }
  }
}

void WbLidar::updatePointCloud(int minWidth, int maxWidth) {
  WbLidarPoint *lidarPoints = pointArray();
  const float *image = lidarImage();

  const int resolution = actualHorizontalResolution();
  const double w = width();
  const double time = WbSimulationState::instance()->time() / 1000.0;

  for (int i = 0; i < actualNumberOfLayers(); ++i) {
    double phi = 0;
    if (actualNumberOfLayers() > 1)  // to avoid division by zero
      phi = verticalFieldOfView() / 2 - i * (verticalFieldOfView() / (actualNumberOfLayers() - 1));
    const double sinPhi = sin(phi + mCurrentTiltAngle);
    const double cosPhi = cos(phi + mCurrentTiltAngle);
    for (int j = minWidth; j < maxWidth; ++j) {
      double theta = actualFieldOfView() / 2 - j * (actualFieldOfView() / (w - 1));
      if (isRotating())
        theta = -((double)j / (double)resolution) * 2 * M_PI;
      const int index = resolution * i + j;
      const double r = image[index];
      lidarPoints[index].x = -r * sin(theta) * cosPhi;
      lidarPoints[index].y = r * sinPhi;
      lidarPoints[index].z = -r * cos(theta) * cosPhi;
      lidarPoints[index].time = (j / w) * (time - mRefreshRate / 1000.0) + (1 - (j / w)) * time;
      lidarPoints[index].layer_id = i;
    }
  }
}

float *WbLidar::lidarImage() const {
  return reinterpret_cast<float *>(image());
}

void WbLidar::createWrenCamera() {
  mActualNumberOfLayers = mNumberOfLayers->value();
  mActualHorizontalResolution = mHorizontalResolution->value();
  mActualVerticalFieldOfView = mVerticalFieldOfView->value();
  mActualFieldOfView = mFieldOfView->value();
  mActualType = mType->value();

  WbAbstractCamera::createWrenCamera();
  applyMaxRangeToWren();
  applyResolutionToWren();
  applyTiltAngleToWren();
}

void WbLidar::deleteWren() {
  if (areWrenObjectsInitialized()) {
    wr_node_delete(WR_NODE(mFrustumRenderable));
    wr_material_delete(mFrustumMaterial);
    wr_static_mesh_delete(mFrustumMesh);

    mFrustumRenderable = NULL;
    mFrustumMaterial = NULL;
    mFrustumMesh = NULL;

    wr_node_delete(WR_NODE(mLidarRaysRenderable));
    wr_node_delete(WR_NODE(mLidarPointsRenderable));
    wr_material_delete(mLidarRaysMaterial);
    wr_material_delete(mLidarPointsMaterial);
    wr_dynamic_mesh_delete(mLidarPointsMesh);
    wr_dynamic_mesh_delete(mLidarRaysMesh);

    mLidarPointsRenderable = NULL;
    mLidarPointsMesh = NULL;
    mLidarPointsMaterial = NULL;

    mLidarRaysRenderable = NULL;
    mLidarRaysMesh = NULL;
    mLidarRaysMaterial = NULL;
  }
}

void WbLidar::displayPointCloud() {
  if (hasBeenSetup() && mImageShm) {
    const float layersNumber = actualNumberOfLayers();
    const int resolution = actualHorizontalResolution();
    const bool showRays = layersNumber * resolution < POINT_CLOUD_RAY_REPRESENTATION_THRESHOLD;

    wr_node_set_visible(WR_NODE(mLidarPointsRenderable), true);
    wr_node_set_visible(WR_NODE(mLidarRaysRenderable), showRays);

    wr_dynamic_mesh_clear(mLidarRaysMesh);
    wr_dynamic_mesh_clear(mLidarPointsMesh);

    const float origin[3] = {0.0f, 0.0f, 0.0f};
    float color[3] = {0.0f, 0.0f, 1.0f};
    unsigned int pointsIndex = 0;
    unsigned int raysIndex = 0;
    for (int k = 0; k < layersNumber; ++k) {
      if (layersNumber > 1) {  // to avoid division by zero
        color[0] = k / (layersNumber - 1.0f);
        color[2] = 1.0f - color[0];
      }
      for (int l = 0; l < resolution; ++l) {
        const float *vertex_x = &pointArray()[k * resolution + l].x;
        const float *vertex_y = &pointArray()[k * resolution + l].y;
        const float *vertex_z = &pointArray()[k * resolution + l].z;

        if (isinf(*vertex_x) || isinf(*vertex_y) || isinf(*vertex_z))
          continue;

        wr_dynamic_mesh_add_vertex(mLidarPointsMesh, vertex_x);
        wr_dynamic_mesh_add_color(mLidarPointsMesh, color);
        wr_dynamic_mesh_add_index(mLidarPointsMesh, pointsIndex++);
        // Ray
        if (showRays) {
          wr_dynamic_mesh_add_vertex(mLidarRaysMesh, origin);
          wr_dynamic_mesh_add_color(mLidarRaysMesh, color);
          wr_dynamic_mesh_add_index(mLidarRaysMesh, raysIndex++);

          wr_dynamic_mesh_add_vertex(mLidarRaysMesh, vertex_x);
          wr_dynamic_mesh_add_color(mLidarRaysMesh, color);
          wr_dynamic_mesh_add_index(mLidarRaysMesh, raysIndex++);
        }
      }
    }
  }
}

void WbLidar::hidePointCloud() {
  if (hasBeenSetup()) {
    wr_node_set_visible(WR_NODE(mLidarPointsRenderable), false);
    wr_node_set_visible(WR_NODE(mLidarRaysRenderable), false);
  }
}

static void pushVertex(float *vertices, unsigned int index, double x, double y, double z) {
  vertices[3 * index] = static_cast<float>(x);
  vertices[3 * index + 1] = static_cast<float>(y);
  vertices[3 * index + 2] = static_cast<float>(z);
}

// (Re)creates the cyan or gray (lidar disabled) ray if needed
void WbLidar::applyFrustumToWren() {
  wr_node_set_visible(WR_NODE(mFrustumRenderable), false);
  wr_static_mesh_delete(mFrustumMesh);
  mFrustumMesh = NULL;

  if (!WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_LIDAR_RAYS_PATHS))
    return;

  WbRgb frustumColorRgb;
  if (mSensor->isEnabled() && mSensor->isFirstValueReady())
    frustumColorRgb = enabledCameraFrustrumColor();
  else
    frustumColorRgb = disabledCameraFrustrumColor();

  const float frustumColor[3] = {static_cast<float>(frustumColorRgb.red()), static_cast<float>(frustumColorRgb.green()),
                                 static_cast<float>(frustumColorRgb.blue())};
  wr_phong_material_set_color(mFrustumMaterial, frustumColor);

  int i = 0;
  const double n = minRange();
  const double f = maxRange();
  const double fovV = verticalFieldOfView();
  double fovH = fieldOfView();
  if (isRotating())
    fovH = 2 * M_PI;

  const int intermediatePointsNumber = floor(fovH / 0.2);
  const int vertexCount = 4 * actualNumberOfLayers() * (intermediatePointsNumber + 3);
  float vertices[3 * vertexCount];

  for (int layer = 0; layer < actualNumberOfLayers(); ++layer) {
    double vAngle = 0;
    if (actualNumberOfLayers() > 1)
      vAngle = fovV / 2.0 - ((int)layer / ((int)actualNumberOfLayers() - 1.0)) * fovV + mTiltAngle->value();
    const double cosV = cos(vAngle);
    const double sinV = sin(vAngle);
    pushVertex(vertices, i++, 0, 0, 0);
    // min range
    for (int j = 0; j < intermediatePointsNumber + 2; ++j) {
      const double tmpHAngle = fovH / 2.0 - fovH * j / (intermediatePointsNumber + 1);
      const double x = n * sin(tmpHAngle) * cosV;
      const double y = n * sinV;
      const double z = n * -cos(tmpHAngle) * cosV;
      pushVertex(vertices, i++, x, y, z);
      pushVertex(vertices, i++, x, y, z);
    }

    pushVertex(vertices, i++, 0, 0, 0);
    pushVertex(vertices, i++, 0, 0, 0);

    // max range
    for (int j = 0; j < intermediatePointsNumber + 2; ++j) {
      const double tmpHAngle = fovH / 2.0 - fovH * j / (intermediatePointsNumber + 1);
      const double x = f * sin(tmpHAngle) * cosV;
      const double y = f * sinV;
      const double z = f * -cos(tmpHAngle) * cosV;
      pushVertex(vertices, i++, x, y, z);
      pushVertex(vertices, i++, x, y, z);
    }
    pushVertex(vertices, i++, 0, 0, 0);
  }

  mFrustumMesh = wr_static_mesh_line_set_new(vertexCount, vertices, NULL);
  wr_renderable_set_mesh(mFrustumRenderable, WR_MESH(mFrustumMesh));
  wr_node_set_visible(WR_NODE(mFrustumRenderable), true);
}

int WbLidar::height() const {
  if (actualNumberOfLayers() == 1)
    return 1;
  // as we want the center of the upper/lower pixel line to be aligned with the upper/lower layer we add 'actualFieldOfView() /
  // width()' to verticalFieldOfView
  return ceil((actualVerticalFieldOfView() + actualFieldOfView() / width()) * (width() / actualFieldOfView()));
}

int WbLidar::width() const {
  if (isRotating())
    return ceil(actualHorizontalResolution() * (actualFieldOfView() / (2.0 * M_PI)));
  return actualHorizontalResolution();
}

WbSolid *WbLidar::solidEndPoint() const {
  WbSolid *solid = dynamic_cast<WbSolid *>(mRotatingHead->value());
  if (solid)
    return solid;
  return NULL;
}

int WbLidar::actualNumberOfLayers() const {
  if (hasBeenSetup())
    return mActualNumberOfLayers;
  return mNumberOfLayers->value();
}

int WbLidar::actualHorizontalResolution() const {
  if (hasBeenSetup())
    return mActualHorizontalResolution;
  return mHorizontalResolution->value();
}

double WbLidar::actualVerticalFieldOfView() const {
  if (hasBeenSetup())
    return mActualVerticalFieldOfView;
  return mVerticalFieldOfView->value();
}

double WbLidar::actualFieldOfView() const {
  if (hasBeenSetup())
    return mActualFieldOfView;
  return mFieldOfView->value();
}

/////////////////////
//  Update methods //
/////////////////////

void WbLidar::updateNear() {
  if (WbFieldChecker::resetDoubleIfNonPositive(this, mNear, 0.01))
    return;

  if (mNear->value() > mMinRange->value()) {
    parsingWarn(tr("'near' is greater than to 'minRange'. Setting 'near' to %1.").arg(mMinRange->value()));
    mNear->setValue(mMinRange->value());
    return;
  }

  if (hasBeenSetup())
    applyNearToWren();
}

void WbLidar::updateMinRange() {
  if (WbFieldChecker::resetDoubleIfNonPositive(this, mMinRange, 0.01))
    return;

  if (mMinRange->value() < mNear->value()) {
    parsingWarn(tr("'minRange' is less than 'near'. Setting 'minRange' to %1.").arg(mNear->value()));
    mMinRange->setValue(mNear->value());
    return;
  }

  if (mMinRange->value() >= mMaxRange->value()) {
    parsingWarn(tr("'minRange' is greater or equal to 'maxRange'. Setting 'maxRange' to %1.").arg(mMinRange->value() + 1.0));
    mMaxRange->setValue(mMinRange->value() + 1.0);
    return;
  }

  mNeedToConfigure = true;

  if (areWrenObjectsInitialized())
    applyFrustumToWren();
}

void WbLidar::updateMaxRange() {
  if (mMaxRange->value() <= mMinRange->value()) {
    double newMaxRange = mMinRange->value() + 1.0;
    parsingWarn(tr("'maxRange' is less or equal to 'minRange'. Setting 'maxRange' to %1.").arg(newMaxRange));
    mMaxRange->setValue(newMaxRange);
    return;
  }

  mNeedToConfigure = true;

  if (hasBeenSetup())
    applyMaxRangeToWren();

  if (areWrenObjectsInitialized())
    applyFrustumToWren();
}

void WbLidar::updateFieldOfView() {
  WbAbstractCamera::updateFieldOfView();

  // warn in case of width modification after the setup
  if (hasBeenSetup())
    warn(tr(
      "'fieldOfView' has been modified. This modification will be taken into account after saving and reloading the world."));
}

void WbLidar::updateResolution() {
  if (WbFieldChecker::resetDoubleIfNonPositiveAndNotDisabled(this, mResolution, -1.0, -1.0))
    return;

  if (hasBeenSetup())
    applyResolutionToWren();
}

void WbLidar::updateTiltAngle() {
  if (hasBeenSetup())
    applyTiltAngleToWren();

  if (areWrenObjectsInitialized())
    applyFrustumToWren();
}

void WbLidar::updateType() {
  if (mType->value().compare("fixed", Qt::CaseInsensitive) != 0 &&
      mType->value().compare("rotating", Qt::CaseInsensitive) != 0) {
    parsingWarn(tr("'type' should either be 'fixed' or 'rotating', reset to 'fixed'"));
    mType->setValue("fixed");
  }
  if (hasBeenSetup())
    warn(tr("'type' has been modified. This modification will be taken into account after saving and reloading the world."));
  else if (areWrenObjectsInitialized())
    applyFrustumToWren();
}

void WbLidar::updateMinFrequency() {
  WbFieldChecker::resetDoubleIfNonPositive(this, mMinFrequency, 0.01);
  if (mMinFrequency->value() > mMaxFrequency->value()) {
    parsingWarn(tr("'minFrequency' should be smaller or equal to 'maxFrequency'."));
    mMinFrequency->setValue(mMaxFrequency->value());
  }
  if (hasBeenSetup())
    mNeedToConfigure = true;
}

void WbLidar::updateMaxFrequency() {
  WbFieldChecker::resetDoubleIfNonPositive(this, mMaxFrequency, mMinFrequency->value());
  if (mMaxFrequency->value() < mMinFrequency->value()) {
    parsingWarn(tr("'maxFrequency' should be bigger or equal to 'minFrequency'."));
    mMaxFrequency->setValue(mMinFrequency->value());
  }
  if (hasBeenSetup())
    mNeedToConfigure = true;
}

void WbLidar::updateDefaultFrequency() {
  WbFieldChecker::resetDoubleIfNonPositive(this, mDefaultFrequency, mMinFrequency->value());
  if (mDefaultFrequency->value() < mMinFrequency->value()) {
    parsingWarn(tr("'defaultFrequency' should be bigger or equal to 'minFrequency'."));
    mDefaultFrequency->setValue(mMinFrequency->value());
  } else if (mDefaultFrequency->value() > mMaxFrequency->value()) {
    parsingWarn(tr("'defaultFrequency' should be bigger or equal to 'maxFrequency'."));
    mDefaultFrequency->setValue(mMaxFrequency->value());
  }
  if (hasBeenSetup())
    mNeedToConfigure = true;
}

void WbLidar::updateHorizontalResolution() {
  WbFieldChecker::resetIntIfNonPositive(this, mHorizontalResolution, 1);

  // make sure we have at least 1 pixel height per layer
  if (height() < actualNumberOfLayers()) {
    int requiredResolution = ceil((actualNumberOfLayers() * actualFieldOfView()) / verticalFieldOfView());
    if (isRotating()) {
      requiredResolution *= 2.0 * M_PI / actualFieldOfView();
      parsingWarn(
        tr("Impossible to have a so small 'horizontalResolution' using this 'numberOfLayers' and 'verticalFieldOfView'. "
           "'horizontalResolution' should be bigger or equal to 2.0 * M_PI * numberOfLayers  / verticalFieldOfView. "
           "'horizontalResolution' set to %1.")
          .arg(requiredResolution));
    } else
      parsingWarn(
        tr("Impossible to have a so small 'horizontalResolution' using this 'fieldOfView', 'numberOfLayers' and "
           "'verticalFieldOfView'. 'horizontalResolution' should be bigger or equal to numberOfLayers * fieldOfView / "
           "verticalFieldOfView. 'horizontalResolution' set to %1.")
          .arg(requiredResolution));
    mHorizontalResolution->setValue(requiredResolution);
  }

  // warn in case of width modification after the setup
  if (hasBeenSetup())
    warn(tr("'horizontalResolution' has been modified. This modification will be taken into account after saving and reloading "
            "the world."));
}

void WbLidar::updateVerticalFieldOfView() {
  WbFieldChecker::resetDoubleIfNonPositive(this, mVerticalFieldOfView, 0.1);
  WbFieldChecker::resetDoubleIfGreater(this, mVerticalFieldOfView, M_PI, M_PI);

  // make sure we have at least 1 pixel height per layer
  if (height() < actualNumberOfLayers()) {
    double requiredVerticalFieldOfView = (actualNumberOfLayers() * actualFieldOfView()) / width();
    if (isRotating())
      parsingWarn(
        tr("Impossible to have a so small 'verticalFieldOfView' using this 'numberOfLayers' and 'horizontalResolution'. "
           "'verticalFieldOfView' should be bigger or equal to 2.0 * M_PI * numberOfLayers / horizontalResolution. "
           "'verticalFieldOfView' set to %1.")
          .arg(requiredVerticalFieldOfView));
    else
      parsingWarn(
        tr("Impossible to have a so small 'verticalFieldOfView' using this 'fieldOfView', 'numberOfLayers' and "
           "'horizontalResolution'. 'verticalFieldOfView' should be bigger or equal to numberOfLayers * fieldOfView / "
           "horizontalResolution. 'verticalFieldOfView' set to %1.")
          .arg(requiredVerticalFieldOfView));
    mVerticalFieldOfView->setValue(requiredVerticalFieldOfView);
  }

  // warn in case of width modification after the setup
  if (hasBeenSetup())
    warn(tr("'verticalFieldOfView' has been modified. This modification will be taken into account after saving and reloading "
            "the world."));
  if (areWrenObjectsInitialized())
    applyFrustumToWren();
}

void WbLidar::updateNumberOfLayers() {
  WbFieldChecker::resetIntIfNonPositive(this, mNumberOfLayers, 1);

  // make sure we have at least 1 pixel height per layer
  if (height() < actualNumberOfLayers()) {
    int requiredNumberOfLayers = height();
    if (isRotating())
      parsingWarn(
        tr("Impossible to have a so big 'numberOfLayers' using this 'verticalFieldOfView' and 'horizontalResolution'. "
           "'numberOfLayers' should be smaller or equal to verticalFieldOfView * actualHorizontalResolution() / (2.0 * "
           "M_PI). 'numberOfLayers' set to %1.")
          .arg(requiredNumberOfLayers));
    else
      parsingWarn(tr("Impossible to have a so big 'numberOfLayers' using this 'fieldOfView', 'verticalFieldOfView' and "
                     "'horizontalResolution'. 'numberOfLayers' should be smaller or equal to verticalFieldOfView * "
                     "horizontalResolution / fieldOfView. 'numberOfLayers' set to %1.")
                    .arg(requiredNumberOfLayers));
    mNumberOfLayers->setValue(requiredNumberOfLayers);
  }

  // warn in case of width modification after the setup
  if (hasBeenSetup())
    warn(tr("'numberOfLayers' has been modified. This modification will be taken into account after saving and reloading the "
            "world."));
  if (areWrenObjectsInitialized())
    applyFrustumToWren();
}

void WbLidar::updateRotatingHead() {
  const WbSolid *head = solidEndPoint();
  if (head && isPostFinalizedCalled()) {
    if (head->isPostFinalizedCalled())
      mBoundingSphere->addSubBoundingSphere(head->boundingSphere());
    else
      connect(head, &WbBaseNode::finalizationCompleted, this, &WbLidar::updateBoundingSphere);
  }
}

void WbLidar::updateBoundingSphere(WbBaseNode *subNode) {
  disconnect(subNode, &WbBaseNode::finalizationCompleted, this, &WbLidar::updateBoundingSphere);
  mBoundingSphere->addSubBoundingSphere(subNode->boundingSphere());
}

/////////////////////
//  Apply methods  //
/////////////////////

void WbLidar::applyResolutionToWren() {
  mWrenCamera->setRangeResolution(mResolution->value());
}

void WbLidar::applyMaxRangeToWren() {
  mWrenCamera->setMaxRange(mMaxRange->value());
}

void WbLidar::applyCameraSettingsToWren() {
  WbAbstractCamera::applyCameraSettingsToWren();
  applyResolutionToWren();
  applyMaxRangeToWren();
}

int WbLidar::textureGLId() const {
  if (mWrenCamera)
    return mWrenCamera->textureGLId();
  return WbRenderingDevice::textureGLId();
}

void WbLidar::applyTiltAngleToWren() {
  mWrenCamera->rotatePitch(mTiltAngle->value() - mCurrentTiltAngle);
  mCurrentTiltAngle = mTiltAngle->value();
}

void WbLidar::createOdeObjects() {
  WbAbstractCamera::createOdeObjects();
  WbSolid *s = solidEndPoint();
  if (s)
    s->createOdeObjects();
}

void WbLidar::propagateSelection(bool selected) {
  WbAbstractCamera::propagateSelection(selected);
  WbSolid *solid = solidEndPoint();
  if (solid)
    solid->propagateSelection(selected);
}

void WbLidar::setMatrixNeedUpdate() {
  WbAbstractCamera::setMatrixNeedUpdate();
  WbSolid *s = solidEndPoint();
  if (s)
    s->setMatrixNeedUpdate();
}

//////////
// WREN //
//////////

void WbLidar::updateCollisionMaterial(bool triggerChange, bool onSelection) {
  WbAbstractCamera::updateCollisionMaterial(triggerChange, onSelection);
  WbSolid *s = solidEndPoint();
  if (s)
    s->updateCollisionMaterial(triggerChange, onSelection);
}

void WbLidar::setSleepMaterial() {
  WbAbstractCamera::setSleepMaterial();
  WbSolid *s = solidEndPoint();
  if (s)
    s->setSleepMaterial();
}

void WbLidar::setScaleNeedUpdate() {
  WbAbstractCamera::setScaleNeedUpdate();
  WbSolid *s = solidEndPoint();
  if (s)
    s->setScaleNeedUpdate();
}

void WbLidar::attachResizeManipulator() {
  WbAbstractCamera::attachResizeManipulator();
  WbSolid *s = solidEndPoint();
  if (s)
    s->attachResizeManipulator();
}

void WbLidar::detachResizeManipulator() const {
  WbAbstractCamera::detachResizeManipulator();
  WbSolid *s = solidEndPoint();
  if (s)
    s->detachResizeManipulator();
}

void WbLidar::createWrenObjects() {
  // Required to draw lidar points
  wr_config_enable_point_size(true);

  // Lidar frustum
  mFrustumMaterial = wr_phong_material_new();
  wr_material_set_default_program(mFrustumMaterial, WbWrenShaders::lineSetShader());

  mFrustumRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mFrustumRenderable, false);
  wr_renderable_set_receive_shadows(mFrustumRenderable, false);
  wr_renderable_set_visibility_flags(mFrustumRenderable, WbWrenRenderingContext::VF_LIDAR_RAYS_PATHS);
  wr_renderable_set_material(mFrustumRenderable, mFrustumMaterial, NULL);
  wr_renderable_set_drawing_mode(mFrustumRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_node_set_visible(WR_NODE(mFrustumRenderable), false);

  // Lidar point cloud
  mLidarPointsMaterial = wr_phong_material_new();
  wr_phong_material_set_color_per_vertex(mLidarPointsMaterial, true);
  wr_material_set_default_program(mLidarPointsMaterial, WbWrenShaders::pointSetShader());
  wr_phong_material_set_transparency(mLidarPointsMaterial, 0.3f);

  mLidarRaysMaterial = wr_phong_material_new();
  wr_phong_material_set_color_per_vertex(mLidarRaysMaterial, true);
  wr_material_set_default_program(mLidarRaysMaterial, WbWrenShaders::lineSetShader());
  wr_phong_material_set_transparency(mLidarRaysMaterial, 0.7f);

  mLidarPointsMesh = wr_dynamic_mesh_new(false, false, true);
  mLidarRaysMesh = wr_dynamic_mesh_new(false, false, true);

  mLidarPointsRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mLidarPointsRenderable, false);
  wr_renderable_set_receive_shadows(mLidarPointsRenderable, false);
  wr_renderable_set_visibility_flags(mLidarPointsRenderable, WbWrenRenderingContext::VF_LIDAR_POINT_CLOUD);
  wr_renderable_set_material(mLidarPointsRenderable, mLidarPointsMaterial, NULL);
  wr_renderable_set_drawing_mode(mLidarPointsRenderable, WR_RENDERABLE_DRAWING_MODE_POINTS);
  wr_renderable_set_mesh(mLidarPointsRenderable, WR_MESH(mLidarPointsMesh));
  wr_renderable_set_drawing_order(mLidarPointsRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);
  wr_renderable_set_point_size(mLidarPointsRenderable, 3.0f);

  mLidarRaysRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mLidarRaysRenderable, false);
  wr_renderable_set_receive_shadows(mLidarRaysRenderable, false);
  wr_renderable_set_visibility_flags(mLidarRaysRenderable, WbWrenRenderingContext::VF_LIDAR_POINT_CLOUD);
  wr_renderable_set_material(mLidarRaysRenderable, mLidarRaysMaterial, NULL);
  wr_renderable_set_drawing_mode(mLidarRaysRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_mesh(mLidarRaysRenderable, WR_MESH(mLidarRaysMesh));
  wr_renderable_set_drawing_order(mLidarRaysRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_0);

  WbAbstractCamera::createWrenObjects();

  wr_transform_attach_child(wrenNode(), WR_NODE(mFrustumRenderable));
  wr_transform_attach_child(wrenNode(), WR_NODE(mLidarRaysRenderable));
  wr_transform_attach_child(wrenNode(), WR_NODE(mLidarPointsRenderable));

  WbSolid *const s = solidEndPoint();
  if (s)
    s->createWrenObjects();
}
