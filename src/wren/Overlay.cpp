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

#include "Overlay.hpp"

#include "Debug.hpp"
#include "GlState.hpp"
#include "Material.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "StaticMesh.hpp"
#include "Texture.hpp"
#include "UniformBuffer.hpp"
#include "Viewport.hpp"

#include <wren/overlay.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <algorithm>

namespace wren {

  static const int BACKGROUND_TEXTURE_INDEX = 0;
  static const int MAIN_TEXTURE_INDEX = 1;
  static const int MASK_TEXTURE_INDEX = 2;
  static const int FOREGROUND_TEXTURE_INDEX = 3;
  static const int ADDITIONAL_TEXTURE_STARTING_INDEX = 4;

  ShaderProgram *Overlay::cDefaultSizeProgram = NULL;
  size_t Overlay::cMaxOrder = 0;
  std::vector<Overlay *> Overlay::cOverlays;

  void Overlay::putOnTop() {
    for (auto &o : cOverlays) {
      if (o->order() > mOrder)
        o->setOrder(o->order() - 1);
    }

    mOrder = cMaxOrder - 1;
  }

  void Overlay::setBackgroundColor(const glm::vec4 color) {
    if (mPremultipliedAlpha) {
      glm::vec4 backgroundColor = color * color[3];
      backgroundColor[3] = color[3];
      mParams.mBackgroundColor = backgroundColor;
    } else
      mParams.mBackgroundColor = color;
  }

  void Overlay::setTexture(Texture *texture, int role) {
    mTextures[role] = texture;
    if (texture)
      mParams.mActiveFlags.x = mParams.mActiveFlags.x | (1u << role);
    else
      mParams.mActiveFlags.x = mParams.mActiveFlags.x & ~(1u << role);
  }

  void Overlay::addAdditionalTexture(Texture *texture) {
    mAdditionalTextures.push_back(texture);
    mParams.mTextureFlags.y = mAdditionalTextures.size();
  }

  void Overlay::setCachePersistency(bool isPersistent) { mMesh->setCachePersistency(isPersistent); }

  void Overlay::render() {
    assert(mMesh);
    assert(mProgram);

    DEBUG("Overlay::render, this=" << this << ", order=" << mOrder);
    DEBUGONE(mTextures[MAIN_TEXTURE_INDEX]);
    debug::printVec4(mParams.mPositionAndSize, "mPositionAndSize");

    if (!mIsVisible || mParams.mPositionAndSize.z == 0.0f || mParams.mPositionAndSize.w == 0.0f)
      return;

    mProgram->bind();

    // Use special blend function if required
    const unsigned int blendSrcFactor = glstate::blendSrcFactor();
    const unsigned int blendDestFactor = glstate::blendDestFactor();
    if (mPremultipliedAlpha)
      glstate::setBlendFunc(GL_ONE, blendDestFactor);

    mParams.mSizeInPixels = glm::vec2(mParams.mPositionAndSize.z * Scene::instance()->currentViewport()->width(),
                                      mParams.mPositionAndSize.w * Scene::instance()->currentViewport()->height());
    glstate::uniformBuffer(WR_GLSL_LAYOUT_UNIFORM_BUFFER_OVERLAY)->writeValue(&mParams);

    for (size_t i = 0; i < ADDITIONAL_TEXTURE_STARTING_INDEX; ++i)
      prepareTexture(mTextures[i], i);

    for (size_t i = 0; i < mAdditionalTextures.size(); ++i)
      prepareTexture(mAdditionalTextures[i], ADDITIONAL_TEXTURE_STARTING_INDEX + i);

    int channelCountLocation = mProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_CHANNEL_COUNT);
    if (channelCountLocation >= 0 && mTextures[MAIN_TEXTURE_INDEX])
      glUniform1i(channelCountLocation, mTextures[MAIN_TEXTURE_INDEX]->glFormatParams().mChannelCount);

    mMesh->render(GL_TRIANGLES);

    // Restore state
    if (mPremultipliedAlpha)
      glstate::setBlendFunc(blendSrcFactor, blendDestFactor);

    if (mShowDefaultSize) {
      DEBUG("Rendering default size indicator, width=" << mParams.mDefaultSize.x << ", height=" << mParams.mDefaultSize.y);
      mParams.mDefaultSize.z = 1.0f;
      glstate::uniformBuffer(WR_GLSL_LAYOUT_UNIFORM_BUFFER_OVERLAY)->writeValue(&mParams);
      mMesh->render(GL_TRIANGLES);
      mParams.mDefaultSize.z = 0.0f;
    }
  }

  Overlay::Overlay() :
    mOrder(cMaxOrder++),
    mIsVisible(true),
    mShowDefaultSize(false),
    mPremultipliedAlpha(false),
    mTextureParams(Texture::DEFAULT_USAGE_PARAMS),
    mMesh(StaticMesh::createQuad()),
    mProgram(NULL) {
    mTextures.resize(FOREGROUND_TEXTURE_INDEX + 1, NULL);
    mTextureParams.mIsInterpolationEnabled = false;

    mParams.mPositionAndSize = glm::vec4(0.0f);
    mParams.mDefaultSize = glm::vec4(0.0f);
    mParams.mBorderColor = glm::vec4(0.0f);
    mParams.mBackgroundColor = glm::vec4(0.0f);
    mParams.mTextureFlags = glm::vec4(0.0f);
    mParams.mActiveFlags = glm::uvec2(0u);
    mParams.mSizeInPixels = glm::vec2(0.0f, 0.0f);
    mParams.mBorderSize = glm::vec2(0.0f);

    cOverlays.push_back(this);
  }

  Overlay::~Overlay() {
    StaticMesh::deleteMesh(mMesh);

    cOverlays.erase(std::remove(cOverlays.begin(), cOverlays.end(), this), cOverlays.end());
  }

  void Overlay::prepareTexture(Texture *texture, int textureUnitIndex) {
    if (!texture)
      return;

    assert(WR_GLSL_LAYOUT_UNIFORM_TEXTURE0 + textureUnitIndex < WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);

    int location =
      mProgram->uniformLocation(static_cast<WrGlslLayoutUniform>(WR_GLSL_LAYOUT_UNIFORM_TEXTURE0 + textureUnitIndex));
    assert(location >= 0);

    texture->setTextureUnit(textureUnitIndex);
    texture->bind(mTextureParams);

    glUniform1i(location, texture->textureUnit());
  }

}  // namespace wren

// C interface implementation
WrOverlay *wr_overlay_new() {
  return reinterpret_cast<WrOverlay *>(wren::Overlay::createOverlay());
}

void wr_overlay_delete(WrOverlay *overlay) {
  wren::Overlay::deleteOverlay(reinterpret_cast<wren::Overlay *>(overlay));
}

void wr_overlay_set_order(WrOverlay *overlay, int order) {
  reinterpret_cast<wren::Overlay *>(overlay)->setOrder(order);
}

void wr_overlay_put_on_top(WrOverlay *overlay) {
  reinterpret_cast<wren::Overlay *>(overlay)->putOnTop();
}

void wr_overlay_set_visible(WrOverlay *overlay, bool visible) {
  reinterpret_cast<wren::Overlay *>(overlay)->setVisible(visible);
}

void wr_overlay_show_default_size(WrOverlay *overlay, bool visible) {
  reinterpret_cast<wren::Overlay *>(overlay)->showDefaultSize(visible);
}

void wr_overlay_set_default_size(WrOverlay *overlay, float width, float height) {
  reinterpret_cast<wren::Overlay *>(overlay)->setDefaultSize(width, height);
}

void wr_overlay_set_position(WrOverlay *overlay, float x, float y) {
  reinterpret_cast<wren::Overlay *>(overlay)->setPosition(x, y);
}

void wr_overlay_set_size(WrOverlay *overlay, float width, float height) {
  reinterpret_cast<wren::Overlay *>(overlay)->setSize(width, height);
}

void wr_overlay_set_use_premultiplied_alpha_textures(WrOverlay *overlay, bool enabled) {
  reinterpret_cast<wren::Overlay *>(overlay)->setUsePremultipliedAlpha(enabled);
}

void wr_overlay_set_anisotropy(WrOverlay *overlay, float anisotropy) {
  reinterpret_cast<wren::Overlay *>(overlay)->setAnisotropy(anisotropy);
}

void wr_overlay_set_translucency(WrOverlay *overlay, bool enabled) {
  reinterpret_cast<wren::Overlay *>(overlay)->setTranslucency(enabled);
}

void wr_overlay_enable_interpolation(WrOverlay *overlay, bool enabled) {
  reinterpret_cast<wren::Overlay *>(overlay)->enableInterpolation(enabled);
}

void wr_overlay_enable_mip_maps(WrOverlay *overlay, bool enable) {
  reinterpret_cast<wren::Overlay *>(overlay)->enableMipMaps(enable);
}

void wr_overlay_set_background_color(WrOverlay *overlay, const float *color) {
  reinterpret_cast<wren::Overlay *>(overlay)->setBackgroundColor(glm::make_vec4(color));
}

void wr_overlay_set_border_active(WrOverlay *overlay, bool active) {
  reinterpret_cast<wren::Overlay *>(overlay)->setBorderActive(active);
}

void wr_overlay_set_border_color(WrOverlay *overlay, const float *color) {
  reinterpret_cast<wren::Overlay *>(overlay)->setBorderColor(glm::make_vec4(color));
}

void wr_overlay_set_border_size(WrOverlay *overlay, float size_horizontal, float size_vertical) {
  reinterpret_cast<wren::Overlay *>(overlay)->setBorderSize(size_horizontal, size_vertical);
}

void wr_overlay_set_texture(WrOverlay *overlay, WrTexture *texture) {
  reinterpret_cast<wren::Overlay *>(overlay)->setTexture(reinterpret_cast<wren::Texture *>(texture), wren::MAIN_TEXTURE_INDEX);
}

void wr_overlay_set_texture_flip_vertical(WrOverlay *overlay, bool flip) {
  reinterpret_cast<wren::Overlay *>(overlay)->setTextureFlipVertical(flip);
}

void wr_overlay_set_background_texture(WrOverlay *overlay, WrTexture *texture) {
  reinterpret_cast<wren::Overlay *>(overlay)->setTexture(reinterpret_cast<wren::Texture *>(texture),
                                                         wren::BACKGROUND_TEXTURE_INDEX);
}

void wr_overlay_set_mask_texture(WrOverlay *overlay, WrTexture *texture) {
  reinterpret_cast<wren::Overlay *>(overlay)->setTexture(reinterpret_cast<wren::Texture *>(texture), wren::MASK_TEXTURE_INDEX);
}

void wr_overlay_set_foreground_texture(WrOverlay *overlay, WrTexture *texture) {
  reinterpret_cast<wren::Overlay *>(overlay)->setTexture(reinterpret_cast<wren::Texture *>(texture),
                                                         wren::FOREGROUND_TEXTURE_INDEX);
}

void wr_overlay_add_additional_texture(WrOverlay *overlay, WrTexture *texture) {
  reinterpret_cast<wren::Overlay *>(overlay)->addAdditionalTexture(reinterpret_cast<wren::Texture *>(texture));
}

void wr_overlay_set_program(WrOverlay *overlay, WrShaderProgram *program) {
  reinterpret_cast<wren::Overlay *>(overlay)->setProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_overlay_set_default_size_program(WrOverlay *overlay, WrShaderProgram *program) {
  wren::Overlay::setDefaultSizeProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_overlay_set_max_range(WrOverlay *overlay, float max_range) {
  reinterpret_cast<wren::Overlay *>(overlay)->setMaxRange(max_range);
}

bool wr_overlay_is_visible(WrOverlay *overlay) {
  return reinterpret_cast<wren::Overlay *>(overlay)->isVisible();
}

int wr_overlay_get_order(WrOverlay *overlay) {
  return reinterpret_cast<wren::Overlay *>(overlay)->order();
}

float wr_overlay_get_x(WrOverlay *overlay) {
  return reinterpret_cast<wren::Overlay *>(overlay)->x();
}

float wr_overlay_get_y(WrOverlay *overlay) {
  return reinterpret_cast<wren::Overlay *>(overlay)->y();
}

float wr_overlay_get_width(WrOverlay *overlay) {
  return reinterpret_cast<wren::Overlay *>(overlay)->width();
}

float wr_overlay_get_height(WrOverlay *overlay) {
  return reinterpret_cast<wren::Overlay *>(overlay)->height();
}

void wr_overlay_set_cache_persistency(WrOverlay *overlay, bool is_persistent) {
  reinterpret_cast<wren::Overlay *>(overlay)->setCachePersistency(is_persistent);
}
