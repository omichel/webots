import WbGroup from './WbGroup.js';
import WbWorld from './WbWorld.js';

import {getAnId} from './utils/utils.js';

// Also used to represent a solid
export default class WbTransform extends WbGroup {
  constructor(id, isSolid, translation, scale, rotation) {
    super(id);
    this.isSolid = isSolid;
    this.translation = translation;
    this.scale = scale;
    this.rotation = rotation;

    this.children = [];
  }

  applyRotationToWren() {
    const rotation = _wrjs_array4(this.rotation.w, this.rotation.x, this.rotation.y, this.rotation.z);
    _wr_transform_set_orientation(this.wrenNode, rotation);
  }

  applyScaleToWren() {
    const scale = _wrjs_array3(this.scale.x, this.scale.y, this.scale.z);
    _wr_transform_set_scale(this.wrenNode, scale);
  }

  applyTranslationToWren() {
    const translation = _wrjs_array3(this.translation.x, this.translation.y, this.translation.z);
    _wr_transform_set_position(this.wrenNode, translation);
  }

  async clone(customID) {
    const transform = new WbTransform(customID, this.isSolid, this.translation, this.scale, this.rotation);

    const length = this.children.length;
    for (let i = 0; i < length; i++) {
      const cloned = await this.children[i].clone(getAnId());
      cloned.parent = customID;
      WbWorld.instance.nodes.set(cloned.id, cloned);
      transform.children.push(cloned);
    }

    this.useList.push(customID);
    return transform;
  }

  createWrenObjects() {
    super.createWrenObjects(true);
    const transform = _wr_transform_new();

    _wr_transform_attach_child(this.wrenNode, transform);
    this.wrenNode = transform;
    this.children.forEach(child => {
      child.createWrenObjects();
    });

    if (typeof this.boundingObject !== 'undefined')
      this.boundingObject.createWrenObjects();

    this.applyTranslationToWren();
    this.applyRotationToWren();
    this.applyScaleToWren();
  }

  delete(isBoundingObject) {
    if (this.wrenObjectsCreatedCalled)
      _wr_node_delete(this.wrenNode);

    if (typeof this.boundingObject !== 'undefined')
      this.boundingObject.delete(true);

    super.delete(isBoundingObject);
  }

  preFinalize() {
    super.preFinalize();

    if (typeof this.boundingObject !== 'undefined')
      this.boundingObject.preFinalize();
  }

  postFinalize() {
    super.postFinalize();

    if (typeof this.boundingObject !== 'undefined')
      this.boundingObject.postFinalize();
  }

  updateBoundingObjectVisibility() {
    super.updateBoundingObjectVisibility();

    if (typeof this.boundingObject !== 'undefined')
      this.boundingObject.updateBoundingObjectVisibility();
  }
}
