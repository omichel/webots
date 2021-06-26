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

#include "ShadowVolumeCaster.hpp"

#include "Config.hpp"
#include "Constants.hpp"
#include "Debug.hpp"
#include "DirectionalLight.hpp"
#include "GlState.hpp"
#include "GlslLayout.hpp"
#include "Mesh.hpp"
#include "PositionalLight.hpp"
#include "Renderable.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <vector>

namespace wren {

  ShadowVolumeCaster::ShadowVolumeCaster(Renderable *renderable) : mRenderable(renderable), mHasCaps(false) {
    assert(mRenderable);
  }

  ShadowVolumeCaster::~ShadowVolumeCaster() {
    for (auto &entry : mShadowVolumes) {
      entry.first->unregisterShadowListener(this);
      glDeleteBuffers(2, &entry.second.mGlNameSidesIndexBuffer);
    }
  }

  const primitive::Aabb &ShadowVolumeCaster::aabb(LightNode *light) {
    auto itShadowVolume = mShadowVolumes.find(light);
    if (itShadowVolume == mShadowVolumes.end())
      return gAabbInf;

    if (itShadowVolume->second.mAabbDirty) {
      ShadowVolume *shadowVolume = &itShadowVolume->second;
      const primitive::Aabb &renderableAabb = mRenderable->aabb();
      shadowVolume->mAabb = renderableAabb;

      if (light->type() == LightNode::TYPE_DIRECTIONAL) {
        const glm::vec3 lightDirection = static_cast<DirectionalLight *>(light)->direction();
        for (int i = 0; i < 3; ++i) {
          if (lightDirection[i] > 0.0f)
            shadowVolume->mAabb.mBounds[1][i] = std::numeric_limits<float>::max();
          else if (lightDirection[i] < 0.0f)
            shadowVolume->mAabb.mBounds[0][i] = -std::numeric_limits<float>::max();
        }
      } else {
        const PositionalLight *positionalLight = static_cast<PositionalLight *>(light);
        const glm::vec3 closestPoint = projectPointOnAabb(positionalLight->position(), renderableAabb);
        const glm::vec3 vecFromLight = closestPoint - positionalLight->position();
        const float distance = positionalLight->radius() - glm::length(vecFromLight);
        if (distance > 0)
          shadowVolume->mAabb.extend(closestPoint + glm::normalize(vecFromLight) * distance);
      }
      shadowVolume->mAabbDirty = false;
    }
    return itShadowVolume->second.mAabb;
  }

  void ShadowVolumeCaster::notifyRenderableDirty() {
    for (auto &entry : mShadowVolumes) {
      entry.second.mIsDirty = true;
      entry.second.mAabbDirty = true;
    }
  }

  void ShadowVolumeCaster::notifyLightDirty(LightNode *light, bool aabbOnly) {
    auto itShadowVolume = mShadowVolumes.find(light);
    if (itShadowVolume != mShadowVolumes.end()) {
      if (!aabbOnly)
        itShadowVolume->second.mIsDirty = true;
      itShadowVolume->second.mAabbDirty = true;
    }
  }

  void ShadowVolumeCaster::notifyLightRemoved(LightNode *light) { mShadowVolumes.erase(light); }

  static glm::vec3 computeNormal(const Mesh *mesh, const unsigned int *indices) {
    const std::vector<glm::vec3> &coords = mesh->coords();
    const glm::vec3 v0 = coords[indices[0] >> 1];
    const glm::vec3 v1 = coords[indices[1] >> 1];
    const glm::vec3 v2 = coords[indices[2] >> 1];
    return glm::cross(v1 - v0, v2 - v0);
  }

  void ShadowVolumeCaster::computeSilhouette(LightNode *light, bool computeCaps) {
    assert(mRenderable->mesh());
    const Mesh *mesh = mRenderable->mesh();
    const std::vector<Mesh::Edge> &edges = mesh->edges();

    mRenderable->mesh()->bindShadowVolume();

    auto itShadowVolume = mShadowVolumes.find(light);
    if (itShadowVolume == mShadowVolumes.end()) {
      mShadowVolumes.emplace(light, ShadowVolume());
      itShadowVolume = mShadowVolumes.find(light);
      itShadowVolume->first->registerShadowListener(this);
      ShadowVolume &shadowVolume = itShadowVolume->second;

      const size_t sidesBufferSize = 6 * edges.size() * sizeof(glm::vec3);
      const size_t capsBufferSize = 6 * mesh->triangles().size() * sizeof(glm::vec3);

      glGenBuffers(1, &shadowVolume.mGlNameSidesIndexBuffer);
      glGenBuffers(1, &shadowVolume.mGlNameCapsIndexBuffer);

      glstate::bindElementArrayBuffer(shadowVolume.mGlNameSidesIndexBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sidesBufferSize, NULL, GL_STREAM_DRAW);
      glstate::releaseElementArrayBuffer(shadowVolume.mGlNameSidesIndexBuffer);

      glstate::bindElementArrayBuffer(shadowVolume.mGlNameCapsIndexBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, capsBufferSize, NULL, GL_STREAM_DRAW);
      glstate::releaseElementArrayBuffer(shadowVolume.mGlNameCapsIndexBuffer);
    }

    ShadowVolume &shadowVolume = itShadowVolume->second;
    if (!shadowVolume.mIsDirty && (mHasCaps == computeCaps || mHasCaps))
      return;

    mHasCaps = computeCaps;

    if (light->type() == LightNode::TYPE_DIRECTIONAL) {
      const DirectionalLight *directionalLight = reinterpret_cast<DirectionalLight *>(light);
      const glm::vec3 lightDirectionInModelSpace =
        glm::vec3(glm::inverse(mRenderable->parentMatrix()) * glm::vec4(directionalLight->direction(), 0.0));

      if (computeCaps) {
        // Map caps index buffer
        const size_t maxSize = 3 * mesh->triangles().size() * sizeof(unsigned int);
        glstate::bindElementArrayBuffer(shadowVolume.mGlNameCapsIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxSize, NULL, GL_STREAM_DRAW);

        // Emscripten only accept the GL_MAP_INVALIDATE_BUFFER_BIT option
#ifdef __EMSCRIPTEN__
        unsigned int *data = static_cast<unsigned int *>(
          glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, maxSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
#else
        unsigned int *data = static_cast<unsigned int *>(
          glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, maxSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
#endif

        size_t idx = 0;
        shadowVolume.mIndexCountCaps = 0;
        // Computing caps, light facing info for all triangles required
        for (const Mesh::Triangle &triangle : mesh->triangles()) {
          glm::vec3 normal = triangle.mNormal;
          if (mesh->isDynamic())
            normal = computeNormal(mesh, triangle.mVertexIndices);

          triangle.mIsFacingLight = glm::dot(lightDirectionInModelSpace, normal) < 0.0f;
          if (triangle.mIsFacingLight) {
            data[idx++] = triangle.mVertexIndices[0];
            data[idx++] = triangle.mVertexIndices[1];
            data[idx++] = triangle.mVertexIndices[2];
          }
        }
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        glstate::releaseElementArrayBuffer(shadowVolume.mGlNameCapsIndexBuffer);
        shadowVolume.mIndexCountCaps = idx;
      } else {
        // Not computing caps, light facing info for triangles connected to edges is sufficient
        for (const Mesh::Edge &edge : mesh->edges()) {
          const Mesh::Triangle &triangle0 = mesh->triangle(edge.mTriangleIndices[0]);
          glm::vec3 normal = triangle0.mNormal;
          if (mesh->isDynamic())
            normal = computeNormal(mesh, triangle0.mVertexIndices);

          triangle0.mIsFacingLight = glm::dot(lightDirectionInModelSpace, normal) < 0.0f;

          // Special handling for edges only connected to a single triangle (non-closed meshes)
          if (edge.mTriangleIndices[1] != static_cast<size_t>(~0)) {
            const Mesh::Triangle &triangle1 = mesh->triangle(edge.mTriangleIndices[1]);
            glm::vec3 triangleNormal = triangle1.mNormal;
            if (mesh->isDynamic())
              triangleNormal = computeNormal(mesh, triangle1.mVertexIndices);

            triangle1.mIsFacingLight = glm::dot(lightDirectionInModelSpace, triangleNormal) < 0.0f;
          }
        }
      }

      // Map sides index buffer
      const size_t maxSize = 3 * mesh->edges().size() * sizeof(unsigned int);
      glstate::bindElementArrayBuffer(shadowVolume.mGlNameSidesIndexBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxSize, NULL, GL_STREAM_DRAW);

      // Emscripten only accept the GL_MAP_INVALIDATE_BUFFER_BIT option
#ifdef __EMSCRIPTEN__
      unsigned int *data = static_cast<unsigned int *>(
        glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, maxSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
#else
      unsigned int *data = static_cast<unsigned int *>(
        glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, maxSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
#endif

      size_t idx = 0;
      shadowVolume.mIndexCountSides = 0;
      for (const Mesh::Edge &edge : mesh->edges()) {
        const bool isTriangle0FacingLight = mesh->triangle(edge.mTriangleIndices[0]).mIsFacingLight;

        // Special handling for edges only connected to a single triangle (non-closed meshes)
        bool isTriangle1FacingLight = false;
        if (edge.mTriangleIndices[1] != static_cast<size_t>(~0))
          isTriangle1FacingLight = mesh->triangle(edge.mTriangleIndices[1]).mIsFacingLight;

        if (isTriangle0FacingLight != isTriangle1FacingLight) {
          // For directional lights only one triangle is needed. The third vertex will be extruded in the vertex shader.
          const unsigned int index0 = isTriangle0FacingLight ? edge.mVertexIndices[1] : edge.mVertexIndices[0];
          const unsigned int index1 = isTriangle0FacingLight ? edge.mVertexIndices[0] : edge.mVertexIndices[1];

          data[idx++] = index0;
          data[idx++] = index1;
          data[idx++] = index1 + 1;
        }
      }
      glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      glstate::releaseElementArrayBuffer(shadowVolume.mGlNameSidesIndexBuffer);
      shadowVolume.mIndexCountSides = idx;
    } else {
      const PositionalLight *positionalLight = reinterpret_cast<PositionalLight *>(light);
      const glm::vec3 lightPositionInModelSpace =
        glm::vec3(glm::inverse(mRenderable->parentMatrix()) * glm::vec4(positionalLight->position(), 1.0));

      if (computeCaps) {
        // Map caps index buffer
        const size_t maxSize = 6 * mesh->triangles().size() * sizeof(unsigned int);
        glstate::bindElementArrayBuffer(shadowVolume.mGlNameCapsIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxSize, NULL, GL_STREAM_DRAW);
        // Emscripten only accept the GL_MAP_INVALIDATE_BUFFER_BIT option
#ifdef __EMSCRIPTEN__
        unsigned long long *data = static_cast<unsigned long long *>(
          glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, maxSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
#else
        unsigned long long *data = static_cast<unsigned long long *>(
          glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, maxSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
#endif

        size_t idx = 0;
        shadowVolume.mIndexCountCaps = 0;
        const int stride = 8 * sizeof(unsigned int);

        // Computing caps, light facing info for all triangles required
        for (const Mesh::Triangle &triangle : mesh->triangles()) {
          glm::vec3 normal = triangle.mNormal;
          if (mesh->isDynamic())
            normal = computeNormal(mesh, triangle.mVertexIndices);

          const glm::vec3 lightToObject =
            glm::normalize(glm::vec3(mesh->shadowCoords()[triangle.mVertexIndices[0]]) - lightPositionInModelSpace);
          triangle.mIsFacingLight = glm::dot(lightToObject, normal) < 0.0f;
          if (triangle.mIsFacingLight) {
            const unsigned long long index0 = static_cast<unsigned long long>(triangle.mVertexIndices[0]);
            const unsigned long long index1 = static_cast<unsigned long long>(triangle.mVertexIndices[1]);
            const unsigned long long index2 = static_cast<unsigned long long>(triangle.mVertexIndices[2]);

            data[idx++] = index0 | (index1 << stride);
            data[idx++] = index2 | ((index2 + 1) << stride);
            data[idx++] = (index1 + 1) | ((index0 + 1) << stride);
          }
        }
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        glstate::releaseElementArrayBuffer(shadowVolume.mGlNameCapsIndexBuffer);
        shadowVolume.mIndexCountCaps = idx * 2;
      } else {
        // Not computing caps, light facing info for triangles connected to edges is sufficient
        for (const Mesh::Edge &edge : mesh->edges()) {
          const Mesh::Triangle &triangle0 = mesh->triangle(edge.mTriangleIndices[0]);
          glm::vec3 triangleNormal = triangle0.mNormal;
          if (mesh->isDynamic())
            triangleNormal = computeNormal(mesh, triangle0.mVertexIndices);

          triangle0.mIsFacingLight =
            glm::dot(glm::vec3(mesh->shadowCoords()[triangle0.mVertexIndices[0]]) - lightPositionInModelSpace, triangleNormal) <
            0.0f;

          // Special handling for edges only connected to a single triangle (non-closed meshes)
          if (edge.mTriangleIndices[1] != static_cast<size_t>(~0)) {
            const Mesh::Triangle &triangle1 = mesh->triangle(edge.mTriangleIndices[1]);
            glm::vec3 normal = triangle1.mNormal;
            if (mesh->isDynamic())
              normal = computeNormal(mesh, triangle1.mVertexIndices);

            triangle1.mIsFacingLight =
              glm::dot(glm::vec3(mesh->shadowCoords()[triangle1.mVertexIndices[0]]) - lightPositionInModelSpace, normal) < 0.0f;
          }
        }
      }

      shadowVolume.mIndexCountSides = 0;
      glstate::bindElementArrayBuffer(shadowVolume.mGlNameSidesIndexBuffer);
      const size_t maxSize = 3 * mesh->edges().size() * sizeof(unsigned long long);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxSize, NULL, GL_STREAM_DRAW);

      // Emscripten only accept the GL_MAP_INVALIDATE_BUFFER_BIT option
#ifdef __EMSCRIPTEN__
      unsigned long long *data = static_cast<unsigned long long *>(
        glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, maxSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
#else
      unsigned long long *data = static_cast<unsigned long long *>(
        glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, maxSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
#endif

      size_t idx = 0;
      const int stride = 8 * sizeof(unsigned int);
      for (const Mesh::Edge &edge : mesh->edges()) {
        const bool isTriangle0FacingLight = mesh->triangle(edge.mTriangleIndices[0]).mIsFacingLight;

        // Special handling for edges only connected to a single triangle (non-closed meshes)
        bool isTriangle1FacingLight = false;
        if (edge.mTriangleIndices[1] != static_cast<size_t>(~0))
          isTriangle1FacingLight = mesh->triangle(edge.mTriangleIndices[1]).mIsFacingLight;

        if (isTriangle0FacingLight != isTriangle1FacingLight) {
          // For point- and spotlights a quad is needed. Three out of six vertices will be extruded in the vertex shader.
          const unsigned long long index0 =
            static_cast<unsigned long long>(isTriangle0FacingLight ? edge.mVertexIndices[1] : edge.mVertexIndices[0]);
          const unsigned long long index1 =
            static_cast<unsigned long long>(isTriangle0FacingLight ? edge.mVertexIndices[0] : edge.mVertexIndices[1]);

          data[idx++] = (index0) | (index1 << stride);
          data[idx++] = ((index1 + 1)) | ((index1 + 1) << stride);
          data[idx++] = ((index0 + 1)) | (index0 << stride);
        }
      }
      glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      glstate::releaseElementArrayBuffer(shadowVolume.mGlNameSidesIndexBuffer);
      shadowVolume.mIndexCountSides = idx * 2;
    }

    shadowVolume.mIsDirty = false;
  }

  void ShadowVolumeCaster::renderSides(LightNode *light) const {
    auto itShadowVolume = mShadowVolumes.find(light);
    assert(itShadowVolume != mShadowVolumes.end());
    const ShadowVolume &shadowVolume = itShadowVolume->second;

    // Shadow volume is bound before testing for shadow because dynamic mesh shadows are updated at binding
    mRenderable->mesh()->bindShadowVolume();
    if (!shadowVolume.mIndexCountSides)
      return;

    glstate::bindElementArrayBuffer(shadowVolume.mGlNameSidesIndexBuffer);
    glDrawElements(GL_TRIANGLES, shadowVolume.mIndexCountSides, GL_UNSIGNED_INT, reinterpret_cast<void *>(0));
    if (config::requiresFlushAfterDraw())
      glFlush();
    glstate::releaseElementArrayBuffer(shadowVolume.mGlNameSidesIndexBuffer);
  }

  void ShadowVolumeCaster::renderCaps(LightNode *light) const {
    auto itShadowVolume = mShadowVolumes.find(light);
    assert(itShadowVolume != mShadowVolumes.end());
    const ShadowVolume &shadowVolume = itShadowVolume->second;

    mRenderable->mesh()->bindShadowVolume();
    if (!shadowVolume.mIndexCountCaps)
      return;

    glstate::bindElementArrayBuffer(shadowVolume.mGlNameCapsIndexBuffer);

    // avoid z-fighting between actual geometry and light cap (shouldn't be necessary?)
    glstate::setPolygonOffset(true, 0.1f, 5.0f);
    glDrawElements(GL_TRIANGLES, shadowVolume.mIndexCountCaps, GL_UNSIGNED_INT, reinterpret_cast<void *>(0));
    if (config::requiresFlushAfterDraw())
      glFlush();
    glstate::setPolygonOffset(false, 0.1f, 5.0f);

    glstate::releaseElementArrayBuffer(shadowVolume.mGlNameCapsIndexBuffer);
  }

}  // namespace wren
