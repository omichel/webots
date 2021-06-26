import {GtaoLevel} from './wb_preferences.js';
import WbWorld from './WbWorld.js';
import WbWrenPostProcessingEffects from './../wren/WbWrenPostProcessingEffects.js';
import WbWrenShaders from './../wren/WbWrenShaders.js';

export default class WbScene {
  constructor(smaaAreaTexture, smaaSearchTexture, gtaoNoiseTexture) {
    _wrjs_init_context(canvas.clientWidth, canvas.clientHeight);

    _wr_scene_init(_wr_scene_get_instance());

    _wr_gl_state_set_context_active(true);

    this.updateFrameBuffer();

    _wr_scene_set_fog_program(_wr_scene_get_instance(), WbWrenShaders.fogShader());
    _wr_scene_set_shadow_volume_program(_wr_scene_get_instance(), WbWrenShaders.shadowVolumeShader());

    WbWrenPostProcessingEffects.loadResources(smaaAreaTexture, smaaSearchTexture, gtaoNoiseTexture);
    this._updateWrenViewportDimensions();
  }

  destroy() {
    WbWrenPostProcessingEffects.clearResources();

    if (typeof this._wrenMainFrameBuffer !== 'undefined')
      _wr_frame_buffer_delete(this._wrenMainFrameBuffer);

    if (typeof this._wrenMainFrameBufferTexture !== 'undefined')
      _wr_texture_delete(this._wrenMainFrameBufferTexture);

    if (typeof this._wrenNormalFrameBufferTexture !== 'undefined')
      _wr_texture_delete(this._wrenNormalFrameBufferTexture);

    if (typeof this._wrenDepthFrameBufferTexture !== 'undefined')
      _wr_texture_delete(this._wrenDepthFrameBufferTexture);

    this._wrenMainFrameBuffer = undefined;
    this._wrenMainFrameBufferTexture = undefined;
    this._wrenNormalFrameBufferTexture = undefined;
    this._wrenDepthFrameBufferTexture = undefined;

    WbWorld.instance.scene = undefined;
  }

  updateFrameBuffer() {
    if (typeof this._wrenMainFrameBuffer !== 'undefined')
      _wr_frame_buffer_delete(this._wrenMainFrameBuffer);

    if (typeof this._wrenMainFrameBufferTexture !== 'undefined')
      _wr_texture_delete(this._wrenMainFrameBufferTexture);

    if (typeof this._wrenNormalFrameBufferTexture !== 'undefined')
      _wr_texture_delete(this._wrenNormalFrameBufferTexture);

    if (typeof this._wrenDepthFrameBufferTexture !== 'undefined')
      _wr_texture_delete(this._wrenDepthFrameBufferTexture);

    this._wrenMainFrameBuffer = _wr_frame_buffer_new();
    _wr_frame_buffer_set_size(this._wrenMainFrameBuffer, canvas.width, canvas.height);

    this._wrenMainFrameBufferTexture = _wr_texture_rtt_new();
    _wr_texture_set_internal_format(this._wrenMainFrameBufferTexture, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGBA16F);

    this._wrenNormalFrameBufferTexture = _wr_texture_rtt_new();

    _wr_texture_set_internal_format(this._wrenNormalFrameBufferTexture, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGBA8);
    _wr_frame_buffer_append_output_texture(this._wrenMainFrameBuffer, this._wrenMainFrameBufferTexture);
    if (GtaoLevel < 1)
      _wr_frame_buffer_append_output_texture_disable(this._wrenMainFrameBuffer, this._wrenNormalFrameBufferTexture);
    else
      _wr_frame_buffer_append_output_texture(this._wrenMainFrameBuffer, this._wrenNormalFrameBufferTexture);
    _wr_frame_buffer_enable_depth_buffer(this._wrenMainFrameBuffer, true);

    this._wrenDepthFrameBufferTexture = _wr_texture_rtt_new();
    _wr_texture_set_internal_format(this._wrenDepthFrameBufferTexture, Enum.WR_TEXTURE_INTERNAL_FORMAT_DEPTH24_STENCIL8);
    _wr_frame_buffer_set_depth_texture(this._wrenMainFrameBuffer, this._wrenDepthFrameBufferTexture);

    _wr_frame_buffer_setup(this._wrenMainFrameBuffer);
    _wr_viewport_set_frame_buffer(_wr_scene_get_viewport(_wr_scene_get_instance()), this._wrenMainFrameBuffer);

    _wr_viewport_set_size(_wr_scene_get_viewport(_wr_scene_get_instance()), canvas.width, canvas.height);
  }

  // Private functions
  _updateWrenViewportDimensions() {
    _wr_viewport_set_pixel_ratio(_wr_scene_get_viewport(_wr_scene_get_instance()), 1);
  }
}
