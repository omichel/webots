import WbGeometry from './WbGeometry.js';

export default class WbPlane extends WbGeometry {
  constructor(id, size) {
    super(id);
    this.size = size;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbPlane(customID, this.size);
  }

  createWrenObjects() {
    super.createWrenObjects();

    this._computeWrenRenderable();

    const createOutlineMesh = super.isInBoundingObject();
    const wrenMesh = _wr_static_mesh_unit_rectangle_new(createOutlineMesh);

    _wr_renderable_set_mesh(this._wrenRenderable, wrenMesh);

    this.updateSize();
  }

  delete() {
    _wr_static_mesh_delete(this._wrenMesh);

    super.delete();
  }

  updateLineScale() {
    if (!this._isAValidBoundingObject())
      return;

    const offset = _wr_config_get_line_scale() / WbGeometry.LINE_SCALE_FACTOR;

    // allow the bounding sphere to scale down
    const scaleY = 0.1 * Math.min(this.size.x, this.size.y);
    wr_transform_set_scale(this.wrenNode, _wrjs_array3(this.size.x * (1.0 + offset), scaleY, this.size.y * (1.0 + offset)));
  }

  updateScale() {
    // allow the bounding sphere to scale down
    const scaleY = 0.1 * Math.min(this.size.x, this.size.y);

    const scale = _wrjs_array3(this.size.x, scaleY, this.size.y);
    _wr_transform_set_scale(this.wrenNode, scale);
  }

  updateSize() {
    if (super.isInBoundingObject())
      this.updateLineScale();
    else
      this.updateScale();
  }

  // Private functions

  _isSuitableForInsertionInBoundingObject() {
    return super._isSuitableForInsertionInBoundingObject() && !(this.size.x <= 0.0 || this.size.y <= 0.0);
  }
}
