export default class WbWrenAbstractPostProcessingEffect {
  constructor() {
    this.hasBeenSetup = false;
  }

  delete() {
    if (typeof this._wrenViewport !== 'undefined')
      _wr_viewport_remove_post_processing_effect(this._wrenViewport, this._wrenPostProcessingEffect);

    _wr_post_processing_effect_delete(this._wrenPostProcessingEffect);
  }

  detachFromViewport() {
    if (this._wrenViewport) {
      _wr_viewport_remove_post_processing_effect(this._wrenViewport, this._wrenPostProcessingEffect);
      _wr_post_processing_effect_delete(this._wrenPostProcessingEffect);
      this._wrenPostProcessingEffect = undefined;
      this._wrenViewport = undefined;
      this.hasBeenSetup = false;
    }
  }
}
