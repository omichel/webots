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

#include "TextureCubeMap.hpp"

#include "Debug.hpp"
#include "GlState.hpp"

#include <wren/gl_state.h>
#include <wren/texture_cubemap.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  TextureCubeMap::TextureCubeMap() : mGlName(0) {
    for (int i = 0; i < WR_TEXTURE_CUBEMAP_COUNT; ++i)
      mDatas[i] = NULL;
  }

  void TextureCubeMap::generateMipMaps(bool regenerate) {
    if (!glName() || (mHaveMipMapsBeenGenerated && !regenerate))
      return;

    glstate::activateTextureUnit(mTextureUnit);
    glstate::bindTextureCubeMap(mGlName, mTextureUnit);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    mHaveMipMapsBeenGenerated = true;
  }

  void TextureCubeMap::bind(const Texture::UsageParams &params) {
    glstate::activateTextureUnit(mTextureUnit);
    glstate::bindTextureCubeMap(mGlName, mTextureUnit);

    if (params.mAreMipMapsEnabled)
      generateMipMaps();

    int minFilter = params.mAreMipMapsEnabled ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
    if (params.mIsInterpolationEnabled)
      minFilter = params.mAreMipMapsEnabled ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

    const int magFilter = params.mIsInterpolationEnabled ? GL_LINEAR : GL_NEAREST;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, params.mWrapR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, params.mWrapS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, params.mWrapT);

    if (wr_gl_state_is_anisotropic_texture_filtering_supported())
      glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, params.mAnisotropy);
  }

  void TextureCubeMap::release() { glstate::releaseTextureCubeMap(mGlName, mTextureUnit); }

  void TextureCubeMap::prepareGl() {
    assert(mWidth);
    assert(mHeight);
    for (int i = 0; i < WR_TEXTURE_CUBEMAP_COUNT; ++i)
      assert(mDatas[i]);

    mGlName = generateNewTexture();

    if (textureUnit() < 0)
      setTextureUnit(0);

    bind(Texture::DEFAULT_USAGE_PARAMS);

    for (unsigned int i = 0; i < WR_TEXTURE_CUBEMAP_COUNT; ++i)
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mGlFormatParams.mInternalFormat, mWidth, mHeight, 0,
                   mGlFormatParams.mFormat, mGlFormatParams.mDataType, mDatas[i]);

    mHaveMipMapsBeenGenerated = false;
  }

}  // namespace wren

// C interface implementation
WrTextureCubeMap *wr_texture_cubemap_new() {
  return reinterpret_cast<WrTextureCubeMap *>(wren::TextureCubeMap::createTextureCubeMap());
}

void wr_texture_cubemap_set_data(WrTextureCubeMap *texture, const char *data, WrTextureOrientation orientation) {
  reinterpret_cast<wren::TextureCubeMap *>(texture)->setData(data, orientation);
}

void wr_texture_cubemap_disable_automatic_mip_map_generation(WrTextureCubeMap *texture) {
  reinterpret_cast<wren::TextureCubeMap *>(texture)->disableAutomaticMipMapGeneration();
}
