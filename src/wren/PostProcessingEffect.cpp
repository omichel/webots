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

#include "PostProcessingEffect.hpp"

#include "Debug.hpp"
#include "FrameBuffer.hpp"
#include "GlState.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "StaticMesh.hpp"
#include "TextureRtt.hpp"
#include "Viewport.hpp"

#include <wren/post_processing_effect.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <algorithm>

namespace wren {

  void PostProcessingEffect::Pass::setup() {
    assert(!mFrameBuffer);
    assert(mOutputWidth && mOutputHeight && mOutputTextureFormat.size());
    assert(mInputTextures.size() <= gMaxShaderTextures);
    assert(mOutputTextureFormat.size() <= static_cast<size_t>(glstate::maxFrameBufferDrawBuffers()));

    mFrameBuffer = FrameBuffer::createFrameBuffer();
    mFrameBuffer->setSize(mOutputWidth, mOutputHeight);

    for (size_t i = 0; i < mOutputTextureFormat.size(); ++i) {
      TextureRtt *outputTexture = TextureRtt::createTextureRtt();
      outputTexture->setInternalFormat(mOutputTextureFormat[i]);
      mFrameBuffer->appendOutputTexture(outputTexture);
    }

    // If during a shader invocation a texture is sampled and written to at the same time,
    // the results are undefined. So if a texture is part of the input textures and also used
    // as a color attachment by the framebuffer, a copy of the texture is created and one texture
    // is sampled while the other is written to. The roles are then alternated every frame.
    for (InputOutputTexture &inputOutput : mInputOutputTextures) {
      inputOutput.mTextureEven = mFrameBuffer->outputTexture(inputOutput.mOutputTextureIndexEven);
      inputOutput.mTextureOdd = TextureRtt::copyTextureRtt(inputOutput.mTextureEven);
      inputOutput.mOutputTextureIndexOdd = mFrameBuffer->outputTextures().size();
      mFrameBuffer->appendOutputTexture(inputOutput.mTextureOdd);

#ifdef __EMSCRIPTEN__
      mFrameBuffer->enableDrawBuffer(inputOutput.mOutputTextureIndexEven, true);
      mFrameBuffer->enableDrawBuffer(inputOutput.mOutputTextureIndexOdd, false);
      mInputTextures[inputOutput.mInputTextureIndex] = inputOutput.mTextureOdd;
#else
      mInputTextures[inputOutput.mInputTextureIndex] = inputOutput.mTextureEven;
#endif
    }

    mFrameBuffer->setup();
  }

  void PostProcessingEffect::Pass::processConnections() {
    for (const Connection &connection : mConnections) {
      if (connection.mTo == this) {
        assert(mInputTextures[connection.mInputIndex] == NULL);
        mInputTextures[connection.mInputIndex] = connection.mFrom->outputTexture(connection.mOutputIndex);
      }
    }
  }

  void PostProcessingEffect::Pass::apply() {
    assert(mProgram);
    assert(mFrameBuffer);
    assert(mFrameBuffer->outputTexture(0));

    glstate::setBlend(mUseAlphaBlending);

    for (auto &element : mProgramParameters)
      mProgram->setCustomUniformValue(element.first, *element.second);

    mProgram->bind();

    int locationIterationNumber = mProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_ITERATION_NUMBER);
    int locationViewportSize = mProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    if (locationViewportSize >= 0)
      glUniform2f(locationViewportSize, mFrameBuffer->width(), mFrameBuffer->height());

    for (int iteration = 0; iteration < mIterationCount; ++iteration) {
#ifdef __EMSCRIPTEN__
      if (iteration != 0)
#endif
        swapInputOutputTextures();

      // this call also sets the drawbuffers, so it must happen after swapInputOutputTextures
      mFrameBuffer->bind();
      glViewport(0, 0, mFrameBuffer->width(), mFrameBuffer->height());

      if (locationIterationNumber >= 0)
        glUniform1i(mProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_ITERATION_NUMBER), iteration);

      for (size_t i = 0; i < mInputTextures.size(); ++i) {
        if (mInputTextures[i]) {
          mInputTextures[i]->setTextureUnit(i);
          mInputTextures[i]->bind(mInputTextureParams[i]);

          const int locationTexture =
            mProgram->uniformLocation(static_cast<WrGlslLayoutUniform>(WR_GLSL_LAYOUT_UNIFORM_TEXTURE0 + i));
          assert(locationTexture >= 0);
          glUniform1i(locationTexture, mInputTextures[i]->textureUnit());
        }
      }
      if (mClearBeforeDraw) {
        GLfloat currentClearColor[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE, currentClearColor);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        mMesh->render(GL_TRIANGLES);
        glClearColor(currentClearColor[0], currentClearColor[1], currentClearColor[2], currentClearColor[3]);
      } else
        mMesh->render(GL_TRIANGLES);
    }
  }

  TextureRtt *PostProcessingEffect::Pass::outputTexture(size_t index) {
    // For InputOutputTextures, forward the texture that was most recently rendered to
    auto it = std::find_if(mInputOutputTextures.cbegin(), mInputOutputTextures.cend(),
                           [&](const InputOutputTexture &t) { return t.mOutputTextureIndexEven == index; });

    if (it != mInputOutputTextures.cend()) {
      if (mInputTextures[it->mInputTextureIndex] == it->mTextureOdd)
        return it->mTextureEven;
      else
        return it->mTextureOdd;
    }

    return mFrameBuffer->outputTexture(index);
  }

  PostProcessingEffect::Pass::Pass() :
    mFrameBuffer(NULL),
    mMesh(StaticMesh::createQuad()),
    mProgram(NULL),
    mIterationCount(1),
    mOutputWidth(0),
    mOutputHeight(0),
    mClearBeforeDraw(false),
    mUseAlphaBlending(true) {}

  PostProcessingEffect::Pass::~Pass() {
    if (mFrameBuffer) {
      for (TextureRtt *texture : mFrameBuffer->outputTextures())
        Texture::deleteTexture(texture);

      FrameBuffer::deleteFrameBuffer(mFrameBuffer);
    }

    StaticMesh::deleteMesh(mMesh);
  }

  void PostProcessingEffect::Pass::swapInputOutputTextures() {
    if (!mInputOutputTextures.size())
      return;

    for (const InputOutputTexture &inputOutput : mInputOutputTextures) {
      if (mInputTextures[inputOutput.mInputTextureIndex] == inputOutput.mTextureOdd) {
        // write to odd texture, sample from even texture
#ifdef __EMSCRIPTEN__
        mFrameBuffer->swapTexture(inputOutput.mTextureOdd);
#else
        mFrameBuffer->enableDrawBuffer(inputOutput.mOutputTextureIndexEven, false);
        mFrameBuffer->enableDrawBuffer(inputOutput.mOutputTextureIndexOdd, true);
#endif
        mInputTextures[inputOutput.mInputTextureIndex] = inputOutput.mTextureEven;

        for (Connection &connection : mConnections) {
          if (connection.mOutputIndex == inputOutput.mOutputTextureIndexEven && connection.mFrom == this)
            connection.mTo->mInputTextures[connection.mInputIndex] = inputOutput.mTextureOdd;
        }
      } else {
        // write to even texture, sample form odd texture
#ifdef __EMSCRIPTEN__
        mFrameBuffer->swapTexture(inputOutput.mTextureEven);
#else
        mFrameBuffer->enableDrawBuffer(inputOutput.mOutputTextureIndexEven, true);
        mFrameBuffer->enableDrawBuffer(inputOutput.mOutputTextureIndexOdd, false);
#endif
        mInputTextures[inputOutput.mInputTextureIndex] = inputOutput.mTextureOdd;

        for (Connection &connection : mConnections) {
          if (connection.mOutputIndex == inputOutput.mOutputTextureIndexEven && connection.mFrom == this)
            connection.mTo->mInputTextures[connection.mInputIndex] = inputOutput.mTextureEven;
        }
      }
    }
  }

  void PostProcessingEffect::connect(Pass *from, int outputIndex, Pass *to, int inputIndex) {
    assert(std::find(mPasses.cbegin(), mPasses.cend(), from) != mPasses.cend());
    assert(std::find(mPasses.cbegin(), mPasses.cend(), to) != mPasses.cend());
    assert(outputIndex < from->outputTextureCount());
    assert(inputIndex < to->inputTextureCount());

    Pass::Connection connection(from, outputIndex, to, inputIndex);
    if (from != to) {
      from->addConnection(connection);
      to->addConnection(connection);
    } else
      from->addInputOutputTexture(inputIndex, outputIndex);
  }

  void PostProcessingEffect::setup() {
    assert(mPasses.size());
    assert(!mInputFrameBuffer || mInputFrameBuffer->outputTexture(0));

    if (mInputFrameBuffer)
      mPasses.front()->setInputTexture(0, mInputFrameBuffer->outputTexture(0));

    for (Pass *pass : mPasses)
      pass->setup();

    for (Pass *pass : mPasses)
      pass->processConnections();

    printPasses();
  }

  void PostProcessingEffect::apply() {
    assert(mResultProgram);
    assert(!mResultFrameBuffer || mResultFrameBuffer->outputTexture(0));

    for (size_t i = 0; i < mPasses.size(); ++i)
      mPasses[i]->apply();

    renderToResultFrameBuffer();

    for (size_t i = 0; i < mPasses.size(); ++i) {
      for (Texture *texture : mPasses[i]->inputTextures()) {
        if (texture)
          texture->release();
      }
    }
  }

  PostProcessingEffect::PostProcessingEffect() :
    mResultProgram(NULL),
    mInputFrameBuffer(NULL),
    mResultFrameBuffer(NULL),
    mMesh(StaticMesh::createQuad()),
    mDrawingIndex(0) {}

  PostProcessingEffect::~PostProcessingEffect() {
    for (Pass *pass : mPasses)
      Pass::deletePass(pass);

    StaticMesh::deleteMesh(mMesh);
  }

  void PostProcessingEffect::renderToResultFrameBuffer() {
    if (mResultFrameBuffer) {
      // Causes a bug when the shadows are on in the streaming-viewer if we don't disable this buffer here.
#ifdef __EMSCRIPTEN__
      mResultFrameBuffer->enableDrawBuffer(1, false);
#endif
      mResultFrameBuffer->bind();
#ifdef __EMSCRIPTEN__
      mResultFrameBuffer->enableDrawBuffer(1, true);
#endif

      glViewport(0, 0, mResultFrameBuffer->width(), mResultFrameBuffer->height());
    } else
      assert(false);

    mResultProgram->bind();
    glstate::setDepthMask(false);
    glstate::setDepthTest(false);
    glstate::setFrontFace(GL_CCW);
    Texture::UsageParams params(Texture::DEFAULT_USAGE_PARAMS);
    params.mIsInterpolationEnabled = false;
    params.mAreMipMapsEnabled = false;
    mPasses.back()->outputTexture(0)->setTextureUnit(0);
    mPasses.back()->outputTexture(0)->bind(params);

    glUniform1i(mResultProgram->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_TEXTURE0),
                mPasses.back()->outputTexture(0)->textureUnit());

    mMesh->render(GL_TRIANGLES);

    if (mResultFrameBuffer) {
      if (mResultFrameBuffer->isCopyingEnabled())
        mResultFrameBuffer->initiateCopyToPbo();
    }
  }

}  // namespace wren

// C interface implementation
WrPostProcessingEffectPass *wr_post_processing_effect_pass_new() {
  return reinterpret_cast<WrPostProcessingEffectPass *>(wren::PostProcessingEffect::Pass::createPass());
}

void wr_post_processing_effect_pass_delete(WrPostProcessingEffectPass *pass) {
  wren::PostProcessingEffect::Pass::deletePass(reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass));
}

void wr_post_processing_effect_pass_set_name(WrPostProcessingEffectPass *pass, const char *name) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setName(name);
}

void wr_post_processing_effect_pass_set_program(WrPostProcessingEffectPass *pass, WrShaderProgram *program) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_post_processing_effect_pass_set_program_parameter(WrPostProcessingEffectPass *pass, const char *parameter_name,
                                                          const char *value) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setProgramParameter(parameter_name, value);
}

void wr_post_processing_effect_pass_set_output_size(WrPostProcessingEffectPass *pass, int width, int height) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setOutputSize(width, height);
}

void wr_post_processing_effect_pass_set_input_texture_count(WrPostProcessingEffectPass *pass, int count) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setInputTextureCount(count);
}

void wr_post_processing_effect_pass_set_output_texture_count(WrPostProcessingEffectPass *pass, int count) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setOutputTextureCount(count);
}

void wr_post_processing_effect_pass_set_input_texture_wrap_mode(WrPostProcessingEffectPass *pass, int index,
                                                                WrTextureWrapMode mode) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setInputTextureWrapMode(index, mode);
}

void wr_post_processing_effect_pass_set_input_texture_interpolation(WrPostProcessingEffectPass *pass, int index, bool enable) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setInputTextureInterpolation(index, enable);
}

void wr_post_processing_effect_pass_set_output_texture_format(WrPostProcessingEffectPass *pass, int index,
                                                              WrTextureInternalFormat format) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setOutputTextureFormat(index, format);
}

void wr_post_processing_effect_pass_set_input_texture(WrPostProcessingEffectPass *pass, int index, WrTexture *texture) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setInputTexture(index,
                                                                              reinterpret_cast<wren::Texture *>(texture));
}

void wr_post_processing_effect_pass_set_iteration_count(WrPostProcessingEffectPass *pass, int count) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setIterationCount(count);
}

WrTextureRtt *wr_post_processing_effect_pass_get_output_texture(WrPostProcessingEffectPass *pass, int index) {
  return reinterpret_cast<WrTextureRtt *>(reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->outputTexture(index));
}

void wr_post_processing_effect_pass_set_clear_before_draw(WrPostProcessingEffectPass *pass, bool enable) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setClearBeforeDraw(enable);
}

void wr_post_processing_effect_pass_set_alpha_blending(WrPostProcessingEffectPass *pass, bool enable) {
  reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass)->setAlphaBlending(enable);
}

WrPostProcessingEffect *wr_post_processing_effect_new() {
  return reinterpret_cast<WrPostProcessingEffect *>(wren::PostProcessingEffect::createPostProcessingEffect());
}

void wr_post_processing_effect_delete(WrPostProcessingEffect *post_processing_effect) {
  wren::PostProcessingEffect::deletePostProcessingEffect(
    reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect));
}

void wr_post_processing_effect_append_pass(WrPostProcessingEffect *post_processing_effect, WrPostProcessingEffectPass *pass) {
  reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)
    ->appendPass(reinterpret_cast<wren::PostProcessingEffect::Pass *>(pass));
}

void wr_post_processing_effect_connect(WrPostProcessingEffect *post_processing_effect, WrPostProcessingEffectPass *from,
                                       int output_index, WrPostProcessingEffectPass *to, int input_index) {
  reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)
    ->connect(reinterpret_cast<wren::PostProcessingEffect::Pass *>(from), output_index,
              reinterpret_cast<wren::PostProcessingEffect::Pass *>(to), input_index);
}

void wr_post_processing_effect_set_input_frame_buffer(WrPostProcessingEffect *post_processing_effect,
                                                      WrFrameBuffer *frame_buffer) {
  reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)
    ->setInputFrameBuffer(reinterpret_cast<wren::FrameBuffer *>(frame_buffer));
}

void wr_post_processing_effect_set_result_program(WrPostProcessingEffect *post_processing_effect, WrShaderProgram *program) {
  reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)
    ->setResultProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_post_processing_effect_set_result_frame_buffer(WrPostProcessingEffect *post_processing_effect,
                                                       WrFrameBuffer *frame_buffer) {
  reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)
    ->setResultFrameBuffer(reinterpret_cast<wren::FrameBuffer *>(frame_buffer));
}

WrPostProcessingEffectPass *wr_post_processing_effect_get_first_pass(WrPostProcessingEffect *post_processing_effect) {
  return reinterpret_cast<WrPostProcessingEffectPass *>(
    reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)->firstPass());
}

WrPostProcessingEffectPass *wr_post_processing_effect_get_last_pass(WrPostProcessingEffect *post_processing_effect) {
  return reinterpret_cast<WrPostProcessingEffectPass *>(
    reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)->lastPass());
}

WrPostProcessingEffectPass *wr_post_processing_effect_get_pass(WrPostProcessingEffect *post_processing_effect,
                                                               const char *name) {
  return reinterpret_cast<WrPostProcessingEffectPass *>(
    reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)->pass(name));
}

void wr_post_processing_effect_set_drawing_index(WrPostProcessingEffect *post_processing_effect, unsigned int index) {
  reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)->setDrawingIndex(index);
}

void wr_post_processing_effect_setup(WrPostProcessingEffect *post_processing_effect) {
  reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)->setup();
}

void wr_post_processing_effect_apply(WrPostProcessingEffect *post_processing_effect) {
  reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect)->apply();
}
