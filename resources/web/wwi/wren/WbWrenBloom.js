import {pointerOnFloat} from './../nodes/utils/utils.js';
import WbWrenAbstractPostProcessingEffect from './WbWrenAbstractPostProcessingEffect.js';
import WbWrenPostProcessingEffects from './WbWrenPostProcessingEffects.js';

export default class WbWrenBloom extends WbWrenAbstractPostProcessingEffect {
  constructor() {
    super();
    this.threshold = 10.0;
  }

  setThreshold(threshold) {
    this.threshold = threshold;

    this._applyParametersToWren();
  }

  setup(viewport) {
    if (typeof this._wrenPostProcessingEffect !== 'undefined') {
      // In case we want to update the viewport, the old postProcessingEffect has to be removed first
      if (this._wrenViewport === viewport)
        _wr_viewport_remove_post_processing_effect(this._wrenViewport, this._wrenPostProcessingEffect);

      _wr_post_processing_effect_delete(this._wrenPostProcessingEffect);
      this._wrenPostProcessingEffect = undefined;
    }

    this._wrenViewport = viewport;

    const width = _wr_viewport_get_width(this._wrenViewport);
    const height = _wr_viewport_get_height(this._wrenViewport);

    // can't use the effect on resolutions smaller than this, it requires 6 passes dividing the viewport each time, so resolutions
    // smaller than 2^6 in width or height preculde the use of this effect
    if (Math.min(width, height) <= 64.0)
      return;

    this._wrenPostProcessingEffect = WbWrenPostProcessingEffects.bloom(width, height, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGBA16F);

    this._applyParametersToWren();

    _wr_viewport_add_post_processing_effect(this._wrenViewport, this._wrenPostProcessingEffect);
    _wr_post_processing_effect_setup(this._wrenPostProcessingEffect);

    this.hasBeenSetup = true;
  }

  // Private functions

  _applyParametersToWren() {
    if (!this._wrenPostProcessingEffect)
      return;
    const pass = Module.ccall('wr_post_processing_effect_get_pass', 'number', ['number', 'string'], [this._wrenPostProcessingEffect, 'brightPassFilter']);
    if (this._thresholdPointer !== 'undefined')
      _free(this._thresholdPointer);
    this._thresholdPointer = pointerOnFloat(this.threshold);
    Module.ccall('wr_post_processing_effect_pass_set_program_parameter', null, ['number', 'string', 'number'], [pass, 'threshold', this._thresholdPointer]);
  }
}
