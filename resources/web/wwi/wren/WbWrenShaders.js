import {arrayXPointer} from './../nodes/utils/utils.js';

export default class WbWrenShaders {
  static buildShader(shader, vertexShaderPath, fragmentShaderpath) {
    Module.ccall('wr_shader_program_set_vertex_shader_path', null, ['number', 'string'], [shader, vertexShaderPath]);
    Module.ccall('wr_shader_program_set_fragment_shader_path', null, ['number', 'string'], [shader, fragmentShaderpath]);
    _wr_shader_program_setup(shader);

    if (!_wr_shader_program_get_gl_name(shader)) {
      console.error('Shader Error');
      if (_wr_shader_program_has_vertex_shader_compilation_failed(shader))
        console.error('Vertex shader compilation failed');
      else if (_wr_shader_program_has_fragment_shader_compilation_failed(shader))
        console.error('Fragment shader compilation failed');
      else
        console.error('Linkage failed');
    }
  }

  static bloomBlendShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE5);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE6);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/blend_bloom.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BLOOM_BLEND];
  }

  static brightPassShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BRIGHT_PASS]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BRIGHT_PASS] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BRIGHT_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

      const defaultThresholdPointer = _wrjs_pointerOnFloat(10.0);
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BRIGHT_PASS], 'threshold', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT, defaultThresholdPointer]);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BRIGHT_PASS], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/bright_pass.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_BRIGHT_PASS];
  }

  static defaultShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0); // main texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1); // pen texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2); // background texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT], '../../../resources/wren/shaders/default.vert', '../../../resources/wren/shaders/default.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEFAULT];
  }

  static depthPixelShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEPTH_PIXEL]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEPTH_PIXEL] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEPTH_PIXEL], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEPTH_PIXEL], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEPTH_PIXEL], '../../../resources/wren/shaders/web_depth.vert', '../../../resources/wren/shaders/web_depth.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_DEPTH_PIXEL];
  }

  static fogShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_FOG]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_FOG] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_FOG], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_FOG], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_FOG], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_FOG);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_FOG], '../../../resources/wren/shaders/fog.vert', '../../../resources/wren/shaders/fog.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_FOG];
  }

  static gaussianBlur13TapShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GAUSSIAN_BLUR_13_TAP]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GAUSSIAN_BLUR_13_TAP] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GAUSSIAN_BLUR_13_TAP], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GAUSSIAN_BLUR_13_TAP], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GAUSSIAN_BLUR_13_TAP], Enum.WR_GLSL_LAYOUT_UNIFORM_ITERATION_NUMBER);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GAUSSIAN_BLUR_13_TAP], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/gaussian_blur_13_tap.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GAUSSIAN_BLUR_13_TAP];
  }

  static gtaoShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], Enum.WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      const paramsPointer = _wrjs_array4(0, 0, 0, 0);
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], 'params', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F, paramsPointer]);

      const clipInfoPointer = _wrjs_array4(0.0, 1000000.0, 0.0, 0.0);
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], 'clipInfo', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_VEC4F, clipInfoPointer]);

      const radiusPointer = _wrjs_pointerOnFloat(2.0);
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], 'radius', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT, radiusPointer]);

      const flipNormalYPointer = _wrjs_pointerOnFloat(0.0);
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], 'flipNormalY', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT, flipNormalYPointer]);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/gtao.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO];
  }

  static gtaoSpatialDenoiseShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_SPATIAL_DENOISE]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_SPATIAL_DENOISE] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_SPATIAL_DENOISE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_SPATIAL_DENOISE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_SPATIAL_DENOISE], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_SPATIAL_DENOISE], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/gtao_spatial_denoise.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_SPATIAL_DENOISE];
  }

  static gtaoTemporalDenoiseShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      const previousInverseViewMatrix = [16].fill(0.0);
      const previousInverseViewMatrixPointer = arrayXPointer(previousInverseViewMatrix);
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE], 'previousInverseViewMatrix', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F, previousInverseViewMatrixPointer]);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/gtao_temporal_denoise.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_TEMPORAL_DENOISE];
  }

  static gtaoCombineShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_COMBINE]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_COMBINE] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_COMBINE], 0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_COMBINE], 1);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_COMBINE], 2);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_COMBINE], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/gtao_combine.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_GTAO_COMBINE];
  }

  static hdrClearShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_CLEAR]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_CLEAR] = _wr_shader_program_new();

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_CLEAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_CLEAR], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/hdr_clear.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_CLEAR];
  }

  static hdrResolveShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_RESOLVE]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_RESOLVE] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_RESOLVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

      const defaultExposureValue = 1.0;
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_RESOLVE], 'exposure', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT, _wrjs_pointerOnFloat(defaultExposureValue)]);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_RESOLVE], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/hdr_resolve.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_HDR_RESOLVE];
  }

  static iblSpecularIrradianceBakingShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_SPECULAR_IRRADIANCE_BAKE]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_SPECULAR_IRRADIANCE_BAKE] = _wr_shader_program_new();

      const projectionAndViewDefaults = [16].fill(0.0);
      const projectionAndViewDefaultsPointer = arrayXPointer(projectionAndViewDefaults);
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], 'projection', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F, projectionAndViewDefaultsPointer]);
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], 'view', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_MAT4F, projectionAndViewDefaultsPointer]);

      const roughness = 0.0;
      Module.ccall('wr_shader_program_create_custom_uniform', null, ['number', 'string', 'number', 'number'], [WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], 'roughness', Enum.WR_SHADER_PROGRAM_UNIFORM_TYPE_FLOAT, _wrjs_pointerOnFloat(roughness)]);

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_SPECULAR_IRRADIANCE_BAKE], '../../../resources/wren/shaders/bake_cubemap.vert', '../../../resources/wren/shaders/bake_specular_cubemap.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_SPECULAR_IRRADIANCE_BAKE];
  }

  static iblBrdfBakingShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_BRDF_BAKE]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_BRDF_BAKE] = _wr_shader_program_new();

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_BRDF_BAKE], '../../../resources/wren/shaders/bake_brdf.vert', '../../../resources/wren/shaders/bake_brdf.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_IBL_BRDF_BAKE];
  }

  static lineSetShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_LINE_SET]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_LINE_SET] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_LINE_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_LINE_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_COLOR_PER_VERTEX);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_LINE_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_LINE_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_LINE_SET], '../../../resources/wren/shaders/line_set.vert', '../../../resources/wren/shaders/line_set.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_LINE_SET];
  }

  static passThroughShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PASS_THROUGH]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PASS_THROUGH] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PASS_THROUGH], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PASS_THROUGH], '../../../resources/wren/shaders/pass_through.vert', '../../../resources/wren/shaders/pass_through.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PASS_THROUGH];
  }

  static pbrShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0); // base color texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1); // roughness texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2); // metalness texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE3); // occlusion map
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE4); // normal map
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE5); // BRDF LUT
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE6); // emissive texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE7); // background texture (for displays)
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE8); // pen texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0); // irradiance cubemap
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PBR);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_FOG);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR], '../../../resources/wren/shaders/pbr.vert', '../../../resources/wren/shaders/pbr.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR];
  }

  static pbrStencilAmbientEmissiveShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE] = _wr_shader_program_new();

      // base color texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      // roughness texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
      // metalness texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
      // occlusion map
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);
      // normal map
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);
      // BRDF LUT
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE5);
      // emissive texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE6);
      // background texture (for displays)
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE7);
      // pen texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE8);
      // irradiance cubemap
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);
      // specular cubemap
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PBR);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE], '../../../resources/wren/shaders/pbr_stencil_ambient_emissive.vert', '../../../resources/wren/shaders/pbr_stencil_ambient_emissive.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_AMBIENT_EMISSIVE];
  }

  static pbrStencilDiffuseSpecularShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR] = _wr_shader_program_new();

      // base color texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      // roughness texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
      // metalness texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
      // occlusion map
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE3);
      // normal map
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE4);
      // background texture (for displays)
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE7);
      // pen texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE8);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PBR);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR], '../../../resources/wren/shaders/pbr_stencil_diffuse_specular.vert', '../../../resources/wren/shaders/pbr_stencil_diffuse_specular.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PBR_STENCIL_DIFFUSE_SPECULAR];
  }

  static phongShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0); // main texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1); // pen texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2); // background texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_FOG);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG], '../../../resources/wren/shaders/phong.vert', '../../../resources/wren/shaders/phong.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG];
  }

  static phongStencilAmbientEmissiveShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0); // main texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1); // pen texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2); // background texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE], '../../../resources/wren/shaders/phong_stencil_ambient_emissive.vert', '../../../resources/wren/shaders/phong_stencil_ambient_emissive.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE];
  }

  static phongStencilDiffuseSpecularShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);// main texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);// pen texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);// background texture
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR], '../../../resources/wren/shaders/phong_stencil_diffuse_specular.vert', '../../../resources/wren/shaders/phong_stencil_diffuse_specular.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR];
  }

  static pickingShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PICKING]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PICKING] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PICKING], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PICKING], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PICKING], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PICKING], '../../../resources/wren/shaders/picking.vert', '../../../resources/wren/shaders/picking.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_PICKING];
  }

  static pointSetShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_COLOR_PER_VERTEX);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_POINT_SIZE);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_MATERIAL_PHONG);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET], '../../../resources/wren/shaders/point_set.vert', '../../../resources/wren/shaders/point_set.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_POINT_SET];
  }

  static shadowVolumeShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SHADOW_VOLUME]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SHADOW_VOLUME] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SHADOW_VOLUME], Enum.WR_GLSL_LAYOUT_UNIFORM_MODEL_TRANSFORM);

      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SHADOW_VOLUME], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHTS);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SHADOW_VOLUME], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_LIGHT_RENDERABLE);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SHADOW_VOLUME], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SHADOW_VOLUME], '../../../resources/wren/shaders/shadow_volume.vert', '../../../resources/wren/shaders/shadow_volume.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SHADOW_VOLUME];
  }

  static skyboxShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SKYBOX]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SKYBOX] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SKYBOX], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE_CUBE0);
      _wr_shader_program_use_uniform_buffer(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SKYBOX], Enum.WR_GLSL_LAYOUT_UNIFORM_BUFFER_CAMERA_TRANSFORMS);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SKYBOX], '../../../resources/wren/shaders/skybox.vert', '../../../resources/wren/shaders/skybox.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SKYBOX];
  }

  static smaaEdgeDetectionShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_EDGE_DETECT_PASS]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_EDGE_DETECT_PASS] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_EDGE_DETECT_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_EDGE_DETECT_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_EDGE_DETECT_PASS], '../../../resources/wren/shaders/smaa_edge_detect.vert', '../../../resources/wren/shaders/smaa_edge_detect.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_EDGE_DETECT_PASS];
  }

  static smaaBlendingWeightCalculationShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE2);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS], '../../../resources/wren/shaders/smaa_blending_weights.vert', '../../../resources/wren/shaders/smaa_blending_weights.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS];
  }

  static smaaFinalBlendShader() {
    if (!WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_FINAL_BLEND_PASS]) {
      WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_FINAL_BLEND_PASS] = _wr_shader_program_new();

      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_FINAL_BLEND_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE0);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_FINAL_BLEND_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_TEXTURE1);
      _wr_shader_program_use_uniform(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_FINAL_BLEND_PASS], Enum.WR_GLSL_LAYOUT_UNIFORM_VIEWPORT_SIZE);

      WbWrenShaders.buildShader(WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_FINAL_BLEND_PASS], '../../../resources/wren/shaders/smaa_final_blend.vert', '../../../resources/wren/shaders/smaa_final_blend.frag');
    }

    return WbWrenShaders.gShaders[WbWrenShaders.SHADER.SHADER_SMAA_FINAL_BLEND_PASS];
  }
}

// gShaders static variable
WbWrenShaders.gShaders = {};

WbWrenShaders.SHADER = {
  SHADER_BLOOM_BLEND: 1,
  SHADER_BRIGHT_PASS: 2,
  SHADER_DEFAULT: 3,
  SHADER_DEPTH_PIXEL: 4,
  SHADER_FOG: 5,
  SHADER_GAUSSIAN_BLUR_13_TAP: 6,
  SHADER_GTAO: 7,
  SHADER_GTAO_SPATIAL_DENOISE: 8,
  SHADER_GTAO_TEMPORAL_DENOISE: 9,
  SHADER_GTAO_COMBINE: 10,
  SHADER_HDR_CLEAR: 11,
  SHADER_HDR_RESOLVE: 12,
  SHADER_IBL_SPECULAR_IRRADIANCE_BAKE: 13,
  SHADER_IBL_BRDF_BAKE: 14,
  SHADER_LINE_SET: 15,
  SHADER_PASS_THROUGH: 16,
  SHADER_PBR: 17,
  SHADER_PBR_STENCIL_AMBIENT_EMISSIVE: 18,
  SHADER_PBR_STENCIL_DIFFUSE_SPECULAR: 19,
  SHADER_PHONG: 20,
  SHADER_PHONG_STENCIL_AMBIENT_EMISSIVE: 21,
  SHADER_PHONG_STENCIL_DIFFUSE_SPECULAR: 22,
  SHADER_PICKING: 23,
  SHADER_POINT_SET: 24,
  SHADER_SHADOW_VOLUME: 25,
  SHADER_SKYBOX: 26,
  SHADER_SMAA_EDGE_DETECT_PASS: 27,
  SHADER_SMAA_BLENDING_WEIGHT_CALCULATION_PASS: 28,
  SHADER_SMAA_FINAL_BLEND_PASS: 29
};
