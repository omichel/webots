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

#include "WbSolid.hpp"

#include "WbBox.hpp"
#include "WbCapsule.hpp"
#include "WbCylinder.hpp"
#include "WbDamping.hpp"
#include "WbField.hpp"
#include "WbGeometry.hpp"
#include "WbImmersionProperties.hpp"
#include "WbIndexedFaceSet.hpp"
#include "WbJoint.hpp"
#include "WbJointParameters.hpp"
#include "WbLog.hpp"
#include "WbMFNode.hpp"
#include "WbMFVector3.hpp"
#include "WbMassChecker.hpp"
#include "WbMathsUtilities.hpp"
#include "WbMatrix3.hpp"
#include "WbMatrix4.hpp"
#include "WbMatter.hpp"
#include "WbMotor.hpp"
#include "WbNodeUtilities.hpp"
#include "WbOdeContact.hpp"
#include "WbOdeContext.hpp"
#include "WbOdeGeomData.hpp"
#include "WbPhysics.hpp"
#include "WbPlane.hpp"
#include "WbPropeller.hpp"
#include "WbResizeManipulator.hpp"
#include "WbRobot.hpp"
#include "WbRotation.hpp"
#include "WbSimulationState.hpp"
#include "WbSlot.hpp"
#include "WbSolidMerger.hpp"
#include "WbSolidReference.hpp"
#include "WbSphere.hpp"
#include "WbSupportPolygonRepresentation.hpp"
#include "WbToken.hpp"
#include "WbTokenizer.hpp"
#include "WbVector4.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"
#include "WbWorldInfo.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenShaders.hpp"

#include <wren/config.h>
#include <wren/material.h>
#include <wren/node.h>
#include <wren/renderable.h>
#include <wren/scene.h>
#include <wren/static_mesh.h>
#include <wren/transform.h>

#include <ode/fluid_dynamics/ode_fluid_dynamics.h>

#include <QtCore/QQueue>
#include <QtCore/QRegularExpression>
#include <QtCore/QStringList>

using namespace WbHiddenKinematicParameters;
using namespace WbSolidUtilities;
using namespace std;

const double WbSolid::MASS_ZERO_THRESHOLD = 1e-10;

QList<const WbSolid *> WbSolid::cSolids;

void WbSolid::init() {
  // ODE stuff
  mJoint = NULL;
  mOdeMass = NULL;
  mMassAroundCoM = NULL;
  mReferenceMass = NULL;

  // Webots physics data
  mGlobalMass = 0.0;
  mGlobalVolume = 0.0;
  mGlobalCenterOfMass = WbVector3();
  mCenterOfMass = WbVector3();
  mScaledCenterOfMass = WbVector3();

  // Flags
  mWasSleeping = false;
  mBoundingObjectHasChanged = false;
  mSelected = false;
  mHasSearchedRobot = false;
  mHasExtractedContactPoints = false;
  mUseInertiaMatrix = false;
  mIsPermanentlyKinematic = false;
  mIsKinematic = false;
  mUpdatedInStep = false;
  mResetPhysicsInStep = false;
  mKinematicWarningPrinted = false;
  mHasDynamicSolidDescendant = false;

  // Merger
  mSolidMerger = NULL;
  mMergerIsSet = false;

  // store position
  // Note: this cannot be put into the preFinalize function because
  //       of the copy constructor last initialization
  mSavedTranslations[stateId()] = translation();
  mSavedRotations[stateId()] = rotation();

  // Support polygon representation
  mY = numeric_limits<double>::max();
  mSupportPolygon = WbPolygon();
  mSupportPolygonNeedsUpdate = false;
  mSupportPolygonRepresentationIsEnabled = false;
  mSupportPolygonRepresentation = NULL;

  // Center of mass representation
  mCenterOfMassTransform = NULL;
  mCenterOfMassMaterial = NULL;
  mCenterOfMassMesh = NULL;
  mCenterOfMassRenderable = NULL;

  // Global center of mass representation
  mGlobalCenterOfMassRepresentationIsEnabled = false;
  mGlobalCenterOfMassTransform = NULL;
  mGlobalCenterOfMassMaterial = NULL;
  mGlobalCenterOfMassMesh = NULL;
  mGlobalCenterOfMassRenderable = NULL;

  // Center of buoyancy representation
  mCenterOfBuoyancyRepresentationIsEnabled = false;
  mHasExtractedImmersions = false;
  mCenterOfBuoyancyTransform = NULL;
  mCenterOfBuoyancyMaterial = NULL;
  mCenterOfBuoyancyRenderable = NULL;

  // user fields
  mContactMaterial = findSFString("contactMaterial");
  mImmersionProperties = findMFNode("immersionProperties");
  mBoundingObject = findSFNode("boundingObject");
  mPhysics = findSFNode("physics");
  mRadarCrossSection = findSFDouble("radarCrossSection");
  mRecognitionColors = findMFColor("recognitionColors");

  // hidden fields
  mLinearVelocity = findSFVector3("linearVelocity");
  mAngularVelocity = findSFVector3("angularVelocity");

  if (mLinearVelocity && mAngularVelocity) {
    updateIsLinearVelocityNull();
    updateIsAngularVelocityNull();
    connect(mLinearVelocity, &WbSFVector3::changed, this, &WbSolid::updateIsLinearVelocityNull);
    connect(mAngularVelocity, &WbSFVector3::changed, this, &WbSolid::updateIsAngularVelocityNull);
  }

  mOriginalHiddenKinematicParameters = NULL;
}

WbSolid::WbSolid(WbTokenizer *tokenizer) : WbMatter("Solid", tokenizer) {
  init();
}

WbSolid::WbSolid(const WbSolid &other) : WbMatter(other) {
  init();
}

WbSolid::WbSolid(const WbNode &other) : WbMatter(other) {
  init();
}

WbSolid::WbSolid(const QString &modelName, WbTokenizer *tokenizer) : WbMatter(modelName, tokenizer) {
  init();
}

WbSolid::~WbSolid() {
  if (mRadarCrossSection->value() > 0.0)
    WbWorld::instance()->removeRadarTarget(this);

  if (!mRecognitionColors->isEmpty())
    WbWorld::instance()->removeCameraRecognitionObject(this);

  qDeleteAll(mHiddenKinematicParametersMap);
  mHiddenKinematicParametersMap.clear();

  cSolids.removeAll(this);

  // Cleanup WREN
  if (areWrenObjectsInitialized()) {
    // Center of mass
    wr_node_delete(WR_NODE(mCenterOfMassTransform));
    wr_node_delete(WR_NODE(mCenterOfMassRenderable));
    wr_material_delete(mCenterOfMassMaterial);
    wr_static_mesh_delete(mCenterOfMassMesh);

    // Global center of mass
    wr_node_delete(WR_NODE(mGlobalCenterOfMassTransform));
    wr_node_delete(WR_NODE(mGlobalCenterOfMassRenderable));
    wr_material_delete(mGlobalCenterOfMassMaterial);
    wr_static_mesh_delete(mGlobalCenterOfMassMesh);

    // Center of buoyancy
    wr_node_delete(WR_NODE(mCenterOfBuoyancyTransform));
    wr_node_delete(WR_NODE(mCenterOfBuoyancyRenderable));
    wr_material_delete(mCenterOfBuoyancyMaterial);
  }

  delete mSupportPolygonRepresentation;
  mSupportPolygonRepresentation = NULL;

  if (isSolidMerger())
    delete mSolidMerger.data();

  if (!mSolidMerger.isNull())
    mSolidMerger->removeSolid(this);

  // cleanup ODE
  delete mReferenceMass;
  mReferenceMass = NULL;
  delete mOdeMass;
  mOdeMass = NULL;
  delete mMassAroundCoM;
  mMassAroundCoM = NULL;
  if (mJoint)
    dJointDestroy(mJoint);
  mJoint = NULL;

  // disconnecting descendants
  foreach (WbSolid *const solid, mSolidChildren)
    disconnect(solid, &WbSolid::destroyed, this, 0);
}

void WbSolid::validateProtoNode() {
  if (isProtoInstance()) {
    bool checkTranslation = !isTranslationFieldVisible();
    bool checkRotation = !isRotationFieldVisible();
    if (!(checkTranslation || checkRotation))
      return;

    foreach (WbField *parameter, parameters()) {
      if (checkTranslation && parameter->name() == "translation" && parameter->isTemplateRegenerator()) {
        parsingWarn(tr("template regenerator field named 'translation' found. "
                       "It is recommended not to use template statements to update the top Solid 'translation' field"));
        checkTranslation = false;
      } else if (checkRotation && parameter->name() == "rotation" && parameter->isTemplateRegenerator()) {
        parsingWarn(tr("template regenerator field named 'rotation' found. "
                       "It is recommended not to use template statements to update the top Solid 'rotation' field"));
        checkTranslation = false;
      }

      if (!(checkTranslation || checkRotation))
        break;
    }
  }
}

void WbSolid::preFinalize() {
  mHasNoSolidAncestor = false;

  cSolids << this;

  updateChildren();

  WbMatter::preFinalize();

  if (physics())
    physics()->preFinalize();
  else
    mIsKinematic = true;

  if (mBoundingObject->value())
    boundingObject()->preFinalize();

  assert(mImmersionProperties);
  WbMFNode::Iterator it(*mImmersionProperties);
  while (it.hasNext()) {
    WbBaseNode *const ip = static_cast<WbImmersionProperties *>(it.next());
    ip->preFinalize();
  }

  setMatrixNeedUpdate();  // force the matrix update after the first ode update

  // needed to be done before createOdeObjects
  // because of the SolidMerger
  mOdeMass = new dMass;  // stores inertia and CoM relative to solid center in the local frame coordinates
  dMassSetZero(mOdeMass);
  mMassAroundCoM = new dMass;  // stores inertia and CoM relative to solid center in the local frame coordinates
  dMassSetZero(mMassAroundCoM);
  mReferenceMass = new dMass;
  dMassSetZero(mReferenceMass);
  mIsPermanentlyKinematic = WbSolidUtilities::isPermanentlyKinematic(this);  // cached because it can't be called in destructor
  if (mIsPermanentlyKinematic)
    mIsKinematic = true;

  // Overwrites loaded values with hidden field and hidden parameter values (translation, rotation, joint position), reads
  // initial velocities
  if (isProtoInstance() && !isNestedProtoNode()) {
    int counter = 0;
    if (!restoreHiddenKinematicParameters(mHiddenKinematicParametersMap, counter)) {
      bool success = resetHiddenKinematicParameters();
      if (success)
        WbLog::instance()->warning(tr("PROTO '%1' changed after this world was saved:\n"
                                      "hidden parameters have been automatically reset.\n\n"
                                      "Please save the current world to get rid of this message.")
                                     .arg(modelName()),
                                   true);
      else
        WbLog::instance()->warning(tr("PROTO '%1' changed after this world was saved:\n"
                                      "hidden parameters cannot be loaded correctly.\n\n"
                                      "Please save the current world to get rid of this message.")
                                     .arg(modelName()),
                                   true);
      qDeleteAll(mHiddenKinematicParametersMap);
      mHiddenKinematicParametersMap.clear();
    }
  }

  checkScaleAtLoad(true);
  if (nodeType() != WB_NODE_TOUCH_SENSOR && mBoundingObject->value() && mPhysics->value() == NULL &&
      mJointParents.size() == 0 && upperSolid() && upperSolid()->physics())
    parsingWarn(tr("As 'physics' is set to NULL, collisions will have no effect"));
}

bool WbSolid::restoreHiddenKinematicParameters(const HiddenKinematicParametersMap &map, int &counter) {
  if (!applyHiddenKinematicParameters(map.value(counter, NULL), true))
    return false;

  ++counter;

  foreach (WbSolid *const solid, mSolidChildren) {
    if (!solid->restoreHiddenKinematicParameters(map, counter))
      return false;
  }

  return true;
}

bool WbSolid::resetHiddenKinematicParameters() {
  foreach (WbSolid *const solid, mSolidChildren) {
    if (!solid->resetHiddenKinematicParameters())
      return false;
  }

  if (mOriginalHiddenKinematicParameters)
    return applyHiddenKinematicParameters(mOriginalHiddenKinematicParameters, false);

  return true;
}

bool WbSolid::applyHiddenKinematicParameters(const HiddenKinematicParameters *hkp, bool backupPrevious) {
  if (!hkp)
    return true;

  WbVector3 *previousT = NULL;
  WbRotation *previousR = NULL;
  WbVector3 *previousL = NULL;
  WbVector3 *previousA = NULL;
  PositionMap *previousP = NULL;

  const WbVector3 *const t = hkp->translation();
  if (t) {
    if (backupPrevious)
      previousT = new WbVector3(translation());
    WbTransform::setTranslation(*t);
  }

  const WbRotation *const r = hkp->rotation();
  if (r) {
    if (backupPrevious)
      previousR = new WbRotation(rotation());
    WbTransform::setRotation(*r);
  }

  const PositionMap *const m = hkp->positions();
  if (m) {
    if (backupPrevious)
      previousP = new PositionMap();

    const PositionMap::const_iterator end = m->constEnd();
    for (PositionMap::const_iterator i = m->constBegin(); i != end; ++i) {
      const WbVector3 *const p = i.value();
      if (!p)
        return false;
      const int jointIndex = i.key();
      WbJoint *const joint = dynamic_cast<WbJoint *>(mJointChildren.at(jointIndex));
      if (!joint)
        return false;

      if (backupPrevious) {
        WbVector3 v(NAN, NAN, NAN);
        const WbJointParameters *const param1 = joint->parameters();
        if (param1)
          v[0] = joint->position();
        const WbJointParameters *const param2 = joint->parameters2();
        if (param2)
          v[1] = joint->position(2);
        const WbJointParameters *const param3 = joint->parameters3();
        if (param3)
          v[2] = joint->position(3);
        previousP->insert(jointIndex, new WbVector3(v));
      }

      for (int j = 0; j < 3; ++j) {
        const double posj = (*p)[j];
        if (!std::isnan(posj))
          joint->setPosition(posj, j + 1);
      }
    }
  }

  const WbVector3 *const l = hkp->linearVelocity();
  if (l) {
    if (backupPrevious)
      previousL = new WbVector3(mLinearVelocity->value());
    mLinearVelocity->setValue(*l);
  }

  const WbVector3 *const a = hkp->angularVelocity();
  if (a) {
    if (backupPrevious)
      previousA = new WbVector3(mAngularVelocity->value());
    mAngularVelocity->setValue(*a);
  }

  if (backupPrevious && (previousT || previousR || previousP || previousL || previousA)) {
    delete mOriginalHiddenKinematicParameters;
    mOriginalHiddenKinematicParameters = new HiddenKinematicParameters(previousT, previousR, previousP, previousL, previousA);
  }

  return true;
}

void WbSolid::postFinalize() {
  delete mOriginalHiddenKinematicParameters;
  mOriginalHiddenKinematicParameters = NULL;

  WbMatter::postFinalize();
  if (physics())
    physics()->postFinalize();

  WbMFNode::Iterator it(*mImmersionProperties);
  while (it.hasNext()) {
    WbBaseNode *const ip = static_cast<WbImmersionProperties *>(it.next());
    ip->postFinalize();
  }

  updateDynamicSolidDescendantFlag();

  connect(mTranslation, &WbSFVector3::changedByUser, this, &WbSolid::resetPhysicsIfRequired);
  connect(mRotation, &WbSFVector3::changedByUser, this, &WbSolid::resetPhysicsIfRequired);

  disconnectFieldNotification(rotationFieldValue());
  disconnectFieldNotification(translationFieldValue());
  connect(WbSimulationState::instance(), &WbSimulationState::modeChanged, this, &WbSolid::onSimulationModeChanged);
  connect(WbSimulationState::instance(), &WbSimulationState::renderingStateChanged, this, &WbSolid::onSimulationModeChanged);
  connect(this, &WbSolid::massPropertiesChanged, this, &WbSolid::displayWarning);
  connect(mPhysics, &WbSFNode::changed, this, &WbSolid::updatePhysics);
  connect(mRadarCrossSection, &WbSFDouble::changed, this, &WbSolid::updateRadarCrossSection);
  connect(mRecognitionColors, &WbMFColor::itemRemoved, this, &WbSolid::updateRecognitionColors);
  connect(mRecognitionColors, &WbMFColor::itemInserted, this, &WbSolid::updateRecognitionColors);

  if (isTopSolid()) {
    updateGlobalCenterOfMass();
    updateGlobalVolume();
  }

  displayWarning();

  if (mRadarCrossSection->value() > 0.0)
    WbWorld::instance()->addRadarTarget(this);

  if (!mRecognitionColors->isEmpty())
    WbWorld::instance()->addCameraRecognitionObject(this);

  if (protoParameterNode()) {
    const QVector<WbNode *> nodes = protoParameterNode()->protoParameterNodeInstances();
    if (nodes.size() > 1 && nodes.at(0) == this)
      parsingWarn(tr("Solid node defined in PROTO field is used multiple times. "
                     "Webots doesn't fully support this because the multiple node instances cannot be identical."));
  }
}

void WbSolid::resolveNameClashIfNeeded(bool automaticallyChange, bool recursive, const QList<WbSolid *> siblings,
                                       QSet<const QString> *topSolidNameSet) const {
  const QString &warningText =
    tr("'name' field value should be unique: '%1' already used by a sibling Solid node.").arg(name());

  if (isProtoParameterNode() || siblings.isEmpty())
    return;

  if (topSolidNameSet && !automaticallyChange) {
    if (topSolidNameSet->contains(name()))
      parsingWarn(warningText);
    else
      topSolidNameSet->insert(name());
  } else {
    QList<int> indices;
    // extract name without index
    QRegularExpression re("(.+)\\(\\d+\\)$");
    QRegularExpressionMatch match = re.match(name());
    QString nameWithoutIndex(name());
    if (match.hasMatch())
      nameWithoutIndex = match.captured(1);

    // loop through sibling nodes
    const WbNode *parameterNode = protoParameterNode();
    while (parameterNode && parameterNode->protoParameterNode())
      parameterNode = parameterNode->protoParameterNode();

    bool found = false;
    re.setPattern(QString("%1\\((\\d+)\\)").arg(QRegularExpression::escape(nameWithoutIndex)));
    foreach (WbSolid *s, siblings) {
      if (!s || s == this)
        continue;

      const bool matchingName = s->name() == name();
      found |= matchingName;
      if (matchingName) {
        if (parameterNode != NULL) {
          // ensure that solid nodes doesn't refer to the same PROTO parameter node
          // otherwise we will loop forever
          const WbNode *otherParameterNode = s->protoParameterNode();
          while (otherParameterNode && otherParameterNode->protoParameterNode())
            otherParameterNode = otherParameterNode->protoParameterNode();
          if (otherParameterNode == parameterNode) {
            parsingWarn(
              warningText +
              tr(" A unique name cannot be automatically generated because the same PROTO parameter is used multiple times."));
            goto recursion;
          }
        }
      }
      match = re.match(s->name());
      if (match.hasMatch()) {
        indices << match.captured(1).toInt();
      }
    }

    if (found) {
      if (automaticallyChange) {
        WbField *nameField = findField("name", true);
        while (nameField->parameter())
          nameField = nameField->parameter();
        bool isTemplateRegenerator = nameField->isTemplateRegenerator();
        if (isTemplateRegenerator)
          parsingWarn(warningText +
                      tr(" A unique name cannot be automatically generated because 'name' is a template regenerator field."));
        else if (!WbNodeUtilities::isVisible(findField("name")))
          parsingWarn(warningText);
        else {
          // find first available index
          std::sort(indices.begin(), indices.end());
          int newIndex = 1;
          foreach (int i, indices) {
            if (i != newIndex)
              break;
            newIndex++;
          }
          QString newName = QString("%1(%2)").arg(nameWithoutIndex).arg(newIndex);
          mName->setValue(newName);
        }
      } else
        parsingWarn(warningText);
    }
  }

recursion:
  if (recursive) {
    QList<WbSolid *> solidChildrenList = mSolidChildren.toList();
    foreach (WbSolid *s, solidChildrenList)
      s->resolveNameClashIfNeeded(automaticallyChange, recursive, solidChildrenList, NULL);
  }
}

void WbSolid::updateName() {
  const WbSolid *us = upperSolid();
  resolveNameClashIfNeeded(false, false, us ? us->solidChildren().toList() : WbWorld::instance()->topSolids(), NULL);
  WbMatter::updateName();
}

QString WbSolid::computeUniqueName() const {
  const WbSolid *solid = this;
  QString uniqueName;
  while (true) {
    QString name = solid->name();
    name.replace("\\", "\\\\");  // escape '\'
    name.replace(":", "\\:");    // escape ':'
    uniqueName.prepend(name);
    solid = solid->upperSolid();
    if (solid)
      uniqueName.prepend(":");
    else
      break;
  }
  return uniqueName;
}

WbSolid *WbSolid::findDescendantSolidFromUniqueName(QStringList &names) const {
  const WbSolid *solid = this;
  while (solid && !names.isEmpty()) {
    QString name = names.takeFirst();
    name.replace("\\:", ":");    // revert escape of ':'
    name.replace("\\\\", "\\");  // revert escape of '\'
    WbSolid *nextSolid = NULL;
    foreach (WbSolid *s, solid->mSolidChildren) {
      if (s->name() == name) {
        if (names.isEmpty())
          return s;
        nextSolid = s;
      }
    }
    solid = nextSolid;
  }
  return NULL;
}

WbSolid *WbSolid::findSolidFromUniqueName(const QString &name) {
  // Solid names joined by ':'
  QStringList names = splitUniqueNamesByEscapedPattern(name, ":");
  QString topName = names.takeFirst();
  topName.replace("\\:", ":");    // revert escape of ':'
  topName.replace("\\\\", "\\");  // revert escape of '\'
  foreach (WbSolid *solid, WbWorld::instance()->topSolids()) {
    if (solid->name() == topName)
      return names.isEmpty() ? solid : solid->findDescendantSolidFromUniqueName(names);
  }
  return NULL;
}

QStringList WbSolid::splitUniqueNamesByEscapedPattern(const QString &text, const QString &pattern) {
  QStringList result;
  // To check that the pattern matched and the first character is not escaped, given that
  // the name can end with '\' (i.e. '\\' because was escaped when writing), we have to
  // check that ':' is preceeded by zero or an even number of '\\'
  QRegularExpression re("[^\\\\](\\\\\\\\)*" + pattern);
  QRegularExpressionMatch match = re.match(text);
  int startIndex = 0;
  while (match.hasMatch()) {
    int capturedStart = match.capturedStart() + match.capturedLength() - pattern.size();
    result << text.mid(startIndex, capturedStart - startIndex);
    startIndex = match.capturedEnd();
    match = re.match(text, startIndex);
  }
  result << text.mid(startIndex, text.size() - startIndex);
  return result;
}

/////////////////////////
// Create WREN Objects //
/////////////////////////

void WbSolid::createWrenObjects() {
  // Center of mass representation
  const float centerOfMassMeshVertices[18] = {-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f, -0.5f, 0.0f,
                                              0.0f,  1.0f, 0.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f,  1.0f};
  const float colors[18] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
  mCenterOfMassMesh = wr_static_mesh_line_set_new(6, centerOfMassMeshVertices, colors);
  mCenterOfMassMaterial = wr_phong_material_new();
  wr_phong_material_set_color_per_vertex(mCenterOfMassMaterial, true);
  wr_phong_material_set_transparency(mCenterOfMassMaterial, 0.5f);
  wr_material_set_default_program(mCenterOfMassMaterial, WbWrenShaders::lineSetShader());

  mCenterOfMassRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mCenterOfMassRenderable, false);
  wr_renderable_set_receive_shadows(mCenterOfMassRenderable, false);
  wr_renderable_set_material(mCenterOfMassRenderable, mCenterOfMassMaterial, NULL);
  wr_renderable_set_mesh(mCenterOfMassRenderable, WR_MESH(mCenterOfMassMesh));
  wr_renderable_set_visibility_flags(mCenterOfMassRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  wr_renderable_set_drawing_mode(mCenterOfMassRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_drawing_order(mCenterOfMassRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);

  mCenterOfMassTransform = wr_transform_new();
  wr_node_set_visible(WR_NODE(mCenterOfMassTransform), false);
  wr_transform_attach_child(mCenterOfMassTransform, WR_NODE(mCenterOfMassRenderable));

  // Global center of mass representation
  const float globalCenterOfMassVertices[18] = {
    -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
  };

  mGlobalCenterOfMassMesh = wr_static_mesh_line_set_new(6, globalCenterOfMassVertices, NULL);

  mGlobalCenterOfMassMaterial = wr_phong_material_new();
  const float color[3] = {0.05f, 0.05f, 0.5f};
  wr_phong_material_set_color(mGlobalCenterOfMassMaterial, color);
  wr_phong_material_set_transparency(mGlobalCenterOfMassMaterial, 0.7f);
  wr_material_set_default_program(mGlobalCenterOfMassMaterial, WbWrenShaders::lineSetShader());

  mGlobalCenterOfMassRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mGlobalCenterOfMassRenderable, false);
  wr_renderable_set_receive_shadows(mGlobalCenterOfMassRenderable, false);
  wr_renderable_set_material(mGlobalCenterOfMassRenderable, mGlobalCenterOfMassMaterial, NULL);
  wr_renderable_set_mesh(mGlobalCenterOfMassRenderable, WR_MESH(mGlobalCenterOfMassMesh));
  wr_renderable_set_visibility_flags(mGlobalCenterOfMassRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  wr_renderable_set_drawing_mode(mGlobalCenterOfMassRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_drawing_order(mGlobalCenterOfMassRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);

  mGlobalCenterOfMassTransform = wr_transform_new();
  wr_node_set_visible(WR_NODE(mGlobalCenterOfMassTransform), false);
  wr_transform_attach_child(mGlobalCenterOfMassTransform, WR_NODE(mGlobalCenterOfMassRenderable));

  WbMatter::createWrenObjects();

  wr_transform_attach_child(wrenNode(), WR_NODE(mCenterOfMassTransform));

  // Global center of mass is attached to root node as it is already in absolute coordinates and also it is easier to handle.
  wr_transform_attach_child(wr_scene_get_root(wr_scene_get_instance()), WR_NODE(mGlobalCenterOfMassTransform));

  // Center of buoyancy
  mCenterOfBuoyancyMaterial = wr_phong_material_new();
  const float centerOfBuoyancyColor[3] = {0.5f, 0.5f, 0.8f};
  wr_phong_material_set_color(mCenterOfBuoyancyMaterial, centerOfBuoyancyColor);
  wr_phong_material_set_transparency(mCenterOfBuoyancyMaterial, 0.7f);
  wr_material_set_default_program(mCenterOfBuoyancyMaterial, WbWrenShaders::lineSetShader());
  mCenterOfBuoyancyRenderable = wr_renderable_new();
  wr_renderable_set_cast_shadows(mCenterOfBuoyancyRenderable, false);
  wr_renderable_set_receive_shadows(mCenterOfBuoyancyRenderable, false);
  wr_renderable_set_mesh(mCenterOfBuoyancyRenderable, WR_MESH(mGlobalCenterOfMassMesh));
  wr_renderable_set_material(mCenterOfBuoyancyRenderable, mCenterOfBuoyancyMaterial, NULL);
  wr_renderable_set_drawing_order(mCenterOfBuoyancyRenderable, WR_RENDERABLE_DRAWING_ORDER_AFTER_1);
  wr_renderable_set_drawing_mode(mCenterOfBuoyancyRenderable, WR_RENDERABLE_DRAWING_MODE_LINES);
  wr_renderable_set_visibility_flags(mCenterOfBuoyancyRenderable, WbWrenRenderingContext::VF_INVISIBLE_FROM_CAMERA);
  mCenterOfBuoyancyTransform = wr_transform_new();
  wr_node_set_visible(WR_NODE(mCenterOfBuoyancyTransform), false);
  wr_transform_attach_child(mCenterOfBuoyancyTransform, WR_NODE(mCenterOfBuoyancyRenderable));
  wr_transform_attach_child(wr_scene_get_root(wr_scene_get_instance()), WR_NODE(mCenterOfBuoyancyTransform));

  // Connects signals for further updates
  connect(WbWrenRenderingContext::instance(), &WbWrenRenderingContext::lineScaleChanged, this, &WbSolid::updateLineScale);

  updateLineScale();
  applyChangesToWren();
}

////////////////////////////
//   Create ODE Objects   //
////////////////////////////

void WbSolid::setSolidMerger() {
  if (mIsKinematic) {
    mSolidMerger = NULL;
    return;
  }

  const WbSolid *const us = jointParent() ? NULL : upperSolid();
  const bool inherit = us && us->physics() && name().compare("right wheel", Qt::CaseInsensitive) != 0 &&
                       name().compare("left wheel", Qt::CaseInsensitive) != 0;
  mSolidMerger = inherit ? us->solidMerger() : QPointer<WbSolidMerger>(new WbSolidMerger(this));
}

void WbSolid::setJointParents() {
  // TouchSensor special joint or fixed joint to static environment
  setOdeJointToUpperSolid();

  // new joints
  typedef QList<WbBasicJoint *>::const_iterator LCI;
  LCI end = mJointParents.constEnd();
  for (LCI it = mJointParents.constBegin(); it != end; ++it)
    (*it)->setJoint();
}

void WbSolid::setupSolidMerger() {
  // Detaches the solid if it was previously merged
  if (isSolidMerger()) {
    setJointParents();
    return;
  }

  if (mSolidMerger)
    mSolidMerger->removeSolid(this);

  // Sets the new solid merger
  setSolidMerger();

  dGeomID g = odeGeom();
  if (boundingObject() && g == NULL)
    createOdeGeoms();
  else if (g && mSolidMerger)
    mSolidMerger->addGeomToSpace(g);

  if (mSolidMerger) {
    assert(isDynamic());  // At this point mSolidMerger == NULL if mIsKinematic == false
    if (mOdeMass->mass == 0.0)
      createOdeMass();
    mSolidMerger->appendSolid(this);
    // Recursively assigns the WbSolid body to every non-space ODE dGeom
    g = odeGeom();
    if (g)
      mSolidMerger->attachGeomsToBody(g);
    if (mSolidMerger->isSet())
      mSolidMerger->mergeMass(this, false);
  }
}

// Recursive method that sets solid mergers, creates masses and attaches dGeoms from top to bottom
void WbSolid::setupSolidMergers() {
  setupSolidMerger();
  // Recurses through all first level solid descendants
  foreach (WbSolid *const solid, mSolidChildren)
    solid->setupSolidMergers();

  mMergerIsSet = true;
}

// Recursive method that sets children joints with referenced endpoints
void WbSolid::setJointChildrenWithReferencedEndpoint() {
  foreach (WbBasicJoint *const joint, mJointChildren)
    if (joint->solidReference()) {
      joint->updateEndPoint();
      joint->setJoint();
    }

  foreach (WbSolid *const solid, mSolidChildren)
    solid->setJointChildrenWithReferencedEndpoint();
}

void WbSolid::createOdeObjects() {
  if (boundingObject())
    boundingObject()->createOdeObjects();

  if (isTopLevel() || !mMergerIsSet) {  // the second condition is for newly inserted solids only
    setupSolidMergers();                // this recursion sets solid mergers but also creates dGeoms, dMasses
    setBodiesAndJointsToParents();      // this recursion sets bodies positions and joints to parents
    setJointChildrenWithReferencedEndpoint();
  }

  // Recurses through solid descendants
  WbTransform::createOdeObjects();
}

// Sets recursively every ODE object which was not set during solid merger settings, i.e. bodies and joints to parents
void WbSolid::setBodiesAndJointsToParents() {
  assert(mMergerIsSet);
  if (isDynamic()) {
    if (isSolidMerger())
      mSolidMerger->setupOdeBody();
    const WbPhysics *const p = physics();
    connect(p, &WbPhysics::massOrDensityChanged, this, &WbSolid::updateOdeMass, Qt::UniqueConnection);
    connect(p, &WbPhysics::massOrDensityChanged, WbMassChecker::instance(), &WbMassChecker::checkMasses, Qt::UniqueConnection);
    connect(p, &WbPhysics::centerOfMassChanged, this, &WbSolid::updateOdeCenterOfMass, Qt::UniqueConnection);
    connect(p, &WbPhysics::inertialPropertiesChanged, this, &WbSolid::updateOdeInertiaMatrix, Qt::UniqueConnection);
    connect(p, &WbPhysics::dampingChanged, this, &WbSolid::updateOdeDamping, Qt::UniqueConnection);
  } else
    updateOdeGeomPosition();  // for kinematic solids

  // Recurses through solid descendants
  foreach (WbSolid *const solid, mSolidChildren)
    solid->setBodiesAndJointsToParents();

  WbBasicJoint *const pj = jointParent();
  if (pj)
    pj->updateAfterParentPhysicsChanged();  // needed also in kinematic mode

  if (isSolidMerger()) {
    // Sets joints
    setOdeJointToUpperSolid();

    // Sets 'new' joints
    if (pj)
      pj->setJoint();
  }
}

// Reset ODE joints (with no position offset) for every solid linked to this one
void WbSolid::resetJointsToLinkedSolids() {
  assert(mMergerIsSet);
  resetJointPositions(true);

  foreach (WbSolid *const solid, mSolidChildren)
    solid->resetJointPositions(true);

  foreach (WbBasicJoint *const joint, mJointChildren)
    if (joint->solidReference())
      joint->resetJointPositions();
}

// Reset ODE joints (with no position offset) for every solid linked to this one or to one of its descendants
void WbSolid::resetJoints() {
  if (isSolidMerger())
    resetJointPositions(true);

  foreach (WbSolid *const solid, mSolidChildren)
    solid->resetJoints();
}

void WbSolid::createOdeGeoms() {
  assert(odeGeom() == NULL);
  dSpaceID space = mSolidMerger ? mSolidMerger->reservedSpace() : WbOdeContext::instance()->space();
  createOdeGeomFromBoundingObject(space);
}

void WbSolid::createOdeGeomFromInsertedGroupItem(WbBaseNode *node) {
  assert(node);

  dSpaceID space = upperSpace();
  assert(space);

  dGeomID insertedGeom = createOdeGeomFromNode(space, node);
  if (!insertedGeom)  // if the inserted node has no Geometry child or it has an indexed face set which is invalid
    return;

  if (isDynamic()) {
    assert(mSolidMerger);
    addMass(node);
    // Attaches the dGeom to merger's body and adjusts the mass of both solid and solid merger
    dGeomSetBody(insertedGeom, mSolidMerger->body());
    adjustOdeMass();
  } else
    updateOdeGeomPosition(insertedGeom);
}

// Methods modifying the mass
void WbSolid::updateTopSolidGlobalMass() const {
  WbSolid *const ts = topSolid();
  if (ts && ts->isPostFinalizedCalled()) {
    ts->updateGlobalCenterOfMass();
    ts->updateGlobalVolume();
  }
}

// Method correcting the ODE dMass of the WbSolid after insertion or deletion of a bounding WbGeometry; it is also called when
// the density and the mass field change
void WbSolid::adjustOdeMass(bool mergeMass) {
  if (mSolidMerger == NULL) {
    updateTopSolidGlobalMass();
    emit massPropertiesChanged();
    return;
  }

  const double currentMass = mReferenceMass->mass;

  updateCenterOfMass();

  if (currentMass <= MASS_ZERO_THRESHOLD) {
    dMassSetZero(mReferenceMass);  // clears possible roundoff errors (float addition/subtraction are not associative)
    if (mergeMass && physics()->mode() == WbPhysics::BOUNDING_OBJECT_BASED) {
      // makes sure ODE is fed with a (non-zero) suitable mass
      // deactivate warning when the mass is temporarily zeroed during a dictionary update
      setDefaultMassSettings(true, !WbNode::cUpdatingDictionary);
      mSolidMerger->mergeMass(this);

      // set mass displayed in the Solid's mass tab
      dMassSetParameters(mMassAroundCoM, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
      mMassAroundCoM->c[0] = mScaledCenterOfMass.x();
      mMassAroundCoM->c[1] = mScaledCenterOfMass.y();
      mMassAroundCoM->c[2] = mScaledCenterOfMass.z();
      updateTopSolidGlobalMass();
      emit massPropertiesChanged();
    }
    return;
  }

  if (mUseInertiaMatrix) {  // true when the two inertia 3D-fields are filled and the CoM is also specified
    setOdeInertiaMatrix();
  } else {
    memcpy(mOdeMass, mReferenceMass, sizeof(dMass));
    const WbPhysics *const p = physics();
    const double fieldMass = p->mass();
    const bool fieldMassIsPositive = fieldMass > 0.0;
    const double s = absoluteScale().x();

    if (fieldMassIsPositive) {
      const double s2 = s * s;
      dMassAdjust(mOdeMass, s * s2 * fieldMass);
    } else {
      const double density = p->density();
      if (density >= 0.0)
        dMassAdjust(mOdeMass, (currentMass * density) / 1000.0);
    }

    memcpy(mMassAroundCoM, mOdeMass, sizeof(dMass));

    // Translate the mass to solid's origin
    if (p->centerOfMass().size() == 1) {
      dVector3 t;
      dSubtractVectors3(t, scaledCenterOfMass().ptr(), mReferenceMass->c);
      dMassTranslate(mOdeMass, t[0], t[1], t[2]);
    }
  }

  // merge mass and reset body, geoms and joints starting from and pointing to this solid
  if (mergeMass)
    mSolidMerger->mergeMass(this);

  // Adjust the global center of mass
  updateTopSolidGlobalMass();
  updateGraphicalGlobalCenterOfMass();
  emit massPropertiesChanged();
}

void WbSolid::addMassFromInsertedNode(WbBaseNode *node) {  // node is a WbGeometry or a WbTransform
  assert(isDynamic() && mSolidMerger);
  addMass(node);
  adjustOdeMass();
}

void WbSolid::subtractOdeMass(const dMass *mass, bool adjustSolidMass) {
  if (mIsKinematic)
    return;

  // Modifies the Solid reference mass (based on boundingObject)
  double m1000 = mReferenceMass->mass;
  double m = mass->mass;
  const double massDifference = m1000 - m;
  if (massDifference <= MASS_ZERO_THRESHOLD)
    dMassSetZero(mReferenceMass);
  else {
    const double r = 1.0 / massDifference;
    m *= r;
    m1000 *= r;
    dAddScaledVectors3(mReferenceMass->c, mReferenceMass->c, mass->c, m1000, -m);
    WbSolidUtilities::subtractInertiaMatrix(mReferenceMass->I, mass->I);
    mReferenceMass->mass = massDifference;
  }

  // Modifies the Solid mass in use
  if (adjustSolidMass) {
    adjustOdeMass();
    awake();
  }
}

void WbSolid::correctOdeMass(const dMass *mass, WbBaseNode *node, bool adjustSolidMass) {
  if (mIsKinematic)
    return;

  subtractOdeMass(mass, false);
  addMass(node);

  if (adjustSolidMass) {  // default case
    adjustOdeMass();
    awake();
  }
}

void WbSolid::removeBoundingGeometry() {
  if (isBeingDeleted())
    return;

  const WbGeometry *const geometry = dynamic_cast<WbGeometry *>(sender());
  const dMass *const dmass = geometry->odeMass();

  if (isDynamic() && dmass->mass > 0.0) {
    // modify the mass only if it is computed by means of the boundingObject
    subtractOdeMass(dmass, physics()->mode() == WbPhysics::BOUNDING_OBJECT_BASED);
  }
}

// This is the default joint creation behavior
// this method is overridden in the WbTouchSensor class
dJointID WbSolid::createJoint(dBodyID body, dBodyID parentBody, dWorldID world) const {
  dJointID joint = dJointCreateFixed(world, 0);

  setJoint(joint, body, parentBody);

  return joint;
}

void WbSolid::setJoint(dJointID joint, dBodyID body, dBodyID parentBody) const {
  dJointAttach(joint, parentBody, body);
  dJointSetFixed(joint);
}

void WbSolid::printKinematicWarningIfNeeded() {
  if (mKinematicWarningPrinted || !mHasDynamicSolidDescendant || !belongsToStaticBasis())
    return;

  mKinematicWarningPrinted = true;
  parsingWarn(tr("This node is controlled in kinematics mode "
                 "but some Solid descendant nodes have physics and won't move along with this node."));
}

WbVector3 WbSolid::relativeLinearVelocity(const WbSolid *parentSolid) const {
  WbVector3 l = isDynamic() ? solidMerger()->solid()->linearVelocity() : linearVelocity();

  const WbSolid *solid = this;
  // if this solid is kinematic we need to add the velocities of the parents
  if (solid->isKinematic()) {
    while (!solid->isTopSolid() && solid != parentSolid) {
      solid = solid->upperSolid();
      l = solid->rotationMatrix() * l;
      l += solid->linearVelocity();
      if (!solid->isKinematic())
        break;
    }
  } else if (parentSolid != NULL) {  // in case of dynamic solid, the velocity is already absolute
    while (!solid->isTopSolid() && solid != parentSolid)
      solid = solid->upperSolid();
    l -= solid->isDynamic() ? solid->solidMerger()->solid()->linearVelocity() : solid->linearVelocity();
  }

  assert(solid == parentSolid || parentSolid == NULL);
  return l;
}

WbVector3 WbSolid::relativeAngularVelocity(const WbSolid *parentSolid) const {
  WbVector3 a = isDynamic() ? solidMerger()->solid()->angularVelocity() : angularVelocity();

  const WbSolid *solid = this;
  // if this solid is kinematic we need to add the velocities of the parents
  if (solid->isKinematic()) {
    while (!solid->isTopSolid() && solid != parentSolid) {
      solid = solid->upperSolid();
      a = solid->rotationMatrix() * a;
      a += solid->angularVelocity();
      if (!solid->isKinematic())
        break;
    }
  } else if (parentSolid != NULL) {  // in case of dynamic solid, the velocity is already absolute
    while (!solid->isTopSolid() && solid != parentSolid)
      solid = solid->upperSolid();
    a -= solid->isDynamic() ? solid->solidMerger()->solid()->angularVelocity() : solid->angularVelocity();
  }

  assert(solid == parentSolid || parentSolid == NULL);
  return a;
}

void WbSolid::setLinearVelocity(const double velocity[3]) {
  mLinearVelocity->setValue(velocity[0], velocity[1], velocity[2]);
  if (isSolidMerger()) {
    dBodyID b = body();
    dBodySetLinearVel(b, velocity[0], velocity[1], velocity[2]);
  }
  printKinematicWarningIfNeeded();
}

void WbSolid::setAngularVelocity(const double velocity[3]) {
  mAngularVelocity->setValue(velocity[0], velocity[1], velocity[2]);
  if (isSolidMerger()) {
    dBodyID b = body();
    dBodySetAngularVel(b, velocity[0], velocity[1], velocity[2]);
  }
  printKinematicWarningIfNeeded();
}

void WbSolid::updateIsLinearVelocityNull() {
  mIsLinearVelocityNull = mLinearVelocity->value().isNull();
}

void WbSolid::updateIsAngularVelocityNull() {
  mIsAngularVelocityNull = mAngularVelocity->value().isNull();
}

dBodyID WbSolid::bodyMerger() const {
  assert(mIsKinematic || mSolidMerger);
  return isDynamic() ? mSolidMerger->body() : NULL;
}

dBodyID WbSolid::body() const {
  return isSolidMerger() ? mSolidMerger->body() : NULL;
}

void WbSolid::appendJointParent(WbBasicJoint *joint) {
  mJointParents.append(joint);
}

void WbSolid::removeJointParent(WbBasicJoint *joint) {
  mJointParents.removeOne(joint);
}

bool WbSolid::needJointToUpperSolid(const WbSolid *upperSolid) const {
  // create a fixed joint to the static environment only if
  // this node doesn't have any joint ancestor and any dynamic solid ancestor
  return mJointParents.isEmpty() && upperSolid->belongsToStaticBasis();
}

void WbSolid::setOdeJointToUpperSolid() {
  const WbSolid *const us = jointParent() ? NULL : upperSolid();
  const bool b = us && needJointToUpperSolid(us) && isSolidMerger() && (us->isDynamic() || us->belongsToStaticBasis());
  if (mJoint == NULL && b) {
    mJoint = createJoint(body(), us->bodyMerger(), WbOdeContext::instance()->world());
    return;
  }

  if (mJoint == NULL)
    return;

  if (b)
    // if the upper solid has no body, the solid is fixed to the static environment
    setJoint(mJoint, body(), us->bodyMerger());
  else
    // Removes the joint from simulation without destroying it
    setJoint(mJoint, NULL, NULL);
}

void WbSolid::setGeomMatter(dGeomID g, WbBaseNode *node) {
  if (mSolidMerger) {
    dGeomSetBody(g, mSolidMerger->body());
    addMassFromInsertedNode(node);
  }
}
/////////////////////
// Update Methods  //
/////////////////////

// Resets recursively ODE dGeoms positions, dBodies and joints starting from *this
void WbSolid::handleJerk() {
  jerk(false);
  if (!belongsToStaticBasis())
    awake();
  else
    WbWorld::instance()->awake();
}

void WbSolid::updateTranslation() {
  WbMatter::updateTranslation();

  if ((mGlobalCenterOfMassRepresentationIsEnabled || mSupportPolygonRepresentation) &&
      WbSimulationState::instance()->isPaused()) {
    float position[3];
    computedGlobalCenterOfMass().toFloatArray(position);
    wr_transform_set_position(mGlobalCenterOfMassTransform, position);
  }

  if (!mJointParents.isEmpty() && WbSimulationState::instance()->isPaused())
    emit positionChangedArtificially();
}

void WbSolid::updateRotation() {
  WbMatter::updateRotation();

  if ((mGlobalCenterOfMassRepresentationIsEnabled || mSupportPolygonRepresentation) &&
      WbSimulationState::instance()->isPaused()) {
    float position[3];
    computedGlobalCenterOfMass().toFloatArray(position);
    wr_transform_set_position(mGlobalCenterOfMassTransform, position);
  }

  if (!mJointParents.isEmpty() && WbSimulationState::instance()->isPaused())
    emit positionChangedArtificially();
}

// Scale

void WbSolid::updateScale(bool warning) {
  const int constraint = constraintType();
  if (checkScale(constraint, warning))
    return;

  WbMatter::applyToScale();

  if (WbOdeContext::instance() && boundingObject())
    applyToOdeScale();

  if (isDynamic())
    applyMassCenterToWren();
}

void WbSolid::setScaleNeedUpdate() {
  WbMatter::setScaleNeedUpdate();

  foreach (WbBasicJoint *const joint, mJointChildren) {
    if (joint->solidReference())
      continue;
    WbSolid *const s = joint->solidEndPoint();
    if (s)
      s->setScaleNeedUpdate();
  }
}

// Creates and updates, or destroys, the ODE dBody according to the existence of a WbPhysics node
void WbSolid::updatePhysics() {
  assert(areOdeObjectsCreated());

  if (isBeingDeleted())
    return;

  // this handles the deleted physics cases, we disable the optional renderings
  // that depend on the solid having a physics node.
  if (!mPhysics->value()) {
    if (isTopSolid())
      showSupportPolygonRepresentation(false);

    showGlobalCenterOfMassRepresentation(false);
    showCenterOfBuoyancyRepresentation(false);
  }

  bool previousKinematic = mIsKinematic;
  mIsKinematic = mPhysics->value() == NULL || mIsPermanentlyKinematic;
  if (mUseInertiaMatrix && mPhysics->value() != NULL)
    mUseInertiaMatrix = false;

  if (mSolidMerger == NULL || mSolidMerger->solid() == this) {
    mMergerIsSet = false;
    delete mSolidMerger.data();
  }

  setupSolidMergers();
  setBodiesAndJointsToParents();
  setJointChildrenWithReferencedEndpoint();

  adjustOdeMass();
  applyMassCenterToWren();
  refreshPhysicsRepresentation();

  if (previousKinematic != mIsKinematic)
    updateDynamicSolidDescendantFlag();
}

void WbSolid::updateRadarCrossSection() {
  if (mRadarCrossSection->value() > 0.0) {
    if (!WbWorld::instance()->radarTargetSolids().contains(this))
      WbWorld::instance()->addRadarTarget(this);
  } else if (WbWorld::instance()->radarTargetSolids().contains(this))
    WbWorld::instance()->removeRadarTarget(this);
}

void WbSolid::updateRecognitionColors() {
  if (!mRecognitionColors->isEmpty()) {
    if (!WbWorld::instance()->cameraRecognitionObjects().contains(this))
      WbWorld::instance()->addCameraRecognitionObject(this);
  } else if (WbWorld::instance()->cameraRecognitionObjects().contains(this))
    WbWorld::instance()->removeCameraRecognitionObject(this);
}

void WbSolid::updateOdeMass() {
  assert(isDynamic());

  const WbPhysics *const p = physics();
  if (!p->hasApositiveMassOrDensity())
    return;

  p->checkMassAndDensity();

  if (mUseInertiaMatrix && p->mode() != WbPhysics::CUSTOM_INERTIA_MATRIX)
    applyToOdeMass();  // the inertia computation mode has changed
  else
    adjustOdeMass();

  awake();
  refreshPhysicsRepresentation();
}

void WbSolid::setOdeInertiaMatrix() {
  assert(isDynamic() && physics()->mode() == WbPhysics::CUSTOM_INERTIA_MATRIX);
  const WbPhysics *const p = physics();
  mUseInertiaMatrix = true;
  const WbMFVector3 &inertiaMatrix = p->inertiaMatrix();
  const WbVector3 &v0 = inertiaMatrix.item(0);
  const WbVector3 &v1 = inertiaMatrix.item(1);
  const double s = absoluteScale().x();
  double s3 = s * s;
  double s5 = s3;
  s3 *= s;
  s5 *= s3;
  const double mass = p->mass();
  dMassSetParameters(mOdeMass, s3 * mass, 0.0, 0.0, 0.0, s5 * v0.x(), s5 * v0.y(), s5 * v0.z(), s5 * v1.x(), s5 * v1.y(),
                     s5 * v1.z());

  memcpy(mMassAroundCoM, mOdeMass, sizeof(dMass));
  updateCenterOfMass();
  const WbVector3 &com = scaledCenterOfMass();
  dMassTranslate(mOdeMass, com.x(), com.y(), com.z());  // translates inertia matrix to solid frame's origin
  emit massPropertiesChanged();
}

void WbSolid::updateOdeInertiaMatrix() {
  assert(isDynamic() && physics()->mode() == WbPhysics::CUSTOM_INERTIA_MATRIX);

  setOdeInertiaMatrix();
  mSolidMerger->mergeMass(this);
  awake();

  refreshPhysicsRepresentation();
}

void WbSolid::setInertiaMatrixFromBoundingObject() {
  assert(isDynamic() && hasAvalidBoundingObject() && physics()->hasApositiveMassOrDensity());

  // Uses the density or the mass of the Physics node together with the geometries
  // in the boundingObject to compute the inertia matrix of the solid.

  dMass dmass;
  dMassSetZero(&dmass);

  // Adds the masses of all the primitives lying in the bounding object
  WbSolidUtilities::addMass(&dmass, boundingObject(), 1000.0);
  memcpy(mReferenceMass, &dmass, sizeof(dMass));

  // Computes the inertia matrix around the center of mass of the bounding object
  const dReal *const com = mReferenceMass->c;
  dMassTranslate(&dmass, -com[0], -com[1], -com[2]);

  const WbField *const parameter = findField("physics", true)->parameter();
  WbPhysics *const p = parameter ? static_cast<WbPhysics *>(static_cast<WbSFNode *>(parameter->value())->value()) : physics();

  const double s = 1.0 / absoluteScale().x();
  double s3 = s * s;
  double s5 = s3;
  s3 *= s;

  if (p->mass() < 0.0) {
    double boundingObjectMass = mReferenceMass->mass;
    boundingObjectMass *= 0.001 * p->density();
    p->setMass(boundingObjectMass * s3, true);
    p->parsingInfo(tr("'mass' set as bounding object's mass based on 'density'."));
  }

  p->setDensity(-1.0, true);

  const double *const I = mReferenceMass->I;
  s5 *= s3;
  p->setInertiaMatrix(I[0] * s5, I[5] * s5, I[10] * s5, I[1] * s5, I[2] * s5, I[6] * s5, true);
  p->checkInertiaMatrix(false);

  const double *const c = mReferenceMass->c;
  p->setCenterOfMass(c[0] * s, c[1] * s, c[2] * s, true);
  p->parsingInfo(tr("Bounding object's center of mass inserted."));

  p->updateMode();
  if (parameter)
    physics()->updateMode();

  updateOdeInertiaMatrix();
}

void WbSolid::updateOdeCenterOfMass() {
  assert(isDynamic() && mSolidMerger);
  updateCenterOfMass();

  applyToOdeMass();
  mSolidMerger->setGeomAndBodyPositions(true);  // reset also joints passing through this solid
  awake();

  refreshPhysicsRepresentation();
}

void WbSolid::updateOdeDamping() {
  assert(isDynamic() && mSolidMerger);
  mSolidMerger->setOdeDamping();
  awake();
}

void WbSolid::updateBoundingObject() {
  if (mBoundingObject->value() != NULL) {
    WbBaseNode *node = dynamic_cast<WbBaseNode *>(mBoundingObject->value());
    assert(node);
    if (!isBoundingObjectFinalizationCompleted(node))
      // postpone bounding object update after finalization
      return;

    createOdeGeoms();
    updateOdeGeomPosition();
    dGeomID g = odeGeom();
    if (g && mSolidMerger) {
      mSolidMerger->attachGeomsToBody(g);
      createOdeMass(false);
      mSolidMerger->mergeMass(this);
    }
  }

  mBoundingObjectHasChanged = true;
  refreshPhysicsRepresentation();
}

// Updates of children nodes

void WbSolid::collectSolidChildren(const WbGroup *group, bool connectSignals, QVector<WbSolid *> &solidChildren,
                                   QVector<WbBasicJoint *> &jointChildren, QVector<WbPropeller *> &propellerChildren) {
  const WbMFNode *const ch = group->childrenField();
  if (connectSignals) {
    connect(ch, &WbMFNode::changed, this, &WbSolid::updateChildren, Qt::UniqueConnection);
    connect(group, &WbGroup::finalizedChildAdded, this, &WbSolid::refreshPhysicsRepresentation, Qt::UniqueConnection);
    connect(group, &WbGroup::finalizedChildAdded, this, &WbSolid::updateTopSolidGlobalMass, Qt::UniqueConnection);
  }
  WbMFNode::Iterator it(ch);
  while (it.hasNext()) {
    WbNode *const n = it.next();

    WbSolid *const solid = dynamic_cast<WbSolid *>(n);
    if (solid) {
      solidChildren.append(solid);
      continue;
    }

    WbBasicJoint *joint = dynamic_cast<WbBasicJoint *>(n);
    if (joint) {
      jointChildren.append(joint);
      WbSolid *const ep = joint->solidEndPoint();
      if (ep && joint->solidReference() == NULL) {
        solidChildren.append(ep);
        continue;
      }
    }

    WbPropeller *propeller = dynamic_cast<WbPropeller *>(n);
    if (propeller) {
      propellerChildren.append(propeller);
      continue;
    }

    const WbGroup *const groupChild = dynamic_cast<WbGroup *>(n);
    if (groupChild)
      collectSolidChildren(groupChild, connectSignals, solidChildren, jointChildren, propellerChildren);

    const WbSlot *slot = dynamic_cast<WbSlot *>(n);
    if (slot) {
      if (slot->hasEndpoint()) {
        WbSlot *sep = slot->slotEndPoint();
        while (sep) {
          slot = sep;
          sep = slot->slotEndPoint();
        }
        if (slot->solidEndPoint())
          solidChildren.append(slot->solidEndPoint());
        else if (slot->groupEndPoint())
          collectSolidChildren(slot->groupEndPoint(), connectSignals, solidChildren, jointChildren, propellerChildren);
        else {
          joint = dynamic_cast<WbBasicJoint *>(slot->endPoint());
          if (joint) {
            jointChildren.append(joint);
            WbSolid *const ep = joint->solidEndPoint();
            if (ep && joint->solidReference() == NULL) {
              solidChildren.append(ep);
              continue;
            }
          }

          propeller = dynamic_cast<WbPropeller *>(slot->endPoint());
          if (propeller) {
            propellerChildren.append(propeller);
            continue;
          }
        }
      }
    }
  }

  if (isPostFinalizedCalled())
    updateDynamicSolidDescendantFlag();
}

void WbSolid::updateDynamicSolidDescendantFlag() {
  mHasDynamicSolidDescendant = false;
  foreach (WbSolid *s, mSolidChildren) {
    if (!s->isPostFinalizedCalled())
      // postpone flag update after finalization
      return;

    if (s->isDynamic() || s->mHasDynamicSolidDescendant) {
      mHasDynamicSolidDescendant = true;
      break;
    }
  }

  WbSolid *us = upperSolid();
  if (us && us->isPostFinalizedCalled())
    us->updateDynamicSolidDescendantFlag();
}

void WbSolid::updateChildren() {
  mSolidChildren.clear();
  mJointChildren.clear();
  mPropellerChildren.clear();
  collectSolidChildren(this, true, mSolidChildren, mJointChildren, mPropellerChildren);

  foreach (WbSolid *const solid, mSolidChildren) {
    connect(solid, &WbSolid::destroyed, this, &WbSolid::updateChildren, Qt::UniqueConnection);
    connect(solid, &WbSolid::destroyed, this, &WbSolid::refreshPhysicsRepresentation, Qt::UniqueConnection);
    connect(solid, &WbSolid::physicsPropertiesChanged, this, &WbSolid::refreshPhysicsRepresentation, Qt::UniqueConnection);
  }
}

bool WbSolid::resetJointPositions(bool allParents) {
  bool b = false;

  setOdeJointToUpperSolid();

  foreach (WbBasicJoint *const joint, mJointParents) {
    if (allParents || joint->upperSolid()->belongsToStaticBasis())
      b |= joint->resetJointPositions();
  }

  return b;
}

void WbSolid::updateGlobalCenterOfMass() {
  mGlobalCenterOfMass.setXyz(0.0, 0.0, 0.0);
  mGlobalMass = 0.0;
  double mass = mGlobalMass;
  WbVector3 com(mGlobalCenterOfMass);
  foreach (WbSolid *const solid, mSolidChildren) {
    if (!solid->isPreFinalizedCalled())
      // skip until finalization is completed
      // it could happen in particular in case of multiple instances of PROTO parameter node
      return;

    solid->updateGlobalCenterOfMass();
    const double childGlobalMass = solid->globalMass();
    mass += childGlobalMass;
    com += childGlobalMass * solid->globalCenterOfMass();
  }

  mGlobalMass = mass;
  mGlobalCenterOfMass = com;

  if (isDynamic()) {
    mGlobalCenterOfMass += mOdeMass->mass * (matrix() * centerOfMass());
    mGlobalMass += mOdeMass->mass;
  }

  if (mGlobalMass > 0.0)
    mGlobalCenterOfMass /= mGlobalMass;
  else
    mGlobalCenterOfMass = position();
}

void WbSolid::updateCenterOfBuoyancy() {
  assert(isDynamic());

  const int size = mListOfImmersions.size();
  double immersedMass = 0.0;
  mCenterOfBuoyancy.setXyz(0.0, 0.0, 0.0);
  for (int i = 0; i < size; ++i) {
    const dImmersionGeom &ig = mListOfImmersions.at(i);
    const double volume = ig.volume;
    const double *const cob = ig.buoyancyCenter;
    const dReal density = dFluidGetDensity(dGeomGetFluid(ig.g2));
    const dReal mass = density * volume;
    mCenterOfBuoyancy += mass * WbVector3(cob[0], cob[1], cob[2]);
    immersedMass += mass;
  }

  if (immersedMass > 0.0)
    mCenterOfBuoyancy /= immersedMass;
}

double WbSolid::averageDensity() const {
  return mGlobalVolume > 0.0 ? mGlobalMass / mGlobalVolume : -1.0;
}

void WbSolid::updateGlobalVolume() {
  double volume = 0.0;

  foreach (WbSolid *const solid, mSolidChildren) {
    if (!solid->isPreFinalizedCalled())
      // skip until finalization is completed
      // it could happen in particular in case of multiple instances of PROTO parameter node
      return;
    solid->updateGlobalVolume();
    volume += solid->globalVolume();
  }

  mGlobalVolume = volume + 0.001 * mReferenceMass->mass;
}

void WbSolid::updateCenterOfMass() {
  assert(isDynamic());

  WbPhysics *const p = physics();
  p->updateMode();
  const int mode = p->mode();

  mCenterOfMass.setXyz(0.0, 0.0, 0.0);
  mScaledCenterOfMass.setXyz(0.0, 0.0, 0.0);

  switch (mode) {
    case WbPhysics::CUSTOM_INERTIA_MATRIX:
      mCenterOfMass = p->centerOfMass().item(0);
      mScaledCenterOfMass = mCenterOfMass;
      mScaledCenterOfMass *= absoluteScale().x();
      break;
    case WbPhysics::BOUNDING_OBJECT_BASED: {
      if (p->centerOfMass().size() == 1) {
        mCenterOfMass = p->centerOfMass().item(0);
        mScaledCenterOfMass = mCenterOfMass;
        mScaledCenterOfMass *= absoluteScale().x();
      } else if (mBoundingObject->value() != NULL) {
        mScaledCenterOfMass.setXyz(mReferenceMass->c[0], mReferenceMass->c[1], mReferenceMass->c[2]);
        mCenterOfMass = mScaledCenterOfMass;
        const double s = 1.0 / absoluteScale().x();
        mCenterOfMass *= s;
      }
      break;
    }
    default:
      assert(mode == WbPhysics::INVALID);
  }

  applyMassCenterToWren();
}

////////////////////
// Apply Methods  //
////////////////////

// Apply to WREN

void WbSolid::applyMassCenterToWren() {
  if (!areWrenObjectsInitialized())
    return;

  if (mIsKinematic) {
    wr_node_set_visible(WR_NODE(mCenterOfMassTransform), false);
    return;
  }

  // if the CoM is (0, 0, 0), it coincides with the axes center, so we hide it in this case
  const bool massCenterVisible = isSelected() && !mCenterOfMass.isNull();
  if (massCenterVisible) {
    float position[3];
    mCenterOfMass.toFloatArray(position);

    wr_transform_set_position(mCenterOfMassTransform, position);
    wr_node_set_visible(WR_NODE(mCenterOfMassTransform), true);
  } else
    wr_node_set_visible(WR_NODE(mCenterOfMassTransform), false);
}

void WbSolid::updateLineScale() {
  WbMatter::updateLineScale();

  const float lineScale = wr_config_get_line_scale() * WbWrenRenderingContext::SOLID_LINE_SCALE_FACTOR;
  const float scale[3] = {lineScale, lineScale, lineScale};

  wr_transform_set_scale(mCenterOfBuoyancyTransform, scale);
  wr_transform_set_scale(mGlobalCenterOfMassTransform, scale);
  wr_transform_set_scale(mCenterOfMassTransform, scale);

  if (mSupportPolygonRepresentation)
    mSupportPolygonRepresentation->setScale(scale);
}

void WbSolid::applyChangesToWren() {
  WbMatter::applyChangesToWren();
  applyMassCenterToWren();
  refreshPhysicsRepresentation();
}

void WbSolid::applyVisibilityFlagsToWren(bool selected) {
  WbMatter::applyVisibilityFlagsToWren(selected);

  if (isDynamic() && !centerOfMass().isNull())
    wr_node_set_visible(WR_NODE(mCenterOfMassTransform), selected);
}

void WbSolid::setDefaultMassSettings(bool applyCenterOfMassTranslation, bool warning) {
  const double mass = physics()->mass();
  if (mass > 0.0) {
    dMassSetParameters(mOdeMass, mass, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
    if (warning)
      parsingWarn(
        tr("Undefined inertia matrix: using the identity matrix. Please specify 'boundingObject' or 'inertiaMatrix' values."));
  } else {
    dMassSetParameters(mOdeMass, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
    if (warning) {
      if (physics()->density() > 0.0)
        parsingWarn(
          tr("Mass is invalid because 'boundingObject' is not defined. Using default mass properties: mass = 1, inertia "
             "matrix = identity"));
      else
        parsingWarn(tr("Mass is invalid: %1. Using default mass properties: mass = 1, inertia matrix = identity").arg(mass));
    }
  }

  if (applyCenterOfMassTranslation)
    dMassTranslate(mOdeMass, mScaledCenterOfMass.x(), mScaledCenterOfMass.y(), mScaledCenterOfMass.z());
}

// Compute the mass and the inertia around solid frame's origin
void WbSolid::createOdeMass(bool reset) {
  assert(isDynamic());

  if (reset) {
    dMassSetZero(mReferenceMass);
    dMassSetZero(mOdeMass);
  }

  // Adds the masses of all the primitives lying in the bounding object
  const WbPhysics *const p = physics();
  const bool customMass = p->mode() == WbPhysics::CUSTOM_INERTIA_MATRIX;
  // needed for average density and average damping
  WbSolidUtilities::addMass(mReferenceMass, boundingObject(), 1000.0, !customMass);

  // Checks whether there is a valid inertia matrix, and uses it if so
  if (customMass)
    setOdeInertiaMatrix();
  else {
    mUseInertiaMatrix = false;
    // We rule out the case of a boundingObject with no Geometry inside
    if (mReferenceMass->mass <= 0.0)
      setDefaultMassSettings(false);
    else {
      // Uses the density or the mass of the Physics node together with the geometries
      // in the boundingObject to compute the inertia matrix of the solid.
      memcpy(mOdeMass, mReferenceMass, sizeof(dMass));
    }

    updateCenterOfMass();

    assert(mOdeMass->mass > 0.0);

    const double density = p->density();
    const double mass = p->mass();
    const double s = absoluteScale().x();

    // Sets the actual total mass
    double actualMass = mOdeMass->mass;
    if (mass > 0.0) {
      const double s2 = s * s;
      actualMass = s * s2 * mass;
    } else if (density != 1000.0)
      actualMass *= 0.001 * density;

    // Adjust the total according to mass and density fields
    dMassAdjust(mOdeMass, actualMass);

    memcpy(mMassAroundCoM, mOdeMass, sizeof(dMass));

    // Translate the mass to solid's origin
    if (p->centerOfMass().size() == 1) {
      dVector3 t;
      dSubtractVectors3(t, scaledCenterOfMass().ptr(), mReferenceMass->c);
      dMassTranslate(mOdeMass, t[0], t[1], t[2]);
    }

    updateTopSolidGlobalMass();
    emit massPropertiesChanged();
  }
}

double WbSolid::mass() const {
  return mOdeMass->mass;
}

double WbSolid::density() const {
  const double d = isDynamic() ? physics()->density() : -1.0;
  const double v = volume();
  return d >= 0.0 ? d : v > 0.0 ? mOdeMass->mass / v : -1.0;
}

double WbSolid::volume() const {
  return 0.001 * mReferenceMass->mass;
}

const double *WbSolid::inertiaMatrix() const {
  return mMassAroundCoM->I;
}

void WbSolid::applyToOdeMass() {
  assert(isDynamic() && mSolidMerger);
  const WbPhysics *const p = physics();
  if (!p->hasApositiveMassOrDensity())
    return;

  createOdeMass();
  mSolidMerger->mergeMass(this);
}

void WbSolid::propagateScale() {
  // Sets new masses and new CoMs from top to bottom
  WbMatter::propagateScale();

  adjustOdeMass(false);  // rescale the ODE dMass in keeping with Webots density or mass value

  if (mSolidMerger == NULL)
    updateOdeGeomPosition();

  foreach (WbSolid *const solid, mSolidChildren)
    solid->propagateScale();

  // Merges new masses and sets new geom and body positions, from bottom to top
  if (isSolidMerger()) {
    mSolidMerger->updateMasses();
    mSolidMerger->setGeomAndBodyPositions();
    mSolidMerger->setOdeDamping();
  }

  if (mSolidMerger)
    applyMassCenterToWren();
  else
    WbWorld::instance()->awake();
}

void WbSolid::applyToOdeScale() {
  propagateScale();
  resetJoints();
}

void WbSolid::updateTransformForPhysicsStep() {
  if (mUpdatedInStep)
    return;

  applyPhysicsTransform();

  QList<WbSolid *> reversedList;
  reversedList << this;
  WbSolid *s = NULL;
  WbNode *p = parentNode();
  while (p != NULL && !p->isWorldRoot()) {
    s = dynamic_cast<WbSolid *>(p);
    if (s != NULL) {
      if (s->mUpdatedInStep)
        break;  // ancestor nodes already updated
      reversedList.prepend(s);
    }
    p = p->parentNode();
  }

  // update transform from root to current node as applyPhysicsTransform uses the upper transform matrix
  QListIterator<WbSolid *> it(reversedList);
  while (it.hasNext()) {
    s = it.next();
    s->applyPhysicsTransform();
    s->mUpdatedInStep = true;
  }
}

void WbSolid::applyPhysicsTransform() {
  dVector3 result;  // VRML translation
  dQuaternion qr;   // VRML rotation

  // get current body rotation
  dBodyID b = body();
  if (!b)
    return;
  dBodyCopyQuaternion(b, qr);

  // update linear and angular velocity
  if (mLinearVelocity && mAngularVelocity) {
    const double *const l = dBodyGetLinearVel(b);
    const double *const a = dBodyGetAngularVel(b);
    mLinearVelocity->setValue(l[0], l[1], l[2]);
    mAngularVelocity->setValue(a[0], a[1], a[2]);
  }

  // find Solid merger's frame center in world coordinates
  const WbVector3 &com = mSolidMerger->scaledCenterOfMass();
  if (com.isNull())
    dBodyCopyPosition(b, result);
  else
    // Solid center != com in this case
    dBodyGetRelPointPos(b, -com.x(), -com.y(), -com.z(), result);
  assert(!std::isnan(result[0]));
  // printf("new body pos = %f, %f, %f (apply phy.)\n", result[0], result[1], result[2]);
  const WbTransform *const ut = upperTransform();
  if (ut) {
    const double invUtScale = 1.0 / ut->absoluteScale().x();
    const double scaleFactor = invUtScale * invUtScale;
    const WbMatrix4 &utm = ut->matrix();
    const WbVector3 &prel = utm.pseudoInversed(WbVector3(result));
    result[0] = scaleFactor * prel[0];
    result[1] = scaleFactor * prel[1];
    result[2] = scaleFactor * prel[2];
    // printf("result = %f, %f, %f (apply phy.))\n", result[0], result[1], result[2]);
    // find rotation difference between upper transform and solid child
    const WbQuaternion &q = utm.extractedQuaternion(invUtScale);
    dQMultiply1(qr, q.ptr(), dBodyGetQuaternion(b));
  }

  if (std::isnan(qr[0]) || std::isnan(qr[1]) || std::isnan(qr[2]) || std::isnan(qr[3]) ||
      (qr[1] == 0.0 && qr[2] == 0.0 && qr[3] == 0.0)) {
    setTransformFromOde(result[0], result[1], result[2], 0.0, 1.0, 0.0, 0.0);
    return;
  }

  const double norm = sqrt(qr[1] * qr[1] + qr[2] * qr[2] + qr[3] * qr[3]);
  double angle = 2.0 * atan2(norm, qr[0]);  // in the range [-2 * M_PI, 2 * M_PI]
  if (angle < -M_PI)
    angle += 2.0 * M_PI;
  else if (angle > M_PI)
    angle -= 2.0 * M_PI;

  const double normInv = 1.0 / norm;
  qr[1] *= normInv;
  qr[2] *= normInv;
  qr[3] *= normInv;

  // block signals from WbTransform (baseclass): we don't want to update the bodies and the geoms
  // printf("pos = %f, %f, %f\n", result[0], result[1], result[2]);
  setTransformFromOde(result[0], result[1], result[2], qr[1], qr[2], qr[3], angle);
}

//////////////////
// Run Methods  //
//////////////////

void WbSolid::postPhysicsStep() {
  int i = 0;
  dBodyID body = this->body();

  if (mResetPhysicsInStep) {
    // physics reset from Supervisor: if the solid is also moved from Supervisor in the same step, ODE may overwrite velocities
    // and forces based on the jerk
    resetSingleSolidPhysics();
    if (mSolidMerger)
      mSolidMerger->setBodyArtificiallyDisabled(false);
    mResetPhysicsInStep = false;
  }

  if (body && dBodyIsEnabled(body))
    applyPhysicsTransform();

  // Warning: do not use foreach here => Qt foreach loop are very inefficient here
  for (i = 0; i < mJointChildren.size(); ++i)
    if (mJointChildren.at(i)->isEnabled())
      mJointChildren.at(i)->postPhysicsStep();

  for (i = 0; i < mSolidChildren.size(); ++i)
    mSolidChildren.at(i)->postPhysicsStep();
}

void WbSolid::prePhysicsStep(double ms) {
  int i = 0;

  if (handleJerkIfNeeded())
    mMovedChildren.clear();
  else if (!mMovedChildren.isEmpty())
    childrenJerk();

  if (mIsKinematic && robot())
    updateOdeGeomPosition();

  // Update position in kinematic mode
  if (mIsKinematic && mLinearVelocity && !mIsLinearVelocityNull)
    translate(mLinearVelocity->value() * (ms / 1000));
  // Update orientation in kinematic mode
  if (mIsKinematic && mAngularVelocity && !mIsAngularVelocityNull)
    rotate(mAngularVelocity->value() * (ms / 1000));

  // Warning: do not use foreach here => Qt foreach loop are very inefficient here
  for (i = 0; i < mJointChildren.size(); ++i)
    if (mJointChildren.at(i)->solidEndPoint())
      mJointChildren.at(i)->prePhysicsStep(ms);

  for (i = 0; i < mSolidChildren.size(); ++i)
    mSolidChildren.at(i)->prePhysicsStep(ms);

  for (i = 0; i < mPropellerChildren.size(); ++i)
    mPropellerChildren.at(i)->prePhysicsStep(ms);

  mUpdatedInStep = false;
}

////////////
// Others //
////////////

// Accessors to relatives

WbBasicJoint *WbSolid::jointParent() const {
  WbSlot *parentSlot = dynamic_cast<WbSlot *>(parentNode());
  if (parentSlot) {
    WbSlot *granParentSlot = dynamic_cast<WbSlot *>(parentSlot->parentNode());
    if (granParentSlot)
      return dynamic_cast<WbBasicJoint *>(granParentSlot->parentNode());
  }
  return dynamic_cast<WbBasicJoint *>(parentNode());
}

dBodyID WbSolid::upperSolidBody() const {
  return upperSolid()->bodyMerger();
}

WbSolid *WbSolid::findSolid(const QString &name, const WbSolid *const exception) {
  if (this->name() == name && this != exception)
    return this;

  foreach (WbSolid *const solid, mSolidChildren) {
    WbSolid *const s = solid->findSolid(name, exception);
    if (s && s != exception)
      return s;
  }

  return NULL;
}

WbRobot *WbSolid::robot() const {
  if (!mHasSearchedRobot) {
    mRobot = WbNodeUtilities::findRobotAncestor(this);
    mHasSearchedRobot = true;
  }

  return mRobot;
}

// Returns true if all solid ancestors have no physics
bool WbSolid::belongsToStaticBasis() const {
  if (isDynamic())
    return false;

  const WbSolid *s = upperSolid();

  while (s) {
    if (s->isDynamic())
      return false;
    s = s->upperSolid();
  }

  return true;
}

WbPhysics *WbSolid::physics() const {
  return static_cast<WbPhysics *>(mPhysics->value());
}

bool WbSolid::isSleeping() const {
  dBodyID b = bodyMerger();
  if (b)
    return !dBodyIsEnabled(b);
  return false;
}

// ODE encapsulated methods
void WbSolid::addForceAtPosition(const WbVector3 &force, const WbVector3 &position) {
  dBodyAddForceAtPos(bodyMerger(), force.x(), force.y(), force.z(), position.x(), position.y(), position.z());
}

void WbSolid::addTorque(const WbVector3 &torque) {
  dBodyAddTorque(bodyMerger(), torque.x(), torque.y(), torque.z());
}

// Selection management
void WbSolid::propagateSelection(bool selected) {
  if (wrenNode() && mIsPermanentlyKinematic) {
    const WbPropeller *const propeller = dynamic_cast<WbPropeller *>(parentNode());
    if (propeller) {
      const bool active = propeller->helix() == this;
      wr_node_set_visible(WR_NODE(wrenNode()), selected || active);
    }
  }

  select(selected);
  WbMatter::propagateSelection(selected);

  foreach (WbBasicJoint *const joint, mJointChildren) {
    if (joint->solidReference())
      continue;
    WbSolid *const solid = joint->solidEndPoint();
    if (solid)
      solid->propagateSelection(selected);
  }

  WbBaseNode *const bo = boundingObject();
  if (bo)
    bo->propagateSelection(selected);
}

void WbSolid::setMatrixNeedUpdate() {
  WbNode *bo = boundingObject();
  WbGroup *g = dynamic_cast<WbGroup *>(bo);
  if (g)
    g->setMatrixNeedUpdate();

  WbTransform::setMatrixNeedUpdate();
}

void WbSolid::reset(const QString &id) {
  WbMatter::reset(id);

  for (int i = 0; i < mImmersionProperties->size(); ++i)
    mImmersionProperties->item(i)->reset(id);
  WbNode *const p = mPhysics->value();
  if (p)
    p->reset(id);

  if (mJointParents.size() == 0) {
    setTranslation(mSavedTranslations[id]);
    setRotation(mSavedRotations[id]);
  }
  resetSingleSolidPhysics();
  resetContactPointsAndSupportPolygon();
  resetContactPoints();
  resetImmersions();

  // remove contact joints
  if (isSolidMerger()) {
    dBodyID b = body();
    int jointNumber = dBodyGetNumJoints(b);
    dJointID joints[jointNumber];
    for (int i = 0; i < jointNumber; ++i)
      joints[i] = dBodyGetJoint(b, i);
    for (int i = 0; i < jointNumber; ++i) {
      if (dJointGetType(joints[i]) == dJointTypeContact) {
        dJointAttach(joints[i], 0, 0);
        dJointDestroy(joints[i]);
      }
    }
  }

  int counter = 0;
  restoreHiddenKinematicParameters(mHiddenKinematicParametersMap, counter);

  if (handleJerkIfNeeded())
    mMovedChildren.clear();
  else if (!mMovedChildren.isEmpty())
    childrenJerk();

  mListOfImmersions.clear();
}

void WbSolid::save(const QString &id) {
  WbMatter::save(id);
  if (isTopSolid())
    saveHiddenFieldValues();

  for (int i = 0; i < mImmersionProperties->size(); ++i)
    mImmersionProperties->item(i)->save(id);
  WbNode *const p = mPhysics->value();
  if (p)
    p->save(id);

  mSavedTranslations[id] = translation();
  mSavedRotations[id] = rotation();
}

// Recursive reset methods
// It resets the positions of all ODE dGeoms, static or not, based on the current translation and rotation fields
// It also resets the velocities of all dBodies to 0.0
void WbSolid::jerk(bool resetVelocities, bool rootJerk) {
  if (isSolidMerger())
    mSolidMerger->setGeomAndBodyPositions(resetVelocities, mJointParents.size() == 0 && !isTopSolid());
  else
    updateOdeGeomPosition();

  foreach (WbSolid *const solid, mSolidChildren)
    solid->jerk(resetVelocities, false);

  foreach (WbBasicJoint *const joint, mJointChildren)
    joint->updateOdeWorldCoordinates();

  if (isDynamic() && mJointParents.size() > 0 && rootJerk)
    emit positionChangedArtificially();
}

void WbSolid::notifyChildJerk(WbTransform *childNode) {
  WbNode *node = childNode->parentNode();
  while (node != this && node != NULL) {
    if (mMovedChildren.contains(dynamic_cast<WbTransform *>(node)))
      return;
    node = node->parentNode();
  }

  mMovedChildren.append(childNode);
}

void WbSolid::childrenJerk() {
  updateOdeGeomPosition();
  foreach (WbTransform *childNode, mMovedChildren) {
    QVector<WbSolid *> solidChildren;
    QVector<WbBasicJoint *> jointChildren;
    QVector<WbPropeller *> propellerChildren;
    collectSolidChildren(childNode, false, solidChildren, jointChildren, propellerChildren);

    foreach (WbSolid *const solid, solidChildren)
      solid->jerk(false, false);

    foreach (WbBasicJoint *const joint, jointChildren)
      joint->updateOdeWorldCoordinates();
  }
  mMovedChildren.clear();
}

void WbSolid::awake() {
  if (mSolidMerger && !mSolidMerger->isBodyArtificiallyDisabled())
    dBodyEnable(mSolidMerger->body());
  else
    WbWorld::instance()->awake();
}

void WbSolid::awakeSolids(WbGroup *group) {
  assert(group);

  WbSolid *const solid = dynamic_cast<WbSolid *>(group);
  if (solid) {
    dBodyID b = solid->body();

    if (b && dBodyIsEnabled(b))  // this body is enabled => all its children are enabled too
      return;

    if (b && solid->solidMerger() && !solid->solidMerger()->isBodyArtificiallyDisabled())
      dBodyEnable(b);
    if (solid->mHasDynamicSolidDescendant) {
      const QVector<WbSolid *> &sl = solid->solidChildren();
      for (int i = 0; i < sl.size(); ++i)
        awakeSolids(sl.at(i));
    }
  } else {  // Handles the case of non-Solid (possibly nested) Groups which are children of the root
    WbMFNode::Iterator it(group->children());
    while (it.hasNext()) {
      WbGroup *const g = dynamic_cast<WbGroup *>(it.next());
      if (g)
        awakeSolids(g);
    }
  }
}

void WbSolid::resetPhysics(bool recursive) {
  resetSingleSolidPhysics();

  // Recurses through all first level solid descendants
  if (recursive)
    foreach (WbSolid *const solid, mSolidChildren)
      solid->resetPhysics();
}

void WbSolid::resetSingleSolidPhysics() {
  // check for joints and disable all motors
  const int size = mJointChildren.size();
  for (int i = 0; i < size; ++i) {
    WbJoint *const j = dynamic_cast<WbJoint *>(mJointChildren[i]);
    if (j)
      j->resetPhysics();
  }

  mLinearVelocity->setValue(0.0, 0.0, 0.0);
  mAngularVelocity->setValue(0.0, 0.0, 0.0);

  if (isSolidMerger()) {
    dBodyID b = body();
    dBodySetLinearVel(b, 0.0, 0.0, 0.0);
    dBodySetAngularVel(b, 0.0, 0.0, 0.0);
    dBodySetForce(b, 0.0, 0.0, 0.0);
    dBodySetTorque(b, 0.0, 0.0, 0.0);
    if (!mSolidMerger->isBodyArtificiallyDisabled())
      dBodyEnable(b);
  }

  if (mJoint) {
    switch (dJointGetType(mJoint)) {
      case dJointTypeHinge2:
        dJointSetHinge2Param(mJoint, dParamFMax, 0.0);
        dJointSetHinge2Param(mJoint, dParamVel, 0.0);
        dJointSetHinge2Param(mJoint, dParamFMax2, 0.0);
        dJointSetHinge2Param(mJoint, dParamVel2, 0.0);
        break;
      case dJointTypeHinge:
        dJointSetHingeParam(mJoint, dParamFMax, 0.0);
        dJointSetHingeParam(mJoint, dParamVel, 0.0);
        break;
      case dJointTypeSlider:
        dJointSetSliderParam(mJoint, dParamFMax, 0.0);
        dJointSetSliderParam(mJoint, dParamVel, 0.0);
        break;
      case dJointTypeBall:
        dJointSetBallParam(mJoint, dParamFMax, 0.0);
        dJointSetBallParam(mJoint, dParamVel, 0.0);
        dJointSetBallParam(mJoint, dParamFMax2, 0.0);
        dJointSetBallParam(mJoint, dParamVel2, 0.0);
        dJointSetBallParam(mJoint, dParamFMax3, 0.0);
        dJointSetBallParam(mJoint, dParamVel3, 0.0);
      default:  // only the above joint types are currently implemented in Webots
        break;
    }
  }
}

void WbSolid::pausePhysics(bool resumeAutomatically) {
  if (resumeAutomatically)
    mResetPhysicsInStep = true;

  if (mSolidMerger)
    mSolidMerger->setBodyArtificiallyDisabled(true);

  foreach (WbSolid *const solid, mSolidChildren)
    solid->pausePhysics();
}

void WbSolid::resumePhysics() {
  resetSingleSolidPhysics();
  if (mSolidMerger)
    mSolidMerger->setBodyArtificiallyDisabled(false);

  foreach (WbSolid *const solid, mSolidChildren)
    solid->resumePhysics();
}

///////////////////////////////
// Contact Points Management //
///////////////////////////////

const QVector<WbVector3> &WbSolid::computedContactPoints(bool includeDescendants) {
  extractContactPoints();
  connect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this, &WbSolid::resetContactPoints,
          Qt::UniqueConnection);
  return includeDescendants ? mGlobalListOfContactPoints : mListOfContactPoints;
}

const QVector<const WbSolid *> &WbSolid::computedSolidPerContactPoints() {
  extractContactPoints();
  connect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this, &WbSolid::resetContactPoints,
          Qt::UniqueConnection);
  return mSolidPerContactPoints;
}

void WbSolid::extractContactPoints() {
  if (mHasExtractedContactPoints)
    return;

  const WbWorld *const world = WbWorld::instance();
  const QList<WbOdeContact> &fullList = world->odeContacts();
  const int size = fullList.size();

  for (int i = 0; i < size; ++i) {
    const dContactGeom &cg = fullList.at(i).contactGeom();
    const WbOdeGeomData *const odeGeomData1 = static_cast<WbOdeGeomData *>(dGeomGetData(cg.g1));
    const WbOdeGeomData *const odeGeomData2 = static_cast<WbOdeGeomData *>(dGeomGetData(cg.g2));
    const WbSolid *const s1 = odeGeomData1->solid();
    const WbSolid *const s2 = odeGeomData2->solid();

    if (s1 == this || s2 == this) {
      const double *const pos = cg.pos;
      const WbVector3 v(pos[0], pos[1], pos[2]);
      mListOfContactPoints.append(v);
    }

    if (s1->topSolid() == this || s2->topSolid() == this) {
      const double *const pos = cg.pos;
      const WbVector3 v(pos[0], pos[1], pos[2]);
      mGlobalListOfContactPoints.append(v);
      if (s1->topSolid() == this)
        mSolidPerContactPoints.append(s1);
      else
        mSolidPerContactPoints.append(s2);
      // stores the smallest y-coordinate of all contact points
      const double downProjection = v.dot(world->worldInfo()->upVector());
      if (downProjection < mY)
        mY = downProjection;
    }
  }

  mHasExtractedContactPoints = true;
}

void WbSolid::extractImmersions() {
  assert(isDynamic());
  if (mHasExtractedImmersions)
    return;

  const WbWorld *const world = WbWorld::instance();
  const QList<dImmersionGeom> &fullList = world->immersionGeoms();
  const int size = fullList.size();

  for (int i = 0; i < size; ++i) {
    const dImmersionGeom &ig = fullList.at(i);
    const WbOdeGeomData *const odeGeomData = static_cast<WbOdeGeomData *>(dGeomGetData(ig.g1));
    const WbSolid *const s = odeGeomData->solid();

    if (s == this)
      mListOfImmersions.append(ig);
  }

  mHasExtractedImmersions = true;
}

// Computes the support polygon of the robot if needed
const WbPolygon &WbSolid::supportPolygon() {
  const WbWorldInfo *const worldInfo = WbWorld::instance()->worldInfo();
  if (!mSupportPolygonNeedsUpdate)
    return mSupportPolygon;

  extractContactPoints();
  const int numberOfContactPoints = mGlobalListOfContactPoints.size();
  const WbVector3 &eastVector = worldInfo->eastVector();
  const WbVector3 &northVector = worldInfo->northVector();
  // Rules out 4 trivial cases
  if (numberOfContactPoints == 0) {
    mSupportPolygon.setActualSize(0);
    return mSupportPolygon;
  }

  const WbVector3 &v0 = mGlobalListOfContactPoints[0];
  mSupportPolygon[0].setXy(v0.dot(northVector), v0.dot(eastVector));
  if (numberOfContactPoints == 1) {
    mSupportPolygon.setActualSize(1);
    return mSupportPolygon;
  }

  const WbVector3 &v1 = mGlobalListOfContactPoints[1];
  mSupportPolygon[1].setXy(v1.dot(northVector), v1.dot(eastVector));
  if (numberOfContactPoints == 2) {
    mSupportPolygon.setActualSize(2);
    return mSupportPolygon;
  }

  const WbVector3 &v2 = mGlobalListOfContactPoints[2];
  mSupportPolygon[2].setXy(v2.dot(northVector), v2.dot(eastVector));
  if (numberOfContactPoints == 3) {
    mSupportPolygon.setActualSize(3);
    return mSupportPolygon;
  }

  // From now on, the robot has at least 4 contact points
  QVector<WbVector2> listOfProjectedContactPoints(numberOfContactPoints);
  // Projects contact points onto a plane orthogonal to the down direction
  for (int i = 0; i < numberOfContactPoints; ++i) {
    const WbVector3 &v = mGlobalListOfContactPoints.at(i);
    listOfProjectedContactPoints[i].setXy(v.dot(northVector), v.dot(eastVector));
  }

  // Gets the indices of points in the convex hull of the projected contact points
  QVector<int> listOfIndices(numberOfContactPoints);
  const int supportPolygonSize = WbMathsUtilities::twoStepsConvexHull(listOfProjectedContactPoints, listOfIndices);

  // Resizes the support polygon only if the number of vertices has increased
  if (supportPolygonSize > mSupportPolygon.size())
    mSupportPolygon.resize(supportPolygonSize);
  mSupportPolygon.setActualSize(supportPolygonSize);

  // Extracts the support polygon from the projected contact points
  for (int i = 0; i < supportPolygonSize; ++i)
    mSupportPolygon[i] = listOfProjectedContactPoints.at(listOfIndices.at(i));

  // For optimization
  mSupportPolygonNeedsUpdate = false;

  return mSupportPolygon;
}

void WbSolid::deleteSupportPolygonRepresentation() {
  delete mSupportPolygonRepresentation;
  mSupportPolygonRepresentation = NULL;
}

// Creates or destroys the graphical support polygon of the solid according to menu actions: the solid must be a top solid
bool WbSolid::showSupportPolygonRepresentation(bool enabled) {
  if (mIsKinematic || !isTopLevel()) {
    if (enabled)
      info(tr("A top Solid with a non-NULL Physics node has to be chosen rather."));
    return false;
  }
  mSupportPolygonRepresentationIsEnabled = enabled;

  if (enabled) {
    if (!mSupportPolygonRepresentation) {
      mSupportPolygonRepresentation = new WbSupportPolygonRepresentation();
      mSupportPolygonNeedsUpdate = true;
    }
    connect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this,
            &WbSolid::refreshSupportPolygonRepresentation, Qt::UniqueConnection);
    connect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this,
            &WbSolid::resetContactPointsAndSupportPolygon, Qt::UniqueConnection);
    mSupportPolygonRepresentation->show(true);
    wr_node_set_visible(WR_NODE(mGlobalCenterOfMassTransform), true);
    refreshSupportPolygonRepresentation();
    updateLineScale();
  } else {
    if (mSupportPolygonRepresentation) {
      disconnect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this,
                 &WbSolid::refreshSupportPolygonRepresentation);
      disconnect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this,
                 &WbSolid::resetContactPointsAndSupportPolygon);
      if (WbSimulationState::instance()->isFast() || !WbSimulationState::instance()->isRendering())
        deleteSupportPolygonRepresentation();
      else {
        mSupportPolygonRepresentation->show(false);
        wr_node_set_visible(WR_NODE(mGlobalCenterOfMassTransform), false);
      }
    }
  }

  return true;
}

// Shows or hides the graphical 'global' center of mass of the solid according to menu actions
bool WbSolid::showGlobalCenterOfMassRepresentation(bool enabled) {
  if (mIsKinematic) {
    if (enabled)
      info(tr("A Solid with a non-NULL Physics node must be chosen."));
    return false;
  }
  mGlobalCenterOfMassRepresentationIsEnabled = enabled;

  if (enabled) {
    connect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this,
            &WbSolid::refreshGlobalCenterOfMassRepresentation, Qt::UniqueConnection);
    wr_node_set_visible(WR_NODE(mGlobalCenterOfMassTransform), true);
    refreshGlobalCenterOfMassRepresentation();
  } else {
    disconnect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this,
               &WbSolid::refreshGlobalCenterOfMassRepresentation);
    wr_node_set_visible(WR_NODE(mGlobalCenterOfMassTransform), false);
  }

  return true;
}

// Shows or hides the graphical center of buoyancy of the solid according to menu actions
bool WbSolid::showCenterOfBuoyancyRepresentation(bool enabled) {
  if (mIsKinematic) {
    if (enabled)
      info(tr("A Solid with a non-NULL Physics node must be chosen."));
    return false;
  }
  mCenterOfBuoyancyRepresentationIsEnabled = enabled;

  if (enabled) {
    connect(WbSimulationState::instance(), &WbSimulationState::physicsStepEnded, this,
            &WbSolid::refreshCenterOfBuoyancyRepresentation, Qt::UniqueConnection);
    refreshCenterOfBuoyancyRepresentation();
  } else {
    disconnect(WbSimulationState::instance(), &WbSimulationState::physicsStepEnded, this,
               &WbSolid::refreshCenterOfBuoyancyRepresentation);
    wr_node_set_visible(WR_NODE(mCenterOfBuoyancyTransform), false);
  }

  return true;
}

void WbSolid::refreshPhysicsRepresentation() {
  if (mSupportPolygonRepresentationIsEnabled)
    refreshSupportPolygonRepresentation();
  else if (mGlobalCenterOfMassRepresentationIsEnabled)
    refreshGlobalCenterOfMassRepresentation();

  if (mCenterOfBuoyancyRepresentationIsEnabled)
    refreshCenterOfBuoyancyRepresentation();

  // propagate change to ancestors
  emit physicsPropertiesChanged();
}

// Redraws the support polygon of the solid after each physics step when required by the menu options
void WbSolid::refreshSupportPolygonRepresentation() {
  const WbVector3 &c = computedGlobalCenterOfMass();
  float position[3];
  c.toFloatArray(position);
  wr_transform_set_position(mGlobalCenterOfMassTransform, position);
  const WbWorldInfo *const worldInfo = WbWorld::instance()->worldInfo();
  const WbVector3 b[3] = {worldInfo->northVector(), worldInfo->upVector(), worldInfo->eastVector()};
  const WbPolygon &p = supportPolygon();
  mSupportPolygonRepresentation->draw(p, mY, c, b);
}

// Redraws the global center of mass of the solid after each physics step when required by the menu options
void WbSolid::refreshGlobalCenterOfMassRepresentation() {
  if (mSupportPolygonRepresentationIsEnabled)
    return;

  float position[3];
  computedGlobalCenterOfMass().toFloatArray(position);
  wr_transform_set_position(mGlobalCenterOfMassTransform, position);
}

// Redraws the center of buoyancy of the solid after each physics step when required by the menu options
void WbSolid::refreshCenterOfBuoyancyRepresentation() {
  connect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this, &WbSolid::resetImmersions,
          Qt::UniqueConnection);
  extractImmersions();
  if (mListOfImmersions.size() > 0) {
    updateCenterOfBuoyancy();
    wr_node_set_visible(WR_NODE(mCenterOfBuoyancyTransform), true);
    float position[3];
    mCenterOfBuoyancy.toFloatArray(position);
    wr_transform_set_position(mCenterOfBuoyancyTransform, position);
  } else
    wr_node_set_visible(WR_NODE(mCenterOfBuoyancyTransform), false);
}

unsigned char WbSolid::staticBalance() {
  const WbVector3 &c = computedGlobalCenterOfMass();
  const WbPolygon &p = supportPolygon();
  const WbWorldInfo *const wi = WbWorld::instance()->worldInfo();
  const double globalComX = c.dot(wi->northVector());
  const double globalComZ = c.dot(wi->eastVector());
  const bool stable = p.contains(globalComX, globalComZ);
  return stable;
}

void WbSolid::resetContactPointsAndSupportPolygon() {
  mGlobalListOfContactPoints.resize(0);
  mSolidPerContactPoints.resize(0);
  mY = numeric_limits<double>::max();
  mSupportPolygonNeedsUpdate = true;
  mHasExtractedContactPoints = false;
}

void WbSolid::resetContactPoints() {
  mListOfContactPoints.resize(0);
  mGlobalListOfContactPoints.resize(0);
  mSolidPerContactPoints.resize(0);
  mHasExtractedContactPoints = false;
  disconnect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this, &WbSolid::resetContactPoints);
}

void WbSolid::resetImmersions() {
  mListOfImmersions.resize(0);
  mHasExtractedImmersions = false;
  disconnect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this, &WbSolid::resetImmersions);
}

void WbSolid::onSimulationModeChanged() {
  if (WbSimulationState::instance()->isFast() || !WbSimulationState::instance()->isRendering()) {
    if (mSupportPolygonRepresentation && !mSupportPolygonRepresentationIsEnabled) {
      deleteSupportPolygonRepresentation();
      disconnect(WbSimulationState::instance(), &WbSimulationState::physicsStepStarted, this,
                 &WbSolid::refreshSupportPolygonRepresentation);
    }
  }
}

void WbSolid::updateGraphicalGlobalCenterOfMass() {
  if (mIsKinematic)
    return;

  if (mSupportPolygonRepresentationIsEnabled || mGlobalCenterOfMassRepresentationIsEnabled) {
    float globalCenterOfMassRelativePosition[3];
    // Place center of mass relative to its position
    globalCenterOfMass().toFloatArray(globalCenterOfMassRelativePosition);
    wr_transform_set_position(mGlobalCenterOfMassTransform, globalCenterOfMassRelativePosition);
  }
}

void WbSolid::resetPhysicsIfRequired(bool changedFromSupervisor) {
  if (!changedFromSupervisor) {
    // For now, only the position modifications done by the user should reset the physics.
    resetPhysics();
  }

  WbViewpoint *viewpoint = WbWorld::instance()->viewpoint();
  if (viewpoint->followedSolid() == this)
    viewpoint->updateFollowSolidState();
}

// Collision and sleep flags management

void WbSolid::propagateBoundingObjectMaterialUpdate(bool onSelection) {
  // Recurses through all first level solid descendants
  foreach (WbSolid *const solid, mSolidChildren)
    solid->propagateBoundingObjectMaterialUpdate(onSelection);

  WbBaseNode *const bo = boundingObject();
  if (!bo)
    return;

  const bool isAsleep = isSleeping();
  const bool triggerChange = mBoundingObjectHasChanged || onSelection;

  // Update with current collision and sleep flags
  if (triggerChange) {
    if (isAsleep)
      bo->setSleepMaterial();
    else
      bo->updateCollisionMaterial(true, onSelection);

    updateSleepFlag();
    return;
  }

  const bool sleepHasChanged = mWasSleeping != isAsleep;
  mWasSleeping = isAsleep;

  // Optimized update with previous and current flags
  if (isAsleep) {
    if (sleepHasChanged)
      bo->setSleepMaterial();
    updateSleepFlag();
    return;
  } else
    bo->updateCollisionMaterial(sleepHasChanged, onSelection);

  updateSleepFlag();
}

void WbSolid::updateSleepFlag() {
  WbMatter::updateSleepFlag();
}

void WbSolid::displayWarning() {
  dBodyID b = bodyMerger();
  if (b) {
    dMass m;
    dBodyGetMass(b, &m);

    double inertialMatrixDiagonalMin = qMin(m.I[0], qMin(m.I[5], m.I[10]));
    double inertialMatrixDiagonalMax = qMax(m.I[0], qMax(m.I[5], m.I[10]));

    // To reproduce this, just play with a cylinder of h=0.125, r=0.0075
    // http://www.gamedev.net/topic/355556-problem-with-ode-and-small-numbers/

    if (inertialMatrixDiagonalMin > 0.0 &&
        // inertialMatrixDiagonalMax > 0.0 && // this is ensured
        inertialMatrixDiagonalMin < 1.0e-5 &&                         // light object : this theshold is empirical
        inertialMatrixDiagonalMax / inertialMatrixDiagonalMin > 15.0  // oblong object : this theshold is empirical
    )
      parsingWarn(tr("Webots has detected that this solid is light and oblong according to its inertia matrix. "
                     "This belongs in the physics edge cases, and can imply weird physical results. "
                     "Increasing the weight of the object or reducing its eccentricity are recommended."));
  }
}

/////////////////////////////////////////////
//  Collecting names of Solid descendants  //
/////////////////////////////////////////////

void WbSolid::collectSolidDescendantNames(QStringList &items, const WbSolid *const solidException) const {
  if (this != solidException)
    items << name();

  // Recurses through all first level solid descendants
  foreach (WbSolid *const solid, mSolidChildren)
    solid->collectSolidDescendantNames(items, solidException);
}

//////////////////////////////////////////////////////////////////
//  Collecting kinematic hidden parameters of Solid descendants //
//////////////////////////////////////////////////////////////////

void WbSolid::collectHiddenKinematicParameters(HiddenKinematicParametersMap &map, int &counter) const {
  const bool merger = isSolidMerger();
  const WbVector3 *t = NULL;
  const WbRotation *r = NULL;
  const WbVector3 *l = NULL;
  const WbVector3 *a = NULL;
  bool copyTranslation = false;
  bool copyRotation = false;
  WbVector3 translationToBeCopied;
  WbRotation rotationToBeCopied;

  if (mSolidMerger == NULL || merger) {
    // TODO: implement an mIsVisible flag in WbNode for sake of efficiency
    WbBasicJoint *parentJoint = jointParent();
    if (parentJoint) {
      // remove unquantified ODE effects on the endPoint Solid
      parentJoint->computeEndPointSolidPositionFromParameters(translationToBeCopied, rotationToBeCopied);
      // Note:
      //   This is an exception to the global double precision which is not sufficient here,
      //   because the accumulated error is big in computeEndPointSolidPositionFromParameters().
      //   cf. https://github.com/omichel/webots-dev/issues/6512
      if (!translationToBeCopied.almostEquals(mSavedTranslations[stateId()],
                                              100000.0 * std::numeric_limits<double>::epsilon()) &&
          !isTranslationFieldVisible())
        copyTranslation = true;
      if (!rotationToBeCopied.almostEquals(mSavedRotations[stateId()], 100000.0 * std::numeric_limits<double>::epsilon()) &&
          !isRotationFieldVisible())
        copyRotation = true;
    } else {
      if (translation() != mSavedTranslations[stateId()] && !isTranslationFieldVisible())
        t = &translation();
      if (rotation() != mSavedRotations[stateId()] && !isRotationFieldVisible())
        r = &rotation();
    }

    if (merger) {
      const dBodyID b = mSolidMerger->body();
      const dReal *lArray = dBodyGetLinearVel(b);
      if (!WbMathsUtilities::isZeroVector3(lArray))
        l = new WbVector3(lArray);
      const dReal *aArray = dBodyGetAngularVel(b);
      if (!WbMathsUtilities::isZeroVector3(aArray))
        a = new WbVector3(aArray);
    }
  }

  PositionMap positions;
  const int size = mJointChildren.size();
  for (int i = 0; i < size; ++i) {
    const WbJoint *const j = dynamic_cast<WbJoint *>(mJointChildren[i]);
    if (j) {
      WbVector3 v(NAN, NAN, NAN);

      // TODO: implement an mIsVisible flag in WbNode for sake of efficiency
      const WbJointParameters *const p = j->parameters();
      if ((p == NULL || !WbNodeUtilities::isVisible(p->findField("position"))) && j->position() != j->initialPosition())
        v[0] = j->position();

      if (j->nodeType() == WB_NODE_HINGE_2_JOINT || j->nodeType() == WB_NODE_BALL_JOINT) {
        const WbJointParameters *const p2 = j->parameters2();
        if ((p2 == NULL || !WbNodeUtilities::isVisible(p2->findField("position"))) && j->position(2) != j->initialPosition(2))
          v[1] = j->position(2);
      }

      if (j->nodeType() == WB_NODE_BALL_JOINT) {
        const WbJointParameters *const p3 = j->parameters3();
        if ((p3 == NULL || !WbNodeUtilities::isVisible(p3->findField("position"))) && j->position(3) != j->initialPosition(3))
          v[2] = j->position(3);
      }

      if (!std::isnan(v[0]) || !std::isnan(v[1]) || !std::isnan(v[2]))
        positions.insert(i, new WbVector3(v));
    }
  }

  PositionMap *const p = positions.size() > 0 ? new PositionMap(positions) : NULL;

  if (t || r || p || l || a || copyTranslation || copyRotation) {
    HiddenKinematicParameters *hkp = new HiddenKinematicParameters(t, r, p, l, a);
    if (copyTranslation)
      hkp->createTranslation(translationToBeCopied[0], translationToBeCopied[1], translationToBeCopied[2]);
    if (copyRotation)
      hkp->createRotation(rotationToBeCopied[0], rotationToBeCopied[1], rotationToBeCopied[2], rotationToBeCopied[3]);
    map.insert(counter, hkp);
  }

  ++counter;

  // Recurses through all first level solid descendants
  foreach (WbSolid *const solid, mSolidChildren)
    solid->collectHiddenKinematicParameters(map, counter);
}

///////////////////
// Hidden fields //
///////////////////

void WbSolid::saveHiddenFieldValues() const {
  if (isSolidMerger()) {
    const dBodyID b = mSolidMerger->body();
    const double *const l = dBodyGetLinearVel(b);
    const double *const a = dBodyGetAngularVel(b);
    mLinearVelocity->setValue(l[0], l[1], l[2]);
    mAngularVelocity->setValue(a[0], a[1], a[2]);
  }
}

////////////////////////
//  Kinematic solids  //
////////////////////////

void WbSolid::enable(bool enabled, bool ode) {
  assert(mIsKinematic);

  wr_node_set_visible(WR_NODE(wrenNode()), enabled);

  if (ode) {
    const dGeomID g = odeGeom();
    if (g) {
      dSpaceID space = WbOdeContext::instance()->space();
      const bool hasSpace = dGeomGetSpace(g) != NULL;
      if (enabled) {
        if (!hasSpace)
          dSpaceAdd(space, g);
      } else if (hasSpace)
        dSpaceRemove(space, g);
    }
  }
}

void WbSolid::exportURDFShape(WbVrmlWriter &writer, const QString &geometry, const WbTransform *transform,
                              bool correctOrientation, const WbVector3 &offset) const {
  const QStringList element = QStringList() << "visual"
                                            << "collision";
  for (int j = 0; j < element.size(); ++j) {
    writer.increaseIndent();
    writer.indent();
    writer << QString("<%1>\n").arg(element[j]);
    writer.increaseIndent();
    if (transform != this || correctOrientation || !offset.isNull()) {
      WbVector3 translation = transform->translation() + offset;
      WbRotation rotation = transform->rotation();
      writer.indent();
      if (correctOrientation) {
        if (transform == this) {
          translation = offset;
          rotation = WbRotation(1.0, 0.0, 0.0, 1.5707963);
        } else
          rotation = WbRotation(rotation.toMatrix3() * WbRotation(1.0, 0.0, 0.0, 1.5707963).toMatrix3());
      } else if (transform == this) {
        rotation = WbRotation(0.0, 1.0, 0.0, 0.0);
        translation = offset;
      }
      writer << QString("<origin xyz=\"%1\" rpy=\"%2\"/>\n")
                  .arg(translation.toString(WbPrecision::FLOAT_ROUND_6))
                  .arg(rotation.toMatrix3().toEulerAnglesZYX().toString(WbPrecision::FLOAT_ROUND_6));
    }
    writer.indent();
    writer << "<geometry>\n";
    writer.increaseIndent();
    writer.indent();
    writer << geometry;
    writer.decreaseIndent();
    writer.indent();
    writer << "</geometry>\n";
    writer.decreaseIndent();
    writer.indent();
    writer << QString("</%1>\n").arg(element[j]);
    writer.decreaseIndent();
  }
}

bool WbSolid::exportNodeHeader(WbVrmlWriter &writer) const {
  if (writer.isUrdf()) {
    const bool ret = WbMatter::exportNodeHeader(writer);
    if (!ret) {
      if (boundingObject()) {
        QList<WbNode *> nodes = boundingObject()->subNodes(true);
        for (int i = 0; i < nodes.size(); ++i) {
          const WbNode *node = nodes[i];
          const WbCylinder *cylinder = dynamic_cast<const WbCylinder *>(node);
          const WbBox *box = dynamic_cast<const WbBox *>(node);
          const WbSphere *sphere = dynamic_cast<const WbSphere *>(node);
          const WbCapsule *capsule = dynamic_cast<const WbCapsule *>(node);
          if (box || cylinder || sphere || capsule) {
            const WbTransform *transform = WbNodeUtilities::findUpperTransform(node);
            QList<QPair<QString, WbVector3>> geometries;  // string of the geometry and its offset

            if (box) {
              QPair<QString, WbVector3> pair;
              pair.first = QString("<box size=\"%1 %2 %3\"/>\n").arg(box->size().x()).arg(box->size().y()).arg(box->size().z());
              geometries << pair;
            } else if (cylinder) {
              QPair<QString, WbVector3> pair;
              pair.first = QString("<cylinder radius=\"%1\" length=\"%2\"/>\n").arg(cylinder->radius()).arg(cylinder->height());
              geometries << pair;
            } else if (capsule) {
              QPair<QString, WbVector3> pair;
              pair.first = QString("<cylinder radius=\"%1\" length=\"%2\"/>\n").arg(capsule->radius()).arg(capsule->height());
              geometries << pair;
              pair.first = QString("<sphere radius=\"%1\"/>\n").arg(capsule->radius());
              pair.second = WbVector3(0.0, 0.5 * capsule->height(), 0.0);
              if (transform)
                pair.second = transform->rotation().toMatrix3() * pair.second;
              geometries << pair;
              pair.first = QString("<sphere radius=\"%1\"/>\n").arg(capsule->radius());
              pair.second = WbVector3(0.0, -0.5 * capsule->height(), 0.0);
              if (transform)
                pair.second = transform->rotation().toMatrix3() * pair.second;
              geometries << pair;
            } else if (sphere) {
              QPair<QString, WbVector3> pair;
              pair.first = QString("<sphere radius=\"%1\"/>\n").arg(sphere->radius());
              geometries << pair;
            } else
              assert(false);
            for (int j = 0; j < geometries.size(); ++j)
              exportURDFShape(writer, geometries[j].first, transform, cylinder || capsule,
                              geometries[j].second + writer.jointOffset());
          }
        }
      }
    }
    return ret;
  }

  return WbMatter::exportNodeHeader(writer);
}

void WbSolid::exportNodeFields(WbVrmlWriter &writer) const {
  WbMatter::exportNodeFields(writer);
  if (writer.isX3d()) {
    if (!name().isEmpty())
      writer << " name='" << sanitizedName() << "'";
    writer << " solid='true'";
  }
}

void WbSolid::exportNodeFooter(WbVrmlWriter &writer) const {
  if (writer.isX3d() && boundingObject()) {
    writer << "<Switch whichChoice='-1' class='selector'>";
    const WbGeometry *geom = dynamic_cast<const WbGeometry *>(boundingObject());
    if (geom)
      writer << "<Shape>";

    boundingObject()->exportBoundingObjectToX3D(writer);

    if (geom)
      writer << "</Shape>";

    writer << "</Switch>";
  }

  WbMatter::exportNodeFooter(writer);
}

const QString WbSolid::sanitizedName() const {
  QString name_dirty = name();
  name_dirty.replace("\'", "&apos;", Qt::CaseInsensitive);
  name_dirty.replace("\"", "&quot;", Qt::CaseInsensitive);
  name_dirty.replace(">", "&gt;", Qt::CaseInsensitive);
  name_dirty.replace("<", "&lt;", Qt::CaseInsensitive);
  return name_dirty.replace("&", "&amp;", Qt::CaseInsensitive);
}
