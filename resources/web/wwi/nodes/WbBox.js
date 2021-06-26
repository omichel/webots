import WbGeometry from './WbGeometry.js';

export default class WbBox extends WbGeometry {
  constructor(id, size) {
    super(id);
    this.size = size;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbBox(customID, this.size);
  }

  createWrenObjects() {
    super.createWrenObjects();
    super._computeWrenRenderable();

    const createOutlineMesh = super.isInBoundingObject();
    this._wrenMesh = _wr_static_mesh_unit_box_new(createOutlineMesh);

    _wr_renderable_set_mesh(this._wrenRenderable, this._wrenMesh);

    this.updateSize();
  }

  delete() {
    _wr_static_mesh_delete(this._wrenMesh);

    super.delete();
  }

  updateLineScale() {
    if (!this._isAValidBoundingObject())
      return;

    const offset = Math.min(this.size.x, Math.min(this.size.y, this.size.z)) * _wr_config_get_line_scale() / WbGeometry.LINE_SCALE_FACTOR;
    _wr_transform_set_scale(this.wrenNode, _wrjs_array3(this.size.x + offset, this.size.y + offset, this.size.z + offset));
  }

  updateSize() {
    if (super.isInBoundingObject())
      this.updateLineScale();
    else
      _wr_transform_set_scale(this.wrenNode, _wrjs_array3(this.size.x, this.size.y, this.size.z));
  }
}

WbBox.IntersectedFace = {
  FRONT_FACE: 0,
  BACK_FACE: 1,
  LEFT_FACE: 2,
  RIGHT_FACE: 3,
  TOP_FACE: 4,
  BOTTOM_FACE: 5
};
