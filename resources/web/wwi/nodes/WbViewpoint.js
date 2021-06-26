import {M_PI_4, TAN_M_PI_8} from './utils/constants.js';
import {direction, up} from './utils/utils.js';
import {GtaoLevel, disableAntiAliasing} from './wb_preferences.js';
import WbBaseNode from './WbBaseNode.js';
import WbMatrix4 from './utils/WbMatrix4.js';
import WbVector3 from './utils/WbVector3.js';
import WbVector4 from './utils/WbVector4.js';
import WbWorld from './WbWorld.js';
import WbWrenHdr from './../wren/WbWrenHdr.js';
import WbWrenGtao from './../wren/WbWrenGtao.js';
import WbWrenBloom from './../wren/WbWrenBloom.js';
import WbWrenSmaa from './../wren/WbWrenSmaa.js';
import {webots} from '../webots.js';

export default class WbViewpoint extends WbBaseNode {
  constructor(id, fieldOfView, orientation, position, exposure, bloomThreshold, near, far, followSmoothness, followedId, ambientOcclusionRadius) {
    super(id);

    // the default orientation and position record the initial viewpoint and the modifications due to the follow
    // of an object to allow a smooth reset of the viewpoint
    this.orientation = this._defaultOrientation = orientation;
    this.position = this._defaultPosition = position;

    this.exposure = exposure;
    this.bloomThreshold = bloomThreshold;
    this.near = near;
    this.far = far;
    this.aspectRatio = canvas.width / canvas.height;
    this.fieldOfView = fieldOfView;
    this._fieldOfViewY = M_PI_4;
    this._tanHalfFieldOfViewY = TAN_M_PI_8;
    this.ambientOcclusionRadius = ambientOcclusionRadius;

    this.followSmoothness = followSmoothness;
    this.followedId = followedId;
    this._viewpointForce = new WbVector3();
    this._viewpointVelocity = new WbVector3();

    this._wrenHdr = new WbWrenHdr();
    this._wrenGtao = new WbWrenGtao();
    this._wrenBloom = new WbWrenBloom();
    this._wrenSmaa = new WbWrenSmaa();
  }

  createWrenObjects() {
    super.createWrenObjects();

    this._wrenViewport = _wr_scene_get_viewport(_wr_scene_get_instance());

    _wr_viewport_set_clear_color_rgb(this._wrenViewport, _wrjs_array3(0.0, 0.0, 0.0));
    this._wrenCamera = _wr_viewport_get_camera(this._wrenViewport);
    this._applyPositionToWren();
    this._applyOrientationToWren();
    this._applyNearToWren();
    this._applyFarToWren();
    this._applyFieldOfViewToWren();
    this.updatePostProcessingEffects();
    this._inverseViewMatrix = _wr_transform_get_matrix(this._wrenCamera);
  }

  delete() {
    if (typeof this._wrenSmaa !== 'undefined')
      this._wrenSmaa.delete();

    if (typeof this._wrenHdr !== 'undefined')
      this._wrenHdr.delete();

    if (typeof this._wrenGtao !== 'undefined')
      this._wrenGtao.delete();

    if (typeof this._wrenBloom !== 'undefined')
      this._wrenBloom.delete();
  }

  preFinalize() {
    super.preFinalize();

    this.updateFieldOfView();
    this.updateNear();
    this.updateFar();
  }

  postFinalize() {
    super.postFinalize();

    this.updatePostProcessingEffects();
  }

  resetViewpoint() {
    this.position = this._defaultPosition;
    this.orientation = this._defaultOrientation;
    this.updatePosition();
    this.updateOrientation();
  }

  // Converts screen coordinates to world coordinates
  toWorld(pos) {
    let zFar = this.far;
    if (zFar === 0)
      zFar = WbViewpoint.DEFAULT_FAR;

    const projection = new WbMatrix4();
    projection.set(1.0 / (this.aspectRatio * this._tanHalfFieldOfViewY), 0, 0, 0, 0, 1.0 / this._tanHalfFieldOfViewY, 0, 0, 0, 0, zFar / (this.near - zFar), -(zFar * this.near) / (zFar - this.near), 0, 0, -1, 0);
    const eye = new WbVector3(this.position.x, this.position.y, this.position.z);
    const center = eye.add(direction(this.orientation));
    const upVec = up(this.orientation);

    const f = (center.sub(eye)).normalized();
    const s = f.cross(upVec).normalized();
    const u = s.cross(f);

    const view = new WbMatrix4();
    view.set(-s.x, -s.y, -s.z, s.dot(eye), u.x, u.y, u.z, -u.dot(eye), f.x, f.y, f.z, -f.dot(eye), 0, 0, 0, 1);

    const inverse = projection.mul(view);
    if (!inverse.inverse())
      return;

    let screenCoord = new WbVector4(pos.x, pos.y, pos.z, 1.0);
    screenCoord = inverse.mulByVec4(screenCoord);
    return screenCoord.div(screenCoord.w);
  }

  updateAspectRatio(renderWindowAspectRatio) {
    if (!this.wrenObjectsCreatedCalled)
      return;

    this.aspectRatio = renderWindowAspectRatio;
    _wr_camera_set_aspect_ratio(this._wrenCamera, this.aspectRatio);

    this._updateFieldOfViewY();

    this._applyFieldOfViewToWren();
  }

  updateExposure() {
    if (this.wrenObjectsCreatedCalled && this._wrenHdr)
      this._wrenHdr.setExposure(this.exposure);
  }

  updateFar() {
    if (this.far > 0.0 && this.far < this.near)
      this.far = this.near + 1.0;

    if (this.wrenObjectsCreatedCalled)
      this._applyFarToWren();
  }

  updateFieldOfView() {
    this._updateFieldOfViewY();

    if (this.wrenObjectsCreatedCalled)
      this._applyFieldOfViewToWren();
  }

  updateFollowUp(time, forcePosition) {
    if (typeof this.followedId === 'undefined' || typeof WbWorld.instance.nodes.get(this.followedId) === 'undefined')
      return;

    if (typeof this._viewpointLastUpdate === 'undefined')
      this._viewpointLastUpdate = time;

    const timeInterval = Math.abs(time - this._viewpointLastUpdate) / 1000;
    const mass = ((this.followSmoothness < 0.05) ? 0.0 : ((this.followSmoothness > 1.0) ? 1.0 : this.followSmoothness));

    if (timeInterval > 0) {
      this._viewpointLastUpdate = time;
      let deltaPosition;

      if (typeof this._followedObjectDeltaPosition !== 'undefined')
        this._viewpointForce = this._viewpointForce.add(this._followedObjectDeltaPosition);

      if (forcePosition || mass === 0 || (timeInterval > 0.1 && typeof webots.animation === 'undefined')) {
        deltaPosition = new WbVector3(this._viewpointForce.x, this._viewpointForce.y, this._viewpointForce.z);
        this._viewpointVelocity = new WbVector3();
      } else {
        let acceleration = new WbVector3(this._viewpointForce.x, this._viewpointForce.y, this._viewpointForce.z);
        acceleration = acceleration.mul(timeInterval / mass);
        this._viewpointVelocity = this._viewpointVelocity.add(acceleration);
        const scalarVelocity = this._viewpointVelocity.length();

        let scalarObjectVelocityProjection;
        if (typeof this._followedObjectDeltaPosition !== 'undefined') {
          let objectVelocity = new WbVector3(this._followedObjectDeltaPosition.x, this._followedObjectDeltaPosition.y, this._followedObjectDeltaPosition.z);
          objectVelocity = objectVelocity.div(timeInterval);
          scalarObjectVelocityProjection = objectVelocity.dot(this._viewpointVelocity) / scalarVelocity;
        } else
          scalarObjectVelocityProjection = 0;

        let viewpointFriction = 0.05 / mass;
        if (viewpointFriction > 0 && scalarVelocity > scalarObjectVelocityProjection) {
          const velocityFactor = (scalarVelocity - (scalarVelocity - scalarObjectVelocityProjection) * viewpointFriction) / scalarVelocity;
          this._viewpointVelocity = this._viewpointVelocity.mul(velocityFactor);
        }

        deltaPosition = this._viewpointVelocity.mul(timeInterval);
      }
      this._viewpointForce = this._viewpointForce.sub(deltaPosition);
      this.position = this.position.add(deltaPosition);
      this._defaultPosition = this._defaultPosition.add(deltaPosition);
      this._followedObjectDeltaPosition = undefined;
      this.updatePosition();
    }
  }

  setFollowedObjectDeltaPosition(newPosition, previousPosition) {
    this._followedObjectDeltaPosition = newPosition.sub(previousPosition);
  }

  updateNear() {
    if (this.far > 0.0 && this.far < this.near)
      this.near = this.far;

    if (this.wrenObjectsCreatedCalled)
      this._applyNearToWren();
  }

  updatePosition() {
    if (this.wrenObjectsCreatedCalled)
      this._applyPositionToWren();

    WbWorld.instance.billboards.forEach(id => {
      let billboard = WbWorld.instance.nodes.get(id);
      if (typeof billboard !== 'undefined')
        billboard.updatePosition();
    });
  }

  updatePostProcessingEffects() {
    if (!this.wrenObjectsCreatedCalled)
      return;

    if (this._wrenSmaa) {
      if (disableAntiAliasing)
        this._wrenSmaa.detachFromViewport();
      else
        this._wrenSmaa.setup(this._wrenViewport);
    }

    if (this._wrenHdr) {
      this._wrenHdr.setup(this._wrenViewport);
      this.updateExposure();
    }

    if (this._wrenGtao) {
      const qualityLevel = GtaoLevel;
      if (qualityLevel === 0)
        this._wrenGtao.detachFromViewport();
      else {
        this._wrenGtao.setHalfResolution(qualityLevel <= 2);
        this._wrenGtao.setup(this._wrenViewport);
        this.updateNear();
        this.updateFar();
        this._updateFieldOfViewY();
      }
    }

    if (this._wrenBloom) {
      if (this.bloomThreshold === -1.0)
        this._wrenBloom.detachFromViewport();
      else
        this._wrenBloom.setup(this._wrenViewport);

      this._wrenBloom.setThreshold(this.bloomThreshold);
    }

    this.updateAspectRatio(canvas.width / canvas.height);
  }

  updatePostProcessingParameters() {
    if (!this.wrenObjectsCreatedCalled)
      return;

    if (this._wrenHdr)
      this.updateExposure();

    if (this._wrenGtao) {
      if (this.ambientOcclusionRadius === 0.0 || GtaoLevel === 0) {
        this._wrenGtao.detachFromViewport();
        return;
      } else if (!this._wrenGtao.hasBeenSetup)
        this._wrenGtao.setup(this._wrenViewport);

      const qualityLevel = GtaoLevel;
      this.updateNear();
      this._wrenGtao.setRadius(this.ambientOcclusionRadius);
      this._wrenGtao.setQualityLevel(qualityLevel);
      this._wrenGtao.applyOldInverseViewMatrixToWren();
      this._wrenGtao.copyNewInverseViewMatrix(this._inverseViewMatrix);
    }

    if (this._wrenBloom) {
      if (this.bloomThreshold === -1.0) {
        this._wrenBloom.detachFromViewport();
        return;
      } else if (!this._wrenBloom.hasBeenSetup)
        this._wrenBloom.setup(this._wrenViewport);

      this._wrenBloom.setThreshold(this.bloomThreshold);
    }
  }

  updateOrientation() {
    if (this.wrenObjectsCreatedCalled)
      this._applyOrientationToWren();
  }

  // Private functions

  _applyFarToWren() {
    if (this.far > 0.0)
      _wr_camera_set_far(this._wrenCamera, this.far);
    else
      _wr_camera_set_far(this._wrenCamera, WbViewpoint.DEFAULT_FAR);
  }

  _applyFieldOfViewToWren() {
    _wr_camera_set_fovy(this._wrenCamera, this._fieldOfViewY);
    if (this._wrenGtao)
      this._wrenGtao.setFov(this._fieldOfViewY);
  }

  _applyNearToWren() {
    _wr_camera_set_near(this._wrenCamera, this.near);
  }

  _applyOrientationToWren() {
    _wr_camera_set_orientation(this._wrenCamera, _wrjs_array4(this.orientation.w, this.orientation.x, this.orientation.y, this.orientation.z));
  }

  _applyPositionToWren() {
    _wr_camera_set_position(this._wrenCamera, _wrjs_array3(this.position.x, this.position.y, this.position.z));
  }

  _updateFieldOfViewY() {
    this._tanHalfFieldOfViewY = Math.tan(0.5 * this.fieldOfView);

    // According to VRML standards, the meaning of fieldOfView depends on the aspect ratio:
    // the view angle is taken with respect to the largest dimension
    if (this.aspectRatio < 1.0)
      this._fieldOfViewY = this.fieldOfView;
    else {
      this._tanHalfFieldOfViewY /= this.aspectRatio;
      this._fieldOfViewY = 2.0 * Math.atan(this._tanHalfFieldOfViewY);
    }
  }
}

WbViewpoint.DEFAULT_FAR = 1000000.0;
