import WbBaseNode from './WbBaseNode.js';
import WbWorld from './WbWorld.js';

export default class WbAbstractAppearance extends WbBaseNode {
  constructor(id, transform) {
    super(id);
    this.textureTransform = transform;
  }

  createWrenObjects() {
    super.createWrenObjects();
    if (typeof this.textureTransform !== 'undefined')
      this.textureTransform.createWrenObjects();
  }

  delete() {
    if (typeof this.parent !== 'undefined') {
      const parent = WbWorld.instance.nodes.get(this.parent);
      if (typeof parent !== 'undefined') {
        parent.appearance = undefined;
        parent.wrenMaterial = undefined;
        parent.updateAppearance();
      }
    }

    if (typeof this.textureTransform !== 'undefined')
      this.textureTransform.delete();

    super.delete();
  }

  preFinalize() {
    super.preFinalize();
    if (typeof this.textureTransform !== 'undefined')
      this.textureTransform.preFinalize();
  }

  postFinalize() {
    super.postFinalize();

    if (typeof this.textureTransform !== 'undefined')
      this.textureTransform.postFinalize();
  }
}
