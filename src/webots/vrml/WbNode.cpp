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

#include "WbNode.hpp"

#include "WbField.hpp"
#include "WbFieldModel.hpp"
#include "WbLog.hpp"
#include "WbMFBool.hpp"
#include "WbMFColor.hpp"
#include "WbMFDouble.hpp"
#include "WbMFInt.hpp"
#include "WbMFNode.hpp"
#include "WbMFRotation.hpp"
#include "WbMFString.hpp"
#include "WbMFVector2.hpp"
#include "WbMFVector3.hpp"
#include "WbNodeFactory.hpp"
#include "WbNodeModel.hpp"
#include "WbNodeReader.hpp"
#include "WbParser.hpp"
#include "WbProject.hpp"
#include "WbProtoModel.hpp"
#include "WbSFBool.hpp"
#include "WbSFColor.hpp"
#include "WbSFDouble.hpp"
#include "WbSFInt.hpp"
#include "WbSFNode.hpp"
#include "WbSFRotation.hpp"
#include "WbSFString.hpp"
#include "WbSFVector2.hpp"
#include "WbSFVector3.hpp"
#include "WbStandardPaths.hpp"
#include "WbToken.hpp"
#include "WbTokenizer.hpp"
#include "WbVrmlWriter.hpp"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QRegularExpression>
#include <QtCore/QSet>
#include <QtCore/QTemporaryFile>

#include <cassert>

struct ProtoParameters {
  const QVector<WbField *> *params;
};
static QList<ProtoParameters *> gProtoParameterList;
static QList<const WbNode *> gUrdfNodesQueue;
static QMap<int, QString> gUrdfNames;
static const WbNode *gUrdfCurrentNode;
static int gUrdfNameIndex = 0;

static bool gInstantiateMode = true;
static QVector<WbNode *> gNodes = {NULL};  // id 0 is reserved for root node
static WbNode *gParent = NULL;
static bool gProtoParameterNodeFlag = false;
static bool gNestedProtoFlag = false;
static bool gDefCloneFlag = false;
static bool gDerivedProtoFlag = false;        // true during the creation of a derived PROTO
static bool gDerivedProtoParentFlag = false;  // true during the creation of a PROTO node whose parent PROTO is derived
static bool gTopParameterFlag = false;
static bool gDerivedProtoAncestorFlag = false;
static QStringList *gInternalDefNamesInWrite = NULL;
static QList<QPair<WbNode *, int>> *gExternalUseNodesInWrite = NULL;
static bool gRestoreUniqueIdOnClone;

bool WbNode::cUpdatingDictionary = false;

const QStringList WbNode::cHiddenParameterNames = QStringList() << "translation"
                                                                << "rotation"
                                                                << "linearVelocity"
                                                                << "angularVelocity"
                                                                << "position"
                                                                << "position2"
                                                                << "position3";

void WbNode::setInstantiateMode(bool mode) {
  gInstantiateMode = mode;
}

bool WbNode::instantiateMode() {
  return gInstantiateMode;
}

void WbNode::setGlobalParentNode(WbNode *parent, bool protoParameterNodeFlag) {
  gParent = parent;

  if (parent)
    gProtoParameterNodeFlag = protoParameterNodeFlag || parent->mIsProtoDescendant || parent->isProtoInstance();
  else
    gProtoParameterNodeFlag = false;
}

WbNode *WbNode::globalParentNode() {
  return gParent;
}

bool WbNode::setRestoreUniqueIdOnClone(bool enable) {
  const bool previousValue = gRestoreUniqueIdOnClone;
  gRestoreUniqueIdOnClone = enable;
  return previousValue;
}

void WbNode::setUniqueId(int newId) {
  // check new id is valid
  // - correct range
  // - the newId slot is empty (the node was deleted)
  assert(newId >= 0);
  assert(newId < gNodes.size());
  assert(gRestoreUniqueIdOnClone || gNodes[newId] == NULL);

  // update the id
  if (mUniqueId != -1 && mUniqueId != newId) {
    if (mUniqueId == gNodes.size() - 1)
      gNodes.removeLast();
    else
      gNodes[mUniqueId] = NULL;
  }
  gNodes[newId] = this;
  mUniqueId = newId;
}

int WbNode::getFreeUniqueId() {
  int id = gNodes.size();
  gNodes.append(NULL);
  return id;
}

void WbNode::init() {
  // nodes at the .wbt level are created with a mUniqueId
  if (gInstantiateMode && !gRestoreUniqueIdOnClone) {
    mUniqueId = gNodes.size();
    gNodes.append(this);
  } else
    // nodes in .proto files are created with mUniqueId = -1
    mUniqueId = -1;

  mDefNode = NULL;
  mHasUseAncestor = false;
  setParentNode(gParent);
  gParent = this;
  mIsBeingDeleted = false;
  mIsProtoDescendant = false;
  mProtoParameterNode = NULL;
  mIsNestedProtoNode = false;
  mProtoParameterNodeInstances.clear();
  mRegenerationRequired = false;
  mIsCreationCompleted = false;
  mInsertionCompleted = false;
  mIsTopParameterDescendant = false;
  mProto = NULL;
  mCurrentStateId = "__init__";
}

WbNode::WbNode(const QString &modelName, const QString &worldPath, WbTokenizer *tokenizer) :
  mModel(WbNodeModel::findModel(modelName)) {
  init();

  // create fields from model
  foreach (WbFieldModel *const fieldModel, mModel->fieldModels())
    mFields.append(new WbField(fieldModel, this));

  if (tokenizer)
    readFields(tokenizer, worldPath);

  foreach (const WbField *const field, mFields)
    connect(field, &WbField::valueChanged, this, &WbNode::notifyFieldChanged);

  gParent = parentNode();

  if (gProtoParameterNodeFlag)
    mIsProtoDescendant = true;
  if (gTopParameterFlag)
    mIsTopParameterDescendant = true;
}

WbNode::WbNode(const WbNode &other) :
  mModel(other.mModel),
  mDefName(other.mDefName),
  mUseName(other.mUseName),
  mProtoInstanceFilePath(other.mProtoInstanceFilePath),
  mProtoInstanceTemplateContent(other.mProtoInstanceTemplateContent) {
  init();
  if (gRestoreUniqueIdOnClone)
    setUniqueId(other.mUniqueId);

  // copy mProto reference in any case
  if (other.mProto) {
    mProto = other.mProto;
    mProto->ref();
  }

  if (gDerivedProtoAncestorFlag && other.mProto && (parentNode() && parentNode()->mIsNestedProtoNode))
    mIsNestedProtoNode = true;

  // do not redirect fields of DEF node descendant even if included in a PROTO parameter
  if (gDefCloneFlag || (parentNode() && parentNode()->mHasUseAncestor))
    mHasUseAncestor = true;

  const bool previousNestedProtoFlag = gNestedProtoFlag;
  if (!mHasUseAncestor && !gDefCloneFlag && other.mIsProtoDescendant && gProtoParameterNodeFlag) {
    // clone PROTO parameter node

    if (other.mIsNestedProtoNode)
      // apply nested PROTO method when copying the internal nodes of the nested PROTO node
      gNestedProtoFlag = true;

    // copy fields
    foreach (WbField *parameterNodeField, other.mFields) {
      WbField *field = NULL;
      if (gNestedProtoFlag) {
        // create an instance of a nested PROTO parameter node
        // don't redirect PROTO instance fields to PROTO node fields
        // PROTO instance fields will be redirected to PROTO node parameters in function cloneAndReferenceProtoInstance()
        field = new WbField(*parameterNodeField, this);
      } else {
        // create an instance of a non-PROTO parameter node
        field = new WbField(parameterNodeField->model(), this);
        field->redirectTo(parameterNodeField);

        if (!other.mProto && gDerivedProtoAncestorFlag && !gTopParameterFlag)
          field->setAlias(parameterNodeField->alias());
      }

      mFields.append(field);
      connect(field, &WbField::valueChanged, this, &WbNode::notifyFieldChanged);
    }

    // copy parameters
    gNestedProtoFlag = true;
    foreach (WbField *parameter, other.parameters()) {
      WbField *copy = new WbField(*parameter, this);
      mParameters.append(copy);
      connect(copy, &WbField::valueChanged, this, &WbNode::notifyParameterChanged);
    }

    mIsProtoDescendant = true;

  } else {
    if (other.mIsNestedProtoNode && !(gTopParameterFlag || other.mIsTopParameterDescendant))
      // apply nested PROTO method when copying the internal nodes of the nested PROTO node
      gNestedProtoFlag = true;

    // copy fields
    foreach (WbField *field, other.mFields) {
      WbField *copiedField = NULL;
      if (mHasUseAncestor || gNestedProtoFlag || gDefCloneFlag || field->alias().isEmpty() ||
          (other.mProto != NULL && field->parameter() == NULL))
        copiedField = new WbField(*field, this);
      else {
        // don't copy PROTO parameter values inside the field
        // they will be copied when redirecting aliased fields to parameters
        copiedField = new WbField(field->model(), this);
        copiedField->setAlias(field->alias());
        copyAliasValue(copiedField, field->alias());
      }
      mFields.append(copiedField);
      connect(copiedField, &WbField::valueChanged, this, &WbNode::notifyFieldChanged);
    }

    if (gProtoParameterNodeFlag)
      mIsProtoDescendant = true;

    if (other.mProto) {
      const bool previousProtoParameterFlag = gProtoParameterNodeFlag;
      if (gInstantiateMode) {
        gProtoParameterNodeFlag = true;

        // detect nested PROTO instances
        mIsNestedProtoNode = other.mIsNestedProtoNode;
      }

      // add parameters with values copied from PROTO model
      gNestedProtoFlag = true;
      foreach (WbField *parameter, other.parameters()) {
        WbField *copy = new WbField(*parameter, this);
        mParameters.append(copy);
        connect(copy, &WbField::valueChanged, this, &WbNode::notifyParameterChanged);
      }
      gNestedProtoFlag = previousNestedProtoFlag;

      // connect fields to PROTO parameters
      if (gInstantiateMode) {
        if (!mIsNestedProtoNode || !mIsProtoDescendant)
          foreach (WbField *parameter, mParameters)
            redirectAliasedFields(parameter, this, other.mProto->isDerived());
      }

      gProtoParameterNodeFlag = previousProtoParameterFlag;
    }
  }

  gParent = parentNode();
  if (gTopParameterFlag)
    mIsTopParameterDescendant = true;
  gNestedProtoFlag = previousNestedProtoFlag;
}

WbNode::~WbNode() {
  mIsBeingDeleted = true;

  // qDeleteAll(mFields); // Delete always USE nodes before DEF nodes
  int n = mFields.size() - 1;
  for (int i = n; i >= 0; --i)
    delete mFields[i];

  if (mProto) {
    // qDeleteAll(mParameters); // Delete Always USE nodes before DEF nodes
    n = mParameters.size() - 1;
    for (int i = n; i >= 0; --i)
      delete mParameters[i];
    mProto->unref();
  }

  foreach (WbNode *instance, mProtoParameterNodeInstances) {
    if (instance->mProtoParameterNode == this)
      instance->mProtoParameterNode = NULL;
  }
  mProtoParameterNodeInstances.clear();

  // nodes in PROTO definitions and in scene tree clipboard are not in gNodes[]
  // in case of PROTO regeneration, the unique ID could already be assigned to a new node
  if (mUniqueId != -1 && gNodes[mUniqueId] == this)
    gNodes[mUniqueId] = NULL;

  if (isUseNode() && mDefNode) {
    mDefNode->mUseNodes.removeOne(this);
    mDefNode->useNodesChanged();
  }

  if (isDefNode()) {
    foreach (WbNode *const useNode, mUseNodes)
      useNode->setDefNode(NULL);
  }

  // delete the PROTO instance temporary file if any
  if (!mProtoInstanceFilePath.isEmpty() && QFile::exists(mProtoInstanceFilePath))
    QFile::remove(mProtoInstanceFilePath);
}

const QString &WbNode::modelName() const {
  return mProto ? mProto->name() : mModel->name();
}

const QString &WbNode::info() const {
  return mProto ? mProto->info() : mModel->info();
}

void WbNode::setDefName(const QString &defName, bool recurse) {
  if (defName == mDefName)
    return;

  foreach (WbNode *const useNode, mUseNodes)
    useNode->setUseName(defName);

  mDefName = defName;
  emit defUseNameChanged(this, false);

  if (!recurse)
    return;

  const WbNode *parent = parentNode();
  const WbNode *previousParentNode = this;
  QList<int> parentIndices;
  while (parent) {
    int index = subNodeIndex(previousParentNode, parent);
    if (index < 0)
      // parent-child link not correctly setup
      return;
    parentIndices.prepend(index);
    if (!parent->useNodes().isEmpty()) {
      const QList<WbNode *> &useList = parent->useNodes();
      foreach (WbNode *const useNode, useList) {
        WbNode *const defNode = findNodeFromSubNodeIndices(parentIndices, useNode);
        assert(defNode != NULL);
        defNode->setDefName(defName, false);
      }
    }
    previousParentNode = parent;
    parent = parent->parentNode();
  }
}

void WbNode::setUseName(const QString &useName, bool signal) {
  if (mUseName == useName)
    return;

  mUseName = useName;

  if (signal)
    emit defUseNameChanged(this, false);
}

QString WbNode::fullName() const {
  if (isUseNode())
    return "USE " + mUseName;
  if (!defName().isEmpty())
    return "DEF " + mDefName + " " + modelName();
  return modelName();
}

QString WbNode::fullVrmlName() const {
  if (isUseNode())
    return "USE " + mUseName;

  QString name = vrmlName();

  if (defName().isEmpty())
    return name;
  return "DEF " + mDefName + " " + name;
}

QString WbNode::usefulName() const {
  if (isUseNode())
    return "USE " + mUseName;

  QString usefulName = "";

  if (!defName().isEmpty())
    usefulName += QString("DEF ") + defName() + " ";

  usefulName += modelName();

  if (isProtoInstance())
    usefulName += " (PROTO)";

  return usefulName;
}

const QString &WbNode::nodeModelName() const {
  return mModel->name();
}

QString WbNode::fullPath(const QString &fieldName, QString &parameterName) const {
  const WbNode *n = this;
  WbField *field = NULL;
  if (!fieldName.isEmpty())
    field = findField(fieldName, true);

  if (field && field->parameter()) {
    // find visible parameter
    WbField *parameter = field->parameter();
    while (parameter->parameter())
      parameter = parameter->parameter();

    parameterName = parameter->name();
    n = parameter->parentNode();
    assert(n);
  } else
    parameterName = "";

  QString path;
  while (n && !n->isWorldRoot()) {
    if (n->protoParameterNode())
      n = n->protoParameterNode();

    if (path.isEmpty())
      path = n->usefulName();
    else
      path = n->usefulName() + " > " + path;

    n = n->parentNode();
  }

  return path;
}

QString WbNode::extractFieldName(const QString &message) const {
  // extract field name
  QString fieldName;
  QRegularExpression regExp("'(\\w)+'");
  QRegularExpressionMatch match = regExp.match(message);
  if (match.hasMatch()) {
    fieldName = match.captured(0);
    fieldName = fieldName.mid(1, fieldName.length() - 2);  // remove single quotes
  }

  return fieldName;
}

void WbNode::parsingWarn(const QString &message) const {
  warn(message, true);
}

void WbNode::parsingInfo(const QString &message) const {
  info(message, true);
}

void WbNode::warn(const QString &message, bool parsingMessage) const {
  QString fieldName = extractFieldName(message);
  QString parameterName;
  QString path = fullPath(fieldName, parameterName);
  QString improvedMsg = message;

  if (!fieldName.isEmpty() && !parameterName.isEmpty())
    // improve message by displaying parameter name instead of hidden field name
    improvedMsg.replace("'" + fieldName + "'", "'" + parameterName + "'");

  if (parsingMessage)
    WbLog::warning(path + ": " + improvedMsg, false, WbLog::PARSING);
  else
    WbLog::warning(path + ": " + improvedMsg);
}

void WbNode::info(const QString &message, bool parsingMessage) const {
  QString fieldName = extractFieldName(message);
  QString parameterName;
  QString path = fullPath(fieldName, parameterName);
  QString improvedMsg = message;

  if (!fieldName.isEmpty() && !parameterName.isEmpty())
    // improve message by displaying parameter name instead of hidden field name
    improvedMsg.replace("'" + fieldName + "'", "'" + parameterName + "'");

  if (parsingMessage)
    WbLog::info(path + ": " + improvedMsg, false, WbLog::PARSING);
  else
    WbLog::info(path + ": " + improvedMsg);
}

void WbNode::cleanup() {
  gNodes.clear();
  gNodes.append(NULL);  // id 0 is reserved for root node
}

WbNode *WbNode::findNode(int uniqueId) {
  if (uniqueId >= 0 && uniqueId < gNodes.size())
    return gNodes[uniqueId];

  return NULL;
}

const QVector<WbField *> &WbNode::fieldsOrParameters() const {
  return isProtoInstance() ? mParameters : mFields;
}

int WbNode::numFields() const {
  return fieldsOrParameters().size();
}

WbField *WbNode::field(int index, bool internal) const {
  if (internal)
    return mFields[index];
  return fieldsOrParameters().at(index);
}

WbField *WbNode::findField(const QString &fieldName, bool internal) const {
  const QVector<WbField *> &l = internal ? mFields : fieldsOrParameters();

  foreach (WbField *const field, l)
    if (fieldName == field->name())
      return field;

  return NULL;
}

int WbNode::findFieldId(const QString &fieldName, bool internal) const {
  int counter = 0;
  const QVector<WbField *> &fields = internal ? mFields : fieldsOrParameters();
  foreach (const WbField *const field, fields) {
    if (field->name() == fieldName)
      return counter;
    ++counter;
  }
  return -1; /* not found */
}

int WbNode::fieldIndex(const WbField *field) const {
  const QVector<WbField *> &list = fieldsOrParameters();
  return list.indexOf(const_cast<WbField *>(field));
}

// For PROTOs
int WbNode::parameterIndex(const WbField *field) const {
  const QVector<WbField *> &parameterList = parameters();
  return parameterList.indexOf(const_cast<WbField *>(field));
}

// Retrieves the field in which this node sits and returns the index of the node within this field
WbField *WbNode::parentFieldAndIndex(int &index, bool internal) const {
  index = -1;
  const WbNode *const parent = parentNode();
  if (!parent)
    return NULL;

  const QVector<WbField *> &fields = internal ? parent->fields() : parent->fieldsOrParameters();
  foreach (WbField *const field, fields) {
    const WbSFNode *const sfnode = dynamic_cast<WbSFNode *>(field->value());
    if (sfnode && sfnode->value() == this) {
      index = 0;
      return field;
    }
    const WbMFNode *const mfnode = dynamic_cast<WbMFNode *>(field->value());
    if (mfnode) {
      index = mfnode->nodeIndex(this);
      if (index != -1)
        return field;
    }
  }

  return NULL;
}

WbValue *WbNode::findValue(const QString &fieldName) const {
  foreach (WbField *const field, mFields) {
    if (fieldName == field->name())
      return field->value();
  }
  return NULL;
}

WbSFString *WbNode::findSFString(const QString &fieldName) const {
  return dynamic_cast<WbSFString *>(findValue(fieldName));
}

WbSFInt *WbNode::findSFInt(const QString &fieldName) const {
  return dynamic_cast<WbSFInt *>(findValue(fieldName));
}

WbSFDouble *WbNode::findSFDouble(const QString &fieldName) const {
  return dynamic_cast<WbSFDouble *>(findValue(fieldName));
}

WbSFVector2 *WbNode::findSFVector2(const QString &fieldName) const {
  return dynamic_cast<WbSFVector2 *>(findValue(fieldName));
}

WbSFVector3 *WbNode::findSFVector3(const QString &fieldName) const {
  return dynamic_cast<WbSFVector3 *>(findValue(fieldName));
}

WbSFColor *WbNode::findSFColor(const QString &fieldName) const {
  return dynamic_cast<WbSFColor *>(findValue(fieldName));
}

WbSFNode *WbNode::findSFNode(const QString &fieldName) const {
  return dynamic_cast<WbSFNode *>(findValue(fieldName));
}

WbSFBool *WbNode::findSFBool(const QString &fieldName) const {
  return dynamic_cast<WbSFBool *>(findValue(fieldName));
}

WbSFRotation *WbNode::findSFRotation(const QString &fieldName) const {
  return dynamic_cast<WbSFRotation *>(findValue(fieldName));
}

WbMFString *WbNode::findMFString(const QString &fieldName) const {
  return dynamic_cast<WbMFString *>(findValue(fieldName));
}

WbMFInt *WbNode::findMFInt(const QString &fieldName) const {
  return dynamic_cast<WbMFInt *>(findValue(fieldName));
}

WbMFDouble *WbNode::findMFDouble(const QString &fieldName) const {
  return dynamic_cast<WbMFDouble *>(findValue(fieldName));
}

WbMFVector2 *WbNode::findMFVector2(const QString &fieldName) const {
  return dynamic_cast<WbMFVector2 *>(findValue(fieldName));
}

WbMFVector3 *WbNode::findMFVector3(const QString &fieldName) const {
  return dynamic_cast<WbMFVector3 *>(findValue(fieldName));
}

WbMFBool *WbNode::findMFBool(const QString &fieldName) const {
  return dynamic_cast<WbMFBool *>(findValue(fieldName));
}

WbMFRotation *WbNode::findMFRotation(const QString &fieldName) const {
  return dynamic_cast<WbMFRotation *>(findValue(fieldName));
}

WbMFColor *WbNode::findMFColor(const QString &fieldName) const {
  return dynamic_cast<WbMFColor *>(findValue(fieldName));
}

WbMFNode *WbNode::findMFNode(const QString &fieldName) const {
  return dynamic_cast<WbMFNode *>(findValue(fieldName));
}

void WbNode::makeUseNode(WbNode *defNode, bool reading) {
  assert(defNode && defNode->isDefNode());

  mDefName = "";  // A USE node cannot be a DEF node at the same time
  mUseName = defNode->defName();

  if (reading)
    return;

  if (mDefNode) {
    mDefNode->mUseNodes.removeOne(this);  // Unsubscribes from updates of the previous referred DEF node
    mDefNode->useNodesChanged();
  }

  mDefNode = defNode;

  if (!mUseNodes.empty()) {
    mUseNodes.clear();
    useNodesChanged();
  }

  mDefNode->mUseNodes.append(this);  // Subscribes to updates of the referred DEF node
  mDefNode->useNodesChanged();
}

// Turns an orphan USE node into a DEF node with the same name
void WbNode::makeDefNode() {
  if (mDefNode) {  // Unsubscribes from previous DEF node
    mDefNode->mUseNodes.removeOne(this);
    mDefNode->useNodesChanged();
  }
  mDefName = mUseName;
  mUseName = "";
  mDefNode = NULL;

  if (parentNode() && parentNode()->mHasUseAncestor)
    return;

  // reset mHasUseAncestor flag in descendant nodes
  resetUseAncestorFlag();

  emit defUseNameChanged(this, true);
}

void WbNode::resetUseAncestorFlag() {
  if (!mHasUseAncestor || mDefNode)
    return;

  mHasUseAncestor = false;
  QList<WbNode *> subNodeList = subNodes(false);
  foreach (WbNode *const n, subNodeList)
    n->resetUseAncestorFlag();
}

// called after any field of this node has changed
void WbNode::notifyFieldChanged() {
  // this is the changed field
  WbField *const field = static_cast<WbField *>(sender());

  if (mIsBeingDeleted || cUpdatingDictionary) {
    emit fieldChanged(field);
    return;
  }

  // see if this node or any of its ancestors is a DEF node
  // in which case we must propagate the change to all matching USE nodes
  // note that there can be several DEF ancestors, and each must be treated
  WbNode *n = this;
  do {
    // is n a DEF node with USE nodes ?
    if (!n->mUseNodes.isEmpty()) {
      // find where the changed field is located in the DEF node
      int index = n->findSubFieldIndex(field);
      if (index >= 0) {
        WbNode *parent = NULL;
        // apply changes to the same field in each USE node
        foreach (WbNode *const useNode, n->mUseNodes) {
          WbField *const subField = useNode->findSubField(index, parent);
          if (!subField || subField->type() != field->type() || subField->name() != field->name())
            continue;
          assert(parent);
          setGlobalParentNode(parent);
          subField->copyValueFrom(field);
          subField->defHasChanged();
        }

        setGlobalParentNode(NULL);
      } else
        // index could be invalid for fields in sub-PROTO scope
        // but in this case the USE node will be updated by the parameter change notification
        break;
    }

    // climb up the family tree but stop if child insertion is not completed
    WbNode *p = n->parentNode();
    if (!p || !n->mInsertionCompleted)
      break;
    n = p;
  } while (!n->isWorldRoot());

  emit fieldChanged(field);
}

void WbNode::notifyParameterChanged() {
  WbField *const parameter = static_cast<WbField *>(sender());

  emit parameterChanged(parameter);
}

int WbNode::findSubFieldIndex(const WbField *const searched) const {
  int count = 0;
  QList<WbNode *> list(subNodes(true, false, false));
  list.prepend(const_cast<WbNode *>(this));
  foreach (WbNode *const node, list) {
    foreach (WbField *const field, node->mFields) {
      if (field == searched)
        return count;
      ++count;
    }
  }
  return -1;
}

void WbNode::disconnectInternalNode() {
  QVector<WbField *> parameterList = parameters();
  for (int i = 0; i < parameterList.size(); ++i) {
    parameterList[i]->disconnectField();
    disconnect(parameterList[i], &WbField::valueChanged, parentNode(), &WbNode::notifyParameterChanged);
  }

  QVector<WbField *> fieldList = fields();
  for (int i = 0; i < fieldList.size(); ++i) {
    fieldList[i]->disconnectField();
    disconnect(fieldList[i], &WbField::valueChanged, parentNode(), &WbNode::notifyFieldChanged);
  }
}

WbField *WbNode::findSubField(int index, WbNode *&parent) const {
  int count = 0;
  QList<WbNode *> list(subNodes(true, false, false));
  list.prepend(const_cast<WbNode *>(this));
  foreach (WbNode *const node, list) {
    foreach (WbField *const field, node->mFields) {
      if (count == index) {
        parent = node;
        return field;
      }
      ++count;
    }
  }
  return NULL;
}

void WbNode::validate(const WbNode *upperNode, const WbField *upperField, bool isInBoundingObject) const {
  if (isUseNode())
    return;
  const WbNode *parent = upperNode == NULL ? this : upperNode;
  QVector<WbField *> fieldsToBeValidated;
  if (upperField)
    fieldsToBeValidated << const_cast<WbField *>(upperField);
  else
    fieldsToBeValidated = fields();

  foreach (const WbField *field, fieldsToBeValidated) {
    WbSFNode *const sfnode = dynamic_cast<WbSFNode *>(field->value());
    WbMFNode *const mfnode = dynamic_cast<WbMFNode *>(field->value());
    if (sfnode) {
      if (field->name() == "boundingObject")
        isInBoundingObject = true;
      WbNode *child = sfnode->value();
      if (child) {
        QString errorMessage;
        if (!WbNodeFactory::instance()->validateExistingChildNode(field, child, parent, isInBoundingObject, errorMessage)) {
          bool emptyErrorMessage = errorMessage.isEmpty();
          if (emptyErrorMessage)
            errorMessage = QString("Skipped unexpected %1 node in '%2' field of %3 node.")
                             .arg(child->nodeModelName(), field->name(), this->nodeModelName());
          else if (upperNode)
            errorMessage.append(tr(" Using 'Slot' nodes mechanism."));

          WbField *parameter = field->parameter();
          if (parameter) {
            WbSFNode *sfParameter = dynamic_cast<WbSFNode *>(parameter->value());
            assert(sfParameter);
            sfParameter->setValue(NULL);
            if (!emptyErrorMessage)
              errorMessage.append(tr(" Skipped node in PROTO parameter '%1'.").arg(parameter->name()));
          } else {
            sfnode->setValue(NULL);
            if (!emptyErrorMessage)
              errorMessage.prepend(tr(" Skipped node: "));
          }

          parsingWarn(errorMessage);
        } else
          child->validate(NULL, NULL, isInBoundingObject);
      }
    } else if (mfnode) {
      for (int i = 0; i < mfnode->size(); ++i) {
        WbNode *child = mfnode->item(i);
        QString errorMessage;
        if (!WbNodeFactory::instance()->validateExistingChildNode(field, child, parent, isInBoundingObject, errorMessage)) {
          bool emptyErrorMessage = errorMessage.isEmpty();
          if (emptyErrorMessage)
            errorMessage = QString("Skipped unexpected %1 node in '%2' field of %3 node.")
                             .arg(child->nodeModelName(), field->name(), this->nodeModelName());
          else if (upperNode)
            errorMessage.append(tr(" Using 'Slot' nodes mechanism."));

          WbField *parameter = field->parameter();
          if (parameter) {
            WbMFNode *mfParameter = dynamic_cast<WbMFNode *>(parameter->value());
            assert(mfParameter);
            mfParameter->removeItem(i);
            if (!emptyErrorMessage)
              errorMessage.append(tr(" Skipped node in PROTO parameter '%1'.").arg(parameter->name()));
          } else {
            mfnode->removeItem(i);
            if (!emptyErrorMessage)
              errorMessage.prepend(tr("Skipped node: "));
          }

          --i;
          parsingWarn(errorMessage);
        } else
          child->validate(NULL, NULL, isInBoundingObject);
      }
    }
  }
}

void WbNode::readFieldValue(WbField *field, WbTokenizer *tokenizer, const QString &worldPath) const {
  if (field->name() == "boundingObject")
    WbNodeReader::current()->setReadingBoundingObject(true);

  field->readValue(tokenizer, worldPath);

  if (field->name() == "boundingObject")
    WbNodeReader::current()->setReadingBoundingObject(false);
}

void WbNode::readFields(WbTokenizer *tokenizer, const QString &worldPath) {
  gParent = this;
  tokenizer->skipToken("{");

  while (tokenizer->peekWord() != "}") {
    const QString &w(tokenizer->nextWord());
    WbField *const field = findField(w);
    if (!field)
      tokenizer->skipField();
    else {
      if (tokenizer->peekWord() == "IS") {
        tokenizer->skipToken("IS");
        const QString &alias = tokenizer->nextWord();
        // qDebug() << field->name() << "->" << alias;
        field->setAlias(alias);
        copyAliasValue(field, alias);
      } else {
        // field->readValue(tokenizer);
        readFieldValue(field, tokenizer, worldPath);
      }
    }
  }

  tokenizer->skipToken("}");
  gParent = parentNode();
}

void WbNode::enableDefNodeTrackInWrite(bool substituteInStream) {
  assert(gInternalDefNamesInWrite == NULL && gExternalUseNodesInWrite == NULL);
  gInternalDefNamesInWrite = new QStringList();
  if (!substituteInStream)
    gExternalUseNodesInWrite = new QList<QPair<WbNode *, int>>();
}

void WbNode::disableDefNodeTrackInWrite() {
  delete gInternalDefNamesInWrite;
  delete gExternalUseNodesInWrite;
  gInternalDefNamesInWrite = NULL;
  gExternalUseNodesInWrite = NULL;
}

QList<QPair<WbNode *, int>> *WbNode::externalUseNodesPositionsInWrite() {
  return gExternalUseNodesInWrite;
}

void WbNode::writeParameters(WbVrmlWriter &writer) const {
  foreach (WbField *parameter, parameters())
    parameter->write(writer);
}

bool WbNode::isUrdfRootLink() const {
  return findSFString("name") ? true : false;
}

WbNode *WbNode::findUrdfLinkRoot() const {
  WbNode *parentRoot = parentNode();
  while (!parentRoot->isUrdfRootLink()) {
    parentRoot = parentRoot->parentNode();
    if (parentRoot == NULL)
      return NULL;
  }
  return parentRoot;
}

void WbNode::write(WbVrmlWriter &writer) const {
  if (uniqueId() == -1) {
    if (nodeModelName() == "Plane" || nodeModelName() == "Capsule") {
      WbNodeFactory::instance()->exportAsVrml(this, writer);
      return;
    }
  }
  if (writer.isUrdf()) {
    // Start naming from scratch
    if (isRobot()) {
      gUrdfNames.clear();
      gUrdfNameIndex = 0;
    }

    if (gUrdfCurrentNode != this && isJoint() && !gUrdfNodesQueue.contains(this)) {
      gUrdfNodesQueue.append(this);
      return;
    }
    if (!gUrdfCurrentNode)
      gUrdfCurrentNode = this;

    writeExport(writer);

    if (gUrdfCurrentNode == this) {
      if (gUrdfNodesQueue.size() > 0) {
        gUrdfCurrentNode = gUrdfNodesQueue.takeLast();
        gUrdfCurrentNode->write(writer);
      } else
        gUrdfCurrentNode = NULL;
    }
    return;
  }
  if (writer.isX3d() || writer.isVrml() || (writer.isProto() && (!writer.rootNode() || this == writer.rootNode()))) {
    writeExport(writer);
    return;
  }

  if (gInternalDefNamesInWrite) {
    // handle DEF nodes defined outside the current export node in order to export a self-describing node
    if (isUseNode() && !gInternalDefNamesInWrite->contains(mUseName)) {
      if (gExternalUseNodesInWrite)
        // keep track of DEF node
        gExternalUseNodesInWrite->append(QPair<WbNode *, int>(mDefNode, writer.string()->size()));
      else {
        // write definition directly on the stream
        mDefNode->write(writer);
        return;
      }
      gInternalDefNamesInWrite->append(mUseName);
    } else if (isDefNode())
      gInternalDefNamesInWrite->append(mDefName);
  }

  writer << fullName();

  if (isUseNode())
    return;

  writer << " {\n";

  writer.increaseIndent();

  if (isProtoInstance())
    writeParameters(writer);
  else
    foreach (WbField *field, fields())
      if (!field->isDeprecated() && (!writer.isVrml() || field->isVrml()))
        field->write(writer);

  writer.decreaseIndent();
  writer.indent();
  writer << "}";
}

// This function lists only the texture files which are explicitly referred to in
// this world file and not the one implicitly referred to by included PROTO files.
// This list may contain duplicate texture files.
QStringList WbNode::listTextureFiles() const {
  QStringList list;
  bool imageTexture = model()->name() == "ImageTexture";
  const QString currentTexturePath = WbProject::current()->worldsPath();
  foreach (WbField *field, fields())
    if (field->value()->type() == WB_SF_NODE) {
      WbSFNode *node = dynamic_cast<WbSFNode *>(field->value());
      if (node->value())
        list << node->value()->listTextureFiles();
    } else if (field->value()->type() == WB_MF_NODE) {
      WbMFNode *mfnode = dynamic_cast<WbMFNode *>(field->value());
      WbMFNode::Iterator it(*mfnode);
      while (it.hasNext()) {
        const WbNode *n = static_cast<WbNode *>(it.next());
        list << n->listTextureFiles();
      }
    } else if (imageTexture && field->value()->type() == WB_MF_STRING && field->name() == "url") {
      WbNode *proto = protoAncestor();
      QString protoPath;
      if (proto) {
        WbProtoModel *protoModel = proto->proto();
        QFileInfo fileInfo(protoModel->fileName());
        protoPath = fileInfo.path() + "/";
      }
      WbMFString *mfstring = dynamic_cast<WbMFString *>(field->value());
      for (int i = 0; i < mfstring->size(); i++) {
        const QString &textureFile = mfstring->item(i);
        if (proto && QFile::exists(protoPath + textureFile))  // PROTO texture
          continue;                                           // skip it
        if (QFile::exists(currentTexturePath + textureFile))
          list << textureFile;
      }
    }
  return list;
}

const QString WbNode::urdfName() const {
  // Use existing name if already given
  if (gUrdfNames.contains(uniqueId()))
    return gUrdfNames[uniqueId()];

  // Name the link/joint according to priority: name -> def -> model
  QString name;
  if (this->findSFString("name") && this->findSFString("name")->value() != "")
    name = this->findSFString("name")->value();
  else if (this->defName() != "")
    name = this->defName();
  else
    name = QString(mModel->name().toLower());
  QString fullName = getUrdfPrefix() + name;

  // Add suffix if needed
  if (gUrdfNames.values().contains(fullName))
    fullName += "_" + QString::number(gUrdfNameIndex++);

  // Return
  gUrdfNames[uniqueId()] = fullName;
  return fullName;
}

bool WbNode::exportNodeHeader(WbVrmlWriter &writer) const {
  if (writer.isX3d())  // actual export is done in WbBaseNode
    return false;
  else if (writer.isUrdf()) {
    if (gUrdfCurrentNode == this) {
      writer.increaseIndent();
      writer.indent();
      writer << "<link name=\"" + urdfName() + "\">\n";
      return false;
    } else if (isUrdfRootLink()) {
      gUrdfNodesQueue.append(this);
      return true;
    }
    return false;
  }
  if (isUseNode()) {
    writer << "USE " << mUseName << "\n";
    return true;
  }
  if (writer.isVrml())
    writer << fullVrmlName();
  else {
    if (isDefNode())
      writer << "DEF " << defName() << " ";
    writer << nodeModelName();
  }
  writer << " {\n";
  writer.increaseIndent();
  return false;
}

void WbNode::exportNodeFields(WbVrmlWriter &writer) const {
  if (writer.isUrdf())
    return;

  foreach (WbField *field, fields()) {
    if (!field->isDeprecated() && ((field->isVrml() || writer.isProto()) && field->singleType() != WB_SF_NODE))
      field->write(writer);
  }
}

void WbNode::exportNodeSubNodes(WbVrmlWriter &writer) const {
  foreach (WbField *field, fields()) {
    if (!field->isDeprecated() &&
        ((field->isVrml() || writer.isProto() || writer.isUrdf()) && field->singleType() == WB_SF_NODE)) {
      const WbSFNode *const node = dynamic_cast<WbSFNode *>(field->value());
      if (node == NULL || node->value() == NULL || node->value()->shallExport() || writer.isProto() || writer.isUrdf()) {
        if (writer.isX3d() || writer.isUrdf())
          field->value()->write(writer);
        else
          field->write(writer);
      }
    }
  }
}

void WbNode::exportNodeFooter(WbVrmlWriter &writer) const {
  if (writer.isX3d())
    writer << "</" << x3dName() << ">";
  else if (writer.isUrdf()) {
    if (gUrdfCurrentNode == this) {
      writer.indent();
      writer << "</link>\n";
      writer.decreaseIndent();
    }
  } else {  // VRML
    writer.decreaseIndent();
    writer.indent();
    writer << "}";
  }
}

void WbNode::exportNodeContents(WbVrmlWriter &writer) const {
  exportNodeFields(writer);
  if (writer.isX3d())
    writer << ">";
  exportNodeSubNodes(writer);
}

void WbNode::writeExport(WbVrmlWriter &writer) const {
  assert(!(writer.isX3d() && isProtoParameterNode()));
  if (exportNodeHeader(writer))
    return;
  if (writer.isUrdf()) {
    exportNodeSubNodes(writer);
    exportNodeFooter(writer);
    if (isUrdfRootLink() && nodeModelName() != "Robot")
      exportURDFJoint(writer);
  } else {
    exportNodeContents(writer);
    exportNodeFooter(writer);
  }
}

bool WbNode::operator==(const WbNode &other) const {
  if (mModel != other.mModel || isProtoInstance() != other.isProtoInstance() ||
      (mProto && mProto->name() != other.mProto->name()) || mDefName != other.mDefName)
    return false;

  if (this == &other)
    return true;

  const QVector<WbField *> fields = fieldsOrParameters();
  const QVector<WbField *> otherFields = other.fieldsOrParameters();
  const int size = fields.size();
  assert(size == otherFields.size());
  for (int i = 0; i < size; ++i) {
    const WbField *const f1 = fields[i];
    const WbField *const f2 = otherFields[i];
    if (!(f1->isDeprecated() || f1->value()->equals(f2->value())))
      return false;
  }

  return true;
}

bool WbNode::isDefault() const {
  QVector<WbField *> fields = fieldsOrParameters();
  foreach (WbField *field, fields) {
    if (!(field->isDeprecated() || field->isDefault()))
      return false;
  }

  return true;
}

void WbNode::removeFromFieldsOrParameters(WbField *item) {
  if (isProtoInstance())
    mParameters.removeAll(item);
  else
    mFields.removeAll(item);
}

// recursively search for matching IS fields/parameters and redirect them to the PROTO parameter
// the search does not look up inside fields of other PROTO instances or PROTO parameter node instances
// because the scope of a PROTO parameter must be local to a PROTO instance
// but, when loading from file, it looks up in the parameters of direct PROTO instances in order to pass a parameter to a sub
// PROTO
void WbNode::redirectAliasedFields(WbField *param, WbNode *protoInstance, bool searchInParameters, bool parametersOnly,
                                   bool copyValueOnly) {
  QVector<WbField *> fields;
  if (searchInParameters) {
    // search for matching IS fields in subPROTO declaration
    // and redirect them to the upper PROTO parameter
    if (this != protoInstance && isProtoInstance())
      fields = mParameters;
  } else
    fields = mFields;

  // search self
  foreach (WbField *field, fields) {
    if (field->alias() == param->name() && field->type() == param->type()) {
      // set parent node
      WbNode *tmpParent = gParent;
      gParent = this;
      bool tmpProtoFlag = gProtoParameterNodeFlag;
      if (copyValueOnly) {
        field->copyValueFrom(param);
        // reset alias value so that the value is copied when node is cloned
        // this is needed for derived PROTO nodes linked to a default base PROTO parameter
        field->setAlias(QString());
      } else {
        gProtoParameterNodeFlag = true;
        field->redirectTo(param);
        gProtoParameterNodeFlag = tmpProtoFlag;
      }
      gParent = tmpParent;
    }
  }

  // do not search in sub nodes of a sub PROTO node
  foreach (WbNode *node, subNodes(false, !parametersOnly, searchInParameters)) {
    if (node->isProtoInstance()) {
      // search also in parameters of direct sub PROTO nodes
      node->redirectAliasedFields(param, protoInstance, true, true, copyValueOnly);
    } else {
      // do not search in fields of PROTO parameter node instances
      WbNode *protoParameterNode = node->protoParameterNode();
      if (!protoParameterNode || !protoParameterNode->isProtoInstance())
        node->redirectAliasedFields(param, protoInstance, false, false, copyValueOnly);
    }
  }
}

// recursively search for matching IS fields and change alias name
void WbNode::swapFieldAlias(const QString &oldAlias, WbField *newParam, bool searchInParameters) {
  QVector<WbField *> fields(searchInParameters ? mParameters : mFields);

  // search self
  foreach (WbField *field, fields) {
    if (field->alias() == oldAlias && field->type() == newParam->type())
      field->setAlias(newParam->name());
  }

  // do not search in sub nodes of a sub PROTO node
  foreach (WbNode *node, subNodes(false, !searchInParameters, true)) {
    if (node->isProtoInstance()) {
      // search in parameters of direct sub PROTO nodes
      node->swapFieldAlias(oldAlias, newParam, true);
    } else {
      // do not search in fields of PROTO parameter node instances
      WbNode *protoParameterNode = node->protoParameterNode();
      if (!protoParameterNode || !protoParameterNode->isProtoInstance())
        node->swapFieldAlias(oldAlias, newParam, false);
    }
  }
}

WbNode *WbNode::cloneDefNode() {
  gDefCloneFlag = true;
  WbNode *copy = clone();
  gDefCloneFlag = false;

  return copy;
}

WbNode *WbNode::cloneAndReferenceProtoInstance() {
  WbNode *copy = clone();

  if (copy && !copy->mHasUseAncestor && mIsProtoDescendant && gProtoParameterNodeFlag) {
    // associate instance with respective PROTO parameter node
    // DEF/USE nodes should not be associated
    mProtoParameterNodeInstances.append(copy);
    connect(copy, &QObject::destroyed, this, &WbNode::removeProtoParameterNodeInstance);
    copy->mProtoParameterNode = this;

    if (mIsNestedProtoNode) {
      // redirect fields of PROTO parameter node instance to the corresponding parameters of PROTO parameter node
      foreach (WbField *protoParam, mParameters) {
        WbField *alias = protoParam;
        if (!mIsTopParameterDescendant) {
          foreach (WbField *copyParam, copy->mParameters) {
            if (protoParam->name() == copyParam->name() && copyParam->type() == protoParam->type())
              alias = copyParam;
          }
        }
        copy->redirectAliasedFields(alias, this);
      }
    }
  }

  return copy;
}

void WbNode::removeProtoParameterNodeInstance(QObject *node) {
  QMutableVectorIterator<WbNode *> it(mProtoParameterNodeInstances);
  while (it.hasNext()) {
    if (it.next() == static_cast<WbNode *>(node))
      it.remove();
  }
}

WbNode *WbNode::clone() const {
  // if clone() is called while reading a .proto file we create a lightweight node
  // to be placed in the PROTO model
  if (!gInstantiateMode)
    return new WbNode(*this);

  // otherwise we need to instantiate the node for a PROTO instance
  WbNode *const copy = WbNodeFactory::instance()->createCopy(*this);
  if (!copy)
    parsingWarn(tr("Could not instantiate '%1' node: this class is not yet implemented in Webots.").arg(model()->name()));

  return copy;
}

void WbNode::copyAliasValue(WbField *field, const QString &alias) {
  // instantiate if possible the alias parameter from the parent PROTO.
  // this avoids double setup of the parameter and is particularly
  // helpful for nested PROTOs
  if (!gProtoParameterList.isEmpty()) {
    foreach (WbField *p, *(gProtoParameterList.last()->params))
      if (p->name() == alias && p->type() == field->type()) {
        bool previousFlag = gProtoParameterNodeFlag;
        if (!gDerivedProtoParentFlag)
          gProtoParameterNodeFlag = true;
        field->copyValueFrom(p);
        gProtoParameterNodeFlag = previousFlag;
      }
  }
}

WbNode *WbNode::createProtoInstance(WbProtoModel *proto, WbTokenizer *tokenizer, const QString &worldPath) {
  static int protoLevel = -1;

  const bool previousDerivedProtoAncestor = gDerivedProtoAncestorFlag;
  const bool previousDerivedProtoFlag = gDerivedProtoParentFlag;
  gDerivedProtoParentFlag = gDerivedProtoFlag;
  gDerivedProtoFlag = proto->isDerived();

  const int previousProtoLevel = protoLevel;
  if (!proto->isDerived()) {
    if (protoLevel < 1 && gProtoParameterNodeFlag)
      // PROTO parameter node of a derived PROTO
      protoLevel = 1;
    else
      ++protoLevel;
  } else
    gDerivedProtoAncestorFlag = true;

  const bool previousTopParameterFlag = gTopParameterFlag;
  const bool topParameter = (previousProtoLevel == -1) &&
                            ((gProtoParameterList.size() < 1 && protoLevel == 0) || (proto->isDerived() && protoLevel == -1));
  bool insertedProto = false;  // node inserted from the scene tree
  if (previousProtoLevel == -1) {
    // detect if node inserted from the scene tree
    WbNode *root = NULL;
    WbNode *parent = gParent;
    if (!(parent && parent->isWorldRoot())) {
      while (parent) {
        root = parent;
        parent = root->parentNode();
      }
      insertedProto = root && root->mIsCreationCompleted;
    }
  }
  if (topParameter || insertedProto)
    gTopParameterFlag = true;

  // 1. get PROTO's field models
  const QList<WbFieldModel *> &protoFieldModels = proto->fieldModels();

  // 2. create the parameters list from the model (default values)
  QVector<WbField *> parameters;
  QVector<QMap<QString, WbNode *>> parametersDefMap;
  bool hasDefaultDefNodes = false;
  QListIterator<WbFieldModel *> fieldModelsIt(protoFieldModels);
  while (fieldModelsIt.hasNext()) {
    WbField *defaultParameter = new WbField(fieldModelsIt.next(), NULL);
    parameters.append(defaultParameter);

    parametersDefMap.append(QMap<QString, WbNode *>());
    if (tokenizer && WbNodeReader::current()) {
      // extract DEF nodes defined in default PROTO parameter
      QList<WbNode *> defNodes = subNodes(defaultParameter, true, false, false);
      QListIterator<WbNode *> defNodesIt(defNodes);
      while (defNodesIt.hasNext()) {
        WbNode *node = defNodesIt.next();
        if (!node->defName().isEmpty())
          parametersDefMap.last().insert(node->defName(), node);
      }
      hasDefaultDefNodes = hasDefaultDefNodes || !parametersDefMap.last().isEmpty();
    }
  }

  const bool previousParameterNodeFlag = gProtoParameterNodeFlag;
  gProtoParameterNodeFlag = true;

  // 3. populate the parameters from the tokenizer if existing
  QSet<QString> parameterNames;
  if (tokenizer) {
    tokenizer->skipToken("{");

    int nextParameterIndex = 0;
    int currentParameterIndex = 0;
    bool fieldOrderWarning = true;
    while (tokenizer->peekWord() != "}") {
      QString parameterName = tokenizer->nextWord();
      WbFieldModel *parameterModel = NULL;
      const bool hidden = parameterName == "hidden";
      if (hidden) {
        static const QRegExp rx1("(_\\d+)+$");  // looks for a substring of the form _7 or _13_1 at the end of the parameter
                                                // name, e.g. as in rotation_7, position2_13_1
        const QString &hiddenParameterName(tokenizer->peekWord());
        const int pos1 = rx1.indexIn(hiddenParameterName);
        static const QRegExp rx2("^[A-Za-z]+\\d?");
        const int pos2 = rx2.indexIn(hiddenParameterName);
        tokenizer->ungetToken();
        if (pos1 != -1 && pos2 != -1 && cHiddenParameterNames.indexOf(rx2.cap(0)) != -1)
          parameterModel = new WbFieldModel(tokenizer, worldPath);
      } else {
        for (currentParameterIndex = 0; currentParameterIndex < protoFieldModels.size(); ++currentParameterIndex) {
          WbFieldModel *model = protoFieldModels.at(currentParameterIndex);
          if (parameterName == model->name()) {
            parameterModel = model;
            break;
          }
        }
      }

      if (hasDefaultDefNodes) {
        if (currentParameterIndex < nextParameterIndex) {
          // if the parameters are not listed using the order defined in the PROTO declaration the DEF map could be wrong
          if (fieldOrderWarning) {
            tokenizer->reportFileError(
              tr("Wrong order of fields in the instance of PROTO %1: USE nodes might refer to the wrong DEF nodes")
                .arg(proto->name()));
            fieldOrderWarning = false;
          }
          // remove DEF nodes from default parameter if any
          QMapIterator<QString, WbNode *> defNodesMapIt(parametersDefMap[currentParameterIndex]);
          while (defNodesMapIt.hasNext()) {
            defNodesMapIt.next();
            WbNodeReader::current()->removeDefNode(defNodesMapIt.value());
          }
        } else {
          // add DEF nodes from default parameter defined before the current parameter
          for (int i = nextParameterIndex; i < currentParameterIndex; ++i) {
            QMapIterator<QString, WbNode *> defNodesMapIt(parametersDefMap[i]);
            while (defNodesMapIt.hasNext()) {
              defNodesMapIt.next();
              WbNodeReader::current()->addDefNode(defNodesMapIt.value());
            }
          }
          nextParameterIndex = currentParameterIndex + 1;
        }
      }

      if (parameterModel) {
        WbField *parameter = new WbField(parameterModel, NULL);

        bool toBeDeleted = parameterNames.contains(parameter->name());
        if (toBeDeleted)
          // duplicated parameter definition to be ignored
          tokenizer->reportFileError(
            tr("Duplicated definition of field %1 in the instance of PROTO %2").arg(parameter->name()).arg(proto->name()));
        else {
          parameterNames << parameter->name();
          bool substitution = false;
          for (int i = 0; i < parameters.size(); ++i) {
            if (parameter->name() == parameters.at(i)->name() && parameter->type() == parameters.at(i)->type()) {
              delete parameters.at(i);
              parameters.replace(i, parameter);
              substitution = true;
              break;
            }
          }

          if (hidden)
            parameters.append(parameter);
          else if (substitution == false) {
            toBeDeleted = true;
            tokenizer->reportFileError(tr("Parameter %1 not supported in PROTO %2").arg(parameter->name()).arg(proto->name()));
          }
        }

        if (tokenizer->peekWord() == "IS") {
          tokenizer->skipToken("IS");
          const QString &alias = tokenizer->nextWord();
          if (!toBeDeleted) {
            parameter->setAlias(alias);
            copyAliasValue(parameter, alias);
          }
        } else if (!hidden)
          parameter->readValue(tokenizer, worldPath);

        if (toBeDeleted)
          delete parameter;
      } else
        tokenizer->reportFileError(tr("Parameter %1 not supported in PROTO %2").arg(parameterName).arg(proto->name()));
    }

    if (hasDefaultDefNodes) {
      // add DEF nodes from the last default parameters
      for (int i = nextParameterIndex; i < protoFieldModels.size(); ++i) {
        QMapIterator<QString, WbNode *> defNodesMapIt(parametersDefMap[i]);
        while (defNodesMapIt.hasNext()) {
          defNodesMapIt.next();
          WbNodeReader::current()->addDefNode(defNodesMapIt.value());
        }
      }
    }
    tokenizer->skipToken("}");
  }

  parametersDefMap.clear();
  gProtoParameterNodeFlag = previousParameterNodeFlag;
  gDerivedProtoFlag = gDerivedProtoParentFlag;
  gDerivedProtoParentFlag = previousDerivedProtoFlag;
  if (topParameter && !insertedProto)
    gTopParameterFlag = false;

  setupDescendantAndNestedProtoFlags(parameters, topParameter || insertedProto);
  WbNode *instance = createProtoInstanceFromParameters(proto, parameters, protoLevel < 1, worldPath);

  protoLevel = previousProtoLevel;
  if (proto->isDerived())
    gDerivedProtoAncestorFlag = previousDerivedProtoAncestor;
  if (insertedProto)
    gTopParameterFlag = previousTopParameterFlag;

  return instance;
}

WbNode *WbNode::regenerateProtoInstanceFromParameters(WbProtoModel *proto, const QVector<WbField *> &parameters,
                                                      bool isTopLevel, const QString &worldPath, bool fromSceneTree,
                                                      int uniqueId) {
  // setup global flags before PROTO instance creation
  const bool previousDerivedProtoAncestor = gDerivedProtoAncestorFlag;
  if (proto->isDerived())
    gDerivedProtoAncestorFlag = true;

  setupDescendantAndNestedProtoFlags(parameters, true);
  WbNode *instance = createProtoInstanceFromParameters(proto, parameters, isTopLevel, worldPath, fromSceneTree, uniqueId);

  // reset global flags
  if (proto->isDerived())
    gDerivedProtoAncestorFlag = previousDerivedProtoAncestor;

  return instance;
}

WbNode *WbNode::createProtoInstanceFromParameters(WbProtoModel *proto, const QVector<WbField *> &parameters, bool isTopLevel,
                                                  const QString &worldPath, bool fromSceneTree, int uniqueId) {
  ProtoParameters *p = new ProtoParameters;
  p->params = &parameters;
  gProtoParameterList << p;

  const bool previousFlag = gProtoParameterNodeFlag;
  gProtoParameterNodeFlag = false;

  bool prevInstantiateMode = instantiateMode();
  setInstantiateMode(false);
  WbNode *newNode = proto->generateRoot(parameters, worldPath, uniqueId);
  setInstantiateMode(prevInstantiateMode);
  if (!newNode) {
    gProtoParameterNodeFlag = previousFlag;
    delete gProtoParameterList.takeLast();
    return NULL;
  }
  proto->ref(true);

  WbNode *const instance = newNode->cloneAndReferenceProtoInstance();
  int id = newNode->uniqueId();  // we want to keep this id because it should match the 'context.id' value used when
                                 // generating procedural PROTO nodes
  delete newNode;

  gProtoParameterNodeFlag = true;

  instance->mProto = proto;
  if (id >= 0)
    instance->setUniqueId(id);

  QVector<WbField *> notAssociatedDerivedParameters;  // populated for derived PROTO only
  if (proto->isDerived()) {
    QMutableVectorIterator<WbField *> paramIt(instance->mParameters);
    while (paramIt.hasNext()) {
      WbField *param = paramIt.next();

      // search for alias parameter and remove intermediate parameters
      QVectorIterator<WbField *> aliasIt(parameters);
      bool remove = false;
      bool aliasNotFound = true;
      while (aliasIt.hasNext()) {
        WbField *aliasParam = aliasIt.next();
        if (aliasParam->name() == param->alias() && aliasParam->type() == param->type()) {
          aliasNotFound = false;
          bool aliasTemplate = aliasParam->isTemplateRegenerator();
          if (!aliasTemplate) {
            bool paramTemplate = param->isTemplateRegenerator();
            aliasParam->setTemplateRegenerator(paramTemplate);
            if (paramTemplate)
              instance->mProto->setIsTemplate(true);
          }

          WbNode *tmpParent = gParent;
          foreach (WbField *internalField, param->internalFields()) {
            gParent = internalField->parentNode();
            internalField->redirectTo(aliasParam);
            internalField->setAlias(aliasParam->name());
          }
          gParent = tmpParent;

          // swap alias name
          instance->swapFieldAlias(param->name(), aliasParam, false);

          remove = true;
        } else if (aliasParam->name() == param->name() && aliasParam->type() == param->type()) {
          // homonymous derived and base parameters
          notAssociatedDerivedParameters.append(aliasParam);
          aliasNotFound = false;
        }
      }

      if (remove) {
        paramIt.remove();
        param->clearInternalFields();
        delete param;
      }

      if (aliasNotFound)
        // base PROTO parameter not overwritten by derived PROTO parameter
        // copy values from base PROTO default parameter
        instance->redirectAliasedFields(param, instance, false, false, true);
    }
  }

  foreach (WbField *parameter, parameters) {
    // remove first the parameters in case of direct nested PROTOs
    QMutableVectorIterator<WbField *> it(instance->mParameters);
    while (it.hasNext()) {
      WbField *field = it.next();
      if (field->name() == parameter->name() && field->type() == parameter->type()) {
        it.remove();
        delete field;
      }
    }

    parameter->setParentNode(instance);
    instance->mParameters.append(parameter);
    connect(parameter, &WbField::valueChanged, instance, &WbNode::notifyParameterChanged);

    // set the parent of the parameter nodes
    switch (parameter->type()) {
      case WB_MF_NODE: {
        WbMFNode *mfnode = static_cast<WbMFNode *>(parameter->value());
        assert(mfnode);
        for (int i = 0; i < mfnode->size(); i++) {
          WbNode *subnode = mfnode->item(i);
          subnode->setParentNode(instance);
        }
        break;
      }
      case WB_SF_NODE: {
        WbSFNode *sfnode = static_cast<WbSFNode *>(parameter->value());
        assert(sfnode);
        WbNode *subnode = sfnode->value();
        if (subnode)
          subnode->setParentNode(instance);
        break;
      }
      default:
        break;
    }
  }

  // these tests are because of the multiple possible contexts to pass in this function (e.g. regular load versus add node
  // from scene tree gui)
  bool topProto = isTopLevel && gProtoParameterList.size() <= 1 && !instance->hasAProtoAncestor();
  instance->setupDescendantAndNestedProtoFlags(topProto, false, fromSceneTree);

  // removed the fake parameters introduced in case of direct nested PROTOs
  QMutableVectorIterator<WbField *> fieldIt(instance->mParameters);
  while (fieldIt.hasNext()) {
    WbField *field = fieldIt.next();
    if (!field->isHiddenParameter() && proto->findFieldModel(field->name()) == NULL) {
      fieldIt.remove();
      delete field;
    }
  }

  // connect internal PROTO parameter to parent parameters
  foreach (WbField *parameter, instance->mParameters) {
    if (!(proto->isDerived() && notAssociatedDerivedParameters.contains(parameter)))
      instance->redirectAliasedFields(parameter, instance, true);
  }

  // don't connect fields and parameters of nested PROTO instance
  // parameters will be directly connected with the PROTO parameter node instances
  if (fromSceneTree || !instance->mIsNestedProtoNode) {
    // connect fields to PROTO parameters
    foreach (WbField *parameter, instance->mParameters) {
      if (!(proto->isDerived() && notAssociatedDerivedParameters.contains(parameter)))
        instance->redirectAliasedFields(parameter, instance, false);
    }
  }

  gProtoParameterNodeFlag = previousFlag;

  delete gProtoParameterList.takeLast();

  if (!instance->mIsNestedProtoNode) {
    QMutableVectorIterator<WbField *> it(instance->mParameters);
    while (it.hasNext()) {
      WbField *parameter = it.next();
      if (parameter->isHiddenParameter()) {
        instance->readHiddenKinematicParameter(parameter);
        it.remove();
        delete parameter;
      }
    }
  }
  return instance;
}

void WbNode::setProtoInstanceTemplateContent(const QByteArray &content) {
  if (!mProtoInstanceFilePath.isEmpty() && QFile::exists(mProtoInstanceFilePath))
    QFile::remove(mProtoInstanceFilePath);
  mProtoInstanceFilePath.clear();
  mProtoInstanceTemplateContent = content;
}

const QString &WbNode::protoInstanceFilePath() {
  if (mProtoInstanceFilePath.isEmpty() && !mProtoInstanceTemplateContent.isEmpty()) {
    // QTemporaryFile is used to generate a good temporary file name
    // a reason for this is that the temporary file should exists during all the node life cycle
    QTemporaryFile tmpFile(QString("%1/%2.XXXXXX.proto").arg(WbStandardPaths::webotsTmpPath()).arg(proto()->name()));
    tmpFile.open();
    tmpFile.setAutoRemove(false);
    tmpFile.write(mProtoInstanceTemplateContent);
    mProtoInstanceFilePath = tmpFile.fileName();
    tmpFile.close();
  }
  return mProtoInstanceFilePath;
}

void WbNode::updateNestedProtoFlag() {
  mIsNestedProtoNode = isProtoInstance() && hasAProtoAncestor();
  foreach (WbNode *instance, protoParameterNodeInstances())
    instance->updateNestedProtoFlag();
}

void WbNode::setupDescendantAndNestedProtoFlags(bool isTopNode, bool isTopParameterDescendant, bool isInsertedFromSceneTree) {
  mIsProtoDescendant = !isTopNode;
  mIsNestedProtoNode = !(isTopNode || isInsertedFromSceneTree) && isProtoInstance();
  if (isTopParameterDescendant)
    mIsTopParameterDescendant = true;
  setupDescendantAndNestedProtoFlags(fields() + parameters(), isTopParameterDescendant);
}

void WbNode::setupDescendantAndNestedProtoFlags(QVector<WbField *> fields, bool isTopParameterDescendant) {
  foreach (WbField *field, fields) {
    const WbSFNode *const sfnode = dynamic_cast<WbSFNode *>(field->value());
    const WbMFNode *const mfnode = dynamic_cast<WbMFNode *>(field->value());
    if (sfnode) {
      WbNode *n = sfnode->value();
      if (n)
        n->setupDescendantAndNestedProtoFlags(false, isTopParameterDescendant, false);
    } else if (mfnode) {
      for (int i = 0; i < mfnode->size(); ++i)
        mfnode->item(i)->setupDescendantAndNestedProtoFlags(false, isTopParameterDescendant, false);
    }
  }
}

void WbNode::setCreationCompleted() {
  mIsProtoParameterNodeDescendant = isProtoParameterNode();
  mIsCreationCompleted = true;
}

void WbNode::reset(const QString &id) {
  mCurrentStateId = id;
  if (isTemplate() && !mProto->isDeterministic())
    // nonDeterministic procedural PROTO must be regenerated on reset
    setRegenerationRequired(true);
}

bool WbNode::isProtoParameterChild(const WbNode *node) const {
  if (!isProtoInstance())
    return false;

  foreach (WbField *const field, parameters()) {
    const WbSFNode *const sfnode = dynamic_cast<WbSFNode *>(field->value());
    if (sfnode && sfnode->value() == node)
      return true;
    const WbMFNode *const mfnode = dynamic_cast<WbMFNode *>(field->value());
    if (mfnode && mfnode->nodeIndex(node) != -1)
      return true;
  }

  return false;
}

bool WbNode::isProtoParameterNode() const {
  if (mIsCreationCompleted)
    return mIsProtoParameterNodeDescendant;

  WbNode *parent = parentNode();
  if (!parent || parent->isWorldRoot())
    return false;

  if (parent->isProtoParameterChild(this))
    return true;

  return parent->isProtoParameterNode();
}

QList<WbNode *> WbNode::subNodes(bool recurse, bool searchInFields, bool searchInParameters) const {
  QList<WbNode *> result;
  QVector<WbField *> fields;
  // first add the parameters and then the fields
  if (searchInParameters)
    fields += mParameters;
  if (searchInFields)
    fields += mFields;
  if (!searchInFields && !searchInParameters)
    fields += fieldsOrParameters();

  QVector<WbField *>::iterator fieldIt;
  for (fieldIt = fields.begin(); fieldIt != fields.end(); ++fieldIt)
    result.append(subNodes((*fieldIt), recurse, searchInFields, searchInParameters));
  return result;
}

QList<WbNode *> WbNode::subNodes(const WbField *field, bool recurse, bool searchInFields, bool searchInParameters) {
  QList<WbNode *> result;
  const WbValue *const value = field->value();
  const WbSFNode *const sfnode = dynamic_cast<const WbSFNode *>(value);
  if (sfnode && sfnode->value()) {
    WbNode *const node = sfnode->value();
    result.append(node);
    if (recurse)
      result.append(node->subNodes(recurse, searchInFields, searchInParameters));
  } else {
    const WbMFNode *const mfnode = dynamic_cast<const WbMFNode *>(value);
    if (mfnode) {
      for (int i = 0; i < mfnode->size(); ++i) {
        WbNode *const node = mfnode->item(i);
        result.append(node);
        if (recurse)
          result.append(node->subNodes(recurse, searchInFields, searchInParameters));
      }
    }
  }

  return result;
}

bool WbNode::isTemplate() const {
  if (mProto)
    return mProto->isTemplate();
  return false;
}

void WbNode::setRegenerationRequired(bool required) {
  mRegenerationRequired = required;
  if (required)
    emit regenerationRequired();
}

bool WbNode::hasAProtoAncestor() const {
  const WbNode *currentNode = parentNode();
  while (currentNode) {
    if (currentNode->isProtoInstance())
      return true;

    WbNode *protoParameterNode = currentNode->protoParameterNode();
    if (protoParameterNode && protoParameterNode->isProtoInstance())
      return true;

    currentNode = currentNode->parentNode();
  }

  return false;
}

WbNode *WbNode::protoAncestor() const {
  WbNode *currentNode = parentNode();
  while (currentNode) {
    if (currentNode->isProtoInstance())
      return currentNode;

    WbNode *protoParameterNode = currentNode->protoParameterNode();
    if (protoParameterNode && protoParameterNode->isProtoInstance())
      return protoParameterNode;

    currentNode = currentNode->parentNode();
  }
  return NULL;
}

bool WbNode::isAnAncestorOf(const WbNode *node) const {
  const WbNode *currentNode = node;
  while (currentNode) {
    currentNode = currentNode->parentNode();
    if (currentNode == this)
      return true;
  }
  return false;
}

bool WbNode::hasAreferredDefNodeDescendant() const {
  return hasAreferredDefNodeDescendant(this);
}

bool WbNode::hasAreferredDefNodeDescendant(const WbNode *root) const {
  const int count = useCount();
  const QList<WbNode *> &useNodes = this->useNodes();
  for (int i = 0; i < count; ++i) {
    if (!root->isAnAncestorOf(useNodes.at(i)))
      return true;
  }

  foreach (WbField *field, fieldsOrParameters()) {
    WbValue *value = field->value();
    const WbSFNode *const sfnode = dynamic_cast<WbSFNode *>(value);
    if (sfnode && sfnode->value()) {
      const WbNode *node = sfnode->value();
      const int nodeCount = node->useCount();
      const QList<WbNode *> &nodeUseNodes = node->useNodes();
      for (int i = 0; i < nodeCount; ++i) {
        if (!root->isAnAncestorOf(nodeUseNodes.at(i)))
          return true;
      }
      const bool subtreeHasDef = node->hasAreferredDefNodeDescendant(root);
      if (subtreeHasDef)
        return subtreeHasDef;
    } else {
      const WbMFNode *const mfnode = dynamic_cast<WbMFNode *>(value);
      if (mfnode) {
        const int size = mfnode->size();
        for (int i = 0; i < size; ++i) {
          const WbNode *node = mfnode->item(i);
          const int nodeCount = node->useCount();
          const QList<WbNode *> &nodeUseNodes = node->useNodes();
          for (int j = 0; j < nodeCount; ++j) {
            if (!root->isAnAncestorOf(nodeUseNodes.at(j)))
              return true;
          }
          const bool subtreeHasDef = node->hasAreferredDefNodeDescendant(root);
          if (subtreeHasDef)
            return subtreeHasDef;
        }
      }
    }
  }

  return false;
}

int WbNode::level() const {
  int level = 0;
  const WbNode *node = this;
  while (node->parentNode()) {
    node = node->parentNode();
    ++level;
  };

  return level;
}

/////////////////////
// Index functions //
/////////////////////

int WbNode::subNodeIndex(const WbNode *subNode, const WbNode *root) {
  assert(subNode && root);

  if (subNode == root)
    return 0;

  bool subNodeFound = false;
  int result = 0;

  const QVector<WbField *> &fields = root->fields();
  foreach (WbField *field, fields) {
    WbValue *value = field->value();
    WbSFNode *sfNode = dynamic_cast<WbSFNode *>(value);
    if (sfNode) {
      WbNode *node = sfNode->value();
      if (node)
        subNodeIndex(node, subNode, result, subNodeFound);
    } else {
      WbMFNode *mfNode = dynamic_cast<WbMFNode *>(value);
      if (mfNode) {
        const int n = mfNode->size();
        for (int i = 0; !subNodeFound && (i < n); ++i) {
          WbNode *node = mfNode->item(i);
          subNodeIndex(node, subNode, result, subNodeFound);
        }
      }
    }

    if (subNodeFound)
      return result;
  }

  return -1;
}

void WbNode::subNodeIndex(const WbNode *currentNode, const WbNode *targetNode, int &index, bool &subNodeFound) {
  ++index;

  if (targetNode == currentNode) {
    subNodeFound = true;
    return;
  }

  const QVector<WbField *> &fields = currentNode->fields();
  foreach (WbField *field, fields) {
    WbValue *value = field->value();
    WbSFNode *sfNode = dynamic_cast<WbSFNode *>(value);
    if (sfNode) {
      WbNode *node = sfNode->value();
      if (node)
        subNodeIndex(node, targetNode, index, subNodeFound);
    } else {
      WbMFNode *mfNode = dynamic_cast<WbMFNode *>(value);
      if (mfNode) {
        const int n = mfNode->size();
        for (int i = 0; !subNodeFound && (i < n); i++) {
          WbNode *node = mfNode->item(i);
          subNodeIndex(node, targetNode, index, subNodeFound);
        }
      }
    }

    if (subNodeFound)
      return;
  }
}

WbNode *WbNode::findNodeFromSubNodeIndices(QList<int> indices, WbNode *root) {
  WbNode *n = root;
  for (int i = 0; i < indices.size() && n != NULL; ++i)
    n = findNodeFromSubNodeIndex(indices[i], n);
  return n;
}

WbNode *WbNode::findNodeFromSubNodeIndex(int index, WbNode *root) {
  if (index == 0)
    return root;

  const QVector<WbField *> &fields = root->fields();
  foreach (WbField *field, fields) {
    WbValue *value = field->value();
    WbSFNode *sfNode = dynamic_cast<WbSFNode *>(value);
    if (sfNode) {
      WbNode *node = sfNode->value();
      if (node) {
        WbNode *returnNode = findNode(index, node);
        if (index == 0)
          return returnNode;
      }
    } else {
      WbMFNode *mfNode = dynamic_cast<WbMFNode *>(value);
      if (mfNode) {
        const int n = mfNode->size();
        for (int i = 0; (index > 0) && (i < n); i++) {
          WbNode *node = mfNode->item(i);
          WbNode *returnNode = findNode(index, node);
          if (index == 0)
            return returnNode;
        }
      }
    }
  }

  return NULL;
}

WbNode *WbNode::findNode(int &index, WbNode *root) {
  --index;

  if (index == 0)
    return root;

  const QVector<WbField *> &fields = root->fields();
  foreach (WbField *field, fields) {
    WbValue *value = field->value();
    WbSFNode *sfNode = dynamic_cast<WbSFNode *>(value);
    if (sfNode) {
      WbNode *node = sfNode->value();
      if (node) {
        WbNode *returnNode = findNode(index, node);
        if (index == 0)
          return returnNode;
      }
    } else {
      WbMFNode *mfNode = dynamic_cast<WbMFNode *>(value);
      if (mfNode) {
        const int n = mfNode->size();
        for (int i = 0; (index > 0) && (i < n); i++) {
          WbNode *returnNode = findNode(index, mfNode->item(i));
          if (index == 0)
            return returnNode;
        }
      }
    }
  }

  return NULL;
}

void WbNode::disconnectFieldNotification(const WbValue *value) {
  foreach (WbField *field, mFields) {
    if (field->value() == value)
      disconnect(field, &WbField::valueChanged, this, &WbNode::notifyFieldChanged);
  }
}

void WbNode::setFieldsParentNode() {
  QList<WbNode *> nodes(subNodes(true, true, true));
  nodes.prepend(this);
  foreach (WbNode *n, nodes) {
    QVector<WbField *> fields;
    fields << n->mFields << n->mParameters;
    foreach (WbField *field, fields)
      field->setParentNode(n);
  }
}

QStringList WbNode::documentationBookAndPage(bool isRobot) const {
  if (isProtoInstance()) {
    QStringList bookAndPage(mProto->documentationBookAndPage(isRobot, false));
    if (!bookAndPage.isEmpty())
      return bookAndPage;
  }
  return mModel->documentationBookAndPage();
}

const WbNode *WbNode::findRobotRootNode() const {
  const WbNode *tmpNode = this;
  while (tmpNode != NULL && !tmpNode->isRobot())
    tmpNode = tmpNode->parentNode();
  return tmpNode;
}

QString WbNode::getUrdfPrefix() const {
  return findRobotRootNode()->mUrdfPrefix;
}

/*
#include <QtCore/QDebug>
void WbNode::printDebugNodeStructure(int level) {
  QString indent;
  for (int i = 0; i < level; ++i)
    indent += "  ";

  QString line;
  line.sprintf("%sNode %s %p id %d parameterNode %p", indent.toStdString().c_str(), usefulName().toStdString().c_str(), this,
               uniqueId(), protoParameterNode());
  qDebug() << line;
  printDebugNodeFields(level, true);
  printDebugNodeFields(level, false);
}

void WbNode::printDebugNodeFields(int level, bool printParameters) {
  QString indent;
  for (int i = 0; i < level; ++i)
    indent += "  ";

  QString line;
  QString type = printParameters ? "Parameter" : "Field";
  QVector<WbField *> fieldList = printParameters ? parameters() : fields();
  foreach (WbField *p, fieldList) {
    line.sprintf("%s%s %s %p (alias %p):", indent.toStdString().c_str(), type.toStdString().c_str(),
                 p->name().toStdString().c_str(), p, p->parameter());
    qDebug() << line;
    if (p->type() == WB_SF_NODE) {
      WbNode *n = dynamic_cast<WbSFNode *>(p->value())->value();
      if (n)
        n->printDebugNodeStructure(level + 1);
    } else if (p->type() == WB_MF_NODE) {
      WbMFNode *mfnode = dynamic_cast<WbMFNode *>(p->value());
      for (int i = 0; i < mfnode->size(); ++i) {
        WbNode *n = mfnode->item(i);
        if (n)
          n->printDebugNodeStructure(level + 1);
      }
    } else {
      line.sprintf("%s  %s", indent.toStdString().c_str(), p->toString(WbPrecision::GUI_LOW).toStdString().c_str());
      qDebug() << line;
    }
  }
}
*/
