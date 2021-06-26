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

#include "CustomUniform.hpp"

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {

  CustomUniformBase::CustomUniformBase(const std::string &name) : mName(name), mLocation(-1) {}

  void CustomUniformBase::upload(float value) const { glUniform1f(mLocation, value); }

  void CustomUniformBase::upload(int value) const { glUniform1i(mLocation, value); }

  void CustomUniformBase::upload(const glm::vec2 &value) const { glUniform2fv(mLocation, 1, glm::value_ptr(value)); }

  void CustomUniformBase::upload(const glm::vec3 &value) const { glUniform3fv(mLocation, 1, glm::value_ptr(value)); }

  void CustomUniformBase::upload(const glm::vec4 &value) const { glUniform4fv(mLocation, 1, glm::value_ptr(value)); }

  void CustomUniformBase::upload(const glm::mat4 &value) const {
    glUniformMatrix4fv(mLocation, 1, false, glm::value_ptr(value));
  }

}  // namespace wren
