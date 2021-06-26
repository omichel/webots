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

#include "WbWrenAbstractResizeManipulator.hpp"

#include "WbWrenOpenGlContext.hpp"
#include "WbWrenPicker.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/file_import.h>
#include <wren/node.h>

#include <QtCore/QByteArray>
#include <QtCore/QFileInfo>

const int WbWrenAbstractResizeManipulator::STANDARD_COORDINATES[3] = {X, Y, Z};
const WbVector3 WbWrenAbstractResizeManipulator::STANDARD_COORDINATE_VECTORS[3] = {
  WbVector3(1.0, 0.0, 0.0), WbVector3(0.0, 0.0, 1.0), WbVector3(0.0, 1.0, 0.0)};

static const char *FILE_PATH_DOUBLE_ARROW = "gl:meshes/double_arrow.obj";

WbWrenAbstractResizeManipulator::WbWrenAbstractResizeManipulator(ResizeConstraint constraint) :
  WbWrenAbstractManipulator(3),
  mConstraint(constraint),
  mUniformMaterialIndex(X) {
  for (int i = 0; i < 3; ++i) {
    mHandleMaterials[i] = NULL;
    mAxisMaterials[i] = NULL;
    mHandleTransforms[i] = NULL;
    mAxisTransforms[i] = NULL;
  }
}

WbWrenAbstractResizeManipulator::~WbWrenAbstractResizeManipulator() {
  WbWrenOpenGlContext::makeWrenCurrent();

  for (WrStaticMesh *mesh : mMeshes)
    wr_static_mesh_delete(mesh);

  for (WrRenderable *renderable : mRenderables) {
    // Delete picking material
    wr_material_delete(wr_renderable_get_material(renderable, "picking"));
    wr_node_delete(WR_NODE(renderable));
  }

  for (int i = 0; i < mNumberOfHandles; ++i) {
    wr_material_delete(mHandleMaterials[i]);
    wr_material_delete(mAxisMaterials[i]);
    wr_node_delete(WR_NODE(mHandleTransforms[i]));
    wr_node_delete(WR_NODE(mAxisTransforms[i]));
  }

  WbWrenOpenGlContext::doneWren();
}

void WbWrenAbstractResizeManipulator::initializeHandlesEntities(bool resize) {
  mHandlesShader = WbWrenShaders::handlesShader();
  mHandlesPickingShader = WbWrenShaders::handlesPickingShader();

  const float colors[3][3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}};
  const float axesAmbientColor[3][3] = {{0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.5f}, {0.0f, 0.5f, 0.0f}};
  const float ones[3] = {1.0f, 1.0f, 1.0f};
  const float zeroes[3] = {0.0f, 0.0f, 0.0f};

  const float handlesTransparency = 0.3f;
  for (int i = 0; i < mNumberOfHandles; ++i) {
    mHandleMaterials[i] = wr_phong_material_new();
    wr_material_set_default_program(mHandleMaterials[i], mHandlesShader);
    wr_phong_material_set_all_parameters(mHandleMaterials[i], axesAmbientColor[i], colors[i], ones, zeroes, 0.2f,
                                         handlesTransparency);

    mAxisMaterials[i] = wr_phong_material_new();
    wr_material_set_default_program(mAxisMaterials[i], mHandlesShader);
    wr_phong_material_set_color(mAxisMaterials[i], colors[i]);
  }

  // Create double arrows

  // We add this tiny offset so that the handles are correctly sorted at rendering
  // (translucent objects are sorted by distance with the camera)
  const float offset[3][3] = {{0.0001f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0001f}, {0.0f, 0.0001f, 0.0f}};

  const float rotation[3][4] = {{1.570796327f, 0.0f, 0.0f, -1.0f}, {1.570796327f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}};

  QByteArray objFile = QFileInfo(FILE_PATH_DOUBLE_ARROW).absoluteFilePath().toUtf8();
  WrStaticMesh *mesh;
  const bool success = wr_import_static_mesh_from_obj(objFile.constData(), &mesh);
  assert(success);
  if (!success) {
    mNumberOfHandles = 0;
    return;
  }

  mMeshes.push_back(mesh);

  const int pickerAxis[3] = {WbWrenPicker::HANDLES_X_AXIS, WbWrenPicker::HANDLES_Z_AXIS, WbWrenPicker::HANDLES_Y_AXIS};

  for (int i = 0; i < mNumberOfHandles; ++i) {
    WrRenderable *renderable = wr_renderable_new();
    wr_renderable_set_scene_culling(renderable, false);
    mRenderables.push_back(renderable);

    WbWrenPicker::setPickable(renderable,
                              (pickerAxis[i] | (resize ? WbWrenPicker::HANDLES_RESIZE : WbWrenPicker::HANDLES_SCALE)), true);
    WrMaterial *pickingMaterial = wr_renderable_get_material(renderable, "picking");
    wr_material_set_default_program(pickingMaterial, mHandlesPickingShader);

    wr_renderable_set_drawing_order(renderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);
    wr_renderable_set_visibility_flags(renderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
    wr_renderable_set_mesh(renderable, WR_MESH(mesh));
    wr_renderable_set_material(renderable, mHandleMaterials[i], NULL);

    WrTransform *transform = wr_transform_new();
    wr_transform_set_orientation(transform, rotation[i]);
    wr_transform_set_position(transform, offset[i]);
    wr_transform_attach_child(transform, WR_NODE(renderable));
    wr_transform_attach_child(mTransform, WR_NODE(transform));
    mHandleTransforms[i] = transform;
  }

  // Create axes
  const float axesCoordinates[3][6] = {
    {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f}};

  for (int i = 0; i < mNumberOfHandles; ++i) {
    WrRenderable *renderable = wr_renderable_new();
    mRenderables.push_back(renderable);

    mesh = wr_static_mesh_line_set_new(2, axesCoordinates[i], NULL);
    mMeshes.push_back(mesh);

    wr_renderable_set_mesh(renderable, WR_MESH(mesh));
    wr_renderable_set_drawing_mode(renderable, WR_RENDERABLE_DRAWING_MODE_LINES);
    wr_renderable_set_drawing_order(renderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);
    wr_renderable_set_visibility_flags(renderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
    wr_renderable_set_material(renderable, mAxisMaterials[i], NULL);

    WrTransform *transform = wr_transform_new();
    wr_transform_attach_child(transform, WR_NODE(renderable));
    wr_transform_attach_child(mTransform, WR_NODE(transform));
    mAxisTransforms[i] = transform;
  }

  updateHandlesMaterial();
}

void WbWrenAbstractResizeManipulator::updateHandlesMaterial() {
  assert(mNumberOfHandles <= 3);

  if (mConstraint == NO_CONSTRAINT) {
    for (int i = 0; i < mNumberOfHandles; ++i) {
      wr_renderable_set_material(mRenderables[mNumberOfHandles + i], mAxisMaterials[i], NULL);  // axis
      wr_renderable_set_material(mRenderables[i], mHandleMaterials[i], NULL);                   // arrow
    }
  } else if (mConstraint == UNIFORM) {
    for (int i = 0; i < mNumberOfHandles; ++i) {
      wr_renderable_set_material(mRenderables[mNumberOfHandles + i], mAxisMaterials[mUniformMaterialIndex], NULL);  // axis
      wr_renderable_set_material(mRenderables[i], mHandleMaterials[mUniformMaterialIndex], NULL);                   // arrow
    }
  } else {  // X_EQUAL_Z
    for (int i = 0; i < mNumberOfHandles; ++i) {
      if (i == Z) {
        wr_renderable_set_material(mRenderables[mNumberOfHandles + i], mAxisMaterials[X], NULL);  // axis
        wr_renderable_set_material(mRenderables[i], mHandleMaterials[X], NULL);                   // arrow
      } else {
        wr_renderable_set_material(mRenderables[mNumberOfHandles + i], mAxisMaterials[i], NULL);  // axis
        wr_renderable_set_material(mRenderables[i], mHandleMaterials[i], NULL);                   // arrow
      }
    }
  }
}

void WbWrenAbstractResizeManipulator::setResizeConstraint(ResizeConstraint constraint) {
  if (mConstraint == constraint)
    return;

  mConstraint = constraint;
  updateHandlesMaterial();
}

void WbWrenAbstractResizeManipulator::highlightAxis(int index) {
  if (mConstraint == NO_CONSTRAINT) {
    for (int i = 0; i < mNumberOfHandles; ++i) {
      wr_node_set_visible(WR_NODE(mHandleTransforms[i]), false);
      wr_node_set_visible(WR_NODE(mAxisTransforms[i]), false);
    }

    wr_node_set_visible(WR_NODE(mHandleTransforms[index]), true);
    wr_node_set_visible(WR_NODE(mAxisTransforms[index]), true);
  } else if (mConstraint == X_EQUAL_Z) {
    const bool showXZ = index != Y;
    for (int i = 0; i < mNumberOfHandles; ++i) {
      if (i == Z || i == X) {
        wr_node_set_visible(WR_NODE(mHandleTransforms[i]), showXZ);
        wr_node_set_visible(WR_NODE(mAxisTransforms[i]), showXZ);
      } else {
        wr_node_set_visible(WR_NODE(mHandleTransforms[i]), !showXZ);
        wr_node_set_visible(WR_NODE(mAxisTransforms[i]), !showXZ);
      }
    }
  }
}

void WbWrenAbstractResizeManipulator::showNormal() {
  // show handles
  for (int i = 0; i < mNumberOfHandles; ++i) {
    wr_node_set_visible(WR_NODE(mHandleTransforms[i]), true);
    wr_node_set_visible(WR_NODE(mAxisTransforms[i]), true);
  }
}

void WbWrenAbstractResizeManipulator::updateHandleDimensions(const float scaleFactor, const float viewDistanceScale) {
  const float scale[3][3] = {{scaleFactor, 1.0f, 1.0f}, {1.0f, 1.0f, scaleFactor}, {1.0f, scaleFactor, 1.0f}};
  const float positionOffset = viewDistanceScale * mScale * (scaleFactor - 1.0f);
  const float position[3][3] = {{positionOffset, 0.0f, 0.0f}, {0.0f, 0.0f, positionOffset}, {0.0f, positionOffset, 0.0f}};

  for (int i = 0; i < mNumberOfHandles; ++i) {
    wr_transform_set_scale(mAxisTransforms[i], scale[i]);
    wr_transform_set_position(mHandleTransforms[i], position[i]);
  }
}
