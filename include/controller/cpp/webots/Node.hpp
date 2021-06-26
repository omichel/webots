// Copyright 1996-2021 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NODE_HPP
#define NODE_HPP

#define WB_USING_CPP_API
#include <string>
#include <webots/Field.hpp>
#include "../../c/webots/types.h"

// Note: should match with node.h

namespace webots {
  class Field;
  class Node {
  public:
    typedef enum {
      NO_NODE,
      // 3D rendering
      APPEARANCE,
      BACKGROUND,
      BILLBOARD,
      BOX,
      CAPSULE,
      COLOR,
      CONE,
      COORDINATE,
      CYLINDER,
      DIRECTIONAL_LIGHT,
      ELEVATION_GRID,
      FOG,
      GROUP,
      IMAGE_TEXTURE,
      INDEXED_FACE_SET,
      INDEXED_LINE_SET,
      MATERIAL,
      MESH,
      MUSCLE,
      NORMAL,
      PBR_APPEARANCE,
      PLANE,
      POINT_LIGHT,
      POINT_SET,
      SHAPE,
      SPHERE,
      SPOT_LIGHT,
      TEXTURE_COORDINATE,
      TEXTURE_TRANSFORM,
      TRANSFORM,
      VIEWPOINT,
      // robots
      ROBOT,
      // devices
      ACCELEROMETER,
      BRAKE,
      CAMERA,
      COMPASS,
      CONNECTOR,
      DISPLAY,
      DISTANCE_SENSOR,
      EMITTER,
      GPS,
      GYRO,
      INERTIAL_UNIT,
      LED,
      LIDAR,
      LIGHT_SENSOR,
      LINEAR_MOTOR,
      PEN,
      POSITION_SENSOR,
      PROPELLER,
      RADAR,
      RANGE_FINDER,
      RECEIVER,
      ROTATIONAL_MOTOR,
      SPEAKER,
      TOUCH_SENSOR,
      // misc
      BALL_JOINT,
      BALL_JOINT_PARAMETERS,
      CHARGER,
      CONTACT_PROPERTIES,
      DAMPING,
      FLUID,
      FOCUS,
      HINGE_JOINT,
      HINGE_JOINT_PARAMETERS,
      HINGE_2_JOINT,
      IMMERSION_PROPERTIES,
      JOINT_PARAMETERS,
      LENS,
      LENS_FLARE,
      PHYSICS,
      RECOGNITION,
      SLIDER_JOINT,
      SLOT,
      SOLID,
      SOLID_REFERENCE,
      TRACK,
      TRACK_WHEEL,
      WORLD_INFO,
      ZOOM,
      // experimental
      MICROPHONE,
      RADIO,
      SKIN
    } Type;

    virtual void remove();
    int getId() const;
    Type getType() const;
    std::string getDef() const;
    std::string getTypeName() const;
    std::string getBaseTypeName() const;
    Node *getParentNode() const;
    bool isProto() const;
    Node *getFromProtoDef(const std::string &name) const;
    int getNumberOfFields() const;
    int getProtoNumberOfFields() const;
    Field *getField(const std::string &fieldName) const;
    Field *getProtoField(const std::string &fieldName) const;
    Field *getFieldByIndex(const int index) const;
    Field *getProtoFieldByIndex(const int index) const;
    const double *getPosition() const;
    const double *getOrientation() const;
    const double *getPose() const;
    const double *getPose(const Node *fromNode) const;
    void enablePoseTracking(int samplingPeriod) const;
    void disablePoseTracking() const;
    void enablePoseTracking(int samplingPeriod, const Node *fromNode) const;
    void disablePoseTracking(const Node *fromNode) const;
    const double *getCenterOfMass() const;
    const double *getContactPoint(int index) const;
    Node *getContactPointNode(int index) const;
    int getNumberOfContactPoints(bool includeDescendants = false) const;
    bool getStaticBalance() const;
    const double *getVelocity() const;
    std::string exportString() const;

    void setVelocity(const double velocity[6]);
    void resetPhysics();
    void restartController();

    void moveViewpoint() const;
    void setVisibility(Node *from, bool visible);

    void addForce(const double force[3], bool relative);
    void addForceWithOffset(const double force[3], const double offset[3], bool relative);
    void addTorque(const double torque[3], bool relative);

    void saveState(const std::string &stateName);
    void loadState(const std::string &stateName);

    // DO NOT USE THESE FUNCTIONS: THEY ARE RESERVED FOR INTERNAL USE:
    static Node *findNode(WbNodeRef ref);
    static void cleanup();

  private:
    Node(WbNodeRef nodeRef);
    virtual ~Node() {}

    WbNodeRef nodeRef;
  };
}  // namespace webots

#endif  // NODE_HPP
