import {arrayXPointerFloat} from './utils/utils.js';
import WbGeometry from './WbGeometry.js';

export default class WbElevationGrid extends WbGeometry {
  constructor(id, height, xDimension, xSpacing, zDimension, zSpacing, thickness) {
    super(id);
    this.height = height;
    this.xDimension = xDimension;
    this.xSpacing = xSpacing;
    this.zDimension = zDimension;
    this.zSpacing = zSpacing;
    this.thickness = thickness;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbElevationGrid(customID, this.height, this.xDimension, this.xSpacing, this.zDimension, this.zSpacing, this.thickness);
  }

  createWrenObjects() {
    super.createWrenObjects();
    this._buildWrenMesh();
  }

  delete() {
    _wr_static_mesh_delete(this._wrenMesh);

    super.delete();
  }

  updateLineScale() {
    if (this._isAValidBoundingObject())
      return;

    const offset = _wr_config_get_line_scale() / WbGeometry.LINE_SCALE_FACTOR;

    const scalePointer = _wrjs_array3(this.xSpacing, 1.0 + offset, this.zSpacing);

    _wr_transform_set_scale(this.wrenNode, scalePointer);
  }

  updateScale() {
    const scalePointer = _wrjs_array3(this.xSpacing, 1.0, this.zSpacing);
    _wr_transform_set_scale(this.wrenNode, scalePointer);
  }

  // Private functions

  _buildWrenMesh() {
    super._deleteWrenRenderable();

    if (typeof this._wrenMesh !== 'undefined') {
      _wr_static_mesh_delete(this._wrenMesh);
      this._wrenMesh = undefined;
    }

    if (this.xDimension < 2 || this.zDimension < 2)
      return;

    if (this.xSpacing === 0.0 || this.zSpacing === 0.0)
      return;

    super._computeWrenRenderable();

    // Restore pickable state
    super.setPickable(this.pickable);

    // convert height values to float, pad with zeroes if necessary
    const numValues = this.xDimension * this.zDimension;
    const heightData = [];

    const availableValues = Math.min(numValues, this.height.length);
    for (let i = 0; i < availableValues; ++i)
      heightData[i] = this.height[i];

    const createOutlineMesh = super.isInBoundingObject();

    const heightDataPointer = arrayXPointerFloat(heightData);
    this._wrenMesh = _wr_static_mesh_unit_elevation_grid_new(this.xDimension, this.zDimension, heightDataPointer, this.thickness, createOutlineMesh);

    _free(heightDataPointer);

    if (createOutlineMesh)
      this.updateLineScale();
    else
      this.updateScale();

    _wr_renderable_set_mesh(this._wrenRenderable, this._wrenMesh);
  }

  _isAValidBoundingObject() {
    return this._isSuitableForInsertionInBoundingObject() && super._isAValidBoundingObject();
  }

  _isSuitableForInsertionInBoundingObject() {
    const invalidDimensions = this.xDimension < 2 || this.zDimension < 2;
    const invalidSpacings = this.xSpacing <= 0.0 || this.zSpacing < 0.0;
    const invalid = invalidDimensions || invalidSpacings;

    return !invalid;
  }
}
