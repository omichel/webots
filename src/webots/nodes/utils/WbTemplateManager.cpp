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

#include "WbTemplateManager.hpp"

#include "WbAppearance.hpp"
#include "WbBasicJoint.hpp"
#include "WbField.hpp"
#include "WbFieldModel.hpp"
#include "WbGeometry.hpp"
#include "WbGroup.hpp"
#include "WbLog.hpp"
#include "WbMFNode.hpp"
#include "WbNode.hpp"
#include "WbNodeUtilities.hpp"
#include "WbPbrAppearance.hpp"
#include "WbProtoModel.hpp"
#include "WbSFNode.hpp"
#include "WbShape.hpp"
#include "WbSkin.hpp"
#include "WbSlot.hpp"
#include "WbSolid.hpp"
#include "WbSolidReference.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"

#include <QtCore/QCoreApplication>

#include <cassert>

WbTemplateManager *WbTemplateManager::cInstance = NULL;
int WbTemplateManager::cRegeneratingNodeCount = 0;

WbTemplateManager *WbTemplateManager::instance() {
  if (!cInstance) {
    cInstance = new WbTemplateManager();
    qAddPostRoutine(WbTemplateManager::cleanup);
  }
  return cInstance;
}

void WbTemplateManager::cleanup() {
  delete cInstance;
  cInstance = NULL;
}

WbTemplateManager::WbTemplateManager() : mBlockRegeneration(false), mTemplatesNeedRegeneration(false) {
}

WbTemplateManager::~WbTemplateManager() {
  clear();
}

void WbTemplateManager::blockRegeneration(bool block) {
  mBlockRegeneration = block;

  if (!block && mTemplatesNeedRegeneration) {  // regenerates all the required nodes
    while (true) {
      bool regenerated = false;
      foreach (WbNode *node, mTemplates) {
        if (node->isRegenerationRequired()) {
          regenerateNode(node);  // mTemplates can be modified during this call
          regenerated = true;
          break;
        }
      }
      if (regenerated)
        continue;
      else
        break;
    }
    mTemplatesNeedRegeneration = false;
  }
}

void WbTemplateManager::clear() {
  foreach (WbNode *node, mTemplates)
    disconnect(node, &WbNode::regenerationRequired, this, &WbTemplateManager::nodeNeedRegeneration);
  mTemplates.clear();
}

void WbTemplateManager::subscribe(WbNode *node, bool subscribedDescendant) {
  bool subscribed = false;
  if (node->isTemplate() && !mTemplates.contains(node)) {
    subscribed = true;
    mTemplates << node;
    connect(node, &QObject::destroyed, this, &WbTemplateManager::unsubscribe, Qt::UniqueConnection);
    connect(node, &WbNode::regenerateNodeRequest, this, &WbTemplateManager::regenerateNode, Qt::UniqueConnection);
    connect(node, &WbNode::regenerationRequired, this, &WbTemplateManager::nodeNeedRegeneration);
  }

  recursiveFieldSubscribeToRegenerateNode(node, subscribed, subscribedDescendant);
}

void WbTemplateManager::unsubscribe(QObject *node) {
  disconnect(static_cast<WbNode *>(node), &WbNode::regenerationRequired, this, &WbTemplateManager::nodeNeedRegeneration);
  mTemplates.removeAll(static_cast<WbNode *>(node));
}

bool WbTemplateManager::nodeNeedsToSubscribe(WbNode *node) {
  if (!node->isProtoInstance())
    return false;

  QVector<WbField *> fields = node->fieldsOrParameters();
  foreach (WbField *field, fields) {
    if (!field->alias().isEmpty())
      return true;
  }
  return false;
}

void WbTemplateManager::recursiveFieldSubscribeToRegenerateNode(WbNode *node, bool subscribedNode, bool subscribedDescendant) {
  if (subscribedNode || subscribedDescendant) {
    if (node->isProtoInstance())
      connect(node, &WbNode::parameterChanged, this, &WbTemplateManager::regenerateNodeFromParameterChange,
              Qt::UniqueConnection);
    else
      connect(node, &WbNode::fieldChanged, this, &WbTemplateManager::regenerateNodeFromFieldChange, Qt::UniqueConnection);
  }

  // if PROTO node:
  //   - subscribe sub-nodes in parameters
  //   - subscribe sub-nodes in fields if a parameter is redirected to the sub-node
  // else normal nodes:
  //   - subscribe sub-nodes in fields
  QVector<WbField *> fields = node->fields();
  int directSubscribeMinIndex = 0;
  if (node->isProtoInstance()) {
    directSubscribeMinIndex = fields.size();
    fields.append(node->parameters());
  }
  WbField *field = NULL;
  for (int i = 0; i < fields.size(); ++i) {
    field = fields[i];
    bool directSubscriptionEnabled = i >= directSubscribeMinIndex;
    switch (field->type()) {
      case WB_MF_NODE: {
        WbMFNode *mfnode = static_cast<WbMFNode *>(field->value());
        assert(mfnode);
        for (int j = 0; j < mfnode->size(); j++) {
          WbNode *subnode = mfnode->item(j);
          if (directSubscriptionEnabled || nodeNeedsToSubscribe(subnode))
            subscribe(subnode, subscribedDescendant || (subscribedNode && field->isTemplateRegenerator()));
        }
        break;
      }
      case WB_SF_NODE: {
        WbSFNode *sfnode = static_cast<WbSFNode *>(field->value());
        assert(sfnode);
        WbNode *subnode = sfnode->value();
        if (subnode && (directSubscriptionEnabled || nodeNeedsToSubscribe(subnode)))
          subscribe(subnode, subscribedDescendant || (subscribedNode && field->isTemplateRegenerator()));
        break;
      }
      default:
        break;
    }
  }
}

void WbTemplateManager::regenerateNodeFromFieldChange(WbField *field) {
  // retrieve the right node
  WbNode *templateNode = dynamic_cast<WbNode *>(sender());
  assert(templateNode);
  if (templateNode)
    regenerateNodeFromField(templateNode, field, false);
}

void WbTemplateManager::regenerateNodeFromParameterChange(WbField *field) {
  // retrieve the right node
  WbNode *templateNode = dynamic_cast<WbNode *>(sender());
  assert(templateNode);
  if (templateNode)
    regenerateNodeFromField(templateNode, field, true);
}

// intermediate function to determine which node should be updated
// Note: The security is probably overkill there, but its also safer for the first versions of the template mechanism
void WbTemplateManager::regenerateNodeFromField(WbNode *templateNode, WbField *field, bool isParameter) {
  // 1. retrieve upper template node where the modification appeared in a template regenerator field
  templateNode = WbNodeUtilities::findUpperTemplateNeedingRegenerationFromField(field, templateNode);

  if (!templateNode)
    return;

  // 2. check it's not a parameter managed by ODE
  if (!isParameter && dynamic_cast<const WbSolid *>(templateNode) &&
      ((field->name() == "translation" && field->type() == WB_SF_VEC3F) ||
       (field->name() == "rotation" && field->type() == WB_SF_ROTATION) ||
       (field->name() == "position" && field->type() == WB_SF_FLOAT)))
    return;

  // 3. regenerate template where the modification appeared in a template regenerator field
  regenerateNode(templateNode);
}

void WbTemplateManager::regenerateNode(WbNode *node, bool restarted) {
  assert(node);

  if (mBlockRegeneration) {
    node->setRegenerationRequired(true);  // will be regenerated when deblocking this manager
    return;
  } else
    node->setRegenerationRequired(false);

  // 1. get stuff
  WbNode *parent = node->parentNode();
  WbProtoModel *proto = node->proto();
  assert(parent && proto);
  if (!parent || !proto)
    return;
  const bool isInBoundingObject = dynamic_cast<WbBaseNode *>(node)->isInBoundingObject();

  QList<WbField *> previousParentRedirections;
  WbField *parentField = node->parentField();
  QVector<WbField *> parameters;
  WbNode::setRestoreUniqueIdOnClone(true);
  foreach (WbField *parameter, node->parameters()) {
    parameters << new WbField(*parameter, NULL);
    if (parameter->parameter() != NULL)
      previousParentRedirections.append(parameter->parameter());
  }
  WbNode::setRestoreUniqueIdOnClone(false);
  int uniqueId = node->uniqueId();
  const WbSolid *solid = dynamic_cast<const WbSolid *>(node);
  WbVector3 translationFromFile;
  WbRotation rotationFromFile;
  if (solid) {
    translationFromFile = solid->translationFromFile();
    rotationFromFile = solid->rotationFromFile();
  }

  WbWorld *world = WbWorld::instance();
  WbGroup *root = WbWorld::instance()->root();
  bool isWorldInitialized = root && root->isPostFinalizedCalled();

  WbViewpoint *viewpoint = world->viewpoint();
  WbSolid *followedSolid = viewpoint == NULL ? NULL : viewpoint->followedSolid();
  bool isFollowedSolid = followedSolid == node;
  QString followedSolidName;
  if (followedSolid)
    followedSolidName = followedSolid->name();

  // 2. regenerate the new node
  WbNode *upperTemplateNode = WbNodeUtilities::findUpperTemplateNeedingRegeneration(node);
  bool nested = upperTemplateNode && upperTemplateNode != node;
  cRegeneratingNodeCount++;
  if (isWorldInitialized && !restarted)
    // signal is not emitted in case a node has been regenerated twice in a row (`restart` == TRUE)
    // to preserve the scene tree selection
    emit preNodeRegeneration(node, nested);

  WbNode::setGlobalParentNode(parent);

  WbNode *newNode = WbNode::regenerateProtoInstanceFromParameters(proto, parameters, node->isTopLevel(),
                                                                  WbWorld::instance()->fileName(), true, uniqueId);

  if (!newNode) {
    WbLog::error(tr("Template regeneration failed. The node cannot be generated."), false, WbLog::PARSING);
    delete newNode;
    if (isWorldInitialized)
      emit abortNodeRegeneration();
    return;
  }

  newNode->setDefName(node->defName());
  WbNode::setGlobalParentNode(NULL);

  WbNodeUtilities::validateInsertedNode(parentField, newNode, parent, isInBoundingObject);

  subscribe(newNode);

  bool ancestorTemplateRegeneration = upperTemplateNode != NULL;
  if (node->isProtoParameterNode()) {
    const QVector<WbField *> &parentFields = parent->fieldsOrParameters();
    foreach (WbField *const parentField, parentFields) {
      if (parentField->type() == WB_SF_NODE) {
        WbSFNode *sfnode = static_cast<WbSFNode *>(parentField->value());
        if (sfnode->value() == node) {
          if (ancestorTemplateRegeneration)
            sfnode->blockSignals(true);

          sfnode->setValue(newNode);

          if (ancestorTemplateRegeneration) {
            sfnode->blockSignals(false);
            regenerateNode(upperTemplateNode);
            return;
          }
        }
      } else if (parentField->type() == WB_MF_NODE) {
        WbMFNode *mfnode = static_cast<WbMFNode *>(parentField->value());
        for (int i = 0; i < mfnode->size(); ++i) {
          WbNode *n = mfnode->item(i);
          if (n == node) {
            if (ancestorTemplateRegeneration)
              mfnode->blockSignals(true);

            mfnode->removeItem(i);
            mfnode->insertItem(i, newNode);

            if (ancestorTemplateRegeneration) {
              mfnode->blockSignals(false);
              regenerateNode(upperTemplateNode);
              return;
            }
            break;
          }
        }
      }
    }
  } else {
    // reassign pointer in parent
    WbGroup *const parentGroup = dynamic_cast<WbGroup *>(parent);
    WbBasicJoint *const parentJoint = dynamic_cast<WbBasicJoint *>(parent);
    WbShape *const parentShape = dynamic_cast<WbShape *>(parent);
    WbSkin *const parentSkin = dynamic_cast<WbSkin *>(parent);
    WbSlot *const parentSlot = dynamic_cast<WbSlot *>(parent);
    WbAppearance *const newAppearance = dynamic_cast<WbAppearance *>(newNode);
    WbPbrAppearance *const newPbrAppearance = dynamic_cast<WbPbrAppearance *>(newNode);
    WbGeometry *const newGeometry = dynamic_cast<WbGeometry *>(newNode);
    WbSlot *const newSlot = dynamic_cast<WbSlot *>(newNode);
    WbSolid *const newSolid = dynamic_cast<WbSolid *>(newNode);
    WbSolidReference *const newSolidReference = dynamic_cast<WbSolidReference *>(newNode);

    if (parentGroup) {
      int i = parentGroup->nodeIndex(node);
      assert(i != -1);

      // TODO: The 3 following lines could be simplified by using WbGroup::setChild(),
      //       but this function has to be fixed first (similar problem in WbSceneTree::transform
      // remove currentNode
      parentGroup->removeChild(node);
      // insert just after currentNode
      parentGroup->insertChild(i, newNode);
      delete node;  // In the other cases the setter function will take care of deleting the node
    } else if (parentSkin && parentSkin->appearanceField() && newAppearance) {
      int i = parentSkin->appearanceField()->nodeIndex(node);
      assert(i != -1);

      // TODO: WbMFNode::setItem doesn't work here either. Fix this along with WbGroup::setChild()
      parentSkin->appearanceField()->removeItem(i);
      parentSkin->appearanceField()->insertItem(i, newAppearance);
    } else if (parentShape && newGeometry)
      parentShape->setGeometry(newGeometry);
    else if (parentShape && newAppearance)
      parentShape->setAppearance(newAppearance);
    else if (parentShape && newPbrAppearance)
      parentShape->setPbrAppearance(newPbrAppearance);
    else if (parentSlot)
      parentSlot->setEndPoint(newNode);
    else if (parentJoint && newSolid)
      parentJoint->setSolidEndPoint(newSolid);
    else if (parentJoint && newSolidReference)
      parentJoint->setSolidEndPoint(newSolidReference);
    else if (parentJoint && newSlot)
      parentJoint->setSolidEndPoint(newSlot);
    else {
      WbLog::error(tr("Template regeneration failed. Unsupported node type."), false, WbLog::PARSING);
      delete newNode;
      emit abortNodeRegeneration();
      return;
    }
  }

  // let the supervisor set field functions work as if the node has not been deleted
  newNode->setUniqueId(uniqueId);

  // restore translation and rotation loaded from file
  WbSolid *newSolid = dynamic_cast<WbSolid *>(newNode);
  if (solid && newSolid) {
    newSolid->setTranslationFromFile(translationFromFile);
    newSolid->setRotationFromFile(rotationFromFile);
  }

  // update nested proto flag of current PROTO node and his instances if any
  newNode->updateNestedProtoFlag();

  // redirect parent parameters
  if (!previousParentRedirections.isEmpty()) {
    foreach (WbField *parentParameter, previousParentRedirections) {
      foreach (WbField *newParam, newNode->parameters()) {
        if (parentParameter->name() == newParam->alias())
          newParam->redirectTo(parentParameter);
      }
    }
  }

  mBlockRegeneration = true;  // prevent regenerating `newNode` in the finalization step due to field checks

  WbBaseNode *base = dynamic_cast<WbBaseNode *>(newNode);
  if (isWorldInitialized) {
    assert(base);
    base->finalize();
  }

  mBlockRegeneration = false;
  if (newNode->isRegenerationRequired()) {  // if needed, trigger `newNode` regeneration with finalized fields values
    regenerateNode(newNode, true);
    return;
  }

  // after regeneration, check if any invisible proto parameter node can be removed from the new node
  base->removeInvisibleProtoNodes();

  // if the viewpoint is being re-generated we need to re-get the correct pointer, not the old dangling pointer from before
  // the node was regenerated
  viewpoint = world->viewpoint();
  if (isFollowedSolid)
    viewpoint->startFollowUp(newSolid, true);
  else if (!followedSolidName.isEmpty() && viewpoint->followedSolid() == NULL)
    // restore follow solid
    viewpoint->startFollowUp(WbSolid::findSolidFromUniqueName(followedSolidName), true);

  cRegeneratingNodeCount--;
  assert(cRegeneratingNodeCount >= 0);
  if (isWorldInitialized)
    emit postNodeRegeneration(newNode);
}

void WbTemplateManager::nodeNeedRegeneration() {
  mTemplatesNeedRegeneration = true;
}
