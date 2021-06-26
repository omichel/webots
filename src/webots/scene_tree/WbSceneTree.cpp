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

#include "WbSceneTree.hpp"

#include "WbAbstractTransform.hpp"
#include "WbAddInertiaMatrixDialog.hpp"
#include "WbAddItemCommand.hpp"
#include "WbAddNodeDialog.hpp"
#include "WbBoundingSphere.hpp"
#include "WbClipboard.hpp"
#include "WbConcreteNodeFactory.hpp"
#include "WbContextMenuGenerator.hpp"
#include "WbEditCommand.hpp"
#include "WbField.hpp"
#include "WbFieldEditor.hpp"
#include "WbGroup.hpp"
#include "WbGuiRefreshOracle.hpp"
#include "WbLog.hpp"
#include "WbMFNode.hpp"
#include "WbMFVector3.hpp"
#include "WbMessageBox.hpp"
#include "WbNodeOperations.hpp"
#include "WbNodeUtilities.hpp"
#include "WbPhysics.hpp"
#include "WbPreferences.hpp"
#include "WbProject.hpp"
#include "WbProtoModel.hpp"
#include "WbRemoveItemCommand.hpp"
#include "WbResetCommand.hpp"
#include "WbSFNode.hpp"
#include "WbSceneTreeModel.hpp"
#include "WbSelection.hpp"
#include "WbSimulationState.hpp"
#include "WbSolid.hpp"
#include "WbStandardPaths.hpp"
#include "WbTemplateManager.hpp"
#include "WbTreeItem.hpp"
#include "WbTreeView.hpp"
#include "WbUndoStack.hpp"
#include "WbValueEditor.hpp"
#include "WbVariant.hpp"
#include "WbViewpoint.hpp"
#include "WbWorld.hpp"

#include <cassert>

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

static int gFactoryFieldEditorHeightHint = 0;

struct TreeItemState {
  bool expanded;
  bool selected;
  QList<TreeItemState *> children;
};

WbSceneTree::WbSceneTree(QWidget *parent) :
  QWidget(parent),
  mSplitter(new QSplitter(Qt::Vertical, this)),
  mActionManager(WbActionManager::instance()),
  mClipboard(WbClipboard::instance()) {
  mModel = NULL;
  mTreeView = NULL;
  mSelectedItem = NULL;
  mRowsAreAboutToBeRemoved = false;
  mFocusWidgetBeforeNodeRegeneration = NULL;

  mSelectionInsideTreeStateRecovery = false;
  mSelectionBeforeTreeStateRegeneration = NULL;
  mTreeItemState = NULL;

  setObjectName("SceneTree");

  mFieldEditor = new WbFieldEditor(this);
  connect(mFieldEditor, &WbFieldEditor::dictionaryUpdateRequested, WbNodeOperations::instance(),
          &WbNodeOperations::requestUpdateDictionary);
  connect(mFieldEditor, &WbFieldEditor::valueChanged, this, &WbSceneTree::valueChangedFromGui);
  connect(mFieldEditor, &WbFieldEditor::editRequested, this, &WbSceneTree::editRequested);
  connect(WbGuiRefreshOracle::instance(), &WbGuiRefreshOracle::canRefreshActivated, this, &WbSceneTree::refreshItems);

  QScrollArea *fieldEditorScrollArea = new QScrollArea(mSplitter);
  fieldEditorScrollArea->setObjectName("editorPane");
  fieldEditorScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  fieldEditorScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  fieldEditorScrollArea->setWidgetResizable(true);
  fieldEditorScrollArea->setFocusPolicy(Qt::ClickFocus);
  fieldEditorScrollArea->setWidget(mFieldEditor);
  gFactoryFieldEditorHeightHint = fieldEditorScrollArea->sizeHint().height();

  mSplitter->addWidget(fieldEditorScrollArea);
  mSplitter->setObjectName("verticalSplitter");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setSpacing(0);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->addWidget(mSplitter, 0);

  connect(mActionManager, &WbActionManager::userWorldEditCommandReceived, this, &WbSceneTree::handleUserCommand);
  connect(mActionManager, &WbActionManager::transformRequested, this, &WbSceneTree::transform);
  connect(mActionManager->action(WbAction::ADD_NEW), &QAction::triggered, this, &WbSceneTree::addNew);
  connect(mActionManager->action(WbAction::MOVE_VIEWPOINT_TO_OBJECT), &QAction::triggered, this,
          &WbSceneTree::moveViewpointToObject);
  connect(mActionManager->action(WbAction::RESET_VALUE), &QAction::triggered, this, &WbSceneTree::reset);
  connect(mActionManager->action(WbAction::CONVERT_TO_BASE_NODES), &QAction::triggered, this, &WbSceneTree::convertToBaseNode);
  connect(mActionManager->action(WbAction::CONVERT_ROOT_TO_BASE_NODES), &QAction::triggered, this,
          &WbSceneTree::convertRootToBaseNode);
  connect(mActionManager->action(WbAction::OPEN_HELP), &QAction::triggered, this, &WbSceneTree::help);
  connect(mActionManager->action(WbAction::SHOW_PROTO_SOURCE), &QAction::triggered, this, &WbSceneTree::openProtoInTextEditor);
  connect(mActionManager->action(WbAction::SHOW_PROTO_RESULT), &QAction::triggered, this,
          &WbSceneTree::openTemplateInstanceInTextEditor);
  connect(mActionManager->action(WbAction::EXPORT_NODE), &QAction::triggered, this, &WbSceneTree::exportObject);
  connect(WbUndoStack::instance(), &WbUndoStack::changed, this, &WbSceneTree::updateValue);

  connect(WbTemplateManager::instance(), &WbTemplateManager::preNodeRegeneration, this, &WbSceneTree::prepareNodeRegeneration);
  connect(WbTemplateManager::instance(), &WbTemplateManager::abortNodeRegeneration, this, &WbSceneTree::abortNodeRegeneration);
  connect(WbTemplateManager::instance(), &WbTemplateManager::postNodeRegeneration, this, &WbSceneTree::applyNodeRegeneration);
}

WbSceneTree::~WbSceneTree() {
  cleanup();
}

void WbSceneTree::cleanup() {
  WbTreeItem::enableUpdates(false);
  mFieldEditor->resetFocus();
  mSelectedItem = NULL;

  delete mTreeView;
  mTreeView = NULL;
  delete mModel;
  mModel = NULL;

  // disconnect all signals
  disconnect(this, 0);
}

void WbSceneTree::prepareWorldLoading() {
  WbUndoStack::instance()->clear();
  mSelectedItem = NULL;
  mFieldEditor->resetFocus();
  WbTreeItem::enableUpdates(false);
  updateToolbar();
  disconnect(WbSelection::instance(), &WbSelection::selectionChangedFromSceneTree, this, &WbSceneTree::updateSelection);
}

void WbSceneTree::applyChanges() {
  mFieldEditor->applyChanges();
}

// compare old tree and new tree side by side and recursively.
// if an index is expanded in the old tree, expand it also in the new tree
void WbSceneTree::restoreState(WbTreeView *t1, WbTreeView *t2, const QModelIndex &i1, const QModelIndex &i2) {
  QModelIndex selectedIndex = t1->currentIndex();

  for (int i = 0; true; i++) {
    // explore indices side by side
    QModelIndex j1 = t1->model()->index(i, 0, i1);
    QModelIndex j2 = t2->model()->index(i, 0, i2);

    // break when no more children
    if (!j1.isValid() || !j2.isValid())
      break;

    // recurse into tree
    restoreState(t1, t2, j1, j2);

    // restore 'expanded' state
    if (t1->isExpanded(j1))
      t2->setExpanded(j2, true);

    // restore current selection
    if (j1 == selectedIndex)
      t2->setCurrentIndex(j2);
  }
}

void WbSceneTree::setWorld(WbWorld *world) {
  // keep to restore state
  WbTreeView *const oldTreeView = mTreeView;
  WbSceneTreeModel *const oldModel = mModel;

  // create new tree widget and model
  mTreeView = new WbTreeView(this);
  mTreeView->setHeaderHidden(true);
  mModel = new WbSceneTreeModel(world->root());

  // connect widget to model
  mTreeView->setModel(mModel);
  mModel->startWatching(mTreeView->rootIndex());

  // enable updates of scene tree items
  WbTreeItem::enableUpdates(true);

  // this must be done before restoreState()
  connect(mTreeView, &WbTreeView::refreshRequested, this, &WbSceneTree::refreshTreeView);
  connect(mTreeView, &WbTreeView::doubleClickOrEnterPressed, this, &WbSceneTree::handleDoubleClickOrEnterPress);
  connect(mTreeView, &WbTreeView::focusIn, this, &WbSceneTree::updateApplicationActions);
  connect(mTreeView, &WbTreeView::expanded, this, &WbSceneTree::startWatching);
  connect(mTreeView, &WbTreeView::collapsed, this, &WbSceneTree::stopWatching);
  connect(mModel, &WbSceneTreeModel::itemInserted, mTreeView, &WbTreeView::itemInserted);
  connect(mModel, &WbSceneTreeModel::rowsAboutToBeRemovedSoon, this, &WbSceneTree::handleRowRemoval);

  connect(mTreeView, &WbTreeView::selectionHasChanged, this, &WbSceneTree::updateSelection);
  connect(WbSelection::instance(), &WbSelection::selectionChangedFromSceneTree, this, &WbSceneTree::updateSelection);

  // attempt to restore expanded state (only if reloading)
  if (world->fileName() == mWorldFileName)
    restoreState(oldTreeView, mTreeView, oldTreeView->rootIndex(), mTreeView->rootIndex());
  else {
    mSelectedItem = NULL;
    mFieldEditor->setTitle("");
    updateToolbar();
  }

  bool hasFocus = oldTreeView && oldTreeView->hasFocus();

  // delete old widget and model
  delete oldTreeView;
  delete oldModel;

  // insert new widget before value editor
  mSplitter->insertWidget(0, mTreeView);
  mSplitter->setStretchFactor(0, 1);
  mSplitter->setStretchFactor(1, 0);

  // set focus if needed
  if (hasFocus)
    mTreeView->setFocus(Qt::OtherFocusReason);

  // just to know if we are reloading
  mWorldFileName = world->fileName();
  mTreeView->scrollToSelection();
}

void WbSceneTree::handleUserCommand(WbAction::WbActionKind actionKind) {
  switch (actionKind) {
    case WbAction::CUT:
      cut();
      return;
    case WbAction::COPY:
      copy();
      return;
    case WbAction::PASTE:
      paste();
      return;
    case WbAction::UNDO:
      WbUndoStack::instance()->undo();
      return;
    case WbAction::REDO:
      WbUndoStack::instance()->redo();
      return;
    case WbAction::DEL:
      del();
    default:
      return;
  }
}

void WbSceneTree::cut() {
  copy();
  del();
  updateToolbar();
}

void WbSceneTree::copy() {
  WbValue *value;
  int row = -1;

  // make a shallow copy of item value
  if (mSelectedItem->isField()) {
    // copy action should not be enabled for multiple fields
    assert(mSelectedItem->field()->isSingle());
    value = mSelectedItem->field()->value();
  } else {
    // node or item
    value = mSelectedItem->parent()->field()->value();
    row = mSelectedItem->row();
  }

  WbSingleValue *singleValue = dynamic_cast<WbSingleValue *>(value);
  WbMultipleValue *multipleValue = dynamic_cast<WbMultipleValue *>(value);
  if (mSelectedItem->isNode() || mSelectedItem->isSFNode())
    mClipboard->setNode(mSelectedItem->node());
  else if (singleValue)
    *mClipboard = singleValue->variantValue();
  else if (multipleValue)
    *mClipboard = multipleValue->variantValue(row);
  else  // reset clipboard
    *mClipboard = WbVariant();

  updateToolbar();
}

void WbSceneTree::paste() {
  if (!mSelectedItem)
    return;

  if (mSelectedItem->isField() && mSelectedItem->field()->isSingle())
    pasteInSFValue();
  else
    pasteInMFValue();
  WbWorld::instance()->setModifiedFromSceneTree();
}

void WbSceneTree::pasteInSFValue() {
  WbTreeItem *selectedItem = mSelectedItem;
  WbField *field = selectedItem->field();
  WbValue *item = field->value();

  if (mClipboard->type() == WB_SF_NODE) {
    const QString &nodeString = mClipboard->computeNodeExportStringForInsertion(selectedItem->parent()->node(), field, -1);
    WbNodeOperations::OperationResult result =
      WbNodeOperations::instance()->importNode(selectedItem->parent()->node(), field, -1, QString(), nodeString, true);
    if (result == WbNodeOperations::FAILURE)
      return;

    if (result == WbNodeOperations::SUCCESS) {
      // update selection scroll position
      QModelIndex currentIndex = mModel->itemToIndex(selectedItem->child(0));
      mTreeView->setCurrentIndex(currentIndex);
    } else
      updateSelection();

    mTreeView->scrollToSelection();

  } else {
    // item
    WbSingleValue *singleValue = dynamic_cast<WbSingleValue *>(item);
    WbUndoStack::instance()->push(new WbEditCommand(singleValue, singleValue->variantValue(), *mClipboard));
  }

  updateValue();
  updateToolbar();
}

// paste item or node
void WbSceneTree::pasteInMFValue() {
  assert(!mClipboard->isEmpty());

  WbMultipleValue *parentItem;
  WbNode *parentNode = NULL;
  WbField *field = NULL;
  WbTreeItem *fieldItem;
  int index = 0;

  if (mSelectedItem->isField()) {
    // multiple field
    const WbTreeItem *nodeItem = mSelectedItem->parent();
    fieldItem = mSelectedItem;
    field = mSelectedItem->field();
    assert(field && field->isMultiple());

    parentItem = static_cast<WbMultipleValue *>(field->value());
    if (nodeItem->isNode() || nodeItem->hasNode())
      parentNode = nodeItem->node();
  } else {
    // sibling is selected (node or item)
    fieldItem = mSelectedItem->parent();
    field = fieldItem->field();
    assert(fieldItem->isField() && field && field->isMultiple());

    index = mSelectedItem->row() + 1;
    parentItem = static_cast<WbMultipleValue *>(field->value());
    if (mSelectedItem->isNode())
      parentNode = mSelectedItem->node()->parentNode();
  }

  if (mClipboard->type() == WB_SF_NODE) {
    assert(parentNode);

    // if newNode is in a template regenerated field, its pointer will be invalid after this call
    const QString &nodeString = mClipboard->computeNodeExportStringForInsertion(parentNode, field, index);
    WbNodeOperations::OperationResult result =
      WbNodeOperations::instance()->importNode(parentNode, field, index, QString(), nodeString, true);
    if (result == WbNodeOperations::FAILURE)
      return;

    if (result == WbNodeOperations::SUCCESS) {
      // update selection and scroll position
      QModelIndex currentIndex = mModel->itemToIndex(fieldItem->child(index));
      mTreeView->setCurrentIndex(currentIndex);
    }

    mTreeView->scrollToSelection();

    WbUndoStack::instance()->clear();  // TODO remove after implementing UNDO action

  } else
    WbUndoStack::instance()->push(new WbAddItemCommand(parentItem, *mClipboard, index));

  updateSelection();
  if (mSelectedItem && mSelectedItem->isField()) {  // if node insertion failed mSelectedItem is NULL
    QModelIndex newNodeIndex = mModel->itemToIndex(mSelectedItem->child(index));
    mTreeView->setCurrentIndex(newNodeIndex);
    mTreeView->scrollToModelIndex(newNodeIndex);
  }

  updateValue();
  updateToolbar();
}

void WbSceneTree::del(WbNode *nodeToDel) {
  WbNode *node = nodeToDel;

  WbTreeItem *deletedItem;
  if (node == NULL) {
    node = mSelectedItem->node();
    deletedItem = mSelectedItem;
  } else
    deletedItem = mModel->indexToItem(mModel->findModelIndexFromNode(node));

  bool dictionaryUpdated = false;
  if (node) {
    dictionaryUpdated = node->hasAreferredDefNodeDescendant();
    if (dictionaryUpdated &&
        WbMessageBox::question(
          tr("This node is a DEF node, or has a descendant DEF node, on which at least one external USE node depends. "
             "Deleting it will make its USE nodes to refer to a previous node having the same DEF keyword if it exists, "
             "or will turn its first USE node into a DEF node.\n"
             "Do you want to continue?"),
          this, tr("DEF node deletion")) == QMessageBox::Cancel)
      return;

    bool previousRowsAboutToBeRemoved = mRowsAreAboutToBeRemoved;
    mFieldEditor->editField(NULL, NULL);  // reset field editor
    if (!(node->isUseNode() && deletedItem->isSFNode()))
      mRowsAreAboutToBeRemoved = true;
    // else no rows will be deleted

    if (!WbNodeOperations::instance()->deleteNode(node)) {
      mRowsAreAboutToBeRemoved = previousRowsAboutToBeRemoved;
      return;
    }

    WbUndoStack::instance()->clear();  // clear undo stack if no available UNDO/REDO implementation of del action
  } else {
    // item
    mRowsAreAboutToBeRemoved = true;
    WbMultipleValue *mvalue = static_cast<WbMultipleValue *>(mSelectedItem->parent()->field()->value());
    WbUndoStack::instance()->push(new WbRemoveItemCommand(mvalue, mSelectedItem->row()));
  }

  mRowsAreAboutToBeRemoved = false;

  if (dictionaryUpdated) {
    mModel->emitLayoutChanged();  // makes the 'expandable' triangle visible for USE nodes turned into DEF nodes
    if (!WbNodeOperations::instance()->isFromSupervisor())
      // selection already removed in handleRowRemoval function but it is changed when updating the dictionay
      clearSelection();
  }

  WbWorld::instance()->setModifiedFromSceneTree();

  refreshTreeView();
  updateValue();
  updateToolbar();
}

void WbSceneTree::reset() {
  WbField *field = mSelectedItem->field();
  assert(field);

  if (field->isTemplateRegenerator())
    // stop editing otherwise unapplied changes could cause issue during template PROTO regeneration
    mFieldEditor->currentEditor()->stopEditing();

  if (field->singleType() == WB_SF_NODE) {
    bool dictionaryNeedsUpdate = false;
    WbNode *parentNode = mSelectedItem->parent()->node();

    // check if referred DEF node is going to be deleted
    bool containsReferredNode = false;
    WbSFNode *sfnode = dynamic_cast<WbSFNode *>(field->value());
    WbMFNode *mfnode = dynamic_cast<WbMFNode *>(field->value());
    if (sfnode) {
      mRowsAreAboutToBeRemoved = sfnode->value();
      containsReferredNode = sfnode->value() && sfnode->value()->hasAreferredDefNodeDescendant();
    } else if (mfnode) {
      mRowsAreAboutToBeRemoved = mfnode->size() > 0;
      WbMFIterator<WbMFNode, WbNode *> it(mfnode);
      while (it.hasNext()) {
        if (it.next()->hasAreferredDefNodeDescendant()) {
          containsReferredNode = true;
          break;
        }
      }
    }
    if (containsReferredNode) {
      if (WbMessageBox::question(tr("This field contains a DEF node on which at least one external USE node depends. "
                                    "Deleting it will turn its first USE node into a DEF node.\n"
                                    "Do you want to continue?"),
                                 this, tr("DEF node deletion")) == QMessageBox::Cancel) {
        mRowsAreAboutToBeRemoved = false;
        return;
      }

      dictionaryNeedsUpdate = true;
    }

    mFieldEditor->editField(NULL, NULL);

    const WbValue *defaultValue = field->defaultValue();
    // in case changes to this field triggers the PROTO template regeneration
    // and the default value is not the empty one, then we want to skip the first
    // template regeneration so that the field pointer is valid when applying the
    // default value
    bool blockTemplateRegeneration =
      field->isTemplateRegenerator() && ((sfnode && dynamic_cast<const WbSFNode *>(defaultValue)->value() != NULL) ||
                                         (mfnode && dynamic_cast<const WbMFNode *>(defaultValue)->size() > 0));

    // notify node deletion (needed for example to propagate it during the streaming)
    if (sfnode && sfnode->value() != NULL)
      WbNodeOperations::instance()->notifyNodeDeleted(sfnode->value());
    else if (mfnode && mfnode->size() > 0) {
      WbMFIterator<WbMFNode, WbNode *> it(mfnode);
      while (it.hasNext())
        WbNodeOperations::instance()->notifyNodeDeleted(it.next());
    }

    // reset field to default value
    // in case of SFNode/MFNode field the value is set to NULL or []
    field->reset(blockTemplateRegeneration);

    // create and finalize new node instances
    if (sfnode) {
      WbNode *defaultNode = dynamic_cast<const WbSFNode *>(defaultValue)->value();
      if (defaultNode) {
        WbNode::setGlobalParentNode(parentNode);
        WbNode *newNode = WbConcreteNodeFactory::instance()->createCopy(*defaultNode);
        WbNode::setGlobalParentNode(NULL);
        newNode->setParentNode(parentNode);

#ifndef NDEBUG
        const WbNodeOperations::OperationResult result =
#endif
          WbNodeOperations::instance()->initNewNode(newNode, parentNode, field, -1, true);
        assert(result != WbNodeOperations::FAILURE);
      }

    } else if (mfnode) {
      const WbMFNode *defaultMFNode = dynamic_cast<const WbMFNode *>(defaultValue);
      WbMFIterator<const WbMFNode, WbNode *> it(defaultMFNode);
      int i = 0;
      while (it.hasNext()) {
        const WbNode *defaultNode = it.next();
        WbNode::setGlobalParentNode(parentNode);
        WbNode *newNode = WbConcreteNodeFactory::instance()->createCopy(*defaultNode);
        WbNode::setGlobalParentNode(NULL);
        newNode->setParentNode(parentNode);
#ifndef NDEBUG
        const WbNodeOperations::OperationResult result =
#endif
          WbNodeOperations::instance()->initNewNode(newNode, parentNode, field, i, true);
        assert(result != WbNodeOperations::FAILURE);
        ++i;
      }
    }

    mRowsAreAboutToBeRemoved = false;
    // no undo function available for SFNode and MFNode
    WbUndoStack::instance()->clear();

    if (dictionaryNeedsUpdate)
      WbNodeOperations::instance()->updateDictionary(false, NULL);
    updateSelection();

  } else {
    WbUndoStack::instance()->push(new WbResetCommand(mSelectedItem->field()));
    mModel->updateItem(mSelectedItem);
  }

  WbWorld::instance()->setModifiedFromSceneTree();

  updateValue();
  updateToolbar();
}

void WbSceneTree::transform(const QString &modelName) {
  WbNode *const currentNode = mSelectedItem->node();
  assert(dynamic_cast<WbGroup *>(currentNode));

  // check if loosing information
  if (WbNodeUtilities::isSuitableForTransform(currentNode, modelName) == WbNodeUtilities::LOOSING_INFO) {
    if (WbMessageBox::question(tr("Warning: Transforming a %1 into a %2 node will loose some information.")
                                   .arg(currentNode->nodeModelName())
                                   .arg(modelName) +
                                 "\n" + tr("Do you still want to proceed?"),
                               this) == QMessageBox::Cancel) {
      mFieldEditor->updateValue();
      return;
    }
  }

  mRowsAreAboutToBeRemoved = true;  // As rows may be removed during the transform operation, we deactivate the item selection
                                    // update and restore it afterwards

  const QModelIndex currentModelIndex = mModel->itemToIndex(mSelectedItem);
  const bool isExpanded = mTreeView->isExpanded(currentModelIndex);

  // create new node
  WbNode::setGlobalParentNode(currentNode->parentNode());
  WbNode *const newNode = WbConcreteNodeFactory::instance()->createNode(modelName, 0, currentNode->parentNode());
  if (!newNode) {
    WbLog::error(tr("Transformation aborted: impossible to create a node of type %1.").arg(modelName));
    mRowsAreAboutToBeRemoved = false;
    return;
  }

  // copy fields and adopt children
  WbNode::setGlobalParentNode(newNode);
  QVector<WbField *> fields = currentNode->fieldsOrParameters();
  foreach (WbField *originalField, fields) {
    // copy field if it exists
    WbField *const newField = newNode->findField(originalField->name());
    if (newField)
      newField->copyValueFrom(originalField);
  }
  newNode->setDefName(currentNode->defName());
  WbNode::setGlobalParentNode(NULL);

  // reassign pointer in parent
  WbField *parentField = mSelectedItem->parent()->field();
  WbNode *upperTemplate =
    WbNodeUtilities::findUpperTemplateNeedingRegenerationFromField(parentField, currentNode->parentNode());
  bool isInsideATemplateRegenerator = upperTemplate && upperTemplate != currentNode;
  if (mSelectedItem->isSFNode()) {
    WbSFNode *const sfnode = dynamic_cast<WbSFNode *>(mSelectedItem->field()->value());
    assert(sfnode);
    WbNodeOperations::instance()->notifyNodeDeleted(currentNode);
    WbTemplateManager::instance()->blockRegeneration(true);
    mSelectedItem->del();  // remove previous item
    sfnode->setValue(newNode);
    newNode->validate();
    WbTemplateManager::instance()->blockRegeneration(false);
  } else {
    assert(mSelectedItem->parent()->isField());
    WbMFNode *mfnode = dynamic_cast<WbMFNode *>(parentField->value());
    assert(mfnode);
    int nodeIndex = mfnode->nodeIndex(currentNode);
    WbNodeOperations::instance()->notifyNodeDeleted(currentNode);
    WbTemplateManager::instance()->blockRegeneration(true);
    // remove currentNode
    mfnode->removeItem(nodeIndex);  // delete currentNode instance
    // insert just after currentNode
    mfnode->insertItem(nodeIndex, newNode);
    // mfnode->setItem(nodeIndex, newNode); // TODO: make WbMFNode::setItem() work!
    newNode->validate();
    WbTemplateManager::instance()->blockRegeneration(false);
  }

  if (!isInsideATemplateRegenerator)
    static_cast<WbBaseNode *>(newNode)->finalize();

  mRowsAreAboutToBeRemoved = false;

  if (!isInsideATemplateRegenerator) {
    const QModelIndex newModelIndex = mModel->findModelIndexFromNode(newNode);
    mTreeView->setCurrentIndex(newModelIndex);
    mTreeView->setExpanded(newModelIndex, isExpanded);
    mTreeView->scrollToModelIndex(newModelIndex);
    WbNodeOperations::instance()->requestUpdateDictionary();
  }

  updateSelection();
  updateValue();
  updateToolbar();

  WbUndoStack::instance()->clear();  // clear undo stack if no available UNDO/REDO implementation of transform action
}

void WbSceneTree::convertToBaseNode() {
  convertProtoToBaseNode(false);
}

void WbSceneTree::convertRootToBaseNode() {
  convertProtoToBaseNode(true);
}

void WbSceneTree::convertProtoToBaseNode(bool rootOnly) {
  WbNode *const currentNode = mSelectedItem->node();
  if (currentNode->isProtoInstance()) {
    const WbSolid *const solid = dynamic_cast<WbSolid *>(currentNode);
    WbViewpoint *viewpoint = WbWorld::instance()->viewpoint();
    const bool isFollowedNode = (solid && viewpoint->followedSolid() == solid);
    int index;
    WbField *parentField = currentNode->parentFieldAndIndex(index);
    WbNode *parentNode = currentNode->parentNode();
    QString nodeString;
    WbVrmlWriter writer(&nodeString, currentNode->modelName() + ".proto");
    if (rootOnly)
      writer.setRootNode(currentNode);
    else
      writer.setRootNode(NULL);
    currentNode->write(writer);

    const bool skipTemplateRegeneration =
      WbNodeUtilities::findUpperTemplateNeedingRegenerationFromField(parentField, parentNode);
    if (skipTemplateRegeneration)
      // PROTO will be regenerated after importing the converted node
      parentField->blockSignals(true);
    // remove previous node
    WbNodeOperations::instance()->deleteNode(currentNode);
    if (skipTemplateRegeneration)
      parentField->blockSignals(false);

    // copy textures
    QHashIterator<QString, QString> it(writer.texturesList());
    while (it.hasNext()) {
      it.next();
      const QString destination(WbProject::current()->worldsPath() + it.key());
      const QFileInfo fileInfo(destination);
      if (!QDir(fileInfo.absolutePath()).exists())
        QDir().mkpath(fileInfo.absolutePath());
      QFile::copy(it.value(), destination);
    }
    // import new node
    if (WbNodeOperations::instance()->importNode(parentNode, parentField, index, "", nodeString) == WbNodeOperations::SUCCESS) {
      WbNode *node = NULL;
      if (parentField->type() == WB_SF_NODE)
        node = static_cast<WbSFNode *>(parentField->value())->value();
      else if (parentField->type() == WB_MF_NODE)
        node = static_cast<WbMFNode *>(parentField->value())->item(index);
      if (isFollowedNode)
        viewpoint->startFollowUp(dynamic_cast<WbSolid *>(node), true);
    }
    WbWorld::instance()->setModifiedFromSceneTree();
  }
  updateSelection();
  updateValue();
  updateToolbar();

  WbUndoStack::instance()->clear();
}

void WbSceneTree::moveViewpointToObject() {
  if (!mSelectedItem)
    return;

  WbTreeItem *itemToMoveTo = mSelectedItem;
  while (true) {
    if (itemToMoveTo->isNode() || itemToMoveTo->isSFNode()) {
      WbNode *node = itemToMoveTo->node();
      WbBaseNode *baseNode = dynamic_cast<WbBaseNode *>(node);
      if (baseNode && WbWorld::instance()->viewpoint()->moveViewpointToObject(baseNode))
        break;
      if (node->isTopLevel())
        break;
    }
    itemToMoveTo = itemToMoveTo->parent();
  }
}

bool WbSceneTree::insertInertiaMatrix(const WbField *selectedField) {
  const QList<WbField *> &internalFields = selectedField->internalFields();
  const int n = internalFields.size();
  const QString &selectedFieldName = (n > 0) ? internalFields.at(0)->name() : selectedField->name();

  if (selectedFieldName != "inertiaMatrix")
    return false;

  WbPhysics *physics = NULL, *internalPhysics = NULL;
  WbSolid *solid = NULL;
  bool validBoundingObject = false;
  bool parameter = selectedField->alias().isEmpty() == false;

  if (n <= 1) {  // selectedField is either a non-parameter 'inertiaMatrix' field or a parameter with only one internal field
    const WbField *p = NULL;
    const WbField *ip = NULL;
    if (parameter == false) {  // non-parameter case
      const WbNode *const nodeParent = selectedField->parentNode();
      assert(nodeParent);
      p = nodeParent->parentField();
    } else
      p = WbNodeUtilities::findFieldParent(internalFields.at(0), true);

    assert(p);
    const int m = p->internalFields().size();
    if (m <= 1) {
      if (m == 1) {
        ip = p->internalFields().at(0);
        internalPhysics = dynamic_cast<WbPhysics *>(dynamic_cast<WbSFNode *>(ip->value())->value());
      }
      physics = dynamic_cast<WbPhysics *>(dynamic_cast<WbSFNode *>(p->value())->value());
      assert(physics);
      solid = internalPhysics ? internalPhysics->upperSolid() : physics->upperSolid();
      assert(solid);
      validBoundingObject |= solid->hasAvalidBoundingObject();
    }
  }

  WbAddInertiaMatrixDialog dialog(validBoundingObject && !parameter, this);

  if (dialog.exec() == QDialog::Rejected)
    return true;

  WbMFVector3 *const mfvector3 = dynamic_cast<WbMFVector3 *>(selectedField->value());
  assert(mfvector3->size() == 0);

  if (dialog.inertiaMatrixType() == WbAddInertiaMatrixDialog::IDENTITY_MATRIX) {
    if (physics && physics->mass() <= 0.0) {
      physics->setMass(1.0, true);
      physics->parsingInfo(tr("A positive mass is mandatory when using inertiaMatrix. 'mass' set to 1."));
    }

    if (physics && physics->centerOfMass().size() == 0) {
      physics->setCenterOfMass(0.0, 0.0, 0.0, true);
      physics->parsingInfo(tr("A center of mass is mandatory when using inertiaMatrix. Default center of mass inserted."));
    }

    mfvector3->insertItem(0, WbVector3(1.0, 1.0, 1.0));
    mfvector3->insertDefaultItem(1);

  } else if (dialog.inertiaMatrixType() == WbAddInertiaMatrixDialog::BOUNDING_OBJECT_BASED && solid)
    solid->setInertiaMatrixFromBoundingObject();

  WbWorld::instance()->setModified();

  updateToolbar();

  return true;
}

void WbSceneTree::addNew() {
  if (mSelectedItem == NULL) {
    mSelectedItem = mModel->rootItem()->lastChild();
    assert(mSelectedItem);
  }

  // set selected WbField and WbNode
  WbTreeItem *selectedFieldItem = NULL;
  WbField *selectedField = NULL;
  WbNode *selectedNodeParent = NULL;
  int newNodeIndex = 0;

  if (!mSelectedItem->isNode()) {
    // field or item
    if (mSelectedItem->isField()) {  // field
      selectedFieldItem = mSelectedItem;
      selectedField = selectedFieldItem->field();
    } else {  // item
      newNodeIndex = mSelectedItem->row() + 1;
      selectedFieldItem = mSelectedItem->parent();
      selectedField = selectedFieldItem->field();
    }

    // if multiple item field
    // directly add item without opening the dialog
    WbMultipleValue *const mvalue = dynamic_cast<WbMultipleValue *>(selectedField->value());
    WbMFNode *const mfnode = dynamic_cast<WbMFNode *>(selectedField->value());
    if (mvalue && !mfnode) {
      if (insertInertiaMatrix(selectedField))
        return;

      // add default item
      WbUndoStack::instance()->push(new WbAddItemCommand(selectedField, mvalue, newNodeIndex));
      return;
    }

    selectedNodeParent = mSelectedItem->parent()->node();

  } else {  // node
    newNodeIndex = mSelectedItem->row() + 1;
    selectedFieldItem = mSelectedItem->parent();
    selectedField = selectedFieldItem->field();
    selectedNodeParent = mSelectedItem->node()->parentNode();
  }

  assert(selectedNodeParent && selectedField);
  if (selectedField->name().startsWith("device") && WbNodeUtilities::hasARobotAncestor(selectedNodeParent) == false) {
    WbMessageBox::info("You cannot insert a device inside a Joint node which is not part of a Robot. Consider transforming the "
                       "Solid at the top into a Robot",
                       this, "Device insertion disabled");
    return;
  }

  WbAddNodeDialog dialog(selectedNodeParent, selectedField, newNodeIndex, this);

  if (dialog.exec() == QDialog::Rejected)
    return;

  // import node
  bool isNodeRegenerated = false;
  if (dialog.action() == WbAddNodeDialog::IMPORT) {
    WbBaseNode *const parentBaseNode = dynamic_cast<WbBaseNode *>(selectedNodeParent);
    WbNodeOperations::instance()->importNode(parentBaseNode, selectedField, newNodeIndex, dialog.fileName());

  } else {  // CREATE

    // create node
    WbNode::setGlobalParentNode(selectedNodeParent);
    WbNode *newNode;
    if (dialog.isUseNode()) {
      // find last DEF node to be copied
      WbNode *const definitionNode = dialog.defNode();
      if (!definitionNode) {
        WbLog::error(tr("New node creation failed: node with DEF name %1 does not exist.").arg(dialog.modelName()));
        return;
      }
      newNode = definitionNode->cloneAndReferenceProtoInstance();
      newNode->makeUseNode(definitionNode);

    } else {
      const QString &str = dialog.protoFilePath();
      const QString *const protoFilePath = str.isEmpty() ? NULL : &str;
      newNode = WbConcreteNodeFactory::instance()->createNode(dialog.modelName(), NULL, selectedNodeParent, protoFilePath);
    }

    if (!newNode) {
      WbLog::error(tr("New node creation failed: model name %1.").arg(dialog.modelName()));
      return;
    }

    const WbNodeOperations::OperationResult result =
      WbNodeOperations::instance()->initNewNode(newNode, selectedNodeParent, selectedField, newNodeIndex);
    if (result == WbNodeOperations::FAILURE)
      return;
    isNodeRegenerated = result == WbNodeOperations::REGENERATION_REQUIRED;

    // if selectedField is a template regenerator, the parent will anyway be regenerated
    if (!isNodeRegenerated && !selectedField->isTemplateRegenerator())
      WbNodeOperations::instance()->notifyNodeAdded(newNode);
  }

  updateSelection();

  if (isNodeRegenerated && mSelectedItem && mSelectedItem->isField() && !mSelectedItem->isSFNode()) {
    QModelIndex newItemModelIndex = mModel->itemToIndex(mSelectedItem->child(newNodeIndex));
    mTreeView->setCurrentIndex(newItemModelIndex);
  }

  mTreeView->scrollToModelIndex(mModel->itemToIndex(mSelectedItem));

  WbWorld::instance()->setModifiedFromSceneTree();

  updateValue();
  updateToolbar();
}

void WbSceneTree::updateToolbar() {
  if (mRowsAreAboutToBeRemoved || WbNodeOperations::instance()->isSkipUpdates())
    // don't use mSelectedItem if updateSelection() was skipped
    return;

  mActionManager->setEnabled(WbAction::DEL, mSelectedItem && mSelectedItem->canDelete());
  mActionManager->setEnabled(WbAction::ADD_NEW, !mSelectedItem || mSelectedItem->canInsert());
  updateApplicationActions();
}

void WbSceneTree::updateApplicationActions() {
  mClipboard->update();
  mActionManager->setEnabled(WbAction::UNDO, WbUndoStack::instance()->canUndo());
  mActionManager->setEnabled(WbAction::REDO, WbUndoStack::instance()->canRedo());
  mActionManager->setEnabled(WbAction::COPY, mSelectedItem && mSelectedItem->canCopy());
  mActionManager->setEnabled(WbAction::CUT, mSelectedItem && mSelectedItem->canCut());
  mActionManager->setEnabled(WbAction::PASTE, isPasteAllowed());
  mActionManager->setEnabled(WbAction::SELECT_ALL, false);
}

bool WbSceneTree::isPasteAllowed() {
  if (!mSelectedItem || mSelectedItem->isInvalid() || mClipboard->isEmpty())
    return false;

  WbField *field = NULL;
  if (mClipboard->type() == WB_SF_NODE) {
    if (mSelectedItem->isItem())
      return false;

    if (mSelectedItem->isField() && !(mSelectedItem->field()->type() & WB_SF_NODE))
      // selected field is neither SFNode nor MFNode
      return false;

    // paste SFNode
    WbNode *parentNode = NULL;
    if (mSelectedItem->isField()) {
      field = mSelectedItem->field();
      parentNode = mSelectedItem->parent()->node();
    } else {  // else sibling node
      field = mSelectedItem->parent()->field();
      parentNode = mSelectedItem->node()->parentNode();
    }

    // prevent pasting a node between WorldInfo and Viewpoint nodes
    if (parentNode->isWorldRoot() && mSelectedItem->row() < 1)
      return false;

    if (!(field->type() & WB_SF_NODE))
      return false;

    // semantic checks
    const WbClipboard::WbClipboardNodeInfo *clipboardNodeInfo = mClipboard->nodeInfo();
    const QString &nodeModelName = clipboardNodeInfo->nodeModelName;
    QString errorMessage;
    if (!WbNodeUtilities::isAllowedToInsert(field, nodeModelName, parentNode, errorMessage,
                                            static_cast<const WbBaseNode *>(parentNode)->nodeUse(), clipboardNodeInfo->slotType,
                                            QStringList() << nodeModelName << clipboardNodeInfo->modelName))
      return false;

    // allow to paste devices node only in robot top nodes
    if (!WbNodeUtilities::isRobotTypeName(nodeModelName) && clipboardNodeInfo->hasADeviceDescendant) {
      const WbNode *const topNode = WbNodeUtilities::findTopNode(parentNode);
      if (!topNode || !WbNodeUtilities::isRobotTypeName(topNode->nodeModelName()))
        return false;
    }

    return true;
  } else {
    if (mSelectedItem->isField())
      field = mSelectedItem->field();
    else if (mSelectedItem->isItem())
      field = mSelectedItem->parent()->field();
  }

  if (mSelectedItem->isNode() || mSelectedItem->isSFNode() || !field || (field->isMultiple() && !mSelectedItem->canInsert()))
    return false;

  int selectedType = mSelectedItem->field()->singleType();
  return mClipboard->type() == selectedType;
}

void WbSceneTree::clearSelection() {
  if (mModel == NULL || mTreeView == NULL)
    // quitting Webots
    return;

  mTreeView->clearSelection();
  mTreeView->setCurrentIndex(QModelIndex());
  mSelectedItem = NULL;

  mFieldEditor->setTitle("");
  mFieldEditor->editField(NULL, NULL);
}

void WbSceneTree::updateSelection() {
  if (mTreeView == NULL)
    // quitting Webots
    return;

  WbNodeOperations *nodeOperations = WbNodeOperations::instance();
  if (nodeOperations->isFromSupervisor())
    // do not update selection if change come from supervisor
    return;

  if (nodeOperations->areNodesAboutToBeInserted() || mRowsAreAboutToBeRemoved || nodeOperations->isSkipUpdates())
    // avoid updating the selection if some nodes are about to be inserted or deleted
    return;

  QModelIndex currentIndex = mTreeView->currentIndex();
  if (!currentIndex.isValid()) {
    mSelectedItem = NULL;
    mActionManager->action(WbAction::MOVE_VIEWPOINT_TO_OBJECT)->setEnabled(false);
    mActionManager->action(WbAction::OPEN_HELP)->setEnabled(false);
    updateToolbar();
    // no item selected
    return;
  }
  mSelectedItem = mModel->indexToItem(currentIndex);
  if (mSelectedItem->isInvalid()) {
    mSelectedItem = NULL;
    mActionManager->action(WbAction::MOVE_VIEWPOINT_TO_OBJECT)->setEnabled(false);
    mActionManager->action(WbAction::OPEN_HELP)->setEnabled(false);
    updateToolbar();
    return;
  }

  bool isNonNullNode = false;

  WbField *const field = mSelectedItem->field();
  if (mSelectedItem->isField()) {
    const WbSFNode *const sfnode = dynamic_cast<WbSFNode *>(field->value());
    isNonNullNode = sfnode && sfnode->value();
    WbNode *const node = mSelectedItem->parent()->node();
    mFieldEditor->editField(node, field, -1);
  } else if (mSelectedItem->isItem()) {
    WbNode *const node = mSelectedItem->parent()->parent()->node();
    mFieldEditor->editField(node, field, mSelectedItem->row());
  } else {  // node
    WbNode *const node = mSelectedItem->parent()->node();
    isNonNullNode = true;
    mFieldEditor->editField(node, mSelectedItem->parent()->field(), mSelectedItem->row());
  }

  WbContextMenuGenerator::enableNodeActions(mSelectedItem->isNode());
  WbContextMenuGenerator::enableRobotActions(mSelectedItem->node() &&
                                             WbNodeUtilities::isRobotTypeName(mSelectedItem->node()->nodeModelName()));
  WbContextMenuGenerator::enableProtoActions(mSelectedItem->node() && mSelectedItem->node()->isProtoInstance());

  QWidget *lastEditorWidget = mFieldEditor->lastEditorWidget();
  if (lastEditorWidget)
    setTabOrder(lastEditorWidget, mTreeView);

  updateToolbar();

  // emit a message in order to inform WbSelection about the selected node
  const WbTreeItem *const item = isNonNullNode ? mSelectedItem : mModel->findUpperNodeItem(mSelectedItem);
  if (item) {
    WbBaseNode *baseNode = dynamic_cast<WbBaseNode *>(item->node());
    if (baseNode && baseNode->isProtoParameterNode())
      // select first proto parameter node instance
      baseNode = baseNode->getFirstFinalizedProtoInstance();

    if (baseNode && !baseNode->isPostFinalizedCalled())
      // ignore not initialized nodes
      baseNode = NULL;

    // enable move viewpoint to object if the item has a corresponding bounding sphere
    mActionManager->action(WbAction::MOVE_VIEWPOINT_TO_OBJECT)
      ->setEnabled(baseNode && WbNodeUtilities::boundingSphereAncestor(baseNode) != NULL &&
                   baseNode->nodeType() != WB_NODE_BILLBOARD &&
                   !WbNodeUtilities::findUpperNodeByType(baseNode, WB_NODE_BILLBOARD));
    mActionManager->action(WbAction::OPEN_HELP)->setEnabled(baseNode);
    emit nodeSelected(baseNode);
  }
}

void WbSceneTree::startWatching(const QModelIndex &index) {
  mModel->startWatching(index);
}

void WbSceneTree::stopWatching(const QModelIndex &index) {
  mModel->stopWatching(index);
}

bool WbSceneTree::isIndexAncestorOfCurrentIndex(const QModelIndex &index, int start, int end) {
  QModelIndex currentIndex = mTreeView->currentIndex();
  while (currentIndex.isValid()) {
    if (currentIndex.parent() == index && currentIndex.row() >= start && currentIndex.row() <= end)
      return true;
    currentIndex = currentIndex.parent();
  }
  return false;
}

void WbSceneTree::handleRowRemoval(const QModelIndex &parentIndex, int start, int end) {
  mRowsAreAboutToBeRemoved = false;
  if (!WbNodeOperations::instance()->isFromSupervisor() || isIndexAncestorOfCurrentIndex(parentIndex, start, end))
    clearSelection();
  updateToolbar();
}

void WbSceneTree::selectTransform(WbAbstractTransform *t) {
  if (t == NULL) {
    clearSelection();
    return;
  }

  QModelIndex newIndex = mModel->findModelIndexFromNode(t->baseNode());
  if (newIndex.isValid()) {
    mTreeView->clearSelection();
    mTreeView->setCurrentIndex(newIndex);
    mTreeView->scrollToModelIndex(newIndex);
  } else if (t->baseNode()->protoParameterNode())
    // if m is proto parameter node instance, select the corresponding parameter node in the scene tree
    selectTransform(dynamic_cast<WbAbstractTransform *>(t->baseNode()->protoParameterNode()));
}

// for the translation and rotation fields of Solid node we need to set
// the initial translation and rotation values
void WbSceneTree::updateValue() {
  WbWorld *world = WbWorld::instance();
  world->setModified();

  if (mSelectedItem && mSelectedItem->isField()) {
    WbField *const field = mSelectedItem->field();
    QString fieldName = field->name();
    if (fieldName == "scale" || mSelectedItem->isSFNode()) {
      // update values displayed in field editor
      mFieldEditor->editField(mSelectedItem->parent()->node(), field, -1);
    }

    mFieldEditor->updateValue();
  }

  emit valueChangedFromGui();
}

void WbSceneTree::refreshItems() {
  if (mModel && mModel->rootItem() && WbSimulationState::instance()->isPaused()) {
    mModel->updateAllSceneTreeValues();
  } else if (mSelectedItem && mSelectedItem->isDataRefreshNeeded()) {
    mSelectedItem->refreshData();
  } else if (mModel)
    // QTreeView refresh is problematic and sometimes won't happen unless forced
    // so we force it here in the default case by telling the WbSceneTreeModel
    // to refresh its data
    mModel->emitLayoutChanged();
}

QByteArray WbSceneTree::saveState() const {
  return mSplitter->saveState();
}

void WbSceneTree::restoreState(QByteArray state) {
  mSplitter->restoreState(state);
  mSplitter->setHandleWidth(mHandleWidth);
}

void WbSceneTree::restoreFactoryLayout() {
  const int halfSplitterHeight = mSplitter->height() * 0.5;
  int preferredFieldEditorHeight = gFactoryFieldEditorHeightHint;
  if (preferredFieldEditorHeight > halfSplitterHeight)
    // default field editor height should never be bigger than scene tree height
    preferredFieldEditorHeight = halfSplitterHeight;

  QList<int> sizes;
  sizes << (mSplitter->height() - preferredFieldEditorHeight) << preferredFieldEditorHeight;
  mSplitter->setSizes(sizes);
  mSplitter->setHandleWidth(mHandleWidth);
}

void WbSceneTree::prepareNodeRegeneration(WbNode *node, bool nested) {
  // The node given as argument will be regenerated soon:
  // - the tree state at this position is stored for a later restoration.
  // - the view updates are blocked.
  // - the focus widget is stored.

  if (nested) {
    if (mSelectedItem)
      // In the case of nested PROTOs, prepareNodeRegeneration() will be
      // called again later on the uppermost PROTO which requires a regeneration.
      // The mSelectedItem pointer should be kept until this next call.
      // Setting it as invalid helps to avoid bad pointer references during the procedural PROTO regeneration.
      mSelectedItem->makeInvalid();
    return;
  }

  assert(node);

  setUpdatesEnabled(false);

  mFocusWidgetBeforeNodeRegeneration = QApplication::focusWidget();

  mSelectionBeforeTreeStateRegeneration = NULL;

  // Store the selected item only if not inside the node which will be regenerated.
  // Indeed this node (and its WbTreeItem(s)) will be destroyed and recreated.
  WbNode *n = NULL;
  if (mSelectedItem && !mSelectedItem->isInvalid()) {
    if (mSelectedItem->isField()) {
      const WbSFNode *const sfnode = dynamic_cast<WbSFNode *>(mSelectedItem->field()->value());
      if (sfnode && sfnode->value())
        n = sfnode->value();
      else
        n = mSelectedItem->parent()->node();
    } else if (mSelectedItem->isItem())
      n = mSelectedItem->parent()->parent()->node();
    else  // node
      n = mSelectedItem->node();
  }
  mSelectionInsideTreeStateRecovery = n == NULL;
  while (n) {
    if (n == node || n->protoParameterNode() == node) {
      mSelectionInsideTreeStateRecovery = true;
      break;
    }
    n = n->parentNode();
  }
  if (!mSelectionInsideTreeStateRecovery)
    mSelectionBeforeTreeStateRegeneration = mModel->indexToItem(mTreeView->currentIndex());

  // Store the tree state of the node about to be regenerated.
  cleanTreeItemState(mTreeItemState);
  mTreeItemState = new TreeItemState;
  storeTreeItemState(mModel->indexToItem(mModel->findModelIndexFromNode(node)), mTreeItemState);

  // Clear the selection the time to regnerate the node, in order to avoid invalid pointers.
  clearSelection();
}

void WbSceneTree::abortNodeRegeneration() {
  // The node regeneration failed: restore the best possible state.

  cleanTreeItemState(mTreeItemState);
  mTreeItemState = NULL;

  // Restore the tree state as best as possible
  if (!mSelectionInsideTreeStateRecovery) {
    QModelIndex index = mModel->itemToIndex(mSelectionBeforeTreeStateRegeneration);
    if (index.isValid()) {
      mSelectedItem = mSelectionBeforeTreeStateRegeneration;
      mTreeView->setCurrentIndex(index);
    } else {
      mSelectedItem = NULL;
      mTreeView->setCurrentIndex(QModelIndex());  // mTreeView->clearSelection() doesn't change the current index.
    }
    mSelectionBeforeTreeStateRegeneration = NULL;
  }
  updateSelection();

  setUpdatesEnabled(true);

  if (mFocusWidgetBeforeNodeRegeneration) {
    mFocusWidgetBeforeNodeRegeneration->setFocus();
    mFocusWidgetBeforeNodeRegeneration = NULL;
  }
}

void WbSceneTree::applyNodeRegeneration(WbNode *node) {
  assert(node);

  // Restore the tree state as best as possible
  restoreTreeItemState(mModel->indexToItem(mModel->findModelIndexFromNode(node)), mTreeItemState, NULL);
  cleanTreeItemState(mTreeItemState);
  mTreeItemState = NULL;

  // Restore the selection if the previous selection was outside the regenerated node.
  if (!mSelectionInsideTreeStateRecovery) {
    QModelIndex index = mModel->itemToIndex(mSelectionBeforeTreeStateRegeneration);
    if (index.isValid()) {
      mSelectedItem = mSelectionBeforeTreeStateRegeneration;
      mTreeView->setCurrentIndex(index);
    } else {
      mSelectedItem = NULL;
      mTreeView->setCurrentIndex(QModelIndex());  // mTreeView->clearSelection() doesn't change the current index.
    }
    mSelectionBeforeTreeStateRegeneration = NULL;
  }
  updateSelection();

  setUpdatesEnabled(true);

  // TODO: this shouldn't be done in the scene tree
  // The best way to fix this would be to move WbDictionary in another module (vrml?)
  WbNodeOperations::instance()->updateDictionary(false, dynamic_cast<WbBaseNode *>(node));

  if (mFocusWidgetBeforeNodeRegeneration) {
    mFocusWidgetBeforeNodeRegeneration->setFocus();
    mFocusWidgetBeforeNodeRegeneration = NULL;
  }
}

void WbSceneTree::storeTreeItemState(WbTreeItem *treeItem, TreeItemState *treeItemState) {
  // Store a tree state at a given item.

  assert(treeItem);
  assert(treeItemState);

  QModelIndex index = mModel->itemToIndex(treeItem);

  treeItemState->expanded = mTreeView->isExpanded(index);
  treeItemState->selected = mSelectedItem == treeItem;

  if (treeItemState->expanded) {  // no need to store the children of an unexpanded node.
    for (int i = 0; i < treeItem->childCount(); ++i) {
      TreeItemState *newChild = new TreeItemState;
      storeTreeItemState(treeItem->child(i), newChild);
      treeItemState->children.append(newChild);
    }
  }
}

void WbSceneTree::restoreTreeItemState(WbTreeItem *treeItem, TreeItemState *treeItemState, WbBaseNode *lastNode) {
  // Restore a tree state at a given item.

  assert(treeItem);
  assert(treeItemState);

  QModelIndex index = mModel->itemToIndex(treeItem);

  // Keep the bottommost node in the selection tree, in order to be able to restore its selection later.
  WbBaseNode *newLastNode = (treeItem->isNode()) ? dynamic_cast<WbBaseNode *>(treeItem->node()) : lastNode;

  // Restore the expansion status.
  if (treeItemState->expanded)
    mTreeView->setExpanded(index, true);
  // Restore the selection status.
  if (treeItemState->selected) {
    // 1. Restore the node selection.
    if (newLastNode && newLastNode->isPostFinalizedCalled())
      WbSelection::instance()->selectNodeFromSceneTree(newLastNode);
    // 2. Restore the tree index which could be a field or a node.
    mTreeView->setCurrentIndex(index);
    mSelectedItem = treeItem;
  }

  int counter = 0;
  foreach (TreeItemState *child, treeItemState->children) {
    if (counter < treeItem->childCount())
      restoreTreeItemState(treeItem->child(counter), child, newLastNode);
    counter++;
  }
}

void WbSceneTree::cleanTreeItemState(TreeItemState *item) {
  // clean a tree state.
  if (item == NULL)
    return;

  foreach (TreeItemState *fChild, item->children)
    cleanTreeItemState(fChild);
  delete item;
}

// Debug function to print the stored tree state.
// #include <QtCore/QDebug>
// void WbSceneTree::printTreeItemState(TreeItemState *treeItemState, int indentation) {
//
//   QString indent(2 * indentation, ' ');
//
//   qDebug() << indent << treeItemState->expanded;
//   qDebug() << indent << treeItemState->selected;
//
//   foreach (TreeItemState *it, treeItemState->children)
//     printTreeItemState(it, indentation + 1);
// }

void WbSceneTree::handleDoubleClickOrEnterPress() {
  if (!mSelectedItem)
    return;

  // we can't use isDefault() on the SFNode field because PROTOs can have
  // non-NULL default SFNode values, so cast to SFNode and get the real value
  // stored in the field
  if ((mSelectedItem->isSFNode() && mSelectedItem->node() == NULL) ||
      (mSelectedItem->isField() && mSelectedItem->field()->isMultiple() &&
       reinterpret_cast<WbMultipleValue *>(mSelectedItem->field()->value())->isEmpty()))
    addNew();
  // set focus on first edit box of the current value editor for immediate keyboard editing
  else if ((mSelectedItem->isItem() && !mSelectedItem->isNode() && mSelectedItem->field()->isMultiple()) ||
           (mSelectedItem->isField() && !mSelectedItem->isSFNode() && !mSelectedItem->field()->isMultiple()))
    mFieldEditor->currentEditor()->takeKeyboardFocus();
  // default behavior, collapse/expoand tree item
  else if (mTreeView->isExpanded(mTreeView->currentIndex()))
    mTreeView->collapse(mTreeView->currentIndex());
  else
    mTreeView->expand(mTreeView->currentIndex());
}

void WbSceneTree::refreshTreeView() {
  mModel->emitLayoutChanged();
}

void WbSceneTree::help() {
  if (!mSelectedItem)
    return;

  const WbNode *node = mSelectedItem->node();
  if (!node && mSelectedItem->field())
    node = mSelectedItem->field()->parentNode();
  if (node) {
    const QStringList &bookAndPage = node->documentationBookAndPage(WbNodeUtilities::isRobotTypeName(node->nodeModelName()));
    emit documentationRequest(bookAndPage[0], bookAndPage[1], true);
  }
}

void WbSceneTree::exportObject() {
  if (!mSelectedItem || !mSelectedItem->node())
    return;

  // Fix for Qt 5.3.0 that does not work correctly on Ubuntu
  // if dialog parent widget is not a top level widget
  QWidget *topLevelWidget = this;
  while (topLevelWidget->parentWidget()) {
    topLevelWidget = topLevelWidget->parentWidget();
  }

  const QString fileName = QFileDialog::getSaveFileName(
    topLevelWidget, tr("Export Webots Object"),
    WbProject::computeBestPathForSaveAs(WbPreferences::instance()->value("Directories/objects").toString() + "/" +
                                        mSelectedItem->node()->modelName() + ".wbo"),
    tr("All files(*.wbo *.WBO *.wrl *.WRL *.urdf *.URDF);;Webots object (*.wbo *.WBO);;VRML (*.wrl *.WRL);;URDF (*.urdf "
       "*.URDF)"));

  if (fileName.isEmpty())
    return;

  const QStringList supportedExtension = QStringList() << ".wbo"
                                                       << ".wrl"
                                                       << ".urdf";
  bool extensionSupported = false;
  for (int i = 0; i < supportedExtension.size(); ++i) {
    if (fileName.endsWith(supportedExtension[i], Qt::CaseInsensitive)) {
      extensionSupported = true;
      break;
    }
  }

  if (!extensionSupported) {
    WbLog::error(tr("Unsupported '%1' extension.").arg(QFileInfo(fileName).suffix()));
    return;
  }

  if (fileName.endsWith("urdf", Qt::CaseInsensitive) && !mSelectedItem->node()->isRobot()) {
    WbLog::error(tr("URDF exportation available only for Robot nodes."));
    return;
  }

  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) {
    WbLog::error(tr("Impossible to write file: '%1'.").arg(fileName) + "\n" + tr("Node exportation failed."));
    return;
  }

  WbNode::enableDefNodeTrackInWrite(true);
  WbVrmlWriter writer(&file, fileName);
  writer.writeHeader(fileName);
  mSelectedItem->node()->write(writer);
  writer.writeFooter();
  WbNode::disableDefNodeTrackInWrite();
  file.close();
}

void WbSceneTree::openProtoInTextEditor() {
  if (mSelectedItem && mSelectedItem->node())
    emit editRequested(mSelectedItem->node()->proto()->fileName());
}

void WbSceneTree::openTemplateInstanceInTextEditor() {
  if (!mSelectedItem || !mSelectedItem->node())
    return;

  if (mSelectedItem->node()->isTemplate()) {
    const QString &templateInstancePath = mSelectedItem->node()->protoInstanceFilePath();
    if (!templateInstancePath.isEmpty())
      emit editRequested(templateInstancePath);
  }
}
