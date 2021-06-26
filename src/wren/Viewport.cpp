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

#include "Viewport.hpp"

#include "ContainerUtils.hpp"
#include "Debug.hpp"
#include "FrameBuffer.hpp"
#include "GlState.hpp"
#include "Overlay.hpp"
#include "PostProcessingEffect.hpp"
#include "TextureRtt.hpp"

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <algorithm>

namespace wren {

  void Viewport::setSize(int width, int height) {
    if (width == mWidth && height == mHeight)
      return;

    mWidth = width;
    mHeight = height;
    mIsSizeDirty = true;
  }

  void Viewport::setCamera(Camera *camera) {
    mCamera = camera;
    mCamera->setAspectRatio(static_cast<float>(mWidth) / mHeight);
  }

  void Viewport::setFrameBuffer(FrameBuffer *frameBuffer) {
    if (frameBuffer == mFrameBuffer)
      return;

    mFrameBuffer = frameBuffer;
    if (mFrameBuffer) {
      for (auto postProcessingEffect : mPostProcessingEffects) {
        postProcessingEffect->firstPass()->setInputTexture(0, mFrameBuffer->outputTexture(0));
        postProcessingEffect->setResultFrameBuffer(mFrameBuffer);
      }
    }
  }

  void Viewport::attachOverlay(Overlay *overlay) { mOverlays.push_back(overlay); }

  void Viewport::detachOverlay(Overlay *overlay) { containerutils::removeElementFromVector(mOverlays, overlay); }

  void Viewport::renderOverlay(Overlay *overlay) {
    assert(std::find(mOverlays.begin(), mOverlays.end(), overlay) != mOverlays.end());

    bind();
    clear();

    overlay->render();
  }

  void Viewport::addPostProcessingEffect(PostProcessingEffect *postProcessingEffect) {
    if (std::find(std::begin(mPostProcessingEffects), std::end(mPostProcessingEffects), postProcessingEffect) !=
        std::end(mPostProcessingEffects))
      return;

    postProcessingEffect->setInputFrameBuffer(mFrameBuffer);
    postProcessingEffect->setResultFrameBuffer(mFrameBuffer);

    // no post processing effects are present, so we don't care about the order of insertion
    if (mPostProcessingEffects.empty()) {
      mPostProcessingEffects.push_back(postProcessingEffect);
      return;
    }

    // if this is the biggest index so far, we can just insert it at the back
    if (postProcessingEffect->drawingIndex() > mPostProcessingEffects[mPostProcessingEffects.size() - 1]->drawingIndex()) {
      mPostProcessingEffects.push_back(postProcessingEffect);
      return;
    }
    // insert in ascending order of drawing index
    for (size_t i = 0; i < mPostProcessingEffects.size(); ++i) {
      assert(postProcessingEffect->drawingIndex() != mPostProcessingEffects[i]->drawingIndex());
      if (postProcessingEffect->drawingIndex() < mPostProcessingEffects[i]->drawingIndex()) {
        mPostProcessingEffects.insert(mPostProcessingEffects.begin() + i, postProcessingEffect);
        break;
      }
    }

    if (mFrameBuffer)
      postProcessingEffect->firstPass()->setInputTexture(0, mFrameBuffer->outputTexture(0));
  }

  void Viewport::removePostProcessingEffect(PostProcessingEffect *postProcessingEffect) {
    containerutils::removeElementFromVector(mPostProcessingEffects, postProcessingEffect);
    if (mAmbientOcclusionEffect == postProcessingEffect)
      mAmbientOcclusionEffect = NULL;
    else if (mAntiAliasingEffect == postProcessingEffect)
      mAntiAliasingEffect = NULL;
  }

  void Viewport::updateUniforms() const {
    assert(mCamera);

    DEBUG("Viewport::update, this=" << this);
    DEBUGONE(mWidth);
    DEBUGONE(mHeight);

    if (mSyncAspectRatio && mIsSizeDirty) {
      mCamera->setAspectRatio(static_cast<float>(mWidth) / mHeight);
      mIsSizeDirty = false;
    }

    mCamera->updateUniforms();
  }

  void Viewport::bind() const {
    DEBUG("Viewport::bind, this=" << this);

    assert(mFrameBuffer);

    mFrameBuffer->bind();

    glViewport(0, 0, width(), height());

    if (mCamera->flipY())
      glstate::setFrontFace(GL_CW);
    glstate::setPolygonMode(mPolygonMode);
  }

  void Viewport::clear() const {
    glstate::setColorMask(true, true, true, true);
    glstate::setDepthMask(true);
    glstate::setStencilMask(~0);
    glstate::setClearColor(mClearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  }

  void Viewport::finishRender() const {
    if (mCamera->flipY())
      // restore default
      glstate::setFrontFace(GL_CCW);

    if (mFrameBuffer && mFrameBuffer->isCopyingEnabled())
      mFrameBuffer->initiateCopyToPbo();
  }

  void Viewport::applyPostProcessing() {
    // post-processing always happens with default winding order, so restore it in case cameras have changed it
    if (mCamera->flipY())
      glstate::setFrontFace(GL_CCW);

    if (mPolygonMode != WR_VIEWPORT_POLYGON_MODE_FILL)
      return;

    if (!mPostProcessingEffects.empty()) {
      if (mFrameBuffer) {
        for (auto postProcessingEffect : mPostProcessingEffects) {
          postProcessingEffect->setInputFrameBuffer(mFrameBuffer);
          postProcessingEffect->setResultFrameBuffer(mFrameBuffer);
          postProcessingEffect->firstPass()->setInputTexture(0, mFrameBuffer->outputTexture(0));
        }
      }

      glstate::bindDrawFrameBuffer(0);

      clear();

      for (size_t i = 0; i < mPostProcessingEffects.size(); ++i)
        mPostProcessingEffects[i]->apply();
    }
  }

  void Viewport::applyAmbientOcclusion() {
    if (mPolygonMode != WR_VIEWPORT_POLYGON_MODE_FILL)
      return;

    if (mCamera->flipY())
      glstate::setFrontFace(GL_CCW);

    if (mAmbientOcclusionEffect) {
      if (mFrameBuffer) {
        mAmbientOcclusionEffect->setInputFrameBuffer(mFrameBuffer);
        mAmbientOcclusionEffect->setResultFrameBuffer(mFrameBuffer);
        mAmbientOcclusionEffect->firstPass()->setInputTexture(0, mFrameBuffer->outputTexture(0));
      }
      glstate::bindDrawFrameBuffer(0);

      clear();

      mAmbientOcclusionEffect->apply();
    }

    if (mCamera->flipY())
      glstate::setFrontFace(GL_CW);
  }

  void Viewport::applyAntiAliasing() {
    if (mPolygonMode != WR_VIEWPORT_POLYGON_MODE_FILL)
      return;

    if (mAntiAliasingEffect) {
      if (mFrameBuffer) {
        mAntiAliasingEffect->setInputFrameBuffer(mFrameBuffer);
        mAntiAliasingEffect->setResultFrameBuffer(mFrameBuffer);
        mAntiAliasingEffect->firstPass()->setInputTexture(0, mFrameBuffer->outputTexture(0));
      }

      glstate::bindDrawFrameBuffer(0);

      clear();

      mAntiAliasingEffect->apply();
    }
  }

  void Viewport::drawOverlays() {
    // Draw overlays
    if (!mOverlays.size())
      return;

    DEBUG("\nViewport::drawOverlays, this=" << this << ", mOverlays.size()=" << mOverlays.size());

    sortOverlays();

    glstate::setDepthMask(false);
    glstate::setDepthTest(false);
    glstate::setStencilTest(false);
    glstate::setBlend(true);
    glstate::setBlendEquation(GL_FUNC_ADD);
    glstate::setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glstate::setCullFace(true);
    glstate::setColorMask(true, true, true, true);
    glstate::setPolygonMode(GL_FILL);

    for (Overlay *overlay : mOverlays) {
      if (overlay->isVisible())
        overlay->render();
    }

    // reset viewport's polygon mode to stored value
    glstate::setPolygonMode(mPolygonMode);
  }

  Viewport::Viewport() :
    mIsSizeDirty(true),
    mSyncAspectRatio(true),
    mClearColor(gVec4ColorBlack),
    mPolygonMode(WR_VIEWPORT_POLYGON_MODE_FILL),
    mVisibilityMask(0xFFFFFFFF),
    mWidth(1),
    mHeight(1),
    mPixelRatio(1),
    mCamera(NULL),
    mFrameBuffer(NULL),
    mAreShadowsEnabled(true),
    mIsSkyboxEnabled(true),
    mAmbientOcclusionEffect(NULL),
    mAntiAliasingEffect(NULL) {}

  Viewport::~Viewport() {}

  void Viewport::sortOverlays() {
    std::sort(mOverlays.begin(), mOverlays.end(),
              [](const Overlay *a, const Overlay *b) -> bool { return a->order() < b->order(); });
  }

}  // namespace wren

// C interface implementation
WrViewport *wr_viewport_new() {
  return reinterpret_cast<WrViewport *>(wren::Viewport::createViewport());
}

void wr_viewport_delete(WrViewport *viewport) {
  wren::Viewport::deleteViewport(reinterpret_cast<wren::Viewport *>(viewport));
}

void wr_viewport_set_clear_color_rgb(WrViewport *viewport, const float *color) {
  glm::vec4 clearColor(glm::make_vec3(color), 1.0);
  reinterpret_cast<wren::Viewport *>(viewport)->setClearColor(clearColor);
}

void wr_viewport_set_clear_color_rgba(WrViewport *viewport, const float *color) {
  reinterpret_cast<wren::Viewport *>(viewport)->setClearColor(glm::make_vec4(color));
}

void wr_viewport_set_polygon_mode(WrViewport *viewport, WrViewportPolygonMode mode) {
  reinterpret_cast<wren::Viewport *>(viewport)->setPolygonMode(mode);
}

void wr_viewport_set_visibility_mask(WrViewport *viewport, int mask) {
  reinterpret_cast<wren::Viewport *>(viewport)->setVisbilityMask(mask);
}

void wr_viewport_set_size(WrViewport *viewport, int width, int height) {
  reinterpret_cast<wren::Viewport *>(viewport)->setSize(width, height);
}

void wr_viewport_set_pixel_ratio(WrViewport *viewport, int ratio) {
  reinterpret_cast<wren::Viewport *>(viewport)->setPixelRatio(ratio);
}

void wr_viewport_set_camera(WrViewport *viewport, WrCamera *camera) {
  reinterpret_cast<wren::Viewport *>(viewport)->setCamera(reinterpret_cast<wren::Camera *>(camera));
}

void wr_viewport_set_frame_buffer(WrViewport *viewport, WrFrameBuffer *frame_buffer) {
  reinterpret_cast<wren::Viewport *>(viewport)->setFrameBuffer(reinterpret_cast<wren::FrameBuffer *>(frame_buffer));
}

void wr_viewport_enable_shadows(WrViewport *viewport, bool enable) {
  reinterpret_cast<wren::Viewport *>(viewport)->enableShadows(enable);
}

void wr_viewport_enable_skybox(WrViewport *viewport, bool enable) {
  reinterpret_cast<wren::Viewport *>(viewport)->enableSkybox(enable);
}

void wr_viewport_sync_aspect_ratio_with_camera(WrViewport *viewport, bool enable) {
  reinterpret_cast<wren::Viewport *>(viewport)->enableAspectRatioSync(enable);
}

void wr_viewport_attach_overlay(WrViewport *viewport, WrOverlay *overlay) {
  reinterpret_cast<wren::Viewport *>(viewport)->attachOverlay(reinterpret_cast<wren::Overlay *>(overlay));
}

void wr_viewport_detach_overlay(WrViewport *viewport, WrOverlay *overlay) {
  reinterpret_cast<wren::Viewport *>(viewport)->detachOverlay(reinterpret_cast<wren::Overlay *>(overlay));
}

void wr_viewport_render_overlay(WrViewport *viewport, WrOverlay *overlay) {
  reinterpret_cast<wren::Viewport *>(viewport)->renderOverlay(reinterpret_cast<wren::Overlay *>(overlay));
}

void wr_viewport_render_overlays(WrViewport *viewport) {
  reinterpret_cast<wren::Viewport *>(viewport)->drawOverlays();
}

void wr_viewport_add_post_processing_effect(WrViewport *viewport, WrPostProcessingEffect *post_processing_effect) {
  reinterpret_cast<wren::Viewport *>(viewport)->addPostProcessingEffect(
    reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect));
}

void wr_viewport_remove_post_processing_effect(WrViewport *viewport, WrPostProcessingEffect *post_processing_effect) {
  reinterpret_cast<wren::Viewport *>(viewport)->removePostProcessingEffect(
    reinterpret_cast<wren::PostProcessingEffect *>(post_processing_effect));
}

void wr_viewport_set_ambient_occlusion_effect(WrViewport *viewport, WrPostProcessingEffect *ambient_occlusion_effect) {
  reinterpret_cast<wren::Viewport *>(viewport)->setAmbientOcclusionEffect(
    reinterpret_cast<wren::PostProcessingEffect *>(ambient_occlusion_effect));
}

void wr_viewport_set_anti_aliasing_effect(WrViewport *viewport, WrPostProcessingEffect *anti_aliasing_effect) {
  reinterpret_cast<wren::Viewport *>(viewport)->setAntiAliasingEffect(
    reinterpret_cast<wren::PostProcessingEffect *>(anti_aliasing_effect));
}

int wr_viewport_get_width(WrViewport *viewport) {
  return reinterpret_cast<wren::Viewport *>(viewport)->width();
}

int wr_viewport_get_height(WrViewport *viewport) {
  return reinterpret_cast<wren::Viewport *>(viewport)->height();
}

WrCamera *wr_viewport_get_camera(WrViewport *viewport) {
  return reinterpret_cast<WrCamera *>(reinterpret_cast<wren::Viewport *>(viewport)->camera());
}

WrFrameBuffer *wr_viewport_get_frame_buffer(WrViewport *viewport) {
  return reinterpret_cast<WrFrameBuffer *>(reinterpret_cast<wren::Viewport *>(viewport)->frameBuffer());
}
