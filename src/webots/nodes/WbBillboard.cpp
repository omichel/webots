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

#include "WbBillboard.hpp"
#include "WbTransform.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"

#include <wren/node.h>
#include <wren/transform.h>

WbBillboard::WbBillboard(WbTokenizer *tokenizer) : WbGroup("Billboard", tokenizer) {
}

WbBillboard::WbBillboard(const WbBillboard &other) : WbGroup(other) {
}

WbBillboard::WbBillboard(const WbNode &other) : WbGroup(other) {
}

WbBillboard::~WbBillboard() {
  if (areWrenObjectsInitialized())
    wr_node_delete(WR_NODE(wrenNode()));
}

void WbBillboard::postFinalize() {
  WbGroup::postFinalize();
  const WbViewpoint *viewpoint = WbWorld::instance()->viewpoint();

  connect(viewpoint, &WbViewpoint::cameraParametersChanged, this, &WbBillboard::updatePosition);
}

void WbBillboard::createWrenObjects() {
  WbBaseNode::createWrenObjects();

  WrTransform *transform = wr_transform_new();
  wr_transform_attach_child(wrenNode(), WR_NODE(transform));
  setWrenNode(transform);

  const int size = children().size();
  for (int i = 0; i < size; ++i) {
    WbBaseNode *const n = child(i);
    n->createWrenObjects();
  }

  updatePosition();
}

void WbBillboard::applyTranslationToWren() {
  const WbViewpoint *viewpoint = WbWorld::instance()->viewpoint();

  float translation[3];
  WbSFVector3 *position = viewpoint->position();
  position->value().toFloatArray(translation);

  wr_transform_set_position(wrenNode(), translation);
}

void WbBillboard::applyRotationToWren() {
  const WbViewpoint *viewpoint = WbWorld::instance()->viewpoint();
  WbSFRotation *orientation = viewpoint->orientation();
  float rotation[4];
  orientation->value().toFloatArray(rotation);
  wr_transform_set_orientation(wrenNode(), rotation);
}

void WbBillboard::updatePosition() {
  applyRotationToWren();
  applyTranslationToWren();
}
