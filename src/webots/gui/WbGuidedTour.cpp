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

#include "WbGuidedTour.hpp"

#include "WbApplication.hpp"
#include "WbConsole.hpp"
#include "WbMFString.hpp"
#include "WbSFString.hpp"
#include "WbSimulationState.hpp"
#include "WbStandardPaths.hpp"
#include "WbWorld.hpp"
#include "WbWorldInfo.hpp"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>

#include <float.h>

static WbGuidedTour *gInstance = NULL;

WbGuidedTour *WbGuidedTour::instance(QWidget *parent) {
  if (!gInstance)
    gInstance = new WbGuidedTour(parent);

  return gInstance;
}

WbGuidedTour::WbGuidedTour(QWidget *parent) :
  QDialog(parent, Qt::Tool) {  // Qt::Tool allows to handle well the z-order. This is mainly advantageous on Mac
  mIndex = -1;
  mDeadline = DBL_MAX;
  setAttribute(Qt::WA_DeleteOnClose, true);

  mTimer = new QTimer(this);
  connect(mTimer, &QTimer::timeout, this, &WbGuidedTour::shoot);
  mTimer->start(250);  // trigger every 250 milliseconds

  setWindowTitle(tr("Guided Tour - Webots"));
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  QVBoxLayout *rightPaneLayout = new QVBoxLayout();
  QHBoxLayout *headerLayout = new QHBoxLayout();
  QHBoxLayout *buttonLayout = new QHBoxLayout();

  mTree = new QTreeWidget(this);
  mTree->setHeaderHidden(true);
  mTree->setSelectionMode(QAbstractItemView::SingleSelection);
  connect(mTree, &QTreeWidget::itemSelectionChanged, this, &WbGuidedTour::selectWorld);

  QPixmap pixmap("coreIcons:webots64x64.png");
  QLabel *pixmapLabel = new QLabel(this);
  pixmapLabel->setPixmap(pixmap);

  mTitleLabel = new QLabel(this);
  setTitleText(tr("Webots Guided Tour!"));

  headerLayout->addWidget(pixmapLabel);
  headerLayout->addSpacing(20);
  headerLayout->addWidget(mTitleLabel);
  headerLayout->addStretch();

  mInfoText =
    new QPlainTextEdit(tr("Welcome to the Webots Guided Tour.") + "\n" +
                         tr("The tour will take you through many examples and will give you an overview of Webots features.") +
                         "\n\n" + tr("Check [Auto] or press [Next] to start...") + "\n",
                       this);
  mInfoText->setReadOnly(true);

  mAutoBox = new QCheckBox(tr("Auto"), this);
  mPrevButton = new QPushButton(tr("Previous"), this);
  mPrevButton->setEnabled(false);
  mNextButton = new QPushButton(tr("Next"), this);
  mNextButton->setDefault(true);
  mNextButton->setAutoDefault(true);
  QPushButton *closeButton = new QPushButton(tr("Close"), this);

  connect(closeButton, &QPushButton::pressed, this, &WbGuidedTour::close);
  connect(mPrevButton, &QPushButton::pressed, this, &WbGuidedTour::prev);
  connect(mNextButton, &QPushButton::pressed, this, &WbGuidedTour::next);
  connect(mAutoBox, &QCheckBox::clicked, this, &WbGuidedTour::setSimulationDeadline);

  buttonLayout->addWidget(mAutoBox);
  buttonLayout->addSpacing(100);
  buttonLayout->addWidget(mPrevButton);
  buttonLayout->addWidget(mNextButton);
  buttonLayout->addWidget(closeButton);

  rightPaneLayout->addLayout(headerLayout);
  rightPaneLayout->addWidget(mInfoText);
  rightPaneLayout->addLayout(buttonLayout);

  mainLayout->addWidget(mTree);
  mainLayout->addLayout(rightPaneLayout);

  loadWorldList();
  updateGUI();
}

WbGuidedTour::~WbGuidedTour() {
  delete mTimer;
  gInstance = NULL;
}

void WbGuidedTour::loadWorldList() {
  QFile file(WbStandardPaths::projectsPath() + "guided_tour.txt");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTreeWidgetItem *treeWidgetItem = NULL;
  QTextStream stream(&file);
  while (!stream.atEnd()) {
    QString line = stream.readLine();
    if (line.startsWith("#") || line.isEmpty())
      continue;
    if (line.startsWith("[")) {
      const int p = line.indexOf("]");
      const QString &title = line.mid(1, p - 1);
      treeWidgetItem = new QTreeWidgetItem(QStringList(title));
      mTree->addTopLevelItem(treeWidgetItem);
      mSections.append(treeWidgetItem);
      continue;
    }
    if (treeWidgetItem == NULL)  // ignore files if a section was not declared
      continue;
    const QStringList list = line.split(" ");
    mFilenames.append(list[0]);
    int duration;
    if (list.size() > 1)
      duration = list[1].toInt();
    else
      duration = 20;  // default to 20 seconds
    mDurations.append(duration);
    const QString &title = list[0].mid(list[0].lastIndexOf("/") + 1);
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidgetItem, QStringList(title));
    item->setIcon(0, QIcon("coreIcons:webots_doc.png"));
    treeWidgetItem->addChild(item);
    mWorlds.append(item);
  }
}

void WbGuidedTour::setTitleText(const QString &title) {
  mTitleLabel->setText("<h2>" + title + "</h2>");
}

static QString formatInfo(const WbMFString &info) {
  QString outputText;
  QString item;
  QString lowerCaseItem;
  for (int i = 0; i < info.size(); i++) {
    item = info.item(i);
    lowerCaseItem = item.toLower();
    if (!lowerCaseItem.startsWith("date")) {
      if (item.contains("Author", Qt::CaseInsensitive)) {
        item.replace(QString("Author"), QString("Credits"), Qt::CaseInsensitive);
        outputText += "\n" + item + "\n";
      } else if (item.contains("Authors", Qt::CaseInsensitive)) {
        item.replace(QString("Authors"), QString("Credits"), Qt::CaseInsensitive);
        outputText += "\n" + item + "\n";
      } else
        outputText += item + "\n";
    }
  }
  return outputText;
}

void WbGuidedTour::updateGUI() {
  if (mFilenames.isEmpty()) {
    setTitleText(tr("Internal error"));
    mInfoText->setPlainText(tr("The Guided Tour is not available."));
  } else if (mIndex == -1) {
    setTitleText(tr("Webots Guided Tour"));
    mInfoText->setPlainText(tr("Welcome to the Webots Guided Tour.") + "\n" +
                            tr("The tour will take you through many examples and "
                               "will give you an overview of Webots features.") +
                            "\n\n" + tr("Check [Auto] or press [Next] to start..."));
  } else if (mIndex == mFilenames.size()) {
    setTitleText(tr("That's all Folks!"));
    mInfoText->setPlainText(tr("Thanks for viewing the Webots Guided Tour.") + "\n\n" + tr("Press [Close] to terminate..."));
  } else {  // Normal case
    // Sets world's title
    setTitleText(WbWorld::instance()->worldInfo()->title() + QString(" (%1/%2)").arg(mIndex + 1).arg(mFilenames.size()));
    // Formats and displays all WorldInfo.info items
    const WbMFString &info = WbWorld::instance()->worldInfo()->info();
    mInfoText->setPlainText(formatInfo(info));
  }
  // Updates buttons
  mNextButton->setEnabled(mIndex < (mFilenames.size() - 1));
  mPrevButton->setEnabled(mIndex > 0);
  mAutoBox->setEnabled(!mFilenames.isEmpty());
}

void WbGuidedTour::prev() {
  mIndex--;
  selectCurrent();
  mAutoBox->setChecked(false);
  mDeadline = DBL_MAX;
  loadWorld();
}

void WbGuidedTour::next() {
  mIndex++;
  selectCurrent();
  mAutoBox->setChecked(false);
  mDeadline = DBL_MAX;
  loadWorld();
}

void WbGuidedTour::nextWorld() {
  // Called only if mDeadline was reached; most probably mAutoBox->isChecked() = true
  mIndex = (mIndex + 1) % mFilenames.size();  // loop
  selectCurrent();
  setSimulationDeadline(mAutoBox->isChecked());
  loadWorld();
}

void WbGuidedTour::selectCurrent() {
  disconnect(mTree, &QTreeWidget::itemSelectionChanged, this, &WbGuidedTour::selectWorld);
  for (int i = 0; i < mWorlds.size(); i++)
    mWorlds[i]->setSelected(i == mIndex);
  for (int i = 0; i < mSections.size(); i++) {
    mSections[i]->setSelected(false);
    if (mWorlds[mIndex]->parent() == mSections[i])
      mSections[i]->setExpanded(true);
  }
  connect(mTree, &QTreeWidget::itemSelectionChanged, this, &WbGuidedTour::selectWorld);
  mTree->scrollToItem(mWorlds[mIndex]);
}

void WbGuidedTour::shoot() {
  // Called by mTimer every 2.5 seconds
  if (WbSimulationState::instance()->time() >= mDeadline)
    nextWorld();
}

void WbGuidedTour::setSimulationDeadline(bool autoChecked) {
  // On the first user-click
  if (mIndex < 0) {
    nextWorld();
    return;
  }
  if (mIndex >= mFilenames.size())  // last world
    return;
  if (autoChecked)
    mDeadline = 1000 * mDurations[mIndex];
  else
    mDeadline = DBL_MAX;
}

void WbGuidedTour::loadWorld() {
  if (mIndex < 0 || mIndex >= mFilenames.size())
    return;
  const QString &fn = WbStandardPaths::webotsHomePath() + mFilenames[mIndex];  // Gets filename
  WbSimulationState::instance()->setMode(WbSimulationState::REALTIME);         // Sets simulation mode to RUN
  emit worldLoaded(fn);                                                        // Load now!
  updateGUI();
}

void WbGuidedTour::selectWorld() {
  // prevent selecting a new world if in the process of canceling the previous one or if invalid
  if (mTree->selectedItems().size() < 1 || WbApplication::instance()->wasWorldLoadingCanceled())
    return;
  QTreeWidgetItem *item = mTree->selectedItems().at(0);
  mIndex = mWorlds.indexOf(item);
  if (mIndex < 0) {  // section is selected
    if (item->childCount() > 0) {
      item->setExpanded(true);
      // select first world of the section
      mTree->blockSignals(true);  // emit signal only once when child is selected
      item->setSelected(false);
      mTree->blockSignals(false);
      item->child(0)->setSelected(true);
    }
    return;
  }
  loadWorld();
  setSimulationDeadline(mAutoBox->isChecked());
}
