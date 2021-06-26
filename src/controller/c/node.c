/*
 * Copyright 1996-2021 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <webots/nodes.h>

const char *wb_node_get_name(WbNodeType t) {
  switch (t) {
    case WB_NODE_ACCELEROMETER:
      return "Accelerometer";
    case WB_NODE_APPEARANCE:
      return "Appearance";
    case WB_NODE_BACKGROUND:
      return "Background";
    case WB_NODE_BALL_JOINT:
      return "BallJoint";
    case WB_NODE_BALL_JOINT_PARAMETERS:
      return "BallJointParameters";
    case WB_NODE_BILLBOARD:
      return "Billboard";
    case WB_NODE_BOX:
      return "Box";
    case WB_NODE_BRAKE:
      return "Brake";
    case WB_NODE_CAMERA:
      return "Camera";
    case WB_NODE_CAPSULE:
      return "Capsule";
    case WB_NODE_CHARGER:
      return "Charger";
    case WB_NODE_COLOR:
      return "Color";
    case WB_NODE_COMPASS:
      return "Compass";
    case WB_NODE_CONE:
      return "Cone";
    case WB_NODE_CONNECTOR:
      return "Connector";
    case WB_NODE_CONTACT_PROPERTIES:
      return "ContactProperties";
    case WB_NODE_COORDINATE:
      return "Coordinate";
    case WB_NODE_CYLINDER:
      return "Cylinder";
    case WB_NODE_DAMPING:
      return "Damping";
    case WB_NODE_DIRECTIONAL_LIGHT:
      return "DirectionalLight";
    case WB_NODE_DISPLAY:
      return "Display";
    case WB_NODE_DISTANCE_SENSOR:
      return "DistanceSensor";
    case WB_NODE_ELEVATION_GRID:
      return "ElevationGrid";
    case WB_NODE_EMITTER:
      return "Emitter";
    case WB_NODE_FLUID:
      return "Fluid";
    case WB_NODE_FOCUS:
      return "Focus";
    case WB_NODE_FOG:
      return "Fog";
    case WB_NODE_GPS:
      return "GPS";
    case WB_NODE_GROUP:
      return "Group";
    case WB_NODE_GYRO:
      return "Gyro";
    case WB_NODE_HINGE_2_JOINT:
      return "Hinge2Joint";
    case WB_NODE_HINGE_JOINT:
      return "HingeJoint";
    case WB_NODE_HINGE_JOINT_PARAMETERS:
      return "HingeJointParameters";
    case WB_NODE_IMAGE_TEXTURE:
      return "ImageTexture";
    case WB_NODE_IMMERSION_PROPERTIES:
      return "ImmersionProperties";
    case WB_NODE_INDEXED_FACE_SET:
      return "IndexedFaceSet";
    case WB_NODE_INDEXED_LINE_SET:
      return "IndexedLineSet";
    case WB_NODE_INERTIAL_UNIT:
      return "InertialUnit";
    case WB_NODE_JOINT_PARAMETERS:
      return "JointParameters";
    case WB_NODE_LED:
      return "LED";
    case WB_NODE_LENS:
      return "Lens";
    case WB_NODE_LENS_FLARE:
      return "LensFlare";
    case WB_NODE_LIDAR:
      return "Lidar";
    case WB_NODE_LIGHT_SENSOR:
      return "LightSensor";
    case WB_NODE_LINEAR_MOTOR:
      return "LinearMotor";
    case WB_NODE_MATERIAL:
      return "Material";
    case WB_NODE_MESH:
      return "Mesh";
    case WB_NODE_MICROPHONE:
      return "Microphone";
    case WB_NODE_MUSCLE:
      return "Muscle";
    case WB_NODE_NORMAL:
      return "Normal";
    case WB_NODE_PBR_APPEARANCE:
      return "PBRAppearance";
    case WB_NODE_PEN:
      return "Pen";
    case WB_NODE_PHYSICS:
      return "Physics";
    case WB_NODE_PLANE:
      return "Plane";
    case WB_NODE_POINT_LIGHT:
      return "PointLight";
    case WB_NODE_POINT_SET:
      return "PointSet";
    case WB_NODE_POSITION_SENSOR:
      return "PositionSensor";
    case WB_NODE_PROPELLER:
      return "Propeller";
    case WB_NODE_RADAR:
      return "Radar";
    case WB_NODE_RADIO:
      return "Radio";
    case WB_NODE_RANGE_FINDER:
      return "RangeFinder";
    case WB_NODE_RECEIVER:
      return "Receiver";
    case WB_NODE_RECOGNITION:
      return "Recognition";
    case WB_NODE_ROBOT:
      return "Robot";
    case WB_NODE_ROTATIONAL_MOTOR:
      return "RotationalMotor";
    case WB_NODE_SHAPE:
      return "Shape";
    case WB_NODE_SKIN:
      return "Skin";
    case WB_NODE_SLIDER_JOINT:
      return "SliderJoint";
    case WB_NODE_SLOT:
      return "Slot";
    case WB_NODE_SOLID:
      return "Solid";
    case WB_NODE_SOLID_REFERENCE:
      return "SolidReference";
    case WB_NODE_SPEAKER:
      return "Speaker";
    case WB_NODE_SPHERE:
      return "Sphere";
    case WB_NODE_SPOT_LIGHT:
      return "SpotLight";
    case WB_NODE_TEXTURE_COORDINATE:
      return "TextureCoordinate";
    case WB_NODE_TEXTURE_TRANSFORM:
      return "TextureTransform";
    case WB_NODE_TOUCH_SENSOR:
      return "TouchSensor";
    case WB_NODE_TRACK:
      return "Track";
    case WB_NODE_TRACK_WHEEL:
      return "TrackWheel";
    case WB_NODE_TRANSFORM:
      return "Transform";
    case WB_NODE_VIEWPOINT:
      return "Viewpoint";
    case WB_NODE_WORLD_INFO:
      return "WorldInfo";
    case WB_NODE_ZOOM:
      return "Zoom";
    default:
      return 0;
  }
}
