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

#include "WbVisualBoundingSphere.hpp"

#include "WbBaseNode.hpp"
#include "WbBoundingSphere.hpp"
#include "WbPerspective.hpp"
#include "WbSphere.hpp"
#include "WbSysInfo.hpp"
#include "WbWorld.hpp"
#include "WbWrenOpenGlContext.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/scene.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

static bool gEnabled = false;

WbVisualBoundingSphere *WbVisualBoundingSphere::cInstance = NULL;

WbVisualBoundingSphere *WbVisualBoundingSphere::instance() {
  if (!cInstance)
    cInstance = new WbVisualBoundingSphere();
  return cInstance;
}

void WbVisualBoundingSphere::deleteInstance() {
  if (!cInstance)
    return;
  cInstance->deleteWrenObjects();
  delete cInstance;
  cInstance = NULL;
}

void WbVisualBoundingSphere::enable(bool enabled, const WbBaseNode *node) {
  gEnabled = enabled;
  if (node || cInstance)
    cInstance->show(node);
}

void WbVisualBoundingSphere::createSphere(const WbVector3 &center, float radius) {
  // The visual bounding sphere can be enabled from the optional rendering if WEBOTS_DEBUG is set.
  if (!mInitialized) {
    mWrenNode = wr_scene_get_root(wr_scene_get_instance());
    mWrenScaleTransform = wr_transform_new();
    wr_transform_attach_child(mWrenNode, WR_NODE(mWrenScaleTransform));
    mWrenRenderable = wr_renderable_new();
    mWrenMaterial = wr_phong_material_new();
    wr_material_set_default_program(mWrenMaterial, WbWrenShaders::lineSetShader());
    wr_renderable_set_cast_shadows(mWrenRenderable, false);
    wr_renderable_set_receive_shadows(mWrenRenderable, false);
    wr_renderable_set_drawing_mode(mWrenRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
    wr_renderable_set_material(mWrenRenderable, mWrenMaterial, NULL);
    mWrenEncodeDepthMaterial = wr_phong_material_new();
    wr_material_set_default_program(mWrenEncodeDepthMaterial, WbWrenShaders::encodeDepthShader());
    wr_renderable_set_material(mWrenRenderable, mWrenEncodeDepthMaterial, "encodeDepth");
    mWrenSegmentationMaterial = wr_phong_material_new();
    wr_material_set_default_program(mWrenSegmentationMaterial, WbWrenShaders::segmentationShader());
    wr_renderable_set_material(mWrenRenderable, mWrenSegmentationMaterial, "segmentation");
    mWrenMesh = wr_static_mesh_unit_sphere_new(16, false, true);
    wr_renderable_set_mesh(mWrenRenderable, WR_MESH(mWrenMesh));
    wr_transform_attach_child(mWrenScaleTransform, WR_NODE(mWrenRenderable));
    mInitialized = true;
  }

  const float scale[] = {radius, radius, radius};
  wr_transform_set_scale(mWrenScaleTransform, scale);
  const float position[] = {(float)center.x(), (float)center.y(), (float)center.z()};
  wr_transform_set_position(mWrenScaleTransform, position);
}

WrMaterial *mWrenMaterial;
WrMaterial *mWrenEncodeDepthMaterial;
WrMaterial *mWrenSegmentationMaterial;
WrStaticMesh *mWrenMesh;
WrRenderable *mWrenRenderable;

WbVisualBoundingSphere::WbVisualBoundingSphere() :
  QObject(),
  mInitialized(false),
  mWrenNode(NULL),
  mWrenScaleTransform(NULL),
  mWrenMaterial(NULL),
  mWrenEncodeDepthMaterial(NULL),
  mWrenSegmentationMaterial(NULL),
  mWrenMesh(NULL),
  mWrenRenderable(NULL) {
}

WbVisualBoundingSphere::~WbVisualBoundingSphere() {
  deleteWrenObjects();
}

void WbVisualBoundingSphere::show(const WbBaseNode *node) {
  // The visual bounding sphere can be enabled from the optional rendering if WEBOTS_DEBUG is set.
  if (!gEnabled) {
    if (mInitialized)
      deleteWrenObjects();
    return;
  }

  WbBoundingSphere *boundingSphere = node ? node->boundingSphere() : NULL;
  if (!boundingSphere) {
    if (mInitialized)
      wr_node_set_visible(WR_NODE(mWrenScaleTransform), false);
    return;
  }

  WbVector3 center;
  double radius;
  boundingSphere->computeSphereInGlobalCoordinates(center, radius);
  WbWrenOpenGlContext::makeWrenCurrent();
  createSphere(center, radius);
  wr_renderable_set_visibility_flags(mWrenRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  wr_node_set_visible(WR_NODE(mWrenScaleTransform), true);
  WbWrenOpenGlContext::doneWren();
}

void WbVisualBoundingSphere::deleteWrenObjects() {
  if (!mInitialized)
    return;

  WbWrenOpenGlContext::makeWrenCurrent();
  wr_static_mesh_delete(mWrenMesh);
  mWrenMesh = NULL;
  if (mWrenRenderable) {
    if (mWrenMaterial)
      wr_renderable_set_material(mWrenRenderable, NULL, NULL);
    wr_material_delete(mWrenMaterial);
    mWrenMaterial = NULL;
    wr_material_delete(mWrenEncodeDepthMaterial);
    mWrenEncodeDepthMaterial = NULL;
    wr_material_delete(mWrenSegmentationMaterial);
    mWrenSegmentationMaterial = NULL;
    wr_node_delete(WR_NODE(mWrenRenderable));
    mWrenRenderable = NULL;
  }
  wr_node_delete(WR_NODE(mWrenScaleTransform));
  mWrenScaleTransform = NULL;
  WbWrenOpenGlContext::doneWren();
  mInitialized = false;
}
