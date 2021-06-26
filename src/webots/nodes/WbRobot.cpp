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

#include "WbRobot.hpp"

#include "WbAbstractCamera.hpp"
#include "WbBinaryIncubator.hpp"
#include "WbControllerPlugin.hpp"
#include "WbDisplay.hpp"
#include "WbJoint.hpp"
#include "WbJoystickInterface.hpp"
#include "WbKinematicDifferentialWheels.hpp"
#include "WbLinearMotor.hpp"
#include "WbLog.hpp"
#include "WbLogicalDevice.hpp"
#include "WbMFDouble.hpp"
#include "WbMFNode.hpp"
#include "WbMouse.hpp"
#include "WbNodeUtilities.hpp"
#include "WbPreferences.hpp"
#include "WbProject.hpp"
#include "WbPropeller.hpp"
#include "WbProtoList.hpp"
#include "WbProtoModel.hpp"
#include "WbRenderingDevice.hpp"
#include "WbResizeManipulator.hpp"
#include "WbSensor.hpp"
#include "WbSimulationState.hpp"
#include "WbSkin.hpp"
#include "WbSlot.hpp"
#include "WbSolidDevice.hpp"
#include "WbStandardPaths.hpp"
#include "WbSupervisorUtilities.hpp"
#include "WbTokenizer.hpp"
#include "WbTrack.hpp"
#include "WbWorld.hpp"
#include "WbWrenRenderingContext.hpp"

#include "../../../include/controller/c/webots/keyboard.h"
#include "../../../include/controller/c/webots/robot.h"
#include "../../../include/controller/c/webots/supervisor.h"
#include "../../controller/c/messages.h"

#include <QtCore/QDataStream>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

#include <limits>

static QHash<int, int> createSpecialKeys() {
  QHash<int, int> map;

  // init LUT for special keys
  map[Qt::SHIFT] = WB_KEYBOARD_SHIFT;
  map[Qt::CTRL] = WB_KEYBOARD_CONTROL;
  map[Qt::ALT] = WB_KEYBOARD_ALT;
  map[Qt::Key_Home] = WB_KEYBOARD_HOME;
  map[Qt::Key_End] = WB_KEYBOARD_END;
  map[Qt::Key_Left] = WB_KEYBOARD_LEFT;
  map[Qt::Key_Up] = WB_KEYBOARD_UP;
  map[Qt::Key_Right] = WB_KEYBOARD_RIGHT;
  map[Qt::Key_Down] = WB_KEYBOARD_DOWN;
  map[Qt::Key_PageUp] = WB_KEYBOARD_PAGEUP;
  map[Qt::Key_PageDown] = WB_KEYBOARD_PAGEDOWN;

  return map;
}

// LUT to convert Qt key values to Webots key values
static QHash<int, int> gSpecialKeys = createSpecialKeys();

enum { CURRENT_ENERGY, MAX_ENERGY, ENERGY_UPLOAD_SPEED };

int WbRobot::mapSpecialKey(int qtKey) {
  if (gSpecialKeys.contains(qtKey))
    return gSpecialKeys.value(qtKey);

  return 0;
}

void WbRobot::init() {
  mPowerOn = true;
  mBatterySensor = NULL;
  mBatteryLastValue = -1.0;
  mKeyboardSensor = NULL;
  mJoystickSensor = NULL;
  mJoystickInterface = NULL;
  mJoystickTimer = NULL;
  mUserInputEventTimer = NULL;
  mJoyStickLastValue = NULL;
  mMouse = NULL;
  mNextTag = 0;

  mNeedToWriteUrdf = false;
  mControllerStarted = false;
  mNeedToRestartController = false;
  mConfigureRequest = true;
  mSimulationModeRequested = false;
  mMonitoredUserInputEventTypes = -1;
  mNeedToWriteUserInputEventAnswer = false;
  mKeyboardHasChanged = false;

  mJoystickConfigureRequest = false;

  mPreviousTime = -1.0;
  mSupervisorUtilitiesNeedUpdate = false;
  mSupervisorUtilities = NULL;
  mKinematicDifferentialWheels = NULL;

  mMessageFromWwi = NULL;
  mShowWindowCalled = false;
  mShowWindowMessage = false;
  mWaitingForWindow = false;
  mUpdateWindowMessage = false;
  mDataNeedToWriteAnswer = false;
  mSupervisorNeedToWriteAnswer = false;
  mModelNeedToWriteAnswer = false;

  mPin = false;
  mPinTranslation = WbVector3();
  mPinRotation = WbRotation();

  mSupervisor = findSFBool("supervisor");
  mSynchronization = findSFBool("synchronization");
  mController = findSFString("controller");
  mControllerArgs = findMFString("controllerArgs");
  mCustomData = findSFString("customData");
  mBattery = findMFDouble("battery");
  mCpuConsumption = findSFDouble("cpuConsumption");
  mSelfCollision = findSFBool("selfCollision");
  mShowWindow = findSFBool("showWindow");
  mWindow = findSFString("window");
  mRemoteControl = findSFString("remoteControl");

  WbSFString *data = findSFString("data");
  if (data->value() != "") {  // Introduced in Webots 2018a
    parsingWarn("Deprecated 'data' field, please use the 'customData' field instead.");
    if (mCustomData->value() == "")
      mCustomData->setValue(data->value());
    data->setValue("");
  }

  mBatteryInitialValues[stateId()] = (mBattery->size() > CURRENT_ENERGY) ? mBattery->item(CURRENT_ENERGY) : -1.0;
}

WbRobot::WbRobot(WbTokenizer *tokenizer) : WbSolid("Robot", tokenizer) {
  init();
}

WbRobot::WbRobot(const WbRobot &other) : WbSolid(other) {
  init();
}

WbRobot::WbRobot(const WbNode &other) : WbSolid(other) {
  init();
}

WbRobot::WbRobot(const QString &modelName, WbTokenizer *tokenizer) : WbSolid(modelName, tokenizer) {
  init();
}

WbRobot::~WbRobot() {
  clearDevices();
  delete mBatterySensor;
  delete mKeyboardSensor;
  delete mJoystickSensor;
  delete mJoystickInterface;
  delete mJoystickTimer;
  delete mUserInputEventTimer;
  delete mJoyStickLastValue;
  if (mMouse)
    WbMouse::destroy(mMouse);
  delete mSupervisorUtilities;
  mPressedKeys.clear();
  WbWorld::instance()->removeRobotIfPresent(this);
}

void WbRobot::preFinalize() {
  WbSolid::preFinalize();
  mBatterySensor = new WbSensor();
  mKeyboardSensor = new WbSensor();
  mJoystickSensor = new WbSensor();

  if ((WbTokenizer::worldFileVersion() < WbVersion(2020, 1, 0) ||
       (proto() && proto()->fileVersion() < WbVersion(2020, 1, 0))) &&
      mControllerArgs->value().size() == 1 && mControllerArgs->value()[0].contains(' ')) {
    // we need to split the controllerArgs[0] at space boundaries into a list of strings
    // taking into account quotes and double quotes to avoid splitting in the middle of a quoted (or double quoted) string
    QStringList arguments;
    const QString args = mControllerArgs->value()[0].trimmed();
    const int l = args.length();
    bool insideDoubleQuote = false;
    bool insideSingleQuote = false;
    int previous = 0;
    for (int i = 0; i < l; i++) {
      if (!insideSingleQuote && args[i] == '"' && ((i == 0) || args[i - 1] != '\\'))
        insideDoubleQuote = !insideDoubleQuote;
      else if (!insideDoubleQuote && args[i] == '\'' && ((i == 0) || args[i - 1] != '\\'))
        insideSingleQuote = !insideSingleQuote;
      else if (args[i] == ' ' && !(insideSingleQuote || insideDoubleQuote)) {
        if (args[i - 1] != ' ')
          arguments << args.mid(previous, i - previous).remove('"').remove('\'');
        previous = i + 1;
      }
    }
    arguments << args.mid(previous).remove('"').remove('\'');
    const WbField *const controllerArgs = findField("controllerArgs", true);
    QString message;
    if (WbNodeUtilities::isTemplateRegeneratorField(controllerArgs))  // it would crash to change controllerArgs from here
      message = tr("Unable to split arguments automatically, please update your world file manually.");
    else {
      mControllerArgs->setValue(arguments);
      message = tr("Splitting arguments at space boundaries.");
    }
    parsingWarn(tr("Robot.controllerArgs data type changed from SFString to MFString in Webots R2020b. %1").arg(message));
  }
  updateSupervisor();
  updateWindow();
  updateRemoteControl();
  updateControllerDir();
  updateDevicesAfterInsertion();
  WbWorld::instance()->addRobotIfNotAlreadyPresent(this);
}

void WbRobot::postFinalize() {
  WbSolid::postFinalize();
  mKinematicDifferentialWheels = WbKinematicDifferentialWheels::createKinematicDifferentialWheelsIfNeeded(this);

  connect(mController, &WbSFString::changed, this, &WbRobot::updateControllerDir);
  connect(mWindow, &WbSFString::changed, this, &WbRobot::updateWindow);
  connect(mRemoteControl, &WbSFString::changed, this, &WbRobot::updateRemoteControl);
  connect(mCustomData, &WbSFString::changed, this, &WbRobot::updateData);
  connect(mSupervisor, &WbSFString::changed, this, &WbRobot::updateSupervisor);
  connect(this, &WbMatter::matterModelChanged, this, &WbRobot::updateModel);
  connect(WbSimulationState::instance(), &WbSimulationState::modeChanged, this, &WbRobot::updateSimulationMode);

  if (absoluteScale() != WbVector3(1.0, 1.0, 1.0))
    parsingWarn(tr("This Robot node is scaled: this is discouraged as it could compromise the correct physical behavior."));
}

void WbRobot::reset(const QString &id) {
  WbSolid::reset(id);
  mPreviousTime = -1.0;
  // restore battery level
  if (mBatteryInitialValues[id] > 0)
    mBattery->setItem(CURRENT_ENERGY, mBatteryInitialValues[id]);
  emit wasReset();
}

void WbRobot::save(const QString &id) {
  WbSolid::save(id);
  mBatteryInitialValues[id] = (mBattery->size() > CURRENT_ENERGY) ? mBattery->item(CURRENT_ENERGY) : -1.0;
}

void WbRobot::addDevices(WbNode *node) {
  if (!node)
    return;

  if (node != this && dynamic_cast<const WbRobot *>(node))
    return;  // do not recurse through child robots, their devices are hidden

  WbGroup *group = dynamic_cast<WbGroup *>(node);
  if (group) {
    WbSolidDevice *solidDevice = dynamic_cast<WbSolidDevice *>(node);
    if (solidDevice) {
      mDevices.append(solidDevice);
      connect(static_cast<WbSolid *>(node), &WbSolid::destroyed, this, &WbRobot::updateDevicesAfterDestruction,
              Qt::UniqueConnection);

      WbRenderingDevice *renderingDevice = dynamic_cast<WbRenderingDevice *>(node);
      if (renderingDevice) {
        connect(renderingDevice, &WbBaseNode::isBeingDestroyed, this, &WbRobot::removeRenderingDevice, Qt::UniqueConnection);
        mRenderingDevices.append(renderingDevice);
        WbAbstractCamera *camera = dynamic_cast<WbAbstractCamera *>(renderingDevice);
        if (camera) {
          connect(camera, &WbAbstractCamera::enabled, this, &WbRobot::updateActiveCameras, Qt::UniqueConnection);
          if (camera->isEnabled())
            mActiveCameras.append(camera);
        }
      }
    }

    WbTrack *const track = dynamic_cast<WbTrack *>(group);
    if (track) {
      const QVector<WbLogicalDevice *> trackDevices = track->devices();
      for (int i = 0; i < trackDevices.size(); ++i) {
        mDevices.append(trackDevices[i]);
        connect(static_cast<WbBaseNode *>(trackDevices[i]), &WbBaseNode::destroyed, this,
                &WbRobot::updateDevicesAfterDestruction, Qt::UniqueConnection);
      }
    }

    WbMFNode::Iterator it(group->children());
    while (it.hasNext())
      addDevices(it.next());
    return;
  }

  WbSkin *const skin = dynamic_cast<WbSkin *>(node);
  if (skin) {
    mDevices.append(skin);
    connect(static_cast<WbBaseNode *>(skin), &WbBaseNode::destroyed, this, &WbRobot::updateDevicesAfterDestruction,
            Qt::UniqueConnection);
    return;
  }

  WbSlot *const slot = dynamic_cast<WbSlot *>(node);
  if (slot) {
    addDevices(slot->endPoint());
    return;
  }

  WbBasicJoint *const basicJoint = dynamic_cast<WbBasicJoint *>(node);
  if (basicJoint) {
    WbJoint *const joint = dynamic_cast<WbJoint *>(basicJoint);
    if (joint) {
      const QVector<WbLogicalDevice *> &jointDevices = joint->devices();
      foreach (WbLogicalDevice *const jointDevice, jointDevices) {
        if (jointDevice == NULL)
          continue;
        mDevices.append(jointDevice);
        connect(static_cast<WbBaseNode *>(jointDevice), &WbBaseNode::destroyed, this, &WbRobot::updateDevicesAfterDestruction,
                Qt::UniqueConnection);
      }
    }
    if (basicJoint->solidReference() == NULL)
      addDevices(basicJoint->solidEndPoint());
    return;
  }

  WbPropeller *const propeller = dynamic_cast<WbPropeller *>(node);
  if (propeller) {
    WbLogicalDevice *const propellerDevice = propeller->device();
    if (propellerDevice) {
      mDevices.append(propellerDevice);
      connect(static_cast<WbBaseNode *>(propellerDevice), &WbBaseNode::destroyed, this, &WbRobot::updateDevicesAfterDestruction,
              Qt::UniqueConnection);
    }
    return;
  }

  // check if there are duplicated names, and print a warning if necessary
  if (dynamic_cast<const WbRobot *>(node)) {  // top node
    QStringList displayedWarnings;
    foreach (WbDevice *deviceA, mDevices) {
      foreach (WbDevice *deviceB, mDevices) {
        if (deviceA != deviceB && deviceA->deviceName() == deviceB->deviceName() &&
            !displayedWarnings.contains(deviceA->deviceName())) {
          parsingWarn(tr("At least two devices are sharing the same name (\"%1\") while unique names are required.")
                        .arg(deviceA->deviceName()));
          displayedWarnings << deviceA->deviceName();
        }
      }
    }
  }
}

void WbRobot::clearDevices() {
  foreach (const WbDevice *device, mDevices)
    disconnect(dynamic_cast<const WbBaseNode *>(device), &WbBaseNode::destroyed, this, &WbRobot::updateDevicesAfterDestruction);
  foreach (const WbRenderingDevice *device, mRenderingDevices)
    disconnect(device, &WbBaseNode::isBeingDestroyed, this, &WbRobot::removeRenderingDevice);
  mDevices.clear();
  mRenderingDevices.clear();
  mActiveCameras.clear();
}

void WbRobot::updateDevicesAfterDestruction() {
  clearDevices();
  addDevices(this);
}

void WbRobot::updateDevicesAfterInsertion() {
  clearDevices();
  addDevices(this);
  assignDeviceTags(false);
}

void WbRobot::pinToStaticEnvironment(bool pin) {
  if (!mShowWindowCalled) {
    warn(tr("Cannot pin the robot to the static environment, because the robot window has not been shown once"));
    return;
  }

  mPin = pin;

  if (mPin) {
    mPinTranslation = translation();
    mPinRotation = rotation();
  }
}

QString WbRobot::protoModelProjectPath() const {
  if (isProtoInstance())
    return proto()->projectPath();
  return QString();
}

QString WbRobot::searchDynamicLibraryAbsolutePath(const QString &key, const QString &pluginSubdirectory) {
  QString libBasename = WbStandardPaths::dynamicLibraryPrefix() + key + WbStandardPaths::dynamicLibraryExtension();

  if (!key.isEmpty()) {
    QString regularPath =
      WbProject::current()->pluginsPath() + pluginSubdirectory + "/" + key + "/build/release/" + libBasename;
    QString buildPath = regularPath;
    regularPath.remove("build/release/");

    if (QFile::exists(buildPath) || QFile::exists(regularPath))
      return regularPath;

    if (isProtoInstance()) {
      // search in project folder associated with PROTO instance
      QString protoProjectPath = protoModelProjectPath();
      if (!protoProjectPath.isEmpty()) {
        QDir protoDir(protoProjectPath + "/plugins/" + pluginSubdirectory + "/" + key);
        if (protoDir.exists()) {
          QDir protoBuildDir(protoDir.absolutePath() + "/build/release/");
          if (protoDir.exists(libBasename) || protoBuildDir.exists(libBasename))
            return protoDir.absolutePath() + "/" + libBasename;
        }
      }
      // search in project folder associated with parent PROTO models
      WbProtoModel *protoModel = proto();
      while (protoModel) {
        if (!protoModel->projectPath().isEmpty()) {
          QDir protoDir(protoModel->projectPath() + "/plugins/" + pluginSubdirectory + "/" + key);
          if (protoDir.exists()) {
            QDir protoBuildDir(protoDir.absolutePath() + "/build/release/");
            if (protoDir.exists(libBasename) || protoBuildDir.exists(libBasename))
              return protoDir.absolutePath() + "/" + libBasename;
          }
        }
        protoModel = WbProtoList::current()->findModel(protoModel->ancestorProtoName(), "");
      }
    }

    WbControllerPlugin::Type t = WbControllerPlugin::pluginSubDirectoryToType(pluginSubdirectory);
    const QStringList &list = WbControllerPlugin::defaultList(t);

    foreach (const QString &item, list) {
      if (item.endsWith(libBasename))
        return item;
    }
  }

  return QString();
}

void WbRobot::updateWindow() {
  mAbsoluteWindowFilename = "";

  if (mConfigureRequest) {
    QString key = mWindow->value().trimmed();
    if (!key.isEmpty()) {
      const QString &absoluteFilePath = searchDynamicLibraryAbsolutePath(key, "robot_windows");
      if (absoluteFilePath.isEmpty() && windowFile().isEmpty())  // not a HTML robot window
        warn(tr("The robot window library has not been found."));
      else
        mAbsoluteWindowFilename = absoluteFilePath;
    }
  } else
    warn(tr("The robot 'window' cannot be modified after the controller is initialized."));

  if (!mWindow->value().isEmpty())
    return;  // HTML robot window without plugin case.

  if (mAbsoluteWindowFilename.isEmpty()) {
    mAbsoluteWindowFilename = WbStandardPaths::resourcesRobotWindowsPluginsPath() + "generic/" +
                              WbStandardPaths::dynamicLibraryPrefix() + "generic" + WbStandardPaths::dynamicLibraryExtension();
    if (!QFile::exists(mAbsoluteWindowFilename))
      warn(tr("The generic robot window is not found. Please check your Webots installation."));
  }

  if (!mAbsoluteWindowFilename.isEmpty())
    WbBinaryIncubator::copyBinaryAndDependencies(mAbsoluteWindowFilename);
}

void WbRobot::updateRemoteControl() {
  mAbsoluteRemoteControlFilename = "";

  if (mConfigureRequest) {
    QString key = mRemoteControl->value().trimmed();
    if (!key.isEmpty()) {
      const QString &absoluteFilePath = searchDynamicLibraryAbsolutePath(key, "remote_controls");
      if (absoluteFilePath.isEmpty())
        warn(tr("The remote control library has not been found."));
      else
        mAbsoluteRemoteControlFilename = absoluteFilePath;
    }
  } else
    warn(tr("'remoteControl' cannot be modified after the controller is initialized."));

  if (!mAbsoluteRemoteControlFilename.isEmpty())
    WbBinaryIncubator::copyBinaryAndDependencies(mAbsoluteRemoteControlFilename);
}

void WbRobot::updateControllerDir() {
  const QString &controllerName = mController->value();
  if (!controllerName.isEmpty() && controllerName != "<extern>") {
    QStringList path;
    path << WbProject::current()->controllersPath() + controllerName + '/';
    const WbProtoModel *const protoModel = proto();
    if (protoModel)
      path << QDir::cleanPath(protoModelProjectPath() + "/controllers/" + controllerName) + '/';
    if (WbProject::extraDefaultProject())
      path << WbProject::extraDefaultProject()->controllersPath() + controllerName + '/';
    path << WbProject::defaultProject()->controllersPath() + controllerName + '/';
    path << WbProject::system()->controllersPath() + controllerName + '/';
    path.removeDuplicates();

    mControllerDir = "";
    const int size = path.size();
    for (int i = 0; i < size; ++i) {
      if (!QDir(path[i]).exists())
        continue;
      mControllerDir = path[i];
      break;
    }

    if (mControllerDir.isEmpty()) {
      QString warning(tr("The controller directory has not been found, searched the following locations:"));
      const int otherSize = path.size();
      for (int i = 0; i < otherSize; ++i)
        warning += "\n" + path[i];
      warn(warning);
    }
  }

  if (isPostFinalizedCalled()) {
    emit controllerChanged();
    foreach (WbRenderingDevice *device, mRenderingDevices) {
      WbAbstractCamera *ac = dynamic_cast<WbAbstractCamera *>(device);
      if (ac)
        ac->resetSharedMemory();  // shared memory automatically deleted at new controller start
    }
  }
}

const QString &WbRobot::controllerDir() {
  if (!isPreFinalizedCalled())
    updateControllerDir();

  return mControllerDir;
}

void WbRobot::restartController() {
  mControllerStarted = false;

  emit controllerChanged();

  foreach (WbDevice *device, mDevices) {
    WbAbstractCamera *ac = dynamic_cast<WbAbstractCamera *>(device);
    if (ac)
      ac->resetSharedMemory();  // shared memory automatically deleted at new controller restart
  }
  if (mSupervisorUtilities)
    mSupervisorUtilities->reset();
}

bool WbRobot::isWaitingForUserInputEvent() const {
  return mMonitoredUserInputEventTypes != -1;
}

void WbRobot::setWaitingForWindow(bool waiting) {
  if (mWaitingForWindow == waiting)
    return;
  mWaitingForWindow = waiting;
  if (!waiting)
    emit windowReady();
}

void WbRobot::updateData() {
  mDataNeedToWriteAnswer = true;
}

void WbRobot::updateSupervisor() {
  mSupervisorNeedToWriteAnswer = true;
  if (mConfigureRequest) {
    delete mSupervisorUtilities;
    mSupervisorUtilities = supervisor() ? new WbSupervisorUtilities(this) : NULL;
  } else
    mSupervisorUtilitiesNeedUpdate = true;
}

void WbRobot::updateModel() {
  mModelNeedToWriteAnswer = true;
}

void WbRobot::removeRenderingDevice() {
  mRenderingDevices.removeOne(static_cast<WbRenderingDevice *>(sender()));
}

void WbRobot::assignDeviceTags(bool reset) {
  int i = reset ? 1 : mNextTag;  // device tag 0 is reserved for the robot
  foreach (WbDevice *const device, mDevices) {
    if (reset || !device->hasTag()) {
      device->setTag(i++);
      if (!reset)
        mNewlyAddedDevices << device;
    }
  }
  mNextTag = i;
}

double WbRobot::currentEnergy() const {
  return (mBattery->size() > CURRENT_ENERGY) ? mBattery->item(CURRENT_ENERGY) : -1.0;
}

void WbRobot::setCurrentEnergy(double e) {
  assert(e >= 0.0);
  if (!mPowerOn && e > 0)
    powerOn(true);
  else if (e == 0 && mPowerOn)
    powerOn(false);

  if (mBattery->size() > CURRENT_ENERGY)
    mBattery->setItem(CURRENT_ENERGY, e);
}

double WbRobot::maxEnergy() const {
  return (mBattery->size() > MAX_ENERGY) ? mBattery->item(MAX_ENERGY) : -1.0;
}

double WbRobot::energyUploadSpeed() const {
  return (mBattery->size() > ENERGY_UPLOAD_SPEED) ? mBattery->item(ENERGY_UPLOAD_SPEED) : -1.0;
}

double WbRobot::energyConsumption() const {
  double e = mCpuConsumption->value();
  foreach (WbDevice *device, mDevices)  // add energy consumption for each device
    e += device->energyConsumption();
  return e;
}

void WbRobot::prePhysicsStep(double ms) {
  WbSolid::prePhysicsStep(ms);
  if (mKinematicDifferentialWheels)
    mKinematicDifferentialWheels->applyKinematicMotion(ms);
}

void WbRobot::postPhysicsStep() {
  WbSolid::postPhysicsStep();
  if (mPin) {
    setTranslation(mPinTranslation);
    setRotation(mPinRotation);
    resetPhysics();
  }
  double energy = currentEnergy();
  if (energy > 0.0) {
    double s = WbWorld::instance()->basicTimeStep() * 0.001;
    energy -= energyConsumption() * s;
    if (energy <= 0.0) {
      energy = 0.0;
      mBatterySensor->updateTimer();  // so that the battery sensor returns 0
    }
    setCurrentEnergy(energy);
  }
  if (mNeedToRestartController) {
    restartController();
    mNeedToRestartController = false;
  }
  if (mSupervisorUtilities)
    mSupervisorUtilities->postPhysicsStep();
}

WbDevice *WbRobot::findDevice(WbDeviceTag tag) const {
  foreach (WbDevice *const device, mDevices)
    if (device->tag() == tag)
      return device;

  return NULL;  // not found
}

void WbRobot::powerOn(bool e) {
  mPowerOn = e;
  foreach (WbDevice *const device, mDevices)
    device->powerOn(e);
}

void WbRobot::keyPressed(int key, int modifiers) {
  const int pressedKeys = modifiers + (gSpecialKeys.contains(key) ? gSpecialKeys.value(key) : key & WB_KEYBOARD_KEY);

  if (pressedKeys && !mPressedKeys.contains(pressedKeys)) {
    mPressedKeys.prepend(pressedKeys);
    mKeyboardHasChanged = true;
    emit keyboardChanged();
  }
}

void WbRobot::keyReleased(int key) {
  bool reset = true;
  QMutableListIterator<int> it(mPressedKeys);
  while (it.hasNext()) {
    int i = it.next();
    if ((i & 0xffff) == (gSpecialKeys.value(key) & 0xffff)) {
      // remove all sequences containing the released special key
      it.remove();
      reset = false;
    } else if ((i & 0xffff) == (key & 0xffff)) {
      // remove all sequences containing the released key
      it.remove();
      reset = false;
    }
  }

  if (reset)
    mPressedKeys.clear();

  mKeyboardHasChanged = true;
  emit keyboardChanged();
}

void WbRobot::writeDeviceConfigure(QList<WbDevice *> devices, QDataStream &stream) const {
  QListIterator<WbDevice *> it(devices);
  while (it.hasNext()) {
    const WbDevice *device = it.next();
    stream << (short int)device->deviceNodeType();
    QByteArray n(device->deviceName().toUtf8());
    stream.writeRawData(n.constData(), n.size() + 1);
    const WbSolidDevice *solidDevice = dynamic_cast<const WbSolidDevice *>(device);
    if (solidDevice)
      n = solidDevice->model().toUtf8();
    else
      n = "";
    stream.writeRawData(n.constData(), n.size() + 1);
  }
}

void WbRobot::writeConfigure(QDataStream &stream) {
  mBatterySensor->connectToRobotSignal(this);
  stream << (short unsigned int)0;
  stream << (unsigned char)C_CONFIGURE;
  stream << (unsigned char)(mSupervisorUtilities ? 1 : 0);
  stream << (unsigned char)(synchronization() ? 1 : 0);
  stream << (short int)(1 + deviceCount());
  stream << (short int)nodeType();
  stream << (double)0.001 * WbSimulationState::instance()->time();

  QByteArray n = name().toUtf8();
  stream.writeRawData(n.constData(), n.size() + 1);

  writeDeviceConfigure(mDevices, stream);

  stream << (double)WbWorld::instance()->basicTimeStep();

  QString projectPath = WbProject::current()->path();
  // remove the last trailing space if any
  if (projectPath.endsWith('/'))
    projectPath.remove(-1, 1);
  n = projectPath.toUtf8();
  stream.writeRawData(n.constData(), n.size() + 1);

  QString worldPath = WbWorld::instance()->fileName();
  n = worldPath.toUtf8();
  stream.writeRawData(n.constData(), n.size() + 1);

  n = model().toUtf8();
  stream.writeRawData(n.constData(), n.size() + 1);
  n = mAbsoluteWindowFilename.toUtf8();
  stream.writeRawData(n.constData(), n.size() + 1);
  n = mAbsoluteRemoteControlFilename.toUtf8();
  stream.writeRawData(n.constData(), n.size() + 1);
  n = controllerName().toUtf8();
  stream.writeRawData(n.constData(), n.size() + 1);
  n = customData().toUtf8();
  stream.writeRawData(n.constData(), n.size() + 1);

  // show the robot window at step 0
  stream << (unsigned char)(mShowWindow->value() || mShowWindowMessage);
  stream << (unsigned char)(!windowFile().isEmpty());
  stream << (int)computeSimulationMode();

  mShowWindowMessage = false;
  mUpdateWindowMessage = false;
  mShowWindowCalled = true;
  mConfigureRequest = false;
  if (mSupervisorUtilities)
    mSupervisorUtilities->writeConfigure(stream);
}

void WbRobot::dispatchMessage(QDataStream &stream) {
  while (!stream.atEnd()) {
    WbDeviceTag tag;
    stream >> tag;
    int size;
    stream >> size;

    if (tag == 0) {
      const int end = stream.device()->pos() + size;
      do  // handle all requests for this robot
        handleMessage(stream);
      while (stream.device()->pos() < end);
    } else {
      WbDevice *const device = findDevice(tag);
      if (device) {
        const int end = stream.device()->pos() + size;
        do  // handle all requests for this device
          device->handleMessage(stream);
        while (stream.device()->pos() < end);
      } else  // device was deleted in Webots (but the controller does not know about it)
        stream.skipRawData(size);
    }
  }
}

void WbRobot::handleMessage(QDataStream &stream) {
  QIODevice *const device = stream.device();

  char byte;
  unsigned char pin;
  device->getChar(&byte);

  switch (byte) {
    case C_CONFIGURE:
      updateDevicesAfterInsertion();
      mConfigureRequest = true;
      return;
    case C_ROBOT_URDF: {
      short size;

      mNeedToWriteUrdf = true;
      stream >> size;
      char data[size];
      stream.readRawData(data, size);
      setUrdfPrefix(QString(data));

      return;
    }
    case C_SET_SAMPLING_PERIOD:  // for the scene tracker
      /*
      stream >> mRefreshRate;
      if (needSceneTracker==false) {
        A_SceneTracker::addNeed();
        needSceneTracker = true;
      }
      */
      return;
    case C_ROBOT_SET_BATTERY_SAMPLING_PERIOD: {
      short rate;
      stream >> rate;
      mBatterySensor->setRefreshRate(rate);
      if (mBattery->isEmpty())
        warn(tr("'wb_robot_battery_sensor_enable' called while the 'battery' field is empty."));
      return;
    }
    case C_ROBOT_SET_DATA: {
      short size;
      stream >> size;
      char data[size];
      stream.readRawData(data, size);
      mCustomData->setValue(data);
      return;
    }
    case C_ROBOT_SET_KEYBOARD_SAMPLING_PERIOD: {
      short rate;
      stream >> rate;
      mKeyboardSensor->setRefreshRate(rate);
      return;
    }
    case C_ROBOT_SET_JOYSTICK_SAMPLING_PERIOD: {
      short rate;
      stream >> rate;
      mJoystickSensor->setRefreshRate(rate);
      if (rate > 0) {
        if (!mJoystickInterface) {
          mJoystickInterface = new WbJoystickInterface();
          if (mJoystickInterface->isCorrectlyInitialized()) {
            mJoystickTimer = new QTimer(this);
            mJoystickTimer->setSingleShot(false);
            connect(mJoystickTimer, &QTimer::timeout, mJoystickInterface, &WbJoystickInterface::step);
            mJoystickConfigureRequest = true;
            mJoystickTimer->start(mJoystickInterface->updateRate());
          } else {
            warn(mJoystickInterface->initializationError());
            delete mJoystickInterface;
            mJoystickInterface = NULL;
          }
        }
      } else if (mJoystickInterface) {
        mJoystickTimer->stop();
        disconnect(mJoystickTimer, &QTimer::timeout, mJoystickInterface, &WbJoystickInterface::step);
        delete mJoystickTimer;
        delete mJoystickInterface;
        mJoystickTimer = NULL;
        mJoystickInterface = NULL;
        delete mJoyStickLastValue;
        mJoyStickLastValue = NULL;
      }
      return;
    }
    case C_ROBOT_SET_MOUSE_SAMPLING_PERIOD: {
      short rate;
      stream >> rate;
      if (rate <= 0 && mMouse != NULL) {
        WbMouse::destroy(mMouse);
        mMouse = NULL;
      } else if (rate > 0) {
        if (mMouse == NULL)
          mMouse = WbMouse::create();
        mMouse->setRefreshRate(rate);
      }
      return;
    }
    case C_ROBOT_MOUSE_ENABLE_3D_POSITION: {
      unsigned char enable;
      stream >> enable;
      if (mMouse)
        mMouse->set3dPositionEnabled(enable);
      return;
    }
    case C_ROBOT_SET_JOYSTICK_FORCE_FEEDBACK: {
      short level;
      stream >> level;
      if (mJoystickInterface && mJoystickInterface->isCorrectlyInitialized())
        mJoystickInterface->addForce(level);
      return;
    }
    case C_ROBOT_SET_JOYSTICK_FORCE_FEEDBACK_DURATION: {
      double duration;
      stream >> duration;
      if (mJoystickInterface && mJoystickInterface->isCorrectlyInitialized())
        mJoystickInterface->setConstantForceDuration(duration);
      return;
    }
    case C_ROBOT_SET_JOYSTICK_AUTO_CENTERING_GAIN: {
      double gain;
      stream >> gain;
      if (mJoystickInterface && mJoystickInterface->isCorrectlyInitialized())
        mJoystickInterface->setAutoCenteringGain(gain);
      return;
    }
    case C_ROBOT_SET_JOYSTICK_RESISTANCE_GAIN: {
      double gain;
      stream >> gain;
      if (mJoystickInterface && mJoystickInterface->isCorrectlyInitialized())
        mJoystickInterface->setResistanceGain(gain);
      return;
    }
    case C_ROBOT_SET_JOYSTICK_FORCE_AXIS: {
      int axis;
      stream >> axis;
      if (mJoystickInterface && mJoystickInterface->isCorrectlyInitialized())
        mJoystickInterface->setForceAxis(axis);
    }
    case C_ROBOT_CLIENT_EXIT_NOTIFY:
      emit controllerExited();
      return;
    case C_ROBOT_REMOTE_ON:
      emit toggleRemoteMode(true);
      return;
    case C_ROBOT_REMOTE_OFF:
      emit toggleRemoteMode(false);
      return;
    case C_ROBOT_PIN:
      stream >> pin;
      pinToStaticEnvironment((bool)pin);
      return;
    case C_CONSOLE_MESSAGE: {
      unsigned char streamChannel = 0;
      stream >> streamChannel;
      unsigned int size;
      stream >> size;
      char nativeMessage[size];
      stream.readRawData(nativeMessage, size);
      QString message(nativeMessage);
      if (!message.endsWith('\n'))
        message += '\n';
      // cppcheck-suppress knownConditionTrueFalse
      emit appendMessageToConsole(message, streamChannel == 0);
      return;
    }
    case C_ROBOT_WWI_MESSAGE: {
      int size;
      stream >> size;
      QByteArray message;
      message.resize(size - 1);  // an extra character is always added for the final '\0'
      stream.readRawData(message.data(), size);
      emit sendToJavascript(message);
      return;
    }
    case C_ROBOT_WAIT_FOR_USER_INPUT_EVENT: {
      int timeout;
      stream >> mMonitoredUserInputEventTypes;
      stream >> timeout;
      mNeedToWriteUserInputEventAnswer = false;
      if (mMonitoredUserInputEventTypes >= 0) {
        if (!mUserInputEventTimer) {
          mUserInputEventTimer = new QTimer(this);
          mUserInputEventTimer->setSingleShot(true);
          connect(mUserInputEventTimer, &QTimer::timeout, this, &WbRobot::userInputEventNeedUpdate);
        }
        if (((WB_EVENT_MOUSE_CLICK | WB_EVENT_MOUSE_MOVE) & mMonitoredUserInputEventTypes) && mMouse) {
          mMouse->setHasMoved(false);
          mMouse->setHasClicked(false);
          mMouse->setTracked(true);
          connect(mMouse, &WbMouse::changed, this, &WbRobot::handleMouseChange);
        }
        if ((WB_EVENT_KEYBOARD & mMonitoredUserInputEventTypes) && mKeyboardSensor->isEnabled()) {
          mKeyboardHasChanged = false;
          connect(this, &WbRobot::keyboardChanged, this, &WbRobot::userInputEventNeedUpdate);
        }
        if (((WB_EVENT_JOYSTICK_BUTTON | WB_EVENT_JOYSTICK_AXIS | WB_EVENT_JOYSTICK_POV) & mMonitoredUserInputEventTypes) &&
            mJoystickInterface && mJoystickInterface->isCorrectlyInitialized()) {
          mJoystickInterface->resetButtonHasChanged();
          mJoystickInterface->resetPovHasChanged();
          mJoystickInterface->resetAxisHasChanged();
          connect(mJoystickInterface, &WbJoystickInterface::changed, this, &WbRobot::handleJoystickChange);
        }
        mUserInputEventTimer->start(timeout);
        mUserInputEventReferenceTime = QDateTime::currentDateTime();
      } else if (mUserInputEventTimer)
        mUserInputEventTimer->stop();
      return;
    }
    default:
      // if it was not catched, then this message is apparently for a subclass of WbRobot
      // we must rewind 1 byte so the Supervisor can read the command
      device->ungetChar(byte);
  }
  if (mSupervisorUtilities)
    mSupervisorUtilities->handleMessage(stream);
}

void WbRobot::dispatchAnswer(QDataStream &stream, bool includeDevices) {
  if (mConfigureRequest) {
    assignDeviceTags(true);
    writeConfigure(stream);
    if (includeDevices) {
      foreach (WbDevice *const device, mDevices) {
        assert(device->hasTag());
        device->writeConfigure(stream);
      }
    }
    mNewlyAddedDevices.clear();
  } else {
    writeAnswer(stream);
    if (includeDevices) {
      foreach (WbDevice *const device, mDevices) {
        assert(device->hasTag());
        device->writeAnswer(stream);
      }
    }
  }
}

void WbRobot::writeAnswer(QDataStream &stream) {
  const double time = 0.001 * WbSimulationState::instance()->time();
  if (time != mPreviousTime) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_TIME;
    stream << (double)time;
    mPreviousTime = time;
  }

  if (mSimulationModeRequested) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_SIMULATION_CHANGE_MODE;
    stream << (int)computeSimulationMode();
    mSimulationModeRequested = false;
  }

  if (refreshBatterySensorIfNeeded() || mBatterySensor->hasPendingValue()) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_BATTERY_VALUE;
    stream << (double)currentEnergy();
    mBatterySensor->resetPendingValue();
  }

  if (mDataNeedToWriteAnswer) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_DATA;
    QByteArray n = customData().toUtf8();
    stream.writeRawData(n.constData(), n.size() + 1);

    mDataNeedToWriteAnswer = false;
  }

  if (mSupervisorNeedToWriteAnswer) {
    if (mSupervisorUtilitiesNeedUpdate) {
      mSupervisorUtilitiesNeedUpdate = false;
      delete mSupervisorUtilities;
      mSupervisorUtilities = supervisor() ? new WbSupervisorUtilities(this) : NULL;
    }

    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_SUPERVISOR;
    stream << (unsigned char)(mSupervisorUtilities ? 1 : 0);

    mSupervisorNeedToWriteAnswer = false;
  }

  if (mModelNeedToWriteAnswer) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_MODEL;
    QByteArray n = model().toUtf8();
    stream.writeRawData(n.constData(), n.size() + 1);

    mModelNeedToWriteAnswer = false;
  }

  if (!mNewlyAddedDevices.isEmpty()) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_NEW_DEVICE;
    stream << (short int)mNewlyAddedDevices.size();
    writeDeviceConfigure(mNewlyAddedDevices, stream);
    QListIterator<WbDevice *> it(mNewlyAddedDevices);
    while (it.hasNext()) {
      WbDevice *device = it.next();
      assert(device->hasTag());
      device->writeConfigure(stream);
    }
    mNewlyAddedDevices.clear();
  }

  int userInputEvents = 0;
  if (mMonitoredUserInputEventTypes >= 0 && mNeedToWriteUserInputEventAnswer) {
    mUserInputEventTimer->stop();
    if (mMouse && (mMonitoredUserInputEventTypes & (WB_EVENT_MOUSE_CLICK | WB_EVENT_MOUSE_MOVE))) {
      if (mMouse->hasClicked() && (mMonitoredUserInputEventTypes & WB_EVENT_MOUSE_CLICK))
        userInputEvents = userInputEvents | WB_EVENT_MOUSE_CLICK;
      if (mMouse->hasMoved() && (mMonitoredUserInputEventTypes & WB_EVENT_MOUSE_MOVE))
        userInputEvents = userInputEvents | WB_EVENT_MOUSE_MOVE;
      mMouse->setTracked(false);
      disconnect(mMouse, &WbMouse::changed, this, &WbRobot::handleMouseChange);
    }
    if (mMonitoredUserInputEventTypes & WB_EVENT_KEYBOARD) {
      if (mKeyboardHasChanged)
        userInputEvents = userInputEvents | WB_EVENT_KEYBOARD;
      disconnect(this, &WbRobot::keyboardChanged, this, &WbRobot::userInputEventNeedUpdate);
    }
    if (mJoystickInterface &&
        (mMonitoredUserInputEventTypes & (WB_EVENT_JOYSTICK_BUTTON | WB_EVENT_JOYSTICK_AXIS | WB_EVENT_JOYSTICK_POV))) {
      if (mJoystickInterface->buttonHasChanged() && (mMonitoredUserInputEventTypes & WB_EVENT_JOYSTICK_BUTTON))
        userInputEvents = userInputEvents | WB_EVENT_JOYSTICK_BUTTON;
      if (mJoystickInterface->povHasChanged() && (mMonitoredUserInputEventTypes & WB_EVENT_JOYSTICK_POV))
        userInputEvents = userInputEvents | WB_EVENT_JOYSTICK_POV;
      if (mJoystickInterface->axisHasChanged() && (mMonitoredUserInputEventTypes & WB_EVENT_JOYSTICK_AXIS))
        userInputEvents = userInputEvents | WB_EVENT_JOYSTICK_AXIS;
      disconnect(mJoystickInterface, &WbJoystickInterface::changed, this, &WbRobot::handleJoystickChange);
    }
  }

  if (refreshKeyboardSensorIfNeeded() || mKeyboardSensor->hasPendingValue()) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_KEYBOARD_VALUE;
    stream << (unsigned char)mKeyboardLastValue.size();

    foreach (int key, mKeyboardLastValue)
      stream << (int)key;

    mKeyboardSensor->resetPendingValue();
  }

  if (mJoystickConfigureRequest && mJoystickInterface && mJoystickInterface->isCorrectlyInitialized()) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_JOYSTICK_CONFIG;
    stream << (int)mJoystickInterface->numberOfAxes();
    stream << (int)mJoystickInterface->numberOfButtons();
    stream << (int)mJoystickInterface->numberOfPovs();
    QByteArray n = mJoystickInterface->model().toUtf8();
    stream.writeRawData(n.constData(), n.size() + 1);
  } else if (mJoystickConfigureRequest) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_JOYSTICK_CONFIG;
    stream << (int)-1;
    stream << (int)-1;
    stream << (int)-1;
  }

  if (refreshJoyStickSensorIfNeeded() || mJoystickSensor->hasPendingValue()) {
    assert(mJoyStickLastValue);
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_JOYSTICK_VALUE;
    stream << (int)mJoyStickLastValue->numberOfPressedButtons;
    for (int i = 0; i < mJoyStickLastValue->pressedButtonsIndices.size(); ++i)
      stream << (int)mJoyStickLastValue->pressedButtonsIndices[i];
    stream << (int)mJoyStickLastValue->numberOfAxes;
    for (int i = 0; i < mJoyStickLastValue->axesValues.size(); ++i)
      stream << (int)mJoyStickLastValue->axesValues[i];
    stream << (int)mJoyStickLastValue->numberOfPovs;
    for (int i = 0; i < mJoyStickLastValue->povsValues.size(); ++i)
      stream << (int)mJoyStickLastValue->povsValues[i];
    mJoystickSensor->resetPendingValue();
  }

  if ((userInputEvents & (WB_EVENT_MOUSE_CLICK | WB_EVENT_MOUSE_MOVE)) ||
      (mMouse != NULL && mPowerOn && mMouse->hasPendingValue())) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_MOUSE_VALUE;
    stream << (unsigned char)mMouse->left();
    stream << (unsigned char)mMouse->middle();
    stream << (unsigned char)mMouse->right();
    stream << (double)mMouse->u();
    stream << (double)mMouse->v();
    stream << (double)mMouse->x();
    stream << (double)mMouse->y();
    stream << (double)mMouse->z();
    mMouse->reset();
  }

  if (mMonitoredUserInputEventTypes >= 0 && mNeedToWriteUserInputEventAnswer) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_WAIT_FOR_USER_INPUT_EVENT;
    stream << (int)userInputEvents;
    mMonitoredUserInputEventTypes = -1;
    mNeedToWriteUserInputEventAnswer = false;
  }

  if (mSupervisorUtilities)
    mSupervisorUtilities->writeAnswer(stream);

  if (mNeedToWriteUrdf) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_URDF;

    QString urdfContent;
    WbVrmlWriter writer(&urdfContent, modelName() + ".urdf");

    writer.writeHeader(name());
    write(writer);
    writer.writeFooter();

    stream.writeRawData(urdfContent.toLocal8Bit(), urdfContent.size() + 1);

    mNeedToWriteUrdf = false;
  }
}

bool WbRobot::hasImmediateAnswer() const {
  if (mConfigureRequest)
    return false;
  return mShowWindowMessage || mUpdateWindowMessage || mMessageFromWwi;
}

void WbRobot::writeImmediateAnswer(QDataStream &stream) {
  if (mConfigureRequest)
    return;
  if (mShowWindowMessage) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_WINDOW_SHOW;
    mShowWindowMessage = false;
    mShowWindowCalled = true;
  }
  if (mUpdateWindowMessage) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_WINDOW_UPDATE;
    mUpdateWindowMessage = false;
  }
  if (mMessageFromWwi) {
    stream << (short unsigned int)0;
    stream << (unsigned char)C_ROBOT_WWI_MESSAGE;
    int length = mMessageFromWwi->length() + 1;
    stream << length;
    stream.writeRawData(mMessageFromWwi->constData(), length);
    delete mMessageFromWwi;
    mMessageFromWwi = NULL;
  }
}

void WbRobot::handleMouseChange() {
  assert(mMouse);
  assert(mUserInputEventTimer);

  if ((mMonitoredUserInputEventTypes & WB_EVENT_MOUSE_CLICK) && mMouse->hasClicked()) {
    mUserInputEventTimer->stop();
    emit userInputEventNeedUpdate();
    return;
  }

  if ((mMonitoredUserInputEventTypes & WB_EVENT_MOUSE_MOVE) && mMouse->hasMoved()) {
    const int elapsedTime = mUserInputEventReferenceTime.msecsTo(QDateTime::currentDateTime());
    const int difference = elapsedTime - mMouse->refreshRate();
    if (difference > 0) {
      mUserInputEventTimer->stop();
      emit userInputEventNeedUpdate();
      return;
    } else
      mUserInputEventTimer->start(-difference);
  }
}

void WbRobot::handleJoystickChange() {
  assert(mJoystickInterface);
  assert(mUserInputEventTimer);

  if (((mMonitoredUserInputEventTypes & WB_EVENT_JOYSTICK_BUTTON) && mJoystickInterface->buttonHasChanged()) ||
      ((mMonitoredUserInputEventTypes & WB_EVENT_JOYSTICK_POV) && mJoystickInterface->povHasChanged())) {
    mUserInputEventTimer->stop();
    emit userInputEventNeedUpdate();
    return;
  }

  if ((mMonitoredUserInputEventTypes & WB_EVENT_JOYSTICK_AXIS) && mJoystickInterface->axisHasChanged()) {
    const int elapsedTime = mUserInputEventReferenceTime.msecsTo(QDateTime::currentDateTime());
    const int difference = elapsedTime - mJoystickSensor->refreshRate();
    if (difference > 0) {
      mUserInputEventTimer->stop();
      emit userInputEventNeedUpdate();
      return;
    } else
      mUserInputEventTimer->start(-difference);
  }
}

QString WbRobot::windowFile(const QString &extension) {
  if (window().isEmpty())
    return WbStandardPaths::resourcesRobotWindowsPluginsPath() + "generic/generic." + extension;

  const QString fileName = window() + "/" + window() + "." + extension;
  QString path = WbProject::current()->robotWindowPluginsPath() + fileName;
  QFileInfo file(path);
  if (file.exists() && file.isFile() && file.isReadable())
    return path;
  if (isProtoInstance()) {
    // search in project folder associated with PROTO instance
    path = protoModelProjectPath() + "/plugins/robot_windows/" + fileName;
    file = QFileInfo(path);
    if (file.exists() && file.isFile() && file.isReadable())
      return path;
    // search in project folder associated with parent PROTO models
    WbProtoModel *protoModel = proto();
    while (protoModel) {
      if (!protoModel->projectPath().isEmpty()) {
        path = protoModel->projectPath() + "/plugins/robot_windows/" + fileName;
        file = QFileInfo(path);
        if (file.exists() && file.isFile() && file.isReadable())
          return path;
      }
      protoModel = WbProtoList::current()->findModel(protoModel->ancestorProtoName(), "");
    }
  }

  return "";
}

void WbRobot::showWindow() {  // show the Qt-based controller robot window (to be deprecated)
  mShowWindowMessage = true;
  if (!isControllerStarted())
    startController();
  emit immediateMessageAdded();
}

void WbRobot::updateControllerWindow() {  // run a html window step with duration 0 ms
  mUpdateWindowMessage = true;
  emit immediateMessageAdded();
}

void WbRobot::processImmediateMessages() {
  if (mSupervisorUtilities)
    mSupervisorUtilities->processImmediateMessages();
}

void WbRobot::startController() {
  emit startControllerRequest(this);
}

void WbRobot::receiveFromJavascript(const QByteArray &message) {
  delete mMessageFromWwi;
  mMessageFromWwi = new QByteArray(message);
  emit immediateMessageAdded();
}

void WbRobot::updateSimulationMode() {
  mSimulationModeRequested = true;
  emit immediateMessageAdded();
}

void WbRobot::descendantNodeInserted(WbBaseNode *decendant) {
  if (isPreFinalizedCalled())
    updateDevicesAfterInsertion();
}

void WbRobot::updateActiveCameras(WbAbstractCamera *camera, bool isActive) {
  if (isActive) {
    if (!mActiveCameras.contains(camera))
      mActiveCameras.append(camera);
    return;
  }

  mActiveCameras.removeOne(camera);
}

void WbRobot::renderCameras() {
  for (int i = 0; i < mActiveCameras.size(); ++i)
    mActiveCameras[i]->updateCameraTexture();
}

void WbRobot::updateSensors() {
  refreshBatterySensorIfNeeded();
  refreshKeyboardSensorIfNeeded();
  refreshJoyStickSensorIfNeeded();

  if (mDevices.isEmpty())
    return;

  foreach (WbDevice *const device, mDevices)
    device->refreshSensorIfNeeded();
}

bool WbRobot::refreshBatterySensorIfNeeded() {
  if (mPowerOn && mBatterySensor->needToRefresh()) {
    mBatteryLastValue = currentEnergy();
    mBatterySensor->updateTimer();
    return true;
  }
  return false;
}

bool WbRobot::refreshKeyboardSensorIfNeeded() {
  if ((mPowerOn && mKeyboardSensor->needToRefresh()) ||
      ((mMonitoredUserInputEventTypes & WB_EVENT_KEYBOARD) && mKeyboardHasChanged)) {
    mKeyboardLastValue = mPressedKeys;
    mKeyboardSensor->updateTimer();
    return true;
  }
  return false;
}

bool WbRobot::refreshJoyStickSensorIfNeeded() {
  if ((mJoystickInterface && mJoystickInterface->isCorrectlyInitialized()) &&
      ((mPowerOn && mJoystickSensor->needToRefresh()) ||
       ((WB_EVENT_JOYSTICK_BUTTON & mMonitoredUserInputEventTypes) && mJoystickInterface->buttonHasChanged()) ||
       ((WB_EVENT_JOYSTICK_POV & mMonitoredUserInputEventTypes) && mJoystickInterface->povHasChanged()) ||
       ((WB_EVENT_JOYSTICK_AXIS & mMonitoredUserInputEventTypes) && mJoystickInterface->axisHasChanged()))) {
    if (mJoyStickLastValue == NULL)
      mJoyStickLastValue = new JoyStickLastValue;
    if (WB_EVENT_JOYSTICK_BUTTON & mMonitoredUserInputEventTypes)
      mJoystickInterface->releaseButtons();
    mJoyStickLastValue->pressedButtonsIndices.clear();
    mJoyStickLastValue->axesValues.clear();
    mJoyStickLastValue->povsValues.clear();
    mJoystickInterface->capture();
    int numberOfButttons = mJoystickInterface->numberOfButtons();
    mJoyStickLastValue->numberOfPressedButtons = mJoystickInterface->numberOfPressedButtons();
    for (int i = 0; i < numberOfButttons; ++i) {
      if (mJoystickInterface->isButtonPressed(i))
        mJoyStickLastValue->pressedButtonsIndices.append(i);
    }
    mJoyStickLastValue->numberOfAxes = mJoystickInterface->numberOfAxes();
    for (int i = 0; i < mJoyStickLastValue->numberOfAxes; ++i)
      mJoyStickLastValue->axesValues.append((int)mJoystickInterface->axisValue(i));
    mJoyStickLastValue->numberOfPovs = mJoystickInterface->numberOfPovs();
    for (int i = 0; i < mJoyStickLastValue->numberOfPovs; ++i)
      mJoyStickLastValue->povsValues.append((int)mJoystickInterface->povValue(i));
    mJoystickInterface->releaseButtons();
    mJoystickSensor->updateTimer();
    return true;
  }
  return false;
}

void WbRobot::exportNodeFields(WbVrmlWriter &writer) const {
  WbSolid::exportNodeFields(writer);
  if (writer.isX3d()) {
    if (findField("controller") && !controllerName().isEmpty()) {
      writer << " controller=";
      writer.writeLiteralString(controllerName());
    }
    if (findField("window") && !window().isEmpty()) {
      writer << " window=";
      writer.writeLiteralString(window());
    }
  }
}

const QString WbRobot::urdfName() const {
  return getUrdfPrefix() + QString("base_link");
}

int WbRobot::computeSimulationMode() {
  WbSimulationState *state = WbSimulationState::instance();
  switch (state->mode()) {
    case WbSimulationState::REALTIME:
      return WB_SUPERVISOR_SIMULATION_MODE_REAL_TIME;
    case WbSimulationState::FAST:
      return WB_SUPERVISOR_SIMULATION_MODE_FAST;
    default:
      return WB_SUPERVISOR_SIMULATION_MODE_PAUSE;
  }
}
