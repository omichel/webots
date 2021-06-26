import WbAppearance from './WbAppearance.js';
import WbBaseNode from './WbBaseNode.js';
import WbPBRAppearance from './WbPBRAppearance.js';
import WbPointSet from './WbPointSet.js';
import WbWorld from './WbWorld.js';
import WbWrenShaders from './../wren/WbWrenShaders.js';
import {getAnId} from './utils/utils.js';

export default class WbShape extends WbBaseNode {
  constructor(id, castShadow, isPickable, geometry, appearance) {
    super(id);
    this.castShadow = castShadow;
    this.isPickable = isPickable;

    this.appearance = appearance;
    this.geometry = geometry;
  }

  applyMaterialToGeometry() {
    if (!this.wrenMaterial)
      this._createWrenMaterial(Enum.WR_MATERIAL_PHONG);
    if (this.geometry) {
      if (this.appearance instanceof WbAppearance) {
        if (this.appearance.wrenObjectsCreatedCalled)
          this.wrenMaterial = this.appearance.modifyWrenMaterial(this.wrenMaterial);
        else
          this.wrenMaterial = WbAppearance.fillWrenDefaultMaterial(this.wrenMaterial);
      } else if ((this.appearance instanceof WbPBRAppearance) && !(this.geometry instanceof WbPointSet)) {
        this._createWrenMaterial();
        if (this.appearance.wrenObjectsCreatedCalled)
          this.wrenMaterial = this.appearance.modifyWrenMaterial(this.wrenMaterial);
      } else
        this.wrenMaterial = WbAppearance.fillWrenDefaultMaterial(this.wrenMaterial);
      this.geometry.setWrenMaterial(this.wrenMaterial, this.castShadow);
    }
  }

  async clone(customID) {
    let geometry, appearance;
    if (typeof this.geometry !== 'undefined') {
      geometry = this.geometry.clone(getAnId());
      geometry.parent = customID;
      WbWorld.instance.nodes.set(geometry.id, geometry);
    }

    if (typeof this.appearance !== 'undefined') {
      appearance = await this.appearance.clone(getAnId());
      appearance.parent = customID;
      WbWorld.instance.nodes.set(appearance.id, appearance);
    }

    this.useList.push(customID);
    return new WbShape(customID, this.castShadow, this.isPickable, geometry, appearance);
  }

  createWrenObjects() {
    super.createWrenObjects();
    if (typeof this.appearance !== 'undefined')
      this.appearance.createWrenObjects();

    if (typeof this.geometry !== 'undefined') {
      this.geometry.createWrenObjects();

      this.applyMaterialToGeometry();
    }
  }

  delete(isBoundingObject) {
    if (typeof this.parent === 'undefined') {
      const index = WbWorld.instance.sceneTree.indexOf(this);
      WbWorld.instance.sceneTree.splice(index, 1);
    } else {
      const parent = WbWorld.instance.nodes.get(this.parent);
      if (typeof parent !== 'undefined') {
        if (isBoundingObject)
          parent.isBoundingObject = null;
        else {
          const index = parent.children.indexOf(this);
          parent.children.splice(index, 1);
        }
      }
    }

    if (typeof this.wrenMaterial !== 'undefined') {
      _wr_material_delete(this.wrenMaterial);
      this.wrenMaterial = undefined;
    }

    if (typeof this.appearance !== 'undefined')
      this.appearance.delete();

    if (typeof this.geometry !== 'undefined')
      this.geometry.delete();

    super.delete();
  }

  updateAppearance() {
    if (this.wrenObjectsCreatedCalled)
      this.applyMaterialToGeometry();
  }

  updateBoundingObjectVisibility() {
    if (typeof this.geometry !== 'undefined')
      this.geometry.updateBoundingObjectVisibility();
  }

  updateCastShadows() {
    if (super.isInBoundingObject())
      return;

    if (typeof this.geometry !== 'undefined')
      this.geometry.computeCastShadows(this.castShadow);
  }

  updateIsPickable() {
    if (super.isInBoundingObject())
      return;

    if (typeof this.geometry !== 'undefined')
      this.geometry.setPickable(this.isPickable);
  }

  preFinalize() {
    super.preFinalize();

    if (typeof this.appearance !== 'undefined')
      this.appearance.preFinalize();

    if (typeof this.geometry !== 'undefined')
      this.geometry.preFinalize();

    this.updateAppearance();
  }

  postFinalize() {
    super.postFinalize();

    if (typeof this.appearance !== 'undefined')
      this.appearance.postFinalize();

    if (typeof this.geometry !== 'undefined')
      this.geometry.postFinalize();

    if (!super.isInBoundingObject()) {
      this.updateCastShadows();
      this.updateIsPickable();
    }
  }

  // Private functions

  _createWrenMaterial(type) {
    const defaultColor = _wrjs_array3(1.0, 1.0, 1.0);
    if (typeof this.wrenMaterial !== 'undefined')
      _wr_material_delete(this.wrenMaterial);

    if (type === Enum.WR_MATERIAL_PHONG) {
      this.wrenMaterial = _wr_phong_material_new();
      _wr_phong_material_set_color(this.wrenMaterial, defaultColor);
      _wr_material_set_default_program(this.wrenMaterial, WbWrenShaders.phongShader());
    } else {
      this.wrenMaterial = _wr_pbr_material_new();
      _wr_pbr_material_set_base_color(this.wrenMaterial, defaultColor);
      _wr_material_set_default_program(this.wrenMaterial, WbWrenShaders.pbrShader());
    }
  }
}
