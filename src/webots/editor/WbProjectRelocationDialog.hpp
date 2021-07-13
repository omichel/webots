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

#ifndef WB_PROJECT_RELOCATION_DIALOG_HPP
#define WB_PROJECT_RELOCATION_DIALOG_HPP

//
// Description: dialog that helps the user to relocate a project outside the installation directory
//

#include <QtWidgets/QDialog>

class WbLineEdit;
class WbProject;

class QCheckBox;
class QDialogButtonBox;
class QLabel;
class QPlainTextEdit;
class QPushButton;

class WbProjectRelocationDialog : public QDialog {
  Q_OBJECT

public:
  // verify that the specified file/directory is located outside of Webots installation
  // otherwise start interactive dialog to relocate the project
  // return true if the file/directory can be written
  static bool validateLocation(QWidget *parent, QString &filename, bool isImportingVrml = false);

  // return the path of a modified external default PROTO project
  // return empty string if no external PROTO project was modified
  static const QString &relocatedExternalProtoProjectPath() { return mExternalProtoProjectPath; }

private slots:
  void selectDirectory();
  void targetEdited(const QString &text);
  void copy();

private:
  WbProject *mProject;
  QString mTargetPath;
  const QString &mRelativeFilename;
  const QString &mAbsoluteFilePath;
  WbLineEdit *mSourceEdit, *mTargetEdit;
  QCheckBox *mProtoCheckBox, *mPluginsCheckBox;
  QLabel *mFilesLabel, *mConclusionLabel;
  QPlainTextEdit *mStatusEdit;
  QPushButton *mSelectButton, *mCancelButton, *mCopyButton;
  QDialogButtonBox *mButtonBox;
  bool mIsProtoModified;
  bool mIsCompleteRelocation;

  // path to the projects folder of the modified PROTO resource located outside the current project path
  static QString mExternalProtoProjectPath;

  // create project relocation dialog
  explicit WbProjectRelocationDialog(WbProject *project, const QString &relativeFilename, const QString &absoluteFilePath,
                                     QWidget *parent = NULL);
  ~WbProjectRelocationDialog();

  void initCompleteRelocation();
  void initProtoSourceRelocation();

  // user's chosen target directory
  const QString &targetPath() const { return mTargetPath; }
  int copyProject();
  int copyExternalProtoProject();

  void setStatus(const QString &text, bool ok = true);
};

#endif
