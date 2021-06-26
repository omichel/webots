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

#include "TextureRtt.hpp"

#include "GlState.hpp"

#include <wren/texture_rtt.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  TextureRtt *TextureRtt::copyTextureRtt(const TextureRtt *original) {
    TextureRtt *copy = TextureRtt::createTextureRtt();
    copy->setSize(original->mWidth, original->mHeight);
    copy->mGlFormatParams = original->mGlFormatParams;
    copy->mIsTranslucent = original->mIsTranslucent;

    return copy;
  }

  TextureRtt::TextureRtt() : mGlName(0), mInitializeData(false) {}

  void TextureRtt::prepareGl() {
    mGlName = generateNewTexture();

    if (textureUnit() < 0)
      setTextureUnit(0);

    Texture::UsageParams params(Texture::DEFAULT_USAGE_PARAMS);
    params.mAreMipMapsEnabled = false;
    bind(params);

    if (mInitializeData) {
      char *data = Texture::generateBlackData(mGlFormatParams, mWidth, mHeight);
      glTexImage2D(GL_TEXTURE_2D, 0, mGlFormatParams.mInternalFormat, mWidth, mHeight, 0, mGlFormatParams.mFormat,
                   mGlFormatParams.mDataType, data);
      delete[] data;
    } else
      glTexImage2D(GL_TEXTURE_2D, 0, mGlFormatParams.mInternalFormat, mWidth, mHeight, 0, mGlFormatParams.mFormat,
                   mGlFormatParams.mDataType, NULL);
  }

}  // namespace wren

// C interface implementation
WrTextureRtt *wr_texture_rtt_new() {
  return reinterpret_cast<WrTextureRtt *>(wren::TextureRtt::createTextureRtt());
}

void wr_texture_rtt_enable_initialize_data(WrTextureRtt *texture, bool enable) {
  reinterpret_cast<wren::TextureRtt *>(texture)->initializeData(enable);
}
