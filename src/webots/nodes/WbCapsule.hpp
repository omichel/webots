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

#ifndef WB_CAPSULE_HPP
#define WB_CAPSULE_HPP

#include "WbGeometry.hpp"
#include "WbSFDouble.hpp"

class WbCapsule : public WbGeometry {
  Q_OBJECT

public:
  // constructors and destructor
  explicit WbCapsule(WbTokenizer *tokenizer = NULL);
  WbCapsule(const WbCapsule &other);
  explicit WbCapsule(const WbNode &other);
  virtual ~WbCapsule();

  // reimplemented public functions
  int nodeType() const override { return WB_NODE_CAPSULE; }
  void postFinalize() override;
  void createWrenObjects() override;
  dGeomID createOdeGeom(dSpaceID space) override;
  void createResizeManipulator() override;
  bool isAValidBoundingObject(bool checkOde = false, bool warning = true) const override;
  bool isSuitableForInsertionInBoundingObject(bool warning = false) const override;
  void write(WbVrmlWriter &writer) const override;
  void rescale(const WbVector3 &scale) override;

  // field accessors
  double radius() const { return mRadius->value(); }
  double scaledRadius() const;
  double height() const { return mHeight->value(); }
  double scaledHeight() const;

  // field setters
  void setRadius(double r) { mRadius->setValue(r); }
  void setHeight(double h) { mHeight->setValue(h); }

  // ray tracing
  void recomputeBoundingSphere() const override;
  bool pickUVCoordinate(WbVector2 &uv, const WbRay &ray, int textureCoordSet = 0) const override;
  double computeDistance(const WbRay &ray) const override;

  // friction
  WbVector3 computeFrictionDirection(const WbVector3 &normal) const override;

  // resize manipulator
  void setResizeManipulatorDimensions() override;

protected:
  bool areSizeFieldsVisibleAndNotRegenerator() const override;
  void exportNodeFields(WbVrmlWriter &writer) const override;

private:
  // user accessible fields
  WbSFBool *mBottom;
  WbSFDouble *mRadius;
  WbSFDouble *mHeight;
  WbSFBool *mSide;
  WbSFBool *mTop;
  WbSFInt *mSubdivision;

  bool sanitizeFields();

  WbCapsule &operator=(const WbCapsule &);  // non copyable
  WbNode *clone() const override { return new WbCapsule(*this); }
  void init();

  // WREN
  void buildWrenMesh();

  // ODE
  void applyToOdeData(bool correctSolidMass = true) override;

  // ray tracing
  double computeLocalCollisionPoint(WbVector3 &point,
                                    const WbRay &ray) const;  // compute the collison point and return the distance

private slots:
  void updateBottom();
  void updateRadius();
  void updateHeight();
  void updateSide();
  void updateTop();
  void updateSubdivision();
  void updateLineScale();
};

#endif
