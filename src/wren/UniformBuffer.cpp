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

#include "UniformBuffer.hpp"

#include "GlState.hpp"

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  UniformBuffer::UniformBuffer(const unsigned int binding, const int size) : mGlName(0), mBinding(binding), mSize(size) {
    setRequireAction(GlUser::GL_ACTION_PREPARE);
  }

  UniformBuffer::~UniformBuffer() { release(); }

  void UniformBuffer::writeValue(const void *data) const {
    bind();

    glBufferData(GL_UNIFORM_BUFFER, mSize, data, GL_DYNAMIC_DRAW);
  }

  void UniformBuffer::bind() const { glstate::bindUniformBuffer(mGlName, mBinding); }

  void UniformBuffer::release() const { glstate::releaseUniformBuffer(mGlName, mBinding); }

  void UniformBuffer::prepareGl() {
    assert(!mGlName);
    assert(mSize);

    glGenBuffers(1, &mGlName);
  }

  void UniformBuffer::cleanupGl() {
    release();

    glDeleteBuffers(1, &mGlName);
  }

}  // namespace wren
