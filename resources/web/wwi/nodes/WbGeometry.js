import WbBaseNode from './WbBaseNode.js';
import WbWorld from './WbWorld.js';
import {isDescendantOfBillboard} from './utils/utils.js';
import WbWrenMeshBuffers from './utils/WbWrenMeshBuffers.js';
import WbWrenPicker from './../wren/WbWrenPicker.js';
import WbWrenRenderingContext from './../wren/WbWrenRenderingContext.js';
import Selector from './../Selector.js';

export default class WbGeometry extends WbBaseNode {
  constructor(id) {
    super(id);

    this.pickable = false;
    this._isShadedGeometryPickable = true;
  }

  computeCastShadows(enabled) {
    if (typeof this._wrenRenderable === 'undefined')
      return;

    if (super.isInBoundingObject() || isDescendantOfBillboard(this)) {
      _wr_renderable_set_cast_shadows(this._wrenRenderable, false);
      _wr_renderable_set_receive_shadows(this._wrenRenderable, false);
    } else
      _wr_renderable_set_cast_shadows(this._wrenRenderable, enabled);
  }

  delete() {
    if (typeof this.parent !== 'undefined') {
      const parent = WbWorld.instance.nodes.get(this.parent);
      if (typeof parent !== 'undefined')
        parent.geometry = undefined;
    }

    if (this.wrenObjectsCreatedCalled)
      this._deleteWrenRenderable();

    super.delete();
  }

  setPickable(pickable) {
    if (typeof this._wrenRenderable === 'undefined'|| super.isInBoundingObject())
      return;

    this.pickable = pickable && this._isShadedGeometryPickable;

    WbWrenPicker.setPickable(this._wrenRenderable, this.id, pickable);
  }

  setWrenMaterial(material, castShadows) {
    if (typeof this._wrenRenderable !== 'undefined') {
      _wr_renderable_set_material(this._wrenRenderable, material, null);
      this.computeCastShadows(castShadows);
    }
  }

  updateBoundingObjectVisibility() {
    this._applyVisibilityFlagToWren(this._isSelected());
  }

  // Private functions

  _applyVisibilityFlagToWren(selected) {
    if (typeof this._wrenScaleTransform === 'undefined')
      return;

    if (super.isInBoundingObject()) {
      if (selected) {
        _wr_renderable_set_visibility_flags(this._wrenRenderable, WbWrenRenderingContext.VF_INVISIBLE_FROM_CAMERA);
        _wr_node_set_visible(this._wrenScaleTransform, true);
      } else if (_wr_node_get_parent(this._wrenScaleTransform))
        _wr_node_set_visible(this._wrenScaleTransform, false);
    } else if (isDescendantOfBillboard(this)) {
      _wr_renderable_set_visibility_flags(this._wrenRenderable, WbWrenRenderingContext.VF_INVISIBLE_FROM_CAMERA);
      _wr_node_set_visible(this._wrenScaleTransform, true);
    } else {
      _wr_renderable_set_visibility_flags(this._wrenRenderable, WbWrenRenderingContext.VM_REGULAR);
      _wr_node_set_visible(this._wrenScaleTransform, true);
    }
  }

  _computeWrenRenderable() {
    if (!this.wrenObjectsCreatedCalled)
      super.createWrenObjects();

    this._wrenScaleTransform = _wr_transform_new();
    _wr_transform_attach_child(this.wrenNode, this._wrenScaleTransform);
    this.wrenNode = this._wrenScaleTransform;

    this._wrenRenderable = _wr_renderable_new();
    if (super.isInBoundingObject()) {
      _wr_renderable_set_cast_shadows(this._wrenRenderable, false);
      _wr_renderable_set_receive_shadows(this._wrenRenderable, false);
      _wr_renderable_set_drawing_mode(this._wrenRenderable, Enum.WR_RENDERABLE_DRAWING_MODE_LINES);
    } else if (this.isMarker) {
      _wr_renderable_set_drawing_order(this._wrenRenderable, Enum.WR_RENDERABLE_DRAWING_ORDER_AFTER_1);
      _wr_renderable_set_receive_shadows(this._wrenRenderable, false);
    }

    _wr_transform_attach_child(this._wrenScaleTransform, this._wrenRenderable);

    this.updateBoundingObjectVisibility();

    this.computeCastShadows(true);
  }

  _createMeshBuffers(verticesCount, indicesCount) {
    if (verticesCount <= 0 || indicesCount <= 0)
      return undefined;

    return new WbWrenMeshBuffers(verticesCount, indicesCount, super.isInBoundingObject() ? 0 : 2, 0);
  }

  _deleteWrenRenderable() {
    if (typeof this._wrenRenderable !== 'undefined') {
      // Delete picking material
      _wr_material_delete(Module.ccall('wr_renderable_get_material', 'number', ['number', 'string'], [this._wrenRenderable, 'picking']));
      _wr_material_delete(Module.ccall('wr_renderable_get_material', 'number', ['number', 'string'], [this._wrenRenderable, 'depth']));

      _wr_node_delete(this._wrenRenderable);
      this._wrenRenderable = undefined;

      this.wrenNode = _wr_node_get_parent(this._wrenScaleTransform);
      _wr_node_delete(this._wrenScaleTransform);
      this._wrenScaleTransform = undefined;
    }
  }

  _isAValidBoundingObject() {
    if (!super.isInBoundingObject())
      return false;

    const upperTransform = super.upperTransform();
    if (typeof upperTransform !== 'undefined' && upperTransform.isInBoundingObject() && upperTransform.geometry !== this)
      return false;

    return true;
  }

  _isSelected() {
    if (Selector.selectedId === this.id)
      return true;
    else if (typeof this.parent !== 'undefined')
      return Selector.checkIfParentIsSelected(this);

    return false;
  }
}

WbGeometry.LINE_SCALE_FACTOR = 250.0;
