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

#include "FrameBuffer.hpp"

#include "Config.hpp"
#include "Debug.hpp"
#include "GlState.hpp"
#include "TextureRtt.hpp"

#include <wren/frame_buffer.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <emscripten.h>
#else
#include <glad/glad.h>
#endif

#include <algorithm>
#include <numeric>

namespace wren {

  void FrameBuffer::deleteFrameBuffer(FrameBuffer *frameBuffer) {
    if (!frameBuffer)
      return;

    if (glstate::isContextActive()) {
      frameBuffer->cleanupGl();
      delete frameBuffer;
    } else
      frameBuffer->setRequireAction(GlUser::GL_ACTION_DELETE);
  }

  void FrameBuffer::appendOutputTexture(TextureRtt *texture) {
    assert(mOutputDrawBuffers.size() <= static_cast<size_t>(glstate::maxFrameBufferDrawBuffers()));

    mOutputDrawBuffers.push_back(DrawBuffer(false, mOutputTextures.size()));
    mOutputTextures.push_back(texture);
  }

  void FrameBuffer::appendOutputTextureDisable(TextureRtt *texture) {
    assert(mOutputDrawBuffers.size() <= static_cast<size_t>(glstate::maxFrameBufferDrawBuffers()));

    mOutputDrawBuffers.push_back(DrawBuffer(false, mOutputTextures.size()));
    mOutputTextures.push_back(texture);

    mOutputDrawBuffers[mOutputDrawBuffers.size() - 1].mIsEnabled = false;
  }

  void FrameBuffer::appendOutputRenderBuffer(WrTextureInternalFormat format) {
    assert(mOutputDrawBuffers.size() <= static_cast<size_t>(glstate::maxFrameBufferDrawBuffers()));

    mOutputDrawBuffers.push_back(DrawBuffer(true, mOutputRenderBuffers.size()));
    mOutputRenderBuffers.push_back(RenderBuffer(Texture::GL_FORMAT_PARAMS[format]));
  }

  void FrameBuffer::enableCopying(size_t index, bool enable) {
    assert(index < mOutputDrawBuffers.size());

    if (static_cast<bool>(mOutputDrawBuffers[index].mGlNamePbo) == enable)
      return;

    // Create a pixel buffer object. This is lets us asynchronously copy
    // the framebuffer output to CPU memory at a later stage.
    if (enable) {
      const Texture::GlFormatParams &params = drawBufferFormat(index);
      glGenBuffers(1, &mOutputDrawBuffers[index].mGlNamePbo);
      glstate::bindPixelPackBuffer(mOutputDrawBuffers[index].mGlNamePbo);
      glBufferData(GL_PIXEL_PACK_BUFFER, mWidth * mHeight * params.mPixelSize, NULL, GL_STREAM_READ);
      glstate::releasePixelPackBuffer(mOutputDrawBuffers[index].mGlNamePbo);
      initiateCopyToPbo();
    } else {
      glDeleteBuffers(1, &mOutputDrawBuffers[index].mGlNamePbo);
      mOutputDrawBuffers[index].mGlNamePbo = 0;
    }

    mIsCopyingEnabled = false;
    for (const DrawBuffer &buffer : mOutputDrawBuffers) {
      if (buffer.mGlNamePbo)
        mIsCopyingEnabled = true;
    }
  }

  void FrameBuffer::enableDrawBuffer(size_t index, bool enable) {
    assert(index < mOutputDrawBuffers.size());

    mOutputDrawBuffers[index].mIsEnabled = enable;
  }

  void FrameBuffer::disableAllDrawBuffers() {
    for (DrawBuffer &drawBuffer : mOutputDrawBuffers)
      drawBuffer.mIsEnabled = false;
  }

  void FrameBuffer::setup() {
    if (glstate::isContextActive())
      prepareGl();
    else
      setRequireAction(GlUser::GL_ACTION_PREPARE);
  }

  void FrameBuffer::bind() {
    glstate::bindFrameBuffer(mGlName);

    std::vector<unsigned int> drawBuffers;
    for (size_t i = 0; i < mOutputDrawBuffers.size(); ++i) {
      if (mOutputDrawBuffers[i].mIsEnabled)
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
  }

  void FrameBuffer::blitToScreen() {
    glstate::bindDrawFrameBuffer(0);

    blit(0, true, false, false, 0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight);
  }

  void FrameBuffer::release() { glstate::releaseFrameBuffer(mGlName); }

  void FrameBuffer::initiateCopyToPbo() {
    if (!mGlName)
      return;

    const unsigned int currentReadFrameBuffer = glstate::boundReadFrameBuffer();
    const unsigned int currentPixelPackBuffer = glstate::boundPixelPackBuffer();
    glstate::bindReadFrameBuffer(mGlName);

    for (size_t i = 0; i < mOutputDrawBuffers.size(); ++i) {
      if (mOutputDrawBuffers[i].mGlNamePbo) {
        const Texture::GlFormatParams &params = drawBufferFormat(i);

        glReadBuffer(GL_COLOR_ATTACHMENT0 + i);

        glstate::bindPixelPackBuffer(mOutputDrawBuffers[i].mGlNamePbo);

        glReadPixels(0, 0, mWidth, mHeight, params.mFormat, params.mDataType, 0);

        glstate::releasePixelPackBuffer(mOutputDrawBuffers[i].mGlNamePbo);
      }
    }

    glstate::bindPixelPackBuffer(currentPixelPackBuffer);
    glstate::bindReadFrameBuffer(currentReadFrameBuffer);
  }

  void FrameBuffer::copyContents(size_t index, void *data) {
    assert(index < mOutputDrawBuffers.size());
    assert(mOutputDrawBuffers[index].mGlNamePbo);

    const unsigned int currentPixelPackBuffer = glstate::boundPixelPackBuffer();
    glstate::bindPixelPackBuffer(mOutputDrawBuffers[index].mGlNamePbo);

    const Texture::GlFormatParams &params = drawBufferFormat(index);
    const int rowSizeInBytes = params.mPixelSize * mWidth;
    const int totalSizeInBytes = rowSizeInBytes * mHeight;

    void *start = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, totalSizeInBytes, GL_MAP_READ_BIT);
    assert(start);
    memcpy(data, start, totalSizeInBytes);

    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

    glstate::bindPixelPackBuffer(currentPixelPackBuffer);
  }

  void FrameBuffer::copyPixel(size_t index, int x, int y, void *data, bool flipY) {
    assert(index < mOutputDrawBuffers.size());
    assert(mOutputDrawBuffers[index].mGlNamePbo);

    const unsigned int currentPixelPackBuffer = glstate::boundPixelPackBuffer();
    glstate::bindPixelPackBuffer(mOutputDrawBuffers[index].mGlNamePbo);

    const Texture::GlFormatParams &params = drawBufferFormat(index);
    const int rowIndex = flipY ? (mHeight - 1 - y) : y;

#ifdef __EMSCRIPTEN__
    int offset = params.mPixelSize * (rowIndex * mWidth + x);
    EM_ASM_({ Module.ctx.getBufferSubData(Module.ctx.PIXEL_PACK_BUFFER, $2, HEAPU8.subarray($0, $0 + $1)); }, data,
            params.mPixelSize, offset);
#else
    glGetBufferSubData(GL_PIXEL_PACK_BUFFER, params.mPixelSize * (rowIndex * mWidth + x), params.mPixelSize, data);
#endif
    glstate::bindPixelPackBuffer(currentPixelPackBuffer);
  }

  void FrameBuffer::copyDepthPixel(int x, int y, void *data, bool flipY) {
    assert(mIsDepthBufferEnabled || mDepthTexture);

    const unsigned int currentReadFrameBuffer = glstate::boundReadFrameBuffer();

    glstate::bindReadFrameBuffer(mGlName);

#ifdef __EMSCRIPTEN__
    glReadPixels(x, (flipY ? mHeight - 1 - y : y), 1, 1, GL_RGBA, GL_FLOAT, data);
#else
    glReadPixels(x, (flipY ? mHeight - 1 - y : y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, data);
#endif
    if (config::requiresDepthBufferDistortion()) {
      GLfloat *fData = (GLfloat *)data;
      fData[0] = fData[0] * fData[0];
    }
    glstate::bindReadFrameBuffer(currentReadFrameBuffer);
  }

  void FrameBuffer::blit(size_t index, bool blitColor, bool blitDepth, bool blitStencil, int srcX, int srcY, int srcWidth,
                         int srcHeight, int destX, int destY, int destWidth, int destHeight) const {
    unsigned int frameBufferGlName = mGlName;

    assert(frameBufferGlName);
    assert(!blitDepth || (mIsDepthBufferEnabled || mDepthTexture));
    assert(!blitStencil || (mIsDepthBufferEnabled ||
                            (mDepthTexture && mDepthTexture->glFormatParams().mInternalFormat == GL_DEPTH24_STENCIL8)));

    if (!blitColor && !blitDepth && !blitStencil)
      return;

    int flags = 0;
    if (blitColor)
      flags |= GL_COLOR_BUFFER_BIT;
    if (blitDepth)
      flags |= GL_DEPTH_BUFFER_BIT;
    if (blitStencil)
      flags |= GL_STENCIL_BUFFER_BIT;

    const unsigned int currentReadFrameBuffer = glstate::boundReadFrameBuffer();

    glstate::bindReadFrameBuffer(frameBufferGlName);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferGlName);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
    glBlitFramebuffer(srcX, srcY, srcX + (srcWidth ? srcWidth : mWidth), srcY + (srcHeight ? srcHeight : mHeight), destX, destY,
                      destX + (destWidth ? destWidth : mWidth), destY + (destHeight ? destHeight : mHeight), flags, GL_NEAREST);

    glstate::bindReadFrameBuffer(currentReadFrameBuffer);
  }

  FrameBuffer::FrameBuffer() :
    mGlName(0),
    mGlNameDepthBuffer(0),
    mIsDepthBufferEnabled(false),
    mIsCopyingEnabled(false),
    mWidth(0),
    mHeight(0),
    mDepthTexture(NULL) {}

  const Texture::GlFormatParams &FrameBuffer::drawBufferFormat(size_t index) const {
    if (mOutputDrawBuffers[index].mIsRenderBuffer)
      return mOutputRenderBuffers[mOutputDrawBuffers[index].mStorageIndex].mGlFormatParams;
    else
      return mOutputTextures[mOutputDrawBuffers[index].mStorageIndex]->glFormatParams();
  }

  void FrameBuffer::swapTexture(TextureRtt *texture) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->glName(), 0);
  }

  void FrameBuffer::prepareGl() {
    assert(!mGlName);
    assert(mWidth && mHeight);

    const unsigned int currentReadFrameBuffer = glstate::boundReadFrameBuffer();
    const unsigned int currentDrawFrameBuffer = glstate::boundDrawFrameBuffer();

    glGenFramebuffers(1, &mGlName);
    glstate::bindFrameBuffer(mGlName);

    assert(mOutputDrawBuffers.size() <= static_cast<size_t>(glstate::maxFrameBufferDrawBuffers()));
    for (size_t i = 0; i < mOutputDrawBuffers.size(); ++i) {
      // output draw buffers can be backed either by a renderbuffer or by a texture
      if (mOutputDrawBuffers[i].mIsRenderBuffer) {
        RenderBuffer &renderBuffer = mOutputRenderBuffers[mOutputDrawBuffers[i].mStorageIndex];
        glGenRenderbuffers(1, &renderBuffer.mGlName);
        glstate::bindRenderBuffer(renderBuffer.mGlName);
        glRenderbufferStorage(GL_RENDERBUFFER, renderBuffer.mGlFormatParams.mInternalFormat, mWidth, mHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, renderBuffer.mGlName);
      } else {
        TextureRtt *texture = mOutputTextures[mOutputDrawBuffers[i].mStorageIndex];
        if (!texture->glName()) {
          texture->setSize(mWidth, mHeight);
          texture->prepareGl();
        } else
          assert(texture->width() == mWidth && texture->height() == mHeight);

        if (mOutputDrawBuffers[i].mIsEnabled)
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->glName(), 0);
      }
    }

    if (mIsDepthBufferEnabled) {
      if (mDepthTexture) {
        assert(mDepthTexture->glFormatParams().mInternalFormat == GL_DEPTH24_STENCIL8 ||
               mDepthTexture->glFormatParams().mInternalFormat == GL_DEPTH_COMPONENT32F);
        if (!mDepthTexture->glName()) {
          mDepthTexture->setSize(mWidth, mHeight);
          mDepthTexture->prepareGl();
        }

        if (mDepthTexture->glFormatParams().mInternalFormat == GL_DEPTH24_STENCIL8)
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture->glName(), 0);
        else
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture->glName(), 0);
      } else {
        glGenRenderbuffers(1, &mGlNameDepthBuffer);
        glstate::bindRenderBuffer(mGlNameDepthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, mWidth, mHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mGlNameDepthBuffer);
      }
    }

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glstate::bindRenderBuffer(0);
    glstate::bindReadFrameBuffer(currentReadFrameBuffer);
    glstate::bindDrawFrameBuffer(currentDrawFrameBuffer);
  }

  void FrameBuffer::cleanupGl() {
    if (mGlName) {
      release();

      for (size_t i = 0; i < mOutputDrawBuffers.size(); ++i) {
        if (mOutputDrawBuffers[i].mGlNamePbo)
          glDeleteBuffers(1, &mOutputDrawBuffers[i].mGlNamePbo);

        if (mOutputDrawBuffers[i].mIsRenderBuffer)
          glDeleteRenderbuffers(1, &mOutputRenderBuffers[mOutputDrawBuffers[i].mStorageIndex].mGlName);
      }

      glDeleteFramebuffers(1, &mGlName);

      if (mIsDepthBufferEnabled && !mDepthTexture)
        glDeleteRenderbuffers(1, &mGlNameDepthBuffer);
    }
  }

}  // namespace wren

// C interface implementation
WrFrameBuffer *wr_frame_buffer_new() {
  return reinterpret_cast<WrFrameBuffer *>(wren::FrameBuffer::createFrameBuffer());
}

void wr_frame_buffer_delete(WrFrameBuffer *frame_buffer) {
  wren::FrameBuffer::deleteFrameBuffer(reinterpret_cast<wren::FrameBuffer *>(frame_buffer));
}

void wr_frame_buffer_append_output_texture(WrFrameBuffer *frame_buffer, WrTextureRtt *texture) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->appendOutputTexture(reinterpret_cast<wren::TextureRtt *>(texture));
}

void wr_frame_buffer_append_output_texture_disable(WrFrameBuffer *frame_buffer, WrTextureRtt *texture) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)
    ->appendOutputTextureDisable(reinterpret_cast<wren::TextureRtt *>(texture));
}

void wr_frame_buffer_set_depth_texture(WrFrameBuffer *frame_buffer, WrTextureRtt *texture) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->setDepthTexture(reinterpret_cast<wren::TextureRtt *>(texture));
}

void wr_frame_buffer_enable_depth_buffer(WrFrameBuffer *frame_buffer, bool enable) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->enableDepthBuffer(enable);
}

void wr_frame_buffer_enable_copying(WrFrameBuffer *frame_buffer, int index, bool enable) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->enableCopying(index, enable);
}

void wr_frame_buffer_set_size(WrFrameBuffer *frame_buffer, int width, int height) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->setSize(width, height);
}

WrTextureRtt *wr_frame_buffer_get_output_texture(WrFrameBuffer *frame_buffer, int index) {
  return reinterpret_cast<WrTextureRtt *>(reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->outputTexture(index));
}

WrTextureRtt *wr_frame_buffer_get_depth_texture(WrFrameBuffer *frame_buffer) {
  return reinterpret_cast<WrTextureRtt *>(reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->depthTexture());
}

void wr_frame_buffer_setup(WrFrameBuffer *frame_buffer) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->setup();
}

void wr_frame_buffer_blit_to_screen(WrFrameBuffer *frame_buffer) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->blitToScreen();
}

void wr_frame_buffer_copy_contents(WrFrameBuffer *frame_buffer, int index, void *data) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->copyContents(index, data);
}

void wr_frame_buffer_copy_pixel(WrFrameBuffer *frame_buffer, int index, int x, int y, void *data, bool flip_y) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->copyPixel(index, x, y, data, flip_y);
}

void wr_frame_buffer_copy_depth_pixel(WrFrameBuffer *frame_buffer, int x, int y, void *data, bool flip_y) {
  reinterpret_cast<wren::FrameBuffer *>(frame_buffer)->copyDepthPixel(x, y, data, flip_y);
}
