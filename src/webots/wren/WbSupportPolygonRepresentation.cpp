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

#include "WbSupportPolygonRepresentation.hpp"

#include "WbMathsUtilities.hpp"
#include "WbPolygon.hpp"
#include "WbRotation.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/config.h>
#include <wren/dynamic_mesh.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/scene.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

// Colors used by support polygon representation in RGBA format
// note: alpha = 1.0f -> full transparency
static const float STABLE_POLYGON_COLOR[4] = {0.6f, 0.9f, 0.6f, 0.5f};
static const float STABLE_POLYGON_OUTLINE_COLOR[4] = {0.2f, 1.0f, 0.2f, 0.5f};
static const float STABLE_CENTER_OF_MASS_COLOR[4] = {0.5f, 1.0f, 0.5f, 0.0f};
static const float UNSTABLE_POLYGON_COLOR[4] = {0.9f, 0.6f, 0.6f, 0.5f};
static const float UNSTABLE_POLYGON_OUTLINE_COLOR[4] = {1.0f, 0.2f, 0.1f, 0.5f};
static const float UNSTABLE_CENTER_OF_MASS_COLOR[4] = {1.0f, 0.2f, 0.2f, 0.0f};

WbSupportPolygonRepresentation::WbSupportPolygonRepresentation() {
  mTransform = wr_transform_new();
  mCenterOfMassTransform = wr_transform_new();

  mPolygonMesh = wr_dynamic_mesh_new(false, false, false);
  mPolygonOutlineMesh = wr_dynamic_mesh_new(false, false, false);

  const float vertices[12] = {-1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f};

  mCenterOfMassMesh = wr_static_mesh_line_set_new(4, vertices, NULL);

  mPolygonMaterial = wr_phong_material_new();
  wr_material_set_default_program(mPolygonMaterial, WbWrenShaders::simpleShader());
  wr_phong_material_set_color(mPolygonMaterial, STABLE_POLYGON_COLOR);
  wr_phong_material_set_transparency(mPolygonMaterial, STABLE_POLYGON_COLOR[3]);

  mPolygonOutlineMaterial = wr_phong_material_new();
  wr_material_set_default_program(mPolygonOutlineMaterial, WbWrenShaders::lineSetShader());
  wr_phong_material_set_color(mPolygonOutlineMaterial, STABLE_POLYGON_OUTLINE_COLOR);
  wr_phong_material_set_transparency(mPolygonOutlineMaterial, STABLE_POLYGON_OUTLINE_COLOR[3]);

  mCenterOfMassMaterial = wr_phong_material_new();
  wr_material_set_default_program(mCenterOfMassMaterial, WbWrenShaders::lineSetShader());
  wr_phong_material_set_color(mCenterOfMassMaterial, STABLE_CENTER_OF_MASS_COLOR);
  wr_phong_material_set_transparency(mCenterOfMassMaterial, STABLE_CENTER_OF_MASS_COLOR[3]);

  mPolygonRenderable = wr_renderable_new();
  wr_renderable_set_mesh(mPolygonRenderable, WR_MESH(mPolygonMesh));
  wr_renderable_set_drawing_order(mPolygonRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);
  wr_renderable_set_material(mPolygonRenderable, mPolygonMaterial, NULL);
  wr_renderable_set_visibility_flags(mPolygonRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  wr_renderable_set_face_culling(mPolygonRenderable, false);
  wr_renderable_set_drawing_mode(mPolygonRenderable, WR_RENDERABLE_DRAWING_MODE_TRIANGLE_FAN);

  mPolygonOutlineRenderable = wr_renderable_new();
  wr_renderable_set_mesh(mPolygonOutlineRenderable, WR_MESH(mPolygonOutlineMesh));
  wr_renderable_set_drawing_order(mPolygonOutlineRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);
  wr_renderable_set_drawing_mode(mPolygonOutlineRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_material(mPolygonOutlineRenderable, mPolygonOutlineMaterial, NULL);
  wr_renderable_set_visibility_flags(mPolygonRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);

  mCenterOfMassRenderable = wr_renderable_new();
  wr_renderable_set_mesh(mCenterOfMassRenderable, WR_MESH(mCenterOfMassMesh));
  wr_renderable_set_drawing_order(mCenterOfMassRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);
  wr_renderable_set_drawing_mode(mCenterOfMassRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_material(mCenterOfMassRenderable, mCenterOfMassMaterial, NULL);
  wr_renderable_set_visibility_flags(mCenterOfMassRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);

  const float s = 0.1f * wr_config_get_line_scale();
  const float scale[3] = {s, s, s};
  wr_transform_set_scale(mCenterOfMassTransform, scale);

  wr_transform_attach_child(mTransform, WR_NODE(mPolygonRenderable));
  wr_transform_attach_child(mTransform, WR_NODE(mPolygonOutlineRenderable));
  wr_transform_attach_child(mCenterOfMassTransform, WR_NODE(mCenterOfMassRenderable));

  WrTransform *root = wr_scene_get_root(wr_scene_get_instance());
  wr_transform_attach_child(mTransform, WR_NODE(mCenterOfMassTransform));
  wr_transform_attach_child(root, WR_NODE(mTransform));
  wr_node_set_visible(WR_NODE(mTransform), false);
}

WbSupportPolygonRepresentation::~WbSupportPolygonRepresentation() {
  cleanup();
}

void WbSupportPolygonRepresentation::show(bool visible) {
  wr_node_set_visible(WR_NODE(mTransform), visible);
}

static void addVertex(WrDynamicMesh *mesh, int index, float x, float y, float z) {
  const float vertex[3] = {x, y, z};
  wr_dynamic_mesh_add_vertex(mesh, vertex);
  wr_dynamic_mesh_add_index(mesh, index);
}

void WbSupportPolygonRepresentation::draw(const WbPolygon &p, float y, const WbVector3 &globalCenterOfMass,
                                          const WbVector3 *worldBasis) {
  const int size = p.actualSize();

  wr_dynamic_mesh_clear(mPolygonMesh);
  wr_dynamic_mesh_clear(mPolygonOutlineMesh);

  // No contact points
  if (size == 0) {
    show(false);
    return;
  }
  show(true);

  const double globalComX = globalCenterOfMass.dot(worldBasis[X]);
  const double globalComZ = globalCenterOfMass.dot(worldBasis[Z]);
  const bool stable = p.contains(globalComX, globalComZ);

  // Set materials
  if (stable) {
    wr_phong_material_set_color(mPolygonMaterial, STABLE_POLYGON_COLOR);
    wr_phong_material_set_color(mPolygonOutlineMaterial, STABLE_POLYGON_OUTLINE_COLOR);
    wr_phong_material_set_color(mCenterOfMassMaterial, STABLE_CENTER_OF_MASS_COLOR);

    wr_phong_material_set_transparency(mPolygonMaterial, STABLE_POLYGON_COLOR[3]);
    wr_phong_material_set_transparency(mPolygonOutlineMaterial, STABLE_POLYGON_OUTLINE_COLOR[3]);
    wr_phong_material_set_transparency(mCenterOfMassMaterial, STABLE_CENTER_OF_MASS_COLOR[3]);
  } else {
    wr_phong_material_set_color(mPolygonMaterial, UNSTABLE_POLYGON_COLOR);
    wr_phong_material_set_color(mPolygonOutlineMaterial, UNSTABLE_POLYGON_OUTLINE_COLOR);
    wr_phong_material_set_color(mCenterOfMassMaterial, UNSTABLE_CENTER_OF_MASS_COLOR);

    wr_phong_material_set_transparency(mPolygonMaterial, UNSTABLE_POLYGON_COLOR[3]);
    wr_phong_material_set_transparency(mPolygonOutlineMaterial, UNSTABLE_POLYGON_OUTLINE_COLOR[3]);
    wr_phong_material_set_transparency(mCenterOfMassMaterial, UNSTABLE_CENTER_OF_MASS_COLOR[3]);
  }

  // Set orientations
  WbRotation rotation(worldBasis[X], worldBasis[Y], worldBasis[Z]);
  rotation.normalize();
  float orientation[4];
  rotation.toFloatArray(orientation);
  wr_transform_set_orientation(mTransform, orientation);
  // Set the projected center of mass position
  const float position[3] = {static_cast<float>(globalComX), y, static_cast<float>(globalComZ)};
  wr_transform_set_position(mCenterOfMassTransform, position);

  const float l = wr_config_get_line_scale() * WbWrenRenderingContext::SOLID_LINE_SCALE_FACTOR;
  // A single contact point
  if (size == 1) {
    addVertex(mPolygonOutlineMesh, 0, p[0].x() - l, y, p[0].y());
    addVertex(mPolygonOutlineMesh, 1, p[0].x() + l, y, p[0].y());
    addVertex(mPolygonOutlineMesh, 2, p[0].x(), y, p[0].y() - l);
    addVertex(mPolygonOutlineMesh, 3, p[0].x(), y, p[0].y() + l);
    addVertex(mPolygonOutlineMesh, 4, p[0].x(), y - l, p[0].y());
    addVertex(mPolygonOutlineMesh, 5, p[0].x(), y + l, p[0].y());
    return;
  }

  // Draws the outline
  int vertexCounter = 0;
  const int sizeMinusOne = size - 1;
  for (int i = 0; i < sizeMinusOne; ++i) {
    addVertex(mPolygonOutlineMesh, vertexCounter++, p[i].x(), y, p[i].y());
    addVertex(mPolygonOutlineMesh, vertexCounter++, p[i + 1].x(), y, p[i + 1].y());
  }

  if (size > 2) {
    // Closes the edge loop
    addVertex(mPolygonOutlineMesh, vertexCounter++, p[sizeMinusOne].x(), y, p[sizeMinusOne].y());
    addVertex(mPolygonOutlineMesh, vertexCounter++, p[0].x(), y, p[0].y());
  } else
    return;

  // Draws the filled polygon
  addVertex(mPolygonMesh, 0, p[0].x(), y, p[0].y());
  addVertex(mPolygonMesh, 1, p[1].x(), y, p[1].y());
  addVertex(mPolygonMesh, 2, p[2].x(), y, p[2].y());
  // Draw one side (face culling disabled)
  for (int i = 3; i <= sizeMinusOne; i++)
    addVertex(mPolygonMesh, i, p[i].x(), y, p[i].y());
}

void WbSupportPolygonRepresentation::setScale(const float *scale) {
  wr_transform_set_scale(mCenterOfMassTransform, scale);
}

void WbSupportPolygonRepresentation::cleanup() {
  wr_node_delete(WR_NODE(mTransform));
  wr_node_delete(WR_NODE(mCenterOfMassTransform));

  wr_node_delete(WR_NODE(mPolygonRenderable));
  wr_node_delete(WR_NODE(mPolygonOutlineRenderable));
  wr_node_delete(WR_NODE(mCenterOfMassRenderable));

  wr_dynamic_mesh_delete(mPolygonMesh);
  wr_dynamic_mesh_delete(mPolygonOutlineMesh);
  wr_static_mesh_delete(mCenterOfMassMesh);

  wr_material_delete(mPolygonMaterial);
  wr_material_delete(mPolygonOutlineMaterial);
  wr_material_delete(mCenterOfMassMaterial);
}
