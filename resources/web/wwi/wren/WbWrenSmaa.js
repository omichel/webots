import WbWrenAbstractPostProcessingEffect from './WbWrenAbstractPostProcessingEffect.js';
import WbWrenPostProcessingEffects from './WbWrenPostProcessingEffects.js';

export default class WbWrenSmaa extends WbWrenAbstractPostProcessingEffect {
  setup(viewport) {
    if (typeof this._wrenPostProcessingEffect === 'undefined') {
      // In case we want to update the viewport, the old postProcessingEffect has to be removed first
      if (this._wrenViewport === viewport)
        _wr_viewport_remove_post_processing_effect(this._wrenViewport, this._wrenPostProcessingEffect);

      _wr_post_processing_effect_delete(this._wrenPostProcessingEffect);
    }

    this._wrenViewport = viewport;

    const width = _wr_viewport_get_width(this._wrenViewport);
    const height = _wr_viewport_get_height(this._wrenViewport);

    this._wrenPostProcessingEffect = WbWrenPostProcessingEffects.smaa(width, height, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGBA8);

    _wr_viewport_set_anti_aliasing_effect(this._wrenViewport, this._wrenPostProcessingEffect);
    _wr_post_processing_effect_setup(this._wrenPostProcessingEffect);

    this.hasBeenSetup = true;
  }
}
