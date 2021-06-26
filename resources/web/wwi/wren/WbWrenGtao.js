import {arrayXPointerFloat, pointerOnFloat} from './../nodes/utils/utils.js';
import WbWrenAbstractPostProcessingEffect from './WbWrenAbstractPostProcessingEffect.js';
import WbWrenPostProcessingEffects from './WbWrenPostProcessingEffects.js';

export default class WbWrenGtao extends WbWrenAbstractPostProcessingEffect {
  constructor() {
    super();
    this._halfResolution = false;

    this._near = 0.0;
    this._far = 0.0;
    this._fov = 0.78;
    this._radius = 2.0;
    this._flipNormalY = 0.0;
    this._frameCounter = 0;

    this._clipInfo = [0, 0, 0, 0];
    this._params = [0.0, 0.0, 0.0, 0.0];
    this._rotations = [60.0, 300.0, 180.0, 240.0, 120.0, 0.0];
    this._offsets = [0.0, 0.5, 0.25, 0.75];
    this._previousInverseViewMatrix = [1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0];

    this._previousAllocation = false;
  }

  applyOldInverseViewMatrixToWren() {
    if (typeof this._wrenPostProcessingEffect === 'undefined')
      return;

    if (this._previousAllocation)
      _free(this._previousInverseViewMatrixPointer);

    if (typeof this._previousInverseViewMatrix !== 'number') {
      this._previousInverseViewMatrixPointer = arrayXPointerFloat(this._previousInverseViewMatrix);
      this._previousAllocation = true;
    } else {
      this._previousInverseViewMatrixPointer = this._previousInverseViewMatrix;
      this._previousAllocation = false;
    }

    Module.ccall('wr_post_processing_effect_pass_set_program_parameter', null, ['number', 'string', 'number'], [this._temporalPass, 'previousInverseViewMatrix', this._previousInverseViewMatrixPointer]);
  }

  copyNewInverseViewMatrix(inverseViewMatrix) {
    this._previousInverseViewMatrix = inverseViewMatrix;
  }

  setFov(fov) {
    this._fov = fov;
    this._applyParametersToWren();
  }

  setHalfResolution(halfResolution) {
    this._halfResolution = halfResolution;
  }

  setQualityLevel(qualityLevel) {
    this._params[2] = 2 << (qualityLevel - 1);
    this._applyParametersToWren();
  }

  setRadius(radius) {
    this._radius = radius;
    this._applyParametersToWren();
  }

  setup(viewport) {
    if (typeof this._wrenPostProcessingEffect !== 'undefined') {
      // In case we want to update the viewport, the old postProcessingEffect has to be removed first
      if (this._wrenViewport === viewport)
        _wr_viewport_remove_post_processing_effect(this._wrenViewport, this._wrenPostProcessingEffect);

      _wr_post_processing_effect_delete(this._wrenPostProcessingEffect);
    }

    this._wrenViewport = viewport;

    let width = _wr_viewport_get_width(this._wrenViewport);
    let height = _wr_viewport_get_height(this._wrenViewport);

    if (this._halfResolution) {
      width = width <= 1.0 ? 2.0 : width;
      height = height <= 1.0 ? 2.0 : height;
    }

    const viewportFramebuffer = _wr_viewport_get_frame_buffer(this._wrenViewport);

    const depthTexture = _wr_frame_buffer_get_depth_texture(viewportFramebuffer);
    const normalTexture = _wr_frame_buffer_get_output_texture(viewportFramebuffer, 1);

    this._wrenPostProcessingEffect = WbWrenPostProcessingEffects.gtao(width, height, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGBA16F, depthTexture, normalTexture, this._halfResolution);

    this._gtaoPass = Module.ccall('wr_post_processing_effect_get_pass', 'number', ['number', 'string'], [this._wrenPostProcessingEffect, 'gtaoForwardPass']);
    this._temporalPass = Module.ccall('wr_post_processing_effect_get_pass', 'number', ['number', 'string'], [this._wrenPostProcessingEffect, 'temporalDenoise']);
    this._applyParametersToWren();

    _wr_viewport_set_ambient_occlusion_effect(this._wrenViewport, this._wrenPostProcessingEffect);
    _wr_post_processing_effect_setup(this._wrenPostProcessingEffect);

    this.hasBeenSetup = true;
  }

  // Private functions

  _applyParametersToWren() {
    if (!this._wrenPostProcessingEffect)
      return;

    this._clipInfo[0] = this._near;
    this._clipInfo[1] = this._far ? this._far : 1000000.0;
    this._clipInfo[2] = 0.5 * (_wr_viewport_get_height(this._wrenViewport) / (2.0 * Math.tan(this._fov * 0.5)));

    if (typeof this._clipInfoPointer !== 'undefined')
      _free(this._clipInfoPointer);
    this._clipInfoPointer = arrayXPointerFloat(this._clipInfo);

    Module.ccall('wr_post_processing_effect_pass_set_program_parameter', null, ['number', 'string', 'number'], [this._gtaoPass, 'clipInfo', this._clipInfoPointer]);

    this._params[0] = this._rotations[this._frameCounter % 6] / 360.0;
    this._params[1] = this._offsets[Math.floor(this._frameCounter / 6) % 4];

    if (typeof this._paramsPointer !== 'undefined')
      _free(this._paramsPointer);
    this._paramsPointer = arrayXPointerFloat(this._params);
    Module.ccall('wr_post_processing_effect_pass_set_program_parameter', null, ['number', 'string', 'number'], [this._gtaoPass, 'params', this._paramsPointer]);

    if (typeof this._radiusPointer !== 'undefined')
      _free(this._radiusPointer);
    this._radiusPointer = pointerOnFloat(this._radius);
    Module.ccall('wr_post_processing_effect_pass_set_program_parameter', null, ['number', 'string', 'number'], [this._gtaoPass, 'radius', this._radiusPointer]);

    if (typeof this._flipNormalYPointer !== 'undefined')
      _free(this._flipNormalYPointer);
    this._flipNormalYPointer = pointerOnFloat(this._flipNormalY);
    Module.ccall('wr_post_processing_effect_pass_set_program_parameter', null, ['number', 'string', 'number'], [this._gtaoPass, 'flipNormalY', this._flipNormalYPointer]);
    ++this._frameCounter;
  }
}
