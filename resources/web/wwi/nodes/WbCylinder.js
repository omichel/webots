import WbGeometry from './WbGeometry.js';

export default class WbCylinder extends WbGeometry {
  constructor(id, radius, height, subdivision, bottom, side, top) {
    super(id);
    this.radius = radius;
    this.height = height;
    this.subdivision = subdivision;
    this.bottom = bottom;
    this.side = side;
    this.top = top;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbCylinder(customID, this.radius, this.height, this.subdivision, this.bottom, this.side, this.top);
  }

  createWrenObjects() {
    super.createWrenObjects();

    if (this.subdivision < 3)
      this.subdivision = 3;

    if (!this.bottom && !this.side && !this.top)
      return;

    this._computeWrenRenderable();

    this._wrenMesh = _wr_static_mesh_unit_cylinder_new(this.subdivision, this.side, this.top, this.bottom, false);

    _wr_renderable_set_mesh(this._wrenRenderable, this._wrenMesh);

    const scale = _wrjs_array3(this.radius, this.height, this.radius);
    _wr_transform_set_scale(this.wrenNode, scale);
  }

  delete() {
    _wr_static_mesh_delete(this._wrenMesh);

    super.delete();
  }
}
