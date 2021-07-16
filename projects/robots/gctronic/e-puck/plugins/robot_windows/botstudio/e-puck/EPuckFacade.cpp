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

/*
 * Description:  Implementation of the EPuckFacade.hpp functions
 */

#include "EPuckFacade.hpp"

#include <webots/camera.h>
#include <webots/device.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <webots/motor.h>
#include <webots/robot.h>

#include <QtCore/QString>
#include <QtCore/QTextStream>

#include <cstdio>

static const int THRESHOLD = 50;

static bool isBlack(int value) {
  return (value < THRESHOLD);
}

static bool isWhite(int value) {
  return (value > THRESHOLD);
}

static int lineDetectionAlgorithm(const int values[], int size) {
  // if all black or all white pixels, return 0 (undetermined)
  bool allBlack = true;
  bool allWhite = true;
  int i;
  for (i = 0; i < size; i++) {
    if (isBlack(values[i]))
      allBlack = false;
    if (isWhite(values[i]))
      allWhite = false;
  }
  if (allBlack || allWhite)
    return 0;

  // if the most left part is black, return -100 (left line detected)
  if (isBlack(values[0]))
    return -100;

  // if the most right part is black, return 100 (right line detected)
  if (isBlack(values[size - 1]))
    return 100;

  // else, return the mean between the most left and the most right black pixel (between -100 and 100)
  int indexFirstBlack = size - 1;
  int indexLastBlack = 0;

  for (i = 0; i < size; i++) {
    if (i < indexFirstBlack && isBlack(values[i]))
      indexFirstBlack = i;
    if (i > indexLastBlack && isBlack(values[i]))
      indexLastBlack = i;
  }

  return (indexFirstBlack + indexLastBlack) * 100 / size - 100;
}

static bool gGroundSensorsExist = false;

bool EPuckFacade::groundSensorsExist() {
  return gGroundSensorsExist;
}

EPuckFacade::EPuckFacade() : mTimer(0.0) {
  gGroundSensorsExist = false;
  initDevices();
  initializeRobot();
  resetActuators();
}

EPuckFacade::~EPuckFacade() {
}

void EPuckFacade::initDevices() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
    QString s = QString("led") + QString::number(i);
    mLeds[i] = wb_robot_get_device(s.toLocal8Bit().constData());
    if (mLeds[i] == 0)
      printDeviceError(s);
  }

  for (int i = 0; i < NUMBER_OF_DISTANCE_SENSORS; i++) {
    QString s = QString("ps") + QString::number(i);
    mDistanceSensors[i] = wb_robot_get_device(s.toLocal8Bit().constData());
    if (mDistanceSensors[i] == 0)
      printDeviceError(s);
    mDistanceSensorValues[i] = 0;
  }

  QString s = "camera";
  mCamera = wb_robot_get_device(s.toLocal8Bit().constData());
  if (mCamera == 0)
    printDeviceError(s);
  mCameraValues = NULL;

  s = "left wheel motor";
  mMotors[LEFT] = wb_robot_get_device(s.toLocal8Bit().constData());
  if (mMotors[LEFT] == 0)
    printDeviceError(s);
  s = "right wheel motor";
  mMotors[RIGHT] = wb_robot_get_device(s.toLocal8Bit().constData());
  if (mMotors[RIGHT] == 0)
    printDeviceError(s);

  // init optional devices
  for (int i = 0; i < NUMBER_OF_GROUND_SENSORS; i++) {
    mGroundSensors[i] = 0;
    mGroundSensorValues[i] = 0;
  }

  WbNodeType deviceType;
  int groundSensorIndex, matchedItems;
  int numberOfDevices = wb_robot_get_number_of_devices();
  for (int index = 0; index < numberOfDevices; index++) {
    const WbDeviceTag tag = wb_robot_get_device_by_index(index);
    deviceType = wb_device_get_node_type(tag);
    if (deviceType == WB_NODE_DISTANCE_SENSOR) {
      const char *deviceName = wb_device_get_name(tag);
      matchedItems = sscanf(deviceName, "gs%d", &groundSensorIndex);
      if (matchedItems > 0) {
        // init ground sensors
        if (groundSensorIndex < NUMBER_OF_GROUND_SENSORS && mGroundSensors[groundSensorIndex] == 0) {
          mGroundSensors[groundSensorIndex] = tag;
          gGroundSensorsExist = true;
        } else
          printDeviceError(deviceName);
      }
    }
  }
}

void EPuckFacade::printDeviceError(const QString &deviceName) {
  QTextStream err(stderr);
  err << QObject::tr("Warning: the device %1 is not found. Please make sure your e-puck model has this device").arg(deviceName)
      << '\n';
}

double EPuckFacade::timer() const {
  return mTimer;
}

void EPuckFacade::initializeRobot() {
  int basicTimeStep = (int)wb_robot_get_basic_time_step();

  for (int i = 0; i < NUMBER_OF_DISTANCE_SENSORS; i++)
    wb_distance_sensor_enable(mDistanceSensors[i], basicTimeStep);

  for (int i = 0; i < NUMBER_OF_GROUND_SENSORS && mGroundSensors[i] != 0; i++)
    wb_distance_sensor_enable(mGroundSensors[i], basicTimeStep);

  wb_camera_enable(mCamera, basicTimeStep);

  mCameraSize[WIDTH] = wb_camera_get_width(mCamera);
  mCameraSize[HEIGHT] = wb_camera_get_height(mCamera);

  if (mCameraSize[HEIGHT] != 1) {
    QTextStream err(stderr);
    err << QObject::tr("Warning: the camera device of the e-puck should have an height of one.") << '\n';
  }
}

void EPuckFacade::update() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++)
    mLedValues[i] = wb_led_get(mLeds[i]);

  for (int i = 0; i < NUMBER_OF_DISTANCE_SENSORS; i++)
    mDistanceSensorValues[i] = wb_distance_sensor_get_value(mDistanceSensors[i]);

  for (int i = 0; i < NUMBER_OF_GROUND_SENSORS && mGroundSensors[i] != 0; i++)
    mGroundSensorValues[i] = wb_distance_sensor_get_value(mGroundSensors[i]);

  mCameraValues = wb_camera_get_image(mCamera);

  if (mMotors[LEFT] != 0)
    mSpeeds[LEFT] = wb_motor_get_velocity(mMotors[LEFT]);
  if (mMotors[RIGHT] != 0)
    mSpeeds[RIGHT] = wb_motor_get_velocity(mMotors[RIGHT]);

  mTimer = wb_robot_get_time();
}

void EPuckFacade::resetActuators() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++)
    mLedValues[i] = false;

  mSpeeds[LEFT] = 0.0;
  mSpeeds[RIGHT] = 0.0;
}

void EPuckFacade::sendActuatorCommands() {
  for (int i = 0; i < NUMBER_OF_LEDS; i++)
    wb_led_set(mLeds[i], mLedValues[i]);

  wb_motor_set_position(mMotors[LEFT], INFINITY);
  wb_motor_set_position(mMotors[RIGHT], INFINITY);
  wb_motor_set_velocity(mMotors[LEFT], mSpeeds[LEFT]);
  wb_motor_set_velocity(mMotors[RIGHT], mSpeeds[RIGHT]);
}

void EPuckFacade::stop() {
  resetActuators();
  sendActuatorCommands();
}

int EPuckFacade::rightSpeed() const {
  return static_cast<int>(mSpeeds[RIGHT] / 0.0628);  // 0.0628 = 10 * ( 2 * pi) / encoder_resolution
}

int EPuckFacade::leftSpeed() const {
  return static_cast<int>(mSpeeds[LEFT] / 0.0628);
}

bool EPuckFacade::ledValue(int id) const {
  return static_cast<bool>(mLedValues[id]);
}

int EPuckFacade::distanceSensorValue(int id) const {
  return static_cast<int>(mDistanceSensorValues[id]) * 100 / 4096;
}

int EPuckFacade::groundSensorValue(int id) const {
  return static_cast<int>(mGroundSensorValues[id]) * 100 / 1024;
}

int EPuckFacade::cameraValue() const {
  int heightToRead = mCameraSize[HEIGHT] - 1;
  int size = mCameraSize[WIDTH];
  if (heightToRead < 0)
    return 0;

  const unsigned char *image = wb_camera_get_image(mCamera);
  if (image == NULL)
    return 0;
  int values[size];
  for (int i = 0; i < size; i++)
    values[i] = wb_camera_image_get_gray(image, size, i, heightToRead);

  return lineDetectionAlgorithm(values, size);
}

void EPuckFacade::setLeftSpeed(int speed) {
  mSpeeds[LEFT] = static_cast<double>(speed) * 0.0628;
}

void EPuckFacade::setRightSpeed(int speed) {
  mSpeeds[RIGHT] = static_cast<double>(speed) * 0.0628;
}

void EPuckFacade::setSpeeds(int leftSpeed, int rightSpeed) {
  mSpeeds[LEFT] = static_cast<double>(leftSpeed) * 0.0628;
  mSpeeds[RIGHT] = static_cast<double>(rightSpeed) * 0.0628;
}

void EPuckFacade::setLedValue(int id, bool value) {
  mLedValues[id] = static_cast<int>(value);
}

bool EPuckFacade::notifyUseOfGroundSensor(int id) {
  if (mGroundSensors[id])
    return false;

  QString message = "The current e-puck model is not equipped with the ground sensors.\n\n"
                    "Insert the 'EPuck_GroundSensorsModule' PROTO node in the 'groundSensorsSlot' field "
                    "of the EPuck node in order to add them.";
  emit warning(message);
  return true;
}
