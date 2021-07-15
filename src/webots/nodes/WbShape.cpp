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

#include "WbShape.hpp"

#include "WbAppearance.hpp"
#include "WbBoundingSphere.hpp"
#include "WbImageTexture.hpp"
#include "WbMaterial.hpp"
#include "WbNodeUtilities.hpp"
#include "WbOdeContext.hpp"
#include "WbPaintTexture.hpp"
#include "WbPbrAppearance.hpp"
#include "WbPreferences.hpp"
#include "WbRay.hpp"
#include "WbRgb.hpp"
#include "WbSFBool.hpp"
#include "WbSFNode.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/material.h>

#include <ode/ode.h>

void WbShape::init() {
  mAppearance = findSFNode("appearance");
  mGeometry = findSFNode("geometry");
  mCastShadows = findSFBool("castShadows");
  mIsPickable = findSFBool("isPickable");

  mWrenMaterial = NULL;
}

WbShape::WbShape(WbTokenizer *tokenizer) : WbBaseNode("Shape", tokenizer) {
  init();
}

WbShape::WbShape(const WbShape &other) : WbBaseNode(other) {
  init();
}

WbShape::WbShape(const WbNode &other) : WbBaseNode(other) {
  init();
}

WbShape::~WbShape() {
  if (mWrenMaterial)
    wr_material_delete(mWrenMaterial);
}

void WbShape::downloadAssets() {
  WbBaseNode::downloadAssets();
  if (abstractAppearance())
    abstractAppearance()->downloadAssets();
  if (geometry())
    geometry()->downloadAssets();
}

void WbShape::preFinalize() {
  WbBaseNode::preFinalize();

  // handle both kinds of appearance nodes
  WbBaseNode *baseNode = dynamic_cast<WbBaseNode *>(mAppearance->value());
  if (baseNode)
    baseNode->preFinalize();

  if (geometry())
    geometry()->preFinalize();

  updateAppearance();
  updateGeometry();
}

void WbShape::postFinalize() {
  WbBaseNode::postFinalize();

  // handle both kinds of appearance nodes
  WbBaseNode *baseNode = dynamic_cast<WbBaseNode *>(mAppearance->value());
  if (baseNode)
    baseNode->postFinalize();

  if (geometry())
    geometry()->postFinalize();

  connect(mAppearance, &WbSFNode::changed, this, &WbShape::updateAppearance);
  connect(mGeometry, &WbSFNode::changed, this, &WbShape::updateGeometry);
  if (!isInBoundingObject()) {
    connect(mCastShadows, &WbSFBool::changed, this, &WbShape::updateCastShadows);
    updateCastShadows();
    connect(mIsPickable, &WbSFBool::changed, this, &WbShape::updateIsPickable);
    updateIsPickable();
  }
  if (geometry())
    connect(geometry(), &WbGeometry::changed, this, &WbShape::updateGeometryMaterial, Qt::UniqueConnection);

  connect(WbPreferences::instance(), &WbPreferences::changedByUser, this, &WbShape::updateAppearance);
}

void WbShape::reset(const QString &id) {
  WbBaseNode::reset(id);

  // handle both kinds of appearance nodes
  WbBaseNode *baseNode = dynamic_cast<WbBaseNode *>(mAppearance->value());
  if (baseNode)
    baseNode->reset(id);

  WbNode *const geometry = mGeometry->value();
  if (geometry)
    geometry->reset(id);
}

WbAppearance *WbShape::appearance() const {
  return dynamic_cast<WbAppearance *>(mAppearance->value());
}

WbPbrAppearance *WbShape::pbrAppearance() const {
  return dynamic_cast<WbPbrAppearance *>(mAppearance->value());
}

WbAbstractAppearance *WbShape::abstractAppearance() const {
  return dynamic_cast<WbAbstractAppearance *>(mAppearance->value());
}

void WbShape::propagateSelection(bool selected) {
  WbGeometry *const g = geometry();
  if (g)
    geometry()->propagateSelection(selected);
}

void WbShape::updateCollisionMaterial(bool triggerChange, bool onSelection) {  // for WbShapes lying into a bounding object only
  WbGeometry *const g = geometry();
  if (g)
    geometry()->updateCollisionMaterial(triggerChange, onSelection);
}

void WbShape::setScaleNeedUpdate() {  // for any WbShape
  WbGeometry *const g = geometry();
  if (g)
    g->setScaleNeedUpdate();
}

void WbShape::setSleepMaterial() {
  WbGeometry *const g = geometry();
  if (g)
    geometry()->setSleepMaterial();
}

void WbShape::updateAppearance() {
  if (appearance())
    connect(appearance(), &WbAppearance::changed, this, &WbShape::updateAppearance, Qt::UniqueConnection);
  else if (pbrAppearance())
    connect(pbrAppearance(), &WbPbrAppearance::changed, this, &WbShape::updateAppearance, Qt::UniqueConnection);

  if (areWrenObjectsInitialized())
    applyMaterialToGeometry();
  else if (!isInBoundingObject())
    emit wrenMaterialChanged();
}

void WbShape::updateGeometry() {
  if (isInBoundingObject())
    return;

  const WbGeometry *const g = geometry();
  if (g) {
    connect(g, &WbGeometry::wrenObjectsCreated, this, &WbShape::updateGeometryMaterial, Qt::UniqueConnection);
    connect(g, &WbGeometry::wrenObjectsCreated, this, &WbShape::updateIsPickable, Qt::UniqueConnection);
    connect(g, &WbGeometry::changed, this, &WbShape::updateGeometryMaterial, Qt::UniqueConnection);
    if (isPostFinalizedCalled()) {
      if (g->isPostFinalizedCalled())
        WbBoundingSphere::addSubBoundingSphereToParentNode(this);
      else
        connect(g, &WbBaseNode::finalizationCompleted, this, &WbShape::updateBoundingSphere, Qt::UniqueConnection);
    }
  }
}

void WbShape::updateBoundingSphere(WbBaseNode *subNode) {
  disconnect(subNode, &WbBaseNode::finalizationCompleted, this, &WbShape::updateBoundingSphere);
  WbBoundingSphere::addSubBoundingSphereToParentNode(this);
}

void WbShape::updateCastShadows() {
  assert(!isInBoundingObject());

  WbGeometry *const g = geometry();
  if (g) {
    g->computeCastShadows(mCastShadows->value());
    emit castShadowsChanged();
  }
}

void WbShape::updateIsPickable() {
  assert(!isInBoundingObject());
  WbGeometry *const g = geometry();
  if (g)
    g->setPickable(mIsPickable->value());
}

void WbShape::setAppearance(WbAppearance *appearance) {
  mAppearance->removeValue();
  mAppearance->setValue(appearance);
}

void WbShape::setPbrAppearance(WbPbrAppearance *appearance) {
  mAppearance->removeValue();
  mAppearance->setValue(appearance);
}

void WbShape::setGeometry(WbGeometry *geometry) {
  mGeometry->removeValue();
  mGeometry->setValue(geometry);
}

void WbShape::updateGeometryMaterial() {
  if (areWrenObjectsInitialized())
    applyMaterialToGeometry();
}

bool WbShape::isCastShadowsEnabled() const {
  return mCastShadows->value();
}

WbBoundingSphere *WbShape::boundingSphere() const {
  const WbGeometry *const g = geometry();
  if (g)
    return g->boundingSphere();

  return NULL;
}

void WbShape::createWrenObjects() {
  WbBaseNode::createWrenObjects();

  // handle both kinds of appearance nodes
  WbBaseNode *baseNode = dynamic_cast<WbBaseNode *>(mAppearance->value());
  if (baseNode)
    baseNode->createWrenObjects();

  WbGeometry *const g = geometry();
  if (g)
    g->createWrenObjects();
}

void WbShape::applyMaterialToGeometry() {
  WbGeometry *const g = geometry();
  if (!mWrenMaterial)
    createWrenMaterial(WR_MATERIAL_PHONG);

  if (g) {
    if (appearance()) {
      if (appearance()->areWrenObjectsInitialized()) {
        mWrenMaterial = appearance()->modifyWrenMaterial(mWrenMaterial);
        if (appearance()->material() && appearance()->material()->transparency() == 1)
          g->setTransparent(true);
        else
          g->setTransparent(false);
      } else {
        mWrenMaterial = WbAppearance::fillWrenDefaultMaterial(mWrenMaterial);
        // We need to call setTransparent for default appearance in case a previous transparent appearance was existing and
        // replaced by the default one.
        g->setTransparent(false);
      }
    } else if (pbrAppearance() && g->nodeType() != WB_NODE_POINT_SET) {
      createWrenMaterial(WR_MATERIAL_PBR);
      if (pbrAppearance()->areWrenObjectsInitialized()) {
        mWrenMaterial = pbrAppearance()->modifyWrenMaterial(mWrenMaterial);
        if (pbrAppearance()->transparency() == 1)
          g->setTransparent(true);
        else
          g->setTransparent(false);
      }
    } else {
      mWrenMaterial = WbAppearance::fillWrenDefaultMaterial(mWrenMaterial);
      // We need to call setTransparent for default appearance in case a previous transparent appearance was existing and
      // replaced by the default one.
      g->setTransparent(false);
    }

    if (!g->isInBoundingObject())
      g->setWrenMaterial(mWrenMaterial, mCastShadows->value());

    emit wrenMaterialChanged();
  }
}

// ray cast to a shape: pick the collided color
// using uv mapping, paint color and diffuse color
// could be improved by computing the exact openGL computing including lighting
void WbShape::pickColor(const WbRay &ray, WbRgb &pickedColor, double *roughness, double *occlusion) const {
  WbAppearance *const app = appearance();
  WbPbrAppearance *const pbrApp = pbrAppearance();
  WbGeometry *const geom = geometry();

  WbRgb diffuseColor(1.0f, 1.0f, 1.0f);
  WbRgb textureColor(1.0f, 1.0f, 1.0f);
  WbRgb paintColor(1.0f, 1.0f, 1.0f);
  pickedColor = WbRgb(1.0f, 1.0f, 1.0f);  // default value
  WbVector2 uv(-1, -1);

  if (roughness)
    *roughness = 0.0;
  if (occlusion)
    *occlusion = 0.0;

  if (geom) {
    float paintContribution = 0.0f;

    WbPaintTexture *paintTexture = WbPaintTexture::findPaintTexture(this);
    if (paintTexture) {
      const bool success = geom->pickUVCoordinate(uv, ray, 0);
      if (!success) {
        if (app) {
          pickedColor = app->diffuseColor();

          // apply transparency factor
          if (app->material()) {
            const float alpha = app->material()->transparency();
            pickedColor.setRed(pickedColor.red() * alpha);
            pickedColor.setGreen(pickedColor.green() * alpha);
            pickedColor.setBlue(pickedColor.blue() * alpha);
          }
        } else if (pbrApp) {
          pickedColor = pbrApp->baseColor();

          // apply transparency factor
          const float alpha = pbrApp->transparency();
          pickedColor.setRed(pickedColor.red() * alpha);
          pickedColor.setGreen(pickedColor.green() * alpha);
          pickedColor.setBlue(pickedColor.blue() * alpha);

          if (roughness)
            *roughness = pbrApp->roughness();
        }

        return;
      }
      // retrieve the corresponding color in the paint texture
      paintTexture->pickColor(uv, paintColor, &paintContribution);
    }

    if (app) {
      // diffuse color
      diffuseColor = app->diffuseColor();

      // apply transparency factor
      if (app->material()) {
        const double alpha = 1.0 - app->material()->transparency();
        diffuseColor.setRed(diffuseColor.red() * alpha);
        diffuseColor.setGreen(diffuseColor.green() * alpha);
        diffuseColor.setBlue(diffuseColor.blue() * alpha);
      }

      // texture color
      if (app->isTextureLoaded()) {
        if (uv.x() == -1) {
          bool success = geom->pickUVCoordinate(uv, ray, 0);
          if (!success) {
            pickedColor = diffuseColor;
            return;
          }
        }

        // retrieve the corresponding color in the texture
        app->pickColorInTexture(uv, textureColor);
      }

    } else if (pbrApp) {
      // diffuse color
      diffuseColor = pbrApp->baseColor();

      // apply transparency factor
      const double alpha = 1.0 - pbrApp->transparency();
      diffuseColor.setRed(diffuseColor.red() * alpha);
      diffuseColor.setGreen(diffuseColor.green() * alpha);
      diffuseColor.setBlue(diffuseColor.blue() * alpha);

      if (roughness)
        *roughness = pbrApp->roughness();

      // texture color
      if (pbrApp->isBaseColorTextureLoaded()) {
        if (uv.x() == -1) {
          bool success = geom->pickUVCoordinate(uv, ray, 0);
          if (!success) {
            pickedColor = diffuseColor;
            return;
          }
        }

        // retrieve the corresponding color in the texture
        pbrApp->pickColorInBaseColorTexture(textureColor, uv);
        if (roughness && pbrApp->isRoughnessTextureLoaded())
          pbrApp->pickRoughnessInTexture(roughness, uv);
        if (occlusion && pbrApp->isOcclusionTextureLoaded())
          pbrApp->pickOcclusionInTexture(occlusion, uv);
      }
    } else if (paintTexture) {
      pickedColor = paintColor;
      return;
    } else
      return;  // default value

    // combine colors
    pickedColor.setRed((1.0f - paintContribution) * diffuseColor.red() * textureColor.red() +
                       paintContribution * paintColor.red());
    pickedColor.setGreen((1.0f - paintContribution) * diffuseColor.green() * textureColor.green() +
                         paintContribution * paintColor.green());
    pickedColor.setBlue((1.0f - paintContribution) * diffuseColor.blue() * textureColor.blue() +
                        paintContribution * paintColor.blue());
  }
}

void WbShape::createWrenMaterial(int type) {
  const float defaultColor[] = {1.0f, 1.0f, 1.0f};
  if (mWrenMaterial)
    wr_material_delete(mWrenMaterial);

  if (type == WR_MATERIAL_PHONG) {
    mWrenMaterial = wr_phong_material_new();
    wr_phong_material_set_color(mWrenMaterial, defaultColor);
    wr_material_set_default_program(mWrenMaterial, WbWrenShaders::defaultShader());
  } else {
    mWrenMaterial = wr_pbr_material_new();
    wr_pbr_material_set_base_color(mWrenMaterial, defaultColor);
    wr_material_set_default_program(mWrenMaterial, WbWrenShaders::pbrShader());
  }
}

///////////////////////////////////////////////////////////
//  ODE related methods for WbShapes in boundingObjects  //
///////////////////////////////////////////////////////////

void WbShape::connectGeometryField() const {
  connect(mGeometry, &WbSFNode::changed, this, &WbShape::createOdeGeom, Qt::UniqueConnection);
}

void WbShape::disconnectGeometryField() const {
  disconnect(mGeometry, &WbSFNode::changed, this, &WbShape::createOdeGeom);
}

void WbShape::createOdeGeom() {
  const WbGeometry *const g = geometry();

  if (g == NULL) {
    parsingInfo(tr("Please specify 'geometry' field (of Shape placed in 'boundingObject')."));
    return;
  }

  emit geometryInShapeInserted();
}

bool WbShape::isSuitableForInsertionInBoundingObject(bool warning) const {
  const WbGeometry *const g = geometry();
  return g ? g->isSuitableForInsertionInBoundingObject(warning) : true;
}

bool WbShape::isAValidBoundingObject(bool checkOde, bool warning) const {
  const WbGeometry *const g = geometry();
  return g && g->isAValidBoundingObject(checkOde, warning);
}

////////////
// Export //
////////////

bool WbShape::exportNodeHeader(WbVrmlWriter &writer) const {
  if (writer.isX3d()) {
    writer << "<" << x3dName() << " id=\'n" << QString::number(uniqueId()) << "\'";
    if (isInvisibleNode())
      writer << " render=\'false\'";

    if (isUseNode() && defNode()) {
      writer << " USE=\'" + QString::number(defNode()->uniqueId()) + "\'></" + x3dName() + ">";
      return true;
    }

    if (!mIsPickable->value())
      writer << " isPickable='false'";
    if (mCastShadows->value())
      writer << " castShadows='true'";

    return false;
  } else
    return WbBaseNode::exportNodeHeader(writer);
}

void WbShape::exportBoundingObjectToX3D(WbVrmlWriter &writer) const {
  assert(writer.isX3d());

  writer << "<Shape>";

  geometry()->exportBoundingObjectToX3D(writer);

  writer << "</Shape>";
}
