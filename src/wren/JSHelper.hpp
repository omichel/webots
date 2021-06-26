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
#ifdef __EMSCRIPTEN__

#ifndef JSHELPER_HPP
#define JSHELPER_HPP

namespace wren {  // namespace wren
  namespace JSHelper {
    static bool extension_texture_filter_anisotropic_on = false;
    void initContext(int width, int height);
    bool isTextureFilterAnisotropicOn();
  }  // namespace JSHelper
}  // namespace wren
#endif  // JSHELPER_HPP

#endif
