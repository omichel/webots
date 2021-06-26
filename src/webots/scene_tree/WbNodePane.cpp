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

#include "WbNodePane.hpp"

#include "WbField.hpp"
#include "WbNodeEditor.hpp"
#include "WbPhysicsViewer.hpp"
#include "WbPositionViewer.hpp"
#include "WbSolid.hpp"
#include "WbTransform.hpp"
#include "WbVelocityViewer.hpp"

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTabWidget>

static const QStringList cTabNames = QStringList() << "Node"
                                                   << "Mass"
                                                   << "Position"
                                                   << "Velocity";

WbNodePane::WbNodePane(QWidget *parent) :
  WbValueEditor(parent),
  mTabs(new QTabWidget(this)),
  mNodeEditor(new WbNodeEditor()),
  mPhysicsViewer(new WbPhysicsViewer()),
  mPositionViewer(new WbPositionViewer()),
  mVelocityViewer(new WbVelocityViewer()) {
  // tabs added only when this editor has focus
  // otherwise they affects the minimum size of other editors
  mLayout->addWidget(mTabs, 1, 1);
  mTabs->setObjectName("NodePane");
  connect(mTabs, &QTabWidget::currentChanged, this, &WbNodePane::updateSelectedTab);
  updateSelectedTab();
}

WbNodePane::~WbNodePane() {
  disconnect(mTabs, &QTabWidget::currentChanged, this, &WbNodePane::updateSelectedTab);
  delete mNodeEditor;
  mNodeEditor = NULL;
  delete mPhysicsViewer;
  mPhysicsViewer = NULL;
  delete mPositionViewer;
  mPositionViewer = NULL;
  delete mVelocityViewer;
  mVelocityViewer = NULL;
}

void WbNodePane::recursiveBlockSignals(bool block) {
  blockSignals(block);
  mNodeEditor->recursiveBlockSignals(block);
  mPhysicsViewer->blockSignals(block);
  mPositionViewer->blockSignals(block);
  mVelocityViewer->blockSignals(block);
  mTabs->blockSignals(block);
}

void WbNodePane::edit(WbNode *node, WbField *field, int index) {
  mNodeEditor->WbValueEditor::edit(node, field, index);
  WbValueEditor::edit(node, field, index);
}

void WbNodePane::cleanValue() {
  mPhysicsViewer->clean();
  mPositionViewer->clean();
  mVelocityViewer->clean();
  WbValueEditor::cleanValue();
  mNodeEditor->cleanValue();
}

void WbNodePane::stopEditing() {
  WbValueEditor::stopEditing();
  mNodeEditor->stopEditing();
  mPhysicsViewer->stopUpdating();
  mPositionViewer->stopUpdating();
  mVelocityViewer->stopUpdating();
  mPhysicsViewer->clean();
  mPositionViewer->clean();
  mVelocityViewer->clean();
  mNodeEditor->cleanValue();
  // remove tabs
  disconnect(mTabs, &QTabWidget::currentChanged, this, &WbNodePane::updateSelectedTab);
  int tabsCount = mTabs->count();
  for (int i = 0; i < tabsCount; ++i)
    mTabs->removeTab(0);
  connect(mTabs, &QTabWidget::currentChanged, this, &WbNodePane::updateSelectedTab);
}

void WbNodePane::edit(bool copyOriginalValue) {
  if (copyOriginalValue) {
    const WbField *const f = field();

    WbNode *node = NULL;
    if (singleValue())
      node = static_cast<WbSFNode *>(f->value())->value();
    else if (multipleValue())
      node = static_cast<WbMFNode *>(f->value())->item(index());

    if (node)
      node = static_cast<WbBaseNode *>(node)->getFirstFinalizedProtoInstance();

    // update and add tabs if needed
    mNodeEditor->edit(false);
    enableTab(NODE_TAB, mNodeEditor, true);

    WbTransform *t = dynamic_cast<WbTransform *>(node);
    WbSolid *s = dynamic_cast<WbSolid *>(node);
    mPhysicsViewer->show(s);
    mPositionViewer->show(t);
    enableTab(POSITION_TAB, mPositionViewer, t != NULL);
    mVelocityViewer->show(s);
    enableTab(VELOCITY_TAB, mVelocityViewer, s != NULL);
  }

  update();
}

void WbNodePane::update() {
  mPositionViewer->update();
  mVelocityViewer->update();

  bool physicsEnabled = mPhysicsViewer->update();
  enableTab(PHYSICS_TAB, mPhysicsViewer, physicsEnabled);
}

void WbNodePane::resetFocus() {
  mNodeEditor->resetFocus();
}

void WbNodePane::apply() {
  if (mTabs->currentWidget() == mNodeEditor)
    mNodeEditor->apply();
}

void WbNodePane::updateSelectedTab() {
  mPhysicsViewer->setSelected(mTabs->currentWidget() == mPhysicsViewer);
  mPositionViewer->setSelected(mTabs->currentWidget() == mPositionViewer);
  mVelocityViewer->setSelected(mTabs->currentWidget() == mVelocityViewer);
}

void WbNodePane::enableTab(int index, QWidget *widget, bool enabled) {
  bool tabExists = false;
  int i = 0;
  for (; i <= index; ++i) {
    if (mTabs->tabText(i) == cTabNames[index]) {
      tabExists = true;
      break;
    }
  }

  int insertIndex = index;
  if (insertIndex > mTabs->count())
    insertIndex = mTabs->count();

  if (enabled) {
    if (!tabExists)
      mTabs->insertTab(insertIndex, widget, cTabNames[index]);
  } else if (tabExists)
    mTabs->removeTab(i);
}
