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

#include "WbProjectRelocationDialog.hpp"

#include "WbFileUtil.hpp"
#include "WbLanguage.hpp"
#include "WbLineEdit.hpp"
#include "WbMessageBox.hpp"
#include "WbPreferences.hpp"
#include "WbProject.hpp"
#include "WbProtoModel.hpp"
#include "WbRobot.hpp"
#include "WbSimulationState.hpp"
#include "WbStandardPaths.hpp"
#include "WbWorld.hpp"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QString WbProjectRelocationDialog::mExternalProjectPath = QString();

WbProjectRelocationDialog::WbProjectRelocationDialog(WbProject *project, const QString &relativeFilename,
                                                     const QString &absoluteFilePath, QWidget *parent) :
  QDialog(parent),
  mProject(project),
  mRelativeFilename(relativeFilename),
  mAbsoluteFilePath(absoluteFilePath),
  mSourceEdit(NULL),
  mTargetEdit(NULL),
  mProtoCheckBox(NULL),
  mPluginsCheckBox(NULL),
  mFilesLabel(NULL),
  mConclusionLabel(NULL),
  mStatusEdit(NULL),
  mSelectButton(NULL),
  mCancelButton(NULL),
  mCopyButton(NULL),
  mButtonBox(NULL) {
  setWindowTitle(tr("Project relocation"));

  mIsProtoModified = WbLanguage::findByFileName(mRelativeFilename)->code() == WbLanguage::PROTO;
  mProtoCheckBox = new QCheckBox(tr("Include all PROTO files"), this);
  mProtoCheckBox->setChecked(mIsProtoModified);
  mPluginsCheckBox = new QCheckBox(tr("Include all plugins files"), this);
  const QString &absoluteFilename = mAbsoluteFilePath + mRelativeFilename;
  const bool isPluginModified =
    WbFileUtil::isLocatedInDirectory(absoluteFilename, mProject->path() + "plugins") ||
    (!mExternalProjectPath.isEmpty() && WbFileUtil::isLocatedInDirectory(absoluteFilename, mExternalProjectPath + "plugins"));
  mPluginsCheckBox->setChecked(mIsProtoModified || isPluginModified);

  mButtonBox = new QDialogButtonBox(this);
  mCancelButton = mButtonBox->addButton(QDialogButtonBox::Cancel);
  mCopyButton = mButtonBox->addButton(tr("Copy"), QDialogButtonBox::AcceptRole);
  connect(mCancelButton, &QPushButton::pressed, this, &WbProjectRelocationDialog::reject);
  connect(mCopyButton, &QPushButton::pressed, this, &WbProjectRelocationDialog::copy);

  mStatusEdit = new QPlainTextEdit(this);
  mStatusEdit->setReadOnly(true);

  mIsCompleteRelocation = mProject->isReadOnly();

  if (mIsCompleteRelocation)
    initCompleteRelocation();
  else
    initProtoSourceRelocation();
}

void WbProjectRelocationDialog::initCompleteRelocation() {
  QLabel *title = new QLabel(tr("<b>Copy necessary files from source to target directory?</b>"), this);

  const QString &sourcePath = QDir::toNativeSeparators(mProject->path());
  mTargetPath =
    QDir::toNativeSeparators(WbPreferences::instance()->value("Directories/projects").toString() + mProject->dirName());

  mSourceEdit = new WbLineEdit(sourcePath, this);
  mSourceEdit->setReadOnly(true);
  mSourceEdit->setMinimumWidth(sourcePath.length() * 8);

  mTargetEdit = new WbLineEdit(mTargetPath, this);
  connect(mTargetEdit, &WbLineEdit::textEdited, this, &WbProjectRelocationDialog::targetEdited);

  mSelectButton = new QPushButton(tr("Select"), this);
  connect(mSelectButton, &QPushButton::pressed, this, &WbProjectRelocationDialog::selectDirectory);

  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(mTargetEdit);
  layout->addWidget(mSelectButton);

  QFormLayout *formLayout = new QFormLayout();
  formLayout->addRow(tr("Source directory: "), mSourceEdit);
  formLayout->addRow(tr("Target directory: "), layout);

  setStatus(tr("Please select a target directory then push the [Copy] button."));

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(title);
  mainLayout->addLayout(formLayout);
  if (mProtoCheckBox)
    mainLayout->addWidget(mProtoCheckBox);
  if (mPluginsCheckBox)
    mainLayout->addWidget(mPluginsCheckBox);
  mainLayout->addWidget(mStatusEdit);
  mainLayout->addWidget(mButtonBox);
}

void WbProjectRelocationDialog::initProtoSourceRelocation() {
  QLabel *title = new QLabel(tr("<b>Copy necessary files from source to current project directory?</b>"), this);

  mTargetPath = mProject->path();

  mSourceEdit = new WbLineEdit(QDir::toNativeSeparators(mAbsoluteFilePath), this);
  mSourceEdit->setReadOnly(true);
  mSourceEdit->setMinimumWidth(mAbsoluteFilePath.length() * 8);

  mTargetEdit = new WbLineEdit(QDir::toNativeSeparators(mTargetPath), this);
  mTargetEdit->setReadOnly(true);
  mTargetEdit->setMinimumWidth(mTargetPath.length() * 8);

  QFormLayout *formLayout = new QFormLayout();
  formLayout->addRow(tr("Source directory: "), mSourceEdit);
  formLayout->addRow(tr("Target directory: "), mTargetEdit);

  setStatus(tr("Please select the copy options then push the [Copy] button."));

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(title);
  mainLayout->addLayout(formLayout);
  if (mProtoCheckBox)
    mainLayout->addWidget(mProtoCheckBox);
  if (mPluginsCheckBox)
    mainLayout->addWidget(mPluginsCheckBox);
  mainLayout->addWidget(mStatusEdit);
  mainLayout->addWidget(mButtonBox);
}

WbProjectRelocationDialog::~WbProjectRelocationDialog() {
}

void WbProjectRelocationDialog::setStatus(const QString &text, bool ok) {
  QTextCharFormat format;

  if (ok)
    format.setForeground(QBrush(Qt::black));
  else
    format.setForeground(QBrush(Qt::red));

  mStatusEdit->setCurrentCharFormat(format);
  mStatusEdit->setPlainText(text);

#ifdef __APPLE__
  // Qt bug on macOS: the QDialog::exec() doesn't deal correctly the repaint events as it should.
  repaint();
#endif
}

void WbProjectRelocationDialog::targetEdited(const QString &text) {
  mTargetPath = QDir::fromNativeSeparators(text);
  setStatus(tr("Push the [Copy] button to copy the project."));
}

void WbProjectRelocationDialog::copy() {
  const QString &home = WbStandardPaths::webotsHomePath();
  if (WbFileUtil::isLocatedInDirectory(mTargetPath, home)) {
    setStatus(tr("Target directory is located in the Webots installation directory.") + "\n" +
                tr("Please select another target directory."),
              false);
    return;
  }

  QDir dir(mTargetPath);
  if (mIsCompleteRelocation) {
    if (dir.exists()) {
      if (dir.count() > 2) {  // 2 because special cases: "." and ".." are counted
        setStatus(tr("Target directory is not empty.") + "\n" + tr("Please select another directory."), false);
        return;
      }
    } else {
      if (!QDir::root().mkpath(mTargetPath)) {
        setStatus(
          tr("Could not create target directory.") + "\n" + tr("Make sure that you have write access at this location."),
          false);
        return;
      }
    }
  }

  int copiedFilesCount = 0;
  if (mIsCompleteRelocation)
    copiedFilesCount = copyProject();
  if (!mExternalProjectPath.isEmpty())
    copiedFilesCount += copyExternalProject();

  if (copiedFilesCount == 0) {
    setStatus(tr("Project relocation failed.") + "\n" + tr("Some files or directories could not be copied."));
    return;
  }

  // change buttons
  if (mSelectButton)
    mSelectButton->setEnabled(false);
  mSourceEdit->setEnabled(false);
  mTargetEdit->setEnabled(false);
  if (mProtoCheckBox)
    mProtoCheckBox->setEnabled(false);
  if (mPluginsCheckBox)
    mPluginsCheckBox->setEnabled(false);
  mButtonBox->removeButton(mCancelButton);
  mButtonBox->removeButton(mCopyButton);
  QPushButton *closeButton = mButtonBox->addButton(QDialogButtonBox::Close);
  closeButton->setFocus();
  connect(closeButton, &QPushButton::pressed, this, &WbProjectRelocationDialog::accept);

  mProject->setPath(dir.path());

  // store the accepted project directory in the preferences
  dir.cdUp();  // store the upper level, probably the path where the directories are stored
  WbPreferences::instance()->setValue("Directories/projects", dir.absolutePath() + "/");

  // good news
  setStatus(tr("Project successfully relocated.") + "\n" + tr("%1 file(s) copied.").arg(copiedFilesCount));
}

int WbProjectRelocationDialog::copyExternalProject() {
  // Here we should copy only the files that may be edited / changed by the user, that is:
  // 1) textures and meshes in protos folders if PROTO file was modified
  // 2) controller that was modified or used by modified PROTO
  // 3) possibly the current controller file
  // 4) project libraries if a controller or plugin is copied
  // 5) project motions folder if a controller is copied
  int result = 0;

  if (mIsProtoModified) {
    // copy all PROTO textures
    result +=
      WbFileUtil::copyDir(mExternalProjectPath + "/protos/textures", mTargetPath + "/protos/textures", true, true, true);
    result += WbFileUtil::copyDir(mExternalProjectPath + "/protos/meshes", mTargetPath + "/protos/meshes", true, true, true);
  }

  // copy PROTO controller
  bool projectLibrariesCopied = false;
  const QList<WbRobot *> &robots = WbWorld::instance()->robots();
  foreach (WbRobot *robot, robots) {
    WbProtoModel *proto = robot->proto();
    if (!proto)
      continue;
    const QString &controllerName = robot->controllerName();
    const QString &controllerPath = robot->controllerDir();
    const QString &projectControllerPath = mExternalProjectPath + "controllers/" + controllerName + "/";
    QDir protoControllerDir(QFileInfo(proto->fileName()).path());
    protoControllerDir.cdUp();
    const QString &protoControllerPath = protoControllerDir.path() + "/controllers/" + controllerName + "/";
    if ((mIsProtoModified && proto->fileName() == (mExternalProjectPath + mRelativeFilename)) ||
        protoControllerPath == projectControllerPath) {
      // copy controller if modified or if the PROTO was modified
      result += WbFileUtil::copyDir(controllerPath, mTargetPath + "/controllers/" + controllerName, true, false, true);
      result += WbFileUtil::copyDir(protoControllerDir.path() + "/libraries", mTargetPath + "/libraries", true, false, true);
      if (!projectLibrariesCopied && protoControllerDir.path() + '/' == mExternalProjectPath)
        projectLibrariesCopied = true;
    }
  }

  // copy the current source folder
  QString relativeDirPath = mRelativeFilename;
  if (QFileInfo(mRelativeFilename).isFile()) {
    // from after the "controllers/" part of the string
    relativeDirPath = mRelativeFilename.left(mRelativeFilename.indexOf("/", 12));
  }

  const QString &currentController = mTargetPath + "/" + relativeDirPath;
  if (!QDir(currentController).exists()) {
    result += WbFileUtil::copyDir(mExternalProjectPath + relativeDirPath, currentController, true, true, true);
    result += WbFileUtil::copyDir(mExternalProjectPath + "motions", mTargetPath + "/motions", true, true, true);
  }
  if (mProtoCheckBox && mProtoCheckBox->isChecked())
    result += WbFileUtil::copyDir(mExternalProjectPath + "protos", mTargetPath + "/protos", true, true, true);
  if (mPluginsCheckBox && mPluginsCheckBox->isChecked()) {
    result += WbFileUtil::copyDir(mExternalProjectPath + "plugins", mTargetPath + "/plugins", true, false, true);
    if (!projectLibrariesCopied)
      result += WbFileUtil::copyDir(mExternalProjectPath + "libraries", mTargetPath + "/libraries", true, false, true);
  }
  return result;
}

int WbProjectRelocationDialog::copyProject() {
  // Here we should copy only the files that may be edited / changed by the user, that is:
  // 1) the current world file and associated wbproj file
  // 2) all the textures used explicitly by this world file and present in the local worlds
  //    folder (but not the ones used implicitly by PROTOs).
  // 3) all the controllers (folders) of the project used by non-PROTO robots from this world file
  // 4) all the controllers (folders) and related libraries used by PROTO robots which are not located in the
  //    controllers folder at the same level as the protos folder of the PROTO robot
  // 5) possibly the current controller file (even if corresponding to a PROTO robot)
  // 6) project libraries if a project controller or plugin is copied
  // 7) copy skins folder if it exists

  const WbWorld *world = WbWorld::instance();
  const QString &worldFileBaseName = QFileInfo(world->fileName()).baseName();

  int result = 0;

  // copy the .wbt file, the .wbproj file
  QDir targetPathDir(mTargetPath + "/worlds");
  targetPathDir.mkpath(".");
  const QString &targetWorld(mTargetPath + "/worlds/" + worldFileBaseName);
  if (QFile::copy(mProject->path() + "worlds/" + worldFileBaseName + ".wbt", targetWorld + ".wbt")) {
    QFile::setPermissions(targetWorld + ".wbt",
                          QFile::permissions(targetWorld + ".wbt") | QFile::WriteOwner | QFile::WriteUser);
    result++;
  }
  const QString &targetProjectFile(mTargetPath + "/worlds/." + worldFileBaseName + ".wbproj");
  if (QFile::copy(mProject->path() + "worlds/." + worldFileBaseName + ".wbproj", targetProjectFile)) {
    QFile::setPermissions(targetProjectFile, QFile::permissions(targetProjectFile) | QFile::WriteOwner | QFile::WriteUser);
    result++;
  }

  // copy only the needed texture files
  const QStringList &textureList = world->listTextureFiles();
  foreach (const QString &textureFile, textureList) {
    const QFileInfo &fi(textureFile);
    targetPathDir.mkpath(fi.path());
    if (QFile::copy(mProject->path() + "worlds/" + textureFile, mTargetPath + "/worlds/" + textureFile))
      result++;
  }

  bool copyLibraries = false;
  bool projectLibrariesCopied = false;

  // copy only the needed robot controllers
  const QList<WbRobot *> &robots = world->robots();
  QStringList copiedControllers;
  foreach (WbRobot *robot, robots) {
    const QString &controllerName = robot->controllerName();
    if (copiedControllers.contains(controllerName))
      continue;
    copiedControllers << controllerName;
    const QString &controllerPath = robot->controllerDir();
    const QString &projectControllerPath = mProject->path() + "controllers/" + controllerName + "/";
    const WbProtoModel *proto = robot->proto();
    bool isThisProtoModified = false;
    QString protoLibrariesPath;
    QDir protoControllerDir;
    if (proto) {
      protoControllerDir.setPath(QFileInfo(proto->fileName()).path());
      protoControllerDir.cdUp();
      const QString &protoControllerPath = protoControllerDir.path() + "/controllers/" + controllerName + "/";
      protoLibrariesPath = protoControllerDir.path() + "/libraries/";
      if (mIsProtoModified && proto->fileName() == (mAbsoluteFilePath + mRelativeFilename))
        isThisProtoModified = true;
      else if ((mProtoCheckBox == NULL || !mProtoCheckBox->isChecked()) && protoControllerPath == projectControllerPath)
        // this controller is associated with a proto file that has not been modified
        // no need to copy it
        continue;
    }
    if (!isThisProtoModified && controllerPath != projectControllerPath)  // the controller is not included in the project
      continue;                                                           // maybe a default controller or a proto controller
    result += WbFileUtil::copyDir(controllerPath, mTargetPath + "/controllers/" + controllerName, true, false, true);
    result += WbFileUtil::copyDir(mProject->path() + "motions", mTargetPath + "/motions", true, true, true);
    if (proto) {
      result += WbFileUtil::copyDir(protoLibrariesPath, mTargetPath + "/libraries/", true, false, true);
      if (!projectLibrariesCopied && protoControllerDir.path() + '/' == mProject->path())
        projectLibrariesCopied = true;
    } else
      copyLibraries = true;
  }

  // copy the current controller (if not already done)
  QString source = mProject->path() + mRelativeFilename;
  QString destination = mTargetPath + "/" + mRelativeFilename;
  if (QFileInfo(source).isFile()) {
    // if it's not a directory, but a file, get the containing controller directory from after the "controllers/" part of the
    // string
    QString dir = mRelativeFilename.left(mRelativeFilename.indexOf("/", 12));
    source = mProject->path() + dir;
    destination = mTargetPath + "/" + dir;
  }

  if (!QDir(destination).exists())
    result += WbFileUtil::copyDir(source, destination, true, true, true);

  if (mProtoCheckBox && mProtoCheckBox->isChecked())
    result += WbFileUtil::copyDir(mProject->path() + "protos", mTargetPath + "/protos", true, true, true);
  if (mPluginsCheckBox && mPluginsCheckBox->isChecked()) {
    result += WbFileUtil::copyDir(mProject->path() + "plugins", mTargetPath + "/plugins", true, false, true);
    copyLibraries = true;
  }

  if (copyLibraries && !projectLibrariesCopied)
    result += WbFileUtil::copyDir(mProject->path() + "libraries", mTargetPath + "/libraries", true, false, true);

  const QString skinsPath = WbProject::current()->path() + "skins/";
  if (QDir(skinsPath).exists())
    result += WbFileUtil::copyDir(skinsPath, mTargetPath + "/skins", true, true, true);

  return result;
}

void WbProjectRelocationDialog::selectDirectory() {
  const QString &path = QFileDialog::getExistingDirectory(this, "Select a directory", mTargetEdit->text());

  if (path.isEmpty())
    return;

  mTargetPath = path;
  mTargetEdit->setText(QDir::toNativeSeparators(mTargetPath));
  setStatus(tr("Push the [Copy] button to copy the necessary project files."));
}

bool WbProjectRelocationDialog::validateLocation(QWidget *parent, QString &filename, bool isImportingVrml) {
  mExternalProjectPath.clear();

  // if file is not in installation directory: it's ok
  if (!WbFileUtil::isLocatedInInstallationDirectory(filename))
    return true;

  WbSimulationState *simulationState = WbSimulationState::instance();
  simulationState->pauseSimulation();

  // use native separators in user dialog
  const QString &nativeFilename = QDir::toNativeSeparators(filename);

  WbProject *current = WbProject::current();
  if (!WbFileUtil::isLocatedInDirectory(filename, current->path()) ||
      WbFileUtil::isLocatedInDirectory(filename, WbStandardPaths::resourcesPath())) {
    const QList<WbRobot *> &robots = WbWorld::instance()->robots();
    foreach (WbRobot *robot, robots) {
      const WbProtoModel *proto = robot->proto();
      if (!proto)
        continue;

      QDir protoProjectDir(QFileInfo(proto->fileName()).path());
      protoProjectDir.cdUp();
      if (WbFileUtil::isLocatedInDirectory(filename, protoProjectDir.absolutePath())) {
        mExternalProjectPath = protoProjectDir.absolutePath();
        if (!mExternalProjectPath.endsWith("/"))
          mExternalProjectPath = mExternalProjectPath + "/";
        break;
      }
    }

    if (mExternalProjectPath.isEmpty()) {
      // file is not in current project
      WbMessageBox::warning(tr("You are trying to modify a file located in Webots installation directory:") + "\n\n'" +
                              nativeFilename + "'\n\n" + tr("This operation is not permitted."),
                            parent);
      simulationState->resumeSimulation();
      return false;
    }
  }

  const QString &modificationDescription =
    isImportingVrml ? tr("You are trying to import VRML97 nodes to a world located in the Webots installation directory:") :
                      tr("You are trying to modify a file located in the Webots installation directory:");

  // file is in current project
  if (WbMessageBox::question(
        modificationDescription + "\n\n'" + nativeFilename + "'\n\n" +
          tr("This operation is not permitted: would you like to copy the necessary files to another location?"),
        parent) == QMessageBox::Cancel) {
    simulationState->resumeSimulation();
    return false;
  }

  // change filename parameter: get relative filename with respect to previous project path
  QString absolutePath;
  if (mExternalProjectPath.isEmpty()) {
    filename = QDir(current->path()).relativeFilePath(filename);
    absolutePath = current->path();
  } else {
    filename = QDir(mExternalProjectPath).relativeFilePath(filename);
    absolutePath = mExternalProjectPath;
  }

  // relocate dialog
  WbProjectRelocationDialog dialog(current, filename, absolutePath, parent);
  dialog.exec();
  if (dialog.result() == QDialog::Rejected) {
    simulationState->resumeSimulation();
    return false;
  }

  // change filename parameter: set absolute filename with respect to the new project path
  filename = QDir(dialog.targetPath()).absoluteFilePath(filename);

  simulationState->resumeSimulation();
  return true;
}
