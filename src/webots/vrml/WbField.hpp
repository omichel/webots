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

#ifndef WB_FIELD_HPP
#define WB_FIELD_HPP

//
// Description: a node's field
//   Each field contains a WbValue
//

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include "../../../include/controller/c/webots/supervisor.h"

#include "WbFieldModel.hpp"
#include "WbPrecision.hpp"

class WbNode;  // circular dependency: needed by PROTO mechanism for easily retrieving the parent of internal fields
class WbTokenizer;
class WbVrmlWriter;
class WbValue;

class WbField : public QObject {
  Q_OBJECT

public:
  // create from a field model
  explicit WbField(const WbFieldModel *model, WbNode *parentNode = NULL);

  // create by copying another field
  explicit WbField(const WbField &other, WbNode *parentNode = NULL);
  virtual ~WbField();

  // the field's model
  const WbFieldModel *model() const { return mModel; }

  // default value
  const WbValue *defaultValue() const;
  virtual bool isDefault() const;
  // reset to default value
  // for SFNode and MFNode only sets value to NULL
  virtual void reset(bool blockValueSignals = false);

  // write in VRML format
  virtual void write(WbVrmlWriter &writer) const;
  bool isVrml() const;

  bool isDeprecated() const;

  // read field value
  void readValue(WbTokenizer *tokenizer, const QString &worldPath);

  // optional redirection to a proto parameter
  void setAlias(const QString &alias) { mAlias = alias; }
  const QString &alias() const { return mAlias; }
  void redirectTo(WbField *parameter);
  WbField *parameter() const { return mParameter; }
  const QList<WbField *> &internalFields() const { return mInternalFields; }
  bool isParameter() const { return mInternalFields.size() != 0; }

  void setParameter(WbField *parameter) { mParameter = parameter; }
  void clearInternalFields() { mInternalFields.clear(); }

  void setParentNode(WbNode *node) { mParentNode = node; }
  WbNode *parentNode() const { return mParentNode; }

  // template
  void setTemplateRegenerator(bool isRegenerator) { mIsTemplateRegenerator = isRegenerator; }
  bool isTemplateRegenerator() const { return mIsTemplateRegenerator; }

  void disconnectField();

  // the field's name
  const QString &name() const;

  // the field's value (this pointer is never NULL)
  WbValue *value() const { return mValue; }

  // set new value (types must match) in multiple steps to update view
  void setValue(const WbValue *otherValue);
  // copy value from another field (the types must be the same) in one step
  void copyValueFrom(const WbField *other);

  // create WREN and ODE objects in USE node fields when the corresponding DEF node has changed
  void defHasChanged();

  // convert to string for the GUI, e.g. "position 0.1 2 3"
  QString toString(WbPrecision::Level level) const;

  // the field's type (shortcuts for WbValue)
  WbFieldType type() const;        // e.g. WB_MF_NODE
  WbFieldType singleType() const;  // e.g. returns WB_SF_NODE for a WB_MF_NODE
  bool isMultiple() const;
  bool isSingle() const;
  bool isHidden() const;
  bool isHiddenParameter() const;

  // accepted values
  bool hasRestrictedValues() const { return mModel->hasRestrictedValues(); }
  const QList<WbVariant> acceptedValues() const { return mModel->acceptedValues(); }

  // enable forwarding signals when the size of MF fields changes
  void listenToValueSizeChanges() const;

signals:
  void valueChanged();
  void valueChangedByOde();
  void valueChangedByWebots();
  void valueSizeChanged();

protected:
private:
  WbField &operator=(const WbField &);  // non-copyable
  const WbFieldModel *mModel;           // field model (name and default value)
  WbValue *mValue;                      // field value (never NULL)
  bool mWasRead;                        // true if the value was read from file

  // for proto definition only
  WbField *mParameter;  // optional connection to a proto parameter
  QString mAlias;       // IS string
  bool mIsTemplateRegenerator;

  // for proto parameter only
  QList<WbField *> mInternalFields;  // internal fields towards which a parameter is redirecting its value

  // for internal fields only
  WbNode *mParentNode;

private slots:
  void parameterChanged();
  void parameterNodeInserted(int index);
  void parameterNodeRemoved(int index);
  void fieldChanged();
  void fieldChangedByOde();
  void removeInternalField(QObject *field);
  void checkValueIsAccepted();
};

#endif
