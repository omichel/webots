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

#include "WbJoint.hpp"

#include "WbBrake.hpp"
#include "WbJointParameters.hpp"
#include "WbMotor.hpp"
#include "WbNodeUtilities.hpp"
#include "WbPositionSensor.hpp"
#include "WbRobot.hpp"
#include "WbWrenRenderingContext.hpp"

#include <wren/config.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include <ode/ode.h>

void WbJoint::init() {
  mDevice = findMFNode("device");

  // hidden field
  mPosition = findSFDouble("position")->value();
  mOdePositionOffset = mPosition;
  mTimeStep = 0.0;
  mSavedPositions[stateId()] = mPosition;
}

// Constructors

WbJoint::WbJoint(const QString &modelName, WbTokenizer *tokenizer) : WbBasicJoint(modelName, tokenizer) {
  init();
}

WbJoint::WbJoint(const WbJoint &other) : WbBasicJoint(other) {
  init();
}

WbJoint::WbJoint(const WbNode &other) : WbBasicJoint(other) {
  init();
}

WbJoint::~WbJoint() {
}

void WbJoint::downloadAssets() {
  WbBasicJoint::downloadAssets();
  WbMotor *m = motor();
  if (m)
    m->downloadAssets();
  m = motor2();
  if (m)
    m->downloadAssets();
  m = motor3();
  if (m)
    m->downloadAssets();
}

void WbJoint::preFinalize() {
  WbBasicJoint::preFinalize();

  mSavedPositions[stateId()] = mPosition;

  for (int i = 0; i < devicesNumber(); ++i) {
    if (device(i) && !device(i)->isPreFinalizedCalled())
      device(i)->preFinalize();
  }
}

void WbJoint::postFinalize() {
  WbBasicJoint::postFinalize();

  for (int i = 0; i < devicesNumber(); ++i) {
    if (device(i) && !device(i)->isPostFinalizedCalled())
      device(i)->postFinalize();
  }

  connect(mDevice, &WbMFNode::itemInserted, this, &WbJoint::addDevice);
  if (brake())
    connect(brake(), &WbBrake::brakingChanged, this, &WbJoint::updateSpringAndDampingConstants, Qt::UniqueConnection);
}

void WbJoint::reset(const QString &id) {
  WbBasicJoint::reset(id);

  for (int i = 0; i < mDevice->size(); ++i)
    mDevice->item(i)->reset(id);

  setPosition(mSavedPositions[id]);
}

void WbJoint::resetPhysics() {
  updatePosition();

  WbMotor *const m = motor();
  if (m)
    m->resetPhysics();
}

void WbJoint::save(const QString &id) {
  WbBasicJoint::save(id);

  for (int i = 0; i < mDevice->size(); ++i)
    mDevice->item(i)->save(id);

  mSavedPositions[id] = mPosition;
}

void WbJoint::setPosition(double position, int index) {
  if (index != 1)
    return;

  mPosition = position;
  mOdePositionOffset = position;
  WbJointParameters *const p = parameters();
  if (p)
    p->setPosition(mPosition);

  WbMotor *const m = motor();
  if (m)
    m->setTargetPosition(position);
}

bool WbJoint::resetJointPositions() {
  mOdePositionOffset = 0.0;
  return WbBasicJoint::resetJointPositions();
}

// Update methods: they check validity and correct if necessary

void WbJoint::addDevice(int index) {
  const WbSolid *const s = upperSolid();
  if (s) {
    WbRobot *const r = s->robot();
    assert(r);
    WbBaseNode *decendant = dynamic_cast<WbBaseNode *>(mDevice->item(index));
    r->descendantNodeInserted(decendant);
  }
  WbBrake *brake = dynamic_cast<WbBrake *>(mDevice->item(index));
  if (brake)
    connect(brake, &WbBrake::brakingChanged, this, &WbJoint::updateSpringAndDampingConstants, Qt::UniqueConnection);
}

void WbJoint::updateParameters() {
  const WbJointParameters *const p = parameters();
  if (p) {
    mOdePositionOffset = p->position();
    mPosition = mOdePositionOffset;
    connect(p, SIGNAL(positionChanged()), this, SLOT(updatePosition()), Qt::UniqueConnection);
    connect(p, &WbJointParameters::minAndMaxStopChanged, this, &WbJoint::updateMinAndMaxStop, Qt::UniqueConnection);
    connect(p, &WbJointParameters::springAndDampingConstantsChanged, this, &WbJoint::updateSpringAndDampingConstants,
            Qt::UniqueConnection);
    connect(p, &WbJointParameters::axisChanged, this, &WbJoint::updateAxis, Qt::UniqueConnection);
  }
}

// Utility functions

WbJointParameters *WbJoint::parameters() const {
  return dynamic_cast<WbJointParameters *>(mParameters->value());
}

WbJointDevice *WbJoint::device(int index) const {
  if (index >= 0 && mDevice->size() > index)
    return dynamic_cast<WbJointDevice *>(mDevice->item(index));
  else
    return NULL;
}

int WbJoint::devicesNumber() const {
  return mDevice->size();
}

QVector<WbLogicalDevice *> WbJoint::devices() const {
  QVector<WbLogicalDevice *> devices;
  for (int i = 0; i < devicesNumber(); ++i)
    devices.append(device(i));

  return devices;
}

WbMotor *WbJoint::motor() const {
  WbMotor *motor = NULL;
  for (int i = 0; i < mDevice->size(); ++i) {
    motor = dynamic_cast<WbMotor *>(mDevice->item(i));
    if (motor)
      return motor;
  }

  return NULL;
}

WbPositionSensor *WbJoint::positionSensor() const {
  WbPositionSensor *sensor = NULL;
  for (int i = 0; i < mDevice->size(); ++i) {
    sensor = dynamic_cast<WbPositionSensor *>(mDevice->item(i));
    if (sensor)
      return sensor;
  }

  return NULL;
}

WbBrake *WbJoint::brake() const {
  WbBrake *brake = NULL;
  for (int i = 0; i < mDevice->size(); ++i) {
    brake = dynamic_cast<WbBrake *>(mDevice->item(i));
    if (brake)
      return brake;
  }

  return NULL;
}

void WbJoint::updateAxis() {
  // update the current endPoint pose based on the new axis value
  // but do not modify the initial endPoint pose
  updatePosition();

  if (mJoint)
    applyToOdeAxis();

  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_JOINT_AXES))
    updateJointAxisRepresentation();
}

void WbJoint::updateMinAndMaxStop(double min, double max) {
  if (mJoint)
    applyToOdeMinAndMaxStop();
}

WbVector3 WbJoint::axis() const {
  static const WbVector3 DEFAULT_AXIS(0.0, 0.0, 1.0);
  const WbJointParameters *p = parameters();
  return p ? p->axis() : DEFAULT_AXIS;
}

void WbJoint::setOdeJoint(dBodyID body, dBodyID parentBody) {
  WbBasicJoint::setOdeJoint(body, parentBody);

  // compute and set the orientation of rotation axis
  applyToOdeAxis();

  // place hard stops if defined
  applyToOdeMinAndMaxStop();
}

void WbJoint::updateOdePositionOffset() {
  double newValue = position();
  if (mOdePositionOffset == newValue)
    return;

  mOdePositionOffset = newValue;
  applyToOdeMinAndMaxStop();
}

//////////
// WREN //
//////////

void WbJoint::createWrenObjects() {
  WbBasicJoint::createWrenObjects();

  if (WbWrenRenderingContext::instance()->isOptionalRenderingEnabled(WbWrenRenderingContext::VF_JOINT_AXES))
    wr_node_set_visible(WR_NODE(mTransform), true);

  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this,
          &WbJoint::updateJointAxisRepresentation);
  updateJointAxisRepresentation();

  // create Wren objects for Muscle devices
  for (int i = 0; i < devicesNumber(); ++i) {
    if (device(i))
      device(i)->createWrenObjects();
  }
}

void WbJoint::updateJointAxisRepresentation() {
  if (!areWrenObjectsInitialized())
    return;

  wr_static_mesh_delete(mMesh);

  const double scaling = 0.5f * wr_config_get_line_scale();

  const WbVector3 &anchorVector = anchor();
  const WbVector3 &axisVector = scaling * axis();

  WbVector3 vertex(anchorVector - axisVector);
  float vertices[6];
  vertex.toFloatArray(vertices);

  vertex = anchorVector + axisVector;
  vertex.toFloatArray(vertices + 3);

  mMesh = wr_static_mesh_line_set_new(2, vertices, NULL);
  wr_renderable_set_mesh(mRenderable, WR_MESH(mMesh));
}

const QString WbJoint::urdfName() const {
  if (motor())
    return getUrdfPrefix() + motor()->deviceName();
  else if (positionSensor())
    return getUrdfPrefix() + positionSensor()->deviceName();
  return WbBaseNode::urdfName();
}

void WbJoint::writeExport(WbVrmlWriter &writer) const {
  if (writer.isUrdf() && solidEndPoint()) {
    const WbNode *const parentRoot = findUrdfLinkRoot();
    const WbVector3 currentOffset = solidEndPoint()->translation() - anchor();
    const WbVector3 translation = solidEndPoint()->translationFrom(parentRoot) - currentOffset + writer.jointOffset();
    writer.setJointOffset(solidEndPoint()->rotationMatrixFrom(parentRoot).transposed() * currentOffset);
    const WbVector3 rotationEuler = solidEndPoint()->rotationMatrixFrom(parentRoot).toEulerAnglesZYX();
    const WbVector3 rotationAxis = axis() * solidEndPoint()->rotationMatrixFrom(WbNodeUtilities::findUpperTransform(this));

    writer.increaseIndent();
    writer.indent();
    const WbMotor *m = motor();
    if (m && (m->minPosition() != 0.0 || m->maxPosition() != 0.0))
      writer << QString("<joint name=\"%1\" type=\"revolute\">\n").arg(urdfName());
    else
      writer << QString("<joint name=\"%1\" type=\"continuous\">\n").arg(urdfName());

    writer.increaseIndent();
    writer.indent();
    writer << QString("<parent link=\"%1\"/>\n").arg(parentRoot->urdfName());
    writer.indent();
    writer << QString("<child link=\"%1\"/>\n").arg(solidEndPoint()->urdfName());
    writer.indent();
    writer << QString("<axis xyz=\"%1\"/>\n").arg(rotationAxis.toString(WbPrecision::FLOAT_ROUND_6));
    writer.indent();

    if (m) {
      if (m->minPosition() != 0.0 || m->maxPosition() != 0.0)
        writer << QString("<limit effort=\"%1\" lower=\"%2\" upper=\"%3\" velocity=\"%4\"/>\n")
                    .arg(m->maxForceOrTorque())
                    .arg(m->minPosition())
                    .arg(m->maxPosition())
                    .arg(m->maxVelocity());
      else
        writer << QString("<limit effort=\"%1\" velocity=\"%2\"/>\n").arg(m->maxForceOrTorque()).arg(m->maxVelocity());
      writer.indent();
    }
    writer << QString("<origin xyz=\"%1\" rpy=\"%2\"/>\n")
                .arg(translation.toString(WbPrecision::FLOAT_ROUND_6))
                .arg(rotationEuler.toString(WbPrecision::FLOAT_ROUND_6));
    writer.decreaseIndent();

    writer.indent();
    writer << QString("</joint>\n");
    writer.decreaseIndent();

    WbNode::exportNodeSubNodes(writer);
    return;
  }

  WbBasicJoint::writeExport(writer);
}
