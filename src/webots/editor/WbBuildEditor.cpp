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

#include "WbBuildEditor.hpp"
#include "WbLanguage.hpp"
#include "WbLog.hpp"
#include "WbMessageBox.hpp"
#include "WbPreferences.hpp"
#include "WbProject.hpp"
#include "WbProjectRelocationDialog.hpp"
#include "WbSimulationState.hpp"
#include "WbStandardPaths.hpp"
#include "WbSysInfo.hpp"
#include "WbTextBuffer.hpp"
#include "WbWorld.hpp"

#include "../../../include/controller/c/webots/utils/ansi_codes.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>

#include <cassert>

static WbBuildEditor *gInstance = NULL;

WbBuildEditor *WbBuildEditor::instance() {
  return gInstance;
}

WbBuildEditor::WbBuildEditor(QWidget *parent, const QString &toolBarAlign) :
  WbTextEditor(parent, toolBarAlign),
  mTargetModificationTimeBeforeMake(),
  mTargetFile(),
  mIsCleaning(false) {
  gInstance = this;
  mProcess = NULL;
  createActions();
}

WbBuildEditor::~WbBuildEditor() {
  gInstance = NULL;
}

void WbBuildEditor::createActions() {
  QAction *action = mBuildAction = new QAction(this);

  QIcon icon = QIcon();
  icon.addFile("enabledIcons:make_button.png", QSize(), QIcon::Normal);
  icon.addFile("disabledIcons:make_button.png", QSize(), QIcon::Disabled);
  action->setText(tr("&Build"));
  action->setStatusTip(tr("Build the current project."));
  action->setToolTip(action->statusTip());
  action->setShortcut(Qt::Key_F7);
  action->setIcon(icon);
  connect(action, &QAction::triggered, this, &WbBuildEditor::build);

  icon = QIcon();
  icon.addFile("enabledIcons:clean_button.png", QSize(), QIcon::Normal);
  icon.addFile("disabledIcons:clean_button.png", QSize(), QIcon::Disabled);
  action = mCleanAction = new QAction(this);
  action->setText(tr("C&lean"));
  action->setStatusTip(tr("Remove intermediate build files."));
  action->setToolTip(action->statusTip());
  action->setShortcut(Qt::SHIFT + Qt::Key_F7);
  action->setIcon(icon);
  connect(action, &QAction::triggered, this, &WbBuildEditor::clean);

  action = mCrossCompileAction = new QAction(this);
  action->setText(tr("Cr&oss-compile"));
  action->setStatusTip(tr("Cross compile the current file."));
  action->setToolTip(action->statusTip());
  action->setShortcut(Qt::Key_F8);
  connect(action, &QAction::triggered, this, &WbBuildEditor::crossCompile);

  action = mCleanCrossCompilationAction = new QAction(this);
  action->setText(tr("Cross-compilation cl&ean"));
  action->setStatusTip(tr("Remove intermediate cross-compilation files."));
  action->setToolTip(action->statusTip());
  action->setShortcut(Qt::SHIFT + Qt::Key_F8);
  connect(action, &QAction::triggered, this, &WbBuildEditor::cleanCrossCompilation);

  action = mMakeJarAction = new QAction(this);
  action->setText(tr("Make JAR file"));
  action->setStatusTip(tr("Create a JAR executable file."));
  action->setToolTip(action->statusTip());
  connect(action, &QAction::triggered, this, &WbBuildEditor::makeJar);

  QToolBar *bar = toolBar();
  bar->addSeparator();
  bar->addAction(mBuildAction);
  bar->widgetForAction(mBuildAction)->setObjectName("editorButton");
  bar->addAction(mCleanAction);
  bar->widgetForAction(mCleanAction)->setObjectName("editorButton");
}

void WbBuildEditor::updateBuildButtons() {
  // see if there is the buffer can be compiled
  bool enable = currentBuffer() && currentBuffer()->language()->isCompilable() && !mProcess;

  mBuildAction->setEnabled(enable);
  mCleanAction->setEnabled(enable);

  mMakeJarAction->setEnabled(enable && (currentBuffer()->language()->code() == WbLanguage::JAVA));

  enable = enable && !mCrossCompileMakefile.isEmpty();
  mCrossCompileAction->setEnabled(enable);
  mCleanCrossCompilationAction->setEnabled(enable);
}

// find the directory just above the compilationDirectories list
const QDir WbBuildEditor::compileDir() const {
  static QStringList compilationDirectories;
  if (compilationDirectories.size() == 0) {
    compilationDirectories << "controllers";
    compilationDirectories << "libraries";
    compilationDirectories << "remote_controls";
    compilationDirectories << "robot_windows";
    compilationDirectories << "physics";
    compilationDirectories << "plugins";
  }

  assert(currentBuffer());
  QDir compileDir = currentBuffer()->fileDir();
  QDir resultDir = compileDir;
  QString subDirectoryPath = compileDir.absolutePath();

  while (compileDir.cdUp()) {
    if (compilationDirectories.contains(compileDir.dirName())) {
      resultDir.setPath(subDirectoryPath);
      break;
    }

    subDirectoryPath = compileDir.absolutePath();
  }

  return resultDir;
}

void WbBuildEditor::updateGui() {
  if (currentBuffer()) {
    // check if there is a cross-compile Makefile
    QStringList filters;
    filters << "Makefile.*"
            << "makefile.*";
    QStringList files = compileDir().entryList(filters);
    if (files.size() > 0)
      mCrossCompileMakefile = files.first();
    else
      mCrossCompileMakefile.clear();
  }

  WbTextEditor::updateGui();
  updateBuildButtons();
}

void WbBuildEditor::build() {
  unmarkError();
  mIsCleaning = false;
  make("");
}

void WbBuildEditor::clean() {
  unmarkError();
  mIsCleaning = true;
  WbLog::appendStdout(ANSI_CLEAR_SCREEN, WbLog::COMPILATION);
  make("clean");
}

void WbBuildEditor::makeJar() {
  unmarkError();
  make("jar");
}

void WbBuildEditor::crossCompile() {
  unmarkError();
  mIsCleaning = false;
  make("-f " + mCrossCompileMakefile);
}

void WbBuildEditor::cleanCrossCompilation() {
  unmarkError();
  mIsCleaning = true;
  WbLog::appendStdout(ANSI_CLEAR_SCREEN, WbLog::COMPILATION);
  make("-f " + mCrossCompileMakefile + " clean");
}

void WbBuildEditor::readStdout() {
  QByteArray bytes = mProcess->readAllStandardOutput();
  QString out = QString::fromUtf8(bytes.data(), bytes.size());

  // on windows replace CR+LF by CR
#ifdef _WIN32
  out.replace("\r\n", "\n");
#endif

  WbLog::appendStdout(out, WbLog::COMPILATION);
}

void WbBuildEditor::readStderr() {
  QByteArray bytes = mProcess->readAllStandardError();
  QString err = QString::fromUtf8(bytes.data(), bytes.size());

  // on windows replace CR+LF by CR
#ifdef _WIN32
  err.replace("\r\n", "\n");
#endif

  WbLog::appendStderr(err, WbLog::COMPILATION);
}

void WbBuildEditor::cleanupProcess() {
  delete mProcess;
  mProcess = NULL;
  updateBuildButtons();
}

void WbBuildEditor::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  switch (exitStatus) {
    case QProcess::NormalExit: {
      if (mIsCleaning)
        WbLog::appendStdout("Clean finished.\n", WbLog::COMPILATION);
      else
        reloadMessageBoxIfNeeded();
      break;
    }
    case QProcess::CrashExit:
      // should not happen, but just in case
      WbLog::appendStderr("external make process crashed!\n", WbLog::COMPILATION);
      break;
  }

  cleanupProcess();
}

void WbBuildEditor::reloadMessageBoxIfNeeded() {
  computeTargetFile();
  QString targetFile = mTargetFile;

  // use the target file instead if existing
  int lastSeparator = targetFile.lastIndexOf('/');
  if (lastSeparator != -1) {
    QString potentialTargetFile = targetFile;
    potentialTargetFile.replace(lastSeparator, 1, "/build/release/");
    if (QFile::exists(potentialTargetFile))
      targetFile = potentialTargetFile;
  }

  // check the modification time
  QFileInfo targetFileInfo(targetFile);
  if (targetFileInfo.exists() && targetFile.startsWith(WbProject::current()->path())) {
    QDateTime targetModificationTimeAfterMake = targetFileInfo.lastModified();
    if (!mTargetModificationTimeBeforeMake.isValid() || targetModificationTimeAfterMake > mTargetModificationTimeBeforeMake) {
      WbLog::appendStdout("Build finished.\n", WbLog::COMPILATION);
      if (WbMessageBox::enabled()) {
        QMessageBox messageBox(QMessageBox::Question, tr("Compilation successful"),
                               tr("Do you want to reset or reload the world?"), QMessageBox::Cancel, this);
        messageBox.addButton(tr("Reload"), QMessageBox::AcceptRole);
        messageBox.setDefaultButton(messageBox.addButton(tr("Reset"), QMessageBox::AcceptRole));
        const int ret = messageBox.exec();
        if (ret == 0)
          emit reloadRequested();
        else if (ret == 1)
          emit resetRequested();
      }
    } else
      WbLog::appendStdout("Nothing to be done for build targets.\n", WbLog::COMPILATION);
  }
}

bool WbBuildEditor::makefileExists(const QString &compilePath) {
  return QFile::exists(compilePath + "/Makefile") || QFile::exists(compilePath + "/makefile");
}

void WbBuildEditor::addMakefileIfNecessary(const QString &compilePath) {
  if (!makefileExists(compilePath)) {
    if (WbMessageBox::question(
          tr("The current directory does not contain any Makefile.") + "\n" + tr("Do you want to add one?"), this) ==
        QMessageBox::Ok) {
      QString original = WbStandardPaths::templatesPath();
      original +=
        (compilePath.contains("/plugins/physics/")) ? QString("plugins/physics/Makefile") : QString("controllers/Makefile");
      if (!QFile::copy(original, compilePath + "/Makefile"))
        WbMessageBox::warning(tr("Makefile creation failed."), this);
    }
  }
}

void WbBuildEditor::make(const QString &target) {
  bool isJavaProgram = (currentBuffer()->language()->code() == WbLanguage::JAVA);

  // find out compilation directory
  // WbTextBuffer *currentBuffer = currentBuffer();
  QString compilePath = compileDir().absolutePath();
// On Windows, make won't work if the Makefile file is located in a path with UTF-8 characters (e.g., Chinese)
#ifdef _WIN32
  if (!isJavaProgram && QString(compilePath.toUtf8()) != QString::fromLocal8Bit(compilePath.toLocal8Bit())) {
    WbMessageBox::warning(tr("\'%1\'\n\nThe path to this Webots project contains non 8-bit characters. "
                             "Webots won't be able to compile any C/C++ controller in this path. "
                             "Please move this Webots project into a folder with only 8-bit characters.")
                            .arg(compilePath),
                          this);
    return;
  }
#endif

  // is in installation directory
  if (!WbProjectRelocationDialog::validateLocation(this, compilePath))
    return;

  // update path of modified files from external project
  const QString &oldProjectPath = WbProjectRelocationDialog::relocatedExternalProtoProjectPath();
  if (!oldProjectPath.isEmpty())
    updateProjectPath(oldProjectPath, WbProject::current()->path());

  // save all files before build
  if (!saveAllFiles())
    return;

  // compute mTarget
  computeTargetFile();

  // store the target modification date
  updateTargetModificationTime();

  QString command;
  QStringList arguments;

  if (isJavaProgram && !makefileExists(compilePath)) {
    QStringList list = getJavaCommandLine(target);
    command = list[0];
    list.removeFirst();
    arguments = list;
  } else {
    command = "make";

    addMakefileIfNecessary(compilePath);

    int numberOfThreads = WbPreferences::instance()->value("General/numberOfThreads", 1).toInt();
    if (numberOfThreads > 1 && target != "clean" && WbSimulationState::instance()->isPaused()) {
      arguments << "-j";
      arguments << QString::number(numberOfThreads);
    }
    if (!target.isEmpty())
      arguments << target;
  }

  if (command.isEmpty())
    return;
  WbLog::appendStdout(command + " " + arguments.join(" ") + "\n", WbLog::COMPILATION);

  // create mProcess
  mProcess = new QProcess(this);
  connect(mProcess, &QProcess::readyReadStandardOutput, this, &WbBuildEditor::readStdout);
  connect(mProcess, &QProcess::readyReadStandardError, this, &WbBuildEditor::readStderr);
  void (QProcess::*processFinished)(int, QProcess::ExitStatus) = &QProcess::finished;
  connect(mProcess, processFinished, this, &WbBuildEditor::processFinished);

  // we should clear environment variables which are used by the Makefile system as they may conflict with it
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.remove("TERM");       // may cause problems to mingw/gcc if defined by the user
  env.remove("INCLUDE");    // may cause gcc to fail because of a wrong path syntax (Windows style with backslashes and spaces)
  env.remove("LIBRARIES");  // same as for INCLUDE
  env.remove("CFLAGS");     // safety
  env.remove("C_SOURCES");
  env.remove("CXX_SOURCES");
  env.remove("USE_C_API");
  mProcess->setProcessEnvironment(env);

  // disable buttons
  updateBuildButtons();

  // launch external make and wait at most 5 sec
  mProcess->setWorkingDirectory(compilePath);
  mProcess->start(command, arguments);
  bool started = mProcess->waitForStarted(5000);

  // check that program is available
  if (!started) {
#ifdef _WIN32
    WbLog::appendStderr(tr("Installation problem: could not start '%1'.\n").arg(command), WbLog::COMPILATION);
#else
    WbLog::appendStderr(tr("The '%1' command appears not to be available on your system.\n").arg(command), WbLog::COMPILATION);
#endif
    cleanupProcess();
  }
}

QStringList WbBuildEditor::getJavaCommandLine(const QString &target) const {
  QDir controllerDir = compileDir();
  QString controllerPath = controllerDir.absolutePath();
  QString controllerName = QFileInfo(controllerPath).baseName();
  QStringList commandLine;

  if (target == "clean")
    commandLine << "rm"
                << "-fr" << controllerDir.entryList(QStringList("*.class"), QDir::Files) << controllerName + ".jar";
  else if (target == "jar")  // create JAR with .class files and all the subfolders in the controller folder
    commandLine << "jar"
                << "cf " << controllerName + ".jar " << controllerDir.entryList(QStringList("*.class"), QDir::Files);
  else if (target == "") {  // build, compile all .java files in the controller folder
#ifdef _WIN32
    const QString separator = ";";
#else
    const QString separator = ":";
#endif
    QString classpath = QDir::toNativeSeparators(WbStandardPaths::controllerLibPath() + "java/Controller.jar") + separator;
    const QString CLASSPATH = qgetenv("CLASSPATH");
    if (!CLASSPATH.isEmpty())
      classpath += CLASSPATH + separator;
    classpath += ".";

    commandLine << "javac"
                << "-Xlint"
                << "-classpath" << classpath << controllerDir.entryList(QStringList("*.java"), QDir::Files);
  }
  return commandLine;
}

void WbBuildEditor::computeTargetFile() {
  mTargetFile.clear();

  QString compilePath = compileDir().absolutePath();
  QString targetTmp;

  int lastIndexOfSlash = compilePath.lastIndexOf('/');
  if (lastIndexOfSlash >= 0)
    targetTmp = compilePath + '/' + compilePath.mid(lastIndexOfSlash + 1);

  QStringListIterator extensionsIterator(WbLanguage::exectuableExtensions());
  while (extensionsIterator.hasNext()) {
    const QString &extension = extensionsIterator.next();

    QString targetWithExtension;
    if (WbLanguage::isUnixLibraryExtension(extension))
      targetWithExtension = compilePath + "/lib" + compilePath.mid(lastIndexOfSlash + 1) + extension;
    else
      targetWithExtension = targetTmp + extension;

    if (QFile::exists(targetWithExtension))
      mTargetFile = targetWithExtension;
  }
}

void WbBuildEditor::updateTargetModificationTime() {
  // invalidate time
  mTargetModificationTimeBeforeMake = QDateTime();

  if (!mTargetFile.isEmpty()) {
    QString targetFile = mTargetFile;

    // use the target file instead if existing
    int lastSeparator = targetFile.lastIndexOf('/');
    if (lastSeparator != -1) {
      QString potentialTargetFile = targetFile;
      potentialTargetFile.replace(lastSeparator, 1, "/build/release/");
      if (QFile::exists(potentialTargetFile))
        targetFile = potentialTargetFile;
    }

    QFileInfo targetFileInfo(targetFile);
    if (targetFileInfo.exists())
      mTargetModificationTimeBeforeMake = targetFileInfo.lastModified();
  }
}

void WbBuildEditor::jumpToError(QString fileName, int line, int column) {
  // TODO: possibly adapt for nested source directories
  if (currentBuffer() && QDir::isRelativePath(fileName))
    fileName = compileDir().absoluteFilePath(fileName);

  if (openFile(fileName)) {
    WbTextBuffer *buffer = currentBuffer();
    if (line != -1)
      buffer->markError(line, column);
  }
}

void WbBuildEditor::unmarkError() {
  for (int i = 0; i < bufferCount(); i++) {
    WbTextBuffer *buf = buffer(i);
    buf->unmarkError();
  }
}
