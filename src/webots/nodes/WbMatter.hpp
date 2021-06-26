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

#ifndef WB_MATTER_HPP
#define WB_MATTER_HPP

#include "WbSFBool.hpp"
#include "WbSFString.hpp"
#include "WbTransform.hpp"

class WbGeometry;

struct WrRenderable;
struct WrTransform;
struct WrMaterial;
struct WrStaticMesh;

class WbMatter : public WbTransform {
  Q_OBJECT

public:
  // constructors and destructor
  virtual ~WbMatter();

  // reimplemented public functions
  void createWrenObjects() override;
  void postFinalize() override;
  void reset(const QString &id) override;
  void save(const QString &id) override;

  int constraintType() const override;
  void setScaleNeedUpdate() override;

  // field accessors
  const QString &name() const { return mName->value(); }
  const QString &model() const { return mModel->value(); }
  WbBaseNode *boundingObject() const;

  // ODE objects accessors
  dGeomID odeGeom() const;
  dSpaceID upperSpace() const;  // the smallest ODE space containing the boudingObject, possibly itself
  dSpaceID space() const;       // the largest space containing the boundingObject
  dSpaceID groupSpace() const;  // returns the ODE space associated to a Group if the bounding object is a group

  // collision flags
  bool isColliding() const;
  bool wasColliding() const;
  void setColliding();

  bool boundingObjectHasChanged() const { return mBoundingObjectHasChanged; }
  void setBoundingObjectFlag(bool changed) { mBoundingObjectHasChanged = changed; }
  bool hasAvalidBoundingObject() const { return boundingObject() && boundingObject()->isAValidBoundingObject(); }

  // for wb_supervisor_simulation_reset_physics()
  virtual void resetPhysics(bool recursive = true) {}
  virtual void pausePhysics(bool resumeAutomatically = false) {}
  virtual void resumePhysics() {}

  // handle artifical moves triggered by the user or a Supervisor
  virtual void jerk(bool resetVelocities = false, bool rootJerk = true) = 0;
  void forwardJerk() override { mNeedToHandleJerk = true; }

  // selection
  void select(bool selected);
  bool isSelected() const { return mSelected; }
  bool isLocked() const { return mLocked->value(); }

  // ODE positioning
  void updateOdeGeomPosition() { updateOdeGeomPosition(odeGeom()); }

  static void enableShowMatterCenter(bool enabled) { cShowMatterCenter = enabled; }

signals:
  void matterNameChanged();
  void matterModelChanged();

public slots:
  // recursions through bounding objects for material updates
  virtual void propagateBoundingObjectMaterialUpdate(bool onSelection = false) = 0;

protected:
  // Abstract class: constructors are reserved for derived classes only
  WbMatter(const WbMatter &other);
  WbMatter(const WbNode &other);
  WbMatter(const QString &modelName, WbTokenizer *tokenizer);

  const QString &vrmlName() const override {
    static const QString returnedName("Transform");
    return returnedName;
  }

  // Renders the frame axes and the center of mass
  virtual void applyVisibilityFlagsToWren(bool selected);
  virtual void applyChangesToWren();
  void applyMatterCenterToWren();

  // Scale
  void createScaleManipulator() override;
  bool checkScalingPhysicsConstraints(WbVector3 &correctedScale, int constraintType, bool warning) const override;
  virtual void propagateScale();
  bool checkScaleAtLoad(bool warning);

  WbSFString *mName;
  WbSFNode *mBoundingObject;

  // Bounding Object
  bool isBoundingObjectFinalizationCompleted(WbBaseNode *node);
  bool checkBoundingObject() const;
  bool mBoundingObjectHasChanged;

  dGeomID createOdeGeomFromNode(dSpaceID space, WbBaseNode *node);
  dGeomID createOdeGeomFromBoundingObject(dSpaceID space);
  void updateOdeGeomPosition(dGeomID g);
  bool handleJerkIfNeeded();

  virtual bool isInsertedOdeGeomPositionUpdateRequired() const { return true; }

  // Sleep flag
  void updateSleepFlag();

protected slots:
  void updateScale(bool warning = false) override = 0;
  virtual void updateLineScale();
  void updateTranslation() override;
  void updateRotation() override;
  virtual void removeBoundingGeometry() {}
  virtual void boundingObjectFinalizationCompleted(WbBaseNode *node);
  virtual void updateName();

private:
  WbMatter &operator=(const WbMatter &);  // non copyable
  void init();

  WbSFString *mModel;
  WbSFString *mDescription;
  WbSFBool *mLocked;

  // Selection
  bool mSelected;

  bool mNeedToHandleJerk;

  void updateOdePlaceableGeomPosition(dGeomID g);
  virtual void handleJerk() = 0;

  // Update static geom position
  void updateOdePlanePosition(dGeomID plane);

  dGeomID createOdeGeomFromGroup(dSpaceID space, WbGroup *group);
  dGeomID createOdeGeomFromGeometry(dSpaceID space, WbGeometry *geometry, bool setOdeData = true);
  dGeomID createOdeGeomFromTransform(dSpaceID space, WbTransform *transform);
  void disconnectFromBoundingObjectUpdates(const WbNode *node) const;

  virtual void createOdeGeoms() = 0;
  virtual void applyToOdeScale() = 0;  // rescale all the ODE dGeoms lying inside the Bounding Object when the WbMatter's scale
                                       // field has changed

  virtual void setGeomMatter(dGeomID g, WbBaseNode *node = NULL) = 0;

  WrTransform *mMatterCenterTransform;
  WrRenderable *mMatterCenterRenderable;
  WrMaterial *mMatterCenterMaterial;
  WrStaticMesh *mMatterCenterMesh;

  void connectNameUpdates() const;

  static bool cShowMatterCenter;

private slots:
  virtual void updateBoundingObject() = 0;
  virtual void createOdeGeomFromInsertedGroupItem(WbBaseNode *node) = 0;
  void createOdeGeomFromInsertedTransformItem();
  void createOdeGeomFromInsertedShapeItem();
  void insertValidGeometryInBoundingObject();
  void updateManipulatorVisibility();
  void updateLocked();
};

#endif
