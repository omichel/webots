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

#define WB_ALLOW_MIXING_C_AND_CPP_API
#include <webots/supervisor.h>
#include <webots/Node.hpp>

#include <iostream>
#include <map>

using namespace std;
using namespace webots;

static map<WbNodeRef, Node *> nodeMap;

Node *Node::findNode(WbNodeRef ref) {
  if (!ref)
    return NULL;

  map<WbNodeRef, Node *>::iterator iter = nodeMap.find(ref);
  if (iter != nodeMap.end())
    return iter->second;

  Node *node = new Node(ref);
  nodeMap.insert(pair<WbNodeRef, Node *>(ref, node));
  return node;
}

void Node::cleanup() {
  map<WbNodeRef, Node *>::iterator iter;
  for (iter = nodeMap.begin(); iter != nodeMap.end(); ++iter)
    delete (*iter).second;

  nodeMap.clear();
}

Node::Node(WbNodeRef ref) {
  nodeRef = ref;
}

void Node::remove() {
  wb_supervisor_node_remove(nodeRef);
}

int Node::getId() const {
  return wb_supervisor_node_get_id(nodeRef);
}

Node::Type Node::getType() const {
  return Type(wb_supervisor_node_get_type(nodeRef));
}

std::string Node::getDef() const {
  return string(wb_supervisor_node_get_def(nodeRef));
}

std::string Node::getTypeName() const {
  return string(wb_supervisor_node_get_type_name(nodeRef));
}

std::string Node::getBaseTypeName() const {
  return string(wb_supervisor_node_get_base_type_name(nodeRef));
}

Node *Node::getParentNode() const {
  WbNodeRef parentRef = wb_supervisor_node_get_parent_node(nodeRef);
  return findNode(parentRef);
}

Node *Node::getFromProtoDef(const std::string &name) const {
  WbNodeRef internalNodeRef = wb_supervisor_node_get_from_proto_def(nodeRef, name.c_str());
  return findNode(internalNodeRef);
}

bool Node::isProto() const {
  return wb_supervisor_node_is_proto(nodeRef);
}

int Node::getNumberOfFields() const {
  return wb_supervisor_node_get_number_of_fields(nodeRef);
}

int Node::getProtoNumberOfFields() const {
  return wb_supervisor_node_get_proto_number_of_fields(nodeRef);
}

Field *Node::getField(const std::string &fieldName) const {
  WbFieldRef fieldRef = wb_supervisor_node_get_field(nodeRef, fieldName.c_str());
  return Field::findField(fieldRef);
}

Field *Node::getFieldByIndex(const int index) const {
  WbFieldRef fieldRef = wb_supervisor_node_get_field_by_index(nodeRef, index);
  return Field::findField(fieldRef);
}

Field *Node::getProtoField(const std::string &fieldName) const {
  WbFieldRef fieldRef = wb_supervisor_node_get_proto_field(nodeRef, fieldName.c_str());
  return Field::findField(fieldRef);
}

Field *Node::getProtoFieldByIndex(const int index) const {
  WbFieldRef fieldRef = wb_supervisor_node_get_proto_field_by_index(nodeRef, index);
  return Field::findField(fieldRef);
}

const double *Node::getPosition() const {
  return wb_supervisor_node_get_position(nodeRef);
}

const double *Node::getOrientation() const {
  return wb_supervisor_node_get_orientation(nodeRef);
}

const double *Node::getPose() const {
  return wb_supervisor_node_get_pose(nodeRef, NULL);
}

const double *Node::getPose(const Node *fromNode) const {
  return wb_supervisor_node_get_pose(nodeRef, fromNode->nodeRef);
}

void Node::enablePoseTracking(int samplingPeriod) const {
  wb_supervisor_node_enable_pose_tracking(nodeRef, samplingPeriod, NULL);
}

void Node::disablePoseTracking() const {
  wb_supervisor_node_disable_pose_tracking(nodeRef, NULL);
}

void Node::enablePoseTracking(int samplingPeriod, const Node *fromNode) const {
  wb_supervisor_node_enable_pose_tracking(nodeRef, samplingPeriod, fromNode->nodeRef);
}

void Node::disablePoseTracking(const Node *fromNode) const {
  wb_supervisor_node_disable_pose_tracking(nodeRef, fromNode->nodeRef);
}

const double *Node::getCenterOfMass() const {
  return wb_supervisor_node_get_center_of_mass(nodeRef);
}

const double *Node::getContactPoint(int index) const {
  return wb_supervisor_node_get_contact_point(nodeRef, index);
}

Node *Node::getContactPointNode(int index) const {
  return findNode(wb_supervisor_node_get_contact_point_node(nodeRef, index));
}

int Node::getNumberOfContactPoints(bool includeDescendants) const {
  return wb_supervisor_node_get_number_of_contact_points(nodeRef, includeDescendants);
}

bool Node::getStaticBalance() const {
  return wb_supervisor_node_get_static_balance(nodeRef);
}

const double *Node::getVelocity() const {
  return wb_supervisor_node_get_velocity(nodeRef);
}

std::string Node::exportString() const {
  return string(wb_supervisor_node_export_string(nodeRef));
}

void Node::setVelocity(const double velocity[6]) {
  wb_supervisor_node_set_velocity(nodeRef, velocity);
}

void Node::resetPhysics() {
  wb_supervisor_node_reset_physics(nodeRef);
}

void Node::restartController() {
  wb_supervisor_node_restart_controller(nodeRef);
}

void Node::setVisibility(Node *from, bool visible) {
  wb_supervisor_node_set_visibility(nodeRef, from->nodeRef, visible);
}

void Node::moveViewpoint() const {
  wb_supervisor_node_move_viewpoint(nodeRef);
}

void Node::addForce(const double force[3], bool relative) {
  wb_supervisor_node_add_force(nodeRef, force, relative);
}

void Node::addForceWithOffset(const double force[3], const double offset[3], bool relative) {
  wb_supervisor_node_add_force_with_offset(nodeRef, force, offset, relative);
}

void Node::addTorque(const double torque[3], bool relative) {
  wb_supervisor_node_add_torque(nodeRef, torque, relative);
}

void Node::saveState(const std::string &stateName) {
  wb_supervisor_node_save_state(nodeRef, stateName.c_str());
}

void Node::loadState(const std::string &stateName) {
  wb_supervisor_node_load_state(nodeRef, stateName.c_str());
}
