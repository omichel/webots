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

#ifndef WB_VRML_WRITER_HPP
#define WB_VRML_WRITER_HPP

//
// Description: a text stream specialized for writing indented VRML or X3D
//

#include <QtCore/QTextStream>

#include "WbVector3.hpp"

class QIODevice;
class WbNode;

class WbVrmlWriter : public QTextStream {
public:
  WbVrmlWriter(QIODevice *device, const QString &fileName);
  WbVrmlWriter(QString *target, const QString &fileName);
  virtual ~WbVrmlWriter();

  bool isVrml() const { return mVrmlType == VRML; }
  bool isX3d() const { return mVrmlType == X3D; }
  bool isProto() const { return mVrmlType == PROTO; }
  bool isUrdf() const { return mVrmlType == URDF; }
  bool isWebots() const { return mVrmlType == VRML_SIM || mVrmlType == VRML_OBJ || mVrmlType == PROTO; }
  bool isWritingToFile() const { return mIsWritingToFile; }
  QString path() const;
  QString relativeTexturesPath() const { return "textures/"; }
  QHash<QString, QString> texturesList() const { return mTexturesList; }
  void addTextureToList(const QString &url, const QString &fileName) { mTexturesList[url] = fileName; }

  void writeLiteralString(const QString &string);
  void writeMFStart();
  void writeMFSeparator(bool first, bool smallSeparator);
  void writeMFEnd(bool empty);
  void writeFieldStart(const QString &name, bool x3dQuote);
  void writeFieldEnd(bool x3dQuote);

  WbVector3 jointOffset() const { return mJointOffset; }
  void setJointOffset(const WbVector3 &offset) { mJointOffset = offset; }

  // change current indentation
  void increaseIndent() { mIndent++; }
  void decreaseIndent() { mIndent--; }

  // write current indentation
  void indent();

  // write .wrl, .wbt, .wbo, .x3d or .urdf header and footer based on VrmlType
  void writeHeader(const QString &title);
  void writeFooter(const QStringList *info = NULL);

  void setRootNode(WbNode *node) { mRootNode = node; }
  WbNode *rootNode() const { return mRootNode; }

  QMap<uint64_t, QString> &indexedFaceSetDefMap() { return mIndexedFaceSetDefMap; }

private:
  void setVrmlType();

  enum VrmlType { VRML, VRML_SIM, VRML_OBJ, X3D, PROTO, URDF };
  QString mFileName;
  VrmlType mVrmlType;
  int mIndent;
  QMap<uint64_t, QString> mIndexedFaceSetDefMap;
  QHash<QString, QString> mTexturesList;  // this hash represents the list of textures used and their associated filepath
  WbNode *mRootNode;
  bool mIsWritingToFile;
  WbVector3 mJointOffset;
};

#endif
