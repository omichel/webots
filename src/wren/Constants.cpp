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

#include "Constants.hpp"

#include "Primitive.hpp"

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace wren {
  const primitive::Aabb gAabbInf(glm::vec3(-std::numeric_limits<float>::max()), glm::vec3(std::numeric_limits<float>::max()));
  const primitive::Aabb gAabbEmpty(glm::vec3(std::numeric_limits<float>::max()), glm::vec3(-std::numeric_limits<float>::max()));
  const primitive::Sphere gSphereInf(glm::vec3(0.0f), std::numeric_limits<float>::max());

}  // namespace wren
