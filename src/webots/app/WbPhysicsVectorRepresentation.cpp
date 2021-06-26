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

#include "WbPhysicsVectorRepresentation.hpp"

#include "WbRotation.hpp"
#include "WbVector3.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/node.h>
#include <wren/scene.h>
#include <wren/shader_program.h>
#include <wren/viewport.h>

// Abstract class //
////////////////////

// WbPhysicsVectorRepresentation functions

WbPhysicsVectorRepresentation::~WbPhysicsVectorRepresentation() {
  wr_material_delete(mMaterial);

  wr_node_delete(WR_NODE(mArrowRenderable));
  wr_node_delete(WR_NODE(mTailRenderable));

  wr_static_mesh_delete(mTailMesh);
  wr_static_mesh_delete(mArrowMesh);

  wr_node_delete(WR_NODE(mHeadTransform));
  wr_node_delete(WR_NODE(mTailTransform));
}

void WbPhysicsVectorRepresentation::initializeTailAndArrow(const float *materialColor) {
  // Create arrow & tail mesh
  const float tailVertices[6] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};

  const float arrowVertices[12] = {0.0f, -0.9f, 0.0f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, -1.0f, 0.0f};

  const float arrowNormals[12] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

  const unsigned int arrowIndices[12] = {2, 1, 0, 0, 3, 2, 0, 1, 2, 2, 3, 0};

  mMaterial = wr_phong_material_new();
  wr_phong_material_set_color(mMaterial, materialColor);
  wr_material_set_default_program(mMaterial, WbWrenShaders::simpleShader());

  mArrowMesh = wr_static_mesh_new(12, 12, arrowVertices, arrowNormals, arrowNormals, arrowNormals, arrowIndices, false);
  mTailMesh = wr_static_mesh_line_set_new(2, tailVertices, NULL);

  mArrowRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mArrowRenderable, false);
  wr_renderable_set_receive_shadows(mArrowRenderable, false);
  wr_renderable_set_visibility_flags(mArrowRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  wr_renderable_set_mesh(mArrowRenderable, WR_MESH(mArrowMesh));
  wr_renderable_set_material(mArrowRenderable, mMaterial, NULL);
  wr_renderable_set_drawing_order(mArrowRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);

  mTailRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mTailRenderable, false);
  wr_renderable_set_receive_shadows(mTailRenderable, false);
  wr_renderable_set_drawing_mode(mTailRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_visibility_flags(mTailRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  wr_renderable_set_mesh(mTailRenderable, WR_MESH(mTailMesh));
  wr_renderable_set_material(mTailRenderable, mMaterial, NULL);
  wr_renderable_set_drawing_order(mTailRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);

  mHeadTransform = wr_transform_new();
  wr_transform_attach_child(mHeadTransform, WR_NODE(mArrowRenderable));

  mTailTransform = wr_transform_new();
  wr_transform_attach_child(mTailTransform, WR_NODE(mTailRenderable));

  WrTransform *root = wr_scene_get_root(wr_scene_get_instance());
  wr_transform_attach_child(root, WR_NODE(mHeadTransform));
  wr_transform_attach_child(root, WR_NODE(mTailTransform));
}

void WbPhysicsVectorRepresentation::setScale(float scale) {
  WrViewport *viewport = wr_scene_get_viewport(wr_scene_get_instance());

  const float width = wr_viewport_get_width(viewport), height = wr_viewport_get_height(viewport),
              maxDimension = height > width ? height : width, scaleFactor = scale * 2.0f / maxDimension;

  const float new_scale[3] = {scaleFactor, scaleFactor, scaleFactor};
  wr_transform_set_scale(mHeadTransform, new_scale);
}

void WbPhysicsVectorRepresentation::updatePosition(const WbVector3 &begin, const WbVector3 &end,
                                                   const WbRotation &orientation) {
  // Update arrow coordinates
  float tail[6];
  const double *data = begin.ptr();
  for (int i = 0; i < 6; ++i) {
    if (i > 2)
      data = end.ptr();

    tail[i] = static_cast<float>(data[i % 3]);
  }

  const float scaleFactor = (end - begin).length();
  const float scale[3] = {scaleFactor, scaleFactor, scaleFactor};

  // Tail
  wr_transform_set_position(mTailTransform, tail);
  wr_transform_set_scale(mTailTransform, scale);

  // Head
  tail[1] += scale[1];
  wr_transform_set_position(mHeadTransform, tail + 3);

  // Arrow orientation
  WbVector3 baseX, baseY, baseZ;
  baseY = (end - begin).normalized();

  // Check if the vector is parallel to the xy-plan of the camera (dot product close to zero)
  if (fabs(baseY.dot(orientation.direction())) < 1e-6)
    baseX = baseY.cross(orientation.direction()).normalized();
  else {
    baseZ = orientation.up().cross(baseY).normalized();
    baseX = baseY.cross(baseZ).normalized();
  }
  baseZ = baseX.cross(baseY).normalized();

  WbRotation rotation(baseX, baseY, baseZ);
  rotation.normalize();

  float rotationArray[4];
  rotation.toFloatArray(rotationArray);
  wr_transform_set_orientation(mTailTransform, rotationArray);
  wr_transform_set_orientation(mHeadTransform, rotationArray);
}

// Implemented classes //
/////////////////////////

// WbForceRepresentation functions

WbForceRepresentation::WbForceRepresentation() {
  const float forceColor[3] = {1.0f, 0.5f, 0.0f};
  initializeTailAndArrow(forceColor);
}

// WbToqueRepresentation functions

WbTorqueRepresentation::WbTorqueRepresentation() {
  const float torqueColor[3] = {1.0f, 0.85f, 0.0f};
  initializeTailAndArrow(torqueColor);
  initializeSpinSymbol();
}

void WbTorqueRepresentation::initializeSpinSymbol() {
  mCoilMaterial = wr_phong_material_new();
  wr_material_set_default_program(mCoilMaterial, WbWrenShaders::simpleShader());
  const float coilColor[3] = {1.0f, 1.0f, 0.0f};
  wr_phong_material_set_color(mCoilMaterial, coilColor);
  // Coil
  const int steps = 32;
  const float coilHeight = 0.1f;
  const float coilRadius = 0.1f;
  const float revolutions = 1.25f * (2 * M_PI);
  const float coilStartHeight = 0.4f;

  // Start from the top of the coil (easier to place the arrow afterward)
  float *vertices = new float[steps * 3 * 2];
  for (int i = 0; i < steps; ++i) {
    const int vertexIndex = i * 6;
    // i is negated to descend clockwise and thus match torque direction (coil is constructed from the end)
    vertices[vertexIndex] = coilRadius * sin(-i * (revolutions / steps));
    vertices[vertexIndex + 1] = coilStartHeight + coilHeight - (coilHeight / steps) * i;
    vertices[vertexIndex + 2] = coilRadius * cos(-i * (revolutions / steps));

    vertices[vertexIndex + 3] = coilRadius * sin(-(i + 1) * (revolutions / steps));
    vertices[vertexIndex + 4] = coilStartHeight + coilHeight - (coilHeight / steps) * (i + 1);
    vertices[vertexIndex + 5] = coilRadius * cos(-(i + 1) * (revolutions / steps));
  }

  mCoilMesh = wr_static_mesh_line_set_new(steps * 2, vertices, NULL);
  mCoilRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mCoilRenderable, false);
  wr_renderable_set_receive_shadows(mCoilRenderable, false);
  wr_renderable_set_drawing_mode(mCoilRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_visibility_flags(mCoilRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  wr_renderable_set_mesh(mCoilRenderable, WR_MESH(mCoilMesh));
  wr_renderable_set_material(mCoilRenderable, mCoilMaterial, NULL);
  wr_renderable_set_drawing_order(mCoilRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);

  wr_transform_attach_child(mTailTransform, WR_NODE(mCoilRenderable));

  // Coil arrow (always at the top of the coil)
  mCoilArrowRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mCoilArrowRenderable, false);
  wr_renderable_set_receive_shadows(mCoilArrowRenderable, false);
  wr_renderable_set_visibility_flags(mCoilArrowRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  wr_renderable_set_mesh(mCoilArrowRenderable, WR_MESH(mArrowMesh));
  wr_renderable_set_material(mCoilArrowRenderable, mCoilMaterial, NULL);
  wr_renderable_set_drawing_order(mCoilArrowRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);

  const float orientation[4] = {-M_PI / 2.0f, 0.0f, 0.0f, 1.0f};
  const float factor = 0.02f;
  const float scale[3] = {factor, factor, factor};
  float *position = vertices;
  position[0] += (factor * 0.9f);

  mCoilArrowTransform = wr_transform_new();
  wr_transform_set_position(mCoilArrowTransform, position);
  wr_transform_set_orientation(mCoilArrowTransform, orientation);
  wr_transform_set_scale(mCoilArrowTransform, scale);
  wr_transform_attach_child(mCoilArrowTransform, WR_NODE(mCoilArrowRenderable));
  wr_transform_attach_child(mTailTransform, WR_NODE(mCoilArrowTransform));

  delete[] vertices;
}

WbTorqueRepresentation::~WbTorqueRepresentation() {
  wr_static_mesh_delete(mCoilMesh);
  wr_node_delete(WR_NODE(mCoilRenderable));
  wr_node_delete(WR_NODE(mCoilArrowRenderable));
  wr_node_delete(WR_NODE(mCoilArrowTransform));
  wr_material_delete(mCoilMaterial);
}
