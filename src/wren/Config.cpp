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

#include "Config.hpp"

#include "Camera.hpp"
#include "Debug.hpp"
#include "GlState.hpp"
#include "GlslLayout.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "Transform.hpp"
#include "UniformBuffer.hpp"
#include "Viewport.hpp"

#include <wren/config.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {
  namespace config {

    static bool cAreShadowsEnabled = true;
    static bool cIsPointSizeEnabled = false;
    static float cLineScale = 1.0f;
    static bool cShowBoundingSpheres = false;
    static bool cShowAabbs = false;
    static bool cShowShadowAabbs = false;
    static StaticMesh *cSphereMesh = NULL;
    static StaticMesh *cBoxMesh = NULL;
    static ShaderProgram *cBoundingVolumeProgram = NULL;
    static bool cRequiresFlushAfterDraw = false;
    static bool cRequiresDepthBufferDistortion = false;

    void enableShadows(bool enable) { cAreShadowsEnabled = enable; }

    void enabledPointSize(bool enable) {
      cIsPointSizeEnabled = enable;
      glstate::enablePointSize(enable);
    }

    void setLineScale(float lineScale) { cLineScale = lineScale; }

    void setShowBoundingSpheres(bool show) {
      if (show == cShowBoundingSpheres)
        return;

      cShowBoundingSpheres = show;
      if (show) {
        if (!cSphereMesh)
          cSphereMesh = StaticMesh::createUnitIcosphere(2);
      } else if (cSphereMesh) {
        Mesh::deleteMesh(cSphereMesh);
        cSphereMesh = NULL;
      }
    }

    void setShowAabbs(bool show) {
      if (show == cShowAabbs)
        return;

      cShowAabbs = show;

      if (show) {
        if (!cBoxMesh)
          cBoxMesh = StaticMesh::createUnitBox(false);
      } else if (cBoxMesh) {
        Mesh::deleteMesh(cBoxMesh);
        cBoxMesh = NULL;
      }
    }

    void setShowShadowAabbs(bool show) {
      if (show == cShowShadowAabbs)
        return;

      cShowShadowAabbs = show;

      if (show) {
        if (!cBoxMesh)
          cBoxMesh = StaticMesh::createUnitBox(false);
      } else if (cBoxMesh) {
        Mesh::deleteMesh(cBoxMesh);
        cBoxMesh = NULL;
      }
    }

    void setBoundingVolumeProgram(ShaderProgram *program) { cBoundingVolumeProgram = program; }

    void setRequiresFlushAfterDraw(bool require) { cRequiresFlushAfterDraw = require; }

    void setRequiresDepthBufferDistortion(bool require) { cRequiresDepthBufferDistortion = require; }

    void drawAabb(const primitive::Aabb &aabb) {
      if (!cShowAabbs && !cShowShadowAabbs)
        return;

      assert(cBoundingVolumeProgram);
      assert(cBoxMesh);

      cBoundingVolumeProgram->bind();

      // In case of infinite AABB
      primitive::Aabb copy = aabb;
      for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < 3; ++i) {
          if (copy.mBounds[j][i] == std::numeric_limits<float>::max())
            copy.mBounds[j][i] = 1e4;
          if (copy.mBounds[j][i] == -std::numeric_limits<float>::max())
            copy.mBounds[j][i] = -1e4;
        }
      }

      const glm::vec3 scale = copy.mBounds[1] - copy.mBounds[0];
      const glm::vec3 position = 0.5f * (copy.mBounds[1] + copy.mBounds[0]);
      glm::mat4 matrix = glm::mat4(scale.x, 0.0f, 0.0f, 0.0f, 0.0f, scale.y, 0.0f, 0.0f, 0.0f, 0.0f, scale.z, 0.0f, position.x,
                                   position.y, position.z, 1.0f);

      glUniformMatrix4fv(cBoundingVolumeProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM), 1, false,
                         glm::value_ptr(matrix));

      glstate::setBlend(true);
      glstate::setBlendEquation(GL_FUNC_ADD);
      glstate::setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glstate::setDepthTest(false);
      glstate::setStencilTest(false);
      glstate::setCullFace(false);
      glstate::setColorMask(true, true, true, true);
      glstate::setPolygonMode(GL_LINE);
      cBoxMesh->render(GL_TRIANGLES);
      glstate::setPolygonMode(Scene::instance()->currentViewport()->polygonMode());
    }

    void drawBoundingSphere(const primitive::Sphere &sphere) {
      if (!cShowBoundingSpheres)
        return;

      assert(cBoundingVolumeProgram);
      assert(cSphereMesh);

      cBoundingVolumeProgram->bind();

      const float scale = sphere.mRadius;
      const glm::vec3 position = sphere.mCenter;
      glm::mat4 matrix = glm::mat4(scale, 0.0f, 0.0f, 0.0f, 0.0f, scale, 0.0f, 0.0f, 0.0f, 0.0f, scale, 0.0f, position.x,
                                   position.y, position.z, 1.0f);

      glUniformMatrix4fv(cBoundingVolumeProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM), 1, false,
                         glm::value_ptr(matrix));

      glstate::setBlend(true);
      glstate::setBlendEquation(GL_FUNC_ADD);
      glstate::setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glstate::setDepthTest(false);
      glstate::setStencilTest(false);
      glstate::setCullFace(false);
      glstate::setColorMask(true, true, true, true);
      glstate::setPolygonMode(GL_LINE);
      cSphereMesh->render(GL_TRIANGLES);
      glstate::setPolygonMode(Scene::instance()->currentViewport()->polygonMode());
    }

    bool showAabbs() { return cShowAabbs; }

    bool showShadowAabbs() { return cShowShadowAabbs; }

    bool showBoundingSpheres() { return cShowBoundingSpheres; }

    bool requiresFlushAfterDraw() { return cRequiresFlushAfterDraw; }

    bool requiresDepthBufferDistortion() { return cRequiresDepthBufferDistortion; }

    bool areShadowsEnabled() { return cAreShadowsEnabled; }

    bool isPointSizeEnabled() { return cIsPointSizeEnabled; }

    float lineScale() { return cLineScale; }

    int maxActiveSpotLightCount() { return gMaxActiveSpotLights; }

    int maxActivePointLightCount() { return gMaxActivePointLights; }

    int maxActiveDirectionalLightCount() { return gMaxActiveDirectionalLights; }

    unsigned int maxVerticesPerMeshForShadowRendering() { return gMaxVerticesPerMeshForShadowRendering; }

    void cleanup() {
      Mesh::deleteMesh(cSphereMesh);
      Mesh::deleteMesh(cBoxMesh);
      cSphereMesh = NULL;
      cBoxMesh = NULL;
    }

  }  // namespace config
}  // namespace wren

// C interface implementation
void wr_config_enable_shadows(bool enable) {
  wren::config::enableShadows(enable);
}

void wr_config_enable_point_size(bool enable) {
  wren::config::enabledPointSize(enable);
}

void wr_config_set_line_scale(float line_scale) {
  wren::config::setLineScale(line_scale);
}

void wr_config_set_show_bounding_spheres(bool show) {
  wren::config::setShowBoundingSpheres(show);
}

void wr_config_set_show_axis_aligned_bounding_boxes(bool show) {
  wren::config::setShowAabbs(show);
}

void wr_config_set_show_shadow_axis_aligned_bounding_boxes(bool show) {
  return wren::config::setShowShadowAabbs(show);
}

void wr_config_set_bounding_volume_program(WrShaderProgram *program) {
  wren::config::setBoundingVolumeProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_config_set_requires_flush_after_draw(bool require) {
  wren::config::setRequiresFlushAfterDraw(require);
}

void wr_config_set_requires_depth_buffer_distortion(bool require) {
  wren::config::setRequiresDepthBufferDistortion(require);
}

bool wr_config_are_shadows_enabled() {
  return wren::config::areShadowsEnabled();
}

float wr_config_get_line_scale() {
  return wren::config::lineScale();
}

int wr_config_get_max_active_spot_light_count() {
  return wren::config::maxActiveSpotLightCount();
}

int wr_config_get_max_active_point_light_count() {
  return wren::config::maxActivePointLightCount();
}

int wr_config_get_max_active_directional_light_count() {
  return wren::config::maxActiveDirectionalLightCount();
}
