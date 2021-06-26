import WbTransform from './nodes/WbTransform.js';
import WbWorld from './nodes/WbWorld.js';

export default class Selector {
  static select(id) {
    Selector.previousId = Selector.selectedId;
    Selector.selectedId = undefined;
    if (typeof WbWorld.instance === 'undefined')
      return;

    const node = WbWorld.instance.nodes.get('n' + id);
    if (typeof node === 'undefined') {
      Selector.preciseId = 'n' + id;
      Selector.previousAncestor = undefined;
      return;
    }

    if (Selector.previousAncestor === Selector.topSolidId(node) && (!Selector.local || Selector.preciseId !== 'n' + id)) {
      Selector.selectedId = Selector.firstSolidId(node);
      Selector.local = true;
    } else {
      Selector.selectedId = Selector.topSolidId(node);
      Selector.previousAncestor = Selector.selectedId;
      Selector.local = false;
    }

    Selector.preciseId = 'n' + id;
  }

  static checkIfParentIsSelected(node) {
    const parent = WbWorld.instance.nodes.get(node.parent);
    if (typeof parent !== 'undefined') {
      if (Selector.selectedId === parent.id)
        return true;
      else if (typeof parent.parent !== 'undefined')
        return Selector.checkIfParentIsSelected(parent);
    }

    return false;
  }

  static firstSolidId(node) {
    if (typeof node !== 'undefined') {
      if (node instanceof WbTransform && node.isSolid)
        return node.id;
      else if (typeof node.parent !== 'undefined' && typeof WbWorld.instance.nodes.get(node.parent) !== 'undefined')
        return Selector.firstSolidId(WbWorld.instance.nodes.get(node.parent));
    }
    return -1;
  }

  static topSolidId(node) {
    let topSolid;
    let currentNode = node;
    while (typeof currentNode !== 'undefined') {
      if (currentNode instanceof WbTransform && currentNode.isSolid)
        topSolid = currentNode.id;

      if (typeof currentNode.parent !== 'undefined')
        currentNode = WbWorld.instance.nodes.get(currentNode.parent);
      else
        break;
    }

    return topSolid;
  }

  static reset() {
    Selector.selectedId = undefined;
    Selector.previousId = undefined;
    Selector.previousAncestor = undefined;
    Selector.local = false;
  }
}

Selector.local = false;
