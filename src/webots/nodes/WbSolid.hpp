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

#ifndef WB_SOLID_HPP
#define WB_SOLID_HPP

#include "WbHiddenKinematicParameters.hpp"
#include "WbMFColor.hpp"
#include "WbMatter.hpp"
#include "WbPolygon.hpp"
#include "WbSolidMerger.hpp"
#include "WbSolidUtilities.hpp"

#include <QtCore/QList>
#include <QtCore/QPointer>
#include <QtCore/QSet>

class WbBasicJoint;
class WbPhysics;
class WbPropeller;
class WbRobot;
class WbSupportPolygonRepresentation;

struct WrTransform;
struct WrRenderable;
struct WrStaticMesh;
struct WrMaterial;

struct dImmersionGeom;

class WbSolid : public WbMatter {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbSolid(WbTokenizer *tokenizer = NULL);
  WbSolid(const WbSolid &other);
  explicit WbSolid(const WbNode &other);
  virtual ~WbSolid();

  // list of finalized solids
  static const QList<const WbSolid *> &solids() { return cSolids; }

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_SOLID; }
  void createWrenObjects() override;
  void preFinalize() override;
  void validateProtoNode() override;
  void postFinalize() override;
  void propagateSelection(bool selected) override;
  void saveHiddenFieldValues() const;
  void setMatrixNeedUpdate() override;
  void reset(const QString &id) override;
  void save(const QString &id) override;

  // processing before / after ODE world step
  virtual void prePhysicsStep(double ms);
  virtual void postPhysicsStep();

  void setScaleNeedUpdate() override;
  void createOdeObjects() override;

  // field accessors
  const QString &contactMaterial() const { return mContactMaterial->value(); }
  WbPhysics *physics() const;
  const WbMFNode &immersionProperties() const { return *mImmersionProperties; }
  float radarCrossSection() const { return mRadarCrossSection->value(); }
  int recognitionColorSize() const { return mRecognitionColors->size(); }
  const WbRgb &recognitionColor(int index) const { return mRecognitionColors->item(index); }
  // hidden field accessors
  const WbVector3 &linearVelocity() const { return mLinearVelocity->value(); }
  const WbVector3 &angularVelocity() const { return mAngularVelocity->value(); }

  // return the velocity relatively to a parent Solid (or absolute velocity if parentSolid is NULL)
  WbVector3 relativeLinearVelocity(const WbSolid *parentSolid = NULL) const;
  WbVector3 relativeAngularVelocity(const WbSolid *parentSolid = NULL) const;

  void setLinearVelocity(const double velocity[3]);
  void setAngularVelocity(const double velocity[3]);

  // ODE objects accessors
  dJointID joint() const { return mJoint; }
  dBodyID body() const;
  double mass() const;
  const double *inertiaMatrix() const;
  double globalMass() const { return mGlobalMass; }
  double globalVolume() const { return mGlobalVolume; }
  const WbVector3 &globalCenterOfMass() const { return mGlobalCenterOfMass; }
  // global center of mass: adds up the solid itself and all its descendants
  const WbVector3 &computedGlobalCenterOfMass() {
    updateGlobalCenterOfMass();
    return mGlobalCenterOfMass;
  }
  void updateCenterOfMass();  // update the center of mass according to the Physics node specification
  const WbVector3 &centerOfMass() const { return mCenterOfMass; }
  const WbVector3 &scaledCenterOfMass() const { return mScaledCenterOfMass; }
  dMass *referenceMass() const { return mReferenceMass; }
  // returns inertia and CoM relative to solid center in the local frame coordinates
  dMass *odeMass() const { return mOdeMass; }

  const WbVector3 &centerOfBuoyancy() const { return mCenterOfBuoyancy; }

  const QVector<WbVector3> &contactPoints(bool includeDescendants = false) const {
    return includeDescendants ? mGlobalListOfContactPoints : mListOfContactPoints;
  }
  const QVector<WbVector3> &computedContactPoints(bool includeDescendants = false);
  const QVector<const WbSolid *> &computedSolidPerContactPoints();

  // accessors to stored fields
  const WbVector3 translationFromFile() const { return mSavedTranslations[stateId()]; }
  const WbRotation rotationFromFile() const { return mSavedRotations[stateId()]; }
  void setTranslationFromFile(const WbVector3 &translation) { mSavedTranslations[stateId()] = translation; }
  void setRotationFromFile(const WbRotation &rotation) { mSavedRotations[stateId()] = rotation; }

  // Solid merger
  QPointer<WbSolidMerger> solidMerger() const { return mSolidMerger; }
  void setupSolidMerger();
  void setupSolidMergers();
  bool isSolidMerger() const { return !mIsKinematic && mSolidMerger && mSolidMerger->solid() == this; }
  bool mergerIsSet() const { return mMergerIsSet; }
  dBodyID bodyMerger() const;

  // New joints
  void appendJointParent(WbBasicJoint *joint);
  void removeJointParent(WbBasicJoint *joint);

  // set up joints for special nodes:
  // - fixed joint between TouchSensor and parent body
  // - fixed joint between dynamic solid child and kinematic parent body (static environment)
  void setOdeJointToUpperSolid();

  // dynamical state
  bool isDynamic() const { return !mIsKinematic; }
  bool isKinematic() const { return mIsKinematic; }

  // sleeping flags
  bool isSleeping() const;

  // awakening
  void awake();
  static void awakeSolids(WbGroup *group);

  void resetPhysics(bool recursive = true) override;
  // pause/resume physics computation on the current solid and its descandants
  void pausePhysics(bool resumeAutomatically = false) override;
  void resumePhysics() override;

  // handle artifical moves triggered by the user or a Supervisor
  void jerk(bool resetVelocities = true, bool rootJerk = true) override;
  void notifyChildJerk(WbTransform *childNode);

  // physics setters
  void addForceAtPosition(const WbVector3 &force, const WbVector3 &position);
  void addTorque(const WbVector3 &torque);

  // physics node setter
  void setInertiaMatrixFromBoundingObject();

  // selection
  void enable(bool enabled, bool ode = true);  // remove a kinematic solid from simulation or reintroduce it

  // support polygon representation
  bool supportPolygonRepresentationEnabled() const { return mSupportPolygonRepresentationIsEnabled; }
  bool showSupportPolygonRepresentation(bool enabled);
  unsigned char staticBalance();  // returns zero if unstable, one otherwise

  // global center of mass representation
  bool globalCenterOfMassRepresentationEnabled() const { return mGlobalCenterOfMassRepresentationIsEnabled; }
  bool showGlobalCenterOfMassRepresentation(bool enabled);

  // utilities
  WbRobot *robot() const;
  bool isTopSolid() const { return (this == topSolid()); }
  virtual WbSolid *findSolid(const QString &name, const WbSolid *const exception = NULL);
  bool belongsToStaticBasis() const;  // Returns true if all solid ancestors have no physics

  // lists of WbSolid and WbBasicJoint children
  const QVector<WbSolid *> &solidChildren() const { return mSolidChildren; }
  const QVector<WbBasicJoint *> &jointChildren() const { return mJointChildren; }

  // ODE mass adjustments
  void correctOdeMass(const dMass *mass, WbBaseNode *node, bool adjustSolidMass = true);

  // ODE positioning
  void resetJointsToLinkedSolids();  // reset joint to any linked solid to this one

  // update the node transform matrix based on the newly computed ODE transform matrix
  // it loops through all the ancestor Solid nodes with a body and updates them
  void updateTransformForPhysicsStep();

  // Density
  double volume() const;
  double density() const;
  double averageDensity() const;
  void updateGlobalVolume();

  // Contact points management
  const WbPolygon &supportPolygon();

  // Immersion
  bool showCenterOfBuoyancyRepresentation(bool enabled);
  bool centerOfBuoyancyRepresentationEnabled() const { return mCenterOfBuoyancyRepresentationIsEnabled; }

  // Collecting and restoring info on descendants
  void collectSolidDescendantNames(QStringList &items, const WbSolid *const solidException = NULL) const;
  void collectHiddenKinematicParameters(WbHiddenKinematicParameters::HiddenKinematicParametersMap &map,
                                        int &counter) const override;

  // Threshold to handle mass round off errors after resize / rescale events
  static const double MASS_ZERO_THRESHOLD;

  WbBasicJoint *jointParent() const;

  // save previous position and orientation
  void savePreviousTransform();

  // print a warning if the current solid is moved in kinematics mode and it
  // has any dynamic Solid descendant node that won't follow it
  void printKinematicWarningIfNeeded();

  // Functions to compute unique name, find a Solid based on the unique name and resolve name clashes
  QString computeUniqueName() const;
  WbSolid *findDescendantSolidFromUniqueName(QStringList &names) const;
  void resolveNameClashIfNeeded(bool automaticallyChange, bool recursive, const QList<WbSolid *> siblings,
                                QSet<const QString> *topSolidNameSet) const;
  static WbSolid *findSolidFromUniqueName(const QString &name);
  static QStringList splitUniqueNamesByEscapedPattern(const QString &text, const QString &pattern);

signals:
  void contactPointsRequested();
  void massPropertiesChanged();
  void physicsPropertiesChanged();
  void positionChangedArtificially();

public slots:
  // recursions through solid children with bounding objects for material updates
  void propagateBoundingObjectMaterialUpdate(bool onSelection = false) override;
  void updateGlobalCenterOfMass();
  void updateGraphicalGlobalCenterOfMass();
  void resetPhysicsIfRequired(bool changedFromSupervisor);

protected:
  // this constructor is reserved for derived classes only
  WbSolid(const QString &modelName, WbTokenizer *tokenizer);

  // physics accessors
  dBodyID upperSolidBody() const;
  const QList<WbBasicJoint *> &jointParents() const { return mJointParents; }
  void setJointParents();

  // to-be-reimplemented in derived classes
  void updateName() override;
  virtual dJointID createJoint(dBodyID body, dBodyID parentBody, dWorldID world) const;
  // check if a ODE joint is needed between current and upper solid
  // special cases for: TouchSensor
  virtual bool needJointToUpperSolid(const WbSolid *upperSolid) const;

  // Avoids joint destruction, which is safer with respect to physics plugins
  void setJoint(dJointID joint, dBodyID body, dBodyID parentBody) const;

  // Renders the frame axes and the center of mass
  void applyVisibilityFlagsToWren(bool selected) override;
  void applyChangesToWren() override;
  void applyMassCenterToWren();

  // Scale
  void propagateScale() override;  // overriden in WbDistanceSensor

  // Solid merger, i.e. solid ancestor (possibly the solid itself) that owns the mass, body and dGeoms of this solid..
  virtual void setSolidMerger();
  // Non NULL only if this solid is dynamic and is not related to its dynamic parent by a joint
  QPointer<WbSolidMerger> mSolidMerger;

  bool isInsertedOdeGeomPositionUpdateRequired() const override { return mIsKinematic; }

  // export
  bool exportNodeHeader(WbVrmlWriter &writer) const override;
  void exportNodeFields(WbVrmlWriter &writer) const override;
  void exportNodeFooter(WbVrmlWriter &writer) const override;
  const QString sanitizedName() const;

protected slots:
  void updateTranslation() override;
  void updateRotation() override;
  void updateScale(bool warning = false) override;
  void updateLineScale() override;
  virtual void updateChildren();
  virtual void updateIsLinearVelocityNull();
  virtual void updateIsAngularVelocityNull();

private:
  WbSolid &operator=(const WbSolid &);  // non copyable
  void init();

  void exportURDFShape(WbVrmlWriter &writer, const QString &geometry, const WbTransform *transform, bool correctOrientation,
                       const WbVector3 &offset) const;

  // list of finalized solids
  static QList<const WbSolid *> cSolids;

  // user accessible fields
  WbSFString *mContactMaterial;
  WbSFNode *mPhysics;
  WbMFNode *mImmersionProperties;
  WbSFDouble *mRadarCrossSection;
  WbMFColor *mRecognitionColors;

  // hidden fields
  WbSFVector3 *mLinearVelocity;
  WbSFVector3 *mAngularVelocity;

  bool mIsLinearVelocityNull;
  bool mIsAngularVelocityNull;

  // Clone
  WbNode *clone() const override { return new WbSolid(*this); }

  // Selection
  bool mSelected;

  // ODE
  dJointID mJoint;
  bool mUpdatedInStep;       // used to update Transform coordinated to setup ray collisions (based on pre-physics step values)
  bool mResetPhysicsInStep;  // used to completely reset physics when the solid is also moved in the same step
  void setGeomAndBodyPositions();
  void applyPhysicsTransform();
  void computePlaneParams(WbTransform *transform, WbVector3 &n, double &d) const;
  void resetJoints();  // reset joint to any linked solid to this one or to one of its descendants
  void setBodiesAndJointsToParents();
  void setJointChildrenWithReferencedEndpoint();
  void updateKinematicPlaceableGeomPosition(dGeomID g);
  bool resetJointPositions(bool allParents = false);
  void handleJerk() override;

  QVector<WbTransform *> mMovedChildren;
  void childrenJerk();

  void resetSingleSolidPhysics();

  // set position and orientation from physics integration (in contrast with supervisor or user setup)
  void inline setTransformFromOde(double tx, double ty, double tz, double rx, double ry, double rz, double angle);

  // Mass and density
  void addMassFromInsertedNode(WbBaseNode *node);
  bool checkMassAndDensity() const;
  double mAverageDensity;
  dMass *mMassAroundCoM;
  dMass *mOdeMass;         // around Solid frame center
  dMass *mReferenceMass;   // the mass of the solid when the density is uniformly set to 1000 kg/m^3
  bool mUseInertiaMatrix;  // indicates that the WbSolid uses the latest valid inertia matrix field for ODE physics computation
  WbVector3 mCenterOfMass;
  WbVector3 mScaledCenterOfMass;

  // ODE mass adjustments
  void createOdeMass(bool reset = true);
  // adjust the mass of the WbSolid after insertion or deletion in the boundingObject
  void adjustOdeMass(bool mergeMass = true);
  void addMass(WbBaseNode *node) { WbSolidUtilities::addMass(mReferenceMass, node, 1000); }
  // subtracts the mass of a deleted node in the boundingObject (called from WbGeometry)
  void subtractOdeMass(const dMass *mass, bool adjustSolidMass = true);
  void setDefaultMassSettings(bool applyCenterOfMassTranslation, bool warning = true);

  // Global center of mass variables
  WbVector3 mGlobalCenterOfMass;  // expressed in absolute (world) coordinates; if the total mass is zero and no CoM is
                                  // specified, it defaults to the Solid frame's center
  bool mGlobalCenterOfMassRepresentationIsEnabled;
  double mGlobalVolume;  // volume of the solid and all its descendants
  double mGlobalMass;    // mass of the solid and all its descendants

  // Immersion
  WbVector3 mCenterOfBuoyancy;
  bool mHasExtractedImmersions;
  bool mCenterOfBuoyancyRepresentationIsEnabled;
  void extractImmersions();
  void updateCenterOfBuoyancy();
  QVector<dImmersionGeom> mListOfImmersions;

  // Sleep flag
  bool mWasSleeping;
  virtual void updateSleepFlag();

  // Merger setup flag
  bool mMergerIsSet;
  bool mIsPermanentlyKinematic;

  bool mIsKinematic;

  bool mKinematicWarningPrinted;
  bool mHasDynamicSolidDescendant;

  // Contact points
  QVector<WbVector3> mListOfContactPoints;
  QVector<WbVector3> mGlobalListOfContactPoints;  // includes contacts of Solid descendants
  // defines the colliding Solid for each for the contact point in mGlobalListOfContactPoints
  QVector<const WbSolid *> mSolidPerContactPoints;
  bool mHasExtractedContactPoints;
  void extractContactPoints();  // populates both local and global lists

  // Support polygon variables

  // used to index the gravity basis vector when projecting contact points on a plan orthogonal to the gravity direction
  enum { X, Y, Z };
  WbSupportPolygonRepresentation *mSupportPolygonRepresentation;
  void deleteSupportPolygonRepresentation();
  mutable WbPolygon mSupportPolygon;
  mutable bool mSupportPolygonNeedsUpdate;
  double mY;  // minimum on the y-coordinate over all contact points
  bool mSupportPolygonRepresentationIsEnabled;

  // Relatives
  mutable bool mHasSearchedRobot;
  QVector<WbSolid *> mSolidChildren;  // either direct children, solid children of a WbGroup appearing in the children list,
                                      // solid endPoints of joint children or solid endPoint of a slot
  QVector<WbBasicJoint *> mJointChildren;
  QVector<WbPropeller *> mPropellerChildren;
  QList<WbBasicJoint *> mJointParents;  //  direct joint parent and joints whose endPoint is this solid
  mutable WbRobot *mRobot;

  // Update of children lists
  void collectSolidChildren(const WbGroup *group, bool connectSignals, QVector<WbSolid *> &solidChildren,
                            QVector<WbBasicJoint *> &jointChildren, QVector<WbPropeller *> &propellerChildren);
  void updateDynamicSolidDescendantFlag();

  void setOdeInertiaMatrix();
  void createOdeGeoms() override;
  // rescale all the ODE dGeoms lying inside the Bounding Object when the WbSolid's scale field has changed
  void applyToOdeScale() override;

  // WREN objects
  WrTransform *mCenterOfMassTransform;
  WrRenderable *mCenterOfMassRenderable;
  WrMaterial *mCenterOfMassMaterial;
  WrStaticMesh *mCenterOfMassMesh;

  WrTransform *mGlobalCenterOfMassTransform;
  WrRenderable *mGlobalCenterOfMassRenderable;
  WrMaterial *mGlobalCenterOfMassMaterial;
  WrStaticMesh *mGlobalCenterOfMassMesh;

  WrTransform *mCenterOfBuoyancyTransform;
  WrRenderable *mCenterOfBuoyancyRenderable;
  WrMaterial *mCenterOfBuoyancyMaterial;

  // Positions and orientations storage
  QMap<QString, WbVector3> mSavedTranslations;
  QMap<QString, WbRotation> mSavedRotations;
  WbHiddenKinematicParameters::HiddenKinematicParameters *mOriginalHiddenKinematicParameters;
  bool applyHiddenKinematicParameters(const WbHiddenKinematicParameters::HiddenKinematicParameters *hkp, bool backupPrevious);
  bool restoreHiddenKinematicParameters(const WbHiddenKinematicParameters::HiddenKinematicParametersMap &map,
                                        int &counter) override;
  bool resetHiddenKinematicParameters() override;

  void setGeomMatter(dGeomID g, WbBaseNode *node = NULL) override;

private slots:
  void updatePhysics();
  void updateRadarCrossSection();
  void updateRecognitionColors();
  void updateBoundingObject() override;
  void updateOdeMass();
  void applyToOdeMass();
  void updateOdeInertiaMatrix();
  void updateOdeCenterOfMass();
  void updateOdeDamping();
  void createOdeGeomFromInsertedGroupItem(WbBaseNode *node) override;
  void removeBoundingGeometry() override;
  void updateTopSolidGlobalMass() const;
  // connected to the physicsStepEnded() signal in computedContactPoints() and disconnected when called
  void resetContactPoints();
  void resetContactPointsAndSupportPolygon();  // connected to the physicsStepEnded() signal
  void resetImmersions();
  void refreshPhysicsRepresentation();             // redraws CoM, support polygon and CoB after physics properties change
  void refreshSupportPolygonRepresentation();      // redraws after each physics step
  void refreshGlobalCenterOfMassRepresentation();  // redraws the global CoM after each physics step
  void refreshCenterOfBuoyancyRepresentation();    // redraws the CoB after each physics step
  void onSimulationModeChanged();                  // deletes the support polygon representation if need be
  void displayWarning();
};

void inline WbSolid::setTransformFromOde(double tx, double ty, double tz, double rx, double ry, double rz, double angle) {
  WbTransform::setTranslationAndRotationFromOde(tx, ty, tz, rx, ry, rz, angle);
  WbTransform::updateTranslationAndRotation();
}

#endif
