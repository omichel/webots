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

#include "DynamicMesh.hpp"

#include "Config.hpp"
#include "Debug.hpp"
#include "GlState.hpp"
#include "Id.hpp"

#include <wren/dynamic_mesh.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <unordered_map>

namespace wren {

  void DynamicMesh::addCoord(const glm::vec3 &coord, bool forceShadowCoords) {
    Mesh::addCoord(coord);

    mCoordsDirty = true;
    mAabbDirty = true;
    mAabbComputed = false;
    mBoundingSphereDirty = true;

    if (mShadowVolume || forceShadowCoords) {
      mShadowCoords.push_back(glm::vec4(coord, 1.0f));
      mShadowCoords.push_back(glm::vec4(coord, 0.0f));
      mShadowCoordsDirty = true;
    }
  }

  void DynamicMesh::addNormal(const glm::vec3 &normal) {
    Mesh::addNormal(normal);
    mNormalsDirty = true;
  }

  void DynamicMesh::addTexCoord(const glm::vec2 &texCoord) {
    Mesh::addTexCoord(texCoord);
    mTexCoordsDirty = true;
  }

  void DynamicMesh::addIndex(unsigned int index) {
    Mesh::addIndex(index);
    mIndicesDirty = true;
    mShadowIndicesDirty = true;
  }

  void DynamicMesh::addColor(const glm::vec3 &color) {
    Mesh::addColor(color);
    mColorsDirty = true;
  }

  DynamicMesh::DynamicMesh(bool hasNormals, bool hasTextureCoordinates, bool hasColorPerVertex) :
    mHasNormals(hasNormals),
    mHasTextureCoordinates(hasTextureCoordinates),
    mHasColorPerVertex(hasColorPerVertex),
    mCoordsDirty(false),
    mIndicesDirty(false),
    mNormalsDirty(false),
    mTexCoordsDirty(false),
    mColorsDirty(false),
    mAabbDirty(false),
    mAabbComputed(false),
    mBoundingSphereDirty(false),
    mSkeletonDirty(true),
    mSupportShadows(true),
    mShadowCoordsDirty(false),
    mShadowIndicesDirty(false),
    mGlNameVertexArrayObject(0),
    mGlNameBufferIndices(0),
    mGlNameBufferCoords(0),
    mGlNameBufferNormals(0),
    mGlNameBufferTexCoords(0),
    mGlNameBufferColors(0),
    mGlNameVertexArrayObjectShadow(0),
    mGlNameBufferShadowCoords(0),
    mShadowVolume(NULL),
    mSkeleton(NULL) {
    Mesh::setup();
  }

  void DynamicMesh::bind() {
    glstate::bindVertexArrayObject(mGlNameVertexArrayObject);
    // Indices should be part VAO state, but this doesn't seem to be the case for all
    // drivers, so we need to bind the buffer manually to be sure
    glstate::bindElementArrayBuffer(mGlNameBufferIndices);

    updateGl();
  }

  void DynamicMesh::release() {
    glstate::releaseVertexArrayObject(mGlNameVertexArrayObject);
    glstate::releaseElementArrayBuffer(mGlNameBufferIndices);
  }

  void DynamicMesh::bindShadowVolume() {
    glstate::bindVertexArrayObject(mGlNameVertexArrayObjectShadow);

    updateGlShadow();
  }

  void DynamicMesh::releaseShadowVolume() { glstate::releaseVertexArrayObject(mGlNameVertexArrayObjectShadow); }

  void DynamicMesh::render(unsigned int drawingMode) {
    bind();
    glDrawElements(drawingMode, mIndices.size(), GL_UNSIGNED_INT, NULL);
    if (config::requiresFlushAfterDraw())
      glFlush();
  }

  void DynamicMesh::clear(bool vertices, bool normals, bool textureCoordinates, bool colors) {
    if (vertices) {
      if (mCoords.size() > 0) {
        mCoords.clear();
        mCoordsDirty = true;
      }
      if (mShadowCoords.size() > 0) {
        mShadowCoords.clear();
        mSupportShadows = true;
      }
    }

    if (normals && mNormals.size() > 0) {
      mNormals.clear();
      mNormalsDirty = true;
    }

    if (textureCoordinates && mTexCoords.size() > 0) {
      mTexCoords.clear();
      mTexCoordsDirty = true;
    }

    if (colors && mColors.size() > 0) {
      mColors.clear();
      mColorsDirty = true;
    }
  }

  size_t DynamicMesh::sortingId() const { return mMeshId.id(); }

  primitive::Aabb DynamicMesh::recomputeAabb(const glm::vec3 &scale) {
    if (!mCoords.size())
      mAabb = primitive::Aabb();
    else if (mAabbDirty && !mAabbComputed)
      mAabb = primitive::Aabb(mCoords);

    mAabbDirty = false;

    const glm::vec3 center = scale * 0.5f * (mAabb.mBounds[0] + mAabb.mBounds[1]);
    const glm::vec3 extents = scale * 0.5f * (mAabb.mBounds[1] - mAabb.mBounds[0]);

    return primitive::Aabb(center - extents, center + extents);
  }

  primitive::Sphere DynamicMesh::recomputeBoundingSphere(const glm::vec3 &scale) {
    if (!mCoords.size())
      mBoundingSphere = primitive::Sphere();
    else if (mBoundingSphereDirty) {
      if (mAabbDirty && !mAabbComputed)
        mBoundingSphere = primitive::computeBoundingSphereFromVertices(mCoords);
      else
        mBoundingSphere = primitive::computeBoundingSphereFromVertices(mAabb.vertices());
    }

    mBoundingSphereDirty = false;

    const glm::vec3 center = mBoundingSphere.mCenter * scale;
    const float radius = mBoundingSphere.mRadius * std::max(std::max(scale.x, scale.y), scale.z);

    return primitive::Sphere(center, radius);
  }

  void DynamicMesh::applySkeletonOffset() {
    if (!mSkeleton)
      return;

    mSkinCoords.clear();
    mSkinNormals.clear();
    mSkinCoords.reserve(mCoords.size());
    mSkinNormals.reserve(mNormals.size());

    const glm::mat4 skeletonMatrix = mSkeleton->matrix();
    for (size_t i = 0; i < mCoords.size(); ++i) {
      mCoordsCopy.push_back(mCoords[i]);
      mSkinCoords.push_back(skeletonMatrix * glm::vec4(mCoords[i], 1.0f));

      if (mHasNormals) {
        mNormalsCopy.push_back(mNormals[i]);
        mSkinNormals.push_back(skeletonMatrix * glm::vec4(mNormals[i], 0.0f));
      }
    }

    mBoundingSphereDirty = true;
    mAabbComputed = false;
    mAabbDirty = true;
    mCoordsDirty = true;
    mNormalsDirty = true;
    mSkeletonDirty = true;
  }

  void DynamicMesh::updateSkeletonOffset(const glm::mat4 &offsetMatrix) {
    if (!mSkeleton)
      return;

    mSkinCoords.clear();
    mSkinCoords.reserve(mCoords.size());
    mSkinNormals.clear();
    mSkinNormals.reserve(mNormals.size());

    for (size_t i = 0; i < mCoords.size(); ++i) {
      mSkinCoords.push_back(offsetMatrix * glm::vec4(mCoordsCopy[i], 1.0f));

      if (mHasNormals)
        mSkinNormals.push_back(offsetMatrix * glm::vec4(mNormalsCopy[i], 0.0f));
    }

    mBoundingSphereDirty = true;
    mAabbComputed = false;
    mAabbDirty = true;
    mCoordsDirty = true;
    mNormalsDirty = true;
    mSkeletonDirty = true;
  }

  void DynamicMesh::applySkeletonTransform() {
    if (!mSkeleton || !mSkeletonDirty)
      return;

    glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferCoords);
    glBufferData(GL_ARRAY_BUFFER, mCoords.size() * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
    float *vertexData = static_cast<float *>(
      glMapBufferRange(GL_ARRAY_BUFFER, 0, mCoords.size() * sizeof(glm::vec3), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));

    float *normalData = NULL;
    if (mHasNormals) {
      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferNormals);
      glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
      normalData = static_cast<float *>(glMapBufferRange(GL_ARRAY_BUFFER, 0, mNormals.size() * sizeof(glm::vec3),
                                                         GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
    }

    float *shadowData = NULL;
    if (mShadowVolume) {
      mShadowVolume->notifyRenderableDirty();

      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferShadowCoords);
      glBufferData(GL_ARRAY_BUFFER, mShadowCoords.size() * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
      shadowData = static_cast<float *>(glMapBufferRange(GL_ARRAY_BUFFER, 0, mShadowCoords.size() * sizeof(glm::vec4),
                                                         GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT));
    }

    mAabb = gAabbEmpty;
    for (size_t i = 0; i < mCoords.size(); ++i) {
      const glm::mat4 matrix = mSkeleton->vertexMatrix(this, i);
      mCoords[i] = matrix * glm::vec4(mSkinCoords[i], 1.0f);

      const size_t index = i * 3;
      vertexData[index] = mCoords[i][0];
      vertexData[index + 1] = mCoords[i][1];
      vertexData[index + 2] = mCoords[i][2];

      if (mHasNormals) {
        mNormals[i] = matrix * glm::vec4(mSkinNormals[i], 0.0f);
        normalData[index] = mNormals[i][0];
        normalData[index + 1] = mNormals[i][1];
        normalData[index + 2] = mNormals[i][2];
      }

      if (mShadowVolume) {
        mShadowCoords[2 * i] = glm::vec4(mCoords[i], 1.0f);
        mShadowCoords[2 * i + 1] = glm::vec4(mCoords[i], 0.0f);

        const size_t shadowIndex = i * 8;
        shadowData[shadowIndex] = mCoords[i][0];
        shadowData[shadowIndex + 1] = mCoords[i][1];
        shadowData[shadowIndex + 2] = mCoords[i][2];
        shadowData[shadowIndex + 3] = 1.0f;

        shadowData[shadowIndex + 4] = mCoords[i][0];
        shadowData[shadowIndex + 5] = mCoords[i][1];
        shadowData[shadowIndex + 6] = mCoords[i][2];
        shadowData[shadowIndex + 7] = 0.0f;
      }
      mAabb.extend(mCoords[i]);
    }

    if (mShadowVolume) {
      glUnmapBuffer(GL_ARRAY_BUFFER);
      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferNormals);
      mShadowCoordsDirty = false;
    }

    if (mHasNormals) {
      glUnmapBuffer(GL_ARRAY_BUFFER);
      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferCoords);
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mBoundingSphereDirty = true;
    mAabbComputed = true;
    mAabbDirty = true;
    mCoordsDirty = false;
    mNormalsDirty = false;
    mSkeletonDirty = false;
  }

  void DynamicMesh::prepareGl() {
    // DEBUG("DynamicMesh::prepareGl, this=" << this << ", VAO=" << mGlNameVertexArrayObject <<
    //       "Normals: " << (mHasNormals ? "Yes" : "No") << " Texture coordinates: " << (mHasTextureCoordinates ? "Yes" :
    //       "No"));

    glGenVertexArrays(1, &mGlNameVertexArrayObject);
    glGenBuffers(1, &mGlNameBufferIndices);
    glGenBuffers(1, &mGlNameBufferCoords);

    // Init buffers
    glstate::bindVertexArrayObject(mGlNameVertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferCoords);
    glVertexAttribPointer(GlslLayout::gLocationCoords, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
    glEnableVertexAttribArray(GlslLayout::gLocationCoords);

    if (mHasNormals) {
      glGenBuffers(1, &mGlNameBufferNormals);
      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferNormals);
      glVertexAttribPointer(GlslLayout::gLocationNormals, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
      glEnableVertexAttribArray(GlslLayout::gLocationNormals);
    }

    if (mHasTextureCoordinates) {
      glGenBuffers(1, &mGlNameBufferTexCoords);
      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferTexCoords);
      glVertexAttribPointer(GlslLayout::gLocationTexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), NULL);
      glEnableVertexAttribArray(GlslLayout::gLocationTexCoords);
    }

    if (mHasColorPerVertex) {
      glGenBuffers(1, &mGlNameBufferColors);
      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferColors);
      glVertexAttribPointer(GlslLayout::gLocationColors, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
      glEnableVertexAttribArray(GlslLayout::gLocationColors);
    }

    // Shadow volume
    glGenVertexArrays(1, &mGlNameVertexArrayObjectShadow);
    glGenBuffers(1, &mGlNameBufferShadowCoords);

    glstate::bindVertexArrayObject(mGlNameVertexArrayObjectShadow);
    glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferShadowCoords);
    glVertexAttribPointer(GlslLayout::gLocationCoords, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), NULL);
    glEnableVertexAttribArray(GlslLayout::gLocationCoords);

    // Release
    glstate::bindVertexArrayObject(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glstate::bindElementArrayBuffer(0);
  }

  void DynamicMesh::cleanupGl() {
    release();

    glDeleteVertexArrays(1, &mGlNameVertexArrayObject);
    glDeleteBuffers(1, &mGlNameBufferCoords);
    glDeleteBuffers(1, &mGlNameBufferIndices);

    if (mHasNormals && mGlNameBufferNormals)
      glDeleteBuffers(1, &mGlNameBufferNormals);

    if (mHasTextureCoordinates && mGlNameBufferTexCoords)
      glDeleteBuffers(1, &mGlNameBufferTexCoords);

    if (mHasColorPerVertex && mGlNameBufferColors)
      glDeleteBuffers(1, &mGlNameBufferColors);

    glDeleteVertexArrays(1, &mGlNameVertexArrayObjectShadow);
    glDeleteBuffers(1, &mGlNameBufferShadowCoords);
  }

  void DynamicMesh::updateGl() {
    applySkeletonTransform();

    if (mIndicesDirty) {
      // Indices buffer is set to GL_STATIC_DRAW as we expect the mesh topology not to change
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), NULL, GL_STATIC_DRAW);
      void *data = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, mIndices.size() * sizeof(unsigned int),
                                    GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
      memcpy(data, &mIndices[0], mIndices.size() * sizeof(unsigned int));
      glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      mIndicesDirty = false;

      // Special case for first rendering
      if (mTriangles.size() == 0 && config::areShadowsEnabled() &&
          mCoords.size() < config::maxVerticesPerMeshForShadowRendering()) {
        computeTrianglesAndEdges();
        mShadowIndicesDirty = false;
        mSupportShadows = true;
      } else
        mSupportShadows = false;
    }

    if (mCoordsDirty) {
      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferCoords);
      glBufferData(GL_ARRAY_BUFFER, mCoords.size() * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
      void *data =
        glMapBufferRange(GL_ARRAY_BUFFER, 0, mCoords.size() * sizeof(glm::vec3), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
      memcpy(data, &mCoords[0], mCoords.size() * sizeof(glm::vec3));
      glUnmapBuffer(GL_ARRAY_BUFFER);
      mCoordsDirty = false;
    }

    if (mHasNormals && mNormalsDirty) {
      assert(mNormals.size() == mCoords.size());

      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferNormals);
      glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
      void *data =
        glMapBufferRange(GL_ARRAY_BUFFER, 0, mNormals.size() * sizeof(glm::vec3), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
      memcpy(data, &mNormals[0], mNormals.size() * sizeof(glm::vec3));
      glUnmapBuffer(GL_ARRAY_BUFFER);
      mNormalsDirty = false;
    }

    if (mHasTextureCoordinates && mTexCoordsDirty) {
      assert(mTexCoords.size() == mCoords.size());

      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferTexCoords);
      glBufferData(GL_ARRAY_BUFFER, mTexCoords.size() * sizeof(glm::vec2), NULL, GL_STREAM_DRAW);
      void *data = glMapBufferRange(GL_ARRAY_BUFFER, 0, mTexCoords.size() * sizeof(glm::vec2),
                                    GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
      memcpy(data, &mTexCoords[0], mTexCoords.size() * sizeof(glm::vec2));
      glUnmapBuffer(GL_ARRAY_BUFFER);
      mTexCoordsDirty = false;
    }

    if (mHasColorPerVertex && mColorsDirty) {
      assert(mCoords.size() == mColors.size());

      glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferColors);
      glBufferData(GL_ARRAY_BUFFER, mColors.size() * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
      void *data =
        glMapBufferRange(GL_ARRAY_BUFFER, 0, mColors.size() * sizeof(glm::vec3), GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
      memcpy(data, &mColors[0], mColors.size() * sizeof(glm::vec3));
      glUnmapBuffer(GL_ARRAY_BUFFER);
      mColorsDirty = false;
    }
  }

  void DynamicMesh::updateGlShadow() {
    // Update shadow volume
    if (mShadowVolume && config::areShadowsEnabled()) {
      if (mShadowIndicesDirty || mTriangles.size() == 0) {
        mTriangles.clear();
        mEdges.clear();
        if (mCoords.size() < config::maxVerticesPerMeshForShadowRendering()) {
          computeTrianglesAndEdges();
          mSupportShadows = true;
        } else
          mSupportShadows = false;
        mShadowIndicesDirty = false;
      }

      if (mShadowCoordsDirty) {
        mShadowVolume->notifyRenderableDirty();

        glBindBuffer(GL_ARRAY_BUFFER, mGlNameBufferShadowCoords);
        glBufferData(GL_ARRAY_BUFFER, mShadowCoords.size() * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
        void *data = glMapBufferRange(GL_ARRAY_BUFFER, 0, mShadowCoords.size() * sizeof(glm::vec4),
                                      GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
        memcpy(data, &mShadowCoords[0], mShadowCoords.size() * sizeof(glm::vec4));
        glUnmapBuffer(GL_ARRAY_BUFFER);
        mShadowCoordsDirty = false;
      }
    }
  }

  static void createOrCompleteEdge(size_t triangleIndex, unsigned int vertexIndex0, unsigned int vertexIndex1,
                                   std::unordered_map<std::pair<size_t, size_t>, Mesh::Edge> &edgeMap,
                                   std::vector<Mesh::Triangle> &triangles, std::vector<Mesh::Edge> &edges) {
    // Either add the edge to the edge map if it isn't present, or complete
    // an existing edge if it is already present in the edge map.
    auto itEdgeInverse = edgeMap.find(std::make_pair(vertexIndex1, vertexIndex0));
    if (itEdgeInverse == edgeMap.end()) {
      // If the same edge already exists in the map, add it to the edge list as a non-shared edge
      // before adding this edge to the edge map.
      auto itEdge = edgeMap.find(std::make_pair(vertexIndex0, vertexIndex1));
      if (itEdge != edgeMap.end()) {
        edges.push_back(itEdge->second);
        edgeMap.erase(itEdge);
      }
      edgeMap.emplace(std::make_pair(vertexIndex0, vertexIndex1), Mesh::Edge(triangleIndex, ~0, vertexIndex0, vertexIndex1));
    } else {
      itEdgeInverse->second.mTriangleIndices[1] = triangleIndex;
      edges.push_back(itEdgeInverse->second);
      edgeMap.erase(itEdgeInverse);
    }
  }

  void DynamicMesh::computeTrianglesAndEdges() {
    // only triangle-list meshes are allowed to cast shadows
    if (mIndices.size() % 3 != 0)
      return;

    // Merge indices
    std::unordered_map<glm::vec3, unsigned int> coordIndexMap;
    for (size_t i = 0; i < mIndices.size(); ++i) {
      const glm::vec3 coord = mCoords[mIndices[i]];
      auto itCoordIndex = coordIndexMap.find(coord);
      if (itCoordIndex == coordIndexMap.end())
        coordIndexMap[coord] = mIndices[i];
      else
        mIndices[i] = itCoordIndex->second;
    }

    const size_t triangleCount = mIndices.size() / 3;
    mTriangles.reserve(triangleCount);
    mEdges.reserve(triangleCount);
    std::unordered_map<std::pair<size_t, size_t>, Mesh::Edge> edgeMap;
    // Dynamic mesh triangle normals are recomputed afterwards, during silhouette computation
    const glm::vec3 normal(0, 0, 0);
    for (size_t startingIndex = 0, triangleIndex = 0; triangleIndex < triangleCount; ++triangleIndex) {
      const size_t index0 = 2 * mIndices[startingIndex++];
      const size_t index1 = 2 * mIndices[startingIndex++];
      const size_t index2 = 2 * mIndices[startingIndex++];

      size_t currentTriangleIndex = mTriangles.size();
      mTriangles.push_back(Mesh::Triangle(index0, index1, index2, normal));

      createOrCompleteEdge(currentTriangleIndex, index0, index1, edgeMap, mTriangles, mEdges);
      createOrCompleteEdge(currentTriangleIndex, index1, index2, edgeMap, mTriangles, mEdges);
      createOrCompleteEdge(currentTriangleIndex, index2, index0, edgeMap, mTriangles, mEdges);
    }

    for (const auto &it : edgeMap)
      mEdges.push_back(it.second);
  }

}  // namespace wren

// C interface implementation
WrDynamicMesh *wr_dynamic_mesh_new(bool normals, bool texture_coordinates, bool color_per_vertex) {
  return reinterpret_cast<WrDynamicMesh *>(
    wren::DynamicMesh::createDynamicMesh(normals, texture_coordinates, color_per_vertex));
}

void wr_dynamic_mesh_delete(WrDynamicMesh *mesh) {
  wren::Mesh::deleteMesh(reinterpret_cast<wren::DynamicMesh *>(mesh));
}

void wr_dynamic_mesh_clear(WrDynamicMesh *mesh) {
  reinterpret_cast<wren::DynamicMesh *>(mesh)->clear();
}

void wr_dynamic_mesh_clear_selected(WrDynamicMesh *mesh, bool vertices, bool normals, bool texture_coordinates, bool colors) {
  reinterpret_cast<wren::DynamicMesh *>(mesh)->clear(vertices, normals, texture_coordinates, colors);
}

void wr_dynamic_mesh_add_vertex(WrDynamicMesh *mesh, const float *vertex) {
  reinterpret_cast<wren::DynamicMesh *>(mesh)->addCoord(glm::make_vec3(vertex));
}

void wr_dynamic_mesh_add_normal(WrDynamicMesh *mesh, const float *normal) {
  reinterpret_cast<wren::DynamicMesh *>(mesh)->addNormal(glm::make_vec3(normal));
}

void wr_dynamic_mesh_add_texture_coordinate(WrDynamicMesh *mesh, const float *texture_coordinate) {
  reinterpret_cast<wren::DynamicMesh *>(mesh)->addTexCoord(glm::make_vec2(texture_coordinate));
}

void wr_dynamic_mesh_add_index(WrDynamicMesh *mesh, unsigned int index) {
  reinterpret_cast<wren::DynamicMesh *>(mesh)->addIndex(index);
}

void wr_dynamic_mesh_add_color(WrDynamicMesh *mesh, const float *color) {
  reinterpret_cast<wren::DynamicMesh *>(mesh)->addColor(glm::make_vec3(color));
}
