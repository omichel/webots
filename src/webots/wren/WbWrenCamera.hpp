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

#ifndef WB_WREN_CAMERA_HPP
#define WB_WREN_CAMERA_HPP

#include "WbRgb.hpp"
#include "WbVector2.hpp"

#include "wren/texture.h"

#include <QtCore/QObject>
#include <QtCore/QVector>

class QIODevice;

struct WrCamera;
struct WrPostProcessingEffect;
struct WrFrameBuffer;
struct WrTransform;
struct WrTexture;
struct WrTexture2d;
struct WrViewport;

class WbWrenBloom;
class WbWrenColorNoise;
class WbWrenDepthOfField;
class WbWrenGtao;
class WbWrenHdr;
class WbWrenLensDistortion;
class WbWrenMotionBlur;
class WbWrenNoiseMask;
class WbWrenRangeNoise;
class WbWrenRangeQuantization;
class WbWrenSmaa;

class WbWrenCamera : public QObject {
  Q_OBJECT

public:
  enum CameraOrientation {
    CAMERA_ORIENTATION_FRONT,
    CAMERA_ORIENTATION_RIGHT,
    CAMERA_ORIENTATION_BACK,
    CAMERA_ORIENTATION_LEFT,
    CAMERA_ORIENTATION_UP,
    CAMERA_ORIENTATION_DOWN,
    CAMERA_ORIENTATION_COUNT
  };

  WbWrenCamera(WrTransform *node, int width, int height, float nearValue, float minRange, float maxRange, float fov, char type,
               bool hasAntiAliasing, bool isSpherical);

  virtual ~WbWrenCamera();

  bool isSpherical() { return mIsSpherical; }
  bool isSubCameraActive(int cameraIndex) { return mIsCameraActive[cameraIndex]; }
  WrCamera *getSubCamera(int cameraIndex) { return mCamera[cameraIndex]; }
  WrViewport *getSubViewport(int cameraIndex) { return mCameraViewport[cameraIndex]; }

  WrTexture *getWrenTexture() const;
  int textureGLId() const;

  void render();

  void setSize(int width, int height);
  void setNear(float nearValue);
  void setFar(float farValue);
  void setExposure(float exposure);
  void setAmbientOcclusionRadius(float radius);
  void setBloomThreshold(float threshold);
  void setMinRange(float minRange);
  void setMaxRange(float maxRange);
  void setFieldOfView(float fov);
  void setColorNoise(float colorNoise);
  void setRangeNoise(float rangeNoise);
  void setRangeResolution(float resolution);
  void setMotionBlur(float blur);
  void setFocus(float distance, float length);
  void enableLensDistortion();
  void disableLensDistortion();
  void setLensDistortionCenter(const WbVector2 &center);
  void setRadialLensDistortionCoefficients(const WbVector2 &coefficients);
  void setTangentialLensDistortionCoefficients(const WbVector2 &coefficients);
  QString setNoiseMask(const char *noiseMaskTexturePath, QIODevice *device);

  void enableCopying(bool enable);
  WbRgb copyPixelColourValue(int x, int y);
  void copyContentsToMemory(void *data, int offset = 0, int size = 0);

  void enableTextureUpdateNotifications(bool enabled) { mNotifyOnTextureUpdate = enabled; }

  void rotateYaw(float angle);
  void rotatePitch(float angle);

  static float computeFieldOfViewY(double fovX, double aspectRatio);

signals:
  void cameraInitialized();
  void textureUpdated();

public slots:
  void setBackgroundColor(const WbRgb &color);

private:
  void init();
  void cleanup();
  void setupCamera(int index, int width, int height);
  void setupSphericalSubCameras();
  void setupCameraPostProcessing(int index);
  void setupSphericalPostProcessingEffect();
  void setCamerasOrientations();
  void setFovy(float fov);
  void setAspectRatio(float aspectRatio);
  void updatePostProcessingParameters(int index);
  void applySphericalPostProcessingEffect();

  WrTransform *mNode;
  WbRgb mBackgroundColor;

  int mWidth;
  int mHeight;
  float mNear;
  float mExposure;
  float mAmbientOcclusionRadius;
  float mBloomThreshold;
  float mMinRange;
  float mMaxRange;
  float mFieldOfView;
  char mType;
  bool mIsColor;
  bool mAntiAliasing;
  bool mIsSpherical;
  bool mFirstRenderingCall;
  bool mIsCopyingEnabled;
  bool mNotifyOnTextureUpdate;

  bool mIsCameraActive[CAMERA_ORIENTATION_COUNT];               // store if the camera is active (in spherical case)
  WrCamera *mCamera[CAMERA_ORIENTATION_COUNT];                  // maximum 6 cameras in case of 'full-spherical'
  WrViewport *mCameraViewport[CAMERA_ORIENTATION_COUNT];        // maximum 6 viewports in case of 'full-spherical'
  WrFrameBuffer *mCameraFrameBuffer[CAMERA_ORIENTATION_COUNT];  // maximum 6 framebuffers in case of 'full-spherical'
  // maximum 6 viewports need to be rendered to in case of 'full-spherical'
  WrViewport *mViewportsToRender[CAMERA_ORIENTATION_COUNT];

  // spherical camera only
  float mSphericalFieldOfViewX;
  float mSphericalFieldOfViewY;
  int mSubCamerasResolutionX;
  int mSubCamerasResolutionY;

  // this ratio is used to artificially increase the sub-camera Y resolution
  // in order to fix issue due to the spherical projection when only horizontal sub-cameras are enabled
  float mSphericalFovYCorrectionCoefficient;

  QVector<WrPostProcessingEffect *> mPostProcessingEffects;
  WrPostProcessingEffect *mSphericalPostProcessingEffect;
  WrPostProcessingEffect *mUpdateTextureFormatEffect;
  WrFrameBuffer *mResultFrameBuffer;
  WrTextureInternalFormat mTextureFormat;

  WbWrenBloom *mWrenBloom[CAMERA_ORIENTATION_COUNT];
  WbWrenColorNoise *mWrenColorNoise[CAMERA_ORIENTATION_COUNT];
  WbWrenDepthOfField *mWrenDepthOfField[CAMERA_ORIENTATION_COUNT];
  WbWrenGtao *mWrenGtao[CAMERA_ORIENTATION_COUNT];
  WbWrenHdr *mWrenHdr[CAMERA_ORIENTATION_COUNT];
  WbWrenLensDistortion *mWrenLensDistortion[CAMERA_ORIENTATION_COUNT];
  WbWrenMotionBlur *mWrenMotionBlur[CAMERA_ORIENTATION_COUNT];
  WbWrenNoiseMask *mWrenNoiseMask[CAMERA_ORIENTATION_COUNT];
  WbWrenRangeNoise *mWrenRangeNoise[CAMERA_ORIENTATION_COUNT];
  WbWrenRangeQuantization *mWrenRangeQuantization[CAMERA_ORIENTATION_COUNT];
  WbWrenSmaa *mWrenSmaa[CAMERA_ORIENTATION_COUNT];
  const float *mInverseViewMatrix[CAMERA_ORIENTATION_COUNT];

  float mColorNoiseIntensity;
  float mRangeNoiseIntensity;
  float mDepthResolution;
  float mFocusDistance;
  float mFocusLength;
  bool mIsLensDistortionEnabled;
  WbVector2 mLensDistortionCenter;
  WbVector2 mLensDistortionRadialCoeffs;
  WbVector2 mLensDistortionTangentialCoeffs;
  float mMotionBlurIntensity;
  WrTexture2d *mNoiseMaskTexture;
  WbVector2 mNoiseMaskTextureFactor;
};

#endif
