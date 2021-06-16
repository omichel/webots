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

#include "WbField.hpp"

#include "WbFieldModel.hpp"
#include "WbLog.hpp"
#include "WbMFBool.hpp"
#include "WbMFColor.hpp"
#include "WbMFDouble.hpp"
#include "WbMFInt.hpp"
#include "WbMFNode.hpp"
#include "WbMFRotation.hpp"
#include "WbMFString.hpp"
#include "WbMFVector2.hpp"
#include "WbMFVector3.hpp"
#include "WbMultipleValue.hpp"
#include "WbNode.hpp"
#include "WbSFDouble.hpp"
#include "WbSFNode.hpp"
#include "WbSFRotation.hpp"
#include "WbSFVector2.hpp"
#include "WbSFVector3.hpp"
#include "WbTokenizer.hpp"
#include "WbValue.hpp"
#include "WbVrmlWriter.hpp"

#include <cassert>
#include <iostream>

// creates with the default value
WbField::WbField(const WbFieldModel *model, WbNode *parentNode) :
  mModel(model),
  mValue(model->defaultValue()->clone()),
  mWasRead(false),
  mParameter(NULL),
  mIsTemplateRegenerator(model->isTemplateRegenerator()),
  mParentNode(parentNode) {
  mModel->ref();
  if (hasRestrictedValues())
    connect(mValue, &WbValue::changed, this, &WbField::checkValueIsAccepted, Qt::UniqueConnection);
  connect(mValue, &WbValue::changed, this, &WbField::valueChanged, Qt::UniqueConnection);
  connect(mValue, &WbValue::changedByOde, this, &WbField::valueChangedByOde, Qt::UniqueConnection);
  connect(mValue, &WbValue::changedByWebots, this, &WbField::valueChangedByWebots, Qt::UniqueConnection);
}

WbField::WbField(const WbField &other, WbNode *parentNode) :
  mModel(other.mModel),
  mValue(other.value()->clone()),
  mWasRead(false),
  mParameter(NULL),
  mAlias(other.mAlias),
  mIsTemplateRegenerator(other.mIsTemplateRegenerator),
  mParentNode(parentNode) {
  mModel->ref();
  if (hasRestrictedValues())
    connect(mValue, &WbValue::changed, this, &WbField::checkValueIsAccepted, Qt::UniqueConnection);
  connect(mValue, &WbValue::changed, this, &WbField::valueChanged, Qt::UniqueConnection);
  connect(mValue, &WbValue::changedByOde, this, &WbField::valueChangedByOde, Qt::UniqueConnection);
  connect(mValue, &WbValue::changedByWebots, this, &WbField::valueChangedByWebots, Qt::UniqueConnection);
}

WbField::~WbField() {
  foreach (WbField *const field, mInternalFields)
    field->mParameter = NULL;
  delete mValue;
  mModel->unref();
}

void WbField::listenToValueSizeChanges() const {
  if (isSingle())
    return;
  const WbMultipleValue *mf = static_cast<WbMultipleValue *>(mValue);
  connect(mf, &WbMultipleValue::itemRemoved, this, &WbField::valueSizeChanged, Qt::UniqueConnection);
  connect(mf, &WbMultipleValue::itemInserted, this, &WbField::valueSizeChanged, Qt::UniqueConnection);
}

const QString &WbField::name() const {
  return mModel->name();
}

bool WbField::isVrml() const {
  return mModel->isVrml();
}

bool WbField::isDeprecated() const {
  return mModel->isDeprecated();
}

void WbField::readValue(WbTokenizer *tokenizer, const QString &worldPath) {
  if (mWasRead)
    tokenizer->reportError(tr("Duplicate field value."));

  mValue->read(tokenizer, worldPath);
  mWasRead = true;
  if (hasRestrictedValues())
    checkValueIsAccepted();
}

void WbField::write(WbVrmlWriter &writer) const {
  if (isDefault())
    return;
  if (writer.isX3d())
    writer << " ";
  const bool notAString = type() != WB_SF_STRING;
  writer.writeFieldStart(name(), notAString);
  mValue->write(writer);
  writer.writeFieldEnd(notAString);
}

const WbValue *WbField::defaultValue() const {
  return mModel->defaultValue();
}

bool WbField::isDefault() const {
  return mValue->equals(mModel->defaultValue());
}

void WbField::reset(bool blockValueSignals) {
  if (singleType() == WB_SF_NODE) {
    // clear field and set value to NULL or []
    // but new default node instances have to be created separately
    WbSFNode *sfnode = dynamic_cast<WbSFNode *>(mValue);
    WbMFNode *mfnode = dynamic_cast<WbMFNode *>(mValue);
    if (sfnode) {
      if (blockValueSignals) {
        sfnode->blockSignals(true);
        sfnode->removeValue();
        sfnode->blockSignals(false);
      } else
        sfnode->removeValue();
    } else if (mfnode) {
      // remove all children
      const int n = mfnode->size() - 1;
      if (blockValueSignals)
        mfnode->blockSignals(true);
      for (int i = n; i >= 0; --i)
        mfnode->removeItem(i);
      if (blockValueSignals)
        mfnode->blockSignals(false);
    }
    return;
  }

  setValue(mModel->defaultValue());
}

void WbField::checkValueIsAccepted() {
  int refusedIndex;
  if (!mModel->isValueAccepted(mValue, &refusedIndex)) {
    QString acceptedValuesList = "";
    foreach (const WbVariant acceptedValue, mModel->acceptedValues())
      acceptedValuesList += acceptedValue.toStringRepresentation() + ", ";
    acceptedValuesList.chop(2);
    QString error;
    if (isSingle()) {
      error = tr("Invalid '%1' changed to %2. The value should be in the list: {%3}.")
                .arg(name())
                .arg(defaultValue()->toString())
                .arg(acceptedValuesList);
      reset(true);
    } else {
      WbMultipleValue *mvalue = dynamic_cast<WbMultipleValue *>(mValue);
      assert(mvalue);
      error = tr("Invalid '%1' removed from '%2' field. The values should be in the list: {%3}.")
                .arg(mvalue->itemToString(refusedIndex))
                .arg(name())
                .arg(acceptedValuesList);
      mvalue->removeItem(refusedIndex);
    }
    if (parentNode())
      parentNode()->parsingWarn(error);
    else
      WbLog::warning(error, false, WbLog::PARSING);
  }
}

void WbField::setValue(const WbValue *otherValue) {
  WbMultipleValue *mvalue = dynamic_cast<WbMultipleValue *>(mValue);
  if (mvalue) {
    // remove all children
    const int n = mvalue->size() - 1;
    for (int i = n; i >= 0; --i)
      mvalue->removeItem(i);

    // add default children
    switch (mvalue->type()) {
      case WB_MF_NODE: {
        const WbMFNode *const otherField = dynamic_cast<const WbMFNode *>(otherValue);
        WbMFNode *const actualField = dynamic_cast<WbMFNode *>(mvalue);
        WbMFIterator<WbMFNode, WbNode *> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      case WB_MF_VEC2F: {
        const WbMFVector2 *const otherField = dynamic_cast<const WbMFVector2 *>(otherValue);
        WbMFVector2 *const actualField = dynamic_cast<WbMFVector2 *>(mvalue);
        WbMFIterator<WbMFVector2, WbVector2> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      case WB_MF_VEC3F: {
        const WbMFVector3 *const otherField = dynamic_cast<const WbMFVector3 *>(otherValue);
        WbMFVector3 *const actualField = dynamic_cast<WbMFVector3 *>(mvalue);
        WbMFIterator<WbMFVector3, WbVector3> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      case WB_MF_ROTATION: {
        const WbMFRotation *const otherField = dynamic_cast<const WbMFRotation *>(otherValue);
        WbMFRotation *const actualField = dynamic_cast<WbMFRotation *>(mvalue);
        WbMFIterator<WbMFRotation, WbRotation> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      case WB_MF_COLOR: {
        const WbMFColor *const otherField = dynamic_cast<const WbMFColor *>(otherValue);
        WbMFColor *const actualField = dynamic_cast<WbMFColor *>(mvalue);
        WbMFIterator<WbMFColor, WbRgb> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      case WB_MF_STRING: {
        const WbMFString *const otherField = dynamic_cast<const WbMFString *>(otherValue);
        WbMFString *const actualField = dynamic_cast<WbMFString *>(mvalue);
        WbMFIterator<WbMFString, QString> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      case WB_MF_BOOL: {
        const WbMFBool *const otherField = dynamic_cast<const WbMFBool *>(otherValue);
        WbMFBool *const actualField = dynamic_cast<WbMFBool *>(mvalue);
        WbMFIterator<WbMFBool, bool> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      case WB_MF_INT32: {
        const WbMFInt *const otherField = dynamic_cast<const WbMFInt *>(otherValue);
        WbMFInt *const actualField = dynamic_cast<WbMFInt *>(mvalue);
        WbMFIterator<WbMFInt, int> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      case WB_MF_FLOAT: {
        const WbMFDouble *const otherField = dynamic_cast<const WbMFDouble *>(otherValue);
        WbMFDouble *const actualField = dynamic_cast<WbMFDouble *>(mvalue);
        WbMFIterator<WbMFDouble, double> it(otherField);
        while (it.hasNext())
          actualField->addItem(it.next());
        break;
      }
      default:
        break;
    }

  } else
    // single value
    mValue->copyFrom(otherValue);
}

QString WbField::toString(WbPrecision::Level level) const {
  return QString("%1 %2").arg(name(), mValue->toString(level));
}

WbFieldType WbField::type() const {
  return mValue->type();
}

WbFieldType WbField::singleType() const {
  return mValue->singleType();
}

bool WbField::isMultiple() const {
  return mModel->isMultiple();
}

bool WbField::isSingle() const {
  return mModel->isSingle();
}

bool WbField::isHidden() const {
  return mModel->isHiddenField();
}

bool WbField::isHiddenParameter() const {
  return mModel->isHiddenParameter();
}

// redirect this node field to a proto parameter
void WbField::redirectTo(WbField *parameter) {
  // qDebug() << "redirectTo: " << this << " " << name() << " -> " << parameter << " " << parameter->name();

  if (this == parameter || parameter->mInternalFields.contains(this)) {
    // skip self and duplicated redirection
    return;
  }

  // propagate top -> down the template regenerator flag
  if (isTemplateRegenerator())
    parameter->setTemplateRegenerator(true);

  mParameter = parameter;
  mParameter->mInternalFields.append(this);
  connect(this, &QObject::destroyed, mParameter, &WbField::removeInternalField);

  // copy parameter value to field
  mValue->copyFrom(mParameter->value());

  WbMFNode *mfnode = dynamic_cast<WbMFNode *>(mParameter->value());
  if (mfnode) {
    connect(mfnode, &WbMFNode::itemInserted, mParameter, &WbField::parameterNodeInserted, Qt::UniqueConnection);
    connect(mfnode, &WbMFNode::itemRemoved, mParameter, &WbField::parameterNodeRemoved, Qt::UniqueConnection);

  } else {
    // make sure the field gets updated when the parameter changes, e.g. by Scene Tree or Supervisor, etc.
    connect(mParameter, &WbField::valueChanged, mParameter, &WbField::parameterChanged, Qt::UniqueConnection);
    connect(mParameter->value(), &WbValue::changedByUser, this->value(), &WbValue::changedByUser, Qt::UniqueConnection);

    // In some case Webots modifies the fields directly and not the proto parameters, e.g. changing "translation" or
    // "rotation" fields with the mouse. In these cases we need to propagate the change back to the proto parameters, e.g. in
    // order to update the Scene Tree
    if (!isHidden())
      connect(this, &WbField::valueChanged, mParameter, &WbField::fieldChanged);
  }

  // ODE updates
  const QString &fieldName = name();
  if (fieldName == "translation") {
    connect(static_cast<WbSFVector3 *>(mValue), &WbSFVector3::changedByOde, mParameter, &WbField::fieldChangedByOde);
    connect(static_cast<WbSFVector2 *>(mValue), &WbSFVector2::changedByWebots, mParameter, &WbField::fieldChangedByOde);
  } else if (fieldName == "rotation")
    connect(static_cast<WbSFRotation *>(mValue), &WbSFRotation::changedByOde, mParameter, &WbField::fieldChangedByOde);
  else if (fieldName == "position")
    connect(static_cast<WbSFDouble *>(mValue), &WbSFDouble::changedByOde, mParameter, &WbField::fieldChangedByOde);
}

void WbField::removeInternalField(QObject *field) {
  mInternalFields.removeAll(static_cast<WbField *>(field));
}

// propagate change in proto parameter to a node field
void WbField::parameterChanged() {
  WbSFNode *sfnode = dynamic_cast<WbSFNode *>(mValue);
  if (sfnode && sfnode->value()) {
    WbNode *node = sfnode->value();

    WbNode *instance = NULL;
    foreach (WbField *internalField, mInternalFields) {
      WbNode::setGlobalParentNode(internalField->parentNode(), true);
      instance = node->cloneAndReferenceProtoInstance();
      sfnode = dynamic_cast<WbSFNode *>(internalField->value());
      sfnode->setValue(instance);
    }

  } else {
    foreach (WbField *const field, mInternalFields)
      field->copyValueFrom(this);
  }
}

// propagate node insertion to internal fields of parameter
void WbField::parameterNodeInserted(int index) {
  WbMFNode *mfnode = dynamic_cast<WbMFNode *>(mValue);
  WbNode *const node = mfnode->item(index);

  WbNode *instance = NULL;
  foreach (WbField *internalField, mInternalFields) {
    WbNode::setGlobalParentNode(internalField->parentNode(), true);
    instance = node->cloneAndReferenceProtoInstance();
    mfnode = dynamic_cast<WbMFNode *>(internalField->value());
    mfnode->insertItem(index, instance);
  }
}

// propagate node remotion to internal fields of parameter
void WbField::parameterNodeRemoved(int index) {
  WbMFNode *mfnode = NULL;
  foreach (WbField *const field, mInternalFields) {
    mfnode = dynamic_cast<WbMFNode *>(field->value());
    mfnode->removeItem(index);
  }
}

// propagate change in a node field to a proto parameter
void WbField::fieldChanged() {
  // do not propagate a node change back to the proto parameter otherwise we would loop infinitly
  // because the break condition (node == node) is not fully functional
  if (singleType() != WB_SF_NODE)
    copyValueFrom(static_cast<WbField *>(sender()));
}

void WbField::fieldChangedByOde() {
  // do not propagate a node change back to the proto parameter otherwise we would loop infinitly
  // because the break condition (node == node) is not fully functional
  mValue->blockSignals(true);
  mValue->copyFrom(static_cast<WbValue *>(sender()));
  mValue->blockSignals(false);
  mValue->emitChangedByOde();
}

void WbField::disconnectField() {
  if (mParameter) {
    if (mValue) {
      const QString &fieldName = name();
      if (fieldName == "translation") {
        disconnect(static_cast<WbSFVector3 *>(mValue), &WbSFVector3::changedByOde, mParameter, &WbField::fieldChangedByOde);
        disconnect(static_cast<WbSFVector2 *>(mValue), &WbSFVector2::changedByWebots, mParameter, &WbField::fieldChangedByOde);
      } else if (fieldName == "rotation")
        disconnect(static_cast<WbSFRotation *>(mValue), &WbSFRotation::changedByOde, mParameter, &WbField::fieldChangedByOde);
      else if (fieldName == "position")
        disconnect(static_cast<WbSFDouble *>(mValue), &WbSFDouble::changedByOde, mParameter, &WbField::fieldChangedByOde);

      disconnect(mValue, &WbValue::changed, this, &WbField::valueChanged);
    }

    disconnect(mParameter, &WbField::valueChanged, mParameter, &WbField::parameterChanged);
    disconnect(mParameter->value(), &WbValue::changedByUser, this->value(), &WbValue::changedByUser);

    disconnect(this, &WbField::valueChanged, mParameter, &WbField::fieldChanged);
  }
}

void WbField::copyValueFrom(const WbField *other) {
  assert(other);
  mValue->copyFrom(other->mValue);
}

void WbField::defHasChanged() {
  mValue->defHasChanged();
}
