import {arrayXPointerFloat} from './utils/utils.js';
import WbGeometry from './WbGeometry.js';
import WbWrenShaders from './../wren/WbWrenShaders.js';

export default class WbPointSet extends WbGeometry {
  constructor(id, coord, color) {
    super(id);
    this.coord = coord;
    this.color = color;

    this._isShadedGeometryPickable = false;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbPointSet(customID, this.coord, this.color);
  }

  createWrenObjects() {
    super.createWrenObjects();
    _wr_config_enable_point_size(true);
    this._updateCoord();
    this._buildWrenMesh();
  }

  delete() {
    _wr_static_mesh_delete(this._wrenMesh);

    super.delete();
  }

  setWrenMaterial(material, castShadows) {
    super.setWrenMaterial(material, castShadows);

    if (typeof material !== 'undefined') {
      _wr_material_set_default_program(material, WbWrenShaders.pointSetShader());
      if (typeof this.color !== 'undefined')
        _wr_phong_material_set_color_per_vertex(material, true);
      else
        _wr_phong_material_set_color_per_vertex(material, false);
    }
  }

  // Private functions

  _buildWrenMesh() {
    super._deleteWrenRenderable();

    if (typeof this._wrenMesh !== 'undefined') {
      _wr_static_mesh_delete(this._wrenMesh);
      this._wrenMesh = undefined;
    }

    if (typeof this.coord === 'undefined' || this.coord.length === 0)
      return;

    super._computeWrenRenderable();

    const coordsData = [];
    let colorData;
    if (typeof this.color !== 'undefined')
      colorData = [];

    const coordsCount = this._computeCoordsAndColorData(coordsData, colorData);

    const coordsDataPointer = arrayXPointerFloat(coordsData);
    const colorDataPointer = arrayXPointerFloat(colorData);
    this._wrenMesh = _wr_static_mesh_point_set_new(coordsCount, coordsDataPointer, colorDataPointer);

    _free(coordsDataPointer);
    _free(colorDataPointer);

    _wr_renderable_set_cast_shadows(this._wrenRenderable, false);
    _wr_renderable_set_receive_shadows(this._wrenRenderable, false);
    _wr_renderable_set_drawing_mode(this._wrenRenderable, Enum.WR_RENDERABLE_DRAWING_MODE_POINTS);
    _wr_renderable_set_point_size(this._wrenRenderable, 4.0);
    _wr_renderable_set_mesh(this._wrenRenderable, this._wrenMesh);
  }

  _computeCoordsAndColorData(coordsData, colorData) {
    if (typeof this.coord === 'undefined')
      return 0;

    let count = 0;
    if (typeof colorData !== 'undefined') {
      const size = Math.min(this.coord.length, this.color.length);
      for (let i = 0; i < size; i++) {
        coordsData[3 * count] = this.coord[i].x;
        coordsData[3 * count + 1] = this.coord[i].y;
        coordsData[3 * count + 2] = this.coord[i].z;
        colorData[3 * count] = this.color[i].x;
        colorData[3 * count + 1] = this.color[i].y;
        colorData[3 * count + 2] = this.color[i].z;
        count++;
      }
    } else {
      for (let i = 0; i < this.coord.length; i++) {
        coordsData[3 * count] = this.coord[i].x;
        coordsData[3 * count + 1] = this.coord[i].y;
        coordsData[3 * count + 2] = this.coord[i].z;
        count++;
      }
    }
    return count;
  }

  _updateCoord() {
    if (this.wrenObjectsCreatedCalled)
      this._buildWrenMesh();
  }
}
