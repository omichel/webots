import WbLight from './WbLight.js';

export default class WbDirectionalLight extends WbLight {
  constructor(id, on, color, direction, intensity, castShadows, ambientIntensity) {
    super(id, on, color, intensity, castShadows, ambientIntensity);

    this.direction = direction;
  }

  createWrenObjects() {
    this._wrenLight = _wr_directional_light_new();
    super.createWrenObjects();

    this._applyLightDirectionToWren();
  }

  delete() {
    if (this.wrenObjectsCreatedCalled)
      _wr_node_delete(this._wrenLight);

    super.delete();
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbDirectionalLight(customID, this.on, this.color, this.direction, this.intensity, this.castShadows, this.ambientIntensity);
  }

  // Private functions

  _applyLightColorToWren() {
    const pointer = _wrjs_array3(this.color.x, this.color.y, this.color.z);

    _wr_directional_light_set_color(this._wrenLight, pointer);
  }

  _applyLightDirectionToWren() {
    const pointer = _wrjs_array3(this.direction.x, this.direction.y, this.direction.z);
    _wr_directional_light_set_direction(this._wrenLight, pointer);
  }

  _applyLightIntensityToWren() {
    _wr_directional_light_set_intensity(this._wrenLight, this.intensity);
  }

  _applyLightShadowsToWren() {
    _wr_directional_light_set_cast_shadows(this._wrenLight, this.castShadows);
  }

  _applyLightVisibilityToWren() {
    _wr_directional_light_set_on(this._wrenLight, this.on);

    const maxCount = _wr_config_get_max_active_directional_light_count();
    const activeCount = _wr_scene_get_active_directional_light_count(_wr_scene_get_instance());
    if (activeCount === maxCount)
      console.log('Maximum number of directional lights ' + maxCount + " has been reached, newly added lights won't be rendered.");
  }
}
