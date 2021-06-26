import {findUpperTransform} from './utils/utils.js';
import WbLight from './WbLight.js';

export default class WbPointLight extends WbLight {
  constructor(id, on, attenuation, color, intensity, location, radius, ambientIntensity, castShadows, parent) {
    super(id, on, color, intensity, castShadows, ambientIntensity);
    this.attenuation = attenuation;
    this.location = location;
    this.radius = radius;

    if (typeof parent !== 'undefined')
      this.parent = parent.id;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbPointLight(customID, this.on, this.attenuation, this.color, this.intensity, this.location, this.radius, this.ambientIntensity, this.castShadows);
  }

  createWrenObjects() {
    this._wrenLight = _wr_point_light_new();
    this._attachToUpperTransform();
    super.createWrenObjects();

    this._applyLightAttenuationToWren();
    this._applyNodeLocationToWren();
  }

  delete() {
    if (this.wrenObjectsCreatedCalled) {
      this._detachFromUpperTransform();
      _wr_node_delete(this._wrenLight);
    }

    super.delete();
  }

  // Private functions

  _attachToUpperTransform() {
    const upperTransform = findUpperTransform(this);

    if (typeof upperTransform !== 'undefined')
      _wr_transform_attach_child(upperTransform.wrenNode, this._wrenLight);
  }

  _applyLightAttenuationToWren() {
    _wr_point_light_set_radius(this._wrenLight, this.radius);
    _wr_point_light_set_attenuation(this._wrenLight, this.attenuation.x, this.attenuation.y, this.attenuation.z);
  }

  _applyLightColorToWren() {
    const pointer = _wrjs_array3(this.color.x, this.color.y, this.color.z);

    _wr_point_light_set_color(this._wrenLight, pointer);
  }

  _applyLightIntensityToWren() {
    _wr_point_light_set_intensity(this._wrenLight, this.intensity);
  }

  _applyLightShadowsToWren() {
    _wr_point_light_set_cast_shadows(this._wrenLight, this.castShadows);
  }

  _applyLightVisibilityToWren() {
    _wr_point_light_set_on(this._wrenLight, this.on);

    const maxCount = _wr_config_get_max_active_point_light_count();
    const activeCount = _wr_scene_get_active_point_light_count(_wr_scene_get_instance());
    if (activeCount === maxCount)
      console.log("Maximum number of active point lights has been reached, newly added lights won't be rendered.");
  }

  _applyNodeLocationToWren() {
    const position = _wrjs_array3(this.location.x, this.location.y, this.location.z);
    _wr_point_light_set_position_relative(this._wrenLight, position);
  }

  _detachFromUpperTransform() {
    const node = this._wrenLight;
    const parent = _wr_node_get_parent(node);
    if (typeof parent !== 'undefined')
      _wr_transform_detach_child(parent, node);
  }
}
