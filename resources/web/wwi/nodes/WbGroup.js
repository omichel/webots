import WbBaseNode from './WbBaseNode.js';
import WbLight from './WbLight.js';
import WbWorld from './WbWorld.js';
import {getAnId} from './utils/utils.js';

export default class WbGroup extends WbBaseNode {
  constructor(id, isPropeller) {
    super(id);
    this.children = [];

    this.isPropeller = isPropeller;
    this.currentHelix = -1; // to switch between fast and slow helix
  }

  async clone(customID) {
    const group = new WbGroup(customID, this.isPropeller);
    const length = this.children.length;
    for (let i = 0; i < length; i++) {
      const cloned = await this.children[i].clone(getAnId());
      cloned.parent = customID;
      WbWorld.instance.nodes.set(cloned.id, cloned);
      group.children.push(cloned);
    }

    this.useList.push(customID);
    return group;
  }

  createWrenObjects(isTransform) {
    super.createWrenObjects();

    if (!isTransform) {
      this.children.forEach(child => {
        child.createWrenObjects();
      });
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

    let index = this.children.length - 1;
    while (index >= 0) {
      this.children[index].delete();
      --index;
    }

    super.delete();
  }
  preFinalize() {
    super.preFinalize();

    this.children.forEach(child => child.preFinalize());
  }

  postFinalize() {
    super.postFinalize();

    this.children.forEach(child => child.postFinalize());

    if (this.isPropeller === true) {
      if (typeof this.children[1] !== 'undefined')
        this.currentHelix = this.children[1].id;
      else if (typeof this.children[0] !== 'undefined')
        this.currentHelix = this.children[0].id;
      this.switchHelix(this.currentHelix, true);
    }
  }

  switchHelix(id, force) {
    if (id !== this.currentHelix || force) {
      this.currentHelix = id;
      this.children.forEach(child => {
        if (child.id === this.currentHelix)
          _wr_node_set_visible(child.wrenNode, true);
        else
          _wr_node_set_visible(child.wrenNode, false);
      });
    }
  }

  updateBoundingObjectVisibility() {
    this.children.forEach(child => {
      if (!(child instanceof WbLight))
        child.updateBoundingObjectVisibility();
    });
  }
}
