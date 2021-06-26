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

#include "ShaderProgram.hpp"

#include "ContainerUtils.hpp"
#include "Debug.hpp"
#include "GlState.hpp"
#include "Texture.hpp"

#include <wren/shader_program.h>

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

#include <algorithm>
#include <cstdlib>
#include <fstream>

namespace wren {

  void ShaderProgram::deleteShaderProgram(ShaderProgram *program) {
    if (!program)
      return;

    if (glstate::isContextActive()) {
      program->cleanupGl();
      delete program;
    } else
      program->setRequireAction(GlUser::GL_ACTION_DELETE);
  }

  void ShaderProgram::useUniform(WrGlslLayoutUniform uniform) {
    if (std::find(mUniforms.cbegin(), mUniforms.cend(), uniform) == mUniforms.cend())
      mUniforms.push_back(uniform);
  }

  void ShaderProgram::useUniformBuffer(WrGlslLayoutUniformBuffer uniformBuffer) {
    if (std::find(mUniformBuffers.cbegin(), mUniformBuffers.cend(), uniformBuffer) == mUniformBuffers.cend())
      mUniformBuffers.push_back(uniformBuffer);
  }

  void ShaderProgram::setup() {
    if (glstate::isContextActive())
      prepareGl();
    else
      setRequireAction(GlUser::GL_ACTION_PREPARE);
  }

  void ShaderProgram::bind() const {
    if (!mGlName)
      return;

    glstate::bindProgram(mGlName);

    for (auto &uniform : mCustomUniforms)
      uniform.second->uploadValue();
  }

  void ShaderProgram::release() const { glstate::releaseProgram(mGlName); }

  bool ShaderProgram::readFile(const std::string &path, std::string &contents) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();

      return true;
    }

    return false;
  }

  unsigned int ShaderProgram::compileShader(const std::string &path, unsigned int type) {
    std::string shaderCode;
    if (!readFile(path, shaderCode)) {
      DEBUG("ShaderProgram::compileShader: file not found!");
      DEBUG("Shader source path: " << path.c_str());
    }

    unsigned int shaderGlName = glCreateShader(type);

#ifdef __EMSCRIPTEN__
    shaderCode.replace(0, 17, "#version 300 es");
#endif

    const char *cString = shaderCode.c_str();
    glShaderSource(shaderGlName, 1, &cString, nullptr);
    glCompileShader(shaderGlName);

    int success;
    glGetShaderiv(shaderGlName, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
      int logLength;
      glGetShaderiv(shaderGlName, GL_INFO_LOG_LENGTH, &logLength);

      char log[logLength];
      glGetShaderInfoLog(shaderGlName, logLength, nullptr, &log[0]);

      DEBUG("ShaderProgram::compileShader: compilation failed!");
      DEBUG("Shader source path: " << path.c_str());
      DEBUG("InfoLog: " << log);
      mCompilationLog.assign(log);

      glDeleteShader(shaderGlName);
      shaderGlName = 0;
    }

    return shaderGlName;
  }

  ShaderProgram::ShaderProgram() :
    mGlName(0),
    mUniformLocations(WR_GLSL_LAYOUT_UNIFORM_COUNT, -1),
    mHasVertexShaderCompilationFailed(false),
    mHasFragmentShaderCompilationFailed(false) {
    mUniforms.reserve(WR_GLSL_LAYOUT_UNIFORM_COUNT);
    mUniformBuffers.reserve(WR_GLSL_LAYOUT_UNIFORM_BUFFER_COUNT);
  }

  ShaderProgram::~ShaderProgram() {
    for (auto &uniform : mCustomUniforms)
      delete uniform.second;
  }

  bool ShaderProgram::linkProgram() {
    unsigned int vertexShaderGlName = ShaderProgram::compileShader(mVertexShaderPath, GL_VERTEX_SHADER);
    if (!vertexShaderGlName) {
      DEBUG("ShaderProgram::createProgram: Vertex shader compilation failed!");
      mHasVertexShaderCompilationFailed = true;
      return false;
    }

    unsigned int fragmentShaderGlName = ShaderProgram::compileShader(mFragmentShaderPath, GL_FRAGMENT_SHADER);
    if (!fragmentShaderGlName) {
      DEBUG("ShaderProgram::createProgram: Fragment shader compilation failed!");
      glDeleteShader(vertexShaderGlName);
      mHasFragmentShaderCompilationFailed = true;
      return false;
    }

    mGlName = glCreateProgram();
    glAttachShader(mGlName, vertexShaderGlName);
    glAttachShader(mGlName, fragmentShaderGlName);
    glLinkProgram(mGlName);

    int success;
    glGetProgramiv(mGlName, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
      int logLength;
      glGetProgramiv(mGlName, GL_INFO_LOG_LENGTH, &logLength);

      char log[logLength];
      glGetProgramInfoLog(mGlName, logLength, nullptr, &log[0]);

      DEBUG("ShaderProgram::createProgram: linking failed!");
      DEBUG("InfoLog:" << log);

      glDeleteProgram(mGlName);
      mGlName = 0;
    }

    glDeleteShader(vertexShaderGlName);
    glDeleteShader(fragmentShaderGlName);

    return mGlName;
  }

  void ShaderProgram::prepareGl() {
    assert(mVertexShaderPath.size() && mFragmentShaderPath.size());

    if (!linkProgram()) {
      std::cerr << "An error occured during shader compilation/linking!" << std::endl;
      std::cerr << "Vertex shader path: " << mVertexShaderPath << std::endl;
      std::cerr << "Fragment shader path: " << mFragmentShaderPath << std::endl;

      return;
    }

    glstate::bindProgram(mGlName);

    for (WrGlslLayoutUniform uni : mUniforms) {
      mUniformLocations[uni] = glGetUniformLocation(mGlName, GlslLayout::gUniformNames[uni]);
      assert(mUniformLocations[uni] != -1);
    }

    for (WrGlslLayoutUniformBuffer ub : mUniformBuffers) {
      unsigned int uniformBlockIndex = glGetUniformBlockIndex(mGlName, GlslLayout::gUniformBufferNames[ub]);
      assert(uniformBlockIndex != GL_INVALID_INDEX);

      glUniformBlockBinding(mGlName, uniformBlockIndex, ub);
    }

    for (auto &el : mCustomUniforms) {
      int location = glGetUniformLocation(mGlName, el.second->name().c_str());
      assert(location >= 0);

      el.second->setLocation(location);
    }
  }

  void ShaderProgram::cleanupGl() {
    if (mGlName) {
      release();

      glDeleteProgram(mGlName);
    }
  }

}  // namespace wren

// C interface implementation
WrShaderProgram *wr_shader_program_new() {
  return reinterpret_cast<WrShaderProgram *>(wren::ShaderProgram::createShaderProgram());
}

void wr_shader_program_delete(WrShaderProgram *program) {
  wren::ShaderProgram::deleteShaderProgram(reinterpret_cast<wren::ShaderProgram *>(program));
}

void wr_shader_program_set_vertex_shader_path(WrShaderProgram *program, const char *path) {
  reinterpret_cast<wren::ShaderProgram *>(program)->setVertexShaderPath(path);
}

void wr_shader_program_set_fragment_shader_path(WrShaderProgram *program, const char *path) {
  reinterpret_cast<wren::ShaderProgram *>(program)->setFragmentShaderPath(path);
}

void wr_shader_program_use_uniform(WrShaderProgram *program, WrGlslLayoutUniform uniform) {
  reinterpret_cast<wren::ShaderProgram *>(program)->useUniform(uniform);
}

void wr_shader_program_use_uniform_buffer(WrShaderProgram *program, WrGlslLayoutUniformBuffer uniform_buffer) {
  reinterpret_cast<wren::ShaderProgram *>(program)->useUniformBuffer(uniform_buffer);
}

void wr_shader_program_create_custom_uniform(WrShaderProgram *program, const char *name, WrShaderProgramUniformType type,
                                             const char *initial_value) {
  wren::ShaderProgram *shaderProgram = reinterpret_cast<wren::ShaderProgram *>(program);
  const std::string uniformName(name);

  switch (type) {
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT:
      shaderProgram->createCustomUniform(uniformName,
                                         *(reinterpret_cast<const float *>(reinterpret_cast<const void *>(initial_value))));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_INT:
      shaderProgram->createCustomUniform(uniformName, *(reinterpret_cast<const int *>(initial_value)));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F:
      shaderProgram->createCustomUniform(uniformName, *(reinterpret_cast<const glm::vec2 *>(initial_value)));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC3F:
      shaderProgram->createCustomUniform(uniformName, *(reinterpret_cast<const glm::vec3 *>(initial_value)));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F:
      shaderProgram->createCustomUniform(uniformName, *(reinterpret_cast<const glm::vec4 *>(initial_value)));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F:
      shaderProgram->createCustomUniform(uniformName, *(reinterpret_cast<const glm::mat4 *>(initial_value)));
      break;
    default:
      assert(false);
  }
}

void wr_shader_program_set_custom_uniform_value(WrShaderProgram *program, const char *name, WrShaderProgramUniformType type,
                                                const char *value) {
  wren::ShaderProgram *shaderProgram = reinterpret_cast<wren::ShaderProgram *>(program);
  std::string uniformName(name);

  switch (type) {
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT:
      shaderProgram->setCustomUniformValue(uniformName,
                                           *(reinterpret_cast<const float *>(reinterpret_cast<const void *>(value))));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_INT:
      shaderProgram->setCustomUniformValue(uniformName, *(reinterpret_cast<const int *>(value)));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F:
      shaderProgram->setCustomUniformValue(uniformName, *(reinterpret_cast<const glm::vec2 *>(value)));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC3F:
      shaderProgram->setCustomUniformValue(uniformName, *(reinterpret_cast<const glm::vec3 *>(value)));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F:
      shaderProgram->setCustomUniformValue(uniformName, *(reinterpret_cast<const glm::vec4 *>(value)));
      break;
    case WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F:
      shaderProgram->setCustomUniformValue(uniformName, *(reinterpret_cast<const glm::mat4 *>(value)));
      break;
    default:
      assert(false);
  }
}

unsigned int wr_shader_program_get_gl_name(WrShaderProgram *program) {
  return reinterpret_cast<wren::ShaderProgram *>(program)->glName();
}

bool wr_shader_program_has_vertex_shader_compilation_failed(WrShaderProgram *program) {
  return reinterpret_cast<wren::ShaderProgram *>(program)->hasVertexShadercompilationFailed();
}

bool wr_shader_program_has_fragment_shader_compilation_failed(WrShaderProgram *program) {
  return reinterpret_cast<wren::ShaderProgram *>(program)->hasFragmentShadercompilationFailed();
}

const char *wr_shader_program_get_compilation_log(WrShaderProgram *program) {
  return reinterpret_cast<wren::ShaderProgram *>(program)->compilationLog().c_str();
}

void wr_shader_program_setup(WrShaderProgram *program) {
  reinterpret_cast<wren::ShaderProgram *>(program)->setup();
}
