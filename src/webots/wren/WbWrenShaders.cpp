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

#include "WbWrenShaders.hpp"

#include "WbLog.hpp"
#include "WbWrenOpenGlContext.hpp"

#include <wren/glsl_layout.h>
#include <wren/shader_program.h>

#include <QtCore/QByteArray>
#include <QtCore/QFileInfo>
#include <QtCore/QTime>
#include <QtCore/QVector>

namespace {
  void buildShader(WrShaderProgram *shader, const QFileInfo &vertexShader, const QFileInfo &fragmentShader) {
    QByteArray vertexPathBytes = vertexShader.absoluteFilePath().toUtf8();
    QByteArray fragmentPathBytes = fragmentShader.absoluteFilePath().toUtf8();

    WbWrenOpenGlContext::makeWrenCurrent();
    wr_shader_program_set_vertex_shader_path(shader, vertexPathBytes.constData());
    wr_shader_program_set_fragment_shader_path(shader, fragmentPathBytes.constData());
    wr_shader_program_setup(shader);
    WbWrenOpenGlContext::doneWren();

    if (!wr_shader_program_get_gl_name(shader)) {
      QString msg("Shader compilation failed!");

      if (wr_shader_program_has_vertex_shader_compilation_failed(shader))
        msg += " Filename: " + vertexShader.fileName();
      else if (wr_shader_program_has_fragment_shader_compilation_failed(shader))
        msg += " Filename: " + fragmentShader.fileName();
      else
        msg += " Unable to link shader objects. Vertex shader: " + vertexShader.fileName() +
               ", fragment shader: " + fragmentShader.fileName();

      msg += "\n";
      msg += wr_shader_program_get_compilation_log(shader);
      msg += "\n";

      WbLog::instance()->error(msg);
    }
  }
};  // namespace

enum SHADER {
  SHADER_BLOOM_BLEND,
  SHADER_BOUNDING_VOLUME,
  SHADER_BRIGHT_PASS,
  SHADER_COLOR_NOISE,
  SHADER_COORDINATE_SYSTEM,
  SHADER_DEFAULT,
  SHADER_DEPTH_OF_FIELD,
  SHADER_DEPTH_ONLY,
  SHADER_DEPTH_RESOLUTION,
  SHADER_ENCODE_DEPTH,
  SHADER_FOG,
  SHADER_GAUSSIAN_BLUR,
  SHADER_GAUSSIAN_BLUR_5_TAP,
  SHADER_GAUSSIAN_BLUR_9_TAP,
  SHADER_GAUSSIAN_BLUR_13_TAP,
  SHADER_GTAO,
  SHADER_GTAO_SPATIAL_DENOISE,
  SHADER_GTAO_TEMPORAL_DENOISE,
  SHADER_GTAO_COMBINE,
  SHADER_HANDLES,
  SHADER_HANDLES_PICKING,
  SHADER_HDR_CLEAR,
  SHADER_HDR_RESOLVE,
  SHADER_IBL_DIFFUSE_IRRADIANCE_BAKE,
  SHADER_IBL_SPECULAR_IRRADIANCE_BAKE,
  SHADER_IBL_BRDF_BAKE,
  SHADER_LENS_DISTORTION,
  SHADER_LENS_FLARE,
  SHADER_LENS_FLARE_BLEND,
  SHADER_LIGHT_REPRESENTATION,
  SHADER_LINE_SET,
  SHADER_MERGE_SPHERICAL,
  SHADER_MOTION_BLUR,
  SHADER_NOISE_MASK,
  SHADER_OVERLAY,
  SHADER_PASS_THROUGH,
  SHADER_PBR,
  SHADER_PBR_STENCIL_AMBIENT_EMISSIVE,
  SHADER_PBR_STENCIL_DIFFUSE_SPECULAR,
  SHADER_PHONG,
  SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE,
  SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR,
  SHADER_PICKING,
  SHADER_POINT_SET,
  SHADER_RANGE_NOISE,
  SHADER_SEGMENTATION,
  SHADER_SHADOW_VOLUME,
  SHADER_SIMPLE,
  SHADER_SKYBOX,
  SHADER_SMAA_EDGE_DETECT_PASS,
  SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS,
  SHADER_SMAA_FINAL_BLEND_PASS,
  SHADER_COUNT
};

static QVector<WrShaderProgram *> gShaders(SHADER_COUNT, NULL);

WrShaderProgram *WbWrenShaders::blendLensFlareShader() {
  if (!gShaders[SHADER_LENS_FLARE_BLEND]) {
    gShaders[SHADER_LENS_FLARE_BLEND] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_LENS_FLARE_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_LENS_FLARE_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);

    const float defaultFloatValue = 0.5f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_FLARE_BLEND], "transparency",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloatValue));
    ::buildShader(gShaders[SHADER_LENS_FLARE_BLEND], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/blend_lens_flare.frag"));
  }

  return gShaders[SHADER_LENS_FLARE_BLEND];
}

WrShaderProgram *WbWrenShaders::brightPassShader() {
  if (!gShaders[SHADER_BRIGHT_PASS]) {
    gShaders[SHADER_BRIGHT_PASS] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_BRIGHT_PASS], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

    const float defaultThreshold = 10.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_BRIGHT_PASS], "threshold", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultThreshold));

    ::buildShader(gShaders[SHADER_BRIGHT_PASS], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/bright_pass.frag"));
  }

  return gShaders[SHADER_BRIGHT_PASS];
}

WrShaderProgram *WbWrenShaders::bloomBlendShader() {
  if (!gShaders[SHADER_BLOOM_BLEND]) {
    gShaders[SHADER_BLOOM_BLEND] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_BLOOM_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_BLOOM_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_BLOOM_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    wr_shader_program_use_uniform(gShaders[SHADER_BLOOM_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);
    wr_shader_program_use_uniform(gShaders[SHADER_BLOOM_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);
    wr_shader_program_use_uniform(gShaders[SHADER_BLOOM_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE5);
    wr_shader_program_use_uniform(gShaders[SHADER_BLOOM_BLEND], WR_GLSL_LAYOUT_UNIFORM_TEXTURE6);

    ::buildShader(gShaders[SHADER_BLOOM_BLEND], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/blend_bloom.frag"));
  }

  return gShaders[SHADER_BLOOM_BLEND];
}

WrShaderProgram *WbWrenShaders::boundingVolumeShader() {
  if (!gShaders[SHADER_BOUNDING_VOLUME]) {
    gShaders[SHADER_BOUNDING_VOLUME] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_BOUNDING_VOLUME], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_BOUNDING_VOLUME], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_BOUNDING_VOLUME], QFileInfo("gl:shaders/bounding_volume.vert"),
                  QFileInfo("gl:shaders/bounding_volume.frag"));
  }

  return gShaders[SHADER_BOUNDING_VOLUME];
}

WrShaderProgram *WbWrenShaders::colorNoiseShader() {
  if (!gShaders[SHADER_COLOR_NOISE]) {
    gShaders[SHADER_COLOR_NOISE] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_COLOR_NOISE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

    float time = static_cast<float>(QTime::currentTime().msec());
    wr_shader_program_create_custom_uniform(gShaders[SHADER_COLOR_NOISE], "time", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&time));

    float intensity = 0.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_COLOR_NOISE], "intensity", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&intensity));

    ::buildShader(gShaders[SHADER_COLOR_NOISE], QFileInfo("gl:shaders/color_noise.vert"),
                  QFileInfo("gl:shaders/color_noise.frag"));
  }

  return gShaders[SHADER_COLOR_NOISE];
}

WrShaderProgram *WbWrenShaders::coordinateSystemShader() {
  if (!gShaders[SHADER_COORDINATE_SYSTEM]) {
    gShaders[SHADER_COORDINATE_SYSTEM] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_COORDINATE_SYSTEM], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_COORDINATE_SYSTEM], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_COORDINATE_SYSTEM], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    const float defaultPositionOnScreen[2] = {0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_COORDINATE_SYSTEM], "screenPosition",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                            reinterpret_cast<const char *>(&defaultPositionOnScreen));
    const float defaultSize = 1.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_COORDINATE_SYSTEM], "size", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultSize));

    ::buildShader(gShaders[SHADER_COORDINATE_SYSTEM], QFileInfo("gl:shaders/coordinate_system.vert"),
                  QFileInfo("gl:shaders/coordinate_system.frag"));
  }

  return gShaders[SHADER_COORDINATE_SYSTEM];
}

WrShaderProgram *WbWrenShaders::defaultShader() {
  if (!gShaders[SHADER_DEFAULT]) {
    gShaders[SHADER_DEFAULT] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_DEFAULT], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);  // main texture
    wr_shader_program_use_uniform(gShaders[SHADER_DEFAULT], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);  // pen texture
    wr_shader_program_use_uniform(gShaders[SHADER_DEFAULT], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);  // background texture
    wr_shader_program_use_uniform(gShaders[SHADER_DEFAULT], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_DEFAULT], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_DEFAULT], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_DEFAULT], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_DEFAULT], QFileInfo("gl:shaders/default.vert"), QFileInfo("gl:shaders/default.frag"));
  }

  return gShaders[SHADER_DEFAULT];
}

WrShaderProgram *WbWrenShaders::depthOfFieldShader() {
  if (!gShaders[SHADER_DEPTH_OF_FIELD]) {
    gShaders[SHADER_DEPTH_OF_FIELD] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_DEPTH_OF_FIELD], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_DEPTH_OF_FIELD], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_DEPTH_OF_FIELD], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    wr_shader_program_use_uniform(gShaders[SHADER_DEPTH_OF_FIELD], WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    float dofParams[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_DEPTH_OF_FIELD], "dofParams", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F,
                                            reinterpret_cast<const char *>(&dofParams));

    float cameraParams[2] = {0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_DEPTH_OF_FIELD], "cameraParams",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                            reinterpret_cast<const char *>(&cameraParams));

    float blurTextureSize[2] = {0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_DEPTH_OF_FIELD], "blurTextureSize",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                            reinterpret_cast<const char *>(&blurTextureSize));

    ::buildShader(gShaders[SHADER_DEPTH_OF_FIELD], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/depth_of_field.frag"));
  }

  return gShaders[SHADER_DEPTH_OF_FIELD];
}

WrShaderProgram *WbWrenShaders::depthOnlyShader() {
  if (!gShaders[SHADER_DEPTH_ONLY]) {
    gShaders[SHADER_DEPTH_ONLY] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_DEPTH_ONLY], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_DEPTH_ONLY], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_DEPTH_ONLY], QFileInfo("gl:shaders/depth_only.vert"),
                  QFileInfo("gl:shaders/depth_only.frag"));
  }

  return gShaders[SHADER_DEPTH_ONLY];
}

WrShaderProgram *WbWrenShaders::depthResolutionShader() {
  if (!gShaders[SHADER_DEPTH_RESOLUTION]) {
    gShaders[SHADER_DEPTH_RESOLUTION] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_DEPTH_RESOLUTION], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

    float resolution = 0.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_DEPTH_RESOLUTION], "resolution",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT, reinterpret_cast<const char *>(&resolution));

    ::buildShader(gShaders[SHADER_DEPTH_RESOLUTION], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/depth_resolution.frag"));
  }

  return gShaders[SHADER_DEPTH_RESOLUTION];
}

WrShaderProgram *WbWrenShaders::encodeDepthShader() {
  if (!gShaders[SHADER_ENCODE_DEPTH]) {
    gShaders[SHADER_ENCODE_DEPTH] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_ENCODE_DEPTH], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_ENCODE_DEPTH], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    float minRange = 0.0f;
    float maxRange = 1.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_ENCODE_DEPTH], "minRange", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&minRange));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_ENCODE_DEPTH], "maxRange", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&maxRange));

    ::buildShader(gShaders[SHADER_ENCODE_DEPTH], QFileInfo("gl:shaders/encode_depth.vert"),
                  QFileInfo("gl:shaders/encode_depth.frag"));
  }

  return gShaders[SHADER_ENCODE_DEPTH];
}

WrShaderProgram *WbWrenShaders::fogShader() {
  if (!gShaders[SHADER_FOG]) {
    gShaders[SHADER_FOG] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_FOG], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_FOG], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_FOG], WR_GLSL_LAYOUT_UNIFORM_BUFFER_FOG);

    ::buildShader(gShaders[SHADER_FOG], QFileInfo("gl:shaders/fog.vert"), QFileInfo("gl:shaders/fog.frag"));
  }

  return gShaders[SHADER_FOG];
}

WrShaderProgram *WbWrenShaders::gaussianBlurShader() {
  if (!gShaders[SHADER_GAUSSIAN_BLUR]) {
    gShaders[SHADER_GAUSSIAN_BLUR] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR], WR_GLSL_LAYOUT_UNIFORM_ITERATION_NUMBER);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR], WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    // These values aren't initialized by default since the uniforms should be explicitly set before each shader invocation.
    int taps;
    float weights[4], offsets[4];
    wr_shader_program_create_custom_uniform(gShaders[SHADER_GAUSSIAN_BLUR], "taps", WR_SHADER_PROGRAM_UNIFORM_TYPE_INT,
                                            reinterpret_cast<const char *>(&taps));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_GAUSSIAN_BLUR], "weights", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F,
                                            reinterpret_cast<const char *>(&weights));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_GAUSSIAN_BLUR], "offsets", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F,
                                            reinterpret_cast<const char *>(&offsets));

    ::buildShader(gShaders[SHADER_GAUSSIAN_BLUR], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/gaussian_blur.frag"));
  }

  return gShaders[SHADER_GAUSSIAN_BLUR];
}

WrShaderProgram *WbWrenShaders::gaussianBlur5TapShader() {
  if (!gShaders[SHADER_GAUSSIAN_BLUR_5_TAP]) {
    gShaders[SHADER_GAUSSIAN_BLUR_5_TAP] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_5_TAP], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_5_TAP], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_5_TAP], WR_GLSL_LAYOUT_UNIFORM_ITERATION_NUMBER);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_5_TAP], WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    ::buildShader(gShaders[SHADER_GAUSSIAN_BLUR_5_TAP], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/gaussian_blur_5_tap.frag"));
  }

  return gShaders[SHADER_GAUSSIAN_BLUR_5_TAP];
}

WrShaderProgram *WbWrenShaders::gaussianBlur9TapShader() {
  if (!gShaders[SHADER_GAUSSIAN_BLUR_9_TAP]) {
    gShaders[SHADER_GAUSSIAN_BLUR_9_TAP] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_9_TAP], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_9_TAP], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_9_TAP], WR_GLSL_LAYOUT_UNIFORM_ITERATION_NUMBER);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_9_TAP], WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    ::buildShader(gShaders[SHADER_GAUSSIAN_BLUR_9_TAP], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/gaussian_blur_9_tap.frag"));
  }

  return gShaders[SHADER_GAUSSIAN_BLUR_9_TAP];
}

WrShaderProgram *WbWrenShaders::gaussianBlur13TapShader() {
  if (!gShaders[SHADER_GAUSSIAN_BLUR_13_TAP]) {
    gShaders[SHADER_GAUSSIAN_BLUR_13_TAP] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_13_TAP], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_13_TAP], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_GAUSSIAN_BLUR_13_TAP], WR_GLSL_LAYOUT_UNIFORM_ITERATION_NUMBER);

    ::buildShader(gShaders[SHADER_GAUSSIAN_BLUR_13_TAP], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/gaussian_blur_13_tap.frag"));
  }

  return gShaders[SHADER_GAUSSIAN_BLUR_13_TAP];
}

WrShaderProgram *WbWrenShaders::gtaoShader() {
  if (!gShaders[SHADER_GTAO]) {
    gShaders[SHADER_GTAO] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_GTAO], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO], WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_GTAO], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    const float params[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_GTAO], "params", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F,
                                            reinterpret_cast<const char *>(&params));

    const float clipInfo[4] = {0.0f, 1000000.0f, 0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_GTAO], "clipInfo", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F,
                                            reinterpret_cast<const char *>(&clipInfo));

    const float radius = 2.0;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_GTAO], "radius", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&radius));

    const float flipNormalY = 0.0;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_GTAO], "flipNormalY", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&flipNormalY));

    ::buildShader(gShaders[SHADER_GTAO], QFileInfo("gl:shaders/pass_through.vert"), QFileInfo("gl:shaders/gtao.frag"));
  }

  return gShaders[SHADER_GTAO];
}

WrShaderProgram *WbWrenShaders::gtaoSpatialDenoiseShader() {
  if (!gShaders[SHADER_GTAO_SPATIAL_DENOISE]) {
    gShaders[SHADER_GTAO_SPATIAL_DENOISE] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_SPATIAL_DENOISE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_SPATIAL_DENOISE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_GTAO_SPATIAL_DENOISE],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_GTAO_SPATIAL_DENOISE], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/gtao_spatial_denoise.frag"));
  }

  return gShaders[SHADER_GTAO_SPATIAL_DENOISE];
}

WrShaderProgram *WbWrenShaders::gtaoTemporalDenoiseShader() {
  if (!gShaders[SHADER_GTAO_TEMPORAL_DENOISE]) {
    gShaders[SHADER_GTAO_TEMPORAL_DENOISE] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_TEMPORAL_DENOISE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_TEMPORAL_DENOISE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_TEMPORAL_DENOISE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_TEMPORAL_DENOISE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_GTAO_TEMPORAL_DENOISE],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    const float defaultPreviousViewMatrix[16] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    wr_shader_program_create_custom_uniform(gShaders[SHADER_GTAO_TEMPORAL_DENOISE], "previousInverseViewMatrix",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F,
                                            reinterpret_cast<const char *>(&defaultPreviousViewMatrix));

    ::buildShader(gShaders[SHADER_GTAO_TEMPORAL_DENOISE], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/gtao_temporal_denoise.frag"));
  }

  return gShaders[SHADER_GTAO_TEMPORAL_DENOISE];
}

WrShaderProgram *WbWrenShaders::gtaoCombineShader() {
  if (!gShaders[SHADER_GTAO_COMBINE]) {
    gShaders[SHADER_GTAO_COMBINE] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_COMBINE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_COMBINE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_GTAO_COMBINE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);

    ::buildShader(gShaders[SHADER_GTAO_COMBINE], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/gtao_combine.frag"));
  }

  return gShaders[SHADER_GTAO_COMBINE];
}

WrShaderProgram *WbWrenShaders::handlesShader() {
  if (!gShaders[SHADER_HANDLES]) {
    gShaders[SHADER_HANDLES] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_HANDLES], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_HANDLES], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_HANDLES], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    const float defaultScaleOnScreen = 1.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_HANDLES], "screenScale", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultScaleOnScreen));

    ::buildShader(gShaders[SHADER_HANDLES], QFileInfo("gl:shaders/handles.vert"), QFileInfo("gl:shaders/handles.frag"));
  }

  return gShaders[SHADER_HANDLES];
}

WrShaderProgram *WbWrenShaders::handlesPickingShader() {
  if (!gShaders[SHADER_HANDLES_PICKING]) {
    gShaders[SHADER_HANDLES_PICKING] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_HANDLES_PICKING], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_HANDLES_PICKING], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_HANDLES_PICKING], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    const float defaultScaleOnScreen = 1.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_HANDLES_PICKING], "screenScale",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultScaleOnScreen));

    ::buildShader(gShaders[SHADER_HANDLES_PICKING], QFileInfo("gl:shaders/handles.vert"), QFileInfo("gl:shaders/picking.frag"));
  }

  return gShaders[SHADER_HANDLES_PICKING];
}

WrShaderProgram *WbWrenShaders::hdrClearShader() {
  if (!gShaders[SHADER_HDR_CLEAR]) {
    gShaders[SHADER_HDR_CLEAR] = wr_shader_program_new();

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_HDR_CLEAR], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);

    buildShader(gShaders[SHADER_HDR_CLEAR], QFileInfo("gl:shaders/pass_through.vert"), QFileInfo("gl:shaders/hdr_clear.frag"));
  }

  return gShaders[SHADER_HDR_CLEAR];
}

WrShaderProgram *WbWrenShaders::hdrResolveShader() {
  if (!gShaders[SHADER_HDR_RESOLVE]) {
    gShaders[SHADER_HDR_RESOLVE] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_HDR_RESOLVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

    const float defaultExposureValue = 1.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_HDR_RESOLVE], "exposure", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultExposureValue));

    buildShader(gShaders[SHADER_HDR_RESOLVE], QFileInfo("gl:shaders/pass_through.vert"),
                QFileInfo("gl:shaders/hdr_resolve.frag"));
  }

  return gShaders[SHADER_HDR_RESOLVE];
}

WrShaderProgram *WbWrenShaders::iblDiffuseIrradianceBakingShader() {
  if (!gShaders[SHADER_IBL_DIFFUSE_IRRADIANCE_BAKE]) {
    gShaders[SHADER_IBL_DIFFUSE_IRRADIANCE_BAKE] = wr_shader_program_new();

    const float projectionAndViewDefaults[16] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    wr_shader_program_create_custom_uniform(gShaders[SHADER_IBL_DIFFUSE_IRRADIANCE_BAKE], "projection",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F,
                                            reinterpret_cast<const char *>(&projectionAndViewDefaults));

    wr_shader_program_create_custom_uniform(gShaders[SHADER_IBL_DIFFUSE_IRRADIANCE_BAKE], "view",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F,
                                            reinterpret_cast<const char *>(&projectionAndViewDefaults));

    wr_shader_program_use_uniform(gShaders[SHADER_IBL_DIFFUSE_IRRADIANCE_BAKE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);

    ::buildShader(gShaders[SHADER_IBL_DIFFUSE_IRRADIANCE_BAKE], QFileInfo("gl:shaders/bake_cubemap.vert"),
                  QFileInfo("gl:shaders/bake_diffuse_cubemap.frag"));
  }

  return gShaders[SHADER_IBL_DIFFUSE_IRRADIANCE_BAKE];
}

WrShaderProgram *WbWrenShaders::iblSpecularIrradianceBakingShader() {
  if (!gShaders[SHADER_IBL_SPECULAR_IRRADIANCE_BAKE]) {
    gShaders[SHADER_IBL_SPECULAR_IRRADIANCE_BAKE] = wr_shader_program_new();

    const float projectionAndViewDefaults[16] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    const float roughness = 0.0f;

    wr_shader_program_create_custom_uniform(gShaders[SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], "projection",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F,
                                            reinterpret_cast<const char *>(&projectionAndViewDefaults));

    wr_shader_program_create_custom_uniform(gShaders[SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], "view",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F,
                                            reinterpret_cast<const char *>(&projectionAndViewDefaults));

    wr_shader_program_create_custom_uniform(gShaders[SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], "roughness",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT, reinterpret_cast<const char *>(&roughness));

    wr_shader_program_use_uniform(gShaders[SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);

    ::buildShader(gShaders[SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], QFileInfo("gl:shaders/bake_cubemap.vert"),
                  QFileInfo("gl:shaders/bake_specular_cubemap.frag"));
  }

  return gShaders[SHADER_IBL_SPECULAR_IRRADIANCE_BAKE];
}

WrShaderProgram *WbWrenShaders::iblBrdfBakingShader() {
  if (!gShaders[SHADER_IBL_BRDF_BAKE]) {
    gShaders[SHADER_IBL_BRDF_BAKE] = wr_shader_program_new();

    ::buildShader(gShaders[SHADER_IBL_BRDF_BAKE], QFileInfo("gl:shaders/bake_brdf.vert"),
                  QFileInfo("gl:shaders/bake_brdf.frag"));
  }

  return gShaders[SHADER_IBL_BRDF_BAKE];
}

WrShaderProgram *WbWrenShaders::lensDistortionShader() {
  if (!gShaders[SHADER_LENS_DISTORTION]) {
    gShaders[SHADER_LENS_DISTORTION] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_LENS_DISTORTION], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

    float center[2] = {0.5f, 0.5f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_DISTORTION], "center", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                            reinterpret_cast<const char *>(&center));

    float radialDistortionCoeffs[2] = {0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_DISTORTION], "radialDistortionCoeffs",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                            reinterpret_cast<const char *>(&radialDistortionCoeffs));

    float tangentialDistortionCoeffs[2] = {0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_DISTORTION], "tangentialDistortionCoeffs",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                            reinterpret_cast<const char *>(&tangentialDistortionCoeffs));

    ::buildShader(gShaders[SHADER_LENS_DISTORTION], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/lens_distortion.frag"));
  }

  return gShaders[SHADER_LENS_DISTORTION];
}

WrShaderProgram *WbWrenShaders::lightRepresentationShader() {
  if (!gShaders[SHADER_LIGHT_REPRESENTATION]) {
    gShaders[SHADER_LIGHT_REPRESENTATION] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_LIGHT_REPRESENTATION], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_LIGHT_REPRESENTATION], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_LIGHT_REPRESENTATION],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    const float defaultScreenScale = 1.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LIGHT_REPRESENTATION], "screenScale",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultScreenScale));

    ::buildShader(gShaders[SHADER_LIGHT_REPRESENTATION], QFileInfo("gl:shaders/light_representation.vert"),
                  QFileInfo("gl:shaders/light_representation.frag"));
  }

  return gShaders[SHADER_LIGHT_REPRESENTATION];
}

WrShaderProgram *WbWrenShaders::lineSetShader() {
  if (!gShaders[SHADER_LINE_SET]) {
    gShaders[SHADER_LINE_SET] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_LINE_SET], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_LINE_SET], WR_GLSL_LAYOUT_UNIFORM_COLOR_PER_VERTEX);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_LINE_SET], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_LINE_SET], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_LINE_SET], QFileInfo("gl:shaders/line_set.vert"), QFileInfo("gl:shaders/line_set.frag"));
  }

  return gShaders[SHADER_LINE_SET];
}

WrShaderProgram *WbWrenShaders::pointSetShader() {
  if (!gShaders[SHADER_POINT_SET]) {
    gShaders[SHADER_POINT_SET] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_POINT_SET], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_POINT_SET], WR_GLSL_LAYOUT_UNIFORM_COLOR_PER_VERTEX);
    wr_shader_program_use_uniform(gShaders[SHADER_POINT_SET], WR_GLSL_LAYOUT_UNIFORM_POINT_SIZE);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_POINT_SET], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_POINT_SET], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_POINT_SET], QFileInfo("gl:shaders/point_set.vert"), QFileInfo("gl:shaders/point_set.frag"));
  }

  return gShaders[SHADER_POINT_SET];
}

WrShaderProgram *WbWrenShaders::mergeSphericalShader() {
  if (!gShaders[SHADER_MERGE_SPHERICAL]) {
    gShaders[SHADER_MERGE_SPHERICAL] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_MERGE_SPHERICAL], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_MERGE_SPHERICAL], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_MERGE_SPHERICAL], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    wr_shader_program_use_uniform(gShaders[SHADER_MERGE_SPHERICAL], WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);
    wr_shader_program_use_uniform(gShaders[SHADER_MERGE_SPHERICAL], WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);
    wr_shader_program_use_uniform(gShaders[SHADER_MERGE_SPHERICAL], WR_GLSL_LAYOUT_UNIFORM_TEXTURE5);

    int defaultInt = 0;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MERGE_SPHERICAL], "rangeCamera", WR_SHADER_PROGRAM_UNIFORM_TYPE_INT,
                                            reinterpret_cast<const char *>(&defaultInt));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MERGE_SPHERICAL], "subCamerasResolutionX",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_INT, reinterpret_cast<const char *>(&defaultInt));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MERGE_SPHERICAL], "subCamerasResolutionY",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_INT, reinterpret_cast<const char *>(&defaultInt));

    float defaultFloat = 0.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MERGE_SPHERICAL], "minRange", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloat));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MERGE_SPHERICAL], "maxRange", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloat));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MERGE_SPHERICAL], "fovX", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloat));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MERGE_SPHERICAL], "fovY", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloat));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MERGE_SPHERICAL], "fovYCorrectionCoefficient",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloat));

    ::buildShader(gShaders[SHADER_MERGE_SPHERICAL], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/merge_spherical.frag"));
  }

  return gShaders[SHADER_MERGE_SPHERICAL];
}

WrShaderProgram *WbWrenShaders::motionBlurShader() {
  if (!gShaders[SHADER_MOTION_BLUR]) {
    gShaders[SHADER_MOTION_BLUR] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_MOTION_BLUR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_MOTION_BLUR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);

    float intensity = 0.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MOTION_BLUR], "intensity", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&intensity));

    float firstRender = 1.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_MOTION_BLUR], "firstRender", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&firstRender));

    ::buildShader(gShaders[SHADER_MOTION_BLUR], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/motion_blur.frag"));
  }

  return gShaders[SHADER_MOTION_BLUR];
}

WrShaderProgram *WbWrenShaders::overlayShader() {
  if (!gShaders[SHADER_OVERLAY]) {
    gShaders[SHADER_OVERLAY] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_OVERLAY], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);       // background
    wr_shader_program_use_uniform(gShaders[SHADER_OVERLAY], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);       // main
    wr_shader_program_use_uniform(gShaders[SHADER_OVERLAY], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);       // mask
    wr_shader_program_use_uniform(gShaders[SHADER_OVERLAY], WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);       // foreground
    wr_shader_program_use_uniform(gShaders[SHADER_OVERLAY], WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);       // close button
    wr_shader_program_use_uniform(gShaders[SHADER_OVERLAY], WR_GLSL_LAYOUT_UNIFORM_TEXTURE5);       // resize button
    wr_shader_program_use_uniform(gShaders[SHADER_OVERLAY], WR_GLSL_LAYOUT_UNIFORM_CHANNEL_COUNT);  // color channel count

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_OVERLAY], WR_GLSL_LAYOUT_UNIFORM_BUFFER_OVERLAY);

    ::buildShader(gShaders[SHADER_OVERLAY], QFileInfo("gl:shaders/overlay.vert"), QFileInfo("gl:shaders/overlay.frag"));
  }

  return gShaders[SHADER_OVERLAY];
}

WrShaderProgram *WbWrenShaders::passThroughShader() {
  if (!gShaders[SHADER_PASS_THROUGH]) {
    gShaders[SHADER_PASS_THROUGH] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_PASS_THROUGH], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

    ::buildShader(gShaders[SHADER_PASS_THROUGH], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/pass_through.frag"));
  }

  return gShaders[SHADER_PASS_THROUGH];
}

WrShaderProgram *WbWrenShaders::pbrShader() {
  if (!gShaders[SHADER_PBR]) {
    gShaders[SHADER_PBR] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);  // base color texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);  // roughness texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);  // metalness texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);  // occlusion map
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);  // normal map
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE5);  // BRDF LUT
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE6);  // emissive texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE7);  // background texture (for displays)
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE8);  // pen texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);  // irradiance cubemap
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PBR);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR], WR_GLSL_LAYOUT_UNIFORM_BUFFER_FOG);

    ::buildShader(gShaders[SHADER_PBR], QFileInfo("gl:shaders/pbr.vert"), QFileInfo("gl:shaders/pbr.frag"));
  }

  return gShaders[SHADER_PBR];
}

WrShaderProgram *WbWrenShaders::pbrStencilAmbientEmissiveShader() {
  if (!gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE]) {
    gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE] = wr_shader_program_new();

    // base color texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    // roughness texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    // metalness texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    // occlusion map
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);
    // normal map
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);
    // BRDF LUT
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE5);
    // emissive texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE6);
    // background texture (for displays)
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE7);
    // pen texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE8);
    // irradiance cubemap
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);
    // specular cubemap
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

    int wireframeRendering = 0;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], "wireframeRendering",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_INT,
                                            reinterpret_cast<const char *>(&wireframeRendering));

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PBR);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], QFileInfo("gl:shaders/pbr_stencil_ambient_emissive.vert"),
                  QFileInfo("gl:shaders/pbr_stencil_ambient_emissive.frag"));
  }

  return gShaders[SHADER_PBR_STENCIL_AMBIENT_EMISSIVE];
}

WrShaderProgram *WbWrenShaders::pbrStencilDiffuseSpecularShader() {
  if (!gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR]) {
    gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR] = wr_shader_program_new();

    // base color texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    // roughness texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    // metalness texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    // occlusion map
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);
    // normal map
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);
    // background texture (for displays)
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE7);
    // pen texture
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE8);
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PBR);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], QFileInfo("gl:shaders/pbr_stencil_diffuse_specular.vert"),
                  QFileInfo("gl:shaders/pbr_stencil_diffuse_specular.frag"));
  }

  return gShaders[SHADER_PBR_STENCIL_DIFFUSE_SPECULAR];
}

WrShaderProgram *WbWrenShaders::phongShader() {
  if (!gShaders[SHADER_PHONG]) {
    gShaders[SHADER_PHONG] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);  // main texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);  // pen texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);  // background texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG], WR_GLSL_LAYOUT_UNIFORM_BUFFER_FOG);

    ::buildShader(gShaders[SHADER_PHONG], QFileInfo("gl:shaders/phong.vert"), QFileInfo("gl:shaders/phong.frag"));
  }

  return gShaders[SHADER_PHONG];
}

WrShaderProgram *WbWrenShaders::phongStencilAmbientEmissiveShader() {
  if (!gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE]) {
    gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE] = wr_shader_program_new();

    // main texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    // pen texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    // background texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], QFileInfo("gl:shaders/phong_stencil_ambient_emissive.vert"),
                  QFileInfo("gl:shaders/phong_stencil_ambient_emissive.frag"));
  }

  return gShaders[SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE];
}

WrShaderProgram *WbWrenShaders::phongStencilDiffuseSpecularShader() {
  if (!gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR]) {
    gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR] = wr_shader_program_new();

    // main texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    // pen texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    // background texture
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR],
                                         WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], QFileInfo("gl:shaders/phong_stencil_diffuse_specular.vert"),
                  QFileInfo("gl:shaders/phong_stencil_diffuse_specular.frag"));
  }

  return gShaders[SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR];
}

WrShaderProgram *WbWrenShaders::pickingShader() {
  if (!gShaders[SHADER_PICKING]) {
    gShaders[SHADER_PICKING] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_PICKING], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PICKING], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_PICKING], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_PICKING], QFileInfo("gl:shaders/picking.vert"), QFileInfo("gl:shaders/picking.frag"));
  }

  return gShaders[SHADER_PICKING];
}

WrShaderProgram *WbWrenShaders::rangeNoiseShader() {
  if (!gShaders[SHADER_RANGE_NOISE]) {
    gShaders[SHADER_RANGE_NOISE] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_RANGE_NOISE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

    float time = static_cast<float>(QTime::currentTime().msec());
    wr_shader_program_create_custom_uniform(gShaders[SHADER_RANGE_NOISE], "time", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&time));

    float intensity = 0.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_RANGE_NOISE], "intensity", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&intensity));

    float minRange = 0.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_RANGE_NOISE], "minRange", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&minRange));

    float maxRange = 0.0f;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_RANGE_NOISE], "maxRange", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&maxRange));

    ::buildShader(gShaders[SHADER_RANGE_NOISE], QFileInfo("gl:shaders/range_noise.vert"),
                  QFileInfo("gl:shaders/range_noise.frag"));
  }

  return gShaders[SHADER_RANGE_NOISE];
}

WrShaderProgram *WbWrenShaders::segmentationShader() {
  if (!gShaders[SHADER_SEGMENTATION]) {
    gShaders[SHADER_SEGMENTATION] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_SEGMENTATION], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_SEGMENTATION], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_SEGMENTATION], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_SEGMENTATION], QFileInfo("gl:shaders/segmentation.vert"),
                  QFileInfo("gl:shaders/segmentation.frag"));
  }

  return gShaders[SHADER_SEGMENTATION];
}

WrShaderProgram *WbWrenShaders::shadowVolumeShader() {
  if (!gShaders[SHADER_SHADOW_VOLUME]) {
    gShaders[SHADER_SHADOW_VOLUME] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_SHADOW_VOLUME], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_SHADOW_VOLUME], WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_SHADOW_VOLUME], WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_SHADOW_VOLUME], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_SHADOW_VOLUME], QFileInfo("gl:shaders/shadow_volume.vert"),
                  QFileInfo("gl:shaders/shadow_volume.frag"));
  }

  return gShaders[SHADER_SHADOW_VOLUME];
}

WrShaderProgram *WbWrenShaders::simpleShader() {
  if (!gShaders[SHADER_SIMPLE]) {
    gShaders[SHADER_SIMPLE] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_SIMPLE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_SIMPLE], WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_SIMPLE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);
    wr_shader_program_use_uniform(gShaders[SHADER_SIMPLE], WR_GLSL_LAYOUT_UNIFORM_CHANNEL_COUNT);

    wr_shader_program_use_uniform_buffer(gShaders[SHADER_SIMPLE], WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_SIMPLE], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_SIMPLE], QFileInfo("gl:shaders/simple.vert"), QFileInfo("gl:shaders/simple.frag"));
  }

  return gShaders[SHADER_SIMPLE];
}

WrShaderProgram *WbWrenShaders::skyboxShader() {
  if (!gShaders[SHADER_SKYBOX]) {
    gShaders[SHADER_SKYBOX] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_SKYBOX], WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);
    wr_shader_program_use_uniform_buffer(gShaders[SHADER_SKYBOX], WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

    ::buildShader(gShaders[SHADER_SKYBOX], QFileInfo("gl:shaders/skybox.vert"), QFileInfo("gl:shaders/skybox.frag"));
  }

  return gShaders[SHADER_SKYBOX];
}

WrShaderProgram *WbWrenShaders::smaaEdgeDetectionShader() {
  if (!gShaders[SHADER_SMAA_EDGE_DETECT_PASS]) {
    gShaders[SHADER_SMAA_EDGE_DETECT_PASS] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_EDGE_DETECT_PASS], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_EDGE_DETECT_PASS], WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    ::buildShader(gShaders[SHADER_SMAA_EDGE_DETECT_PASS], QFileInfo("gl:shaders/smaa_edge_detect.vert"),
                  QFileInfo("gl:shaders/smaa_edge_detect.frag"));
  }

  return gShaders[SHADER_SMAA_EDGE_DETECT_PASS];
}

WrShaderProgram *WbWrenShaders::smaaBlendingWeightCalculationShader() {
  if (!gShaders[SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS]) {
    gShaders[SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    ::buildShader(gShaders[SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], QFileInfo("gl:shaders/smaa_blending_weights.vert"),
                  QFileInfo("gl:shaders/smaa_blending_weights.frag"));
  }

  return gShaders[SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS];
}

WrShaderProgram *WbWrenShaders::smaaFinalBlendShader() {
  if (!gShaders[SHADER_SMAA_FINAL_BLEND_PASS]) {
    gShaders[SHADER_SMAA_FINAL_BLEND_PASS] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_FINAL_BLEND_PASS], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_FINAL_BLEND_PASS], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
    wr_shader_program_use_uniform(gShaders[SHADER_SMAA_FINAL_BLEND_PASS], WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

    ::buildShader(gShaders[SHADER_SMAA_FINAL_BLEND_PASS], QFileInfo("gl:shaders/smaa_final_blend.vert"),
                  QFileInfo("gl:shaders/smaa_final_blend.frag"));
  }

  return gShaders[SHADER_SMAA_FINAL_BLEND_PASS];
}

WrShaderProgram *WbWrenShaders::noiseMaskShader() {
  if (!gShaders[SHADER_NOISE_MASK]) {
    gShaders[SHADER_NOISE_MASK] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_NOISE_MASK], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_NOISE_MASK], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);

    float offset[2] = {0.0f, 0.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_NOISE_MASK], "textureOffset", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                            reinterpret_cast<const char *>(offset));
    float factor[2] = {1.0f, 1.0f};
    wr_shader_program_create_custom_uniform(gShaders[SHADER_NOISE_MASK], "textureFactor", WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC2F,
                                            reinterpret_cast<const char *>(factor));

    ::buildShader(gShaders[SHADER_NOISE_MASK], QFileInfo("gl:shaders/pass_through.vert"),
                  QFileInfo("gl:shaders/noise_mask.frag"));
  }

  return gShaders[SHADER_NOISE_MASK];
}

WrShaderProgram *WbWrenShaders::lensFlareShader() {
  if (!gShaders[SHADER_LENS_FLARE]) {
    gShaders[SHADER_LENS_FLARE] = wr_shader_program_new();

    wr_shader_program_use_uniform(gShaders[SHADER_LENS_FLARE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
    wr_shader_program_use_uniform(gShaders[SHADER_LENS_FLARE], WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);

    const float defaultFloatValue = 0.0f;
    const int defaultIntValue = 0;
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_FLARE], "uBias", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloatValue));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_FLARE], "uScale", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloatValue));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_FLARE], "uGhostDispersal",
                                            WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloatValue));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_FLARE], "uHaloWidth", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloatValue));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_FLARE], "uDistortion", WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT,
                                            reinterpret_cast<const char *>(&defaultFloatValue));
    wr_shader_program_create_custom_uniform(gShaders[SHADER_LENS_FLARE], "uSamples", WR_SHADER_PROGRAM_UNIFORM_TYPE_INT,
                                            reinterpret_cast<const char *>(&defaultIntValue));

    ::buildShader(gShaders[SHADER_LENS_FLARE], QFileInfo("gl:shaders/lens_flare.vert"),
                  QFileInfo("gl:shaders/lens_flare.frag"));
  }

  return gShaders[SHADER_LENS_FLARE];
}

void WbWrenShaders::deleteShaders() {
  for (int i = 0; i < SHADER_COUNT; ++i) {
    wr_shader_program_delete(gShaders[i]);
    gShaders[i] = NULL;
  }
}
