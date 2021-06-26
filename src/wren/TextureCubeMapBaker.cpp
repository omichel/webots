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

#include "TextureCubeMapBaker.hpp"

#include "Constants.hpp"
#include "Debug.hpp"
#include "GlState.hpp"
#include "GlslLayout.hpp"
#include "ShaderProgram.hpp"
#include "TextureCubeMap.hpp"
#include "TextureRtt.hpp"

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <wren/shader_program.h>
#include <wren/texture_cubemap.h>
#include <wren/texture_cubemap_baker.h>
#include <wren/texture_rtt.h>

namespace wren {
  namespace texturecubemapbaker {
    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;
    void renderCube() {
      // initialize (if necessary)
      if (cubeVAO == 0) {
        float vertices[] = {
          // back face
          -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
          1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
          1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,   // bottom-right
          1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top-right
          -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
          -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   // top-left

          // front face
          -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
          1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom-right
          1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
          1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top-right
          -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top-left
          -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left

          // left face
          -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right
          -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-left
          -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
          -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
          -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
          -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-right

          // right face
          1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-left
          1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
          1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top-right
          1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
          1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // top-left
          1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left

          // bottom face
          -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right
          1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // top-left
          1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
          1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,    // bottom-left
          -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom-right
          -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top-right

          // top face
          -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
          1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
          1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
          1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,    // bottom-right
          -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left
          -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f    // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glstate::bindVertexArrayObject(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glstate::releaseVertexArrayObject(cubeVAO);
      }
      // render Cube
      glstate::bindVertexArrayObject(cubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glstate::releaseVertexArrayObject(cubeVAO);
    }

    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad() {
      if (quadVAO == 0) {
        float quadVertices[] = {
          // positions        // texture Coords
          -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
          1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glstate::releaseVertexArrayObject(quadVAO);
      }
      glstate::bindVertexArrayObject(quadVAO);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      glstate::releaseVertexArrayObject(quadVAO);
    }

    TextureCubeMap *bakeDiffuseIrradiance(TextureCubeMap *inputCube, ShaderProgram *irradianceShader, unsigned int size) {
      unsigned int captureFBO = 0;
      unsigned int captureRBO = 0;

      glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
      glm::mat4 captureViews[] = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

      // ensure opengl is in the correct state
      glstate::setBlend(false);
      glstate::setDepthClamp(true);
      glstate::setDepthMask(true);
      glstate::setDepthTest(true);
      glstate::setDepthFunc(GL_LESS);
      glstate::setStencilTest(false);
      glstate::setColorMask(true, true, true, true);
      glstate::setCullFace(false);

      glGenFramebuffers(1, &captureFBO);
      glGenRenderbuffers(1, &captureRBO);

      // set up cubemap texture (manually)
      unsigned int irradianceMapGlName = Texture::generateNewTexture();
      TextureCubeMap *irradianceMap = TextureCubeMap::createTextureCubeMap();
      irradianceMap->setGlName(irradianceMapGlName);
      irradianceMap->setTextureUnit(13);
      glstate::activateTextureUnit(13);
      irradianceMap->setSize(size, size);
      glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMapGlName);

      for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, nullptr);

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glstate::bindFrameBuffer(captureFBO);
      glstate::bindRenderBuffer(captureRBO);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

      irradianceShader->setCustomUniformValue("projection", captureProjection);

      inputCube->setTextureUnit(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);
      inputCube->bind(Texture::DEFAULT_USAGE_PARAMS);

      glstate::bindProgram(irradianceShader->glName());
      glViewport(0, 0, size, size);
      glstate::bindFrameBuffer(captureFBO);
      glUniform1i(irradianceShader->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0),
                  WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);
      for (unsigned int i = 0; i < 6; ++i) {
        irradianceShader->setCustomUniformValue("view", captureViews[i]);
        irradianceShader->bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                               irradianceMap->glName(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
      }

      irradianceShader->release();
      glstate::releaseFrameBuffer(captureFBO);
      glstate::releaseRenderBuffer(captureRBO);
      glDeleteFramebuffers(1, &captureFBO);
      glDeleteRenderbuffers(1, &captureRBO);
      glDeleteVertexArrays(1, &cubeVAO);
      cubeVAO = cubeVBO = 0;

      return irradianceMap;
    }

    TextureCubeMap *bakeSpecularIrradiance(TextureCubeMap *inputCube, ShaderProgram *irradianceShader, unsigned int size) {
      glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
      glm::mat4 captureViews[] = {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

      // ensure opengl is in the correct state
      glstate::setBlend(false);
      glstate::setDepthClamp(true);
      glstate::setDepthMask(true);
      glstate::setDepthTest(true);
      glstate::setDepthFunc(GL_LESS);
      glstate::setStencilTest(false);
      glstate::setColorMask(true, true, true, true);
      glstate::setCullFace(false);

      unsigned int captureFBO = 0;
      unsigned int captureRBO = 0;
      glGenFramebuffers(1, &captureFBO);
      glGenRenderbuffers(1, &captureRBO);

      // set up cubemap texture (manually)
      unsigned int prefilteredCubeGlName = Texture::generateNewTexture();
      TextureCubeMap *prefilteredCube = TextureCubeMap::createTextureCubeMap();
      prefilteredCube->setGlName(prefilteredCubeGlName);
      prefilteredCube->setTextureUnit(14);
      glstate::activateTextureUnit(14);
      prefilteredCube->setSize(size, size);
      glBindTexture(GL_TEXTURE_CUBE_MAP, prefilteredCubeGlName);

      for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, size, size, 0, GL_RGBA, GL_FLOAT, nullptr);

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

      glstate::bindFrameBuffer(captureFBO);
      glstate::bindRenderBuffer(captureRBO);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

      irradianceShader->setCustomUniformValue("projection", captureProjection);

      glBindTexture(GL_TEXTURE_CUBE_MAP, inputCube->glName());
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

      inputCube->setTextureUnit(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);
      inputCube->bind(Texture::DEFAULT_USAGE_PARAMS);

      glstate::bindProgram(irradianceShader->glName());
      glstate::bindFrameBuffer(captureFBO);
      glUniform1i(irradianceShader->uniformLocation(WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0),
                  WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);

      unsigned int maxMipLevels = log2(size) + 1;
      for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = size * std::pow(0.5, mip);
        unsigned int mipHeight = mipWidth;
        glstate::bindRenderBuffer(captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        irradianceShader->setCustomUniformValue("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i) {
          irradianceShader->setCustomUniformValue("view", captureViews[i]);
          irradianceShader->bind();
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                 prefilteredCube->glName(), mip);

          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          renderCube();
        }
      }

      irradianceShader->release();
      glstate::releaseFrameBuffer(captureFBO);
      glstate::releaseRenderBuffer(captureRBO);
      glDeleteFramebuffers(1, &captureFBO);
      glDeleteRenderbuffers(1, &captureRBO);
      glDeleteVertexArrays(1, &cubeVAO);
      cubeVAO = cubeVBO = 0;
      prefilteredCube->release();
      return prefilteredCube;
    }

    TextureRtt *bakeBrdf(ShaderProgram *brdfShader, unsigned int size) {
      unsigned int captureFBO = 0;
      unsigned int captureRBO = 0;
      glGenFramebuffers(1, &captureFBO);
      glGenRenderbuffers(1, &captureRBO);

      // ensure opengl is in the correct state
      glstate::setBlend(false);
      glstate::setDepthClamp(true);
      glstate::setDepthMask(true);
      glstate::setDepthTest(true);
      glstate::setDepthFunc(GL_LESS);
      glstate::setStencilTest(false);
      glstate::setColorMask(true, true, true, true);
      glstate::setCullFace(false);

      unsigned int brdfTextureGlName = Texture::generateNewTexture();

      TextureRtt *brdfTexture = TextureRtt::createTextureRtt();

      brdfTexture->setTextureUnit(5);
      brdfTexture->setGlName(brdfTextureGlName);
      glstate::activateTextureUnit(5);
      brdfTexture->setSize(size, size);

      // pre-allocate enough memory for the LUT texture.
      glBindTexture(GL_TEXTURE_2D, brdfTextureGlName);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size, size, 0, GL_RGBA, GL_FLOAT, 0);
      // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
      glstate::bindFrameBuffer(captureFBO);
      glstate::bindRenderBuffer(captureRBO);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfTextureGlName, 0);

      glViewport(0, 0, size, size);
      brdfShader->bind();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      renderQuad();

      glstate::releaseFrameBuffer(captureFBO);
      glstate::releaseRenderBuffer(captureRBO);
      glDeleteFramebuffers(1, &captureFBO);
      glDeleteRenderbuffers(1, &captureRBO);
      glstate::releaseFrameBuffer(captureFBO);
      glstate::releaseRenderBuffer(captureRBO);
      brdfShader->release();
      glDeleteVertexArrays(1, &quadVAO);
      quadVAO = quadVBO = 0;
      return brdfTexture;
    }

  }  // namespace texturecubemapbaker
}  // namespace wren

// C interface implementation
WrTextureCubeMap *wr_texture_cubemap_bake_diffuse_irradiance(WrTextureCubeMap *input_cubemap, WrShaderProgram *shader,
                                                             unsigned int size) {
  return reinterpret_cast<WrTextureCubeMap *>(wren::texturecubemapbaker::bakeDiffuseIrradiance(
    reinterpret_cast<wren::TextureCubeMap *>(input_cubemap), reinterpret_cast<wren::ShaderProgram *>(shader), size));
}

WrTextureCubeMap *wr_texture_cubemap_bake_specular_irradiance(WrTextureCubeMap *input_cubemap, WrShaderProgram *shader,
                                                              unsigned int size) {
  return reinterpret_cast<WrTextureCubeMap *>(wren::texturecubemapbaker::bakeSpecularIrradiance(
    reinterpret_cast<wren::TextureCubeMap *>(input_cubemap), reinterpret_cast<wren::ShaderProgram *>(shader), size));
}

WrTextureRtt *wr_texture_cubemap_bake_brdf(WrShaderProgram *shader, unsigned int size) {
  return reinterpret_cast<WrTextureRtt *>(
    wren::texturecubemapbaker::bakeBrdf(reinterpret_cast<wren::ShaderProgram *>(shader), size));
}
