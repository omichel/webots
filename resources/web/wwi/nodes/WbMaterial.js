import {array3Pointer} from './utils/utils.js';
import WbBaseNode from './WbBaseNode.js';
import WbVector3 from './utils/WbVector3.js';
import WbWorld from './WbWorld.js';

export default class WbMaterial extends WbBaseNode {
  constructor(id, ambientIntensity, diffuseColor, specularColor, emissiveColor, shininess, transparency) {
    super(id);
    this.ambientIntensity = ambientIntensity;
    this.diffuseColor = diffuseColor;
    this.specularColor = specularColor;
    this.emissiveColor = emissiveColor;
    this.shininess = shininess;
    this.transparency = transparency;
  }

  clone(customID) {
    this.useList.push(customID);
    return new WbMaterial(customID, this.ambientIntensity, this.diffuseColor, this.specularColor, this.emissiveColor, this.shininess, this.transparency);
  }

  delete() {
    const parent = WbWorld.instance.nodes.get(this.parent);

    if (typeof parent !== 'undefined') {
      const shape = WbWorld.instance.nodes.get(parent.parent);
      if (typeof shape !== 'undefined') {
        parent.material = undefined;
        shape.updateAppearance();
      }
    }
    super.delete();
  }

  modifyWrenMaterial(wrenMaterial, textured) {
    let ambient, diffuse, specular, shininess;

    if (textured) {
      ambient = new WbVector3(this.ambientIntensity, this.ambientIntensity, this.ambientIntensity);
      diffuse = new WbVector3(1.0, 1.0, 1.0);
      specular = new WbVector3(1.0, 1.0, 1.0);
      shininess = 0.0;
    } else {
      ambient = new WbVector3(this.ambientIntensity * this.diffuseColor.x, this.ambientIntensity * this.diffuseColor.y,
        this.ambientIntensity * this.diffuseColor.z);
      diffuse = new WbVector3(this.diffuseColor.x, this.diffuseColor.y, this.diffuseColor.z);
      specular = new WbVector3(this.specularColor.x, this.specularColor.y, this.specularColor.z);
      shininess = this.shininess;
    }

    const ambientColorPointer = array3Pointer(ambient.x, ambient.y, ambient.z);
    const diffuseColorPointer = array3Pointer(diffuse.x, diffuse.y, diffuse.z);
    const specularColorPointer = array3Pointer(specular.x, specular.y, specular.z);
    const emissiveColorPointer = array3Pointer(this.emissiveColor.x, this.emissiveColor.y, this.emissiveColor.z);

    _wr_phong_material_set_all_parameters(wrenMaterial, ambientColorPointer, diffuseColorPointer, specularColorPointer, emissiveColorPointer, shininess, this.transparency);

    _free(ambientColorPointer);
    _free(diffuseColorPointer);
    _free(specularColorPointer);
    _free(emissiveColorPointer);
  }
}
