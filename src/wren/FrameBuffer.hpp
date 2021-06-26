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

#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include "Constants.hpp"
#include "GlUser.hpp"
#include "Texture.hpp"

#include <vector>

namespace wren {

  class TextureRtt;
  class Viewport;

  // Represents an off-screen buffer that can be drawn to.
  // Will store the results of a drawing operation in its attached texture(s).
  class FrameBuffer : public GlUser {
  public:
    struct RenderBuffer {
      explicit RenderBuffer(const Texture::GlFormatParams &params) : mGlName(0), mGlFormatParams(params) {}
      unsigned int mGlName;
      Texture::GlFormatParams mGlFormatParams;
    };

    struct DrawBuffer {
      DrawBuffer(bool isRenderBuffer, size_t storageIndex) :
        mIsRenderBuffer(isRenderBuffer),
        mIsEnabled(true),
        mStorageIndex(storageIndex),
        mGlNamePbo(0) {}
      bool mIsRenderBuffer;
      bool mIsEnabled;
      size_t mStorageIndex;
      unsigned int mGlNamePbo;
    };

    // Encapsulate memory management
    static FrameBuffer *createFrameBuffer() { return new FrameBuffer(); }
    static void deleteFrameBuffer(FrameBuffer *frameBuffer);
    static void swapTexture(TextureRtt *texture);

    void appendOutputTexture(TextureRtt *texture);
    void appendOutputTextureDisable(TextureRtt *texture);
    void appendOutputRenderBuffer(WrTextureInternalFormat format);
    void setDepthTexture(TextureRtt *texture) { mDepthTexture = texture; }
    void enableDepthBuffer(bool enable) { mIsDepthBufferEnabled = enable; }
    void enableCopying(size_t index, bool enable);
    void enableDrawBuffer(size_t index, bool enable);
    void disableAllDrawBuffers();
    void setSize(int width, int height) {
      mWidth = width;
      mHeight = height;
    }

    TextureRtt *outputTexture(size_t index) const {
      assert(index < mOutputTextures.size());
      return mOutputTextures[index];
    }

    const std::vector<TextureRtt *> &outputTextures() const { return mOutputTextures; }

    TextureRtt *depthTexture() const { return mDepthTexture; }
    bool isCopyingEnabled() const { return mIsCopyingEnabled; }
    int width() const { return mWidth; }
    int height() const { return mHeight; }

    unsigned int glName() const { return mGlName; }

    void setup();
    void blitToScreen();
    void bind();
    void release();

    void initiateCopyToPbo();
    void copyContents(size_t index, void *data);
    void copyPixel(size_t index, int x, int y, void *data, bool flipY = true);
    void copyDepthPixel(int x, int y, void *data, bool flipY = true);

    // Blits the contents of color attachment 'index' to the currently set draw buffer.
    // If width or height are 0, the width and height of the framebuffer is used instead.
    void blit(size_t index, bool blitColor = true, bool blitDepth = false, bool blitStencil = false, int srcX = 0, int srcY = 0,
              int srcWidth = 0, int srcHeight = 0, int destX = 0, int destY = 0, int destWidth = 0, int destHeight = 0) const;

  private:
    FrameBuffer();
    ~FrameBuffer() {}

    const Texture::GlFormatParams &drawBufferFormat(size_t index) const;

    void prepareGl() override;
    void cleanupGl() override;

    unsigned int mGlName;
    unsigned int mGlNameDepthBuffer;

    bool mIsDepthBufferEnabled;
    bool mIsCopyingEnabled;
    int mWidth;
    int mHeight;
    TextureRtt *mDepthTexture;
    std::vector<TextureRtt *> mOutputTextures;
    std::vector<RenderBuffer> mOutputRenderBuffers;
    std::vector<DrawBuffer> mOutputDrawBuffers;
  };

}  // namespace wren

#endif  // FRAME_BUFFER_HPP
