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

// Description:   main file of Ros controller
#include "Ros.hpp"

#include "RosAccelerometer.hpp"
#include "RosBatterySensor.hpp"
#include "RosBrake.hpp"
#include "RosCamera.hpp"
#include "RosCompass.hpp"
#include "RosConnector.hpp"
#include "RosDisplay.hpp"
#include "RosDistanceSensor.hpp"
#include "RosEmitter.hpp"
#include "RosGPS.hpp"
#include "RosGyro.hpp"
#include "RosInertialUnit.hpp"
#include "RosJoystick.hpp"
#include "RosKeyboard.hpp"
#include "RosLED.hpp"
#include "RosLidar.hpp"
#include "RosLightSensor.hpp"
#include "RosMotor.hpp"
#include "RosPen.hpp"
#include "RosPositionSensor.hpp"
#include "RosRadar.hpp"
#include "RosRangeFinder.hpp"
#include "RosReceiver.hpp"
#include "RosSensor.hpp"
#include "RosSkin.hpp"
#include "RosSpeaker.hpp"
#include "RosSupervisor.hpp"
#include "RosTouchSensor.hpp"
#include "highlevel/RosControl.hpp"

#include <webots/Node.hpp>
#include <webots/Supervisor.hpp>

#include <rosgraph_msgs/Clock.h>

#include <algorithm>
#include <ctime>
#include "ros/master.h"
#include "std_msgs/String.h"

// IP resolution includes
#ifdef _WIN32
#include <TlHelp32.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif

using namespace std;

Ros::Ros() :
  mRobot(NULL),
  mRosSupervisor(NULL),
  mRosJoystick(NULL),
  mRosKeyboard(NULL),
  mNodeHandle(NULL),
  mStepSize(1),
  mStep(0),
  mEnd(false),
  mShouldPublishClock(false),
  mIsSynchronized(false),
  mUseWebotsSimTime(false),
  mAutoPublish(false),
  mUseRosControl(false),
  mRobotDescriptionPrefix(""),
  mSetRobotDescription(false),
  mRosControl(NULL) {
}

Ros::~Ros() {
  mNamePublisher.shutdown();
  mTimeStepService.shutdown();
  mWaitForUserInputEventService.shutdown();
  mGetTimeService.shutdown();
  mGetModelService.shutdown();
  mGetUrdfService.shutdown();
  mGetDataService.shutdown();
  mSetDataService.shutdown();
  mGetModeService.shutdown();
  mGetSupervisorService.shutdown();
  mGetSynchronizationService.shutdown();
  mGetProjectPathService.shutdown();
  mGetWorldPathService.shutdown();
  mGetBasicTimeStepService.shutdown();
  mGetNumberOfDevicesService.shutdown();
  mGetTypeService.shutdown();
  mSetModeService.shutdown();
  mWwiReceiveTextService.shutdown();
  mWwiSendTextService.shutdown();

  ros::shutdown();
  delete mRobot;
  delete mRosControl;
  for (unsigned int i = 0; i < mDeviceList.size(); i++)
    delete mDeviceList[i];
  delete mRosJoystick;
  delete mRosKeyboard;
}

void Ros::launchRos(int argc, char **argv) {
  std_msgs::String robotName;
  setupRobot();
  fixName();
  bool rosMasterUriSet = false;

  mStepSize = mRobot->getBasicTimeStep();

  for (int i = 1; i < argc; ++i) {
    const char masterUri[] = "--ROS_MASTER_URI=";
    const char name[] = "--name=";
    if (strncmp(argv[i], masterUri, sizeof(masterUri) - 1) == 0) {
      char address[64];
      strncpy(address, argv[i] + sizeof(masterUri) - 1, strlen(argv[i]) - strlen(masterUri) + 1);
#ifdef _WIN32
      _putenv_s("ROS_MASTER_URI", address);
#else
      setenv("ROS_MASTER_URI", address, 0);
#endif
      rosMasterUriSet = true;
    } else if (strncmp(argv[i], name, sizeof(name) - 1) == 0) {
      char robot_name[64];
      strncpy(robot_name, argv[i] + sizeof(name) - 1, strlen(argv[i]) - strlen(name) + 1);
      mRobotName = string(robot_name);
    } else if (strcmp(argv[i], "--clock") == 0)
      mShouldPublishClock = true;
    else if (strcmp(argv[i], "--synchronize") == 0)
      mIsSynchronized = true;
    else if (strcmp(argv[i], "--use-sim-time") == 0)
      mUseWebotsSimTime = true;
    else if (strcmp(argv[i], "--use-ros-control") == 0)
      mUseRosControl = true;
    else if (strcmp(argv[i], "--auto-publish") == 0)
      mAutoPublish = true;
    else if (std::string(argv[i]).rfind("--robot-description") == 0) {
      const std::string argument = std::string(argv[i]);
      const size_t valueStart = std::max(argument.find("="), argument.find(" "));
      mRobotDescriptionPrefix = (valueStart == std::string::npos) ? "" : argument.substr(valueStart + 1);
      mSetRobotDescription = true;
    } else
      ROS_ERROR("ERROR: unkown argument %s.", argv[i]);
  }

  if (!rosMasterUriSet) {
    if (getenv("ROS_MASTER_URI") == NULL) {
#ifdef _WIN32
      _putenv_s("ROS_MASTER_URI", "http://localhost:11311");
#else
      setenv("ROS_MASTER_URI", "http://localhost:11311", 0);
#endif
    }
  }

  ROS_INFO("Robot's unique name is %s.", mRobotName.c_str());
  ros::init(argc, argv, mRobotName);

  if (!ros::master::check()) {
    ROS_FATAL("Failed to contact master at %s. Please start ROS master and restart this controller.", getenv("ROS_MASTER_URI"));
    exit(EXIT_SUCCESS);
  }

  mNodeHandle = new ros::NodeHandle;
  ROS_INFO("The controller is now connected to the ROS master.");

  mNamePublisher = mNodeHandle->advertise<std_msgs::String>("model_name", 1, true);
  robotName.data = mRobotName;
  mNamePublisher.publish(robotName);

  mTimeStepService = mNodeHandle->advertiseService(mRobotName + "/robot/time_step", &Ros::timeStepCallback, this);
  mWaitForUserInputEventService =
    mNodeHandle->advertiseService(mRobotName + "/robot/wait_for_user_input_event", &Ros::waitForUserInputEventCallback, this);
  mGetTimeService = mNodeHandle->advertiseService(mRobotName + "/robot/get_time", &Ros::getTimeCallback, this);
  mGetModelService = mNodeHandle->advertiseService(mRobotName + "/robot/get_model", &Ros::getModelCallback, this);
  mGetUrdfService = mNodeHandle->advertiseService(mRobotName + "/robot/get_urdf", &Ros::getUrdfCallback, this);
  mGetDataService = mNodeHandle->advertiseService(mRobotName + "/robot/get_data", &Ros::getDataCallback, this);
  mSetDataService = mNodeHandle->advertiseService(mRobotName + "/robot/set_data", &Ros::setDataCallback, this);
  mGetCustomDataService =
    mNodeHandle->advertiseService(mRobotName + "/robot/get_custom_data", &Ros::getCustomDataCallback, this);
  mSetCustomDataService =
    mNodeHandle->advertiseService(mRobotName + "/robot/set_custom_data", &Ros::setCustomDataCallback, this);
  mGetModeService = mNodeHandle->advertiseService(mRobotName + "/robot/get_mode", &Ros::getModeCallback, this);
  mGetSupervisorService =
    mNodeHandle->advertiseService(mRobotName + "/robot/get_supervisor", &Ros::getSupervisorCallback, this);
  mGetSynchronizationService =
    mNodeHandle->advertiseService(mRobotName + "/robot/get_synchronization", &Ros::getSynchronizationCallback, this);
  mGetProjectPathService =
    mNodeHandle->advertiseService(mRobotName + "/robot/get_project_path", &Ros::getProjectPathCallback, this);
  mGetWorldPathService = mNodeHandle->advertiseService(mRobotName + "/robot/get_world_path", &Ros::getWorldPathCallback, this);
  mGetBasicTimeStepService =
    mNodeHandle->advertiseService(mRobotName + "/robot/get_basic_time_step", &Ros::getBasicTimeStepCallback, this);
  mGetNumberOfDevicesService =
    mNodeHandle->advertiseService(mRobotName + "/robot/get_number_of_devices", &Ros::getNumberOfDevicesCallback, this);
  mGetTypeService = mNodeHandle->advertiseService(mRobotName + "/robot/get_type", &Ros::getTypeCallback, this);
  mSetModeService = mNodeHandle->advertiseService(mRobotName + "/robot/set_mode", &Ros::setModeCallback, this);
  mWwiReceiveTextService =
    mNodeHandle->advertiseService(mRobotName + "/robot/wwi_receive_text", &Ros::wwiReceiveTextCallback, this);
  mWwiSendTextService = mNodeHandle->advertiseService(mRobotName + "/robot/wwi_send_text", &Ros::wwiSendTextCallback, this);

  if (mShouldPublishClock)
    mClockPublisher = mNodeHandle->advertise<rosgraph_msgs::Clock>("clock", 1);

  if (mRobot->getSupervisor())
    mRosSupervisor = new RosSupervisor(this, static_cast<Supervisor *>(mRobot));
  mRosJoystick = new RosJoystick(mRobot->getJoystick(), this);
  mRosKeyboard = new RosKeyboard(mRobot->getKeyboard(), this);
  setRosDevices(NULL, 0);

  bool useSimTime;
  if (mUseWebotsSimTime && mNodeHandle->getParam("/use_sim_time", useSimTime))
    mUseWebotsSimTime = useSimTime;
}

void Ros::setupRobot() {
  mRobot = new Supervisor();
}

Device *Ros::getDevice(const std::string &name) {
  for (unsigned int i = 0; i < mDeviceList.size(); ++i) {
    if ((mDeviceList[i])->deviceName() == name)
      return (mDeviceList[i])->device();
  }
  return NULL;
}

std::string Ros::fixedNameString(const std::string &name) {
  std::string fixedName = name;
  std::replace(fixedName.begin(), fixedName.end(), '-', '_');
  std::replace(fixedName.begin(), fixedName.end(), '.', '_');
  std::replace(fixedName.begin(), fixedName.end(), ' ', '_');
  std::replace(fixedName.begin(), fixedName.end(), ')', '_');
  std::replace(fixedName.begin(), fixedName.end(), '(', '_');
  return fixedName;
}

// create the unique name identifier of the controller that can be seen on ros network
// and used by other nodes to communicate with him
void Ros::fixName() {
  string webotsPID;
  string webotsHostname;
  ostringstream s;

// retrieve Webots' PID
#ifdef _WIN32
  HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = {0};
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(h, &pe)) {
    while (Process32Next(h, &pe) && s.str() == "") {
      if (!strcmp(pe.szExeFile, "ros.exe"))
        s << pe.th32ParentProcessID;
    }
  }
  CloseHandle(h);
#else
  s << getppid();
#endif
  webotsPID = s.str();

  // retrieve local hostname
  char hostname[256];
  gethostname(hostname, 256);
  webotsHostname = hostname;

  mRobotName = mRobot->getName();
  mRobotName += '_' + webotsPID + '_' + webotsHostname;
  // remove unhautorized symbols ('-', ' ' and '.') for ROS
  mRobotName = Ros::fixedNameString(mRobotName);
}

// runs accros the list of devices availables and creates the corresponding RosDevices.
// also stores pointers to sensors to be able to call their publishValues function at each step
void Ros::setRosDevices(const char **hiddenDevices, int numberHiddenDevices) {
  int nDevices = mRobot->getNumberOfDevices();
  for (int i = 0; i < nDevices; i++) {
    Device *tempDevice = mRobot->getDeviceByIndex(i);
    if (hiddenDevices) {
      bool hidden = false;
      for (int j = 0; j < numberHiddenDevices; ++j) {
        if (strcmp(hiddenDevices[j], tempDevice->getName().c_str()) == 0)
          hidden = true;
      }
      if (hidden)
        continue;
    }

    const unsigned int previousDevicesCount = mDeviceList.size();
    switch (tempDevice->getNodeType()) {
      case Node::ACCELEROMETER:
        mSensorList.push_back(static_cast<RosSensor *>(new RosAccelerometer(dynamic_cast<Accelerometer *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::BRAKE:
        mDeviceList.push_back(static_cast<RosDevice *>(new RosBrake(dynamic_cast<Brake *>(tempDevice), this)));
        break;
      case Node::CAMERA:
        mSensorList.push_back(static_cast<RosSensor *>(new RosCamera(dynamic_cast<Camera *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::COMPASS:
        mSensorList.push_back(static_cast<RosSensor *>(new RosCompass(dynamic_cast<Compass *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::CONNECTOR:
        mSensorList.push_back(static_cast<RosSensor *>(new RosConnector(dynamic_cast<Connector *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::DISPLAY:
        mDeviceList.push_back(static_cast<RosDevice *>(new RosDisplay(dynamic_cast<Display *>(tempDevice), this)));
        break;
      case Node::DISTANCE_SENSOR:
        mSensorList.push_back(
          static_cast<RosSensor *>(new RosDistanceSensor(dynamic_cast<DistanceSensor *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::EMITTER:
        mDeviceList.push_back(static_cast<RosDevice *>(new RosEmitter(dynamic_cast<Emitter *>(tempDevice), this)));
        break;
      case Node::GPS:
        mSensorList.push_back(static_cast<RosSensor *>(new RosGPS(dynamic_cast<GPS *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::GYRO:
        mSensorList.push_back(static_cast<RosSensor *>(new RosGyro(dynamic_cast<Gyro *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::INERTIAL_UNIT:
        mSensorList.push_back(static_cast<RosSensor *>(new RosInertialUnit(dynamic_cast<InertialUnit *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::LED:
        mDeviceList.push_back(static_cast<RosDevice *>(new RosLED(dynamic_cast<LED *>(tempDevice), this)));
        break;
      case Node::LIDAR:
        mSensorList.push_back(static_cast<RosSensor *>(new RosLidar(dynamic_cast<Lidar *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::LIGHT_SENSOR:
        mSensorList.push_back(static_cast<RosSensor *>(new RosLightSensor(dynamic_cast<LightSensor *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::LINEAR_MOTOR: {
        RosMotor *tempLinearMotor = new RosMotor(dynamic_cast<Motor *>(tempDevice), this);
        mDeviceList.push_back(static_cast<RosDevice *>(tempLinearMotor));
        mSensorList.push_back(static_cast<RosSensor *>(tempLinearMotor->mForceFeedbackSensor));
        break;
      }
      case Node::ROTATIONAL_MOTOR: {
        RosMotor *tempRotationalMotor = new RosMotor(dynamic_cast<Motor *>(tempDevice), this);
        mDeviceList.push_back(static_cast<RosDevice *>(tempRotationalMotor));
        mSensorList.push_back(static_cast<RosSensor *>(tempRotationalMotor->mTorqueFeedbackSensor));
        break;
      }
      case Node::PEN:
        mDeviceList.push_back(static_cast<RosDevice *>(new RosPen(dynamic_cast<Pen *>(tempDevice), this)));
        break;
      case Node::POSITION_SENSOR:
        mSensorList.push_back(
          static_cast<RosSensor *>(new RosPositionSensor(dynamic_cast<PositionSensor *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::RADAR:
        mSensorList.push_back(static_cast<RosSensor *>(new RosRadar(dynamic_cast<Radar *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::RANGE_FINDER:
        mSensorList.push_back(static_cast<RosSensor *>(new RosRangeFinder(dynamic_cast<RangeFinder *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::RECEIVER:
        mSensorList.push_back(static_cast<RosSensor *>(new RosReceiver(dynamic_cast<Receiver *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
      case Node::SKIN:
        mDeviceList.push_back(static_cast<RosDevice *>(new RosSkin(dynamic_cast<Skin *>(tempDevice), this)));
        break;
      case Node::SPEAKER:
        mDeviceList.push_back(static_cast<RosDevice *>(new RosSpeaker(dynamic_cast<Speaker *>(tempDevice), this)));
        break;
      case Node::TOUCH_SENSOR:
        mSensorList.push_back(static_cast<RosSensor *>(new RosTouchSensor(dynamic_cast<TouchSensor *>(tempDevice), this)));
        mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
        break;
    }
    if (previousDevicesCount < mDeviceList.size())
      mDeviceList.back()->init();
  }
  if (mAutoPublish) {
    for (RosSensor *rosSensor : mSensorList)
      rosSensor->enableSensor(mRobot->getBasicTimeStep());
  }
  mSensorList.push_back(static_cast<RosSensor *>(new RosBatterySensor(mRobot, this)));
  mDeviceList.push_back(static_cast<RosDevice *>(mSensorList.back()));
  mDeviceList.back()->init();
  if (mRosKeyboard)
    mSensorList.push_back(mRosKeyboard);
  if (mRosJoystick)
    mSensorList.push_back(mRosJoystick);
  // Once the list is created, make it available to other rosnodes
  mDeviceListService = mNodeHandle->advertiseService(mRobotName + "/robot/get_device_list", &Ros::getDeviceListCallback, this);
}

// timestep callback allowing a ros node to run the simulation step by step
// cppcheck-suppress constParameter
bool Ros::timeStepCallback(webots_ros::set_int::Request &req, webots_ros::set_int::Response &res) {
  if (req.value >= 1 && (req.value % static_cast<int>(mRobot->getBasicTimeStep()) == 0)) {
    mStep++;
    if (step(req.value) == -1) {
      mEnd = true;
      mStep = 0;
      res.success = false;
    } else
      res.success = true;
    if ((unsigned int)req.value != mStepSize)
      mStepSize = req.value;

  } else if (req.value == 0) {
    // the rosnode has stopped and won't control the simulation runtime anymore
    step(mRobot->getBasicTimeStep());
    mStep = 0;
    res.success = true;
  } else
    res.success = false;
  return true;
}

// send the list of the name of the devices by index order
bool Ros::getDeviceListCallback(webots_ros::robot_get_device_list::Request &req,
                                webots_ros::robot_get_device_list::Response &res) {
  int nDevices = mRobot->getNumberOfDevices();
  for (int j = 0; j < nDevices; ++j)
    res.list.push_back(Ros::fixedNameString(mRobot->getDeviceByIndex(j)->getName()));
  return true;
}

void Ros::publishClockIfNeeded() {
  if (mShouldPublishClock) {
    rosgraph_msgs::Clock simulationClock;
    double time = mRobot->getTime();
    simulationClock.clock.sec = (int)time;
    // round prevents precision issues that can cause problems with ROS timers
    simulationClock.clock.nsec = round(1000 * (time - simulationClock.clock.sec)) * 1.0e+6;
    mClockPublisher.publish(simulationClock);
  }
}

void Ros::run(int argc, char **argv) {
  launchRos(argc, argv);
  ros::Rate loopRate(1000);  // Hz
  ros::AsyncSpinner spinner(2);
  spinner.start();

  if (mSetRobotDescription)
    mNodeHandle->setParam("robot_description", mRobot->getUrdf(mRobotDescriptionPrefix));

  if (mUseRosControl)
    mRosControl = new highlevel::RosControl(mRobot, mNodeHandle);

  while (!mEnd && ros::ok()) {
    if (!ros::master::check()) {
      ROS_ERROR("ROS master has stopped or is not responding anymore.");
      mEnd = true;
    }

    if (mRosControl)
      mRosControl->read();

    publishClockIfNeeded();
    for (unsigned int i = 0; i < mSensorList.size(); i++)
      mSensorList[i]->publishValues(mStep * mStepSize);

    if (!mUseWebotsSimTime && mIsSynchronized) {
      int oldStep = mStep;
      while (mStep == oldStep && !mEnd && ros::ok()) {
        loopRate.sleep();
        publishClockIfNeeded();
      }
    } else if (step(mRobot->getBasicTimeStep()) == -1)
      mEnd = true;

    if (mRosControl)
      mRosControl->write();
    mStep++;
  }
}

bool Ros::waitForUserInputEventCallback(webots_ros::robot_wait_for_user_input_event::Request &req,
                                        webots_ros::robot_wait_for_user_input_event::Response &res) {
  res.event = mRobot->waitForUserInputEvent(Robot::UserInputEvent(req.eventType), req.timeout);
  return true;
}

bool Ros::getTimeCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mRobot);
  res.value = mRobot->getTime();
  return true;
}

bool Ros::getModelCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res) {
  assert(mRobot);
  res.value = mRobot->getModel();
  return true;
}

bool Ros::getUrdfCallback(webots_ros::get_urdf::Request &req, webots_ros::get_urdf::Response &res) {
  assert(mRobot);
  res.value = mRobot->getUrdf(req.prefix);
  return true;
}

bool Ros::getDataCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res) {
  assert(mRobot);
  res.value = mRobot->getData();
  return true;
}

bool Ros::setDataCallback(webots_ros::set_string::Request &req, webots_ros::set_string::Response &res) {
  assert(mRobot);
  mRobot->setData(req.value);
  res.success = true;
  return true;
}

bool Ros::getCustomDataCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res) {
  assert(mRobot);
  res.value = mRobot->getCustomData();
  return true;
}

bool Ros::setCustomDataCallback(webots_ros::set_string::Request &req, webots_ros::set_string::Response &res) {
  mRobot->setCustomData(req.value);
  res.success = true;
  return true;
}

bool Ros::getModeCallback(webots_ros::get_int::Request &req, webots_ros::get_int::Response &res) {
  assert(mRobot);
  res.value = mRobot->getMode();
  return true;
}

bool Ros::getSupervisorCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res) {
  assert(mRobot);
  res.value = mRobot->getSupervisor();
  return true;
}

bool Ros::getSynchronizationCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res) {
  assert(mRobot);
  res.value = mRobot->getSynchronization();
  return true;
}

bool Ros::getProjectPathCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res) {
  assert(mRobot);
  res.value = mRobot->getProjectPath();
  return true;
}

bool Ros::getWorldPathCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res) {
  assert(mRobot);
  res.value = mRobot->getWorldPath();
  return true;
}

bool Ros::getBasicTimeStepCallback(webots_ros::get_float::Request &req, webots_ros::get_float::Response &res) {
  assert(mRobot);
  res.value = mRobot->getBasicTimeStep();
  return true;
}

bool Ros::getNumberOfDevicesCallback(webots_ros::get_int::Request &req, webots_ros::get_int::Response &res) {
  assert(mRobot);
  res.value = mRobot->getNumberOfDevices();
  return true;
}

bool Ros::getTypeCallback(webots_ros::get_int::Request &req, webots_ros::get_int::Response &res) {
  assert(mRobot);
  res.value = mRobot->getType();
  return true;
}

bool Ros::setModeCallback(webots_ros::robot_set_mode::Request &req, webots_ros::robot_set_mode::Response &res) {
  char *arg;
  char buffer[req.arg.size()];
  for (unsigned int i = 0; i < req.arg.size(); i++)
    buffer[i] = req.arg[i];
  arg = buffer;
  mRobot->setMode(Robot::Mode(req.mode), arg);
  res.success = 1;
  return true;
}

bool Ros::wwiReceiveTextCallback(webots_ros::get_string::Request &req, webots_ros::get_string::Response &res) {
  res.value = mRobot->wwiReceiveText();
  return true;
}

bool Ros::wwiSendTextCallback(webots_ros::set_string::Request &req, webots_ros::set_string::Response &res) {
  mRobot->wwiSendText(req.value);
  res.success = 1;
  return true;
}
