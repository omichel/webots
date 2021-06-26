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

#include "Material.hpp"

#include "Debug.hpp"
#include "GlState.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"
#include "TextureCubeMap.hpp"
#include "TextureTransform.hpp"
#include "UniformBuffer.hpp"

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  void Material::setTexture(Texture *texture, size_t index) {
    // For now only 13 texture flags are passed to the shader, this number can be increased when necessary
    assert(index < gMaxShaderTextures);

    // Replacing a texture in this slot
    if (mTextures[index].first) {
      int numberOfInstancesOfThisTexture = countTextureInstances(mTextures[index].first);
      // remove this material from texture's list of users if we're replacing the last instance of this texture
      if (numberOfInstancesOfThisTexture == 1)
        mTextures[index].first->removeMaterialUser(this);
    }

    mTextures[index].first = texture;
    if (texture)
      texture->addMaterialUser(this);

    mHasPremultipliedAlpha = false;
    for (auto &t : mTextures)
      mHasPremultipliedAlpha |= t.first && t.first->hasPremultipliedAlpha();
  }

  void Material::setTextureTransform(TextureTransform *transform) {
    if (mTextureTransform)
      mTextureTransform->removeMaterialUser(this);
    mTextureTransform = transform;
    if (mTextureTransform)
      mTextureTransform->addMaterialUser(this);
  }

  void Material::setTextureCubeMap(TextureCubeMap *texture, size_t index) {
    // For now only 2 texture flags are passed to the shader, this number can be increased when necessary
    assert(index < gMaxShaderCubemapTextures);

    mTextureCubes[index].first = texture;
  }

  void Material::removeDeletedTexture(Texture *texture) {
    for (size_t i = 0; i < mTextures.size(); ++i) {
      if (mTextures[i].first == texture)
        setTexture(NULL, i);
    }
  }

  Material::Material() :
    mHasPremultipliedAlpha(false),
    mIsTranslucent(false),
    mTextures(gMaxShaderTextures, std::make_pair(nullptr, Texture::DEFAULT_USAGE_PARAMS)),
    mTextureCubes(gMaxShaderCubemapTextures, std::make_pair(nullptr, Texture::DEFAULT_USAGE_PARAMS)),
    mTextureTransform(NULL),
    mMaterialStructure(NULL),
    mDefaultProgram(NULL),
    mStencilAmbientEmissiveProgram(NULL),
    mStencilDiffuseSpecularProgram(NULL),
    mEffectiveProgram(NULL) {}

  Material::~Material() {
    for (auto &t : mTextures) {
      if (t.first)
        t.first->removeMaterialUser(this);
    }
    if (mTextureTransform)
      mTextureTransform->removeMaterialUser(this);
  }

  void Material::updateTranslucency() {
    mIsTranslucent = false;

    for (auto &t : mTextures) {
      if (t.first && t.first->isTranslucent())
        mIsTranslucent = true;
    }
  }

  void Material::useProgram() const {
    assert(mEffectiveProgram);

    mEffectiveProgram->bind();
  }

  void Material::bindTextures() const {
    for (int i = 0; i < gMaxShaderTextures; ++i) {
      if (mTextures[i].first) {
        mTextures[i].first->setTextureUnit(i);
        mTextures[i].first->bind(mTextures[i].second);
      }
      glUniform1i(mEffectiveProgram->uniformLocation(static_cast<WrGlslLayoutUniform>(WR_GLSL_LAYOUT_UNIFORM_TEXTURE0 + i)), i);
    }

    for (int i = 0; i < gMaxShaderCubemapTextures; ++i) {
      if (mTextureCubes[i].first) {
        mTextureCubes[i].first->setTextureUnit(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0 + i);
        mTextureCubes[i].first->bind(mTextureCubes[i].second);
      }
      glUniform1i(
        mEffectiveProgram->uniformLocation(static_cast<WrGlslLayoutUniform>(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0 + i)),
        WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0 + i);
    }
  }

  void Material::updateUniforms() const {
    const int textureTransformLocation = mEffectiveProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);
    if (textureTransformLocation >= 0) {
      if (mTextureTransform)
        glUniformMatrix4fv(mEffectiveProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM), 1, false,
                           glm::value_ptr(mTextureTransform->matrix()));
      else
        glUniformMatrix4fv(mEffectiveProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM), 1, false,
                           glm::value_ptr(gMat4Identity));
    }

    if (mTextures[0].first) {
      const int channelCountLocation = mEffectiveProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_CHANNEL_COUNT);
      if (channelCountLocation >= 0)
        glUniform1i(mEffectiveProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_CHANNEL_COUNT),
                    mTextures[0].first->glFormatParams().mChannelCount);
    }
  }

  int Material::countTextureInstances(Texture *texture) {
    int instances = 0;
    for (auto &textureInstance : mTextures) {
      if (textureInstance.first == texture)
        ++instances;
    }
    return instances;
  }

}  // namespace wren

// C interface implementation

void wr_material_delete(WrMaterial *material) {
  if (!material)
    return;

  if (material->type == WR_MATERIAL_PHONG)
    wren::PhongMaterial::deleteMaterial(reinterpret_cast<wren::PhongMaterial *>(material->data));
  else if (material->type == WR_MATERIAL_PBR)
    wren::PbrMaterial::deleteMaterial(reinterpret_cast<wren::PbrMaterial *>(material->data));
}

void wr_material_set_texture(WrMaterial *material, WrTexture *texture, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTexture(reinterpret_cast<wren::Texture *>(texture), index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTexture(reinterpret_cast<wren::Texture *>(texture), index);
}

void wr_material_set_texture_wrap_s(WrMaterial *material, WrTextureWrapMode mode, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTextureWrapS(mode, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->setTextureWrapS(mode, index);
}

void wr_material_set_texture_wrap_t(WrMaterial *material, WrTextureWrapMode mode, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTextureWrapT(mode, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->setTextureWrapT(mode, index);
}

void wr_material_set_texture_anisotropy(WrMaterial *material, float anisotropy, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTextureAnisotropy(anisotropy, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->setTextureAnisotropy(anisotropy, index);
}

void wr_material_set_texture_enable_interpolation(WrMaterial *material, bool enable, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->enableTextureInterpolation(enable, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->enableTextureInterpolation(enable, index);
}

void wr_material_set_texture_enable_mip_maps(WrMaterial *material, bool enable, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->enableTextureMipMaps(enable, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->enableTextureMipMaps(enable, index);
}

void wr_material_set_texture_cubemap(WrMaterial *material, WrTextureCubeMap *texture, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)
      ->setTextureCubeMap(reinterpret_cast<wren::TextureCubeMap *>(texture), index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)
      ->setTextureCubeMap(reinterpret_cast<wren::TextureCubeMap *>(texture), index);
}

void wr_material_set_texture_cubemap_wrap_r(WrMaterial *material, WrTextureWrapMode mode, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTextureCubeMapWrapR(mode, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->setTextureCubeMapWrapR(mode, index);
}

void wr_material_set_texture_cubemap_wrap_s(WrMaterial *material, WrTextureWrapMode mode, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTextureCubeMapWrapS(mode, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->setTextureCubeMapWrapS(mode, index);
}

void wr_material_set_texture_cubemap_wrap_t(WrMaterial *material, WrTextureWrapMode mode, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTextureCubeMapWrapT(mode, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->setTextureCubeMapWrapT(mode, index);
}

void wr_material_set_texture_cubemap_anisotropy(WrMaterial *material, float anisotropy, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->setTextureCubeMapAnisotropy(anisotropy, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->setTextureCubeMapAnisotropy(anisotropy, index);
}

void wr_material_set_texture_cubemap_enable_interpolation(WrMaterial *material, bool enable, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->enableTextureCubeMapInterpolation(enable, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->enableTextureCubeMapInterpolation(enable, index);
}

void wr_material_set_texture_cubemap_enable_mip_maps(WrMaterial *material, bool enable, int index) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)->enableTextureCubeMapMipMaps(enable, index);
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->enableTextureCubeMapMipMaps(enable, index);
}

void wr_material_set_texture_transform(WrMaterial *material, WrTextureTransform *transform) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)
      ->setTextureTransform(reinterpret_cast<wren::TextureTransform *>(transform));
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)
      ->setTextureTransform(reinterpret_cast<wren::TextureTransform *>(transform));
}

void wr_material_set_default_program(WrMaterial *material, WrShaderProgram *program) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)
      ->setDefaultProgram(reinterpret_cast<wren::ShaderProgram *>(program));
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)->setDefaultProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_material_set_stencil_ambient_emissive_program(WrMaterial *material, WrShaderProgram *program) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)
      ->setStencilAmbientEmissiveProgram(reinterpret_cast<wren::ShaderProgram *>(program));
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)
      ->setStencilAmbientEmissiveProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_material_set_stencil_diffuse_specular_program(WrMaterial *material, WrShaderProgram *program) {
  if (material->type == WR_MATERIAL_PHONG)
    reinterpret_cast<wren::PhongMaterial *>(material->data)
      ->setStencilDiffuseSpecularProgram(reinterpret_cast<wren::ShaderProgram *>(program));
  else if (material->type == WR_MATERIAL_PBR)
    reinterpret_cast<wren::PbrMaterial *>(material->data)
      ->setStencilDiffuseSpecularProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

WrTexture *wr_material_get_texture(const WrMaterial *material, int index) {
  return reinterpret_cast<WrTexture *>(reinterpret_cast<const wren::Material *>(material->data)->texture(index));
}

WrTextureWrapMode wr_material_get_texture_wrap_s(const WrMaterial *material, int index) {
  return static_cast<WrTextureWrapMode>(reinterpret_cast<const wren::Material *>(material->data)->textureWrapS(index));
}

WrTextureWrapMode wr_material_get_texture_wrap_t(const WrMaterial *material, int index) {
  return static_cast<WrTextureWrapMode>(reinterpret_cast<const wren::Material *>(material->data)->textureWrapT(index));
}

float wr_material_get_texture_anisotropy(const WrMaterial *material, int index) {
  return reinterpret_cast<const wren::Material *>(material->data)->textureAnisotropy(index);
}

bool wr_material_is_texture_interpolation_enabled(const WrMaterial *material, int index) {
  return reinterpret_cast<const wren::Material *>(material->data)->isTextureInterpolationEnabled(index);
}

bool wr_material_are_texture_mip_maps_enabled(const WrMaterial *material, int index) {
  return reinterpret_cast<const wren::Material *>(material->data)->areTextureMipMapsEnabled(index);
}
