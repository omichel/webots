import WbGeometry from './WbGeometry.js';

export default class WbCone extends WbGeometry {
  constructor(id, bottomRadius, height, subdivision, side, bottom) {
    super(id);
    this.bottomRadius = bottomRadius;
    this.height = height;
    this.subdivision = subdivision;
    this.side = side;
    this.bottom = bottom;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbCone(customID, this.bottomRadius, this.height, this.subdivision, this.side, this.bottom);
  }

  createWrenObjects() {
    super.createWrenObjects();

    if (!this.bottom && !this.side)
      return;

    this._computeWrenRenderable();

    this._wrenMesh = _wr_static_mesh_unit_cone_new(this.subdivision, this.side, this.bottom);

    _wr_renderable_set_mesh(this._wrenRenderable, this._wrenMesh);

    const scale = _wrjs_array3(this.bottomRadius, this.height, this.bottomRadius);
    _wr_transform_set_scale(this.wrenNode, scale);
  }

  delete() {
    _wr_static_mesh_delete(this._wrenMesh);

    super.delete();
  }
}
