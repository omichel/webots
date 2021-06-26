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

#include "PhongMaterial.hpp"

#include "ColorUtils.hpp"
#include "Debug.hpp"
#include "GlState.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "TextureCubeMap.hpp"
#include "TextureTransform.hpp"
#include "UniformBuffer.hpp"

#include <wren/material.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  std::unordered_map<cache::Key, cache::PhongMaterialData> PhongMaterial::cCache;

  void PhongMaterial::setTexture(Texture *texture, size_t index) {
    Material::setTexture(texture, index);

    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    material.mTextureFlags[index] = texture ? 1.0f : 0.0f;
    updateMaterial(material);
  }

  size_t PhongMaterial::sortingId() const {
    const size_t programId = static_cast<size_t>(mDefaultProgram->glName());

    size_t textureId = 0;
    if (mTextures[0].first)
      textureId = static_cast<size_t>(mTextures[0].first->glName());

    return static_cast<size_t>(mCacheData->id() << 1) | (textureId << 16) | (programId << 32) |
           (mHasPremultipliedAlpha ? 1 : 0);
  }

  PhongMaterial *PhongMaterial::createMaterial() {
    PhongMaterial *material = new PhongMaterial();
    material->init();
    return material;
  }

  void PhongMaterial::deleteMaterial(PhongMaterial *material) {
    material->releaseMaterial();
    delete material;
  }

  void PhongMaterial::clearMaterial() {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);

    material.mAmbient = glm::vec4(gVec3Ones, 1.0f);
    material.mDiffuse = glm::vec4(gVec3Ones, 1.0f);
    material.mSpecularAndExponent = glm::vec4(gVec3Ones, 25.0f);
    material.mEmissiveAndOpacity = glm::vec4(gVec3Zeros, 1.0f);
    material.mTextureFlags = glm::vec4(0.0f);

    for (auto &texture : mTextures)
      texture = std::make_pair(nullptr, Texture::DEFAULT_USAGE_PARAMS);

    for (auto &textureCube : mTextureCubes)
      textureCube = std::make_pair(nullptr, Texture::DEFAULT_USAGE_PARAMS);

    mTextureTransform = NULL;

    updateMaterial(material);
  }

  void PhongMaterial::setColor(const glm::vec3 &color) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    glm::vec4 linearColor = colorutils::srgbToLinear(glm::vec4(color, 1.0f));
    material.mAmbient = linearColor;
    material.mDiffuse = linearColor;
    material.mEmissiveAndOpacity = glm::vec4(color, material.mEmissiveAndOpacity.w);
    material.mSpecularAndExponent = glm::vec4(color, material.mSpecularAndExponent.w);

    updateMaterial(material);
  }

  void PhongMaterial::setAmbient(const glm::vec3 &ambient, bool linearColor) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    material.mAmbient = linearColor ? glm::vec4(ambient, 1.0f) : colorutils::srgbToLinear(glm::vec4(ambient, 1.0f));

    updateMaterial(material);
  }

  void PhongMaterial::setDiffuse(const glm::vec3 &diffuse, bool linearColor) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    material.mDiffuse = linearColor ? glm::vec4(diffuse, 1.0f) : colorutils::srgbToLinear(glm::vec4(diffuse, 1.0f));

    updateMaterial(material);
  }

  void PhongMaterial::setSpecular(const glm::vec3 &specular) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    material.mSpecularAndExponent = colorutils::srgbToLinear(glm::vec4(specular, material.mSpecularAndExponent.w));

    updateMaterial(material);
  }

  void PhongMaterial::setEmissive(const glm::vec3 &emissive) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    material.mEmissiveAndOpacity = colorutils::srgbToLinear(glm::vec4(emissive, material.mEmissiveAndOpacity.w));

    updateMaterial(material);
  }

  void PhongMaterial::setShininess(float shininess) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    material.mSpecularAndExponent.w = shininess * 128.0f;

    updateMaterial(material);
  }

  void PhongMaterial::setSpecularExponent(float specularExponent) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    material.mSpecularAndExponent.w = specularExponent;

    updateMaterial(material);
  }

  void PhongMaterial::setTransparency(float transparency) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);
    material.mEmissiveAndOpacity.w = 1.0f - transparency;

    updateMaterial(material);
  }

  void PhongMaterial::setAllParameters(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                                       const glm::vec3 &emissive, float shininess, float transparency) {
    GlslLayout::PhongMaterial material(mCacheData->mMaterial);

    material.mAmbient = colorutils::srgbToLinear(glm::vec4(ambient, 1.0f));
    material.mDiffuse = colorutils::srgbToLinear(glm::vec4(diffuse, 1.0f));
    material.mSpecularAndExponent = colorutils::srgbToLinear(glm::vec4(specular, shininess * 128.0f));
    material.mEmissiveAndOpacity = colorutils::srgbToLinear(glm::vec4(emissive, 1.0f - transparency));

    updateMaterial(material);
  }

  void PhongMaterial::bind(bool bindProgram) const {
    if (bindProgram)
      Material::useProgram();

    assert(mCacheData);
    glstate::bindPhongMaterial(mCacheData);

    updateUniforms();
    Material::bindTextures();
  }

  PhongMaterial::PhongMaterial() : Material(), mColorPerVertex(false), mCacheData(NULL) {
    mMaterialStructure = new WrMaterial;
    mMaterialStructure->type = WR_MATERIAL_PHONG;
    mMaterialStructure->data = reinterpret_cast<void *>(this);
  }

  void PhongMaterial::init() {
    GlslLayout::PhongMaterial material;
    material.mAmbient = glm::vec4(gVec3Ones, 1.0f);
    material.mDiffuse = glm::vec4(gVec3Ones, 1.0f);
    material.mSpecularAndExponent = glm::vec4(gVec3Ones, 25.0f);
    material.mEmissiveAndOpacity = glm::vec4(gVec3Zeros, 1.0f);
    material.mTextureFlags = glm::vec4(0.0f);
    updateMaterial(material);
  }

  void PhongMaterial::updateMaterial(const GlslLayout::PhongMaterial &material) {
    // remove this instance from the set of users of the previous material
    auto it = PhongMaterial::cCache.find(mCacheKey);
    if (it != PhongMaterial::cCache.end())
      releaseMaterial();

    mCacheKey = cache::Key(cache::sipHash13c(reinterpret_cast<const char *>(&material), sizeof(GlslLayout::PhongMaterial)));

    it = PhongMaterial::cCache.find(mCacheKey);
    if (it != PhongMaterial::cCache.end()) {
      mCacheData = &it->second;
      ++mCacheData->mNumUsers;
    } else {
      PhongMaterial::cCache.emplace(mCacheKey, cache::PhongMaterialData(material));
      mCacheData = &PhongMaterial::cCache.at(mCacheKey);
    }

    updateTranslucency();
  }

  void PhongMaterial::updateUniforms() const {
    Material::updateUniforms();

    const int colorPerVertexLocation = mEffectiveProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_COLOR_PER_VERTEX);
    if (colorPerVertexLocation >= 0)
      glUniform1i(mEffectiveProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_COLOR_PER_VERTEX), mColorPerVertex);
  }

  void PhongMaterial::updateTranslucency() {
    Material::updateTranslucency();

    if (!mIsTranslucent)
      mIsTranslucent = mCacheData->mMaterial.mEmissiveAndOpacity.w < 1.0f;
  }

  void PhongMaterial::releaseMaterial() {
    if (!mCacheData)
      return;

    --mCacheData->mNumUsers;

    if (!mCacheData->mNumUsers) {
      glstate::releasePhongMaterial(mCacheData);
      PhongMaterial::cCache.erase(mCacheKey);
    }

    mCacheKey = cache::Key();
    mCacheData = NULL;
  }

}  // namespace wren

// C interface implementation

WrMaterial *wr_phong_material_new() {
  return wren::PhongMaterial::createMaterial()->materialStructure();
}

void wr_phong_material_clear(WrMaterial *material) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->clearMaterial();
}

void wr_phong_material_set_transparency(WrMaterial *material, float transparency) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setTransparency(transparency);
}

void wr_phong_material_set_color(WrMaterial *material, const float *color) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setColor(glm::make_vec3(color));
}

void wr_phong_material_set_ambient(WrMaterial *material, const float *ambient) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setAmbient(glm::make_vec3(ambient), false);
}

void wr_phong_material_set_diffuse(WrMaterial *material, const float *diffuse) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setDiffuse(glm::make_vec3(diffuse), false);
}

void wr_phong_material_set_linear_ambient(WrMaterial *material, const float *ambient) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setAmbient(glm::make_vec3(ambient), true);
}

void wr_phong_material_set_linear_diffuse(WrMaterial *material, const float *diffuse) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setDiffuse(glm::make_vec3(diffuse), true);
}

void wr_phong_material_set_specular(WrMaterial *material, const float *specular) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setSpecular(glm::make_vec3(specular));
}

void wr_phong_material_set_emissive(WrMaterial *material, const float *emissive) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setEmissive(glm::make_vec3(emissive));
}

void wr_phong_material_set_shininess(WrMaterial *material, float shininess) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setShininess(shininess);
}

void wr_phong_material_set_all_parameters(WrMaterial *material, const float *ambient, const float *diffuse,
                                          const float *specular, const float *emissive, float shininess, float transparency) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)
    ->setAllParameters(glm::make_vec3(ambient), glm::make_vec3(diffuse), glm::make_vec3(specular), glm::make_vec3(emissive),
                       shininess, transparency);
}

void wr_phong_material_set_color_per_vertex(WrMaterial *material, bool enabled) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  reinterpret_cast<wren::PhongMaterial *>(material->data)->setColorPerVertex(enabled);
}

bool wr_phong_material_is_translucent(const WrMaterial *material) {
  assert(material && material->type == WR_MATERIAL_PHONG);
  return reinterpret_cast<const wren::PhongMaterial *>(material->data)->isTranslucent();
}
