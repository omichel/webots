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

#ifndef WB_LIDAR_HPP
#define WB_LIDAR_HPP

#include "WbAbstractCamera.hpp"
#include "WbSFInt.hpp"

#include "../../../include/controller/c/webots/lidar_point.h"

struct WrRenderable;
struct WrDynamicMesh;
struct WrStaticMesh;
struct WrMaterial;

class WbLidar : public WbAbstractCamera {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbLidar(WbTokenizer *tokenizer = NULL);
  WbLidar(const WbLidar &other);
  explicit WbLidar(const WbNode &other);
  virtual ~WbLidar();

  // reimplemented public functions
  void createOdeObjects() override;
  void createWrenObjects() override;
  void preFinalize() override;
  void postFinalize() override;
  void writeAnswer(QDataStream &stream) override;
  void reset(const QString &id) override;
  void updateCollisionMaterial(bool triggerChange = false, bool onSelection = false) override;
  void setSleepMaterial() override;
  void setScaleNeedUpdate() override;
  void attachResizeManipulator() override;
  void detachResizeManipulator() const override;
  void handleMessage(QDataStream &) override;
  int nodeType() const override { return WB_NODE_LIDAR; }
  QString pixelInfo(int x, int y) const override;
  void prePhysicsStep(double ms) override;
  void postPhysicsStep() override;
  void write(WbVrmlWriter &writer) const override;
  WbRgb enabledCameraFrustrumColor() const override { return WbRgb(0.0f, 1.0f, 1.0f); }

  double maxRange() const override { return mMaxRange->value(); }

  // These functions return the value actually used by the lidar (that was initially loaded from the world file or changed
  // before the start of the simulation). It may be different from the current value of the field if it was changed after the
  // start of the simulation. Once the simulation starts, such changes cannot be applied directly and are applied only after a
  // save and reload. This is explained to the user in a warning message.
  int actualNumberOfLayers() const;
  int actualHorizontalResolution() const;
  double actualVerticalFieldOfView() const;
  double actualFieldOfView() const;

  int textureGLId() const override;
  int width() const override;
  int height() const override;
  double fieldOfView() const override { return actualFieldOfView(); }

  WbSolid *solidEndPoint() const;

  // selection
  void propagateSelection(bool selected) override;

  // lazy matrix multiplication system
  void setMatrixNeedUpdate() override;

private:
  // user accessible fields
  WbSFDouble *mTiltAngle;
  WbSFInt *mHorizontalResolution;
  WbSFDouble *mVerticalFieldOfView;
  WbSFInt *mNumberOfLayers;
  WbSFDouble *mMinRange;
  WbSFDouble *mMaxRange;
  WbSFDouble *mResolution;
  WbSFDouble *mDefaultFrequency;
  WbSFDouble *mMinFrequency;
  WbSFDouble *mMaxFrequency;
  WbSFString *mType;
  WbSFNode *mRotatingHead;

  bool mIsPointCloudEnabled;
  double mCurrentRotatingAngle;
  double mPreviousRotatingAngle;
  double mCurrentTiltAngle;

  int mActualNumberOfLayers;
  int mActualHorizontalResolution;
  double mActualVerticalFieldOfView;
  double mActualFieldOfView;
  QString mActualType;

  WrRenderable *mFrustumRenderable;
  WrMaterial *mFrustumMaterial;
  WrStaticMesh *mFrustumMesh;

  WrRenderable *mLidarPointsRenderable;
  WrDynamicMesh *mLidarPointsMesh;
  WrMaterial *mLidarPointsMaterial;

  WrRenderable *mLidarRaysRenderable;
  WrDynamicMesh *mLidarRaysMesh;
  WrMaterial *mLidarRaysMaterial;

  // private functions
  void addConfigureToStream(QDataStream &stream, bool reconfigure = false) override;

  void copyAllLayersToSharedMemory();
  void updatePointCloud(int minWidth, int maxWidth);
  float *lidarImage() const;

  WbLidar &operator=(const WbLidar &);  // non copyable
  WbNode *clone() const override { return new WbLidar(*this); }
  void init();
  void initializeImageSharedMemory() override;

  int size() const override {
    return (sizeof(float) + sizeof(WbLidarPoint)) * actualHorizontalResolution() * actualNumberOfLayers();
  }
  double minRange() const override { return mMinRange->value(); }
  bool isRotating() const { return mActualType.startsWith('r', Qt::CaseInsensitive); }
  double verticalFieldOfView() const { return actualFieldOfView() * ((double)height() / (double)width()); }

  WbLidarPoint *pointArray() { return (WbLidarPoint *)(lidarImage() + actualHorizontalResolution() * actualNumberOfLayers()); }

  // WREN methods
  void createWrenCamera() override;
  void deleteWren();
  void displayPointCloud();
  void hidePointCloud();
  void applyMaxRangeToWren();
  void applyResolutionToWren();
  void applyTiltAngleToWren();

private slots:
  void updateNear();
  void updateMinRange();
  void updateMaxRange();
  void updateResolution();
  void updateTiltAngle();
  void updateType();
  void updateMinFrequency();
  void updateMaxFrequency();
  void updateDefaultFrequency();
  void updateHorizontalResolution();
  void updateVerticalFieldOfView();
  void updateNumberOfLayers();
  void updateRotatingHead();
  void updateBoundingSphere(WbBaseNode *subNode);
  void applyCameraSettingsToWren() override;
  void applyFrustumToWren() override;
  void updateOptionalRendering(int option) override;
  void updateFieldOfView() override;
};

#endif  // WB_LIDAR_HPP
