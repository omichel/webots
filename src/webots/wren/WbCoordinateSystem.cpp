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

#include "WbCoordinateSystem.hpp"

#include "WbQuaternion.hpp"
#include "WbRotation.hpp"
#include "WbStandardPaths.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/node.h>
#include <wren/scene.h>
#include <wren/shader_program.h>
#include <wren/texture.h>

WbCoordinateSystem::WbCoordinateSystem(WbWrenRenderingContext *context) {
  const float labelMeshCoords[12] = {-0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f};

  const unsigned int labelMeshIndices[6] = {2, 1, 0, 3, 1, 2};

  const float labelTexCoords[8] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};

  // Null normals as we wont be using them
  const float labelMeshNormals[12] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

  // Labels
  mLabelsMesh =
    wr_static_mesh_new(4, 6, labelMeshCoords, labelMeshNormals, labelTexCoords, labelTexCoords, labelMeshIndices, false);
  const float labelsOffset[3][3] = {{1.15f, 0.0f, 0.0f}, {0.0f, 1.15f, 0.0f}, {0.0f, 0.0f, 1.15f}};

  mTransform = wr_transform_new();

  mFont = wr_font_new();
  wr_font_set_face(mFont, (WbStandardPaths::fontsPath() + "Arial.ttf").toUtf8().constData());
  wr_font_set_size(mFont, 64);

  float labelsColor[3][4] = {{0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}};
  const char *labels[3] = {"X", "Y", "Z"};
  const float labelsScale = 0.3f;

  // Setup coordinate system position & scale
  const float globalScale = 0.1f;
  const float scale[3] = {globalScale, globalScale, globalScale};
  wr_transform_set_scale(mTransform, scale);
  const float position[3] = {0.0f, 0.0f, -0.5f};
  wr_transform_set_position(mTransform, position);

  const float screenPosition[2] = {0.9f, -0.9f};
  WrShaderProgram *shader = WbWrenShaders::coordinateSystemShader();
  wr_shader_program_set_custom_uniform_value(shader, "screenPosition", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                             reinterpret_cast<const char *>(screenPosition));
  wr_shader_program_set_custom_uniform_value(shader, "size", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                             reinterpret_cast<const char *>(&globalScale));

  const float axesCoordinates[3][6] = {
    {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};

  const float axesColor[3][3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};

  // Create axes & their labels
  for (int i = 0; i < 3; ++i) {
    // Axis
    mAxesMesh[i] = wr_static_mesh_line_set_new(2, axesCoordinates[i], NULL);
    mAxesMaterial[i] = wr_phong_material_new();
    wr_phong_material_set_color(mAxesMaterial[i], axesColor[i]);
    wr_material_set_default_program(mAxesMaterial[i], WbWrenShaders::coordinateSystemShader());

    WrRenderable *renderable = wr_renderable_new();
    mRenderables[i][0] = renderable;
    wr_renderable_set_cast_shadows(renderable, false);
    wr_renderable_set_receive_shadows(renderable, false);
    wr_renderable_set_mesh(renderable, WR_MESH(mAxesMesh[i]));
    wr_renderable_set_material(renderable, mAxesMaterial[i], NULL);
    wr_renderable_set_drawing_order(renderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_2);
    wr_renderable_set_drawing_mode(renderable, WR_RENDERABLE_DRAWING_MODE_LINES);
    wr_renderable_set_visibility_flags(renderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
    wr_renderable_set_scene_culling(renderable, false);
    wr_renderable_set_in_view_space(renderable, true);
    wr_renderable_set_z_sorted_rendering(renderable, true);

    wr_transform_attach_child(mTransform, WR_NODE(renderable));

    // Label
    renderable = wr_renderable_new();
    mRenderables[i][1] = renderable;

    mLabelsTexture[i] = wr_drawable_texture_new();
    int width, height;
    wr_font_get_bounding_box(mFont, labels[i], &width, &height);

    wr_texture_set_size(WR_TEXTURE(mLabelsTexture[i]), width, height);
    wr_texture_set_translucent(WR_TEXTURE(mLabelsTexture[i]), true);
    wr_drawable_texture_set_use_premultiplied_alpha(mLabelsTexture[i], true);
    wr_drawable_texture_set_color(mLabelsTexture[i], labelsColor[i]);
    wr_drawable_texture_set_font(mLabelsTexture[i], mFont);
    wr_texture_setup(WR_TEXTURE(mLabelsTexture[i]));
    wr_drawable_texture_clear(mLabelsTexture[i]);
    wr_drawable_texture_draw_text(mLabelsTexture[i], labels[i], 0, 0);

    mLabelsMaterial[i] = wr_phong_material_new();
    wr_material_set_default_program(mLabelsMaterial[i], shader);
    wr_material_set_texture(mLabelsMaterial[i], WR_TEXTURE(mLabelsTexture[i]), 0);

    wr_renderable_set_cast_shadows(renderable, false);
    wr_renderable_set_receive_shadows(renderable, false);
    wr_renderable_set_mesh(renderable, WR_MESH(mLabelsMesh));
    wr_renderable_set_material(renderable, mLabelsMaterial[i], NULL);
    wr_renderable_set_drawing_order(renderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_2);
    wr_renderable_set_visibility_flags(renderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
    wr_renderable_set_scene_culling(renderable, false);
    wr_renderable_set_in_view_space(renderable, true);
    wr_renderable_set_z_sorted_rendering(renderable, true);

    mLabelsTransform[i] = wr_transform_new();
    wr_transform_set_position(mLabelsTransform[i], labelsOffset[i]);
    const float aspectRatio = width / (float)(height);
    const float transformScale[3] = {aspectRatio * labelsScale, labelsScale, 1.0f};
    wr_transform_set_scale(mLabelsTransform[i], transformScale);

    wr_transform_attach_child(mLabelsTransform[i], WR_NODE(renderable));
    wr_transform_attach_child(mTransform, WR_NODE(mLabelsTransform[i]));
  }

  WrTransform *root = wr_scene_get_root(wr_scene_get_instance());
  wr_transform_attach_child(root, WR_NODE(mTransform));
}

void WbCoordinateSystem::deleteWrenObjects() {
  wr_node_delete(WR_NODE(mTransform));
  wr_static_mesh_delete(mLabelsMesh);
  wr_font_delete(mFont);

  for (int i = 0; i < 3; ++i) {
    wr_node_delete(WR_NODE(mRenderables[i][0]));
    wr_node_delete(WR_NODE(mRenderables[i][1]));

    wr_static_mesh_delete(mAxesMesh[i]);

    wr_material_delete(mAxesMaterial[i]);
    wr_material_delete(mLabelsMaterial[i]);

    wr_node_delete(WR_NODE(mLabelsTransform[i]));

    wr_texture_delete(WR_TEXTURE(mLabelsTexture[i]));
  }
}

WbCoordinateSystem::~WbCoordinateSystem() {
  deleteWrenObjects();
}

void WbCoordinateSystem::setVisible(bool b) {
  wr_node_set_visible(WR_NODE(mTransform), b);
}

void WbCoordinateSystem::setOrientation(const WbQuaternion &quaternion) {
  float rotation[4];
  WbRotation(quaternion).toFloatArray(rotation);
  wr_transform_set_orientation(mTransform, rotation);

  WbRotation(quaternion.conjugated()).toFloatArray(rotation);
  for (int i = 0; i < 3; ++i)
    wr_transform_set_orientation(mLabelsTransform[i], rotation);
}
