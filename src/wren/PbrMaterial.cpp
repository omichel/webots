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

#include "PbrMaterial.hpp"

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

  std::unordered_map<cache::Key, cache::PbrMaterialData> PbrMaterial::cCache;

  void PbrMaterial::setTexture(Texture *texture, size_t index) {
    Material::setTexture(texture, index);

    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    if (index < 4)
      material.mBaseColorRoughnessMetalnessOcclusionMapFlags[index] = texture ? 1.0f : 0.0f;
    else if (index < 8)
      material.mNormalBrdfEmissiveBackgroundFlags[index - 4] = texture ? 1.0f : 0.0f;
    else if (index < 12)
      material.mPenFlags[index - 8] = texture ? 1.0f : 0.0f;

    updateMaterial(material);
  }

  void PbrMaterial::setTextureCubeMap(TextureCubeMap *texture, size_t index) {
    Material::setTextureCubeMap(texture, index);

    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mCubeTextureFlags[index] = texture ? 1.0f : 0.0f;

    updateMaterial(material);
  }

  size_t PbrMaterial::sortingId() const {
    const size_t programId = static_cast<size_t>(mDefaultProgram->glName());

    size_t textureId = 0;
    if (mTextures[0].first)
      textureId = static_cast<size_t>(mTextures[0].first->glName());

    return static_cast<size_t>(mCacheData->id() << 1) | (textureId << 16) | (programId << 32) |
           (mHasPremultipliedAlpha ? 1 : 0);
  }

  PbrMaterial *PbrMaterial::createMaterial() {
    PbrMaterial *material = new PbrMaterial();
    material->init();
    return material;
  }

  void PbrMaterial::deleteMaterial(PbrMaterial *material) {
    material->releaseMaterial();
    delete material;
  }

  void PbrMaterial::clearMaterial() {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);

    material.mBaseColorAndTransparency = glm::vec4(gVec3Ones, 0.0f);
    material.mRoughnessMetalnessNormalMapFactorOcclusion = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    material.mBackgroundColorAndIblStrength = glm::vec4(0.0f);
    material.mEmissiveColorAndIntensity = glm::vec4(0.0f);
    material.mBaseColorRoughnessMetalnessOcclusionMapFlags = glm::vec4(0.0f);
    material.mNormalBrdfEmissiveBackgroundFlags = glm::vec4(0.0f);
    material.mPenFlags = glm::vec4(0.0f);
    material.mCubeTextureFlags = glm::vec4(0.0f);

    for (auto &texture : mTextures)
      texture = std::make_pair(nullptr, Texture::DEFAULT_USAGE_PARAMS);

    for (auto &textureCube : mTextureCubes)
      textureCube = std::make_pair(nullptr, Texture::DEFAULT_USAGE_PARAMS);

    mTextureTransform = NULL;

    updateMaterial(material);
  }

  void PbrMaterial::setBaseColor(const glm::vec3 &baseColor) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mBaseColorAndTransparency = colorutils::srgbToLinear(glm::vec4(baseColor, material.mBaseColorAndTransparency.w));

    updateMaterial(material);
  }

  void PbrMaterial::setBackgroundColor(const glm::vec3 &backgroundColor) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mBackgroundColorAndIblStrength =
      colorutils::srgbToLinear(glm::vec4(backgroundColor, material.mBackgroundColorAndIblStrength.w));

    updateMaterial(material);
  }

  void PbrMaterial::setEmissiveColor(const glm::vec3 &emissiveColor) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mEmissiveColorAndIntensity =
      colorutils::srgbToLinear(glm::vec4(emissiveColor, material.mEmissiveColorAndIntensity.w));

    updateMaterial(material);
  }

  void PbrMaterial::setRoughness(float roughness) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mRoughnessMetalnessNormalMapFactorOcclusion =
      glm::vec4(roughness, material.mRoughnessMetalnessNormalMapFactorOcclusion.y,
                material.mRoughnessMetalnessNormalMapFactorOcclusion.z, material.mRoughnessMetalnessNormalMapFactorOcclusion.w);

    updateMaterial(material);
  }

  void PbrMaterial::setMetalness(float metalness) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mRoughnessMetalnessNormalMapFactorOcclusion =
      glm::vec4(material.mRoughnessMetalnessNormalMapFactorOcclusion.x, metalness,
                material.mRoughnessMetalnessNormalMapFactorOcclusion.z, material.mRoughnessMetalnessNormalMapFactorOcclusion.w);

    updateMaterial(material);
  }

  void PbrMaterial::setIblStrength(float iblStrength) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mBackgroundColorAndIblStrength.w = iblStrength;

    updateMaterial(material);
  }

  void PbrMaterial::setNormalMapFactor(float normalMapFactor) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mRoughnessMetalnessNormalMapFactorOcclusion =
      glm::vec4(material.mRoughnessMetalnessNormalMapFactorOcclusion.x, material.mRoughnessMetalnessNormalMapFactorOcclusion.y,
                normalMapFactor, material.mRoughnessMetalnessNormalMapFactorOcclusion.w);

    updateMaterial(material);
  }

  void PbrMaterial::setOcclusionMapStrength(float occlusionMapStrength) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mRoughnessMetalnessNormalMapFactorOcclusion.w = occlusionMapStrength;

    updateMaterial(material);
  }

  void PbrMaterial::setEmissiveIntensity(float emissiveIntensity) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mEmissiveColorAndIntensity.w = emissiveIntensity;

    updateMaterial(material);
  }

  void PbrMaterial::setTransparency(float transparency) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);
    material.mBaseColorAndTransparency.w = transparency;

    updateMaterial(material);
  }

  void PbrMaterial::setAllParameters(const glm::vec3 &backgroundColor, const glm::vec3 &baseColor, float transparency,
                                     float roughness, float metalness, float iblStrength, float normalMapFactor,
                                     float occlusionMapStrength, const glm::vec3 &emissiveColor, float emissiveIntensity) {
    GlslLayout::PbrMaterial material(mCacheData->mMaterial);

    material.mBaseColorAndTransparency = colorutils::srgbToLinear(glm::vec4(baseColor, transparency));
    material.mRoughnessMetalnessNormalMapFactorOcclusion =
      glm::vec4(roughness, metalness, normalMapFactor, occlusionMapStrength);
    material.mBackgroundColorAndIblStrength = glm::vec4(backgroundColor, iblStrength);
    material.mEmissiveColorAndIntensity = glm::vec4(emissiveColor, emissiveIntensity);

    updateMaterial(material);
  }

  void PbrMaterial::bind(bool bindProgram) const {
    if (bindProgram)
      Material::useProgram();

    assert(mCacheData);
    glstate::bindPbrMaterial(mCacheData);

    updateUniforms();
    Material::bindTextures();
  }

  PbrMaterial::PbrMaterial() : Material(), mCacheData(NULL) {
    mMaterialStructure = new WrMaterial;
    mMaterialStructure->type = WR_MATERIAL_PBR;
    mMaterialStructure->data = reinterpret_cast<void *>(this);
  }

  void PbrMaterial::init() {
    GlslLayout::PbrMaterial material;
    material.mBaseColorAndTransparency = glm::vec4(gVec3Ones, 0.0f);
    material.mRoughnessMetalnessNormalMapFactorOcclusion = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    material.mBackgroundColorAndIblStrength = glm::vec4(0.0f);
    material.mEmissiveColorAndIntensity = glm::vec4(0.0f);
    material.mBaseColorRoughnessMetalnessOcclusionMapFlags = glm::vec4(0.0f);
    material.mNormalBrdfEmissiveBackgroundFlags = glm::vec4(0.0f);
    material.mPenFlags = glm::vec4(0.0f);
    material.mCubeTextureFlags = glm::vec4(0.0f);
    updateMaterial(material);
  }

  void PbrMaterial::updateMaterial(const GlslLayout::PbrMaterial &material) {
    // remove this instance from the set of users of the previous material
    auto it = PbrMaterial::cCache.find(mCacheKey);
    if (it != PbrMaterial::cCache.end())
      releaseMaterial();

    mCacheKey = cache::Key(cache::sipHash13c(reinterpret_cast<const char *>(&material), sizeof(GlslLayout::PbrMaterial)));

    it = PbrMaterial::cCache.find(mCacheKey);
    if (it != PbrMaterial::cCache.end()) {
      mCacheData = &it->second;
      ++mCacheData->mNumUsers;
    } else {
      PbrMaterial::cCache.emplace(mCacheKey, cache::PbrMaterialData(material));
      mCacheData = &PbrMaterial::cCache.at(mCacheKey);
    }

    updateTranslucency();
  }

  void PbrMaterial::updateTranslucency() {
    Material::updateTranslucency();

    if (!mIsTranslucent)
      mIsTranslucent = mCacheData->mMaterial.mBaseColorAndTransparency.w > 0.0f;
  }

  void PbrMaterial::releaseMaterial() {
    if (!mCacheData)
      return;

    --mCacheData->mNumUsers;

    if (!mCacheData->mNumUsers) {
      glstate::releasePbrMaterial(mCacheData);
      PbrMaterial::cCache.erase(mCacheKey);
    }

    mCacheKey = cache::Key();
    mCacheData = NULL;
  }

}  // namespace wren

// C interface implementation

WrMaterial *wr_pbr_material_new() {
  return wren::PbrMaterial::createMaterial()->materialStructure();
}

void wr_pbr_material_clear(WrMaterial *material) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->clearMaterial();
}

void wr_pbr_material_set_transparency(WrMaterial *material, float transparency) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setTransparency(transparency);
}

void wr_pbr_material_set_background_color(WrMaterial *material, const float *background_color) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setBackgroundColor(glm::make_vec3(background_color));
}

void wr_pbr_material_set_base_color(WrMaterial *material, const float *base_color) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setBaseColor(glm::make_vec3(base_color));
}

void wr_pbr_material_set_emissive_color(WrMaterial *material, const float *emissive_color) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setEmissiveColor(glm::make_vec3(emissive_color));
}

void wr_pbr_material_set_roughness(WrMaterial *material, float roughness) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setRoughness(roughness);
}

void wr_pbr_material_set_metalness(WrMaterial *material, float metalness) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setMetalness(metalness);
}

void wr_pbr_material_set_ibl_strength(WrMaterial *material, float ibl_strength) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setIblStrength(ibl_strength);
}

void wr_pbr_material_set_normal_map_strength(WrMaterial *material, float normal_map_factor) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setNormalMapFactor(normal_map_factor);
}

void wr_pbr_material_set_occlusion_map_strength(WrMaterial *material, float occlusion_map_strength) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setOcclusionMapStrength(occlusion_map_strength);
}

void wr_pbr_material_set_emissive_intensity(WrMaterial *material, float emissive_intensity) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)->setEmissiveIntensity(emissive_intensity);
}

void wr_pbr_material_set_all_parameters(WrMaterial *material, const float *background_color, const float *base_color,
                                        float transparency, float roughness, float metalness, float ibl_strength,
                                        float normal_map_factor, float occlusion_map_strength, const float *emissive_color,
                                        float emissive_intensity) {
  assert(material && material->type == WR_MATERIAL_PBR);
  reinterpret_cast<wren::PbrMaterial *>(material->data)
    ->setAllParameters(glm::make_vec3(background_color), glm::make_vec3(base_color), transparency, roughness, metalness,
                       ibl_strength, normal_map_factor, occlusion_map_strength, glm::make_vec3(emissive_color),
                       emissive_intensity);
}

bool wr_pbr_material_is_translucent(const WrMaterial *material) {
  assert(material && material->type == WR_MATERIAL_PBR);
  return reinterpret_cast<const wren::PbrMaterial *>(material->data)->isTranslucent();
}
