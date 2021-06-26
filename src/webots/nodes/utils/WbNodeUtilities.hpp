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

#ifndef WB_NODE_UTILITIES_HPP
#define WB_NODE_UTILITIES_HPP

//
// Description: utility class allowing to query nodes in their tree context
//              through static functions
//

#include <QtCore/QList>
#include <cstddef>
#include "WbNode.hpp"

class WbAbstractTransform;
class WbBaseNode;
class WbBoundingSphere;
class WbField;
class WbMatter;
class WbNode;
class WbProtoModel;
class WbRay;
class WbRobot;
class WbShape;
class WbSolid;
class WbTransform;

class QString;

namespace WbNodeUtilities {

  //////////////////////////
  // Permanent properties //
  //////////////////////////

  // find the closest WbTransform ancestor
  WbTransform *findUpperTransform(const WbNode *node);

  // find the closest template ancestor in which the modified node is contained in template field
  // which requires a template instance regeneration
  WbNode *findUpperTemplateNeedingRegeneration(WbNode *modifiedNode);

  // find the closest template ancestor of given field in which the modified field is contained
  // in template field which requires a template instance regeneration
  WbNode *findUpperTemplateNeedingRegenerationFromField(WbField *modifiedField, WbNode *parentNode);

  // find the closest WbSolid ancestor
  WbSolid *findUpperSolid(const WbNode *node);

  // find the closest WbMatter ancestor
  WbMatter *findUpperMatter(const WbNode *node);

  // find the closest ancestor of specified type
  // searchDegree specifies how many ancestor have to be checked, if lower or equal to 0 all the hierarchy is inspected
  WbNode *findUpperNodeByType(const WbNode *node, int nodeType, int searchDegrees = 0);

  // return if this node contains descendant nodes of the specified types
  bool hasDescendantNodesOfType(const WbNode *node, QList<int> nodeTypes);

  // return all the descendant nodes fulfilling the specified type condition
  // typeCondition is a function that checks the type of the node
  // if recursive is set to FALSE children of the descendant node having the specified type are not inspected
  QList<WbNode *> findDescendantNodesOfType(WbNode *node, bool (&typeCondition)(WbBaseNode *), bool recursive);

  // find the uppermost WbTransform ancestor (may be the node itself)
  WbTransform *findUppermostTransform(const WbNode *node);

  // find the uppermost WbTransform ancestor (may be the node itself)
  WbSolid *findUppermostSolid(const WbNode *node);

  // find the uppermost WbMatter ancestor (may be the node itself)
  WbMatter *findUppermostMatter(WbNode *node);

  // find the top node and return it if it is a WbSolid, return NULL otherwise
  WbSolid *findTopSolid(const WbNode *node);

  // find a robot ancestor above the node in the scene tree, return NULL if no robot found
  WbRobot *findRobotAncestor(const WbNode *node);

  // is this node directly attached to the root node
  bool isTopNode(const WbNode *node);

  // find the ancestor node directly attached to the root node
  const WbNode *findTopNode(const WbNode *node);

  // return direct Solid descendant nodes
  // in case of PROTO nodes only internal nodes are checked
  QList<WbSolid *> findSolidDescendants(WbNode *node);

  // is this node located directly or indirectly in the given field
  bool isFieldDescendant(const WbNode *node, const QString &fieldName);

  // is this node located directly or indirectly under a Billboard
  bool isDescendantOfBillboard(const WbNode *node);

  // is this node located in the boundingObject field of a Solid
  // use checkNodeUse() to inspect USE nodes and PROTO parameter instances
  bool isInBoundingObject(const WbNode *node);

  // check if node is used in a boundingObject field and/or in the global structure
  WbNode::NodeUse checkNodeUse(const WbNode *n);

  // find the WbMatter ancestor whose boundingObject field contains this node
  WbMatter *findBoundingObjectAncestor(const WbBaseNode *node);

  // is this node a valid USEable node
  bool isAValidUseableNode(const WbNode *node, QString *warning = NULL);

  // find (innermost) enclosing PROTO if any
  WbProtoModel *findContainingProto(const WbNode *node);

  // find the field parent of the target field, i.e. the closest upper field in the tree hierarchy
  WbField *findFieldParent(const WbField *target, bool internal = false);

  // is the target field or node visible in the Scene Tree (possibly as a nested proto parameter)
  bool isVisible(const WbNode *node);
  bool isVisible(const WbField *target);

  // return closest WbMatter ancestor that is visible in the scene tree (given node included)
  WbMatter *findUpperVisibleMatter(WbNode *node);

  // is the target field or the target parameter field a template regenerator field
  bool isTemplateRegeneratorField(const WbField *field);

  WbAbstractTransform *abstractTransformCast(WbBaseNode *node);

  //////////////////////////////
  // Non-permanent properties //
  //////////////////////////////

  // has this node a DEF node ancestor
  bool hasADefNodeAncestor(const WbNode *node);

  // has this node a USE node ancestor
  bool hasAUseNodeAncestor(const WbNode *node);

  // fid all ancestor USE nodes
  QList<WbNode *> findUseNodeAncestors(WbNode *node);

  // has this node a robot ancestor
  bool hasARobotAncestor(const WbNode *node);

  // has this node a child of type Solid
  bool hasSolidChildren(const WbNode *node);

  // has this node a Robot node descendant
  bool hasARobotDescendant(const WbNode *node);

  // has this node a Device node descendant
  bool hasADeviceDescendant(const WbNode *node);

  // has this node a Solid node descendant
  bool hasASolidDescendant(const WbNode *node);

  // has this node a Joint node descendant
  bool hasAJointDescendant(const WbNode *node);

  // has this DEF node a subsequent USE or DEF node using its new definition
  bool hasASubsequentUseOrDefNode(const WbNode *defNode, const QString &defName, const QString &previousDefName,
                                  bool &useOverlap, bool &defOverlap);

  // is this node selected
  bool isSelected(const WbNode *node);

  // is this node or a WbMatter ancestor of the current node locked
  bool isNodeOrAncestorLocked(WbNode *node);

  // tests node types
  bool isGeometryTypeName(const QString &modelName);
  bool isCollisionDetectedGeometryTypeName(const QString &modelName);
  bool isRobotTypeName(const QString &modelName);
  bool isDeviceTypeName(const QString &modelName);
  bool isSolidDeviceTypeName(const QString &modelName);
  bool isSolidTypeName(const QString &modelName);
  bool isSolidButRobotTypeName(const QString &modelName);
  bool isMatterTypeName(const QString &modelName);
  QString slotType(const WbNode *node);
  // return true for nodes which should have only one occurence in each world, i.e. WorldInfo, Viewpoint, Background
  bool isSingletonTypeName(const QString &modelName);

  bool isTrackAnimatedGeometry(const WbNode *node);

  ///////////
  // Other //
  ///////////

  // find intersecting Shape
  const WbShape *findIntersectingShape(const WbRay &ray, double maxDistance, double &distance, double minDistance = 0.0);

  // validate a new inserted node
  // this functions helps handling properly the validation of a Slot node
  // return false if the Slot structure is invalid and insertion should be aborted
  bool validateInsertedNode(WbField *field, const WbNode *newNode, const WbNode *parentNode, bool isInBoundingObject);

  // check if a node with node model 'modelName' can be inserted in the field 'field' of parent node 'node'
  // in case of PROTO parent node and parameter field,
  // it first retrieve the base field and model and then check the validity
  // type is checked in case of Slot node
  bool isAllowedToInsert(const WbField *const field, const QString &nodeName, const WbNode *node, QString &errorMessage,
                         WbNode::NodeUse nodeUse, const QString &type, const QStringList &restrictionValidNodeNames,
                         bool automaticBoundingObjectCheck = true);

  // check existing node structure
  bool validateExistingChildNode(const WbField *const field, const WbNode *childNode, const WbNode *node,
                                 bool isInBoundingObject, QString &errorMessage);

  // can srcNode be transformed
  enum Answer { SUITABLE, UNSUITABLE, LOOSING_INFO };
  Answer isSuitableForTransform(const WbNode *srcNode, const QString &destModelName);

  // check if type of two Slot nodes is compatible
  bool isSlotTypeMatch(const QString &firstType, const QString &secondType, QString &errorMessage);

  // return a node's bounding sphere ancestor if it exists (can be the node's own)
  WbBoundingSphere *boundingSphereAncestor(const WbNode *node);
};  // namespace WbNodeUtilities

#endif
