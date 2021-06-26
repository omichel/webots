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

#include "JSHelper.hpp"

#ifdef __EMSCRIPTEN__
#include <wren/js_helper.h>

#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <emscripten/html5.h>

#include <unistd.h>
#include <iostream>

namespace wren {
  namespace JSHelper {
    bool isTextureFilterAnisotropicOn() { return extension_texture_filter_anisotropic_on; }

    void initContext(int width, int height) {
      EmscriptenWebGLContextAttributes attr;
      emscripten_webgl_init_context_attributes(&attr);
      attr.alpha = attr.depth = attr.stencil = attr.antialias = attr.preserveDrawingBuffer = attr.failIfMajorPerformanceCaveat =
        0;
      attr.enableExtensionsByDefault = 1;
      attr.premultipliedAlpha = 0;
      attr.majorVersion = 2;
      attr.minorVersion = 2;
      EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
      emscripten_set_canvas_element_size("#canvas", width, height);
      emscripten_webgl_make_context_current(ctx);

      // Needed to suppress a warning on Firefox
      emscripten_webgl_enable_extension(ctx, "EXT_float_blend");

      JSHelper::extension_texture_filter_anisotropic_on =
        static_cast<bool>(emscripten_webgl_enable_extension(ctx, "EXT_texture_filter_anisotropic")) ||
        static_cast<bool>(emscripten_webgl_enable_extension(ctx, "MOZ_EXT_texture_filter_anisotropic")) ||
        static_cast<bool>(emscripten_webgl_enable_extension(ctx, "WEBKIT_EXT_texture_filter_anisotropic"));
    }
  }  // namespace JSHelper
}  // namespace wren

float *wrjs_array3(float element0, float element1, float element2) {
  static float array[3];
  array[0] = element0;
  array[1] = element1;
  array[2] = element2;

  return array;
}

float *wrjs_array4(float element0, float element1, float element2, float element3) {
  static float array[4];
  array[0] = element0;
  array[1] = element1;
  array[2] = element2;
  array[3] = element3;

  return array;
}

char *wrjs_pointerOnFloat(float number) {
  static float storedNumber = number;
  return reinterpret_cast<char *>(&storedNumber);
}

int *wrjs_pointerOnInt(int number) {
  static int storedNumber = number;
  return reinterpret_cast<int *>(&storedNumber);
}

int *wrjs_pointerOnIntBis(int number) {
  static int storedNumber = number;
  return reinterpret_cast<int *>(&storedNumber);
}

void wrjs_init_context(int width, int height) {
  wren::JSHelper::initContext(width, height);
}

void wrjs_exit() {
  exit(0);
}

#endif
