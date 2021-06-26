import {arrayXPointerInt, arrayXPointerFloat} from './../nodes/utils/utils.js';
import WbVector3 from './../nodes/utils/WbVector3.js';
import WbWrenShaders from './WbWrenShaders.js';

export default class WbWrenPicker {
  constructor() {
    this.selectedId = -1;
    this.coordinates = new WbVector3();
    this._width = 0;
    this._height = 0;

    this._viewport = _wr_viewport_new();
    this._viewportDepth = _wr_viewport_new();

    const colorPointer = _wrjs_array4(0.0, 0.0, 0.0, 0.0);
    _wr_viewport_set_clear_color_rgba(this._viewport, colorPointer);

    this._setup();
  }

  pick(x, y) {
    const viewport = _wr_scene_get_viewport(_wr_scene_get_instance());
    // This fix a bug that occurs when switching between different worlds
    _wr_viewport_set_camera(this._viewport, _wr_viewport_get_camera(viewport));
    _wr_viewport_set_camera(this._viewportDepth, _wr_viewport_get_camera(viewport));

    this.coordinates.setXyz(0.0, 0.0, 0.0);
    this.selectedId = -1;

    // Recreate framebuffer and textures if viewport's size has changed
    if (this._hasSizeChanged()) {
      this._cleanup();
      this._setup();
    }

    // Check if object was picked & decode ID
    const scene = _wr_scene_get_instance();
    _wr_viewport_enable_skybox(this._viewport, false);
    _wr_scene_enable_translucence(scene, false);
    _wr_scene_enable_depth_reset(scene, false);
    Module.ccall('wr_scene_render_to_viewports', null, ['number', 'number', 'number', 'string', 'boolean'], [scene, 1, _wrjs_pointerOnInt(this._viewport), 'picking', true]);
    _wr_scene_enable_depth_reset(scene, true);
    _wr_viewport_enable_skybox(this._viewport, true);
    _wr_scene_enable_translucence(scene, true);

    let data = [];
    let dataPointer = arrayXPointerInt(data);
    _wr_frame_buffer_copy_pixel(this._frameBuffer, 0, x, y, dataPointer, true);
    data[0] = Module.getValue(dataPointer, 'i8');
    data[1] = Module.getValue(dataPointer + 1, 'i8');
    data[2] = Module.getValue(dataPointer + 2, 'i8');
    data[3] = Module.getValue(dataPointer + 3, 'i8');

    _free(dataPointer);

    data[0] = data[0] >= 0 ? data[0] : 256 + data[0];
    data[1] = data[1] >= 0 ? data[1] : 256 + data[1];
    data[2] = data[2] >= 0 ? data[2] : 256 + data[2];
    data[3] = data[3] >= 0 ? data[3] : 256 + data[3];

    const id = (data[2] << 24) | (data[1] << 16) | (data[0] << 8) | data[3];
    if (id === 0)
      return false;
    else
      this.selectedId = id - 1;

    _wr_viewport_enable_skybox(this._viewportDepth, false);
    _wr_scene_enable_translucence(scene, false);
    _wr_scene_enable_depth_reset(scene, false);
    Module.ccall('wr_scene_render_to_viewports', null, ['number', 'number', 'number', 'string', 'boolean'], [scene, 1, _wrjs_pointerOnIntBis(this._viewportDepth), 'depth', true]);
    _wr_scene_enable_depth_reset(scene, true);
    _wr_viewport_enable_skybox(this._viewportDepth, true);
    _wr_scene_enable_translucence(scene, true);

    data = [0, 0, 0, 0];
    dataPointer = arrayXPointerFloat(data);
    _wr_frame_buffer_copy_depth_pixel(this._frameBufferDepth, x, y, dataPointer, true);

    data[0] = Module.getValue(dataPointer, 'float');
    _free(dataPointer);

    this.coordinates = new WbVector3(x, this._height - y - 1, data[0]);
    return true;
  }

  // Setup & attach picking material, based on the unique ID
  // ID is encoded in the following way:
  // Most significant word: red and green channels of ambient color
  // Least signigicant word: red and green channels of diffuse color
  // These are combined in RGBA channels in the picking fragment shader
  static setPickable(renderable, uniqueId, pickable) {
    uniqueId = parseFloat(uniqueId.substring(1));

    let material = Module.ccall('wr_renderable_get_material', 'number', ['number', 'string'], [renderable, 'picking']);

    if (!material) {
      material = _wr_phong_material_new();
      _wr_material_set_default_program(material, WbWrenShaders.pickingShader());

      Module.ccall('wr_renderable_set_material', null, ['number', 'number', 'string'], [renderable, material, 'picking']);
    }

    let depthMaterial = Module.ccall('wr_renderable_get_material', 'number', ['number', 'string'], [renderable, 'depth']);

    if (!depthMaterial) {
      depthMaterial = _wr_phong_material_new();
      _wr_material_set_default_program(depthMaterial, WbWrenShaders.depthPixelShader());

      Module.ccall('wr_renderable_set_material', null, ['number', 'number', 'string'], [renderable, depthMaterial, 'depth']);
    }

    const encodedId = [0, 0, 0, 0, 0, 0];

    if (pickable) {
      // ID is incremented since a 0 value would mean that no object was picked
      // (ID = 0 is valid)
      let id = uniqueId + 1;

      for (let i = 4; i >= 0; --i) {
        if (i === 2)
          continue;

        encodedId[i] = (id & 0x000000FF) / 255.0;
        id >>= 8;
      }
    }

    const encodedIdPointer = arrayXPointerFloat(encodedId);
    const encodedIdPointerSecondHalf = arrayXPointerFloat([encodedId[3], encodedId[4], encodedId[5]]);
    _wr_phong_material_set_linear_ambient(material, encodedIdPointer);
    _wr_phong_material_set_linear_diffuse(material, encodedIdPointerSecondHalf);
    _free(encodedIdPointer);
    _free(encodedIdPointerSecondHalf);
  }

  // Private functions

  _cleanup() {
    if (typeof this._outputTexture !== 'undefined')
      _wr_texture_delete(this._outputTexture);
    if (typeof this._frameBuffer !== 'undefined')
      _wr_frame_buffer_delete(this._frameBuffer);
    if (typeof this._outputTextureDepth !== 'undefined')
      _wr_texture_delete(this._outputTextureDepth);
    if (typeof this._frameBufferDepth !== 'undefined')
      _wr_frame_buffer_delete(this._frameBufferDepth);
  }

  _hasSizeChanged() {
    const viewport = _wr_scene_get_viewport(_wr_scene_get_instance());
    const width = _wr_viewport_get_width(viewport);
    const height = _wr_viewport_get_height(viewport);

    if (this._width !== width || this._height !== height) {
      this._width = width;
      this._height = height;
      return true;
    }
    return false;
  }

  _setup() {
    const viewport = _wr_scene_get_viewport(_wr_scene_get_instance());
    this._width = _wr_viewport_get_width(viewport);
    this._height = _wr_viewport_get_height(viewport);
    _wr_viewport_set_size(this._viewport, this._width, this._height);

    this._frameBuffer = _wr_frame_buffer_new();
    this._outputTexture = _wr_texture_rtt_new();

    _wr_frame_buffer_set_size(this._frameBuffer, this._width, this._height);
    _wr_frame_buffer_enable_depth_buffer(this._frameBuffer, true);

    _wr_frame_buffer_append_output_texture(this._frameBuffer, this._outputTexture);
    _wr_frame_buffer_enable_copying(this._frameBuffer, 0, true);
    _wr_frame_buffer_setup(this._frameBuffer);

    _wr_viewport_set_frame_buffer(this._viewport, this._frameBuffer);
    _wr_viewport_set_camera(this._viewport, _wr_viewport_get_camera(viewport));

    // DEPTH
    _wr_viewport_set_size(this._viewportDepth, this._width, this._height);

    this._frameBufferDepth = _wr_frame_buffer_new();
    this._outputTextureDepth = _wr_texture_rtt_new();
    _wr_texture_set_internal_format(this._outputTextureDepth, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGBA16F);

    _wr_frame_buffer_set_size(this._frameBufferDepth, this._width, this._height);
    _wr_frame_buffer_enable_depth_buffer(this._frameBufferDepth, true);

    _wr_frame_buffer_append_output_texture(this._frameBufferDepth, this._outputTextureDepth);
    _wr_frame_buffer_setup(this._frameBufferDepth);

    _wr_viewport_set_frame_buffer(this._viewportDepth, this._frameBufferDepth);
    _wr_viewport_set_camera(this._viewportDepth, _wr_viewport_get_camera(viewport));
  }
}
