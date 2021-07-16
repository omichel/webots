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

/*******************************************************************************************************/
/* Description:  Swig interface which maps the OO C++ files into a python module called "controller"   */
/*******************************************************************************************************/

%module(threads=1) controller

%begin %{
#define SWIG_PYTHON_2_UNICODE
%}

%pythonbegin %{
import sys
import os
if os.name == 'nt' and sys.version_info >= (3, 8):  # we need to explicitly list the folders containing the DLLs
    webots_home = os.environ['WEBOTS_HOME']
    os.add_dll_directory(os.path.join(webots_home, 'lib', 'controller'))
    # MSYS2_HOME should be set by Webots or ~/.bash_profile
    # if not set, we are in the case of an extern controller and a regularly installed version of Webots
    msys64_root = os.environ['MSYS2_HOME'] if 'MSYS2_HOME' in os.environ else os.path.join(webots_home, 'msys64')
    cpp_folder = os.path.join(msys64_root, 'mingw64', 'bin', 'cpp')
    if not os.path.isdir(cpp_folder):  # development environment
        cpp_folder = os.path.join(msys64_root, 'mingw64', 'bin')
    os.add_dll_directory(cpp_folder)
%}

%{
#include <webots/Accelerometer.hpp>
#include <webots/Altimeter.hpp>
#include <webots/Brake.hpp>
#include <webots/Camera.hpp>
#include <webots/camera_recognition_object.h>
#include <webots/Compass.hpp>
#include <webots/Connector.hpp>
#include <webots/Device.hpp>
#include <webots/Display.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Emitter.hpp>
#include <webots/Field.hpp>
#include <webots/GPS.hpp>
#include <webots/Gyro.hpp>
#include <webots/InertialUnit.hpp>
#include <webots/ImageRef.hpp>
#include <webots/Joystick.hpp>
#include <webots/Keyboard.hpp>
#include <webots/LED.hpp>
#include <webots/Lidar.hpp>
#include <webots/lidar_point.h>
#include <webots/LightSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/Mouse.hpp>
#include <webots/Node.hpp>
#include <webots/Pen.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Radar.hpp>
#include <webots/radar_target.h>
#include <webots/RangeFinder.hpp>
#include <webots/Receiver.hpp>
#include <webots/Robot.hpp>
#include <webots/Skin.hpp>
#include <webots/Speaker.hpp>
#include <webots/Supervisor.hpp>
#include <webots/TouchSensor.hpp>
#include <webots/utils/Motion.hpp>

using namespace std;
%}

//----------------------------------------------------------------------------------------------
//  Miscellaneous - controller module's level
//----------------------------------------------------------------------------------------------

%thread webots::Robot::step(int duration);
%nothreadblock;

//handling std::string
%include "std_string.i"
%include "typemaps.i"

%rename ("__internalGetLookupTableSize") getLookupTableSize;

// manage double arrays
%typemap(out) const double * {
  int len = 3;
  const string test("$name");
  if (test == "getSFVec2f" || test == "getMFVec2f")
    len = 2;
  else if (test == "getSFRotation" || test == "getQuaternion")
    len = 4;
  else if (test == "getVelocity")
    len = 6;
  else if (test == "getOrientation" || test == "virtualRealityHeadsetGetOrientation")
    len = 9;
  else if (test == "getPose")
    len = 16;
  $result = PyList_New(len);
  for (int i = 0; i < len; ++i)
    PyList_SetItem($result, i, PyFloat_FromDouble($1[i]));
}
%typemap(out) const double *getLookupTable {
  const int len = arg1->getLookupTableSize()*3;
  $result = PyList_New(len);
  for (int i = 0; i < len; ++i)
    PyList_SetItem($result, i, PyFloat_FromDouble($1[i]));
}
%typemap(in) const double [ANY] {
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_TypeError, "in method '$name', expected 'PyList'\n");
    return NULL;
  }
  const int len = PyList_Size($input);
  $1 = (double*)malloc(len * sizeof(double));
  for (int i = 0; i < len; ++i)
    $1[i] = PyFloat_AsDouble(PyList_GetItem($input, i));
}
%typemap(freearg) const double [ANY] {
  free($1);
}
%typemap(in) const int * {
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_TypeError, "in method '$name', expected 'PyList'\n");
    return NULL;
  }
  const int len = PyList_Size($input);
  $1 = (int*)malloc(len * sizeof(int));
  for (int i = 0; i < len; ++i)
    $1[i] = PyInt_AsLong(PyList_GetItem($input, i));
}
%typemap(freearg) const int * {
  free($1);
}
//----------------------------------------------------------------------------------------------
//  ANSI Support
//----------------------------------------------------------------------------------------------

%pythoncode %{
class AnsiCodes(object):
    RESET = u'\u001b[0m'

    BOLD = u'\u001b[1m'
    UNDERLINE = u'\u001b[4m'

    BLACK_BACKGROUND = u'\u001b[40m'
    RED_BACKGROUND = u'\u001b[41m'
    GREEN_BACKGROUND = u'\u001b[42m'
    YELLOW_BACKGROUND = u'\u001b[43m'
    BLUE_BACKGROUND = u'\u001b[44m'
    MAGENTA_BACKGROUND = u'\u001b[45m'
    CYAN_BACKGROUND = u'\u001b[46m'
    WHITE_BACKGROUND = u'\u001b[47m'

    BLACK_FOREGROUND = u'\u001b[30m'
    RED_FOREGROUND = u'\u001b[31m'
    GREEN_FOREGROUND = u'\u001b[32m'
    YELLOW_FOREGROUND = u'\u001b[33m'
    BLUE_FOREGROUND = u'\u001b[34m'
    MAGENTA_FOREGROUND = u'\u001b[35m'
    CYAN_FOREGROUND = u'\u001b[36m'
    WHITE_FOREGROUND = u'\u001b[37m'

    CLEAR_SCREEN = u'\u001b[2J'
%}

//----------------------------------------------------------------------------------------------
//  Device
//----------------------------------------------------------------------------------------------

%include <webots/Device.hpp>

//----------------------------------------------------------------------------------------------
//  Accelerometer
//----------------------------------------------------------------------------------------------

%include <webots/Accelerometer.hpp>

//----------------------------------------------------------------------------------------------
//  Altimeter
//----------------------------------------------------------------------------------------------

%include <webots/Altimeter.hpp>

//----------------------------------------------------------------------------------------------
//  Brake
//----------------------------------------------------------------------------------------------

%include <webots/Brake.hpp>

%extend webots::Brake {
  %pythoncode %{
  def getMotor(self):
      try:
          return self.__motor
      except AttributeError:
          self.__motor = Robot.internalGetDeviceFromTag(self.getMotorTag())
          return self.__motor
  def getPositionSensor(self):
      try:
          return self.instance
      except AttributeError:
          self.__positionSensor = Robot.internalGetDeviceFromTag(self.getPositionSensorTag())
          return self.__positionSensor
  %}
}

//----------------------------------------------------------------------------------------------
//  Camera
//----------------------------------------------------------------------------------------------

%rename WbCameraRecognitionObject CameraRecognitionObject;



%include <webots/camera_recognition_object.h>

%extend WbCameraRecognitionObject {
  PyObject *get_position() {
    const double *position = $self->position;
    PyObject *ret = PyList_New(3);
    PyList_SetItem(ret, 0, PyFloat_FromDouble(position[0]));
    PyList_SetItem(ret, 1, PyFloat_FromDouble(position[1]));
    PyList_SetItem(ret, 2, PyFloat_FromDouble(position[2]));
    return ret;
  }
  PyObject *get_orientation() {
    const double *orientation = $self->orientation;
    PyObject *ret = PyList_New(4);
    PyList_SetItem(ret, 0, PyFloat_FromDouble(orientation[0]));
    PyList_SetItem(ret, 1, PyFloat_FromDouble(orientation[1]));
    PyList_SetItem(ret, 2, PyFloat_FromDouble(orientation[2]));
    PyList_SetItem(ret, 3, PyFloat_FromDouble(orientation[3]));
    return ret;
  }
  PyObject *get_size() {
    const double *size = $self->size;
    PyObject *ret = PyList_New(2);
    PyList_SetItem(ret, 0, PyFloat_FromDouble(size[0]));
    PyList_SetItem(ret, 1, PyFloat_FromDouble(size[1]));
    return ret;
  }
  PyObject *get_position_on_image() {
    const int *position_on_image = $self->position_on_image;
    PyObject *ret = PyList_New(2);
    PyList_SetItem(ret, 0, PyInt_FromLong(position_on_image[0]));
    PyList_SetItem(ret, 1, PyInt_FromLong(position_on_image[1]));
    return ret;
  }
  PyObject *get_size_on_image() {
    const int *size_on_image = $self->size_on_image;
    PyObject *ret = PyList_New(2);
    PyList_SetItem(ret, 0, PyInt_FromLong(size_on_image[0]));
    PyList_SetItem(ret, 1, PyInt_FromLong(size_on_image[1]));
    return ret;
  }
  PyObject *get_colors() {
    const double *colors = $self->colors;
    const int number_of_color = $self->number_of_colors;
    PyObject *ret = PyList_New(3 * number_of_color);
    for (int i = 0; i < 3 * number_of_color; ++i)
      PyList_SetItem(ret, i, PyFloat_FromDouble(colors[i]));
    return ret;
  }
  PyObject *get_id() {
    return PyInt_FromLong($self->id);
  }
  PyObject *get_number_of_colors() {
    return PyInt_FromLong($self->number_of_colors);
  }
  PyObject *get_model() {
    return PyBytes_FromStringAndSize($self->model, strlen($self->model));
  }
};


%typemap(out) unsigned char * {
  const int width = arg1->getWidth();
  const int height = arg1->getHeight();
  if ($1)
    $result = PyBytes_FromStringAndSize((const char*)$1, 4 * width * height);
  else
    $result = Py_None;

}

%extend webots::Camera {
  PyObject *getImageArray() {
    const unsigned char *im = $self->getImage();
    const int width = $self->getWidth();
    const int height = $self->getHeight();
    PyObject *ret = Py_None;
    if (im) {
      ret = PyList_New(width);
      for (int x = 0; x < width; ++x) {
        PyObject *dim2 = PyList_New(height);
        PyList_SetItem(ret, x, dim2);
        for (int y = 0; y < height; ++y) {
          PyObject *dim3 = PyList_New(3);
          PyList_SetItem(dim2, y, dim3);
          for (int ch = 0; ch < 3; ++ch)
            PyList_SetItem(dim3, ch, PyInt_FromLong((unsigned int)(im[4 * (x + y * width) + 2 - ch])));
        }
      }
    }
    return ret;
  }

  static PyObject *imageGetRed(PyObject *im, int width, int x, int y) {
    if (!PyString_Check(im)) {
      PyErr_SetString(PyExc_TypeError, "in method 'Camera_imageGetRed', argument 2 of type 'PyString'\n");
      return NULL;
    }
    const unsigned char *s = (unsigned char*)PyString_AsString(im);
    return PyInt_FromLong(s[4 * (y * width + x) + 2]);
  }

  static PyObject *imageGetGreen(PyObject *im, int width, int x, int y) {
    if (!PyString_Check(im)) {
      PyErr_SetString(PyExc_TypeError, "in method 'Camera_imageGetGreen', argument 2 of type 'PyString'\n");
      return NULL;
    }
    const unsigned char *s = (unsigned char*)PyString_AsString(im);
    return PyInt_FromLong(s[4 * (y * width + x) + 1]);
  }

  static PyObject *imageGetBlue(PyObject *im, int width, int x, int y) {
    if (!PyString_Check(im)) {
      PyErr_SetString(PyExc_TypeError, "in method 'Camera_imageGetBlue', argument 2 of type 'PyString'\n");
      return NULL;
    }
    const unsigned char *s = (unsigned char*)PyString_AsString(im);
    return PyInt_FromLong(s[4 * (y * width + x)]);
  }

  static PyObject *imageGetGray(PyObject *im, int width, int x, int y) {
    if (!PyString_Check(im)) {
      PyErr_SetString(PyExc_TypeError, "in method 'Camera_imageGetGrey', argument 2 of type 'PyString'\n");
      return NULL;
    }
    const unsigned char *s = (unsigned char*)PyString_AsString(im);
    return PyInt_FromLong((s[4 * (y * width + x)] + s[4 * (y * width + x) + 1] + s[4 * (y * width + x) + 2]) / 3);
  }

  static PyObject *imageGetGrey(PyObject *im, int width, int x, int y) {
    if (!PyString_Check(im)) {
      PyErr_SetString(PyExc_TypeError, "in method 'Camera_imageGetGrey', argument 2 of type 'PyString'\n");
      return NULL;
    }
    const unsigned char *s = (unsigned char*)PyString_AsString(im);
    return PyInt_FromLong((s[4 * (y * width + x)] + s[4 * (y * width + x) + 1] + s[4 * (y * width + x) + 2]) / 3);
  }

  webots::CameraRecognitionObject getRecognitionObject(int index) const {
    const webots::CameraRecognitionObject *objects = $self->getRecognitionObjects();
    return objects[index];
  }

  %pythoncode %{
  def getRecognitionObjects(self):
     ret = []
     for i in range(self.getRecognitionNumberOfObjects()):
       ret.append(self.getRecognitionObject(i))
     return ret
  %}

  PyObject *getRecognitionSegmentationImageArray() {
    const unsigned char *im = $self->getRecognitionSegmentationImage();
    const int width = $self->getWidth();
    const int height = $self->getHeight();
    PyObject *ret = Py_None;
    if (im) {
      ret = PyList_New(width);
      for (int x = 0; x < width; ++x) {
        PyObject *dim2 = PyList_New(height);
        PyList_SetItem(ret, x, dim2);
        for (int y = 0; y < height; ++y) {
          PyObject *dim3 = PyList_New(3);
          PyList_SetItem(dim2, y, dim3);
          for (int ch = 0; ch < 3; ++ch)
            PyList_SetItem(dim3, ch, PyInt_FromLong((unsigned int)(im[4 * (x + y * width) + 2 - ch])));
        }
      }
    }
    return ret;
  }
};

%include <webots/Camera.hpp>

%typemap(out) unsigned char *;

//----------------------------------------------------------------------------------------------
//  Compass
//----------------------------------------------------------------------------------------------

%include <webots/Compass.hpp>

//----------------------------------------------------------------------------------------------
//  Connector
//----------------------------------------------------------------------------------------------

%include <webots/Connector.hpp>

//----------------------------------------------------------------------------------------------
//  Display
//----------------------------------------------------------------------------------------------

%typemap(in) const void *(bool need_to_delete) {
  if (!PyList_Check($input) && !PyString_Check($input)) {
    PyErr_SetString(PyExc_TypeError, "expected 'PyList' or 'PyString'\n");
    return NULL;
  }
  if (PyList_Check($input)) {
    const int len1 = PyList_Size($input);
    PyObject *l2 = PyList_GetItem($input, 0);
    if (!PyList_Check(l2)) {
      PyErr_SetString(PyExc_TypeError, "expected 'PyList' of 'PyList'\n");
      return NULL;
    }
    const int len2 = PyList_Size(l2);
    PyObject *l3 = PyList_GetItem(l2, 0);
    if (!PyList_Check(l3)) {
      PyErr_SetString(PyExc_TypeError, "expected 'PyList' of 'PyList' of 'PyList'\n");
      return NULL;
    }
    const int len3 = PyList_Size(l3);
    $1 = (void *)malloc(len1 * len2 * len3 * sizeof(unsigned char));
    need_to_delete = true;
    for (int i = 0; i < len1; ++i)
      for (int j = 0; j < len2; ++j)
        for (int k = 0; k < len3; ++k)
          ((unsigned char *)$1)[(j * len1 * len3) + (i * len3) + k] = (unsigned char) PyInt_AsLong(PyList_GetItem(PyList_GetItem(PyList_GetItem($input, i), j), k));
  } else { // PyString case
    $1 = PyString_AsString($input);
    need_to_delete = false;
  }
}

%typemap(freearg) const void * {
  if (need_to_delete$argnum)
    free($1);
}

%rename (__internalImageNew) imageNew(int width, int height, const void *data, int format) const;
%rename (__internalDrawPolygon) drawPolygon(const int *x, const int *y, int size);
%rename (__internalFillPolygon) fillPolygon(const int *x, const int *y, int size);

%extend webots::Display {
  %pythoncode %{
    def imageNew(self, data, format, width=None, height=None):
      if isinstance(data, list):
        return self.__internalImageNew(len(data), len(data[0]), data, format)
      elif width is None or height is None:
        raise TypeError('imageNew : width and height must be given if data is not a list')
      else:
        return self.__internalImageNew(width, height, data, format)
    def drawPolygon(self, x, y):
      self.__internalDrawPolygon(x, y, min(len(x), len(y)))
    def fillPolygon(self, x, y):
      self.__internalFillPolygon(x, y, min(len(x), len(y)))
  %}
}

%include <webots/ImageRef.hpp>
%include <webots/Display.hpp>

//----------------------------------------------------------------------------------------------
//  Distance sensor
//----------------------------------------------------------------------------------------------

%include <webots/DistanceSensor.hpp>

//----------------------------------------------------------------------------------------------
//  Emitter
//----------------------------------------------------------------------------------------------

%typemap(in) (const void *data, int size) {
  $1 = PyString_AsString($input);
  $2 = PyString_Size($input);
}

%include <webots/Emitter.hpp>

%typemap(in) (const void *data, int size);

//----------------------------------------------------------------------------------------------
//  Field
//----------------------------------------------------------------------------------------------

%ignore webots::Field::findField(WbFieldRef ref);
%ignore webots::Field::cleanup();

%include <webots/Field.hpp>

//----------------------------------------------------------------------------------------------
//  GPS
//----------------------------------------------------------------------------------------------

%include <webots/GPS.hpp>

//----------------------------------------------------------------------------------------------
//  Gyro
//----------------------------------------------------------------------------------------------

%include <webots/Gyro.hpp>

//----------------------------------------------------------------------------------------------
//  InertialUnit
//----------------------------------------------------------------------------------------------

%include <webots/InertialUnit.hpp>

//----------------------------------------------------------------------------------------------
//  Joystick
//----------------------------------------------------------------------------------------------

%include <webots/Joystick.hpp>

//----------------------------------------------------------------------------------------------
//  Keyboard
//----------------------------------------------------------------------------------------------

%include <webots/Keyboard.hpp>

//----------------------------------------------------------------------------------------------
//  LED
//----------------------------------------------------------------------------------------------

%include <webots/LED.hpp>

//----------------------------------------------------------------------------------------------
//  Lidar
//----------------------------------------------------------------------------------------------

%rename WbLidarPoint LidarPoint;

%include <webots/lidar_point.h>

%typemap(out) float * {
  const int width = arg1->getHorizontalResolution();
  const int height = arg1->getNumberOfLayers();
  const string functionName("$name");
  int len;
  if (functionName == "getLayerRangeImage")
    len = width;
  else
    len = width * height;
  if ($1) {
    $result = PyList_New(len);
    for (int x = 0; x < len; ++x)
      PyList_SetItem($result, x, PyFloat_FromDouble($1[x]));
  } else
    $result = Py_None;
}

%ignore webots::Lidar::getPointCloud();
%ignore webots::Lidar::getLayerPointCloud();

%extend webots::Lidar {

  PyObject *__getPointCloudBuffer(int layer) const {
    const char *points = layer < 0 ? (const char *)$self->getPointCloud() : (const char *)$self->getLayerPointCloud(layer);
    const int numberOfPoints = layer < 0 ? $self->getNumberOfPoints() : $self->getHorizontalResolution();
    const int size = numberOfPoints * sizeof(WbLidarPoint);
    return PyBytes_FromStringAndSize(points, size);
  }

  PyObject* __getPointCloudList(int layer) const {
    const WbLidarPoint *rawPoints = layer < 0 ? $self->getPointCloud() : $self->getLayerPointCloud(layer);
    const int size = layer < 0 ? $self->getNumberOfPoints() : $self->getHorizontalResolution();

    PyObject *points = PyList_New(size);
    for (int i = 0; i < size; i++) {
      PyObject *value = SWIG_NewPointerObj(SWIG_as_voidptr(&rawPoints[i]), $descriptor(WbLidarPoint), 0);
      PyList_SetItem(points, i, value);
    }
    return points;
  }

  %pythoncode %{
  import sys

  def getPointCloud(self, data_type='list'):
    if data_type == 'list':
      return self.__getPointCloudList(-1)
    elif data_type == 'buffer':
      return self.__getPointCloudBuffer(-1)
    else:
      sys.stderr.write("Error: `data_type` cannot be `{}`! Supported values are 'list' and 'buffer'.\n".format(data_type))
      return None

  def getLayerPointCloud(self, layer, data_type='list'):
     if data_type == 'list':
       return self.__getPointCloudList(layer)
     elif data_type == 'buffer':
       return self.__getPointCloudBuffer(layer)
     else:
       sys.stderr.write("Error: `data_type` cannot be `{}`! Supported values are 'list' and 'buffer'.\n".format(data_type))
       return None
  %}

  PyObject *getRangeImageArray() {
    const float *im = $self->getRangeImage();
    const int width = $self->getHorizontalResolution();
    const int height = $self->getNumberOfLayers();
    PyObject *ret = Py_None;
    if (im) {
      ret = PyList_New(width);
      for (int x = 0; x < width; ++x) {
        PyObject *dim2 = PyList_New(height);
        PyList_SetItem(ret, x, dim2);
        for (int y = 0; y < height; ++y) {
          PyObject *v = PyFloat_FromDouble(im[x + y * width]);
          PyList_SetItem(dim2, y, v);
        }
      }
    }
    return ret;
  }
};

%include <webots/Lidar.hpp>

%typemap(out) float *;

//----------------------------------------------------------------------------------------------
//  LightSensor
//----------------------------------------------------------------------------------------------

%include <webots/LightSensor.hpp>

//----------------------------------------------------------------------------------------------
//  Motion
//----------------------------------------------------------------------------------------------

%include <webots/utils/Motion.hpp>

//----------------------------------------------------------------------------------------------
//  Motor
//----------------------------------------------------------------------------------------------

%include <webots/Motor.hpp>

%extend webots::Motor {
  %pythoncode %{
  def getBrake(self):
      try:
          return self.__brake
      except AttributeError:
          self.__brake = Robot.internalGetDeviceFromTag(self.getBrakeTag())
          return self.__brake
  def getPositionSensor(self):
      try:
          return self.__positionSensor
      except AttributeError:
          self.__positionSensor = Robot.internalGetDeviceFromTag(self.getPositionSensorTag())
          return self.__positionSensor
  %}
}

//----------------------------------------------------------------------------------------------
//  Mouse
//----------------------------------------------------------------------------------------------

%rename WbMouseState MouseState;

%include <webots/mouse_state.h>

%include <webots/Mouse.hpp>

//----------------------------------------------------------------------------------------------
//  Node
//----------------------------------------------------------------------------------------------

%rename WbContactPoint ContactPoint;

%include <webots/contact_point.h>

%extend WbContactPoint {
  PyObject *get_point() {
    const double *point = $self->point;
    PyObject *ret = PyList_New(3);
    PyList_SetItem(ret, 0, PyFloat_FromDouble(point[0]));
    PyList_SetItem(ret, 1, PyFloat_FromDouble(point[1]));
    PyList_SetItem(ret, 2, PyFloat_FromDouble(point[2]));
    return ret;
  }
  PyObject *get_node_id() {
    const double orientation = $self->node_id;
    return PyInt_FromLong(orientation);
  }

  %pythoncode %{
  @property
  def point(self):
      return self.get_point()
  %}
};

%ignore webots::Node::findNode(WbNodeRef ref);
%ignore webots::Node::cleanup();
%rename ("getContactPointsPrivate") webots::Node::getContactPoints;
%apply int *OUTPUT { int *size };

%extend webots::Node {
  %pythoncode %{
  def __eq__(self, other):
      if self is None and other is None:
          return True
      elif self is None or other is None:
          return False
      elif self.getId() == other.getId():
          return True
      return False

  def __ne__(self, other):
      return not self.__eq__(other)

  def getContactPoints(self, includeDescendants=False):
    point_data = self.getContactPointsPrivate(includeDescendants)
    if not point_data:
      return []
    ret = []
    points, size = point_data
    for i in range(size):
      ret.append(self.getContactPointFromList(points, i))
    return ret
  %}

  webots::ContactPoint* getContactPointFromList(ContactPoint* points, int index) const {
    return &points[index];
  }
};

%include <webots/Node.hpp>

//----------------------------------------------------------------------------------------------
//  Pen
//----------------------------------------------------------------------------------------------

%include <webots/Pen.hpp>

//----------------------------------------------------------------------------------------------
//  PositionSensor
//----------------------------------------------------------------------------------------------

%include <webots/PositionSensor.hpp>

%extend webots::PositionSensor {
  %pythoncode %{
  def getBrake(self):
      try:
          return self.__brake
      except AttributeError:
          self.__brake = Robot.internalGetDeviceFromTag(self.getBrakeTag())
          return self.__brake
  def getMotor(self):
      try:
          return self.__motor
      except AttributeError:
          self.__motor = Robot.internalGetDeviceFromTag(self.getMotorTag())
          return self.__motor
  %}
}

//----------------------------------------------------------------------------------------------
//  Radar
//----------------------------------------------------------------------------------------------

%rename WbRadarTarget RadarTarget;

%include <webots/radar_target.h>

%ignore webots::Lidar::getTargets();

%extend webots::Radar {
  webots::RadarTarget getTarget(int index) const {
    const webots::RadarTarget *targets = $self->getTargets();
    return targets[index];
  }

  %pythoncode %{
  def getTargets(self):
     ret = []
     for i in range(self.getNumberOfTargets()):
       ret.append(self.getTarget(i))
     return ret
  %}
};

%include <webots/Radar.hpp>

//----------------------------------------------------------------------------------------------
//  RangeFinder
//----------------------------------------------------------------------------------------------

%extend webots::RangeFinder {

  PyObject *__getRangeImageList() {
    const float *im = $self->getRangeImage();
    const int width = $self->getWidth();
    const int height = $self->getHeight();
    const int len = width * height;
    if (im) {
      PyObject *ret = PyList_New(len);
      for (int x = 0; x < len; ++x)
        PyList_SetItem(ret, x, PyFloat_FromDouble(im[x]));
      return ret;
    } else {
      return Py_None;
    }
  }

  PyObject *__getRangeImageBuffer() {
    const float *im = $self->getRangeImage();
    const char *im_bytes = (const char *)im;
    const int size = $self->getWidth() * $self->getHeight() * sizeof(float);
    return PyBytes_FromStringAndSize(im_bytes, size);
  }

  PyObject *getRangeImageArray() {
    const float *im = $self->getRangeImage();
    const int width = $self->getWidth();
    const int height = $self->getHeight();
    PyObject *ret = Py_None;
    if (im) {
      ret = PyList_New(width);
      for (int x = 0; x < width; ++x) {
        PyObject *dim2 = PyList_New(height);
        PyList_SetItem(ret, x, dim2);
        for (int y = 0; y < height; ++y) {
          PyObject *v = PyFloat_FromDouble(im[x + y * width]);
          PyList_SetItem(dim2, y, v);
        }
      }
    }
    return ret;
  }

  %pythoncode %{
    def getRangeImage(self, data_type='list'):
      if data_type == 'list':
        return self.__getRangeImageList()
      elif data_type == 'buffer':
        return self.__getRangeImageBuffer()
      else:
        print("Error: `data_type` cannot be `{}`! Supported values are 'list' and 'buffer'.".format(data_type), file=sys.stderr)
        return None
  %}

  static PyObject *rangeImageGetValue(PyObject *im, double minRange, double maxRange, int width, int x, int y) {
    if (!PyList_Check(im)) {
      PyErr_SetString(PyExc_TypeError, "in method 'RangeFinder_rangeImageGetValue', argument 2 of type 'PyList'\n");
      return NULL;
    }
    PyObject *value = PyList_GetItem(im, y * width + x);
    if (!PyFloat_Check(value)) {
      PyErr_SetString(PyExc_TypeError, "in method 'RangeFinder_rangeImageGetValue', argument 2 of type 'PyList' of 'PyFloat'\n");
      return NULL;
    }
    fprintf(stderr, "Warning: RangeFinder.rangeImageGetValue is deprecated, please use RangeFinder.rangeImageGetDepth instead\n");
    // inform Python runtime that the object is used somewhere else
    // this prevents crashes when updating the range image internal list
    Py_INCREF(value);
    return value;
  }

  static PyObject *rangeImageGetDepth(PyObject *im, int width, int x, int y) {
    if (!PyList_Check(im)) {
      PyErr_SetString(PyExc_TypeError, "in method 'RangeFinder_rangeImageGetDepth', argument 2 of type 'PyList'\n");
      return NULL;
    }
    PyObject *value = PyList_GetItem(im, y * width + x);
    if (!PyFloat_Check(value)) {
      PyErr_SetString(PyExc_TypeError, "in method 'RangeFinder_rangeImageGetDepth', argument 2 of type 'PyList' of 'PyFloat'\n");
      return NULL;
    }
    // inform Python runtime that the object is used somewhere else
    // this prevents crashes when updating the range image internal list
    Py_INCREF(value);
    return value;
  }
};

%include <webots/RangeFinder.hpp>

%typemap(out) float *;

//----------------------------------------------------------------------------------------------
//  Receiver
//----------------------------------------------------------------------------------------------

%typemap(out) const void * {
  $result = PyBytes_FromStringAndSize((const char*) $1, arg1->getDataSize());
}

%include <webots/Receiver.hpp>

%typemap(out) const void *;

//----------------------------------------------------------------------------------------------
//  Skin
//----------------------------------------------------------------------------------------------

%include <webots/Skin.hpp>

//----------------------------------------------------------------------------------------------
//  Speaker
//----------------------------------------------------------------------------------------------

%include <webots/Speaker.hpp>

//----------------------------------------------------------------------------------------------
//  TouchSensor
//----------------------------------------------------------------------------------------------

%include <webots/TouchSensor.hpp>

//----------------------------------------------------------------------------------------------
//  Robot
//----------------------------------------------------------------------------------------------

%ignore webots::Robot::getAccelerometer(const std::string &name);
%ignore webots::Robot::getAltimeter(const std::string &name);
%ignore webots::Robot::getBrake(const std::string &name);
%ignore webots::Robot::getCamera(const std::string &name);
%ignore webots::Robot::getCompass(const std::string &name);
%ignore webots::Robot::getConnector(const std::string &name);
%ignore webots::Robot::getDisplay(const std::string &name);
%ignore webots::Robot::getDistanceSensor(const std::string &name);
%ignore webots::Robot::getEmitter(const std::string &name);
%ignore webots::Robot::getGPS(const std::string &name);
%ignore webots::Robot::getGyro(const std::string &name);
%ignore webots::Robot::getInertialUnit(const std::string &name);
%ignore webots::Robot::getJoystick();
%ignore webots::Robot::getKeyboard();
%ignore webots::Robot::getLED(const std::string &name);
%ignore webots::Robot::getLidar(const std::string &name);
%ignore webots::Robot::getLightSensor(const std::string &name);
%ignore webots::Robot::getMotor(const std::string &name);
%ignore webots::Robot::getMouse();
%ignore webots::Robot::getPen(const std::string &name);
%ignore webots::Robot::getPositionSensor(const std::string &name);
%ignore webots::Robot::getRadar(const std::string &name);
%ignore webots::Robot::getRangeFinder(const std::string &name);
%ignore webots::Robot::getReceiver(const std::string &name);
%ignore webots::Robot::getSkin(const std::string &name);
%ignore webots::Robot::getSpeaker(const std::string &name);
%ignore webots::Robot::getTouchSensor(const std::string &name);
%ignore webots::Robot::windowCustomFunction(void *arg);
%ignore webots::Robot::wwiSend(const char *data, int size);
%ignore webots::Robot::wwiReceive(int *size);

%rename ("__internalGetDevice") webots::Robot::getDevice;
%rename ("__internalGetDeviceByIndex") webots::Robot::getDeviceByIndex;
%rename ("__internalGetDeviceTypeFromTag") webots::Robot::getDeviceTypeFromTag;
%rename ("__internalGetDeviceNameFromTag") webots::Robot::getDeviceNameFromTag;
%rename ("__internalGetDeviceTagFromIndex") webots::Robot::getDeviceTagFromIndex;
%rename ("__internalGetDeviceTagFromName") webots::Robot::getDeviceTagFromName;

%extend webots::Robot {
  %pythoncode %{
    __devices = []
    joystick = Joystick()
    keyboard = Keyboard()
    mouse = Mouse()
    import sys
    if sys.version_info[0] < 3:
      sys.stderr.write("DEPRECATION: Python 2.7 will reach the end of its life on January 1st, 2020. Please upgrade your Python as Python 2.7 won't be maintained after that date. A future version of Webots will drop support for Python 2.7.\n")
    def createAccelerometer(self, name):
      return Accelerometer(name)
    def getAccelerometer(self, name):
      sys.stderr.write("DEPRECATION: Robot.getAccelerometer is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.ACCELEROMETER):
        return None
      return self.__getOrCreateDevice(tag)
    def createAltimeter(self, name):
      return Altimeter(name)
    def getAltimeter(self, name):
      sys.stderr.write("DEPRECATION: Robot.getAltimeter is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.ALTIMETER):
        return None
      return self.__getOrCreateDevice(tag)
    def createBrake(self, name):
      return Brake(name)
    def getBrake(self, name):
      sys.stderr.write("DEPRECATION: Robot.getBrake is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.BRAKE):
        return None
      return self.__getOrCreateDevice(tag)
    def createCamera(self, name):
      return Camera(name)
    def getCamera(self, name):
      sys.stderr.write("DEPRECATION: Robot.getCamera is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.CAMERA):
        return None
      return self.__getOrCreateDevice(tag)
    def createCompass(self, name):
      return Compass(name)
    def getCompass(self, name):
      sys.stderr.write("DEPRECATION: Robot.getCompass is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.COMPASS):
        return None
      return self.__getOrCreateDevice(tag)
    def createConnector(self, name):
      return Connector(name)
    def getConnector(self, name):
      sys.stderr.write("DEPRECATION: Robot.getConnector is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.CONNECTOR):
        return None
      return self.__getOrCreateDevice(tag)
    def createDisplay(self, name):
      return Display(name)
    def getDisplay(self, name):
      sys.stderr.write("DEPRECATION: Robot.getDisplay is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.DISPLAY):
        return None
      return self.__getOrCreateDevice(tag)
    def createDistanceSensor(self, name):
      return DistanceSensor(name)
    def getDistanceSensor(self, name):
      sys.stderr.write("DEPRECATION: Robot.getDistanceSensor is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.DISTANCE_SENSOR):
        return None
      return self.__getOrCreateDevice(tag)
    def createEmitter(self, name):
      return Emitter(name)
    def getEmitter(self, name):
      sys.stderr.write("DEPRECATION: Robot.getEmitter is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.EMITTER):
        return None
      return self.__getOrCreateDevice(tag)
    def createGPS(self, name):
      return GPS(name)
    def getGPS(self, name):
      sys.stderr.write("DEPRECATION: Robot.getGPS is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.GPS):
        return None
      return self.__getOrCreateDevice(tag)
    def createGyro(self, name):
      return Gyro(name)
    def getGyro(self, name):
      sys.stderr.write("DEPRECATION: Robot.getGyro is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.GYRO):
        return None
      return self.__getOrCreateDevice(tag)
    def createInertialUnit(self, name):
      return InertialUnit(name)
    def getInertialUnit(self, name):
      sys.stderr.write("DEPRECATION: Robot.getInertialUnit is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.INERTIAL_UNIT):
        return None
      return self.__getOrCreateDevice(tag)
    def getJoystick(self):
      return self.joystick
    def getKeyboard(self):
      return self.keyboard
    def createLED(self, name):
      return LED(name)
    def getLED(self, name):
      sys.stderr.write("DEPRECATION: Robot.getLED is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.LED):
        return None
      return self.__getOrCreateDevice(tag)
    def createLidar(self, name):
      return Lidar(name)
    def getLidar(self, name):
      sys.stderr.write("DEPRECATION: Robot.getLidar is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.LIDAR):
        return None
      return self.__getOrCreateDevice(tag)
    def createLightSensor(self, name):
      return LightSensor(name)
    def getLightSensor(self, name):
      sys.stderr.write("DEPRECATION: Robot.getLightSensor is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.LIGHT_SENSOR):
        return None
      return self.__getOrCreateDevice(tag)
    def createMotor(self, name):
      return Motor(name)
    def getMotor(self, name):
      sys.stderr.write("DEPRECATION: Robot.getMotor is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.LINEAR_MOTOR) and not Device.hasType(tag, Node.ROTATIONAL_MOTOR):
        return None
      return self.__getOrCreateDevice(tag)
    def getMouse(self):
      return self.mouse
    def createPen(self, name):
      return Pen(name)
    def getPen(self, name):
      sys.stderr.write("DEPRECATION: Robot.getPen is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.PEN):
        return None
      return self.__getOrCreateDevice(tag)
    def createPositionSensor(self, name):
      return PositionSensor(name)
    def getPositionSensor(self, name):
      sys.stderr.write("DEPRECATION: Robot.getPositionSensor is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.POSITION_SENSOR):
        return None
      return self.__getOrCreateDevice(tag)
    def createRadar(self, name):
      return Radar(name)
    def getRadar(self, name):
      sys.stderr.write("DEPRECATION: Robot.getRadar is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.RADAR):
        return None
      return self.__getOrCreateDevice(tag)
    def createRangeFinder(self, name):
      return RangeFinder(name)
    def getRangeFinder(self, name):
      sys.stderr.write("DEPRECATION: Robot.getRangeFinder is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.RANGE_FINDER):
        return None
      return self.__getOrCreateDevice(tag)
    def createReceiver(self, name):
      return Receiver(name)
    def getReceiver(self, name):
      sys.stderr.write("DEPRECATION: Robot.getReceiver is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.RECEIVER):
        return None
      return self.__getOrCreateDevice(tag)
    def createSkin(self, name):
      return Skin(name)
    def getSkin(self, name):
      sys.stderr.write("DEPRECATION: Robot.getSkin is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.SKIN):
        return None
      return self.__getOrCreateDevice(tag)
    def createSpeaker(self, name):
      return Speaker(name)
    def getSpeaker(self, name):
      sys.stderr.write("DEPRECATION: Robot.getSpeaker is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.SPEAKER):
        return None
      return self.__getOrCreateDevice(tag)
    def createTouchSensor(self, name):
      return TouchSensor(name)
    def getTouchSensor(self, name):
      sys.stderr.write("DEPRECATION: Robot.getTouchSensor is deprecated, please use Robot.getDevice instead.\n")
      tag = self.__internalGetDeviceTagFromName(name)
      if not Device.hasType(tag, Node.TOUCH_SENSOR):
        return None
      return self.__getOrCreateDevice(tag)
    def getDeviceByIndex(self, index):
      tag = self.__internalGetDeviceTagFromIndex(index)
      return self.__getOrCreateDevice(tag)
    def getDevice(self, name):
      tag = self.__internalGetDeviceTagFromName(name)
      return self.__getOrCreateDevice(tag)
    @staticmethod
    def internalGetDeviceFromTag(tag):
      if tag == 0:
          return None
      size = len(Robot.__devices)
      if size == 0 or tag >= size:
          return None
      return Robot.__devices[tag]
    def __getOrCreateDevice(self, tag):
      if tag == 0:
          return None
      count = self.getNumberOfDevices()
      size = len(Robot.__devices)
      # if new devices have been added, then count is greater than size
      # deleted devices are not removed from the C API list and don't affect the number of devices
      if size == count + 1 and size > 0 and tag < size:
          return Robot.__devices[tag]

      # (re-)initialize Robot.__devices list
      if tag > count:
          return None
      Robot.__devices = [None] * (count + 1)
      for i in range(0, count):
          otherTag = self.__internalGetDeviceTagFromIndex(i)
          name = self.__internalGetDeviceNameFromTag(otherTag)
          nodeType = self.__internalGetDeviceTypeFromTag(otherTag)
          if nodeType == Node.ACCELEROMETER:
              Robot.__devices[otherTag] = self.createAccelerometer(name)
          elif nodeType == Node.ALTIMETER:
              Robot.__devices[otherTag] = self.createAltimeter(name)
          elif nodeType == Node.BRAKE:
              Robot.__devices[otherTag] = self.createBrake(name)
          elif nodeType == Node.CAMERA:
              Robot.__devices[otherTag] = self.createCamera(name)
          elif nodeType == Node.COMPASS:
              Robot.__devices[otherTag] = self.createCompass(name)
          elif nodeType == Node.CONNECTOR:
              Robot.__devices[otherTag] = self.createConnector(name)
          elif nodeType == Node.DISPLAY:
              Robot.__devices[otherTag] = self.createDisplay(name)
          elif nodeType == Node.DISTANCE_SENSOR:
              Robot.__devices[otherTag] = self.createDistanceSensor(name)
          elif nodeType == Node.EMITTER:
              Robot.__devices[otherTag] = self.createEmitter(name)
          elif nodeType == Node.GPS:
              Robot.__devices[otherTag] = self.createGPS(name)
          elif nodeType == Node.GYRO:
              Robot.__devices[otherTag] = self.createGyro(name)
          elif nodeType == Node.INERTIAL_UNIT:
              Robot.__devices[otherTag] = self.createInertialUnit(name)
          elif nodeType == Node.LED:
              Robot.__devices[otherTag] = self.createLED(name)
          elif nodeType == Node.LIDAR:
              Robot.__devices[otherTag] = self.createLidar(name)
          elif nodeType == Node.LIGHT_SENSOR:
              Robot.__devices[otherTag] = self.createLightSensor(name)
          elif nodeType == Node.LINEAR_MOTOR or nodeType == Node.ROTATIONAL_MOTOR:
              Robot.__devices[otherTag] = self.createMotor(name)
          elif nodeType == Node.PEN:
              Robot.__devices[otherTag] = self.createPen(name)
          elif nodeType == Node.POSITION_SENSOR:
              Robot.__devices[otherTag] = self.createPositionSensor(name)
          elif nodeType == Node.RADAR:
              Robot.__devices[otherTag] = self.createRadar(name)
          elif nodeType == Node.RANGE_FINDER:
              Robot.__devices[otherTag] = self.createRangeFinder(name)
          elif nodeType == Node.RECEIVER:
              Robot.__devices[otherTag] = self.createReceiver(name)
          elif nodeType == Node.SPEAKER:
              Robot.__devices[otherTag] = self.createSpeaker(name)
          elif nodeType == Node.TOUCH_SENSOR:
              Robot.__devices[otherTag] = self.createTouchSensor(name)
      return Robot.__devices[tag]
  %}
}

%include <webots/Robot.hpp>

//----------------------------------------------------------------------------------------------
//  Supervisor
//----------------------------------------------------------------------------------------------

%rename ("__internalGetFromDeviceTag") getFromDeviceTag;

%include <webots/Supervisor.hpp>
