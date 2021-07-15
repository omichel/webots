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

#ifndef WB_GEOMETRY_HPP
#define WB_GEOMETRY_HPP

//
// Description: abstract base class for all geometry primitives
// Inherited by:
//   WbBox, WbSphere, WbPlane, WbCapsule, WbCylinder, WbCone,
//   WbElevationGrid, WbIndexedFaceSet, WbIndexedLineSet and WbMesh
//

#include "WbBaseNode.hpp"
#include "WbMatrix3.hpp"
#include "WbOdeTypes.hpp"
#include "WbVector2.hpp"

class WbBoundingSphere;
class WbMatrix4;
class WbMatter;
class WbWrenMeshBuffers;
class WbRay;
class WbRgb;
class WbWrenAbstractResizeManipulator;
struct dMass;

struct WrMaterial;
struct WrStaticMesh;
struct WrRenderable;
struct WrTransform;

class WbGeometry : public WbBaseNode {
  Q_OBJECT

public:
  // Destructor
  virtual ~WbGeometry();

  // Reimplemented public functions
  void postFinalize() override;
  void createOdeObjects() override;
  void updateContextDependentObjects() override;
  bool isAValidBoundingObject(bool checkOde = false, bool warning = true) const override;
  void propagateSelection(bool selected) override;

  virtual bool hasDefaultMaterial() { return false; }
  void computeCastShadows(bool enabled);
  void updateCollisionMaterial(bool triggerChange = false, bool onSelection = false) override;
  void setSleepMaterial() override;
  virtual WbWrenMeshBuffers *createMeshBuffers(int verticesCount, int indicesCount) const;
  virtual void buildGeomIntoBuffers(WbWrenMeshBuffers *buffers, const WbMatrix4 &m, bool generateUserTexCoords = true) const {}
  void setPickable(bool pickable);

  // WREN
  WrStaticMesh *wrenMesh() const { return mWrenMesh; }
  virtual void computeWrenRenderable();
  virtual void deleteWrenRenderable();
  virtual void setWrenMaterial(WrMaterial *material, bool castShadows);
  void destroyWrenObjects();

  // Create ODE dGeom (for a WbGeometry lying into a boundingObject)
  virtual dGeomID createOdeGeom(dSpaceID space);
  void destroyOdeObjects();
  dGeomID odeGeom() const { return mOdeGeom; }
  void setOdeMass(const dMass *mass);
  const dMass *odeMass() const { return mOdeMass; }
  virtual void setOdePosition(const WbVector3 &translation);
  virtual void setOdeRotation(const WbMatrix3 &rotation);

  void setOdeData(dGeomID geom, WbMatter *matterAncestor);  // stores an ODE dGeom if the WbGeometry lies into a boundingObject
  virtual void applyToOdeData(bool correctSolidMass = true) {}  // Resize the ODE dGeom stored in a bounding WbGeometry

  // Ray tracing
  WbBoundingSphere *boundingSphere() const override { return mBoundingSphere; }
  virtual void recomputeBoundingSphere() const = 0;
  virtual bool pickUVCoordinate(WbVector2 &uv, const WbRay &ray, int textureCoordSet = 0) const = 0;
  virtual double computeDistance(const WbRay &ray) const = 0;

  // friction
  virtual WbVector3 computeFrictionDirection(const WbVector3 &normal) const = 0;

  // Non-recursive texture mapping
  virtual WbVector2 nonRecursiveTextureSizeFactor() const { return WbVector2(1, 1); }

  virtual void rescale(const WbVector3 &scale) = 0;
  const WbVector3 absoluteScale() const;
  WbVector3 absolutePosition() const;

  // ODE collision info
  void setColliding();

  // Position and orientation matrix of the upper transform
  WbMatrix4 matrix() const;

  // Scale handles constraints
  int constraintType() const;

  // resize manipulator
  bool hasResizeManipulator() const override { return areSizeFieldsVisibleAndNotRegenerator(); }
  WbWrenAbstractResizeManipulator *resizeManipulator();
  void attachResizeManipulator() override;
  void detachResizeManipulator() const override;
  void updateResizeHandlesSize() override;
  virtual void createResizeManipulatorIfNeeded();
  virtual void setResizeManipulatorDimensions() {}
  void setUniformConstraintForResizeHandles(bool enabled) override;

  // export
  void exportBoundingObjectToX3D(WbVrmlWriter &writer) const override;

  static int maxIndexNumberToCastShadows();
  int triangleCount() const;

  // visibility
  void setTransparent(bool isTransparent);

signals:
  void changed();
  void wrenObjectsCreated();
  void boundingGeometryRemoved();

public slots:
  void showResizeManipulator(bool enabled) override;

protected:
  bool exportNodeHeader(WbVrmlWriter &writer) const override;

  static const float LINE_SCALE_FACTOR;

  // All constructors are reserved for derived classes only
  WbGeometry(const WbGeometry &other);
  WbGeometry(const WbNode &other);
  WbGeometry(const QString &modelName, WbTokenizer *tokenizer);

  // for bounding object representation use a subdivision >= MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION
  // so that it is clear for the user that the ODE object is a real rounded shape and not an approximation as the graphical mesh
  const int MIN_BOUNDING_OBJECT_CIRCLE_SUBDIVISION = 16;

  // Wren
  WrMaterial *mWrenMaterial;
  WrMaterial *mWrenEncodeDepthMaterial;
  WrMaterial *mWrenSegmentationMaterial;
  WrStaticMesh *mWrenMesh;
  WrRenderable *mWrenRenderable;
  WrTransform *mWrenScaleTransform;

  // Selection
  bool isSelected();
  bool isPickable() { return mPickable; }

  // query flags
  virtual bool isShadedGeometryPickable() { return true; }

  // check if the fields that resize the geometry are visible in the scene tree
  virtual bool areSizeFieldsVisibleAndNotRegenerator() const { return false; }

  // ODE objects for a WbGeometry lying into a boundingObject
  // Scaling
  dGeomID mOdeGeom;          // stores a pointer on the ODE dGeom object when the WbGeometry lies into a boundingObject
  bool mIs90DegreesRotated;  // rotate Capsule and Cylinder by 90 degrees: ODE to VRML rotation
  WbVector3 mLocalOdeGeomOffsetPosition;
  dMass *mOdeMass;        // needed to correct the WbSolid parent mass after the destruction of a bounding WbGeometry
  void applyToOdeMass();  // modifies the ODE dMass when the dimensions change
  WbBaseNode *transformedGeometry();
  // Fluid
  virtual void checkFluidBoundingObjectOrientation();

  // Ray tracing
  mutable WbBoundingSphere *mBoundingSphere;

  // Resize handles
  WbWrenAbstractResizeManipulator *mResizeManipulator;  // Set of handles allowing resize by dragging the mouse
  bool mResizeManipulatorInitialized;
  int mResizeConstraint;
  void checkForResizeManipulator();  // if needed create the resize manipulator according to the node location in the scene tree

private:
  WbGeometry &operator=(const WbGeometry &);  // non copyable
  // Only derived classes can be cloned
  WbNode *clone() const override = 0;

  void init();

  void applyVisibilityFlagToWren(bool selected);
  void setSegmentationColor(const WbRgb &color);
  virtual void createResizeManipulator() {}

  // ODE info
  double mCollisionTime;          // milliseconds
  double mPreviousCollisionTime;  // milliseconds

  WbVector3 mOdeOffsetTranslation;
  WbVector3 mOdePositionSet;
  WbMatrix3 mOdeOffsetRotation;

  bool mPickable;
  bool mIsTransparent;

private slots:
  virtual void updateBoundingObjectVisibility(int optionalRendering);
};

#endif
