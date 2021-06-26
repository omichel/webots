import WbWorld from './nodes/WbWorld.js';
import {disableShadows} from './nodes/wb_preferences.js';

export default class WrenRenderer {
  constructor() {
    this._canvas = document.createElement('canvas');
    this._canvas.id = 'canvas';
    let div = document.getElementById('view3d');

    if (typeof div === 'undefined' || div === null)
      div = document.getElementsByTagName('webots-streaming');
    div.insertBefore(this._canvas, div.firstChild);

    _wr_config_enable_shadows(!disableShadows);

    // glGetError causes freeze with Firefox on Windows 10
    _wr_gl_state_disable_check_error();
  }

  setSize(width, height) {
    this._canvas.width = width;
    this._canvas.height = height;
  }

  render() {
    if (!_wr_gl_state_is_initialized())
      return;

    try {
      WbWorld.instance.viewpoint.updatePostProcessingParameters();

      _wr_scene_render(_wr_scene_get_instance(), null, true);
    } catch (error) {
      console.log('No Context');
    }
  }

  renderMinimal() {
    if (!_wr_gl_state_is_initialized())
      return;

    try {
      _wr_scene_render(_wr_scene_get_instance(), null, true);
    } catch (error) {
      console.log('No Context');
    }
  }
}
