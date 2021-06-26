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

#ifndef WB_PROTO_MODEL_HPP
#define WB_PROTO_MODEL_HPP

//
// Description: a class for managing PROTO definition stored in .proto files
//

class WbField;
class WbFieldModel;
class WbNode;
class WbTokenizer;

#include "WbVersion.hpp"

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVector>

class WbProtoModel : public QObject {
  Q_OBJECT

public:
  // create
  WbProtoModel(WbTokenizer *tokenizer, const QString &worldPath, const QString &fileName = QString(),
               QStringList baseTypeList = QStringList());

  // node name, e.g. "NaoV3R", "EPuck" ...
  const QString &name() const { return mName; }

  // file version found in file header
  const WbVersion &fileVersion() const { return mFileVersion; }

  // info comments (#) found at the beginning of the .proto file
  const QString &info() const { return mInfo; }

  // list of tags found at the beginning of the .proto file (after the 'tags:' keyword)
  const QStringList &tags() const { return mTags; }

  // license found at the beginning of the .proto file (after the 'license:' string)
  const QString &license() const { return mLicense; }

  // license found at the beginning of the .proto file (after the 'license url:' string)
  const QString &licenseUrl() const { return mLicenseUrl; }

  // documentation url the beginning of the .proto file (after the 'documentation url:' string)
  const QString &documentationUrl() const { return mDocumentationUrl; }
  // return the documentation book and page for this PROTO model:
  // - robot or object page matching the node name
  // - if !skipProtoTag: data specified in PROTO 'documentation url' tag
  // - empty if none of the previous searches are successful
  QStringList documentationBookAndPage(bool isRobot, bool skipProtoTag) const;

  // field models
  WbFieldModel *findFieldModel(const QString &fieldName) const;
  const QList<WbFieldModel *> &fieldModels() const { return mFieldModels; }

  // full .proto file name
  const QString &fileName() const { return mFileName; }

  // path of the folder that contains this .proto file e.g. "/home/yvan/develop/webots/projects/robots/softbank/nao/protos/"
  const QString &path() const { return mPath; }

  // path of the project that contains this .proto file
  const QString projectPath() const;

  // template
  bool isTemplate() const { return mTemplate; }
  bool isDeterministic() const { return mIsDeterministic; }

  // proto derived from another proto
  bool isDerived() const { return mDerived; }

  const QString &ancestorProtoName() const { return mAncestorProtoName; }

  const QString &baseType() const { return mBaseType; }

  const QString &slotType() const { return mSlotType; }

  QStringList parameterNames() const;

  // set nested proto property based on base proto
  void setIsTemplate(bool value);

  // add/remove a reference to this proto model from a proto instance
  // when the reference count reaches zero (in unref()) the proto model gets deleted
  // the optional argument defines if it is called from the creation of a proto instance
  // (in that case if it is a derived proto the unref will automatically be called for the ancestor)
  void ref(bool isFromProtoInstanceCreation = false);
  void unref();

  // delete this proto model
  // reference count has to be zero
  void destroy();

  WbNode *generateRoot(const QVector<WbField *> &parameters, const QString &worldPath, int uniqueId = -1);

private:
  // cppcheck-suppress unknownMacro
  Q_DISABLE_COPY(WbProtoModel)

  QMap<QString, QString> mDeterministicContentMap;
  QString mContent;

  bool mTemplate;
  WbVersion mFileVersion;
  QString mName;
  QString mInfo;
  bool mIsDeterministic;  // i.e doesn't have the 'nonDeterministic' tag
  QList<WbFieldModel *> mFieldModels;
  QString mFileName;  // .proto file name
  QString mPath;      // path of .proto file
  int mRefCount;
  int mAncestorRefCount;
  int mContentStartingLine;
  bool mDerived;               // proto with proto base type
  QString mAncestorProtoName;  // name of the direct proto ancestor
  WbProtoModel *mAncestorProtoModel;
  QString mBaseType;
  QString mSlotType;
  QString mLicense;
  QString mLicenseUrl;
  QString mDocumentationUrl;
  QStringList mTags;

  ~WbProtoModel();  // called from unref()
  void verifyAliasing(WbNode *root, WbTokenizer *tokenizer) const;
  void verifyNodeAliasing(WbNode *node, WbFieldModel *param, WbTokenizer *tokenizer, bool searchInParameters, bool &ok) const;
  bool checkIfDocumentationPageExist(const QString &page) const;
};

#endif
