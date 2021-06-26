import WbAbstractAppearance from './WbAbstractAppearance.js';
import WbWorld from './WbWorld.js';
import WbWrenShaders from './../wren/WbWrenShaders.js';
import {getAnId} from './utils/utils.js';

export default class WbAppearance extends WbAbstractAppearance {
  constructor(id, material, texture, transform) {
    super(id, transform);
    this.material = material;
    this.texture = texture;
  }

  async clone(customID) {
    let material, texture, transform;
    if (typeof this.material !== 'undefined') {
      material = this.material.clone(getAnId());
      material.parent = customID;
      WbWorld.instance.nodes.set(material.id, material);
    }

    if (typeof this.texture !== 'undefined') {
      texture = await this.texture.clone(getAnId());
      texture.parent = customID;
      WbWorld.instance.nodes.set(texture.id, texture);
    }

    if (typeof this.textureTransform !== 'undefined') {
      transform = this.textureTransform.clone(getAnId());
      transform.parent = customID;
      WbWorld.instance.nodes.set(transform.id, transform);
    }

    this.useList.push(customID);
    return new WbAppearance(customID, material, texture, transform);
  }

  createWrenObjects() {
    super.createWrenObjects();
    if (typeof this.material !== 'undefined')
      this.material.createWrenObjects();

    if (typeof this.texture !== 'undefined')
      this.texture.createWrenObjects();
  }

  delete() {
    if (typeof this.material !== 'undefined')
      this.material.delete();

    if (typeof this.texture !== 'undefined')
      this.texture.delete();

    super.delete();
  }

  modifyWrenMaterial(wrenMaterial) {
    if (typeof this.material !== 'undefined') {
      _wr_material_set_default_program(wrenMaterial, WbWrenShaders.phongShader());
      _wr_material_set_stencil_ambient_emissive_program(wrenMaterial, WbWrenShaders.phongStencilAmbientEmissiveShader());
      _wr_material_set_stencil_diffuse_specular_program(wrenMaterial, WbWrenShaders.phongStencilDiffuseSpecularShader());

      this.material.modifyWrenMaterial(wrenMaterial, this.texture && this.texture._wrenTexture);
    } else
      wrenMaterial = WbAppearance.fillWrenDefaultMaterial(wrenMaterial);

    if (this.texture)
      this.texture.modifyWrenMaterial(wrenMaterial, 0, 2);
    else
      _wr_material_set_texture(wrenMaterial, null, 0);

    if (typeof this.textureTransform !== 'undefined')
      this.textureTransform.modifyWrenMaterial(wrenMaterial);
    else
      _wr_material_set_texture_transform(wrenMaterial, null);

    return wrenMaterial;
  }

  preFinalize() {
    super.preFinalize();

    if (typeof this.material !== 'undefined')
      this.material.preFinalize();

    if (typeof this.texture !== 'undefined')
      this.texture.preFinalize();
  }

  postFinalize() {
    super.postFinalize();

    if (typeof this.material !== 'undefined')
      this.material.postFinalize();
    if (typeof this.texture !== 'undefined')
      this.texture.postFinalize();
  }

  // Static functions

  static fillWrenDefaultMaterial(wrenMaterial) {
    if (typeof wrenMaterial === 'undefined') {
      _wr_material_delete(wrenMaterial);
      wrenMaterial = _wr_phong_material_new();
    }

    _wr_material_set_default_program(wrenMaterial, WbWrenShaders.defaultShader());

    return wrenMaterial;
  }
}
