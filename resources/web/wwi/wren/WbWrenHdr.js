import {pointerOnFloat} from './../nodes/utils/utils.js';
import WbWrenAbstractPostProcessingEffect from './WbWrenAbstractPostProcessingEffect.js';
import WbWrenRenderingContext from './WbWrenRenderingContext.js';
import WbWrenShaders from './WbWrenShaders.js';

export default class WbWrenHdr extends WbWrenAbstractPostProcessingEffect {
  constructor() {
    super();
    this.exposure = 1.0;
  }

  setExposure(exposure) {
    this.exposure = exposure;

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

    const width = _wr_viewport_get_width(this._wrenViewport);
    const height = _wr_viewport_get_height(this._wrenViewport);

    this._wrenPostProcessingEffect = this._hdrResolve(width, height);

    this._applyParametersToWren();

    _wr_viewport_add_post_processing_effect(this._wrenViewport, this._wrenPostProcessingEffect);
    _wr_post_processing_effect_setup(this._wrenPostProcessingEffect);

    this.hasBeenSetup = true;
  }

  // Private functions

  _applyParametersToWren() {
    if (!this._wrenPostProcessingEffect)
      return;

    const firstPass = _wr_post_processing_effect_get_first_pass(this._wrenPostProcessingEffect);
    if (typeof this._exposurePointer !== 'undefined')
      _free(this._exposurePointer);
    this._exposurePointer = pointerOnFloat(this.exposure);
    Module.ccall('wr_post_processing_effect_pass_set_program_parameter', null, ['number', 'string', 'number'], [firstPass, 'exposure', this._exposurePointer]);
  }

  _hdrResolve(width, height) {
    const hdrResolveEffect = _wr_post_processing_effect_new();
    _wr_post_processing_effect_set_drawing_index(hdrResolveEffect, WbWrenRenderingContext.PP_HDR);

    const hdrPass = _wr_post_processing_effect_pass_new();
    Module.ccall('wr_post_processing_effect_pass_set_name', null, ['number', 'string'], [hdrPass, 'hdrResolve']);
    _wr_post_processing_effect_pass_set_program(hdrPass, WbWrenShaders.hdrResolveShader());
    _wr_post_processing_effect_pass_set_output_size(hdrPass, width, height);
    _wr_post_processing_effect_pass_set_alpha_blending(hdrPass, false);
    _wr_post_processing_effect_pass_set_input_texture_count(hdrPass, 1);
    _wr_post_processing_effect_pass_set_output_texture_count(hdrPass, 1);
    _wr_post_processing_effect_pass_set_output_texture_format(hdrPass, 0, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGB8);
    _wr_post_processing_effect_append_pass(hdrResolveEffect, hdrPass);

    _wr_post_processing_effect_set_result_program(hdrResolveEffect, WbWrenShaders.passThroughShader());

    return hdrResolveEffect;
  }
}
