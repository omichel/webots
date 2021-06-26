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

#include "Renderable.hpp"

#include "Config.hpp"
#include "Debug.hpp"
#include "DirectionalLight.hpp"
#include "DynamicMesh.hpp"
#include "GlState.hpp"
#include "Material.hpp"
#include "PointLight.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "ShadowVolumeCaster.hpp"
#include "SpotLight.hpp"
#include "StaticMesh.hpp"
#include "Transform.hpp"
#include "UniformBuffer.hpp"

#include <wren/renderable.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  const char *Renderable::cUseMaterialName = NULL;

  void Renderable::setMesh(Mesh *mesh) {
    if (mesh == mMesh)
      return;

    mMesh = mesh;
    setBoundingVolumeDirty();
    setCastShadows(mCastShadows);
  }

  void Renderable::setCastShadows(bool castShadows) {
    if (mMesh && !mMesh->supportShadows())
      castShadows = false;

    mCastShadows = castShadows;
    updateShadowVolumeCaster();
  }

  const glm::mat4 &Renderable::parentMatrix() const { return mParent->matrix(); }

  Material *Renderable::optionalMaterial(const std::string &name) const {
    const auto it = mOptionalMaterials.find(name);
    if (it == mOptionalMaterials.end())
      return NULL;
    else
      return it->second;
  }

  bool Renderable::zSortedRendering() const { return mZSortedRendering || mDefaultMaterial->isTranslucent(); }

  void Renderable::render(const ShaderProgram *program) {
    if (!mEffectiveMaterial)
      return;

    assert(mMesh);

    // Only bind material program if no override program has been specified
    mEffectiveMaterial->bind(!program);

    const ShaderProgram *effectiveProgram = program ? program : mEffectiveMaterial->effectiveProgram();

    setupAndRender(effectiveProgram);
  }

  void Renderable::renderWithoutMaterial(const ShaderProgram *program) {
    assert(program);
    assert(mMesh);

    program->bind();

    setupAndRender(program);
  }

  void Renderable::recomputeBoundingSphereInViewSpace(const glm::mat4 &viewMatrix) {
    mBoundingSphereInViewSpace = boundingSphere();

    if (!mInViewSpace)
      mBoundingSphereInViewSpace.mCenter = viewMatrix * glm::vec4(mBoundingSphere.mCenter, 1.0f);
  }

  void Renderable::recomputeAabb() const {
    if (!mParent)
      return;

    if (!mMesh) {
      Node::recomputeAabb();
      return;
    }

    mAabb = mMesh->recomputeAabb(mParent->scale());
    static std::vector<glm::vec3> vertices(8);
    vertices = mAabb.vertices();

    for (glm::vec3 &vertex : vertices) {
      vertex = mParent->orientation() * vertex;
      vertex += mParent->position();
    }

    mAabb = primitive::Aabb(vertices);
  }

  void Renderable::recomputeBoundingSphere() const {
    if (!mParent)
      return;

    if (!mMesh) {
      Node::recomputeBoundingSphere();
      return;
    }

    mBoundingSphere = mMesh->recomputeBoundingSphere(mParent->scale());
    mBoundingSphere.mCenter = mParent->orientation() * mBoundingSphere.mCenter;
    mBoundingSphere.mCenter += mParent->position();
  }

  bool Renderable::isTranslucent() const {
    if (!mDefaultMaterial)
      return false;

    return mDefaultMaterial->isTranslucent();
  }

  size_t Renderable::sortingId() const {
    return (mFaceCulling ? 1 : 0) | (mMesh->sortingId() << 1) | (mDefaultMaterial->sortingId() << 16);
  }

  void Renderable::updateFromParent() {
    if (!isVisible() || !mMesh)
      return;

    if (Renderable::cUseMaterialName) {
      const auto it = mOptionalMaterials.find(Renderable::cUseMaterialName);
      if (it == mOptionalMaterials.end())
        mEffectiveMaterial = NULL;
      else
        mEffectiveMaterial = it->second;
    } else
      mEffectiveMaterial = mDefaultMaterial;

    Scene::instance()->enqueueRenderable(this);
  }

  const primitive::Aabb &Renderable::aabb() {
    if (mMesh->isAabbDirty())
      recomputeAabb();

    return Node::aabb();
  }

  const primitive::Sphere &Renderable::boundingSphere() const {
    if (mMesh->isBoundingSphereDirty())
      recomputeBoundingSphere();

    return Node::boundingSphere();
  }

  void Renderable::setMatrixDirty() const {
    Node::setMatrixDirty();

    if (mShadowVolumeCaster)
      mShadowVolumeCaster->notifyRenderableDirty();
  }

  Renderable::Renderable() :
    mDefaultMaterial(NULL),
    mEffectiveMaterial(NULL),
    mOptionalMaterials(),
    mMesh(NULL),
    mShadowVolumeCaster(NULL),
    mDrawingMode(WR_RENDERABLE_DRAWING_MODE_TRIANGLES),
    mDrawingOrder(WR_RENDERABLE_DRAWING_ORDER_MAIN),
    mVisibilityFlags(0xFFFFFFFF),
    mCastShadows(true),
    mReceiveShadows(true),
    mSceneCulling(true),
    mInViewSpace(false),
    mZSortedRendering(false),
    mFaceCulling(true),
    mPointSize(-1.0f) {}

  Renderable::~Renderable() { delete mShadowVolumeCaster; }

  void Renderable::setupAndRender(const ShaderProgram *program) {
    // Few Renderables use premultiplied alpha, if this is the case then
    // save current blend state and restore it after rendering
    const unsigned int blendSrcFactor = glstate::blendSrcFactor();
    const unsigned int blendDestFactor = glstate::blendDestFactor();
    if (mDefaultMaterial->hasPremultipliedAlpha())
      glstate::setBlendFunc(GL_ONE, blendDestFactor);

    if (config::isPointSizeEnabled() && mPointSize > 0.0f)
      glUniform1f(program->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_POINT_SIZE), mPointSize);

    glstate::setCullFace(mFaceCulling);

    glUniformMatrix4fv(program->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM), 1, false,
                       glm::value_ptr(mParent->matrix()));

    mMesh->render(mDrawingMode);

    if (mDefaultMaterial->hasPremultipliedAlpha())
      glstate::setBlendFunc(blendSrcFactor, blendDestFactor);
  }

  void Renderable::updateShadowVolumeCaster() {
    delete mShadowVolumeCaster;

    if (mMesh && mCastShadows)
      mShadowVolumeCaster = new ShadowVolumeCaster(this);
    else
      mShadowVolumeCaster = NULL;

    // In case the mesh is dynamic, it needs the shadow volume to recompute the silhouette when required
    if (mMesh && mMesh->isDynamic())
      dynamic_cast<DynamicMesh *>(mMesh)->setShadowVolume(mShadowVolumeCaster);
  }

}  // namespace wren

// C interface implementation
WrRenderable *wr_renderable_new() {
  return reinterpret_cast<WrRenderable *>(wren::Renderable::createRenderable());
}

void wr_renderable_set_mesh(WrRenderable *renderable, WrMesh *mesh) {
  reinterpret_cast<wren::Renderable *>(renderable)->setMesh(reinterpret_cast<wren::Mesh *>(mesh));
}

void wr_renderable_set_material(WrRenderable *renderable, WrMaterial *material, const char *name) {
  if (material) {
    if (material->type == WR_MATERIAL_PHONG) {
      if (!name)
        reinterpret_cast<wren::Renderable *>(renderable)
          ->setDefaultMaterial(reinterpret_cast<wren::PhongMaterial *>(material->data));
      else
        reinterpret_cast<wren::Renderable *>(renderable)
          ->setOptionalMaterial(std::string(name), reinterpret_cast<wren::PhongMaterial *>(material->data));
    } else {
      if (!name)
        reinterpret_cast<wren::Renderable *>(renderable)
          ->setDefaultMaterial(reinterpret_cast<wren::PbrMaterial *>(material->data));
      else
        reinterpret_cast<wren::Renderable *>(renderable)
          ->setOptionalMaterial(std::string(name), reinterpret_cast<wren::PbrMaterial *>(material->data));
    }
  }
}

void wr_renderable_set_drawing_mode(WrRenderable *renderable, WrRenderableDrawingMode drawing_mode) {
  reinterpret_cast<wren::Renderable *>(renderable)->setDrawingMode(drawing_mode);
}

void wr_renderable_set_visibility_flags(WrRenderable *renderable, int flags) {
  reinterpret_cast<wren::Renderable *>(renderable)->setVisibilityFlags(flags);
}

void wr_renderable_set_cast_shadows(WrRenderable *renderable, bool cast_shadows) {
  reinterpret_cast<wren::Renderable *>(renderable)->setCastShadows(cast_shadows);
}

void wr_renderable_set_receive_shadows(WrRenderable *renderable, bool receive_shadows) {
  reinterpret_cast<wren::Renderable *>(renderable)->setReceiveShadows(receive_shadows);
}

void wr_renderable_set_scene_culling(WrRenderable *renderable, bool culling) {
  reinterpret_cast<wren::Renderable *>(renderable)->setSceneCulling(culling);
}

void wr_renderable_set_face_culling(WrRenderable *renderable, bool face_culling) {
  reinterpret_cast<wren::Renderable *>(renderable)->setFaceCulling(face_culling);
}

void wr_renderable_set_in_view_space(WrRenderable *renderable, bool in_view_space) {
  reinterpret_cast<wren::Renderable *>(renderable)->setInViewSpace(in_view_space);
}

void wr_renderable_set_z_sorted_rendering(WrRenderable *renderable, bool z_sorted) {
  reinterpret_cast<wren::Renderable *>(renderable)->setZSortedRendering(z_sorted);
}

WrMaterial *wr_renderable_get_material(WrRenderable *renderable, const char *name) {
  if (!name) {
    wren::Material *defaultMaterial = reinterpret_cast<wren::Renderable *>(renderable)->defaultMaterial();
    return defaultMaterial->materialStructure();
  } else {
    wren::Material *optionalMaterial = reinterpret_cast<wren::Renderable *>(renderable)->optionalMaterial(std::string(name));
    if (optionalMaterial)
      return optionalMaterial->materialStructure();
    else
      return NULL;
  }
}

void wr_renderable_get_bounding_sphere(WrRenderable *renderable, float *sphere) {
  const wren::primitive::Sphere s = reinterpret_cast<wren::Renderable *>(renderable)->boundingSphere();
  sphere[0] = s.mCenter.x;
  sphere[1] = s.mCenter.y;
  sphere[2] = s.mCenter.z;
  sphere[3] = s.mRadius;
}

void wr_renderable_set_drawing_order(WrRenderable *renderable, WrRenderableDrawingOrder order) {
  reinterpret_cast<wren::Renderable *>(renderable)->setDrawingOrder(order);
}

void wr_renderable_set_point_size(WrRenderable *renderable, float point_size) {
  reinterpret_cast<wren::Renderable *>(renderable)->setPointSize(point_size);
}
