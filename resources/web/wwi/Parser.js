import {M_PI_4} from './nodes/utils/constants.js';
import WbAbstractAppearance from './nodes/WbAbstractAppearance.js';
import WbAppearance from './nodes/WbAppearance.js';
import WbBackground from './nodes/WbBackground.js';
import WbBillboard from './nodes/WbBillboard.js';
import WbBox from './nodes/WbBox.js';
import WbCapsule from './nodes/WbCapsule.js';
import WbCone from './nodes/WbCone.js';
import WbCylinder from './nodes/WbCylinder.js';
import WbDirectionalLight from './nodes/WbDirectionalLight.js';
import WbElevationGrid from './nodes/WbElevationGrid.js';
import WbFog from './nodes/WbFog.js';
import WbGeometry from './nodes/WbGeometry.js';
import WbGroup from './nodes/WbGroup.js';
import WbImage from './nodes/WbImage.js';
import WbImageTexture from './nodes/WbImageTexture.js';
import WbIndexedFaceSet from './nodes/WbIndexedFaceSet.js';
import WbIndexedLineSet from './nodes/WbIndexedLineSet.js';
import WbLight from './nodes/WbLight.js';
import WbMaterial from './nodes/WbMaterial.js';
import WbPBRAppearance from './nodes/WbPBRAppearance.js';
import WbPlane from './nodes/WbPlane.js';
import WbPointLight from './nodes/WbPointLight.js';
import WbPointSet from './nodes/WbPointSet.js';
import WbScene from './nodes/WbScene.js';
import WbShape from './nodes/WbShape.js';
import WbSphere from './nodes/WbSphere.js';
import WbSpotLight from './nodes/WbSpotLight.js';
import WbTextureTransform from './nodes/WbTextureTransform.js';
import WbTransform from './nodes/WbTransform.js';
import WbVector2 from './nodes/utils/WbVector2.js';
import WbVector3 from './nodes/utils/WbVector3.js';
import WbVector4 from './nodes/utils/WbVector4.js';
import WbViewpoint from './nodes/WbViewpoint.js';
import WbWorld from './nodes/WbWorld.js';
import {getAnId} from './nodes/utils/utils.js';

import DefaultUrl from './DefaultUrl.js';
import loadHdr from './hdr_loader.js';
import {webots} from './webots.js';

/*
  This module takes an x3d world, parse it and populate the scene.
*/
export default class Parser {
  constructor(prefix = '') {
    this._prefix = prefix;
    WbWorld.init();
  }

  async parse(text, renderer, parent, callback) {
    let xml = null;
    if (window.DOMParser) {
      const parser = new DOMParser();
      xml = parser.parseFromString(text, 'text/xml');
    }

    if (typeof xml === 'undefined')
      console.error('File to parse not found');
    else {
      const scene = xml.getElementsByTagName('Scene')[0];
      if (typeof scene === 'undefined') {
        const node = xml.getElementsByTagName('nodes')[0];
        if (typeof node === 'undefined')
          console.error('Unknown content, nor Scene, nor Node');
        else
          await this._parseChildren(node, parent);
      } else
        await this._parseNode(scene);
    }

    if (document.getElementById('webotsProgressMessage'))
      document.getElementById('webotsProgressMessage').innerHTML = 'Finalizing...';

    if (typeof WbWorld.instance.viewpoint === 'undefined')
      return;

    WbWorld.instance.viewpoint.finalize();

    WbWorld.instance.sceneTree.forEach(node => {
      node.finalize();
    });

    WbWorld.instance.readyForUpdates = true;

    webots.currentView.x3dScene.resize();
    renderer.render();
    if (document.getElementById('webotsProgress'))
      document.getElementById('webotsProgress').style.display = 'none';
    if (webots.currentView.toolBar) {
      webots.currentView.toolBar.enableToolBarButtons(true);
      if (webots.currentView.runOnLoad === 'real-time')
        webots.currentView.toolBar.realTime();
      else if (webots.currentView.runOnLoad === 'run' || webots.currentView.runOnLoad === 'fast')
        webots.currentView.toolBar.run();
    }

    if (typeof callback === 'function')
      callback();
  }

  async _parseNode(node, parentNode, isBoundingObject) {
    if (typeof WbWorld.instance === 'undefined')
      WbWorld.init();

    let result;
    if (node.tagName === 'Scene') {
      WbWorld.instance.scene = await this._parseScene(node);
      await this._parseChildren(node, parentNode);
    } else if (node.tagName === 'WorldInfo')
      this._parseWorldInfo(node);
    else if (node.tagName === 'Viewpoint')
      WbWorld.instance.viewpoint = this._parseViewpoint(node);
    else if (node.tagName === 'Background')
      result = await this._parseBackground(node);
    else if (node.tagName === 'Transform')
      result = await this._parseTransform(node, parentNode, isBoundingObject);
    else if (node.tagName === 'Billboard')
      result = await this._parseBillboard(node, parentNode);
    else if (node.tagName === 'Group')
      result = await this._parseGroup(node, parentNode, isBoundingObject);
    else if (node.tagName === 'Shape')
      result = await this._parseShape(node, parentNode, isBoundingObject);
    else if (node.tagName === 'Switch')
      result = await this._parseSwitch(node, parentNode);
    else if (node.tagName === 'DirectionalLight')
      result = await this._parseDirectionalLight(node, parentNode);
    else if (node.tagName === 'PointLight')
      result = await this._parsePointLight(node, parentNode);
    else if (node.tagName === 'SpotLight')
      result = await this._parseSpotLight(node, parentNode);
    else if (node.tagName === 'Fog') {
      if (!WbWorld.instance.hasFog)
        result = await this._parseFog(node);
      else
        console.error('This world already has a fog.');
    } else {
      // Either it is a node added after the whole scene, or it is an unknown node
      let id;
      if (typeof parentNode !== 'undefined')
        id = parentNode.id;
      result = await this._parseGeometry(node, id);

      // We are forced to check if the result correspond to the class we expect because of the case of a USE
      if (typeof result !== 'undefined' && result instanceof WbGeometry) {
        if (typeof parentNode !== 'undefined' && parentNode instanceof WbShape) {
          if (typeof parentNode.geometry !== 'undefined')
            parentNode.geometry.delete();
          parentNode.geometry = result;
        }
      } else if (node.tagName === 'PBRAppearance') {
        if (typeof parentNode !== 'undefined' && parentNode instanceof WbShape) {
          if (typeof parentNode.appearance !== 'undefined')
            parentNode.appearance.delete();
          result = await this._parsePBRAppearance(node, id);
          parentNode.appearance = result;
        }
      } else if (node.tagName === 'Appearance') {
        if (typeof parentNode !== 'undefined' && parentNode instanceof WbShape) {
          if (typeof parentNode.appearance !== 'undefined')
            parentNode.appearance.delete();
          result = await this._parseAppearance(node, id);
          parentNode.appearance = result;
        }
      } else if (node.tagName === 'Material') {
        result = await this._parseMaterial(node, id);
        if (typeof result !== 'undefined') {
          if (typeof parentNode !== 'undefined' && parentNode instanceof WbAppearance) {
            if (typeof parentNode.material !== 'undefined')
              parentNode.material.delete();
            parentNode.material = result;
          }
        }
      } else if (node.tagName === 'ImageTexture') {
        result = await this._parseImageTexture(node, id);
        if (typeof result !== 'undefined') {
          if (typeof parentNode !== 'undefined' && parentNode instanceof WbAppearance) {
            if (typeof parentNode.material !== 'undefined')
              parentNode.texture.delete();
            parentNode.texture = result;
          }
        }
      } else if (node.tagName === 'TextureTransform') {
        result = await this._parseTextureTransform(node, id);
        if (typeof result !== 'undefined') {
          if (typeof parentNode !== 'undefined' && parentNode instanceof WbAbstractAppearance) {
            if (typeof parentNode.textureTransform !== 'undefined')
              parentNode.textureTransform.delete();
            parentNode.textureTransform = result;
          }
        }
      } else
        console.error("The parser doesn't support this type of node: " + node.tagName);
    }

    // check if top-level nodes
    if (typeof result !== 'undefined' && typeof parentNode === 'undefined')
      WbWorld.instance.sceneTree.push(result);

    return result;
  }

  async _parseChildren(node, parentNode, isBoundingObject) {
    for (let i = 0; i < node.childNodes.length; i++) {
      const child = node.childNodes[i];
      if (typeof child.tagName !== 'undefined')
        await this._parseNode(child, parentNode, isBoundingObject);
    }
    return 1;
  }

  async _parseScene(node) {
    const prefix = DefaultUrl.wrenImagesUrl();
    const smaaAreaTexture = await Parser.loadTextureData(prefix, 'smaa_area_texture.png');
    smaaAreaTexture.isTranslucent = false;
    const smaaSearchTexture = await Parser.loadTextureData(prefix, 'smaa_search_texture.png');
    smaaSearchTexture.isTranslucent = false;
    const gtaoNoiseTexture = await Parser.loadTextureData(prefix, 'gtao_noise_texture.png');
    gtaoNoiseTexture.isTranslucent = true;
    return new WbScene(smaaAreaTexture, smaaSearchTexture, gtaoNoiseTexture);
  }

  _parseWorldInfo(node) {
    const coordinateSystem = getNodeAttribute(node, 'coordinateSystem', 'ENU');
    WbWorld.instance.coordinateSystem = coordinateSystem;
    WbWorld.computeUpVector();
  }

  _parseViewpoint(node) {
    const id = getNodeAttribute(node, 'id');
    const fieldOfView = parseFloat(getNodeAttribute(node, 'fieldOfView', M_PI_4));
    const orientation = convertStringToQuaternion(getNodeAttribute(node, 'orientation', '0 1 0 0'));
    const position = convertStringToVec3(getNodeAttribute(node, 'position', '0 0 10'));
    const exposure = parseFloat(getNodeAttribute(node, 'exposure', '1.0'));
    const bloomThreshold = parseFloat(getNodeAttribute(node, 'bloomThreshold', 21));
    const far = parseFloat(getNodeAttribute(node, 'zFar', '2000'));
    const near = parseFloat(getNodeAttribute(node, 'zNear', '0.1'));
    const followSmoothness = parseFloat(getNodeAttribute(node, 'followSmoothness'));
    const followedId = getNodeAttribute(node, 'followedId');
    const ambientOcclusionRadius = parseFloat(getNodeAttribute(node, 'ambientOcclusionRadius', 2));

    return new WbViewpoint(id, fieldOfView, orientation, position, exposure, bloomThreshold, near, far, followSmoothness, followedId, ambientOcclusionRadius);
  }

  async _parseBackground(node) {
    const id = getNodeAttribute(node, 'id');
    const skyColor = convertStringToVec3(getNodeAttribute(node, 'skyColor', '0 0 0'));
    const luminosity = parseFloat(getNodeAttribute(node, 'luminosity', '1'));

    let backUrl = getNodeAttribute(node, 'backUrl');
    let bottomUrl = getNodeAttribute(node, 'bottomUrl');
    let frontUrl = getNodeAttribute(node, 'frontUrl');
    let leftUrl = getNodeAttribute(node, 'leftUrl');
    let rightUrl = getNodeAttribute(node, 'rightUrl');
    let topUrl = getNodeAttribute(node, 'topUrl');

    const cubeImages = [];
    if (typeof backUrl !== 'undefined' && typeof bottomUrl !== 'undefined' && typeof frontUrl !== 'undefined' && typeof leftUrl !== 'undefined' && typeof rightUrl !== 'undefined' && typeof topUrl !== 'undefined') {
      backUrl = backUrl.slice(1, backUrl.length - 1);
      bottomUrl = bottomUrl.slice(1, bottomUrl.length - 1);
      frontUrl = frontUrl.slice(1, frontUrl.length - 1);
      leftUrl = leftUrl.slice(1, leftUrl.length - 1);
      rightUrl = rightUrl.slice(1, rightUrl.length - 1);
      topUrl = topUrl.slice(1, topUrl.length - 1);

      if (WbWorld.instance.coordinateSystem === 'ENU') {
        cubeImages[0] = await Parser.loadTextureData(this._prefix, backUrl, false, 90);
        cubeImages[4] = await Parser.loadTextureData(this._prefix, bottomUrl, false, -90);
        cubeImages[1] = await Parser.loadTextureData(this._prefix, frontUrl, false, -90);
        cubeImages[3] = await Parser.loadTextureData(this._prefix, leftUrl, false, 180);
        cubeImages[2] = await Parser.loadTextureData(this._prefix, rightUrl);
        cubeImages[5] = await Parser.loadTextureData(this._prefix, topUrl, false, -90);
      } else {
        cubeImages[5] = await Parser.loadTextureData(this._prefix, backUrl);
        cubeImages[3] = await Parser.loadTextureData(this._prefix, bottomUrl);
        cubeImages[4] = await Parser.loadTextureData(this._prefix, frontUrl);
        cubeImages[1] = await Parser.loadTextureData(this._prefix, leftUrl);
        cubeImages[0] = await Parser.loadTextureData(this._prefix, rightUrl);
        cubeImages[2] = await Parser.loadTextureData(this._prefix, topUrl);
      }
    }

    let backIrradianceUrl = getNodeAttribute(node, 'backIrradianceUrl');
    let bottomIrradianceUrl = getNodeAttribute(node, 'bottomIrradianceUrl');
    let frontIrradianceUrl = getNodeAttribute(node, 'frontIrradianceUrl');
    let leftIrradianceUrl = getNodeAttribute(node, 'leftIrradianceUrl');
    let rightIrradianceUrl = getNodeAttribute(node, 'rightIrradianceUrl');
    let topIrradianceUrl = getNodeAttribute(node, 'topIrradianceUrl');

    const irradianceCubeURL = [];
    if (typeof backIrradianceUrl !== 'undefined' && typeof bottomIrradianceUrl !== 'undefined' && typeof frontIrradianceUrl !== 'undefined' && typeof leftIrradianceUrl !== 'undefined' && typeof rightIrradianceUrl !== 'undefined' && typeof topIrradianceUrl !== 'undefined') {
      backIrradianceUrl = backIrradianceUrl.slice(1, backIrradianceUrl.length - 1);
      bottomIrradianceUrl = bottomIrradianceUrl.slice(1, bottomIrradianceUrl.length - 1);
      frontIrradianceUrl = frontIrradianceUrl.slice(1, frontIrradianceUrl.length - 1);
      leftIrradianceUrl = leftIrradianceUrl.slice(1, leftIrradianceUrl.length - 1);
      rightIrradianceUrl = rightIrradianceUrl.slice(1, rightIrradianceUrl.length - 1);
      topIrradianceUrl = topIrradianceUrl.slice(1, topIrradianceUrl.length - 1);

      if (WbWorld.instance.coordinateSystem === 'ENU') {
        irradianceCubeURL[0] = await Parser.loadTextureData(this._prefix, backIrradianceUrl, true, 90);
        irradianceCubeURL[4] = await Parser.loadTextureData(this._prefix, bottomIrradianceUrl, true, -90);
        irradianceCubeURL[1] = await Parser.loadTextureData(this._prefix, frontIrradianceUrl, true, -90);
        irradianceCubeURL[3] = await Parser.loadTextureData(this._prefix, leftIrradianceUrl, true, 180);
        irradianceCubeURL[2] = await Parser.loadTextureData(this._prefix, rightIrradianceUrl, true);
        irradianceCubeURL[5] = await Parser.loadTextureData(this._prefix, topIrradianceUrl, true, -90);
      } else {
        irradianceCubeURL[2] = await Parser.loadTextureData(this._prefix, topIrradianceUrl, true);
        irradianceCubeURL[5] = await Parser.loadTextureData(this._prefix, backIrradianceUrl, true);
        irradianceCubeURL[3] = await Parser.loadTextureData(this._prefix, bottomIrradianceUrl, true);
        irradianceCubeURL[4] = await Parser.loadTextureData(this._prefix, frontIrradianceUrl, true);
        irradianceCubeURL[1] = await Parser.loadTextureData(this._prefix, leftIrradianceUrl, true);
        irradianceCubeURL[0] = await Parser.loadTextureData(this._prefix, rightIrradianceUrl, true);
      }
    }

    const background = new WbBackground(id, skyColor, luminosity, cubeImages, irradianceCubeURL);
    WbBackground.instance = background;

    WbWorld.instance.nodes.set(background.id, background);

    return background;
  }

  async _checkUse(node, parentNode) {
    let use = getNodeAttribute(node, 'USE');
    if (typeof use === 'undefined')
      return;

    const id = getNodeAttribute(node, 'id');
    let result = WbWorld.instance.nodes.get(use);

    if (typeof result === 'undefined') {
      use = 'n' + use;
      result = WbWorld.instance.nodes.get(use);
    }

    if (typeof result === 'undefined')
      return;

    const useNode = await result.clone(id);
    if (typeof parentNode !== 'undefined') {
      useNode.parent = parentNode.id;
      if (result instanceof WbShape || result instanceof WbGroup || result instanceof WbLight)
        parentNode.children.push(useNode);
    }

    WbWorld.instance.nodes.set(id, useNode);
    return useNode;
  }

  async _parseTransform(node, parentNode, isBoundingObject) {
    const use = await this._checkUse(node, parentNode);
    if (typeof use !== 'undefined')
      return use;

    let id = getNodeAttribute(node, 'id');
    if (typeof id === 'undefined')
      id = getAnId();
    const isSolid = getNodeAttribute(node, 'solid', 'false').toLowerCase() === 'true';
    const translation = convertStringToVec3(getNodeAttribute(node, 'translation', '0 0 0'));
    const scale = convertStringToVec3(getNodeAttribute(node, 'scale', '1 1 1'));
    const rotation = convertStringToQuaternion(getNodeAttribute(node, 'rotation', '0 1 0 0'));

    const transform = new WbTransform(id, isSolid, translation, scale, rotation);

    WbWorld.instance.nodes.set(transform.id, transform);

    await this._parseChildren(node, transform, isBoundingObject);

    if (typeof parentNode !== 'undefined') {
      transform.parent = parentNode.id;
      parentNode.children.push(transform);
    }

    return transform;
  }

  async _parseGroup(node, parentNode, isBoundingObject) {
    const use = await this._checkUse(node, parentNode);
    if (typeof use !== 'undefined')
      return use;

    let id = getNodeAttribute(node, 'id');
    if (typeof id === 'undefined')
      id = getAnId();

    const isPropeller = getNodeAttribute(node, 'isPropeller', 'false').toLowerCase() === 'true';

    const group = new WbGroup(id, isPropeller);

    WbWorld.instance.nodes.set(group.id, group);
    await this._parseChildren(node, group, isBoundingObject);

    if (typeof parentNode !== 'undefined') {
      group.parent = parentNode.id;
      parentNode.children.push(group);
    }

    return group;
  }

  async _parseShape(node, parentNode, isBoundingObject) {
    const use = await this._checkUse(node, parentNode);
    if (typeof use !== 'undefined')
      return use;

    let id = getNodeAttribute(node, 'id');
    if (typeof id === 'undefined')
      id = getAnId();

    const castShadows = getNodeAttribute(node, 'castShadows', 'false').toLowerCase() === 'true';
    const isPickable = getNodeAttribute(node, 'isPickable', 'true').toLowerCase() === 'true';
    let geometry;
    let appearance;

    for (let i = 0; i < node.childNodes.length; i++) {
      const child = node.childNodes[i];
      if (typeof child.tagName === 'undefined')
        continue;

      if (typeof appearance === 'undefined') {
        if (child.tagName === 'Appearance') {
          if (isBoundingObject)
            continue;
          // If a sibling PBRAppearance is detected, prefer it.
          let pbrAppearanceChild = false;
          for (let j = 0; j < node.childNodes.length; j++) {
            const child0 = node.childNodes[j];
            if (child0.tagName === 'PBRAppearance') {
              pbrAppearanceChild = true;
              break;
            }
          }
          if (pbrAppearanceChild)
            continue;
          appearance = await this._parseAppearance(child);
        } else if (child.tagName === 'PBRAppearance')
          appearance = await this._parsePBRAppearance(child);
        if (typeof appearance !== 'undefined')
          continue;
      }

      if (typeof geometry === 'undefined') {
        geometry = await this._parseGeometry(child, id);
        if (typeof geometry !== 'undefined')
          continue;
      }

      console.log('X3dLoader: Unknown node: ' + child.tagName);
    }

    if (isBoundingObject)
      appearance = undefined;

    const shape = new WbShape(id, castShadows, isPickable, geometry, appearance);

    if (typeof parentNode !== 'undefined') {
      parentNode.children.push(shape);
      shape.parent = parentNode.id;
    }

    if (typeof appearance !== 'undefined')
      appearance.parent = shape.id;

    WbWorld.instance.nodes.set(shape.id, shape);

    return shape;
  }

  async _parseBillboard(node, parentNode) {
    let id = getNodeAttribute(node, 'id');
    if (typeof id === 'undefined')
      id = getAnId();

    const billboard = new WbBillboard(id);

    WbWorld.instance.nodes.set(billboard.id, billboard);
    await this._parseChildren(node, billboard);

    return billboard;
  }

  async _parseDirectionalLight(node, parentNode) {
    const use = await this._checkUse(node, parentNode);
    if (typeof use !== 'undefined')
      return use;

    const id = getNodeAttribute(node, 'id');
    const on = getNodeAttribute(node, 'on', 'true').toLowerCase() === 'true';
    const color = convertStringToVec3(getNodeAttribute(node, 'color', '1 1 1'));
    const direction = convertStringToVec3(getNodeAttribute(node, 'direction', '0 0 -1'));
    const intensity = parseFloat(getNodeAttribute(node, 'intensity', '1'));
    const ambientIntensity = parseFloat(getNodeAttribute(node, 'ambientIntensity', '0'));
    const castShadows = getNodeAttribute(node, 'castShadows', 'false').toLowerCase() === 'true';

    const dirLight = new WbDirectionalLight(id, on, color, direction, intensity, castShadows, ambientIntensity);

    if (typeof parentNode !== 'undefined' && typeof dirLight !== 'undefined') {
      parentNode.children.push(dirLight);
      dirLight.parent = parentNode.id;
    }

    WbWorld.instance.nodes.set(dirLight.id, dirLight);

    return dirLight;
  }

  async _parsePointLight(node, parentNode) {
    const use = await this._checkUse(node, parentNode);
    if (typeof use !== 'undefined')
      return use;

    const id = getNodeAttribute(node, 'id');
    const on = getNodeAttribute(node, 'on', 'true').toLowerCase() === 'true';
    const attenuation = convertStringToVec3(getNodeAttribute(node, 'attenuation', '1 0 0'));
    const color = convertStringToVec3(getNodeAttribute(node, 'color', '1 1 1'));
    const intensity = parseFloat(getNodeAttribute(node, 'intensity', '1'));
    const location = convertStringToVec3(getNodeAttribute(node, 'location', '0 0 0'));
    const radius = parseFloat(getNodeAttribute(node, 'radius', '100'));
    const ambientIntensity = parseFloat(getNodeAttribute(node, 'ambientIntensity', '0'));
    const castShadows = getNodeAttribute(node, 'castShadows', 'false').toLowerCase() === 'true';

    const pointLight = new WbPointLight(id, on, attenuation, color, intensity, location, radius, ambientIntensity, castShadows, parentNode);

    if (typeof parentNode !== 'undefined' && typeof pointLight !== 'undefined')
      parentNode.children.push(pointLight);

    WbWorld.instance.nodes.set(pointLight.id, pointLight);

    return pointLight;
  }

  async _parseSpotLight(node, parentNode) {
    const use = await this._checkUse(node, parentNode);
    if (typeof use !== 'undefined')
      return use;

    const id = getNodeAttribute(node, 'id');
    const on = getNodeAttribute(node, 'on', 'true').toLowerCase() === 'true';
    const attenuation = convertStringToVec3(getNodeAttribute(node, 'attenuation', '1 0 0'));
    const beamWidth = parseFloat(getNodeAttribute(node, 'beamWidth', '0.785'));
    const color = convertStringToVec3(getNodeAttribute(node, 'color', '1 1 1'));
    const cutOffAngle = parseFloat(getNodeAttribute(node, 'cutOffAngle', '0.785'));
    const direction = convertStringToVec3(getNodeAttribute(node, 'direction', '0 0 -1'));
    const intensity = parseFloat(getNodeAttribute(node, 'intensity', '1'));
    const location = convertStringToVec3(getNodeAttribute(node, 'location', '0 0 0'));
    const radius = parseFloat(getNodeAttribute(node, 'radius', '100'));
    const ambientIntensity = parseFloat(getNodeAttribute(node, 'ambientIntensity', '0'));
    const castShadows = getNodeAttribute(node, 'castShadows', 'false').toLowerCase() === 'true';

    const spotLight = new WbSpotLight(id, on, attenuation, beamWidth, color, cutOffAngle, direction, intensity, location, radius, ambientIntensity, castShadows, parentNode);

    if (typeof parentNode !== 'undefined' && typeof spotLight !== 'undefined')
      parentNode.children.push(spotLight);

    WbWorld.instance.nodes.set(spotLight.id, spotLight);

    return spotLight;
  }

  async _parseFog(node) {
    const id = getNodeAttribute(node, 'id');
    const color = convertStringToVec3(getNodeAttribute(node, 'color', '1 1 1'));
    const visibilityRange = parseFloat(getNodeAttribute(node, 'visibilityRange', '0'));
    const fogType = getNodeAttribute(node, 'fogType', 'LINEAR');

    const fog = new WbFog(id, color, visibilityRange, fogType);

    WbWorld.instance.nodes.set(fog.id, fog);

    if (typeof fog !== 'undefined')
      WbWorld.instance.hasFog = true;

    return fog;
  }

  async _parseGeometry(node, parentId) {
    const use = await this._checkUse(node);
    if (typeof use !== 'undefined') {
      use.parent = parentId;
      return use;
    }

    let id = getNodeAttribute(node, 'id');
    if (typeof id === 'undefined')
      id = getAnId();

    let geometry;
    if (node.tagName === 'Box')
      geometry = this._parseBox(node, id);
    else if (node.tagName === 'Sphere')
      geometry = this._parseSphere(node, id);
    else if (node.tagName === 'Cone')
      geometry = this._parseCone(node, id);
    else if (node.tagName === 'Plane')
      geometry = this._parsePlane(node, id);
    else if (node.tagName === 'Cylinder')
      geometry = this._parseCylinder(node, id);
    else if (node.tagName === 'Capsule')
      geometry = this._parseCapsule(node, id);
    else if (node.tagName === 'IndexedFaceSet')
      geometry = this._parseIndexedFaceSet(node, id);
    else if (node.tagName === 'IndexedLineSet')
      geometry = this._parseIndexedLineSet(node, id);
    else if (node.tagName === 'ElevationGrid')
      geometry = this._parseElevationGrid(node, id);
    else if (node.tagName === 'PointSet')
      geometry = this._parsePointSet(node, id);
    else
      console.log('Not a recognized geometry : ' + node.tagName);

    if (typeof parentId !== 'undefined' && typeof geometry !== 'undefined')
      geometry.parent = parentId;

    return geometry;
  }

  _parseBox(node, id) {
    const size = convertStringToVec3(getNodeAttribute(node, 'size', '2 2 2'));

    const box = new WbBox(id, size);
    WbWorld.instance.nodes.set(box.id, box);
    return box;
  }

  _parseSphere(node, id) {
    const radius = parseFloat(getNodeAttribute(node, 'radius', '1'));
    const ico = getNodeAttribute(node, 'ico', 'false').toLowerCase() === 'true';
    const subdivision = parseInt(getNodeAttribute(node, 'subdivision', '1,1'));

    const sphere = new WbSphere(id, radius, ico, subdivision);

    WbWorld.instance.nodes.set(sphere.id, sphere);

    return sphere;
  }

  _parseCone(node, id) {
    const bottomRadius = getNodeAttribute(node, 'bottomRadius', '1');
    const height = getNodeAttribute(node, 'height', '2');
    const subdivision = getNodeAttribute(node, 'subdivision', '32');
    const side = getNodeAttribute(node, 'side', 'true').toLowerCase() === 'true';
    const bottom = getNodeAttribute(node, 'bottom', 'true').toLowerCase() === 'true';

    const cone = new WbCone(id, bottomRadius, height, subdivision, side, bottom);

    WbWorld.instance.nodes.set(cone.id, cone);

    return cone;
  }

  _parseCylinder(node, id) {
    const radius = getNodeAttribute(node, 'radius', '1');
    const height = getNodeAttribute(node, 'height', '2');
    const subdivision = getNodeAttribute(node, 'subdivision', '32');
    const bottom = getNodeAttribute(node, 'bottom', 'true').toLowerCase() === 'true';
    const side = getNodeAttribute(node, 'side', 'true').toLowerCase() === 'true';
    const top = getNodeAttribute(node, 'top', 'true').toLowerCase() === 'true';

    const cylinder = new WbCylinder(id, radius, height, subdivision, bottom, side, top);

    WbWorld.instance.nodes.set(cylinder.id, cylinder);

    return cylinder;
  }

  _parsePlane(node, id) {
    const size = convertStringToVec2(getNodeAttribute(node, 'size', '1,1'));

    const plane = new WbPlane(id, size);

    WbWorld.instance.nodes.set(plane.id, plane);

    return plane;
  }

  _parseCapsule(node, id) {
    const radius = getNodeAttribute(node, 'radius', '1');
    const height = getNodeAttribute(node, 'height', '2');
    const subdivision = getNodeAttribute(node, 'subdivision', '32');
    const bottom = getNodeAttribute(node, 'bottom', 'true').toLowerCase() === 'true';
    const side = getNodeAttribute(node, 'side', 'true').toLowerCase() === 'true';
    const top = getNodeAttribute(node, 'top', 'true').toLowerCase() === 'true';

    const capsule = new WbCapsule(id, radius, height, subdivision, bottom, side, top);

    WbWorld.instance.nodes.set(capsule.id, capsule);

    return capsule;
  }

  _parseIndexedFaceSet(node, id) {
    const coordIndexStr = getNodeAttribute(node, 'coordIndex', '').trim().split(/\s/); ;
    const coordIndex = coordIndexStr.map(Number).filter(el => { return el !== -1; });

    const normalIndexStr = getNodeAttribute(node, 'normalIndex', '').trim().split(/\s/);
    const normalIndex = normalIndexStr.map(Number).filter(el => { return el !== -1; });

    const texCoordIndexStr = getNodeAttribute(node, 'texCoordIndex', '').trim().split(/\s/); ;
    const texCoordIndex = texCoordIndexStr.map(Number).filter(el => { return el !== -1; });

    const coordArray = [];
    const coordinate = node.getElementsByTagName('Coordinate')[0];
    if (typeof coordinate !== 'undefined') {
      const coordStr = getNodeAttribute(coordinate, 'point', '').split(/\s/);
      const coord = coordStr.map(el => parseFloat(el));
      for (let i = 0; i < coord.length; i = i + 3)
        coordArray.push(new WbVector3(coord[i], coord[i + 1], coord[i + 2]));
    }

    const texCoordArray = [];
    const textureCoordinate = node.getElementsByTagName('TextureCoordinate')[0];
    if (typeof textureCoordinate !== 'undefined') {
      const texcoordsStr = getNodeAttribute(textureCoordinate, 'point', '').split(/\s/);
      const texCoord = texcoordsStr.map(el => parseFloat(el));
      for (let i = 0; i < texCoord.length; i = i + 2)
        texCoordArray.push(new WbVector2(texCoord[i], texCoord[i + 1]));
    }

    const normalArray = [];
    const normalNode = node.getElementsByTagName('Normal')[0];
    if (typeof normalNode !== 'undefined') {
      const normalStr = getNodeAttribute(normalNode, 'vector', '').split(/[\s,]+/);
      const normal = normalStr.map(el => parseFloat(el));
      for (let i = 0; i < normal.length; i = i + 3)
        normalArray.push(new WbVector3(normal[i], normal[i + 1], normal[i + 2]));
    }

    const ccw = parseFloat(getNodeAttribute(node, 'ccw', '1'));

    const ifs = new WbIndexedFaceSet(id, coordIndex, normalIndex, texCoordIndex, coordArray, texCoordArray, normalArray, ccw);
    WbWorld.instance.nodes.set(ifs.id, ifs);

    return ifs;
  }

  _parseIndexedLineSet(node, id) {
    const coordinate = node.getElementsByTagName('Coordinate')[0];

    if (typeof coordinate === 'undefined')
      return undefined;

    const indicesStr = getNodeAttribute(node, 'coordIndex', '').trim().split(/\s/);

    const verticesStr = getNodeAttribute(coordinate, 'point', '').trim().split(/\s/);

    const coord = [];
    for (let i = 0; i < verticesStr.length; i += 3)
      coord.push(new WbVector3(parseFloat(verticesStr[i]), parseFloat(verticesStr[i + 1]), parseFloat(verticesStr[i + 2])));

    const coordIndex = indicesStr.map(Number);

    const ils = new WbIndexedLineSet(id, coord, coordIndex);
    WbWorld.instance.nodes.set(ils.id, ils);

    return ils;
  }

  _parseElevationGrid(node, id) {
    const heightStr = getNodeAttribute(node, 'height');
    if (typeof heightStr === 'undefined')
      return;

    const xDimension = parseInt(getNodeAttribute(node, 'xDimension', '0'));
    const xSpacing = parseFloat(getNodeAttribute(node, 'xSpacing', '1'));
    const zDimension = parseInt(getNodeAttribute(node, 'zDimension', '0'));
    const zSpacing = parseFloat(getNodeAttribute(node, 'zSpacing', '1'));
    const thickness = parseFloat(getNodeAttribute(node, 'thickness', '1'));

    const height = heightStr.split(' ').map(Number);

    const eg = new WbElevationGrid(id, height, xDimension, xSpacing, zDimension, zSpacing, thickness);
    WbWorld.instance.nodes.set(eg.id, eg);

    return eg;
  }

  _parsePointSet(node, id) {
    const coordinate = node.getElementsByTagName('Coordinate')[0];

    if (typeof coordinate === 'undefined')
      return;

    const coordStrArray = getNodeAttribute(coordinate, 'point', '').trim().split(/\s/);

    if (typeof coordStrArray === 'undefined')
      return;

    const coordArray = coordStrArray.map(Number);
    const coord = [];
    for (let i = 0; i < coordArray.length; i += 3)
      coord.push(new WbVector3(coordArray[i], coordArray[i + 1], coordArray[i + 2]));

    const colorNode = node.getElementsByTagName('Color')[0];
    let color;
    if (typeof colorNode !== 'undefined') {
      const colorStrArray = getNodeAttribute(colorNode, 'color', '').trim().split(/\s/);
      if (typeof colorStrArray !== 'undefined') {
        const colorArray = colorStrArray.map(Number);
        color = [];
        for (let i = 0; i < colorArray.length; i += 3)
          color.push(new WbVector3(colorArray[i], colorArray[i + 1], colorArray[i + 2]));
      }
    }

    const ps = new WbPointSet(id, coord, color);
    WbWorld.instance.nodes.set(ps.id, ps);

    return ps;
  }

  async _parseSwitch(node, parent) {
    if (typeof parent === 'undefined')
      return;

    const child = node.childNodes[0];

    let boundingObject;
    if (child.tagName === 'Shape')
      boundingObject = await this._parseShape(child, undefined, true);
    else if (child.tagName === 'Transform')
      boundingObject = await this._parseTransform(child, undefined, true);
    else if (child.tagName === 'Group')
      boundingObject = await this._parseGroup(child, undefined, true);
    else
      console.error('Unknown boundingObject: ' + child.tagName);

    if (typeof boundingObject === 'undefined')
      return;

    boundingObject.parent = parent.id;
    parent.boundingObject = boundingObject;

    return boundingObject;
  }

  async _parseAppearance(node, parentId) {
    const use = await this._checkUse(node);
    if (typeof use !== 'undefined')
      return use;

    let id = getNodeAttribute(node, 'id');
    if (typeof id === 'undefined')
      id = getAnId();

    // Get the Material tag.
    const materialNode = node.getElementsByTagName('Material')[0];
    let material;
    if (typeof materialNode !== 'undefined')
      material = await this._parseMaterial(materialNode);

    // Check to see if there is a texture.
    const imageTexture = node.getElementsByTagName('ImageTexture')[0];
    let texture;
    if (typeof imageTexture !== 'undefined')
      texture = await this._parseImageTexture(imageTexture);

    // Check to see if there is a textureTransform.
    const textureTransform = node.getElementsByTagName('TextureTransform')[0];
    let transform;
    if (typeof textureTransform !== 'undefined')
      transform = await this._parseTextureTransform(textureTransform);

    const appearance = new WbAppearance(id, material, texture, transform);
    if (typeof appearance !== 'undefined') {
      if (typeof material !== 'undefined')
        material.parent = appearance.id;

      if (typeof texture !== 'undefined')
        texture.parent = appearance.id;

      if (typeof transform !== 'undefined')
        transform.parent = appearance.id;

      if (typeof parentId !== 'undefined')
        appearance.parent = parentId;

      WbWorld.instance.nodes.set(appearance.id, appearance);
    }

    return appearance;
  }

  async _parseMaterial(node, parentId) {
    const use = await this._checkUse(node);
    if (typeof use !== 'undefined')
      return use;

    let id = getNodeAttribute(node, 'id');
    if (typeof id === 'undefined')
      id = getAnId();

    const ambientIntensity = parseFloat(getNodeAttribute(node, 'ambientIntensity', '0.2'));
    const diffuseColor = convertStringToVec3(getNodeAttribute(node, 'diffuseColor', '0.8 0.8 0.8'));
    const specularColor = convertStringToVec3(getNodeAttribute(node, 'specularColor', '0 0 0'));
    const emissiveColor = convertStringToVec3(getNodeAttribute(node, 'emissiveColor', '0 0 0'));
    const shininess = parseFloat(getNodeAttribute(node, 'shininess', '0.2'));
    const transparency = parseFloat(getNodeAttribute(node, 'transparency', '0'));

    const material = new WbMaterial(id, ambientIntensity, diffuseColor, specularColor, emissiveColor, shininess, transparency);

    if (typeof parentId !== 'undefined')
      material.parent = parentId;

    WbWorld.instance.nodes.set(material.id, material);

    return material;
  }

  async _parseImageTexture(node, parentId) {
    const use = await this._checkUse(node);
    if (typeof use !== 'undefined')
      return use;

    const id = getNodeAttribute(node, 'id');
    let url = getNodeAttribute(node, 'url', '');
    url = url.slice(1, url.length - 1);
    const isTransparent = getNodeAttribute(node, 'isTransparent', 'false').toLowerCase() === 'true';
    const s = getNodeAttribute(node, 'repeatS', 'true').toLowerCase() === 'true';
    const t = getNodeAttribute(node, 'repeatT', 'true').toLowerCase() === 'true';
    const filtering = parseFloat(getNodeAttribute(node, 'filtering', '4'));

    let imageTexture;
    if (typeof url !== 'undefined' && url !== '') {
      imageTexture = new WbImageTexture(id, this._prefix, url, isTransparent, s, t, filtering);
      await imageTexture.updateUrl();
    }

    if (typeof imageTexture !== 'undefined') {
      if (typeof parentId !== 'undefined')
        imageTexture.parent = parentId;

      WbWorld.instance.nodes.set(imageTexture.id, imageTexture);
    }

    return imageTexture;
  }

  async _parsePBRAppearance(node, parentId) {
    const use = await this._checkUse(node);
    if (typeof use !== 'undefined')
      return use;

    const id = getNodeAttribute(node, 'id');

    const baseColor = convertStringToVec3(getNodeAttribute(node, 'baseColor', '1 1 1'));
    const transparency = parseFloat(getNodeAttribute(node, 'transparency', '0'));
    const roughness = parseFloat(getNodeAttribute(node, 'roughness', '0'));
    const metalness = parseFloat(getNodeAttribute(node, 'metalness', '1'));
    const IBLStrength = parseFloat(getNodeAttribute(node, 'IBLStrength', '1'));
    const normalMapFactor = parseFloat(getNodeAttribute(node, 'normalMapFactor', '1'));
    const occlusionMapStrength = parseFloat(getNodeAttribute(node, 'occlusionMapStrength', '1'));
    const emissiveColor = convertStringToVec3(getNodeAttribute(node, 'emissiveColor', '0 0 0'));
    const emissiveIntensity = parseFloat(getNodeAttribute(node, 'emissiveIntensity', '1'));

    // Check to see if there is a textureTransform.
    const textureTransform = node.getElementsByTagName('TextureTransform')[0];
    let transform;
    if (typeof textureTransform !== 'undefined')
      transform = await this._parseTextureTransform(textureTransform);

    const imageTextures = node.getElementsByTagName('ImageTexture');
    let baseColorMap, roughnessMap, metalnessMap, normalMap, occlusionMap, emissiveColorMap;
    for (let i = 0; i < imageTextures.length; i++) {
      const imageTexture = imageTextures[i];
      const type = getNodeAttribute(imageTexture, 'type', undefined);
      if (type === 'baseColor') {
        baseColorMap = await this._parseImageTexture(imageTexture);
        if (typeof baseColorMap !== 'undefined')
          baseColorMap.type = 'baseColorMap';
      } else if (type === 'roughness') {
        roughnessMap = await this._parseImageTexture(imageTexture);
        if (typeof roughnessMap !== 'undefined')
          roughnessMap.type = 'roughnessMap';
      } else if (type === 'metalness') {
        metalnessMap = await this._parseImageTexture(imageTexture);
        if (typeof metalnessMap !== 'undefined')
          metalnessMap.type = 'metalnessMap';
      } else if (type === 'normal') {
        normalMap = await this._parseImageTexture(imageTexture);
        if (typeof normalMap !== 'undefined')
          normalMap.type = 'normalMap';
      } else if (type === 'occlusion') {
        occlusionMap = await this._parseImageTexture(imageTexture);
        if (typeof occlusionMap !== 'undefined')
          occlusionMap.type = 'occlusionMap';
      } else if (type === 'emissiveColor') {
        emissiveColorMap = await this._parseImageTexture(imageTexture);
        if (typeof emissiveColorMap !== 'undefined')
          emissiveColorMap.type = 'emissiveColorMap';
      }
    }

    const pbrAppearance = new WbPBRAppearance(id, baseColor, baseColorMap, transparency, roughness, roughnessMap, metalness, metalnessMap, IBLStrength,
      normalMap, normalMapFactor, occlusionMap, occlusionMapStrength, emissiveColor, emissiveColorMap, emissiveIntensity, transform);

    if (typeof pbrAppearance !== 'undefined') {
      if (typeof transform !== 'undefined')
        transform.parent = pbrAppearance.id;

      if (typeof baseColorMap !== 'undefined')
        baseColorMap.parent = pbrAppearance.id;

      if (typeof roughnessMap !== 'undefined')
        roughnessMap.parent = pbrAppearance.id;

      if (typeof metalnessMap !== 'undefined')
        metalnessMap.parent = pbrAppearance.id;

      if (typeof normalMap !== 'undefined')
        normalMap.parent = pbrAppearance.id;

      if (typeof occlusionMap !== 'undefined')
        occlusionMap.parent = pbrAppearance.id;

      if (typeof emissiveColorMap !== 'undefined')
        emissiveColorMap.parent = pbrAppearance.id;

      if (typeof parentId !== 'undefined')
        pbrAppearance.parent = parentId;

      WbWorld.instance.nodes.set(pbrAppearance.id, pbrAppearance);
    }

    return pbrAppearance;
  }

  async _parseTextureTransform(node, parentId) {
    const use = await this._checkUse(node);
    if (typeof use !== 'undefined')
      return use;

    const id = getNodeAttribute(node, 'id');
    const center = convertStringToVec2(getNodeAttribute(node, 'center', '0 0'));
    const rotation = parseFloat(getNodeAttribute(node, 'rotation', '0'));
    const scale = convertStringToVec2(getNodeAttribute(node, 'scale', '1 1'));
    const translation = convertStringToVec2(getNodeAttribute(node, 'translation', '0 0'));

    const textureTransform = new WbTextureTransform(id, center, rotation, scale, translation);

    if (typeof parentId !== 'undefined')
      textureTransform.parent = parentId;

    WbWorld.instance.nodes.set(textureTransform.id, textureTransform);

    return textureTransform;
  }

  static async loadTextureData(prefix, url, isHdr, rotation) {
    const canvas2 = document.createElement('canvas');
    const context = canvas2.getContext('2d');

    const image = new WbImage();
    if (typeof prefix !== 'undefined' && !url.startsWith('https://raw.githubusercontent.com'))
      url = prefix + url;
    if (isHdr) {
      const img = await Parser.loadHDRImage(url);
      image.bits = img.data;
      image.width = img.width;
      image.height = img.height;
      image.url = url;
      if (typeof rotation !== 'undefined')
        image.bits = rotateHDR(image, rotation);
    } else {
      const img = await Parser.loadImage(url);
      canvas2.width = img.width;
      canvas2.height = img.height;
      if (typeof rotation !== 'undefined') {
        context.save();
        context.translate(canvas2.width / 2, canvas2.height / 2);
        context.rotate(rotation * Math.PI / 180);
        context.drawImage(img, -canvas2.width / 2, -canvas2.height / 2);
        context.restore();
      } else
        context.drawImage(img, 0, 0);

      const dataBGRA = context.getImageData(0, 0, img.width, img.height).data;
      let data = new Uint8ClampedArray(dataBGRA.length);
      data = dataBGRA;

      image.bits = data;
      image.width = img.width;
      image.height = img.height;
      image.url = url;
    }
    return image;
  }

  static loadImage(src) {
    return new Promise((resolve, reject) => {
      const img = new Image();
      img.onload = () => {
        resolve(img);
      };
      img.onerror = () => console.log('Error in loading : ' + src);
      img.setAttribute('crossOrigin', '');
      img.src = src;
    });
  }

  static loadHDRImage(src) {
    return new Promise((resolve, reject) => {
      loadHdr(src, function(img) { resolve(img); });
    });
  }
}

function getNodeAttribute(node, attributeName, defaultValue) {
  console.assert(node && node.attributes);
  if (attributeName in node.attributes)
    return node.attributes.getNamedItem(attributeName).value;
  return defaultValue;
}

function convertStringToVec2(s) {
  s = s.split(/\s/);
  return new WbVector2(parseFloat(s[0]), parseFloat(s[1]));
}

function convertStringToVec3(s) {
  s = s.split(/\s/);
  return new WbVector3(parseFloat(s[0]), parseFloat(s[1]), parseFloat(s[2]));
}

function convertStringToQuaternion(s) {
  const pos = s.split(/\s/);
  return new WbVector4(parseFloat(pos[0]), parseFloat(pos[1]), parseFloat(pos[2]), parseFloat(pos[3]));
}

function rotateHDR(image, rotate) {
  let rotatedbits = [];
  if (rotate === 90) {
    for (let x = 0; x < image.width; x++) {
      for (let y = 0; y < image.height; y++) {
        const u = y * image.width * 3 + x * 3;
        const v = (image.width - 1 - x) * image.height * 3 + y * 3;
        for (let c = 0; c < 3; c++)
          rotatedbits[u + c] = image.bits[v + c];
      }
    }
    const swap = image.width;
    image.width = image.height;
    image.height = swap;
  } else if (rotate === -90) {
    for (let x = 0; x < image.width; x++) {
      for (let y = 0; y < image.height; y++) {
        const u = y * image.width * 3 + x * 3;
        const v = x * image.width * 3 + (image.height - 1 - y) * 3;
        for (let c = 0; c < 3; c++)
          rotatedbits[u + c] = image.bits[v + c];
      }
    }
    const swap = image.width;
    image.width = image.height;
    image.height = swap;
  } else if (rotate === 180) {
    for (let x = 0; x < image.width; x++) {
      for (let y = 0; y < image.height; y++) {
        const u = y * image.width * 3 + x * 3;
        const v = (image.height - 1 - y) * image.width * 3 + (image.width - 1 - x) * 3;
        for (let c = 0; c < 3; c++)
          rotatedbits[u + c] = image.bits[v + c];
      }
    }
  }
  return rotatedbits;
}

export {convertStringToVec3, convertStringToQuaternion};
