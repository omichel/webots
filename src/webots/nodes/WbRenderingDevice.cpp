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

#include "WbRenderingDevice.hpp"

#include "WbField.hpp"
#include "WbFieldChecker.hpp"
#include "WbPreferences.hpp"
#include "WbSFInt.hpp"
#include "WbSFVector2.hpp"
#include "WbWorld.hpp"
#include "WbWrenRenderingContext.hpp"
#include "WbWrenTextureOverlay.hpp"

#ifdef _WIN32
#include "WbVirtualRealityHeadset.hpp"
#endif

QList<WbRenderingDevice *> WbRenderingDevice::cRenderingDevices;

void WbRenderingDevice::init() {
  // initialized some stuff
  mOverlay = NULL;
  mSetupWidth = -1;
  mSetupHeight = -1;
  mHasBeenSetup = false;
  mIsExternalWindowEnabled = false;

  // Fields initialization
  mWidth = findSFInt("width");
  mHeight = findSFInt("height");

  // backward compatibility of the deprecated fields
  mWindowPositionField = findSFVector2("windowPosition");
  if (mWindowPositionField && mWindowPositionField->value() != WbVector2()) {
    parsingWarn(tr("'windowPosition' is deprecated.") + "\n" +
                tr("The position of the overlay will be automatically stored after moving it from the 3D view."));
  }
  mPixelSizeField = findSFDouble("pixelSize");
  if (mPixelSizeField && mPixelSizeField->value() != 1.0) {
    parsingWarn(tr("'pixelSize' is deprecated.") + "\n" +
                tr("The size of the overlay will be automatically stored after resizing it from the 3D view."));
  }
}

WbRenderingDevice::WbRenderingDevice(const QString &modelName, WbTokenizer *tokenizer) : WbSolidDevice(modelName, tokenizer) {
  init();
}

WbRenderingDevice::WbRenderingDevice(const WbRenderingDevice &other) : WbSolidDevice(other) {
  init();
}

WbRenderingDevice::WbRenderingDevice(const WbNode &other) : WbSolidDevice(other) {
  init();
}

WbRenderingDevice::~WbRenderingDevice() {
  delete mOverlay;
  cRenderingDevices.removeAll(this);
}

void WbRenderingDevice::preFinalize() {
  WbSolidDevice::preFinalize();

  cRenderingDevices << this;

  updateWidth();
  updateHeight();
}

void WbRenderingDevice::postFinalize() {
  WbSolidDevice::postFinalize();

  if (mWidth)
    connect(mWidth, &WbSFInt::changed, this, &WbRenderingDevice::updateWidth);
  if (mHeight)
    connect(mHeight, &WbSFInt::changed, this, &WbRenderingDevice::updateHeight);
}

int WbRenderingDevice::width() const {
  if (mHasBeenSetup)
    return mSetupWidth;
  else
    return mWidth->value();
}

int WbRenderingDevice::height() const {
  if (mHasBeenSetup)
    return mSetupHeight;
  else
    return mHeight->value();
}

void WbRenderingDevice::setup() {
  if (mHasBeenSetup)
    return;
  if (mWidth)
    mSetupWidth = mWidth->value();
  if (mHeight)
    mSetupHeight = mHeight->value();
  mHasBeenSetup = true;
}

void WbRenderingDevice::updateWidth() {
  if (mWidth && mWidth->value() < 1) {
    parsingWarn(tr("Invalid 'width': changed to 1."));
    mWidth->setValue(1);
    return;
  }

  // warn in case of width modification after the setup
  if (mHasBeenSetup)
    warn(tr("'width' has been modified. This modification will be taken into account after saving and reloading the world."));
}

void WbRenderingDevice::updateHeight() {
  if (mHeight && mHeight->value() < 1) {
    parsingWarn(tr("Invalid 'height': changed to 1."));
    mHeight->setValue(1);
    return;
  }

  // warn in case of height modification after the setup
  if (mHasBeenSetup)
    warn(tr("'height' has been modified. This modification will be taken into account after saving and reloading the world."));
}

// backward compatibility
void WbRenderingDevice::applyWorldSettings() {
  if (mPixelSizeField) {
    mOverlay->resize(mPixelSizeField->value());
    mPixelSizeField = NULL;
  }
  if (mWindowPositionField) {
    WbVector2 position = mWindowPositionField->value();
    mOverlay->updatePercentagePosition(position.x(), position.y());
    mWindowPositionField = NULL;
  }
}

void WbRenderingDevice::setPixelSize(double pixelSize) {
  if (mOverlay) {
    bool success = mOverlay->resize(pixelSize);
    if (!success)
      emit overlayVisibilityChanged(false);
  }
}

void WbRenderingDevice::moveWindow(int dx, int dy) {
  if (mOverlay)
    mOverlay->translateInPixels(dx, dy);
}

double WbRenderingDevice::pixelSize() const {
  if (mOverlay)
    return mOverlay->pixelSize();
  else if (mPixelSizeField)
    // backward compatibility
    return mPixelSizeField->value();
  return 1.0;
}

WbRenderingDevice *WbRenderingDevice::fromMousePosition(int x, int y) {
  int iMax = -1;
  int maxZorder = -1;
  int size = cRenderingDevices.size();

  for (int i = 0; i < size; i++) {
    const WbWrenTextureOverlay *overlay = cRenderingDevices.at(i)->overlay();
    if (overlay && overlay->isVisible()) {
      int currentZorder = overlay->zOrder();
      if (overlay->isInside(x, y) && currentZorder > maxZorder) {
        iMax = i;
        maxZorder = currentZorder;
      }
    }
  }

  if (iMax < 0 || iMax >= size)
    return NULL;

  return cRenderingDevices.at(iMax);
}

void WbRenderingDevice::toggleOverlayVisibility(bool enabled, bool emitSignal) {
  if (!mOverlay)
    return;

  if (!enabled) {
    // hide overlay
    if (mIsExternalWindowEnabled)
      emit closeWindow();
    else
      mOverlay->setVisible(enabled, areOverlaysEnabled());
  } else if (!mIsExternalWindowEnabled) {
    // show overlay
    mOverlay->setVisible(enabled, areOverlaysEnabled());
  }

  if (emitSignal)
    emit overlayVisibilityChanged(enabled);
}

bool WbRenderingDevice::isOverlayEnabled() const {
  return (mOverlay && mOverlay->isEnabled()) || mIsExternalWindowEnabled;
}

QStringList WbRenderingDevice::perspective() const {
  QStringList perspective;
  if (mOverlay)
    perspective = mOverlay->perspective();
  return perspective;
}

void WbRenderingDevice::restorePerspective(QStringList &perspective) {
  if (mOverlay) {
    mOverlay->restorePerspective(perspective, areOverlaysEnabled());
    if (!perspective.isEmpty())
      emit restoreWindowPerspective(this, perspective);
  }
}

int WbRenderingDevice::textureGLId() const {
  assert(mOverlay);
  if (mOverlay)
    return mOverlay->textureGLId();
  return 0;
}

int WbRenderingDevice::backgroundTextureGLId() const {
  assert(mOverlay);
  if (mOverlay)
    return mOverlay->backgroundTextureGLId();
  return 0;
}

int WbRenderingDevice::maskTextureGLId() const {
  assert(mOverlay);
  if (mOverlay)
    return mOverlay->maskTextureGLId();
  return 0;
}

int WbRenderingDevice::foregroundTextureGLId() const {
  assert(mOverlay);
  if (mOverlay)
    return mOverlay->foregroundTextureGLId();
  return 0;
}

void WbRenderingDevice::enableExternalWindow(bool enabled) {
  mIsExternalWindowEnabled = enabled;
  mOverlay->setVisible(!enabled, areOverlaysEnabled());
  emit overlayStatusChanged(!mIsExternalWindowEnabled);
  if (isPostFinalizedCalled() && !isBeingDeleted())
    WbWrenRenderingContext::instance()->requestView3dRefresh();
}

bool WbRenderingDevice::areOverlaysEnabled() const {
#ifdef _WIN32
  if (WbVirtualRealityHeadset::isInUse())
    return false;
#endif
  // cppcheck-suppress knownConditionTrueFalse
  if (nodeType() == WB_NODE_CAMERA)
    return !WbPreferences::instance()->value("View3d/hideAllCameraOverlays").toBool();
  // cppcheck-suppress knownConditionTrueFalse
  if (nodeType() == WB_NODE_RANGE_FINDER)
    return !WbPreferences::instance()->value("View3d/hideAllRangeFinderOverlays").toBool();
  return !WbPreferences::instance()->value("View3d/hideAllDisplayOverlays").toBool();
}
