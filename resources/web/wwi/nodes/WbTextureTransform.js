import WbBaseNode from './WbBaseNode.js';
import WbWorld from './WbWorld.js';

export default class WbTextureTransform extends WbBaseNode {
  constructor(id, center, rotation, scale, translation) {
    super(id);
    this.center = center;
    this.rotation = rotation;
    this.scale = scale;
    this.translation = translation;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbTextureTransform(customID, this.center, this.rotation, this.scale, this.translation);
  }

  delete() {
    if (typeof this.parent !== 'undefined') {
      const parent = WbWorld.instance.nodes.get(this.parent);
      if (typeof parent !== 'undefined')
        parent.textureTransform = undefined;
    }

    this._destroyWrenObjects();

    super.delete();
  }

  modifyWrenMaterial(wrenMaterial) {
    this._destroyWrenObjects();

    // apply translation before rotation
    this._wrenTextureTransform = _wr_texture_transform_new();
    _wr_texture_transform_set_scale(this._wrenTextureTransform, this.scale.x, this.scale.y);
    _wr_texture_transform_set_position(this._wrenTextureTransform, this.translation.x, this.translation.y);
    _wr_texture_transform_set_center(this._wrenTextureTransform, this.center.x, this.center.y);
    _wr_texture_transform_set_rotation(this._wrenTextureTransform, this.rotation);

    _wr_material_set_texture_transform(wrenMaterial, this._wrenTextureTransform);
  }

  // Private functions

  _destroyWrenObjects() {
    if (typeof this._wrenTextureTransform !== 'undefined')
      _wr_texture_transform_delete(this._wrenTextureTransform);
  }
}
