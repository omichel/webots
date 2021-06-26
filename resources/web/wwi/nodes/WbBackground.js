import {arrayXPointer, arrayXPointerFloat} from './utils/utils.js';
import WbBaseNode from './WbBaseNode.js';
import WbPBRAppearance from './WbPBRAppearance.js';
import WbVector3 from './utils/WbVector3.js';
import WbWorld from './WbWorld.js';
import WbWrenShaders from './../wren/WbWrenShaders.js';

export default class WbBackground extends WbBaseNode {
  constructor(id, skyColor, luminosity, cubeArray, irradianceCubeArray) {
    super(id);
    this.skyColor = skyColor;
    this.luminosity = luminosity;
    this._cubeArray = cubeArray;
    this._irradianceCubeArray = irradianceCubeArray;
  }

  createWrenObjects() {
    super.createWrenObjects();

    let skyboxShaderProgram = WbWrenShaders.skyboxShader();
    this._skyboxMaterial = _wr_phong_material_new();
    this._skyboxRenderable = _wr_renderable_new();
    this._skyboxMesh = _wr_static_mesh_unit_box_new(false);

    _wr_material_set_default_program(this._skyboxMaterial, skyboxShaderProgram);
    _wr_renderable_set_cast_shadows(this._skyboxRenderable, false);
    _wr_renderable_set_receive_shadows(this._skyboxRenderable, false);
    _wr_renderable_set_mesh(this._skyboxRenderable, this._skyboxMesh);
    _wr_renderable_set_material(this._skyboxRenderable, this._skyboxMaterial, null);
    _wr_renderable_set_drawing_mode(this._skyboxRenderable, Enum.WR_RENDERABLE_DRAWING_MODE_TRIANGLES);
    _wr_renderable_set_face_culling(this._skyboxRenderable, false);

    this._skyboxTransform = _wr_transform_new();
    _wr_transform_attach_child(this._skyboxTransform, this._skyboxRenderable);

    let hdrClearShaderProgram = WbWrenShaders.hdrClearShader();
    this._hdrClearMaterial = _wr_phong_material_new();
    this._hdrClearRenderable = _wr_renderable_new();
    this._hdrClearMesh = _wr_static_mesh_quad_new();

    _wr_material_set_default_program(this._hdrClearMaterial, hdrClearShaderProgram);
    _wr_renderable_set_cast_shadows(this._hdrClearRenderable, false);
    _wr_renderable_set_receive_shadows(this._hdrClearRenderable, false);
    _wr_renderable_set_mesh(this._hdrClearRenderable, this._hdrClearMesh);
    _wr_renderable_set_material(this._hdrClearRenderable, this._hdrClearMaterial, null);
    _wr_renderable_set_drawing_mode(this._hdrClearRenderable, Enum.WR_RENDERABLE_DRAWING_MODE_TRIANGLES);

    this._hdrClearTransform = _wr_transform_new();
    _wr_transform_attach_child(this._hdrClearTransform, this._hdrClearRenderable);

    this._applyColourToWren();
  }

  delete() {
    if (typeof this.parent === 'undefined') {
      const index = WbWorld.instance.sceneTree.indexOf(this);
      WbWorld.instance.sceneTree.splice(index, 1);
    }

    this._destroySkyBox();

    this.skyColor = new WbVector3(0, 0, 0);
    this._applyColourToWren();

    _wr_scene_set_hdr_clear_quad(_wr_scene_get_instance(), null);
    // Delete skybox
    // Shader program is not deleted, a singleton instance is kept in WbWrenShaders
    _wr_node_delete(this._skyboxRenderable);

    if (typeof this._skyboxMaterial !== 'undefined')
      _wr_material_delete(this._skyboxMaterial);

    _wr_node_delete(this._skyboxTransform);
    _wr_static_mesh_delete(this._skyboxMesh);

    _wr_node_delete(this._hdrClearRenderable);
    this._hdrClearRenderable = null;
    _wr_scene_set_hdr_clear_quad(_wr_scene_get_instance(), this._hdrClearRenderable);

    if (typeof this._hdrClearMaterial !== 'undefined')
      _wr_material_delete(this._hdrClearMaterial);

    _wr_node_delete(this._hdrClearTransform);
    _wr_static_mesh_delete(this._hdrClearMesh);

    WbBackground.instance = undefined;

    this._updatePBRs();

    super.delete();
  }

  postFinalize() {
    super.postFinalize();

    this._applySkyBoxToWren();
    this._updatePBRs();
  }

  // Private functions
  _applyColourToWren() {
    const colorPointer = _wrjs_array3(this.skyColor.x, this.skyColor.y, this.skyColor.z);

    _wr_viewport_set_clear_color_rgb(_wr_scene_get_viewport(_wr_scene_get_instance()), colorPointer);
    if (typeof this.wrenObjectsCreatedCalled !== 'undefined') {
      // use wren's set_diffuse to transform to linear color space
      _wr_phong_material_set_diffuse(this._hdrClearMaterial, colorPointer);

      // de-gamma correct
      const hdrColor = [Math.pow(this.skyColor.x, 2.2), Math.pow(this.skyColor.y, 2.2), Math.pow(this.skyColor.z, 2.2)];
      // reverse tone map
      const exposure = WbWorld.instance.viewpoint.exposure;
      for (let i = 0; i < 3; ++i)
        hdrColor[i] = -Math.log(1.000000001 - hdrColor[i]) / exposure;

      const hdrColorPointer = _wrjs_array3(hdrColor[0], hdrColor[1], hdrColor[2]);
      _wr_phong_material_set_linear_diffuse(this._hdrClearMaterial, hdrColorPointer);
      _wr_scene_set_hdr_clear_quad(_wr_scene_get_instance(), this._hdrClearRenderable);
    }
  }

  _applySkyBoxToWren() {
    this._destroySkyBox();

    // 1. Load the background.
    if (typeof this._cubeArray !== 'undefined' && this._cubeArray.length === 6) {
      this._cubeMapTexture = _wr_texture_cubemap_new();
      _wr_texture_set_internal_format(this._cubeMapTexture, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGBA8);

      const bitsPointers = [];
      for (let i = 0; i < 6; ++i) {
        bitsPointers[i] = arrayXPointer(this._cubeArray[i].bits);
        _wr_texture_cubemap_set_data(this._cubeMapTexture, bitsPointers[i], i);
      }

      _wr_texture_set_size(this._cubeMapTexture, this._cubeArray[0].width, this._cubeArray[0].height);
      _wr_texture_setup(this._cubeMapTexture);
      _wr_material_set_texture_cubemap(this._skyboxMaterial, this._cubeMapTexture, 0);
      _wr_material_set_texture_cubemap_wrap_r(this._skyboxMaterial, Enum.WR_TEXTURE_WRAP_MODE_CLAMP_TO_EDGE, 0);
      _wr_material_set_texture_cubemap_wrap_s(this._skyboxMaterial, Enum.WR_TEXTURE_WRAP_MODE_CLAMP_TO_EDGE, 0);
      _wr_material_set_texture_cubemap_wrap_t(this._skyboxMaterial, Enum.WR_TEXTURE_WRAP_MODE_CLAMP_TO_EDGE, 0);
      _wr_scene_set_skybox(_wr_scene_get_instance(), this._skyboxRenderable);

      for (let i = 0; i < 6; ++i)
        _free(bitsPointers[i]);
    }

    // 2. Load the irradiance map.
    const cubeMap = _wr_texture_cubemap_new();
    const hdrImageData = [];
    if (typeof this._cubeArray !== 'undefined' && this._irradianceCubeArray.length === 6) {
      _wr_texture_set_internal_format(cubeMap, Enum.WR_TEXTURE_INTERNAL_FORMAT_RGB32F);

      for (let i = 0; i < 6; ++i) {
        hdrImageData[i] = arrayXPointerFloat(this._irradianceCubeArray[i].bits);
        _wr_texture_cubemap_set_data(cubeMap, hdrImageData[i], i);
      }

      _wr_texture_set_size(cubeMap, this._irradianceCubeArray[0].width, this._irradianceCubeArray[0].height);
      _wr_texture_set_texture_unit(cubeMap, 13);
      _wr_texture_setup(cubeMap);

      this.irradianceCubeTexture = _wr_texture_cubemap_bake_specular_irradiance(cubeMap, WbWrenShaders.iblSpecularIrradianceBakingShader(), this._irradianceCubeArray[0].width);
      _wr_texture_cubemap_disable_automatic_mip_map_generation(this.irradianceCubeTexture);
    } else {
      if (typeof this.irradianceCubeTexture !== 'undefined') {
        _wr_texture_delete(this.irradianceCubeTexture);
        this.irradianceCubeTexture = null;
      }
      // Fallback: a cubemap is found but no irradiance map: bake a small irradiance map to have right colors.
      // Reflections won't be good in such case.
      if (typeof this._cubeMapTexture !== 'undefined') {
        this.irradianceCubeTexture = _wr_texture_cubemap_bake_specular_irradiance(this._cubeMapTexture, WbWrenShaders.iblSpecularIrradianceBakingShader(), 64);
        _wr_texture_cubemap_disable_automatic_mip_map_generation(this.irradianceCubeTexture);
      }
    }

    _wr_texture_delete(cubeMap);

    for (let i = 0; i < hdrImageData.length; ++i)
      _free(hdrImageData[i]);
  }

  _destroySkyBox() {
    _wr_scene_set_skybox(_wr_scene_get_instance(), null);

    if (typeof this._skyboxMaterial !== 'undefined')
      _wr_material_set_texture_cubemap(this._skyboxMaterial, null, 0);

    if (typeof this._cubeMapTexture !== 'undefined') {
      _wr_texture_delete(this._cubeMapTexture);
      this._cubeMapTexture = undefined;
    }

    if (typeof this.irradianceCubeTexture !== 'undefined') {
      _wr_texture_delete(this.irradianceCubeTexture);
      this.irradianceCubeTexture = undefined;
    }
  }

  _updatePBRs() {
    WbWorld.instance.nodes.forEach((value, key, map) => {
      if (value instanceof WbPBRAppearance && typeof value.parent !== 'undefined') {
        const parent = WbWorld.instance.nodes.get(value.parent);
        if (typeof parent !== 'undefined')
          parent.applyMaterialToGeometry();
      }
    });
  }
}
