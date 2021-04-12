import {WbGeometry} from './wbGeometry.js';

class WbSphere extends WbGeometry {
  constructor(id, radius, ico, subdivision) {
    super(id);
    this.radius = radius;
    this.ico = ico;
    this.subdivision = subdivision;
  }

  delete() {
    _wr_static_mesh_delete(this.wrenMesh);

    super.delete();
  }

  createWrenObjects() {
    super.createWrenObjects();
    this.buildWrenMesh();
  }

  buildWrenMesh() {
    super.deleteWrenRenderable();

    if (typeof this.wrenMesh !== 'undefined') {
      _wr_static_mesh_delete(this.wrenMesh);
      this.wrenMesh = undefined;
    }

    super.computeWrenRenderable();

    const createOutlineMesh = super.isInBoundingObject();
    this.wrenMesh = _wr_static_mesh_unit_sphere_new(this.subdivision, this.ico, false);

    // Restore pickable state
    super.setPickable(this.isPickable);

    _wr_renderable_set_mesh(this.wrenRenderable, this.wrenMesh);

    if (createOutlineMesh)
      this.updateLineScale();
    else
      this.updateScale();
  }

  updateLineScale() {
    if (!this.isAValidBoundingObject())
      return;

    const offset = _wr_config_get_line_scale() / WbGeometry.LINE_SCALE_FACTOR;
    const scaledRadius = this.radius * (1.0 + offset);
    _wr_transform_set_scale(this.wrenNode, _wrjs_array3(scaledRadius, scaledRadius, scaledRadius));
  }

  updateScale() {
    const scaledRadius = this.radius;

    _wr_transform_set_scale(this.wrenNode, _wrjs_array3(scaledRadius, scaledRadius, scaledRadius));
  }

  isAValidBoundingObject() {
    return super.isAValidBoundingObject() && this.radius > 0;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbSphere(customID, this.radius, this.ico, this.subdivision);
  }
}

export {WbSphere};
