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

#ifndef WB_CAMERA_HPP
#define WB_CAMERA_HPP

#include "WbAbstractCamera.hpp"

struct WrTexture;

class WbAffinePlane;
class WbDownloader;
class WbFocus;
class WbLensFlare;
class WbWrenLabelOverlay;
class WbRecognition;
class WbRecognizedObject;
class WbZoom;

class WbCamera : public WbAbstractCamera {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbCamera(WbTokenizer *tokenizer = NULL);
  WbCamera(const WbCamera &other);
  explicit WbCamera(const WbNode &other);
  virtual ~WbCamera();

  // reimplemented public functions
  void downloadAssets() override;
  void preFinalize() override;
  void postFinalize() override;
  void writeAnswer(QDataStream &) override;
  void writeConfigure(QDataStream &) override;
  void handleMessage(QDataStream &) override;
  int nodeType() const override { return WB_NODE_CAMERA; }
  void prePhysicsStep(double ms) override;
  void postPhysicsStep() override;
  void reset(const QString &id) override;
  void resetSharedMemory() override;
  bool isEnabled() const override;
  void updateTextureUpdateNotifications(bool enabled) override;

  // specific functions
  void rayCollisionCallback(dGeomID geom, WbSolid *collidingSolid, double depth);
  QString pixelInfo(int x, int y) const override;
  void updateRecognizedObjectsOverlay(double screenX, double screenY, double overlayX, double overlayY);
  void clearRecognizedObjectsOverlay();
  WbRgb enabledCameraFrustrumColor() const override { return WbRgb(1.0f, 0.0f, 1.0f); }

  virtual WrTexture *getWrenTexture();

protected:
  void setup() override;
  void render() override;
  bool needToRender() const override;

private:
  WbSFNode *mFocus;
  WbSFNode *mZoom;
  WbSFNode *mRecognition;
  WbSFString *mNoiseMaskUrl;
  WbSFBool *mAntiAliasing;
  WbSFDouble *mAmbientOcclusionRadius;
  WbSFDouble *mBloomThreshold;
  WbSFNode *mLensFlare;
  WbSFDouble *mFar;
  WbSFDouble *mExposure;

  // private functions
  void addConfigureToStream(QDataStream &stream, bool reconfigure = false) override;

  WbFocus *focus() const;
  WbZoom *zoom() const;
  WbRecognition *recognition() const;
  WbLensFlare *lensFlare() const;

  WbCamera &operator=(const WbCamera &);  // non copyable
  WbNode *clone() const override { return new WbCamera(*this); }
  void init();
  void initializeImageSharedMemory() override;

  int size() const override { return 4 * width() * height(); }
  double minRange() const override { return mNear->value(); }
  double maxRange() const override { return mFar->value(); }
  bool antiAliasing() const override { return mAntiAliasing->value(); }
  bool isFrustumEnabled() const override;

  // WREN methods
  void createWrenCamera() override;
  void createWrenOverlay() override;
  void createLensFlare();

  // smart camera
  void displayRecognizedObjectsInOverlay();
  bool refreshRecognitionSensorIfNeeded();
  void removeOccludedRecognizedObjects();
  WbVector2 projectOnImage(const WbVector3 &position);
  void computeObjects(bool finalSetup, bool needCollisionDetection);
  bool computeObject(const WbVector3 &cameraPosition, const WbMatrix3 &cameraRotation, const WbMatrix3 &cameraInverseRotation,
                     const WbAffinePlane *frustumPlanes, WbRecognizedObject *recognizedObject, bool fromRayUpdate);
  void updateRaysSetupIfNeeded() override;
  short mRecognitionRefreshRate;
  bool mNeedToDeleteRecognizedObjectsRays;
  WbSensor *mRecognitionSensor;
  WbWrenLabelOverlay *mLabelOverlay;
  QList<WbRecognizedObject *> mRecognizedObjects;
  QList<WbRecognizedObject *> mInvalidRecognizedObjects;
  WrTexture *mRecognizedObjectsTexture;
  // smart camera segmentation
  void initializeSegmentationSharedMemory();
  void createSegmentationCamera();
  bool mSegmentationEnabled;
  bool mSegmentationChanged;
  WbWrenCamera *mSegmentationCamera;
  WbSharedMemory *mSegmentationShm;
  bool mHasSegmentationSharedMemoryChanged;
  bool mSegmentationImageChanged;
  // URL downloader
  WbDownloader *mDownloader;

private slots:
  void updateFocus();
  void updateRecognition();
  void updateSegmentation();
  void updateNear();
  void updateFar();
  void updateExposure();
  void updateAmbientOcclusionRadius();
  void updateBloomThreshold();
  void updateNoiseMaskUrl();
  void updateLensFlare();
  void applyFocalSettingsToWren();
  void applyFarToWren();
  void applyNearToWren() override;
  void applyFieldOfViewToWren() override;
  void applyCameraSettingsToWren() override;
  void updateFrustumDisplayIfNeeded(int optionalRendering) override;
  void updateOverlayMaskTexture();
};

#endif  // WB_CAMERA_HPP
