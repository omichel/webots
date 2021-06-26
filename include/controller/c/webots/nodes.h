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

#ifndef WB_NODES_H
#define WB_NODES_H

// IMPORTANT: any modification of this file must also be propagated to:
//  1. include/controller/cpp/webots/Node.hpp
//  2. lib/matlab/mgenerate.py

typedef enum {
  WB_NODE_NO_NODE,
  // 3D rendering
  WB_NODE_APPEARANCE,
  WB_NODE_BACKGROUND,
  WB_NODE_BILLBOARD,
  WB_NODE_BOX,
  WB_NODE_CAPSULE,
  WB_NODE_COLOR,
  WB_NODE_CONE,
  WB_NODE_COORDINATE,
  WB_NODE_CYLINDER,
  WB_NODE_DIRECTIONAL_LIGHT,
  WB_NODE_ELEVATION_GRID,
  WB_NODE_FOG,
  WB_NODE_GROUP,
  WB_NODE_IMAGE_TEXTURE,
  WB_NODE_INDEXED_FACE_SET,
  WB_NODE_INDEXED_LINE_SET,
  WB_NODE_MATERIAL,
  WB_NODE_MESH,
  WB_NODE_MUSCLE,
  WB_NODE_NORMAL,
  WB_NODE_PBR_APPEARANCE,
  WB_NODE_PLANE,
  WB_NODE_POINT_LIGHT,
  WB_NODE_POINT_SET,
  WB_NODE_SHAPE,
  WB_NODE_SPHERE,
  WB_NODE_SPOT_LIGHT,
  WB_NODE_TEXTURE_COORDINATE,
  WB_NODE_TEXTURE_TRANSFORM,
  WB_NODE_TRANSFORM,
  WB_NODE_VIEWPOINT,
  // robots
  WB_NODE_ROBOT,
  // devices
  WB_NODE_ACCELEROMETER,
  WB_NODE_BRAKE,
  WB_NODE_CAMERA,
  WB_NODE_COMPASS,
  WB_NODE_CONNECTOR,
  WB_NODE_DISPLAY,
  WB_NODE_DISTANCE_SENSOR,
  WB_NODE_EMITTER,
  WB_NODE_GPS,
  WB_NODE_GYRO,
  WB_NODE_INERTIAL_UNIT,
  WB_NODE_LED,
  WB_NODE_LIDAR,
  WB_NODE_LIGHT_SENSOR,
  WB_NODE_LINEAR_MOTOR,
  WB_NODE_PEN,
  WB_NODE_POSITION_SENSOR,
  WB_NODE_PROPELLER,
  WB_NODE_RADAR,
  WB_NODE_RANGE_FINDER,
  WB_NODE_RECEIVER,
  WB_NODE_ROTATIONAL_MOTOR,
  WB_NODE_SPEAKER,
  WB_NODE_TOUCH_SENSOR,
  // misc
  WB_NODE_BALL_JOINT,
  WB_NODE_BALL_JOINT_PARAMETERS,
  WB_NODE_CHARGER,
  WB_NODE_CONTACT_PROPERTIES,
  WB_NODE_DAMPING,
  WB_NODE_FLUID,
  WB_NODE_FOCUS,
  WB_NODE_HINGE_JOINT,
  WB_NODE_HINGE_JOINT_PARAMETERS,
  WB_NODE_HINGE_2_JOINT,
  WB_NODE_IMMERSION_PROPERTIES,
  WB_NODE_JOINT_PARAMETERS,
  WB_NODE_LENS,
  WB_NODE_LENS_FLARE,
  WB_NODE_PHYSICS,
  WB_NODE_RECOGNITION,
  WB_NODE_SLIDER_JOINT,
  WB_NODE_SLOT,
  WB_NODE_SOLID,
  WB_NODE_SOLID_REFERENCE,
  WB_NODE_TRACK,
  WB_NODE_TRACK_WHEEL,
  WB_NODE_WORLD_INFO,
  WB_NODE_ZOOM,
  // experimental
  WB_NODE_MICROPHONE,
  WB_NODE_RADIO,
  WB_NODE_SKIN
} WbNodeType;

const char *wb_node_get_name(WbNodeType t);

#endif /* WB_NODES_H */
