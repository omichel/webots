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

#include "Texture.hpp"

#include "Constants.hpp"
#include "Debug.hpp"
#include "GlState.hpp"
#include "Material.hpp"

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  const Texture::UsageParams Texture::DEFAULT_USAGE_PARAMS(WR_TEXTURE_WRAP_MODE_REPEAT, WR_TEXTURE_WRAP_MODE_REPEAT,
                                                           WR_TEXTURE_WRAP_MODE_REPEAT, gVec4Zeros, 1.0f, true, false);

  const Texture::GlFormatParams Texture::GL_FORMAT_PARAMS[WR_TEXTURE_INTERNAL_FORMAT_COUNT] = {
    GlFormatParams(GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1, 1),
    GlFormatParams(GL_RG8, GL_RG, GL_UNSIGNED_BYTE, 2, 2),
#ifdef __EMSCRIPTEN__
    GlFormatParams(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 3, 3),
    GlFormatParams(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4, 4),
#else
    GlFormatParams(GL_RGB8, GL_BGR, GL_UNSIGNED_BYTE, 3, 3),
    GlFormatParams(GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE, 4, 4),
#endif
    GlFormatParams(GL_R16F, GL_RED, GL_HALF_FLOAT, 2, 1),
    GlFormatParams(GL_RGB16F, GL_RGB, GL_HALF_FLOAT, 6, 3),
    GlFormatParams(GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT, 8, 4),
    GlFormatParams(GL_R32F, GL_RED, GL_FLOAT, 4, 1),
    GlFormatParams(GL_RG32F, GL_RG, GL_FLOAT, 8, 2),
    GlFormatParams(GL_RGB32F, GL_RGB, GL_FLOAT, 12, 3),
    GlFormatParams(GL_RGBA32F, GL_RGBA, GL_FLOAT, 16, 4),
    GlFormatParams(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 4, 2),
    GlFormatParams(GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT, 4, 1)};

  char *Texture::generateBlackData(const GlFormatParams &glFormatParams, int width, int height) {
    const int sizeInBytes = width * height * glFormatParams.mPixelSize;
    char *data = new char[sizeInBytes];

    char color[16];
    memset(&color[0], 0, 16);

    char *current = data;
    const char *end = data + sizeInBytes;
    while (current < end) {
      memcpy(current, &color[0], glFormatParams.mPixelSize);
      current += glFormatParams.mPixelSize;
    }

    return data;
  }

  unsigned int Texture::generateNewTexture() {
    unsigned int glName;
    glGenTextures(1, &glName);
    glstate::checkError();
    glstate::initializeTextureParams(glName);
    return glName;
  }

  void Texture::deleteTexture(Texture *texture) {
    if (!texture)
      return;

    if (glstate::isContextActive()) {
      texture->cleanupGl();
      delete texture;
    } else
      texture->setRequireAction(GlUser::GL_ACTION_DELETE);
  }

  void Texture::setInternalFormat(WrTextureInternalFormat format) {
    assert(format < WR_TEXTURE_INTERNAL_FORMAT_COUNT);
    mGlFormatParams = GL_FORMAT_PARAMS[format];
  }

  void Texture::setTextureUnit(int textureUnit) {
    if (textureUnit == mTextureUnit)
      return;

    if (mTextureUnit >= 0 && glName())
      release();

    mTextureUnit = textureUnit;
  }

  void Texture::changeData(void *data, int x, int y, int width, int height) {
    assert(glstate::isContextActive());
    assert(glName());

    bind(DEFAULT_USAGE_PARAMS);

    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, mGlFormatParams.mFormat, mGlFormatParams.mDataType, data);
    mHaveMipMapsBeenGenerated = false;
  }

  void Texture::generateMipMaps(bool regenerate) {
    if (!glName() || (mHaveMipMapsBeenGenerated && !regenerate))
      return;

    glGenerateMipmap(GL_TEXTURE_2D);
    mHaveMipMapsBeenGenerated = true;
  }

  void Texture::setSize(int width, int height) {
    mWidth = width;
    mHeight = height;
  }

  void Texture::setup() {
    if (glstate::isContextActive())
      prepareGl();
    else
      setRequireAction(GlUser::GL_ACTION_PREPARE);
  }

  void Texture::bind(const Texture::UsageParams &params) {
    glstate::activateTextureUnit(mTextureUnit);
    glstate::bindTexture(glName(), mTextureUnit);

    if (params.mAreMipMapsEnabled)
      generateMipMaps();

    glstate::setTextureWrapS(glName(), mTextureUnit, params.mWrapS);
    glstate::setTextureWrapT(glName(), mTextureUnit, params.mWrapT);
    glstate::setTextureBorderColor(glName(), mTextureUnit, params.mBorderColor);
    glstate::setTextureAnisotropy(glName(), mTextureUnit, params.mAnisotropy);
    glstate::setTextureInterpolation(glName(), mTextureUnit, params.mIsInterpolationEnabled, params.mAreMipMapsEnabled);
  }

  void Texture::release() { glstate::releaseTexture(glName(), mTextureUnit); }

  void Texture::cleanupGl() {
    if (glName()) {
      // Reset parameters to default state before freeing OpenGL name
      glstate::clearTextureParams(glName());

      release();

      unsigned int name = glName();
      glDeleteTextures(1, &name);

      setTextureUnit(-1);
      setGlName(0);
    }
  }

  Texture::Texture() :
    mGlFormatParams(GL_FORMAT_PARAMS[WR_TEXTURE_INTERNAL_FORMAT_RGBA8]),
    mTextureUnit(-1),
    mWidth(0),
    mHeight(0),
    mHaveMipMapsBeenGenerated(false),
    mIsTranslucent(false),
    mMaterialsUsingThisTexture() {}

  Texture::~Texture() {
    for (auto &material : mMaterialsUsingThisTexture)
      material->removeDeletedTexture(this);
  }

  void Texture::addMaterialUser(Material *material) {
    assert(material);
    for (auto &user : mMaterialsUsingThisTexture) {
      if (user == material)
        return;
    }

    mMaterialsUsingThisTexture.push_back(material);
  }

  void Texture::removeMaterialUser(Material *material) {
    assert(material);
    for (size_t i = 0; i < mMaterialsUsingThisTexture.size(); ++i) {
      if (mMaterialsUsingThisTexture[i] == material)
        mMaterialsUsingThisTexture.erase(mMaterialsUsingThisTexture.begin() + i);
    }
  }

}  // namespace wren

// C interface implementation
void wr_texture_delete(WrTexture *texture) {
  wren::Texture::deleteTexture(reinterpret_cast<wren::Texture *>(texture));
}

void wr_texture_set_internal_format(WrTexture *texture, WrTextureInternalFormat format) {
  reinterpret_cast<wren::Texture *>(texture)->setInternalFormat(format);
}

void wr_texture_set_texture_unit(WrTexture *texture, unsigned int unit) {
  reinterpret_cast<wren::Texture *>(texture)->setTextureUnit(unit);
}

void wr_texture_set_size(WrTexture *texture, int width, int height) {
  reinterpret_cast<wren::Texture *>(texture)->setSize(width, height);
}

void wr_texture_set_translucent(WrTexture *texture, bool translucent) {
  reinterpret_cast<wren::Texture *>(texture)->setTranslucent(translucent);
}

void wr_texture_change_data(WrTexture *texture, void *data, int x, int y, int width, int height) {
  reinterpret_cast<wren::Texture *>(texture)->changeData(data, x, y, width, height);
}

void wr_texture_setup(WrTexture *texture) {
  reinterpret_cast<wren::Texture *>(texture)->setup();
}

int wr_texture_get_width(const WrTexture *texture) {
  return reinterpret_cast<const wren::Texture *>(texture)->width();
}

int wr_texture_get_height(const WrTexture *texture) {
  return reinterpret_cast<const wren::Texture *>(texture)->height();
}

bool wr_texture_is_translucent(const WrTexture *texture) {
  return reinterpret_cast<const wren::Texture *>(texture)->isTranslucent();
}

WrTextureType wr_texture_get_type(const WrTexture *texture) {
  return reinterpret_cast<const wren::Texture *>(texture)->type();
}

unsigned int wr_texture_get_gl_name(const WrTexture *texture) {
  return reinterpret_cast<const wren::Texture *>(texture)->glName();
}
