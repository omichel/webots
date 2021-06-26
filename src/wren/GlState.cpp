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

#include "GlState.hpp"

#include "Cache.hpp"
#include "Debug.hpp"
#include "GlslLayout.hpp"
#include "UniformBuffer.hpp"

#include <wren/gl_state.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <emscripten.h>
#include <emscripten/html5.h>

#include "JSHelper.hpp"
#else
#include <glad/glad.h>
#endif

#include <algorithm>
#include <map>
#include <string>

namespace wren {
  namespace glstate {

    static bool cIsGlInitialized = false;
    static bool cIsGlContextActive = false;

    static const char *cVendor = NULL;
    static const char *cRenderer = NULL;
    static const char *cVersion = NULL;
    static const char *cGlslVersion = NULL;

    static bool cDepthTest = false;
    static unsigned int cDepthFunc = GL_LESS;
#ifndef __EMSCRIPTEN__
    static bool cDepthClamp = false;
#endif
    static bool cDepthMask = true;
    static bool cStencilTest = false;
    static unsigned int cStencilFuncFunc = GL_ALWAYS;
    static int cStencilFuncRef = 0;
    static unsigned int cStencilFuncMask = ~0;
    static unsigned int cStencilOpFrontSfail = GL_KEEP;
    static unsigned int cStencilOpFrontDpfail = GL_KEEP;
    static unsigned int cStencilOpFrontDppass = GL_KEEP;
    static unsigned int cStencilOpBackSfail = GL_KEEP;
    static unsigned int cStencilOpBackDpfail = GL_KEEP;
    static unsigned int cStencilOpBackDppass = GL_KEEP;
    static unsigned int cStencilMask = ~0;
    static bool cColorMaskRed = true;
    static bool cColorMaskGreen = true;
    static bool cColorMaskBlue = true;
    static bool cColorMaskAlpha = true;
    static bool cCullFace = false;
    static unsigned int cCullFaceMode = GL_BACK;
    static unsigned int cFrontFace = GL_CCW;
    static bool cBlend = false;
    static unsigned int cBlendSrcFactor = GL_ONE;
    static unsigned int cBlendDestFactor = GL_ZERO;
    static unsigned int cBlendModeRgb = GL_FUNC_ADD;
    static unsigned int cBlendModeAlpha = GL_FUNC_ADD;
    static unsigned int cPolygonMode = GL_FILL;
    static bool cPolygonOffset = false;
    static float cPolygonOffsetFactor = 0.0f;
    static float cPolygonOffsetUnits = 0.0f;
    static unsigned int cActiveProgram = 0;
    static unsigned int cActiveVertexArrayObject = 0;
    static unsigned int cActiveElementArrayBuffer = 0;
    static int cActiveTextureUnit = -1;
    static uint16_t cActivePhongMaterial = 0;
    static uint16_t cActivePbrMaterial = 0;
    static unsigned int cBoundTextures[gMaxTextureUnits] = {};
    static unsigned int cBoundDrawFrameBuffer = 0;
    static unsigned int cBoundReadFrameBuffer = 0;
    static unsigned int cBoundPixelPackBuffer = 0;
    static unsigned int cBoundRenderBuffer = 0;
    static unsigned int cBoundUniformBuffer = 0;
    static glm::vec4 cClearColor = glm::vec4(0.0, 0.0, 0.0, 0.0);
    std::map<unsigned int, int> cTextureWrapS;
    std::map<unsigned int, int> cTextureWrapT;
    std::map<unsigned int, glm::vec4> cTextureBorderColor;
    std::map<unsigned int, float> cTextureAnisotropy;
    std::map<unsigned int, int> cTextureMinFilter;
    std::map<unsigned int, int> cTextureMagFilter;
    static int cGpuMemory = 0;
    static int cMaxCombinedTextureUnits = 0;
    static int cMaxFrameBufferDrawBuffers = 0;
    static float cMaxTextureAnisotropy = 1.0f;
    static bool cPointSize = false;
    static bool cDisableCheck = false;

    static std::vector<std::unique_ptr<UniformBuffer>> cUniformBuffers;

    void init() {
#ifndef __EMSCRIPTEN__
      if (!gladLoadGL())
        std::cerr << "ERROR: Unable to load OpenGL functions!" << std::endl;

      // attempt to use clip-space Z-values in [0, 1] instead of [-1, 1] for better precision
      if (!GLAD_GL_ARB_clip_control)
        DEBUG("GLAD_GL_ARB_clip_control extension not supported by hardware" << std::endl);
      else
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
#endif
      glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &cMaxCombinedTextureUnits);
      glGetIntegerv(GL_MAX_DRAW_BUFFERS, &cMaxFrameBufferDrawBuffers);
      if (wr_gl_state_is_anisotropic_texture_filtering_supported())
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &cMaxTextureAnisotropy);

      DEBUG("Max. combined texture units supported by hardware: " << cMaxCombinedTextureUnits);
      DEBUG("Max. framebuffer draw buffers supported by hardware: " << cMaxFrameBufferDrawBuffers);
      DEBUG("Max. anisotropy supported by hardware: " << cMaxTextureAnisotropy);

      cVendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
      cRenderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
      cVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
      cGlslVersion = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
#ifdef __EMSCRIPTEN__
      int array[4];
      array[0] = -1;

      cGpuMemory = array[0];
      checkError(GL_INVALID_ENUM);
#else

      if (GLAD_GL_NVX_gpu_memory_info)
        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &cGpuMemory);
      else {
        // Try to use GL_TEXTURE_FREE_MEMORY_ATI:
        // it seems to be working even if the corresponding GLAD_GL_ATI_meminfo is not available
        int array[4];
        array[0] = -1;
        glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, array);
        cGpuMemory = array[0];
        checkError(GL_INVALID_ENUM);  // check errors skipping any possible GL_INVALID_ENUM error
      }
#endif
      // setup uniform buffers
      size_t count = GlslLayout::gUniformBufferNames.size();
      cUniformBuffers.reserve(count);
      for (size_t i = 0; i < count; ++i)
        cUniformBuffers.push_back(std::unique_ptr<UniformBuffer>(new UniformBuffer(i, GlslLayout::gUniformBufferSizes[i])));

      glstate::setDepthTest(true);
      glstate::setCullFace(true);
      glstate::setPolygonMode(GL_FILL);
#ifndef __EMSCRIPTEN__
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  // for proper interpolation across cubemap faces
#endif

      checkError();

      cIsGlInitialized = true;
    }  // namespace glstate

    bool isInitialized() { return cIsGlInitialized; }

    bool isContextActive() { return cIsGlContextActive; }

    void setContextActive(bool active) { cIsGlContextActive = active; }

    void setDefaultState() {
      setDepthTest(true);
      setDepthClamp(false);
      setDepthFunc(GL_LESS);
      setDepthMask(true);
      setStencilTest(false);
      setStencilFunc(GL_ALWAYS, 0, ~0);
      setStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      setColorMask(true, true, true, true);
      setCullFace(true);
      setCullFaceMode(GL_BACK);
      setFrontFace(GL_CCW);
      setBlend(false);
      setBlendFunc(GL_ONE, GL_ZERO);
      setBlendEquation(GL_FUNC_ADD);
      setPolygonOffset(false, 0.0f, 0.0f);
    }

    void setDepthTest(bool enable) {
      if (cDepthTest != enable) {
        cDepthTest = enable;
        if (enable)
          glEnable(GL_DEPTH_TEST);
        else
          glDisable(GL_DEPTH_TEST);
      }
    }

    void setDepthFunc(unsigned int func) {
      if (cDepthFunc != func) {
        cDepthFunc = func;
        glDepthFunc(func);
      }
    }

    void setDepthMask(bool enable) {
      if (cDepthMask != enable) {
        cDepthMask = enable;
        glDepthMask(enable ? GL_TRUE : GL_FALSE);
      }
    }

    void setStencilTest(bool enable) {
      if (cStencilTest != enable) {
        cStencilTest = enable;
        if (enable)
          glEnable(GL_STENCIL_TEST);
        else
          glDisable(GL_STENCIL_TEST);
      }
    }

    void setStencilFunc(unsigned int func, int ref, unsigned int mask) {
      if (cStencilFuncFunc != func || cStencilFuncRef != ref || cStencilFuncMask != mask) {
        cStencilFuncFunc = func;
        cStencilFuncRef = ref;
        cStencilFuncMask = mask;
        glStencilFunc(func, ref, mask);
      }
    }

    void setStencilOp(unsigned int sfail, unsigned int dpfail, unsigned int dppass) {
      if (cStencilOpFrontSfail != sfail || cStencilOpBackSfail != sfail || cStencilOpFrontDpfail != dpfail ||
          cStencilOpBackDpfail != dpfail || cStencilOpFrontDppass != dppass || cStencilOpBackDppass != dppass) {
        cStencilOpFrontSfail = cStencilOpBackSfail = sfail;
        cStencilOpFrontDpfail = cStencilOpBackDpfail = dpfail;
        cStencilOpFrontDppass = cStencilOpBackDppass = dppass;
        glStencilOp(sfail, dpfail, dppass);
      }
    }

    void setStencilOpFront(unsigned int sfail, unsigned int dpfail, unsigned int dppass) {
      if (cStencilOpFrontSfail != sfail || cStencilOpFrontDpfail != dpfail || cStencilOpFrontDppass != dppass) {
        cStencilOpFrontSfail = sfail;
        cStencilOpFrontDpfail = dpfail;
        cStencilOpFrontDppass = dppass;
        glStencilOpSeparate(GL_FRONT, sfail, dpfail, dppass);
      }
    }

    void setStencilOpBack(unsigned int sfail, unsigned int dpfail, unsigned int dppass) {
      if (cStencilOpBackSfail != sfail || cStencilOpBackDpfail != dpfail || cStencilOpBackDppass != dppass) {
        cStencilOpBackSfail = sfail;
        cStencilOpBackDpfail = dpfail;
        cStencilOpBackDppass = dppass;
        glStencilOpSeparate(GL_BACK, sfail, dpfail, dppass);
      }
    }

    void setStencilMask(unsigned int mask) {
      if (cStencilMask != mask) {
        cStencilMask = mask;
        glStencilMask(mask);
      }
    }

    void setColorMask(bool red, bool green, bool blue, bool alpha) {
      if (cColorMaskRed != red || cColorMaskGreen != green || cColorMaskBlue != blue || cColorMaskAlpha != alpha) {
        cColorMaskRed = red;
        cColorMaskGreen = green;
        cColorMaskBlue = blue;
        cColorMaskAlpha = alpha;
        glColorMask(red, green, blue, alpha);
      }
    }

    void setCullFace(bool enable) {
      if (cCullFace != enable) {
        cCullFace = enable;
        if (enable)
          glEnable(GL_CULL_FACE);
        else
          glDisable(GL_CULL_FACE);
      }
    }

    void setCullFaceMode(unsigned int mode) {
      if (cCullFaceMode != mode) {
        cCullFaceMode = mode;
        glCullFace(mode);
      }
    }

    void setFrontFace(unsigned int mode) {
      if (cFrontFace != mode) {
        cFrontFace = mode;
        glFrontFace(mode);
      }
    }

    void setBlend(bool enable) {
      if (cBlend != enable) {
        cBlend = enable;
        if (enable)
          glEnable(GL_BLEND);
        else
          glDisable(GL_BLEND);
      }
    }

    void setBlendFunc(unsigned int srcFactor, unsigned int destFactor) {
      if (cBlendSrcFactor != srcFactor || cBlendDestFactor != destFactor) {
        cBlendSrcFactor = srcFactor;
        cBlendDestFactor = destFactor;
        glBlendFunc(srcFactor, destFactor);
      }
    }

    void setBlendEquation(unsigned int mode) {
      if (cBlendModeRgb != mode || cBlendModeAlpha != mode) {
        cBlendModeRgb = cBlendModeAlpha = mode;
        glBlendEquation(mode);
      }
    }

    void setBlendEquationSeparate(unsigned int modeRgb, unsigned int modeAlpha) {
      if (cBlendModeRgb != modeRgb || cBlendModeAlpha != modeAlpha) {
        cBlendModeRgb = modeRgb;
        cBlendModeAlpha = modeAlpha;
        glBlendEquationSeparate(modeRgb, modeAlpha);
      }
    }

    void setDepthClamp(bool enable) {
#ifndef __EMSCRIPTEN__
      if (cDepthClamp != enable) {
        cDepthClamp = enable;
        if (enable)
          glEnable(GL_DEPTH_CLAMP);
        else
          glDisable(GL_DEPTH_CLAMP);
      }
#endif
    }

    void setPolygonMode(unsigned int polygonMode) {
#ifdef __EMSCRIPTEN__
      cPolygonMode = polygonMode;
#else
      if (cPolygonMode != polygonMode) {
        cPolygonMode = polygonMode;
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
      }
#endif
    }  // namespace glstate

    void setPolygonOffset(bool enable, float factor, float units) {
      if (cPolygonOffset != enable) {
        cPolygonOffset = enable;
        if (enable)
          glEnable(GL_POLYGON_OFFSET_FILL);
        else
          glDisable(GL_POLYGON_OFFSET_FILL);
      }

      if (cPolygonOffsetFactor != factor || cPolygonOffsetUnits != units) {
        cPolygonOffsetFactor = factor;
        cPolygonOffsetUnits = units;
        glPolygonOffset(factor, units);
      }
    }

    void setClearColor(const glm::vec4 &clearColor) {
      if (cClearColor != clearColor) {
        cClearColor = clearColor;
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
      }
    }

    void enablePointSize(bool enable) {
#ifdef __EMSCRIPTEN__
      cPointSize = enable;
#else
      if (cPointSize != enable) {
        cPointSize = enable;
        if (enable)
          glEnable(GL_PROGRAM_POINT_SIZE);
        else
          glDisable(GL_PROGRAM_POINT_SIZE);
      }
#endif
    }

    void activateTextureUnit(int textureUnit) {
      if (cActiveTextureUnit != textureUnit) {
        cActiveTextureUnit = textureUnit;
        glActiveTexture(GL_TEXTURE0 + textureUnit);
      }
    }

    void setTextureWrapS(unsigned int glName, int textureUnit, int mode) {
      assert(cBoundTextures[textureUnit] == glName);
      if (cTextureWrapS[glName] != mode) {
        cTextureWrapS[glName] = mode;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
      }
    }

    void setTextureWrapT(unsigned int glName, int textureUnit, int mode) {
      assert(cBoundTextures[textureUnit] == glName);
      if (cTextureWrapT[glName] != mode) {
        cTextureWrapT[glName] = mode;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
      }
    }

    void setTextureBorderColor(unsigned int glName, int textureUnit, const glm::vec4 &color) {
      assert(cBoundTextures[textureUnit] == glName);
      if (cTextureBorderColor[glName] != color) {
        cTextureBorderColor[glName] = color;
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
      }
    }

    void setTextureAnisotropy(unsigned int glName, int textureUnit, float anisotropy) {
      assert(cBoundTextures[textureUnit] == glName);
#ifndef __EMSCRIPTEN__
      if (!GLAD_GL_EXT_texture_filter_anisotropic)
        return;
#endif

      if (cTextureAnisotropy[glName] != anisotropy) {
        anisotropy = std::max(std::min(anisotropy, maxTextureAnisotropy()), 1.0f);
        cTextureAnisotropy[glName] = anisotropy;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
      }
    }

    void setTextureInterpolation(unsigned int glName, int textureUnit, bool useInterpolation, bool useMipMapping) {
      assert(cBoundTextures[textureUnit] == glName);
      int minFilter = useMipMapping ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
      if (useInterpolation)
        minFilter = useMipMapping ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

      const int magFilter = useInterpolation ? GL_LINEAR : GL_NEAREST;

      if (cTextureMinFilter[glName] != minFilter) {
        cTextureMinFilter[glName] = minFilter;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
      }

      if (cTextureMagFilter[glName] != magFilter) {
        cTextureMagFilter[glName] = magFilter;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
      }
    }

    void initializeTextureParams(unsigned int glName) {
      assert(glName);

      cTextureWrapS[glName] = GL_REPEAT;
      cTextureWrapT[glName] = GL_REPEAT;
      cTextureBorderColor[glName] = gVec4Zeros;
      cTextureAnisotropy[glName] = 1.0f;
      cTextureMinFilter[glName] = GL_NEAREST_MIPMAP_LINEAR;
      cTextureMagFilter[glName] = GL_LINEAR;
    }

    void clearTextureParams(unsigned int glName) {
      assert(glName);

      std::map<unsigned int, int>::iterator cTextureWrapSIterator = cTextureWrapS.find(glName);
      if (cTextureWrapSIterator != cTextureWrapS.end())
        cTextureWrapS.erase(cTextureWrapSIterator);
      else
        assert(0);

      std::map<unsigned int, int>::iterator cTextureWrapTIterator = cTextureWrapT.find(glName);
      if (cTextureWrapTIterator != cTextureWrapT.end())
        cTextureWrapT.erase(cTextureWrapTIterator);
      else
        assert(0);

      std::map<unsigned int, glm::vec4>::iterator cTextureBorderColorIterator = cTextureBorderColor.find(glName);
      if (cTextureBorderColorIterator != cTextureBorderColor.end())
        cTextureBorderColor.erase(cTextureBorderColorIterator);
      else
        assert(0);

      std::map<unsigned int, float>::iterator cTextureAnisotropyIterator = cTextureAnisotropy.find(glName);
      if (cTextureAnisotropyIterator != cTextureAnisotropy.end())
        cTextureAnisotropy.erase(cTextureAnisotropyIterator);
      else
        assert(0);

      std::map<unsigned int, int>::iterator cTextureMinFilterIterator = cTextureMinFilter.find(glName);
      if (cTextureMinFilterIterator != cTextureMinFilter.end())
        cTextureMinFilter.erase(cTextureMinFilterIterator);
      else
        assert(0);

      std::map<unsigned int, int>::iterator cTextureMagFilterIterator = cTextureMagFilter.find(glName);
      if (cTextureMagFilterIterator != cTextureMagFilter.end())
        cTextureMagFilter.erase(cTextureMagFilterIterator);
      else
        assert(0);
    }

    void bindProgram(unsigned int glName) {
      if (cActiveProgram != glName) {
        cActiveProgram = glName;
        glUseProgram(glName);
      }
    }

    void bindVertexArrayObject(unsigned int glName) {
      if (cActiveVertexArrayObject != glName) {
        cActiveVertexArrayObject = glName;
        glBindVertexArray(glName);
      }
    }

    void bindElementArrayBuffer(unsigned int glName) {
      if (cActiveElementArrayBuffer != glName) {
        cActiveElementArrayBuffer = glName;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glName);
      }
    }

    void bindTexture(unsigned int glName, int textureUnit) {
      assert(textureUnit >= 0 && textureUnit < gMaxTextureUnits);
      assert(cActiveTextureUnit == textureUnit);

      if (cBoundTextures[textureUnit] != glName) {
        cBoundTextures[textureUnit] = glName;
        activateTextureUnit(textureUnit);
        glBindTexture(GL_TEXTURE_2D, glName);
      }
    }

    void bindTextureCubeMap(unsigned int glName, int textureUnit) {
      assert(textureUnit >= 0 && textureUnit < gMaxTextureUnits);

      if (cBoundTextures[textureUnit] != glName) {
        cBoundTextures[textureUnit] = glName;
        activateTextureUnit(textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, glName);
      }
    }

    void bindFrameBuffer(unsigned int glName) {
      bindDrawFrameBuffer(glName);
      bindReadFrameBuffer(glName);
    }

    void bindDrawFrameBuffer(unsigned int glName) {
      if (cBoundDrawFrameBuffer != glName) {
        cBoundDrawFrameBuffer = glName;
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, glName);
      }
    }

    void bindReadFrameBuffer(unsigned int glName) {
      if (cBoundReadFrameBuffer != glName) {
        cBoundReadFrameBuffer = glName;
        glBindFramebuffer(GL_READ_FRAMEBUFFER, glName);
      }
    }

    void bindPixelPackBuffer(unsigned int glName) {
      if (cBoundPixelPackBuffer != glName) {
        cBoundPixelPackBuffer = glName;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, glName);
      }
    }

    void bindRenderBuffer(unsigned int glName) {
      if (cBoundRenderBuffer != glName) {
        cBoundRenderBuffer = glName;
        glBindRenderbuffer(GL_RENDERBUFFER, glName);
      }
    }

    void bindUniformBuffer(unsigned int glName, unsigned int binding) {
      if (cBoundUniformBuffer != glName) {
        cBoundUniformBuffer = glName;
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, glName);
      }
    }

    void bindPhongMaterial(const cache::PhongMaterialData *materialData) {
      assert(materialData);

      if (cActivePhongMaterial != materialData->id()) {
        cActivePhongMaterial = materialData->id();

        uniformBuffer(WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG)->writeValue(&materialData->mMaterial);
      }
    }

    void bindPbrMaterial(const cache::PbrMaterialData *materialData) {
      assert(materialData);

      if (cActivePbrMaterial != materialData->id()) {
        cActivePbrMaterial = materialData->id();

        uniformBuffer(WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PBR)->writeValue(&materialData->mMaterial);
      }
    }

    void releaseProgram(unsigned int glName) {
      assert(glName);

      if (cActiveProgram == glName) {
        cActiveProgram = 0;
        glUseProgram(0);
      }
    }

    void releaseVertexArrayObject(unsigned int glName) {
      assert(glName);

      if (cActiveVertexArrayObject == glName) {
        cActiveVertexArrayObject = 0;
        glBindVertexArray(0);
      }
    }

    void releaseElementArrayBuffer(unsigned int glName) {
      assert(glName);

      if (cActiveElementArrayBuffer == glName) {
        cActiveElementArrayBuffer = 0;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }
    }

    void releaseTexture(unsigned int glName, int textureUnit) {
      assert(textureUnit >= 0 && textureUnit < gMaxTextureUnits);

      if (cBoundTextures[textureUnit] == glName) {
        cBoundTextures[textureUnit] = 0;
        activateTextureUnit(textureUnit);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
    }

    void releaseTextureCubeMap(unsigned int glName, int textureUnit) {
      assert(textureUnit >= 0 && textureUnit < gMaxTextureUnits);

      if (cBoundTextures[textureUnit] == glName) {
        cBoundTextures[textureUnit] = 0;
        activateTextureUnit(textureUnit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
      }
    }

    void releaseFrameBuffer(unsigned int glName) {
      assert(glName);

      if (cBoundDrawFrameBuffer == glName || cBoundReadFrameBuffer == glName) {
        cBoundDrawFrameBuffer = 0;
        cBoundReadFrameBuffer = 0;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }
    }

    void releaseDrawFrameBuffer(unsigned int glName) {
      assert(glName);

      if (cBoundDrawFrameBuffer == glName) {
        cBoundDrawFrameBuffer = 0;
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
      }
    }

    void releaseReadFrameBuffer(unsigned int glName) {
      assert(glName);

      if (cBoundReadFrameBuffer == glName) {
        cBoundReadFrameBuffer = 0;
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      }
    }

    void releasePixelPackBuffer(unsigned int glName) {
      assert(glName);

      if (cBoundPixelPackBuffer == glName) {
        cBoundPixelPackBuffer = 0;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
      }
    }

    void releaseRenderBuffer(unsigned int glName) {
      assert(glName);

      if (cBoundRenderBuffer == glName) {
        cBoundRenderBuffer = 0;
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
      }
    }

    void releaseUniformBuffer(unsigned int glName, unsigned int binding) {
      assert(glName);

      if (cBoundUniformBuffer == glName) {
        cBoundUniformBuffer = 0;
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, 0);
      }
    }

    void releasePhongMaterial(const cache::PhongMaterialData *materialData) {
      assert(materialData);

      if (cActivePhongMaterial == materialData->id())
        cActivePhongMaterial = 0;
    }

    void releasePbrMaterial(const cache::PbrMaterialData *materialData) {
      assert(materialData);

      if (cActivePbrMaterial == materialData->id())
        cActivePbrMaterial = 0;
    }

    unsigned int boundReadFrameBuffer() { return cBoundReadFrameBuffer; }

    unsigned int boundDrawFrameBuffer() { return cBoundDrawFrameBuffer; }

    unsigned int boundPixelPackBuffer() { return cBoundPixelPackBuffer; }

    unsigned int blendSrcFactor() { return cBlendSrcFactor; }

    unsigned int blendDestFactor() { return cBlendDestFactor; }

    const char *vendor() { return cVendor; }

    const char *renderer() { return cRenderer; }

    const char *version() { return cVersion; }

    const char *glslVersion() { return cGlslVersion; }

    int gpuMemory() { return cGpuMemory; }

    int maxCombinedTextureUnits() { return cMaxCombinedTextureUnits; }

    int maxFrameBufferDrawBuffers() { return cMaxFrameBufferDrawBuffers; }

    float maxTextureAnisotropy() { return cMaxTextureAnisotropy; }

    unsigned int activeProgram() { return cActiveProgram; }

    const UniformBuffer *uniformBuffer(WrGlslLayoutUniformBuffer buffer) {
      assert(buffer >= 0 && buffer < WR_GLSL_LAYOUT_UNIFORM_BUFFER_COUNT);

      return cUniformBuffers[buffer].get();
    }

    void checkError(int ignore) {
      if (cDisableCheck)
        return;

      int error;
      do {
        error = glGetError();
        if (error != GL_NO_ERROR && error != ignore) {
          std::cerr << "OpenGL error: ";
          switch (error) {
            case GL_INVALID_ENUM:
              std::cerr << "GL_INVALID_ENUM";
              break;
            case GL_INVALID_VALUE:
              std::cerr << "GL_INVALID_VALUE";
              break;
            case GL_INVALID_OPERATION:
              std::cerr << "GL_INVALID_OPERATION";
              break;
            case GL_OUT_OF_MEMORY:
              std::cerr << "GL_OUT_OF_MEMORY";
              break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
              std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
              break;
            default:
              std::cerr << "unknown error (" << error << ")";
          }
          std::cerr << std::endl;
        }
      } while (error != GL_NO_ERROR);
    }

  }  // namespace glstate
}  // namespace wren

// C interface implementation
bool wr_gl_state_is_initialized() {
  return wren::glstate::isInitialized();
}

void wr_gl_state_set_context_active(bool active) {
  wren::glstate::setContextActive(active);
}

const char *wr_gl_state_get_vendor() {
  return wren::glstate::vendor();
}

const char *wr_gl_state_get_renderer() {
  return wren::glstate::renderer();
}

const char *wr_gl_state_get_version() {
  return wren::glstate::version();
}

const char *wr_gl_state_get_glsl_version() {
  return wren::glstate::glslVersion();
}

int wr_gl_state_get_gpu_memory() {
  return wren::glstate::gpuMemory();
}

bool wr_gl_state_is_anisotropic_texture_filtering_supported() {
#ifdef __EMSCRIPTEN__
  return wren::JSHelper::isTextureFilterAnisotropicOn();
#else
  return static_cast<bool>(GLAD_GL_EXT_texture_filter_anisotropic);
#endif
}

float wr_gl_state_max_texture_anisotropy() {
  return wren::glstate::maxTextureAnisotropy();
}

void wr_gl_state_disable_check_error() {
  wren::glstate::cDisableCheck = true;
}
