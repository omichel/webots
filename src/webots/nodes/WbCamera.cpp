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

#include "WbCamera.hpp"

#include "WbAffinePlane.hpp"
#include "WbBoundingSphere.hpp"
#include "WbDownloader.hpp"
#include "WbFieldChecker.hpp"
#include "WbFocus.hpp"
#include "WbLensFlare.hpp"
#include "WbLight.hpp"
#include "WbObjectDetection.hpp"
#include "WbPerformanceLog.hpp"
#include "WbPreferences.hpp"
#include "WbProtoModel.hpp"
#include "WbRecognition.hpp"
#include "WbRgb.hpp"
#include "WbRobot.hpp"
#include "WbSFNode.hpp"
#include "WbSensor.hpp"
#include "WbStandardPaths.hpp"
#include "WbUrl.hpp"
#include "WbWorld.hpp"
#include "WbWrenCamera.hpp"
#include "WbWrenLabelOverlay.hpp"
#include "WbWrenOpenGlContext.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenTextureOverlay.hpp"
#include "WbZoom.hpp"

#include "../../controller/c/messages.h"

#include <QtCore/QDataStream>
#include <QtCore/QtGlobal>

#ifndef _WIN32
#include "WbPosixSharedMemory.hpp"
#else
#include <QtCore/QSharedMemory>
#endif

class WbRecognizedObject : public WbObjectDetection {
public:
  WbRecognizedObject(WbCamera *camera, WbSolid *object, bool needToCheckCollision, double maxRange) :
    WbObjectDetection(camera, object, needToCheckCollision, maxRange) {
    mId = object->uniqueId();
    mModel = "";
    mRelativeOrientation = WbRotation(0.0, 1.0, 0.0, 0.0);
    mPositionOnImage = WbVector2(0, 0);
    mPixelSize = WbVector2(0, 0);
    mColors.clear();
  };

  virtual ~WbRecognizedObject() {}

  int id() const { return mId; }
  const QString model() const { return mModel; }
  const WbRotation relativeOrientation() const { return mRelativeOrientation; }
  const WbVector2 positionOnImage() const { return mPositionOnImage; }
  const WbVector2 pixelSize() const { return mPixelSize; }
  const QList<WbRgb> colors() const { return mColors; }

  void setModel(const QString &model) { mModel = model; }
  void setRelativeOrientation(const WbRotation &relativeOrientation) { mRelativeOrientation = relativeOrientation; }
  void setPositionOnImage(const WbVector2 &positionOnImage) { mPositionOnImage = positionOnImage; }
  void setPixelSize(const WbVector2 &pixelSize) { mPixelSize = pixelSize; }
  void addColor(WbRgb colors) { mColors.append(colors); }

protected:
  double distance() override { return fabs(mObjectRelativePosition.z()); }

  int mId;
  QString mModel;
  WbRotation mRelativeOrientation;
  WbVector2 mPositionOnImage;
  WbVector2 mPixelSize;
  QList<WbRgb> mColors;
};

void WbCamera::init() {
  mCharType = 'c';
  // Fields initialization
  mFocus = findSFNode("focus");
  mZoom = findSFNode("zoom");
  mRecognition = findSFNode("recognition");
  mNoiseMaskUrl = findSFString("noiseMaskUrl");
  mAntiAliasing = findSFBool("antiAliasing");
  mAmbientOcclusionRadius = findSFDouble("ambientOcclusionRadius");
  mBloomThreshold = findSFDouble("bloomThreshold");
  mLensFlare = findSFNode("lensFlare");
  mFar = findSFDouble("far");
  mExposure = findSFDouble("exposure");

  // backward compatibility
  const WbSFString *type = findSFString("type");
  if (type->value().startsWith('r', Qt::CaseInsensitive))
    parsingWarn("Range finder type is not available for camera since Webots 8.4, please use a RangeFinder node instead.");

  WbSFDouble *colorNoise = findSFDouble("colorNoise");
  if (colorNoise->value() != 0.0) {  // Introduced in Webots 8.4.0
    parsingWarn("Deprecated 'colorNoise' field, please use the 'noise' field instead.");
    if (mNoise->value() == 0.0)
      mNoise->setValue(colorNoise->value());
    colorNoise->setValue(0.0);
  }

  mLabelOverlay = NULL;
  mNeedToDeleteRecognizedObjectsRays = false;
  mRecognitionSensor = NULL;
  mRecognitionRefreshRate = 0;
  mRecognizedObjects.clear();
  mRecognizedObjectsTexture = NULL;
  mSegmentationChanged = false;
  mSegmentationCamera = NULL;
  mSegmentationEnabled = false;
  mSegmentationShm = NULL;
  mSegmentationImageChanged = false;
  mHasSegmentationSharedMemoryChanged = false;
  mInvalidRecognizedObjects = QList<WbRecognizedObject *>();
  mDownloader = NULL;
}

WbCamera::WbCamera(WbTokenizer *tokenizer) : WbAbstractCamera("Camera", tokenizer) {
  init();
}

WbCamera::WbCamera(const WbCamera &other) : WbAbstractCamera(other) {
  init();
}

WbCamera::WbCamera(const WbNode &other) : WbAbstractCamera(other) {
  init();
}

WbCamera::~WbCamera() {
  delete mRecognitionSensor;
  qDeleteAll(mRecognizedObjects);
  mRecognizedObjects.clear();

  delete mSegmentationCamera;
  delete mSegmentationShm;
}

void WbCamera::downloadAssets() {
  const QString &noiseMaskUrl = mNoiseMaskUrl->value();
  if (WbUrl::isWeb(noiseMaskUrl)) {
    delete mDownloader;
    mDownloader = new WbDownloader(this);
    if (isPostFinalizedCalled())  // URL changed from the scene tree or supervisor
      connect(mDownloader, &WbDownloader::complete, this, &WbCamera::updateNoiseMaskUrl);
    mDownloader->download(QUrl(noiseMaskUrl));
  }
}

void WbCamera::preFinalize() {
  WbAbstractCamera::preFinalize();

  if (zoom())
    zoom()->preFinalize();

  if (recognition())
    recognition()->preFinalize();

  if (focus())
    focus()->preFinalize();

  if (lensFlare())
    lensFlare()->preFinalize();

  mRecognitionSensor = new WbSensor();

  updateNear();
  updateFar();
  updateExposure();
  updateBloomThreshold();
  updateAmbientOcclusionRadius();
}

void WbCamera::postFinalize() {
  WbAbstractCamera::postFinalize();

  if (zoom())
    zoom()->postFinalize();

  if (recognition()) {
    recognition()->postFinalize();
    updateRecognition();
  }

  if (focus()) {
    focus()->postFinalize();
    updateFocus();
  }

  connect(mFocus, &WbSFNode::changed, this, &WbCamera::updateFocus);
  connect(mRecognition, &WbSFNode::changed, this, &WbCamera::updateRecognition);
  connect(mNear, &WbSFDouble::changed, this, &WbCamera::updateNear);
  connect(mFar, &WbSFDouble::changed, this, &WbCamera::updateFar);
  connect(mExposure, &WbSFDouble::changed, this, &WbCamera::updateExposure);
  connect(mAmbientOcclusionRadius, &WbSFDouble::changed, this, &WbCamera::updateAmbientOcclusionRadius);
  connect(mBloomThreshold, &WbSFDouble::changed, this, &WbCamera::updateBloomThreshold);
  connect(mAntiAliasing, &WbSFBool::changed, this, &WbAbstractCamera::updateAntiAliasing);
  connect(WbPreferences::instance(), &WbPreferences::changedByUser, this, &WbCamera::setup);

  if (lensFlare())
    lensFlare()->postFinalize();
}

WbFocus *WbCamera::focus() const {
  return dynamic_cast<WbFocus *>(mFocus->value());
}

WbZoom *WbCamera::zoom() const {
  return dynamic_cast<WbZoom *>(mZoom->value());
}

WbRecognition *WbCamera::recognition() const {
  return dynamic_cast<WbRecognition *>(mRecognition->value());
}

WbLensFlare *WbCamera::lensFlare() const {
  return dynamic_cast<WbLensFlare *>(mLensFlare->value());
}

void WbCamera::initializeImageSharedMemory() {
  WbAbstractCamera::initializeImageSharedMemory();
  if (mImageShm) {
    // initialize the shared memory with a black image
    int *im = reinterpret_cast<int *>(image());
    const int size = width() * height();
    for (int i = 0; i < size; i++)
      im[i] = 0xFF000000;
  }
}

void WbCamera::initializeSegmentationSharedMemory() {
  cCameraNumber++;
  delete mSegmentationShm;
  mSegmentationShm = initializeSharedMemory();
  mHasSegmentationSharedMemoryChanged = true;
  if (mSegmentationShm) {
    unsigned char *data = (unsigned char *)mSegmentationShm->data();
    // initialize the shared memory with a black image
    int *im = reinterpret_cast<int *>(data);
    const int size = width() * height();
    for (int i = 0; i < size; i++)
      im[i] = 0xFF000000;
  }
}

QString WbCamera::pixelInfo(int x, int y) const {
  WbRgb color;
  if (hasBeenSetup())
    color = mWrenCamera->copyPixelColourValue(x, y);

  const int red = color.red() * 255;
  const int green = color.green() * 255;
  const int blue = color.blue() * 255;

  return QString::asprintf("pixel(%d,%d)=#%02X%02X%02X", x, y, red, green, blue);
}

void WbCamera::updateRecognizedObjectsOverlay(double screenX, double screenY, double overlayX, double overlayY) {
  if (!recognition() || !mSensor->isEnabled()) {
    clearRecognizedObjectsOverlay();
    return;
  }

  // check if mouse is over an object
  int objectIndex = -1;
  for (int i = 0; i < mRecognizedObjects.size(); ++i) {
    const WbVector2 positionOnImage = mRecognizedObjects.at(i)->positionOnImage();
    const WbVector2 pixelsSize = mRecognizedObjects.at(i)->pixelSize();
    if (overlayX < positionOnImage.x() - pixelsSize.x() / 2)
      continue;
    if (overlayX > positionOnImage.x() + pixelsSize.x() / 2)
      continue;
    if (overlayY < positionOnImage.y() - pixelsSize.y() / 2)
      continue;
    if (overlayY > positionOnImage.y() + pixelsSize.y() / 2)
      continue;
    objectIndex = i;
    break;
  }
  // display information of the object (if any) in an overlay
  if (objectIndex >= 0) {
    const WbRecognizedObject *object = mRecognizedObjects.at(objectIndex);
    QString text(object->model());
    text += tr("\nId: %1").arg(object->id());
    text += tr("\nRelative position: %1 %2 %3")
              .arg(WbPrecision::doubleToString(object->objectRelativePosition().x(), WbPrecision::GUI_LOW))
              .arg(WbPrecision::doubleToString(object->objectRelativePosition().y(), WbPrecision::GUI_LOW))
              .arg(WbPrecision::doubleToString(object->objectRelativePosition().z(), WbPrecision::GUI_LOW));
    text += tr("\nRelative orientation: %1 %2 %3 %4")
              .arg(WbPrecision::doubleToString(object->relativeOrientation().x(), WbPrecision::GUI_LOW))
              .arg(WbPrecision::doubleToString(object->relativeOrientation().y(), WbPrecision::GUI_LOW))
              .arg(WbPrecision::doubleToString(object->relativeOrientation().z(), WbPrecision::GUI_LOW))
              .arg(WbPrecision::doubleToString(object->relativeOrientation().angle(), WbPrecision::GUI_LOW));
    text += tr("\nSize: %1 %2")
              .arg(WbPrecision::doubleToString(object->objectSize().x(), WbPrecision::GUI_LOW))
              .arg(WbPrecision::doubleToString(object->objectSize().y(), WbPrecision::GUI_LOW));
    text += tr("\nPosition on the image: %1 %2").arg(object->positionOnImage().x()).arg(object->positionOnImage().y());
    text += tr("\nSize on the image: %1 %2").arg(object->pixelSize().x()).arg(object->pixelSize().y());
    for (int i = 0; i < object->colors().size(); ++i)
      text += tr("\nColor %1: %2 %3 %4")
                .arg(i)
                .arg(WbPrecision::doubleToString(object->colors().at(i).red(), WbPrecision::GUI_LOW))
                .arg(WbPrecision::doubleToString(object->colors().at(i).green(), WbPrecision::GUI_LOW))
                .arg(WbPrecision::doubleToString(object->colors().at(i).blue(), WbPrecision::GUI_LOW));
    if (mLabelOverlay == NULL) {
      mLabelOverlay = WbWrenLabelOverlay::createOrRetrieve(WbWrenLabelOverlay::cameraCaptionOverlayId(),
                                                           WbStandardPaths::fontsPath() + "Arial.ttf");
      mLabelOverlay->setText(text);
      mLabelOverlay->setPosition(screenX, screenY);
      mLabelOverlay->setSize(0.06);
      mLabelOverlay->setColor(0xFF0000);
      mLabelOverlay->applyChangesToWren();
    } else {
      mLabelOverlay->moveToPosition(screenX, screenY);
      mLabelOverlay->updateText(text);
    }
  } else
    clearRecognizedObjectsOverlay();
}

void WbCamera::clearRecognizedObjectsOverlay() {
  if (mLabelOverlay) {
    WbWrenLabelOverlay::removeLabel(WbWrenLabelOverlay::cameraCaptionOverlayId());
    mLabelOverlay = NULL;
  }
}

WrTexture *WbCamera::getWrenTexture() {
  if (!hasBeenSetup())
    setup();
  if (mWrenCamera)
    return mWrenCamera->getWrenTexture();
  return NULL;
}

void WbCamera::displayRecognizedObjectsInOverlay() {
  assert(recognition());

  WbPerformanceLog *log = WbPerformanceLog::instance();
  if (log)
    log->startMeasure(WbPerformanceLog::GPU_MEMORY_TRANSFER);

  const int w = width();
  const int h = height();
  const int frameThickness = recognition()->frameThickness();

  if (frameThickness > 0) {
    const int color = 0xFF000000 + (recognition()->frameColor().redByte() << 16) +
                      (recognition()->frameColor().greenByte() << 8) + recognition()->frameColor().blueByte();

    int *data = new int[w * h];
    int *clearData = new int[w * h];
    const int size = w * h;
    for (int i = 0; i < size; ++i) {
      data[i] = color;
      clearData[i] = 0;
    }

    WbWrenOpenGlContext::makeWrenCurrent();

    // Clear texture
    wr_texture_change_data(mRecognizedObjectsTexture, clearData, 0, 0, w, h);

    for (int i = 0; i < mRecognizedObjects.size(); ++i) {
      const WbRecognizedObject *object = mRecognizedObjects.at(i);
      const int x1 = qMax(0, (int)(object->positionOnImage().x() - object->pixelSize().x() / 2 - 1));
      const int x2 = qMin(w - 1, (int)(object->positionOnImage().x() + object->pixelSize().x() / 2 + 1));
      const int y1 = qMax(0, (int)(object->positionOnImage().y() - object->pixelSize().y() / 2 - 1));
      const int y2 = qMin(h - 1, (int)(object->positionOnImage().y() + object->pixelSize().y() / 2 + 1));

      wr_texture_change_data(mRecognizedObjectsTexture, data, x1, y1, frameThickness, y2 - y1);
      wr_texture_change_data(mRecognizedObjectsTexture, data, x2 - frameThickness, y1, frameThickness, y2 - y1);
      wr_texture_change_data(mRecognizedObjectsTexture, data, x1, y1, x2 - x1, frameThickness);
      wr_texture_change_data(mRecognizedObjectsTexture, data, x1, y2 - frameThickness, x2 - x1, frameThickness);
    }
    WbWrenOpenGlContext::doneWren();

    delete[] data;
    delete[] clearData;
  }

  if (log)
    log->stopMeasure(WbPerformanceLog::GPU_MEMORY_TRANSFER);
}

void WbCamera::prePhysicsStep(double ms) {
  WbAbstractCamera::prePhysicsStep(ms);

  if (isPowerOn() && mRecognitionSensor->isEnabled() && mRecognitionSensor->needToRefreshInMs(ms)) {
    // delete them only when sensor will be updated (we need to keep them for the overlay)
    qDeleteAll(mRecognizedObjects);
    mRecognizedObjects.clear();
  } else if (mNeedToDeleteRecognizedObjectsRays) {
    // we can destroy the ray because we don't need it anymore
    foreach (WbRecognizedObject *recognizedObject, mRecognizedObjects)
      recognizedObject->deleteRay();
    mNeedToDeleteRecognizedObjectsRays = false;
  }

  if (isPowerOn() && mRecognitionSensor->isEnabled() && mRecognitionSensor->needToRefreshInMs(ms) && recognition() &&
      recognition()->occlusion()) {
    // create rays
    computeObjects(false, true);
    mNeedToDeleteRecognizedObjectsRays = true;

    if (!mRecognizedObjects.isEmpty())
      // camera or object could move during physics step
      subscribeToRaysUpdate(mRecognizedObjects[0]->geom());
  }
}

void WbCamera::postPhysicsStep() {
  WbAbstractCamera::postPhysicsStep();

  // delete invalid WbRecognizedObject and ODE rays
  // it is preferable to not delete them during the physics step to avoid
  // possible inconsistencies in other clusters
  qDeleteAll(mInvalidRecognizedObjects);
  mInvalidRecognizedObjects.clear();
}

void WbCamera::reset(const QString &id) {
  WbAbstractCamera::reset(id);

  WbNode *const focus = mFocus->value();
  if (focus)
    focus->reset(id);
  WbNode *const zoom = mZoom->value();
  if (zoom)
    zoom->reset(id);
  WbNode *const recognition = mRecognition->value();
  if (recognition)
    recognition->reset(id);
  WbNode *const lensFlare = mLensFlare->value();
  if (lensFlare)
    lensFlare->reset(id);
}

void WbCamera::updateRaysSetupIfNeeded() {
  updateTransformForPhysicsStep();

  // compute the camera position and rotation
  const WbVector3 cameraPosition = matrix().translation();
  const WbMatrix3 cameraRotation = rotationMatrix();
  const WbMatrix3 cameraInverseRotation = cameraRotation.transposed();
  const double horizontalFieldOfView = fieldOfView();
  const double verticalFieldOfView =
    WbWrenCamera::computeFieldOfViewY(horizontalFieldOfView, (double)width() / (double)height());
  const WbAffinePlane *frustumPlanes = WbObjectDetection::computeFrustumPlanes(
    cameraPosition, cameraRotation, verticalFieldOfView, horizontalFieldOfView, recognition()->maxRange());
  foreach (WbRecognizedObject *recognizedObject, mRecognizedObjects) {
    recognizedObject->object()->updateTransformForPhysicsStep();
    bool valid =
      recognizedObject->recomputeRayDirection(this, cameraPosition, cameraRotation, cameraInverseRotation, frustumPlanes);
    if (valid)
      valid = computeObject(cameraPosition, cameraRotation, cameraInverseRotation, frustumPlanes, recognizedObject, true);
    if (!valid) {
      mRecognizedObjects.removeAll(recognizedObject);
      mInvalidRecognizedObjects.append(recognizedObject);
    }
  }

  delete[] frustumPlanes;
}

void WbCamera::rayCollisionCallback(dGeomID geom, WbSolid *collidingSolid, double depth) {
  foreach (WbRecognizedObject *recognizedObject, mRecognizedObjects) {
    // check if this object is the one that collides
    if (recognizedObject->geom() == geom) {
      // make sure the colliding solid is not the target itself (or a sub-part)
      if (recognizedObject->object() != collidingSolid && !recognizedObject->object()->solidChildren().contains(collidingSolid))
        recognizedObject->setCollided(depth);
      return;
    }
  }
}

void WbCamera::addConfigureToStream(QDataStream &stream, bool reconfigure) {
  WbAbstractCamera::addConfigureToStream(stream, reconfigure);
  if (zoom()) {
    stream << (double)zoom()->minFieldOfView();
    stream << (double)zoom()->maxFieldOfView();
  } else {
    stream << (double)mFieldOfView->value();
    stream << (double)mFieldOfView->value();
  }
  stream << (unsigned char)(recognition() ? 1 : 0);
  stream << (unsigned char)(recognition() && recognition()->segmentation() ? 1 : 0);
  stream << (double)mExposure->value();
  if (focus()) {
    stream << (double)focus()->focalLength();
    stream << (double)focus()->focalDistance();
    stream << (double)focus()->minFocalDistance();
    stream << (double)focus()->maxFocalDistance();
  } else {
    stream << (double)0.0;
    stream << (double)0.0;
    stream << (double)0.0;
    stream << (double)0.0;
  }
}

void WbCamera::resetSharedMemory() {
  WbAbstractCamera::resetSharedMemory();
  if (hasBeenSetup() && (mSegmentationShm || (recognition() && recognition()->segmentation())))
    // the previous shared memory will be released by the new controller start
    initializeSegmentationSharedMemory();
}

void WbCamera::writeConfigure(QDataStream &stream) {
  WbAbstractCamera::writeConfigure(stream);

  mRecognitionSensor->connectToRobotSignal(robot());
}

void WbCamera::writeAnswer(QDataStream &stream) {
  WbAbstractCamera::writeAnswer(stream);

  if (mSegmentationImageChanged) {
    copyImageToSharedMemory(mSegmentationCamera, (unsigned char *)mSegmentationShm->data());
    mSegmentationImageChanged = false;
  }

  if (recognition()) {
    if (refreshRecognitionSensorIfNeeded() || mRecognitionSensor->hasPendingValue()) {
      stream << tag();
      stream << (unsigned char)C_CAMERA_OBJECTS;
      const int numberOfObjects = mRecognizedObjects.size();
      stream << (int)numberOfObjects;
      for (int i = 0; i < numberOfObjects; ++i) {
        const WbRecognizedObject *recognizedObject = mRecognizedObjects.at(i);
        // id
        stream << (int)recognizedObject->id();
        // relative position
        stream << (double)recognizedObject->objectRelativePosition().x();
        stream << (double)recognizedObject->objectRelativePosition().y();
        stream << (double)recognizedObject->objectRelativePosition().z();
        // relative orientation
        stream << (double)recognizedObject->relativeOrientation().x();
        stream << (double)recognizedObject->relativeOrientation().y();
        stream << (double)recognizedObject->relativeOrientation().z();
        stream << (double)recognizedObject->relativeOrientation().angle();
        // size
        stream << (double)recognizedObject->objectSize().x();
        stream << (double)recognizedObject->objectSize().y();
        // position on the image
        stream << (int)recognizedObject->positionOnImage().x();
        stream << (int)recognizedObject->positionOnImage().y();
        // size on the image
        stream << (int)recognizedObject->pixelSize().x();
        stream << (int)recognizedObject->pixelSize().y();
        // colors
        const int numberOfColors = recognizedObject->colors().size();
        stream << (int)numberOfColors;
        for (int j = 0; j < numberOfColors; ++j) {
          stream << (double)recognizedObject->colors().at(j).red();
          stream << (double)recognizedObject->colors().at(j).green();
          stream << (double)recognizedObject->colors().at(j).blue();
        }
        // model
        const QByteArray model = recognizedObject->model().toUtf8();
        stream.writeRawData(model.constData(), model.size() + 1);
      }

      mRecognitionSensor->resetPendingValue();
      if (mSensor->isEnabled())
        displayRecognizedObjectsInOverlay();
    }

    if (mSegmentationChanged) {
      stream << (short unsigned int)tag();
      stream << (unsigned char)C_CAMERA_SET_SEGMENTATION;
      stream << (unsigned char)recognition()->segmentation();
      mSegmentationChanged = false;
    }

    if (mSegmentationCamera) {
      if (mHasSegmentationSharedMemoryChanged) {
        stream << (short unsigned int)tag();
        stream << (unsigned char)C_CAMERA_SEGMENTATION_SHARED_MEMORY;
        if (mSegmentationShm) {
          stream << (int)(mSegmentationShm->size());
          const QByteArray n = QFile::encodeName(mSegmentationShm->nativeKey());
          stream.writeRawData(n.constData(), n.size() + 1);
        } else
          stream << (int)(0);
        mHasSegmentationSharedMemoryChanged = false;
      }
    }
  }
}

void WbCamera::handleMessage(QDataStream &stream) {
  unsigned char command;
  stream >> command;

  if (WbAbstractCamera::handleCommand(stream, command))
    return;

  switch (command) {
    case C_CAMERA_SET_FOV: {
      double fov;
      stream >> fov;

      const WbZoom *z = zoom();
      if (z) {
        if (fov >= z->minFieldOfView() && fov <= z->maxFieldOfView())
          mFieldOfView->setValue(fov);
        else
          warn(
            tr("wb_camera_set_fov(%1) out of zoom range [%2, %3].").arg(fov).arg(z->minFieldOfView()).arg(z->maxFieldOfView()));
      } else
        warn(tr("wb_camera_set_fov() cannot be applied to this camera: missing 'zoom'."));
      break;
    }
    case C_CAMERA_SET_EXPOSURE: {
      double exposure;
      stream >> exposure;
      mExposure->setValue(exposure);
      break;
    }
    case C_CAMERA_SET_FOCAL: {
      double focalDistance;
      stream >> focalDistance;

      WbFocus *f = focus();
      if (f) {
        if ((focalDistance >= f->minFocalDistance()) && (focalDistance <= f->maxFocalDistance()))
          f->setFocalDistance(focalDistance);
        else
          warn(tr("wb_camera_set_focal_distance(%1) out of focus range [%2, %3].")
                 .arg(focalDistance)
                 .arg(f->minFocalDistance())
                 .arg(f->maxFocalDistance()));
      } else
        warn(tr("wb_camera_set_focal_distance() cannot be applied to this camera: missing 'focus'."));
      break;
    }
    case C_CAMERA_SET_RECOGNITION_SAMPLING_PERIOD: {
      stream >> mRecognitionRefreshRate;
      mRecognitionSensor->setRefreshRate(mRecognitionRefreshRate);
      break;
    }
    case C_CAMERA_ENABLE_SEGMENTATION:
      unsigned char segmentationEnabled;
      stream >> segmentationEnabled;
      mSegmentationEnabled = segmentationEnabled;
      if (!hasBeenSetup())
        setup();
      else
        updateOverlayMaskTexture();
      emit enabled(this, isEnabled());
      break;
    default:
      assert(0);
  }
}

void WbCamera::computeObjects(bool finalSetup, bool needCollisionDetection) {
  // compute the camera referential
  const WbVector3 cameraPosition = matrix().translation();
  const WbMatrix3 cameraRotation = rotationMatrix();
  const WbMatrix3 cameraInverseRotation = cameraRotation.transposed();
  const double horizontalFieldOfView = fieldOfView();
  const double verticalFieldOfView = (horizontalFieldOfView * height()) / width();
  const WbAffinePlane *frustumPlanes = WbObjectDetection::computeFrustumPlanes(
    cameraPosition, cameraRotation, verticalFieldOfView, horizontalFieldOfView, recognition()->maxRange());

  // loop for each possible target to check if it is visible
  const QList<WbSolid *> objects = WbWorld::instance()->cameraRecognitionObjects();
  for (int i = 0; i < objects.size(); i++) {
    WbSolid *object = objects.at(i);
    if (object == this || robot() == object || robot()->solidChildren().contains(object))
      continue;
    // We should discard targets as soon as possible to improve performance.
    if ((cameraPosition - object->matrix().translation() - object->boundingSphere()->center()).length() >
        (recognition()->maxRange() + object->boundingSphere()->radius()))
      continue;
    // create target
    WbRecognizedObject *generatedObject =
      new WbRecognizedObject(this, object, needCollisionDetection, recognition()->maxRange());
    if (finalSetup) {
      const bool valid =
        computeObject(cameraPosition, cameraRotation, cameraInverseRotation, frustumPlanes, generatedObject, false);
      if (!valid) {
        delete generatedObject;
        continue;
      }
    }
    mRecognizedObjects.append(generatedObject);
  }

  delete[] frustumPlanes;
}

WbVector2 WbCamera::projectOnImage(const WbVector3 &position) {
  const int w = width();
  const int h = height();
  const double fovX = fieldOfView();
  const double fovY = WbWrenCamera::computeFieldOfViewY(fovX, (double)w / h);
  const double theta1 = atan2(position.x(), fabs(position.z()));
  const double theta2 = atan2(position.y(), fabs(position.z()));
  int u = (double)w * (0.5 * tan(theta1) / tan(0.5 * fovX) + 0.5);
  int v = (double)h * (0.5 - 0.5 * tan(theta2) / tan(0.5 * fovY));
  u = qMax(0, qMin(u, w - 1));
  v = qMax(0, qMin(v, h - 1));
  return WbVector2(u, v);
}

bool WbCamera::computeObject(const WbVector3 &cameraPosition, const WbMatrix3 &cameraRotation,
                             const WbMatrix3 &cameraInverseRotation, const WbAffinePlane *frustumPlanes,
                             WbRecognizedObject *recognizedObject, bool fromRayUpdate) {
  assert(recognizedObject);

  if (!fromRayUpdate) {
    if (!recognizedObject->computeObject(cameraPosition, cameraRotation, cameraInverseRotation, frustumPlanes))
      return false;
  }

  // compute object relative orientation
  WbRotation relativeRotation;
  relativeRotation.fromMatrix3(recognizedObject->object()->rotationMatrix() * cameraInverseRotation);
  relativeRotation.normalize();
  recognizedObject->setRelativeOrientation(relativeRotation);

  // set the objects colors
  for (int j = 0; j < recognizedObject->object()->recognitionColorSize(); ++j)
    recognizedObject->addColor(recognizedObject->object()->recognitionColor(j));

  // set the object model
  recognizedObject->setModel(recognizedObject->object()->model());

  // compute position and size in the camera image
  const WbVector3 objectSize = recognizedObject->objectSize();
  const WbVector2 centerPosition = projectOnImage(recognizedObject->objectRelativePosition());
  double minU = centerPosition.x();
  double minV = centerPosition.y();
  double maxU = centerPosition.x();
  double maxV = centerPosition.y();
  const WbVector3 corners[8] = {
    // corners of the bounding box of the object in the camera referential
    recognizedObject->objectRelativePosition() + 0.5 * WbVector3(objectSize.x(), objectSize.y(), objectSize.z()),
    recognizedObject->objectRelativePosition() + 0.5 * WbVector3(-objectSize.x(), objectSize.y(), objectSize.z()),
    recognizedObject->objectRelativePosition() + 0.5 * WbVector3(objectSize.x(), -objectSize.y(), objectSize.z()),
    recognizedObject->objectRelativePosition() + 0.5 * WbVector3(-objectSize.x(), -objectSize.y(), objectSize.z()),
    recognizedObject->objectRelativePosition() + 0.5 * WbVector3(objectSize.x(), objectSize.y(), -objectSize.z()),
    recognizedObject->objectRelativePosition() + 0.5 * WbVector3(-objectSize.x(), objectSize.y(), -objectSize.z()),
    recognizedObject->objectRelativePosition() + 0.5 * WbVector3(objectSize.x(), -objectSize.y(), -objectSize.z()),
    recognizedObject->objectRelativePosition() + 0.5 * WbVector3(-objectSize.x(), -objectSize.y(), -objectSize.z())};
  for (int i = 0; i < 8; ++i) {  // project each of the corners in the camera image
    const WbVector2 &positionOnImage = projectOnImage(corners[i]);
    if (positionOnImage.x() < minU)
      minU = positionOnImage.x();
    if (positionOnImage.y() < minV)
      minV = positionOnImage.y();
    if (positionOnImage.x() > maxU)
      maxU = positionOnImage.x();
    if (positionOnImage.y() > maxV)
      maxV = positionOnImage.y();
  }
  recognizedObject->setPositionOnImage(WbVector2(round((maxU + minU) / 2.0), round((maxV + minV) / 2.0)));
  recognizedObject->setPixelSize(WbVector2(ceil(maxU - minU), ceil(maxV - minV)));
  if (recognizedObject->pixelSize().x() < 1 || recognizedObject->pixelSize().y() < 1)
    return false;

  return true;
}

bool WbCamera::refreshRecognitionSensorIfNeeded() {
  assert(recognition());

  if (!isPowerOn() || !mRecognitionSensor->needToRefresh())
    return false;

  if (!recognition()->occlusion())
    // no need of ODE ray collision detection
    // rays can be created at the end of the step when all the body positions are up-to-date
    computeObjects(true, false);

  // post process objects
  if (recognition()->occlusion())
    removeOccludedRecognizedObjects();

  // check the number of objects and keep only the biggest (in pixel size)
  if (recognition()->maxObjects() > 0 && mRecognizedObjects.size() > recognition()->maxObjects()) {
    QMultiMap<int, WbRecognizedObject *> objectsMap;
    for (int i = 0; i < mRecognizedObjects.size(); ++i) {
      const int pixelSurface = mRecognizedObjects.at(i)->pixelSize().x() * mRecognizedObjects.at(i)->pixelSize().y();
      objectsMap.insert(pixelSurface, mRecognizedObjects.at(i));
    }
    mRecognizedObjects.clear();
    QMapIterator<int, WbRecognizedObject *> i(objectsMap);
    i.toBack();
    while (i.hasPrevious() && mRecognizedObjects.size() < recognition()->maxObjects()) {
      i.previous();
      mRecognizedObjects.append(i.value());
    }
    // delete the remaining ones
    while (i.hasPrevious()) {
      i.previous();
      delete i.value();
    }
  }

  mRecognitionSensor->updateTimer();
  return true;
}

void WbCamera::removeOccludedRecognizedObjects() {
  for (int i = mRecognizedObjects.size() - 1; i >= 0; --i) {
    if (mRecognizedObjects.at(i)->hasCollided()) {
      delete mRecognizedObjects[i];
      mRecognizedObjects.removeAt(i);
    }
  }
}

void WbCamera::createWrenCamera() {
  // Safely detach lens flare postProcessingEffect from camera viewport (before it is deleted)
  if (lensFlare())
    lensFlare()->detachFromViewport();

  WbAbstractCamera::createWrenCamera();

  if (mSegmentationCamera)
    delete mSegmentationCamera;
  if (recognition() && recognition()->segmentation()) {
    mSegmentationCamera = new WbWrenCamera(wrenNode(), width(), height(), nearValue(), minRange(), recognition()->maxRange(),
                                           fieldOfView(), 's', false, mSpherical->value());
    connect(mSensor, &WbSensor::stateChanged, this, &WbCamera::updateOverlayMaskTexture);
  } else {
    mSegmentationCamera = NULL;
    disconnect(mSensor, &WbSensor::stateChanged, this, &WbCamera::updateOverlayMaskTexture);
  }

  applyFocalSettingsToWren();
  applyFarToWren();
  updateExposure();
  updateBloomThreshold();
  updateAmbientOcclusionRadius();

  updateLensFlare();
  connect(mWrenCamera, &WbWrenCamera::cameraInitialized, this, &WbCamera::updateLensFlare);
}

void WbCamera::createWrenOverlay() {
  WbAbstractCamera::createWrenOverlay();

  // mRecognizedObjectsTexture deleted when creating the WREN overlay
  assert(recognition() || !mRecognizedObjectsTexture);
  if (mWrenCamera && recognition()) {
    mRecognizedObjectsTexture = WR_TEXTURE(mOverlay->createForegroundTexture());
    emit textureIdUpdated(mOverlay->foregroundTextureGLId(), FOREGROUND_TEXTURE);
    updateOverlayMaskTexture();
  }
}

void WbCamera::updateOverlayMaskTexture() {
  if (!mOverlay)
    return;

  if (mWrenCamera && mSensor->isEnabled() && recognition() && mSegmentationEnabled && mSegmentationCamera)
    mOverlay->setMaskTexture(mSegmentationCamera->getWrenTexture());
  else
    mOverlay->unsetMaskTexture();
  emit textureIdUpdated(mOverlay->maskTextureGLId(), MASK_TEXTURE);
}

void WbCamera::updateTextureUpdateNotifications(bool enabled) {
  WbAbstractCamera::updateTextureUpdateNotifications(enabled);
  if (!mWrenCamera || !mSegmentationCamera)
    return;
  if (enabled && mExternalWindowEnabled)
    connect(mSegmentationCamera, &WbWrenCamera::textureUpdated, this, &WbRenderingDevice::textureUpdated, Qt::UniqueConnection);
  else
    disconnect(mSegmentationCamera, &WbWrenCamera::textureUpdated, this, &WbRenderingDevice::textureUpdated);
  mSegmentationCamera->enableTextureUpdateNotifications(mExternalWindowEnabled);
}

void WbCamera::setup() {
  WbAbstractCamera::setup();
  createSegmentationCamera();
  if (mSegmentationShm || (recognition() && recognition()->segmentation()))
    initializeSegmentationSharedMemory();

  if (spherical())
    return;

  updateNoiseMaskUrl();
  updateExposure();
  updateAmbientOcclusionRadius();
  updateBloomThreshold();
  connect(mNoiseMaskUrl, &WbSFString::changed, this, &WbCamera::updateNoiseMaskUrl);
}

bool WbCamera::isEnabled() const {
  return (mSegmentationEnabled && recognition() && recognition()->segmentation()) || WbAbstractCamera::isEnabled();
}

bool WbCamera::needToRender() const {
  return WbAbstractCamera::needToRender() ||
         (mSegmentationEnabled && mRecognitionSensor->isEnabled() && mRecognitionSensor->needToRefresh());
}

void WbCamera::render() {
  if (mSegmentationCamera && mSegmentationEnabled && isPowerOn() && mRecognitionSensor->isEnabled() &&
      mRecognitionSensor->needToRefresh()) {
    mSegmentationCamera->render();
    mSegmentationImageChanged = true;
  }
}

/////////////////////
//  Update methods //
/////////////////////

void WbCamera::updateFocus() {
  if (focus())
    connect(focus(), &WbFocus::focusSettingsChanged, this, &WbCamera::applyFocalSettingsToWren);
  else if (hasBeenSetup())
    mNeedToConfigure = true;
  if (hasBeenSetup())
    applyFocalSettingsToWren();
}

void WbCamera::updateRecognition() {
  if (!hasBeenSetup())
    return;

  const WbRecognition *recognitionNode = recognition();
  if (recognitionNode && !mOverlay->foregroundTexture()) {
    mRecognizedObjectsTexture = WR_TEXTURE(mOverlay->createForegroundTexture());
    emit textureIdUpdated(mOverlay->foregroundTextureGLId(), FOREGROUND_TEXTURE);
  } else if (mOverlay->foregroundTexture()) {
    mOverlay->deleteForegroundTexture(true);
    emit textureIdUpdated(mOverlay->foregroundTextureGLId(), FOREGROUND_TEXTURE);
    mRecognizedObjectsTexture = NULL;
  }

  // clear mRecognizedObjects if Recognition node changed but not if `Recognition.segmentation` changed
  qDeleteAll(mRecognizedObjects);
  mRecognizedObjects.clear();

  createSegmentationCamera();

  mNeedToConfigure = true;
}

void WbCamera::updateSegmentation() {
  mSegmentationChanged = true;
  if (mSegmentationEnabled && (!recognition() || !recognition()->segmentation())) {
    mSegmentationEnabled = false;
    updateOverlayMaskTexture();
  }
  createSegmentationCamera();
}

void WbCamera::createSegmentationCamera() {
  const WbRecognition *recognitionNode = recognition();
  if (recognitionNode)
    connect(recognitionNode, &WbRecognition::segmentationChanged, this, &WbCamera::updateSegmentation, Qt::UniqueConnection);

  delete mSegmentationCamera;

  if (recognitionNode && recognitionNode->segmentation()) {
    mSegmentationCamera = new WbWrenCamera(wrenNode(), width(), height(), nearValue(), minRange(), recognition()->maxRange(),
                                           fieldOfView(), 's', false, mSpherical->value());
    connect(mSensor, &WbSensor::stateChanged, this, &WbCamera::updateOverlayMaskTexture);
    if (!mSegmentationShm)
      initializeSegmentationSharedMemory();
  } else {
    mSegmentationCamera = NULL;
    disconnect(mSensor, &WbSensor::stateChanged, this, &WbCamera::updateOverlayMaskTexture);
  }
  updateOverlayMaskTexture();
  if (mExternalWindowEnabled)
    updateTextureUpdateNotifications(mExternalWindowEnabled);
}

void WbCamera::updateLensFlare() {
  if (hasBeenSetup() && lensFlare()) {
    if (spherical()) {
      parsingWarn(tr("Lens flare cannot be applied to spherical cameras."));
      return;
    }
    WrViewport *viewport = mWrenCamera->getSubViewport(WbWrenCamera::CAMERA_ORIENTATION_FRONT);
    lensFlare()->setup(viewport);
  }
}

void WbCamera::updateNear() {
  if (WbFieldChecker::resetDoubleIfNonPositive(this, mNear, 0.01))
    return;

  mNeedToConfigure = true;

  if (mFar->value() > 0.0 and mFar->value() < mNear->value()) {
    mNear->setValue(mFar->value());
    parsingWarn(tr("'near' is greater than 'far'. Setting 'near' to %1.").arg(mNear->value()));
  }

  if (hasBeenSetup())
    applyNearToWren();

  if (areWrenObjectsInitialized()) {
    applyFrustumToWren();
    if (!spherical() && hasBeenSetup())
      updateFrustumDisplay();
  }
}

void WbCamera::updateFar() {
  if (WbFieldChecker::resetDoubleIfNegative(this, mFar, 0.0))
    return;

  if (mFar->value() > 0.0 and mFar->value() < mNear->value()) {
    mFar->setValue(mNear->value() + 1.0);
    parsingWarn(tr("'far' is less than 'near'. Setting 'far' to %1.").arg(mFar->value()));
    return;
  }

  if (hasBeenSetup())
    applyFarToWren();

  if (areWrenObjectsInitialized())
    applyFrustumToWren();
}

void WbCamera::updateExposure() {
  if (WbFieldChecker::resetDoubleIfNegative(this, mExposure, 1.0))
    return;

  if (mWrenCamera)
    mWrenCamera->setExposure(mExposure->value());
}

void WbCamera::updateAmbientOcclusionRadius() {
  WbFieldChecker::resetDoubleIfNegative(this, mAmbientOcclusionRadius, 2.0);

  if (mWrenCamera)
    mWrenCamera->setAmbientOcclusionRadius(mAmbientOcclusionRadius->value());
}

void WbCamera::updateBloomThreshold() {
  WbFieldChecker::resetDoubleIfNegativeAndNotDisabled(this, mBloomThreshold, 21.0, -1.0);

  if (mWrenCamera)
    mWrenCamera->setBloomThreshold(mBloomThreshold->value());
}

void WbCamera::updateNoiseMaskUrl() {
  if (!hasBeenSetup())
    return;

  QString noiseMaskUrl = mNoiseMaskUrl->value();
  if (!noiseMaskUrl.isEmpty()) {  // use custom noise mask
    QIODevice *device;
    if (WbUrl::isWeb(noiseMaskUrl)) {
      if (isPostFinalizedCalled() && mDownloader == NULL) {
        // url was changed from the scene tree or supervisor
        downloadAssets();
        return;
      }
      assert(mDownloader);
      if (!mDownloader->error().isEmpty()) {
        warn(mDownloader->error());
        delete mDownloader;
        mDownloader = NULL;
        return;
      }
      device = mDownloader->device();
      assert(device);
    } else {
      noiseMaskUrl = WbUrl::computePath(this, "noiseMaskUrl", noiseMaskUrl);
      device = NULL;
    }
    const QString error = mWrenCamera->setNoiseMask(noiseMaskUrl.toUtf8().constData(), device);
    if (!error.isEmpty())
      parsingWarn(error);
    delete mDownloader;
    mDownloader = NULL;
  }
}

bool WbCamera::isFrustumEnabled() const {
  return WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_CAMERA_FRUSTUMS);
}

void WbCamera::updateFrustumDisplayIfNeeded(int optionalRendering) {
  if (optionalRendering == WbWrenRenderingContext::VF_CAMERA_FRUSTUMS)
    updateFrustumDisplay();
}

/////////////////////
//  Apply methods  //
/////////////////////

void WbCamera::applyFocalSettingsToWren() {
  if (hasBeenSetup()) {
    if (focus())
      mWrenCamera->setFocus(focus()->focalDistance(), focus()->focalLength());
    else
      mWrenCamera->setFocus(0.0, 0.0);
    mNeedToConfigure = true;
  }
}

void WbCamera::applyFarToWren() {
  mWrenCamera->setFar(mFar->value());
  if (mSegmentationCamera)
    mSegmentationCamera->setFar(mFar->value());
}

void WbCamera::applyCameraSettingsToWren() {
  WbAbstractCamera::applyCameraSettingsToWren();
  applyFocalSettingsToWren();
}

void WbCamera::applyNearToWren() {
  WbAbstractCamera::applyNearToWren();
  if (mSegmentationCamera)
    mSegmentationCamera->setNear(nearValue());
}

void WbCamera::applyFieldOfViewToWren() {
  WbAbstractCamera::applyFieldOfViewToWren();
  if (mSegmentationCamera)
    mSegmentationCamera->setFieldOfView(mFieldOfView->value());
}
