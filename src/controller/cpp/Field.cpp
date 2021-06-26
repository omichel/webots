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
#include <webots/Field.hpp>

#include <stdio.h>
#include <map>

using namespace std;
using namespace webots;

static map<WbFieldRef, Field *> fieldMap;

Field *Field::findField(WbFieldRef ref) {
  if (!ref)
    return NULL;

  map<WbFieldRef, Field *>::iterator iter = fieldMap.find(ref);
  if (iter != fieldMap.end())
    return iter->second;

  Field *field = new Field(ref);
  fieldMap.insert(pair<WbFieldRef, Field *>(ref, field));
  return field;
}

void Field::cleanup() {
  map<WbFieldRef, Field *>::iterator iter;
  for (iter = fieldMap.begin(); iter != fieldMap.end(); ++iter)
    delete (*iter).second;

  fieldMap.clear();
}

Field::Field(WbFieldRef ref) {
  fieldRef = ref;
}

Field::Type Field::getType() const {
  return Type(wb_supervisor_field_get_type(fieldRef));
}

void Field::enableSFTracking(int samplingPeriod) {
  wb_supervisor_field_enable_sf_tracking(fieldRef, samplingPeriod);
}

void Field::disableSFTracking() {
  wb_supervisor_field_disable_sf_tracking(fieldRef);
}

std::string Field::getTypeName() const {
  return string(wb_supervisor_field_get_type_name(fieldRef));
}

std::string Field::getName() const {
  return string(wb_supervisor_field_get_name(fieldRef));
}

int Field::getCount() const {
  return wb_supervisor_field_get_count(fieldRef);
}

bool Field::getSFBool() const {
  return wb_supervisor_field_get_sf_bool(fieldRef);
}

int Field::getSFInt32() const {
  return wb_supervisor_field_get_sf_int32(fieldRef);
}

double Field::getSFFloat() const {
  return wb_supervisor_field_get_sf_float(fieldRef);
}

const double *Field::getSFVec2f() const {
  return wb_supervisor_field_get_sf_vec2f(fieldRef);
}

const double *Field::getSFVec3f() const {
  return wb_supervisor_field_get_sf_vec3f(fieldRef);
}

const double *Field::getSFRotation() const {
  return wb_supervisor_field_get_sf_rotation(fieldRef);
}

const double *Field::getSFColor() const {
  return wb_supervisor_field_get_sf_color(fieldRef);
}

string Field::getSFString() const {
  return string(wb_supervisor_field_get_sf_string(fieldRef));
}

Node *Field::getSFNode() const {
  WbNodeRef nodeRef = wb_supervisor_field_get_sf_node(fieldRef);
  return Node::findNode(nodeRef);
}

bool Field::getMFBool(int index) const {
  return wb_supervisor_field_get_mf_bool(fieldRef, index);
}

int Field::getMFInt32(int index) const {
  return wb_supervisor_field_get_mf_int32(fieldRef, index);
}

double Field::getMFFloat(int index) const {
  return wb_supervisor_field_get_mf_float(fieldRef, index);
}

const double *Field::getMFVec2f(int index) const {
  return wb_supervisor_field_get_mf_vec2f(fieldRef, index);
}

const double *Field::getMFVec3f(int index) const {
  return wb_supervisor_field_get_mf_vec3f(fieldRef, index);
}

const double *Field::getMFRotation(int index) const {
  return wb_supervisor_field_get_mf_rotation(fieldRef, index);
}

const double *Field::getMFColor(int index) const {
  return wb_supervisor_field_get_mf_color(fieldRef, index);
}

string Field::getMFString(int index) const {
  return string(wb_supervisor_field_get_mf_string(fieldRef, index));
}

Node *Field::getMFNode(int index) const {
  WbNodeRef nodeRef = wb_supervisor_field_get_mf_node(fieldRef, index);
  return Node::findNode(nodeRef);
}

void Field::setSFBool(bool value) {
  wb_supervisor_field_set_sf_bool(fieldRef, value);
}

void Field::setSFInt32(int value) {
  wb_supervisor_field_set_sf_int32(fieldRef, value);
}

void Field::setSFFloat(double value) {
  wb_supervisor_field_set_sf_float(fieldRef, value);
}

void Field::setSFVec2f(const double values[2]) {
  wb_supervisor_field_set_sf_vec2f(fieldRef, values);
}

void Field::setSFVec3f(const double values[3]) {
  wb_supervisor_field_set_sf_vec3f(fieldRef, values);
}

void Field::setSFRotation(const double values[4]) {
  wb_supervisor_field_set_sf_rotation(fieldRef, values);
}

void Field::setSFColor(const double values[4]) {
  wb_supervisor_field_set_sf_color(fieldRef, values);
}

void Field::setSFString(const string &value) {
  wb_supervisor_field_set_sf_string(fieldRef, value.c_str());
}

void Field::setMFBool(int index, bool value) {
  wb_supervisor_field_set_mf_bool(fieldRef, index, value);
}

void Field::setMFInt32(int index, int value) {
  wb_supervisor_field_set_mf_int32(fieldRef, index, value);
}

void Field::setMFFloat(int index, double value) {
  wb_supervisor_field_set_mf_float(fieldRef, index, value);
}

void Field::setMFVec2f(int index, const double values[2]) {
  wb_supervisor_field_set_mf_vec2f(fieldRef, index, values);
}

void Field::setMFVec3f(int index, const double values[3]) {
  wb_supervisor_field_set_mf_vec3f(fieldRef, index, values);
}

void Field::setMFRotation(int index, const double values[4]) {
  wb_supervisor_field_set_mf_rotation(fieldRef, index, values);
}

void Field::setMFColor(int index, const double values[3]) {
  wb_supervisor_field_set_mf_color(fieldRef, index, values);
}

void Field::setMFString(int index, const string &value) {
  wb_supervisor_field_set_mf_string(fieldRef, index, value.c_str());
}

void Field::insertMFBool(int index, bool value) {
  wb_supervisor_field_insert_mf_bool(fieldRef, index, value);
}

void Field::insertMFInt32(int index, int value) {
  wb_supervisor_field_insert_mf_int32(fieldRef, index, value);
}

void Field::insertMFFloat(int index, double value) {
  wb_supervisor_field_insert_mf_float(fieldRef, index, value);
}
void Field::insertMFVec2f(int index, const double values[2]) {
  wb_supervisor_field_insert_mf_vec2f(fieldRef, index, values);
}

void Field::insertMFVec3f(int index, const double values[3]) {
  wb_supervisor_field_insert_mf_vec3f(fieldRef, index, values);
}

void Field::insertMFRotation(int index, const double values[4]) {
  wb_supervisor_field_insert_mf_rotation(fieldRef, index, values);
}

void Field::insertMFColor(int index, const double values[3]) {
  wb_supervisor_field_insert_mf_color(fieldRef, index, values);
}

void Field::insertMFString(int index, const std::string &value) {
  wb_supervisor_field_insert_mf_string(fieldRef, index, value.c_str());
}

void Field::removeMF(int index) {
  wb_supervisor_field_remove_mf(fieldRef, index);
}

void Field::removeSF() {
  wb_supervisor_field_remove_sf(fieldRef);
}

void Field::importMFNode(int position, const std::string &filename) {
  wb_supervisor_field_import_mf_node(fieldRef, position, filename.c_str());
}

void Field::importMFNodeFromString(int position, const std::string &nodeString) {
  wb_supervisor_field_import_mf_node_from_string(fieldRef, position, nodeString.c_str());
}

void Field::importSFNode(const std::string &filename) {
  wb_supervisor_field_import_sf_node(fieldRef, filename.c_str());
}

void Field::importSFNodeFromString(const std::string &nodeString) {
  wb_supervisor_field_import_sf_node_from_string(fieldRef, nodeString.c_str());
}

void Field::removeMFNode(int position) {
  fprintf(stderr, "Field::removeMFNode is deprecated, please use "
                  "Field::removeMF instead\n");
  wb_supervisor_field_remove_mf(fieldRef, position);
}
