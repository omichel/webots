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

#include "WbPen.hpp"

#include "WbFieldChecker.hpp"
#include "WbMatrix3.hpp"
#include "WbNodeUtilities.hpp"
#include "WbPaintTexture.hpp"
#include "WbRay.hpp"
#include "WbSFColor.hpp"
#include "WbSFDouble.hpp"
#include "WbShape.hpp"
#include "WbSimulationState.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/config.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include "../../controller/c/messages.h"

#include <QtCore/QDataStream>
#include <cassert>

void WbPen::init() {
  mInkColor = findSFColor("inkColor");
  mInkDensity = findSFDouble("inkDensity");
  mLeadSize = findSFDouble("leadSize");
  mWrite = findSFBool("write");
  mMaxDistance = findSFDouble("maxDistance");

  mLastPaintTexture = NULL;
  WbSimulationState::instance()->subscribeToRayTracing();

  mTransform = NULL;
  mRenderable = NULL;
  mMaterial = NULL;
  mMesh = NULL;
}

WbPen::WbPen(WbTokenizer *tokenizer) : WbSolidDevice("Pen", tokenizer) {
  init();
}

WbPen::WbPen(const WbPen &other) : WbSolidDevice(other) {
  init();
}

WbPen::WbPen(const WbNode &other) : WbSolidDevice(other) {
  init();
}

WbPen::~WbPen() {
  WbSimulationState::instance()->unsubscribeToRayTracing();

  if (areWrenObjectsInitialized()) {
    wr_node_delete(WR_NODE(mTransform));
    wr_node_delete(WR_NODE(mRenderable));
    wr_static_mesh_delete(mMesh);
    wr_material_delete(mMaterial);
  }
}

void WbPen::preFinalize() {
  WbSolidDevice::preFinalize();

  WbFieldChecker::clampDoubleToRangeWithIncludedBounds(this, mInkDensity, 0.0, 1.0);
}

void WbPen::handleMessage(QDataStream &stream) {
  unsigned char command;
  stream >> command;

  switch (command) {
    case C_PEN_WRITE:
      mWrite->setValue(true);
      return;
    case C_PEN_DONT_WRITE:
      mWrite->setValue(false);
      return;
    case C_PEN_SET_INK_COLOR: {
      unsigned char r, g, b;
      stream >> r >> g >> b;
      mInkColor->setValue(r / 255.0f, g / 255.0f, b / 255.0f);
      double density;
      stream >> density;
      mInkDensity->setValue(density);
      WbFieldChecker::clampDoubleToRangeWithIncludedBounds(this, mInkDensity, 0.0, 1.0);
      return;
    }
    default:
      assert(0);
  }
}

void WbPen::prePhysicsStep(double ms) {
  WbSolidDevice::prePhysicsStep(ms);

  double maxDistance = mMaxDistance->value();
  if (maxDistance <= 0.0)
    maxDistance = std::numeric_limits<double>::infinity();

  if (mWrite->isTrue()) {
    // find shape/texture that intersects the ray
    const WbMatrix4 &m = matrix();
    const WbVector3 globalDirection = m.sub3x3MatrixDot(WbVector3(0, -1, 0));
    const WbRay ray(m.translation(), globalDirection);
    double distance;
    const WbShape *shape = WbNodeUtilities::findIntersectingShape(ray, maxDistance, distance);

    if (shape && WbPaintTexture::isPaintable(shape)) {
      if (!mLastPaintTexture || shape != mLastPaintTexture->shape())
        mLastPaintTexture = WbPaintTexture::paintTexture(shape);

      if (mLastPaintTexture)
        mLastPaintTexture->paint(ray, mLeadSize->value(), mInkColor->value(), mInkDensity->value());
    }
  }
}

void WbPen::createWrenObjects() {
  WbSolidDevice::createWrenObjects();

  const float coords[6] = {0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f};
  mTransform = wr_transform_new();
  mRenderable = wr_renderable_new();
  mMaterial = wr_phong_material_new();
  mMesh = wr_static_mesh_line_set_new(2, coords, NULL);

  const float color[3] = {0.5f, 0.5f, 0.5f};
  wr_phong_material_set_color(mMaterial, color);
  wr_material_set_default_program(mMaterial, WbWrenShaders::lineSetShader());

  wr_renderable_set_drawing_mode(mRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_mesh(mRenderable, WR_MESH(mMesh));
  wr_renderable_set_material(mRenderable, mMaterial, NULL);
  wr_renderable_set_cast_shadows(mRenderable, false);
  wr_renderable_set_receive_shadows(mRenderable, false);
  wr_renderable_set_visibility_flags(mRenderable, WbWrenRenderingContext::VF_PEN_RAYS);

  wr_transform_attach_child(mTransform, WR_NODE(mRenderable));
  wr_transform_attach_child(wrenNode(), WR_NODE(mTransform));

  applyOptionalRenderingToWren();

  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::optionalRenderingChanged, this,
          &WbPen::updateOptionalRendering);
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this,
          &WbPen::applyOptionalRenderingToWren);
  connect(mWrite, &WbSFBool::changed, this, &WbPen::applyOptionalRenderingToWren);
}

void WbPen::reset(const QString &id) {
  WbSolid::reset(id);
  WbPaintTexture::clearAllTextures();
}

void WbPen::updateOptionalRendering(int option) {
  if (option == WbWrenRenderingContext::VF_PEN_RAYS)
    wr_node_set_visible(WR_NODE(mTransform), WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(option));
}

void WbPen::applyOptionalRenderingToWren() {
  if (!areWrenObjectsInitialized())
    return;

  const float lineScale = wr_config_get_line_scale();
  const float scale[3] = {lineScale, lineScale, lineScale};
  wr_transform_set_scale(mTransform, scale);

  const float enabledColor[3] = {0.5f, 0.0f, 1.0f};   // violet
  const float disabledColor[3] = {0.5f, 0.5f, 0.5f};  // grey
  if (mWrite->value())
    wr_phong_material_set_color(mMaterial, enabledColor);
  else
    wr_phong_material_set_color(mMaterial, disabledColor);
}
