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

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>

#ifdef _WIN32
#include <winsock.h>
typedef int socklen_t;
#define MSG_NOSIGNAL 0
void usleep(__int64 usec) {
  HANDLE timer;
  LARGE_INTEGER ft;
  ft.QuadPart = -10 * usec;
  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <google/protobuf/text_format.h>
#include "messages.pb.h"
#if GOOGLE_PROTOBUF_VERSION < 3006001
#define ByteSizeLong ByteSize
#endif

// #define JPEG_COMPRESSION 1  // uncomment this to test JPEG compression

// #define TURBOJPEG 1
// It turns out that the libjpeg interface to turbojpeg runs faster than the native turbojpeg interface
// Alternatives to be considered: NVIDIA CUDA nvJPEG Encoder, Intel IPP JPEG encoder
#ifdef TURBOJPEG
#include <turbojpeg.h>
#else
#include <jpeglib.h>
#endif

#include <webots/Accelerometer.hpp>
#include <webots/Camera.hpp>
#include <webots/Gyro.hpp>
#include <webots/Motor.hpp>
#include <webots/Node.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Robot.hpp>
#include <webots/TouchSensor.hpp>

#include <algorithm>
#include <chrono>
#include <deque>

#define RED 0
#define BLUE 1

// Time to wait before attempting to send the message again when the buffer is full
#define BUFFER_FULL_SLEEP_US 500

using sc = std::chrono::steady_clock;
using time_point = std::chrono::time_point<sc>;
using duration = std::chrono::duration<double, std::milli>;

static fd_set rfds;
static int n_allowed_hosts;
static std::vector<std::string> allowed_hosts;

static bool set_blocking(int fd, bool blocking) {
#ifdef _WIN32
  unsigned long mode = blocking ? 0 : 1;
  return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
    return false;
  flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
  return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

static void close_socket(int fd) {
#ifdef _WIN32
  closesocket(fd);
#else
  close(fd);
#endif
}

// TODO: make a non-blocking version and store the data sent to send it again
// afterwards
// The return value might be insufficient to consider the three different
// cases if we want to consider real async communication:
// 1. Message is properly sent
// 2. Message is sent partially because buffer is full (currently not possible)
// 3. Connection broke
static bool send_all(int socket, const char *buffer, size_t length) {
  while (length > 0) {
    int i = send(socket, buffer, length, MSG_NOSIGNAL);
    if (i < 1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        usleep(BUFFER_FULL_SLEEP_US);
      else if (errno == EPIPE) {
        fprintf(stderr, "Connection broke\n");
        return false;
      } else {
        fprintf(stderr, "Unknown error while sending message\n");
        return false;
      }
    } else {
      buffer += i;
      length -= i;
    }
  }
  return true;
}

static int create_socket_server(int port) {
  int rc;
  int server_fd;
  struct sockaddr_in address;

#ifdef _WIN32
  WSADATA info;
  rc = WSAStartup(MAKEWORD(2, 2), &info);  // Winsock 2.2
  if (rc != 0) {
    fprintf(stderr, "Cannot initialize Winsock\n");
    return -1;
  }
#endif

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    fprintf(stderr, "Cannot create socket\n");
    return -1;
  }
  memset(&address, 0, sizeof(struct sockaddr_in));
  address.sin_family = AF_INET;
  address.sin_port = htons((unsigned short)port);
  address.sin_addr.s_addr = INADDR_ANY;
  rc = bind(server_fd, (struct sockaddr *)&address, sizeof(struct sockaddr));
  if (rc == -1) {
    fprintf(stderr, "Cannot bind port %d\n", port);
    close_socket(server_fd);
    return -1;
  }
  if (listen(server_fd, 1) == -1) {
    fprintf(stderr, "Cannot listen for connections\n");
    close_socket(server_fd);
    return -1;
  }
  return server_fd;
}

#ifdef JPEG_COMPRESSION

static void encode_jpeg(const unsigned char *image, int width, int height, int quality, unsigned long *size,
                        unsigned char **buffer) {
#ifdef TURBOJPEG
  tjhandle compressor = tjInitCompress();
  tjCompress2(compressor, image, width, 0, height, TJPF_RGB, buffer, size, TJSAMP_444, quality, TJFLAG_FASTDCT);
  tjDestroy(compressor);
#else
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE);
  jpeg_mem_dest(&cinfo, buffer, size);
  jpeg_start_compress(&cinfo, TRUE);
  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = (unsigned char *)&image[cinfo.next_scanline * width * 3];
    jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
#endif
}

static void free_jpeg(unsigned char *buffer) {
#ifdef TURBOJPEG
  tjFree(buffer);
#else
  free(buffer);
#endif
}

#endif  // JPEG_COMPRESSION

static void warn(SensorMeasurements &sensor_measurements, std::string text) {
  Message *message = sensor_measurements.add_messages();
  message->set_message_type(Message::WARNING_MESSAGE);
  message->set_text(text);
}

class MotorCommand {
public:
  explicit MotorCommand(webots::Motor *m) {
    motor = m;
    position = NAN;
    velocity = m->getVelocity();
    force_or_torque = NAN;
  }
  webots::Motor *motor;
  double position;
  double velocity;
  double force_or_torque;
};

class PlayerServer {
public:
  PlayerServer(const std::vector<std::string> &allowed_hosts, int port, int player_id, int team, webots::Robot *robot) :
    allowed_hosts(allowed_hosts),
    port(port),
    player_id(player_id),
    team(team),
    client_fd(-1),
    controller_time(0),
    recv_buffer(NULL),
    recv_index(0),
    recv_size(0),
    content_size(0),
    robot(robot) {
    actuators_enabled = TRUE;
    devices_enabled = TRUE;
    basic_time_step = robot->getBasicTimeStep();
    printMessage("server started on port " + std::to_string(port));
    server_fd = create_socket_server(port);
    set_blocking(server_fd, false);
  }

  int accept_client(int server_fd) {
    int cfd;
    struct sockaddr_in client;
    socklen_t size = sizeof(struct sockaddr_in);
    cfd = accept(server_fd, (struct sockaddr *)&client, &size);
    if (cfd != -1) {
      struct hostent *client_info = gethostbyname((char *)inet_ntoa(client.sin_addr));
      bool allowed = false;
      for (int i = 0; i < n_allowed_hosts; i++) {
        if (std::string(client_info->h_name) == allowed_hosts[i]) {
          allowed = true;
          break;
        }
      }
      if (allowed) {
        printMessage("Accepted connection from " + std::string(client_info->h_name));
        send_all(cfd, "Welcome", 8);
      } else {
        printMessage("Refused connection from " + std::string(client_info->h_name));
        send_all(cfd, "Refused", 8);
        close_socket(cfd);
        cfd = -1;
      }
    }
    return cfd;
  }

  void step() {
    controller_time += basic_time_step;
    if (client_fd == -1) {
      client_fd = accept_client(server_fd);
      if (client_fd != -1) {
        set_blocking(client_fd, false);
      }
    } else {
      FD_ZERO(&rfds);
      FD_SET(client_fd, &rfds);
      struct timeval tv = {0, 0};
      auto start = sc::now();
      int retval = select(client_fd + 1, &rfds, NULL, NULL, &tv);
      auto after_select = sc::now();
      if (retval == -1)
        perror("select()");
      else
        receiveMessages();
      std::string customData = robot->getCustomData();
      if (customData == "" && actuators_enabled == FALSE) {
        resumeMotors();
        actuators_enabled = TRUE;
      } else if (customData == "penalized" && actuators_enabled) {
        // penalized robots gets only their actuators disabled so that they become asleep
        stopMotors();
        actuators_enabled = FALSE;
      } else if (customData == "red_card") {  // robots with a red card get both sensors and actuators disabled
        devices_enabled = FALSE;
        for (const auto &entry : sensors)
          enableSensor(entry.first, 0);
        sensors.clear();
      }

      auto after_receive = sc::now();
      // Independently from if we received a message or not, send a message to the Controller
      prepareSensorMessage();
      auto after_prepare = sc::now();
      updateDevices();
      sendSensorMessage();
      auto after_send = sc::now();

      double step_time = duration(after_send - start).count();

      bool diagnose_time = benchmark_level != 0 && step_time > budget_ms;

      if (benchmark_level >= 3 || diagnose_time) {
        benchmarkPrint("\tSelect time", after_select, start);
        benchmarkPrint("\tReceive time", after_receive, after_select);
        benchmarkPrint("\tPrepare time", after_prepare, after_receive);
        benchmarkPrint("\tSend time", after_send, after_prepare);
      }
      if (benchmark_level >= 2 || diagnose_time)
        benchmarkPrint("Step time: ", after_send, start);
    }
  }

  void receiveMessages() {
    while (client_fd != -1) {
      size_t bytes_received = 0;
      if (content_size == 0) {
        // If no content is available, receive header
        bytes_received = receiveData((char *)&content_size, sizeof(uint32_t));
        content_size = ntohl(content_size);
        recv_buffer = new char[content_size];
      } else {
        // If content is expected, read it and treat message if fully received
        bytes_received = receiveData(recv_buffer + recv_index, content_size - recv_index);
        recv_index += bytes_received;
        if (recv_index == content_size && devices_enabled)
          processBuffer();
      }
      // If we consumed all data, stop trying to read
      if (bytes_received == 0)
        break;
    }
  }

  // Attempts to read up-to length from the client_fd, if data is missing, stops reading.
  // Returns the number of bytes read so far
  size_t receiveData(char *buffer, size_t length) {
    size_t received = 0;
    while (received < length) {
      int n = recv(client_fd, buffer, length - received, 0);
      if (n == -1) {
        if (errno && errno != EAGAIN && errno != EWOULDBLOCK) {
          perror("recv()");
          printMessage("Unexpected failure while receiving data");
          closeClientSocket();
        }
        break;
      }
      if (n == 0) {
        printMessage("Client disconnected");
        closeClientSocket();
        break;
      }
      received += n;
    }
    return received;
  }

  MotorCommand *getMotorCommand(webots::Motor *motor) {
    for (size_t i = 0; i != motor_commands.size(); i++)
      if (motor_commands[i]->motor == motor)
        return motor_commands[i];
    MotorCommand *motor_command = new MotorCommand(motor);
    motor_commands.push_back(motor_command);
    return motor_command;
  }

  void stopMotors() const {
    for (size_t i = 0; i != motor_commands.size(); i++) {
      webots::Motor *motor = motor_commands[i]->motor;
      motor->setVelocity(0);
      if (motor->getType() == webots::Motor::ROTATIONAL)
        motor->setTorque(0);
      else
        motor->setForce(0);
    }
  }
  void resumeMotors() {
    for (size_t i = 0; i != motor_commands.size(); i++) {
      webots::Motor *motor = motor_commands[i]->motor;
      if (!isnan(motor_commands[i]->position))
        motor->setPosition(motor_commands[i]->position);
      if (!isnan(motor_commands[i]->velocity))
        motor->setVelocity(motor_commands[i]->velocity);
      if (!isnan(motor_commands[i]->force_or_torque)) {
        if (motor->getType() == webots::Motor::ROTATIONAL)
          motor->setTorque(motor_commands[i]->force_or_torque);
        else
          motor->setForce(motor_commands[i]->force_or_torque);
      }
    }
  }
  void enableSensor(webots::Device *device, int time_step) {
    start_sensoring_time[device] = controller_time;  // For sensor synchronisation in case of different timesteps
    switch (device->getNodeType()) {
      case webots::Node::ACCELEROMETER: {
        webots::Accelerometer *accelerometer = static_cast<webots::Accelerometer *>(device);
        accelerometer->enable(time_step);
        break;
      }
      case webots::Node::CAMERA: {
        webots::Camera *camera = static_cast<webots::Camera *>(device);
        camera->enable(time_step);
        break;
      }
      case webots::Node::GYRO: {
        webots::Gyro *gyro = static_cast<webots::Gyro *>(device);
        gyro->enable(time_step);
        break;
      }
      case webots::Node::POSITION_SENSOR: {
        webots::PositionSensor *positionSensor = static_cast<webots::PositionSensor *>(device);
        positionSensor->enable(time_step);
        break;
      }
      case webots::Node::TOUCH_SENSOR: {
        webots::TouchSensor *touchSensor = static_cast<webots::TouchSensor *>(device);
        touchSensor->enable(time_step);
        break;
      }
      default:
        warn(sensor_measurements, "Device \"" + device->getName() + "\" is not supported, time step command, ignored.");
    }
  }

  void processBuffer() {
    ActuatorRequests actuatorRequests;
    actuatorRequests.ParseFromArray(recv_buffer, recv_index);
    // Reset buffer associated values
    recv_index = 0;
    content_size = 0;
    delete[] recv_buffer;
    // Processing actuatorRequests and adding warnings to the sensor message
    for (int i = 0; i < actuatorRequests.motor_positions_size(); i++) {
      const MotorPosition motorPosition = actuatorRequests.motor_positions(i);
      webots::Motor *motor = robot->getMotor(motorPosition.name());
      if (motor) {
        getMotorCommand(motor)->position = motorPosition.position();
        if (actuators_enabled)
          motor->setPosition(motorPosition.position());
      } else
        warn(sensor_measurements, "Motor \"" + motorPosition.name() + "\" not found, position command ignored.");
    }
    for (int i = 0; i < actuatorRequests.motor_velocities_size(); i++) {
      const MotorVelocity motorVelocity = actuatorRequests.motor_velocities(i);
      webots::Motor *motor = robot->getMotor(motorVelocity.name());
      if (motor) {
        getMotorCommand(motor)->velocity = motorVelocity.velocity();
        if (actuators_enabled)
          motor->setVelocity(motorVelocity.velocity());
      } else
        warn(sensor_measurements, "Motor \"" + motorVelocity.name() + "\" not found, velocity command ignored.");
    }
    for (int i = 0; i < actuatorRequests.motor_forces_size(); i++) {
      const MotorForce motorForce = actuatorRequests.motor_forces(i);
      webots::Motor *motor = robot->getMotor(motorForce.name());
      if (motor) {
        getMotorCommand(motor)->force_or_torque = motorForce.force();
        if (actuators_enabled)
          motor->setForce(motorForce.force());
      } else
        warn(sensor_measurements, "Motor \"" + motorForce.name() + "\" not found, force command ignored.");
    }
    for (int i = 0; i < actuatorRequests.motor_torques_size(); i++) {
      const MotorTorque motorTorque = actuatorRequests.motor_torques(i);
      webots::Motor *motor = robot->getMotor(motorTorque.name());
      if (motor) {
        getMotorCommand(motor)->force_or_torque = motorTorque.torque();
        if (actuators_enabled)
          motor->setTorque(motorTorque.torque());
      } else
        warn(sensor_measurements, "Motor \"" + motorTorque.name() + "\" not found, torque command ignored.");
    }
    for (int i = 0; i < actuatorRequests.motor_pids_size(); i++) {
      const MotorPID motorPID = actuatorRequests.motor_pids(i);
      webots::Motor *motor = robot->getMotor(motorPID.name());
      if (motor) {
        getMotorCommand(motor);
        motor->setControlPID(motorPID.pid().x(), motorPID.pid().y(), motorPID.pid().z());
      } else
        warn(sensor_measurements, "Motor \"" + motorPID.name() + "\" not found, PID command ignored.");
    }
    for (int i = 0; i < actuatorRequests.camera_qualities_size(); i++) {
      const CameraQuality cameraQuality = actuatorRequests.camera_qualities(i);
      webots::Camera *camera = robot->getCamera(cameraQuality.name());
      if (camera)
        warn(sensor_measurements, "CameraQuality is not yet implemented, ignored.");
      else
        warn(sensor_measurements, "Camera \"" + cameraQuality.name() + "\" not found, quality command ignored.");
    }
    for (int i = 0; i < actuatorRequests.camera_exposures_size(); i++) {
      const CameraExposure cameraExposure = actuatorRequests.camera_exposures(i);
      webots::Camera *camera = robot->getCamera(cameraExposure.name());
      if (camera)
        camera->setExposure(cameraExposure.exposure());
      else
        warn(sensor_measurements, "Camera \"" + cameraExposure.name() + "\" not found, exposure command ignored.");
    }
    // we need to enable the sensors after we sent the sensor value to avoid
    // sending values for disabled sensors.
    for (int i = 0; i < actuatorRequests.sensor_time_steps_size(); i++) {
      const SensorTimeStep sensorTimeStep = actuatorRequests.sensor_time_steps(i);
      webots::Device *device = robot->getDevice(sensorTimeStep.name());
      if (device) {
        const int sensor_time_step = sensorTimeStep.timestep();

        int min_time_step = basic_time_step;
        if (device->getNodeType() == webots::Node::CAMERA)
          min_time_step = camera_min_time_step;

        bool add_sensor = false;
        if (sensor_time_step == 0)
          sensors.erase(device);
        else if (sensor_time_step < min_time_step)
          warn(sensor_measurements, "Time step for \"" + sensorTimeStep.name() + "\" should be greater or equal to " +
                                      std::to_string(min_time_step) + ", ignoring " + std::to_string(sensor_time_step) +
                                      " value.");
        else if (sensor_time_step % basic_time_step != 0)
          warn(sensor_measurements, "Time step for \"" + sensorTimeStep.name() + "\" should be a multiple of " +
                                      std::to_string(basic_time_step) + ", ignoring " + std::to_string(sensor_time_step) +
                                      " value.");
        else if (device->getNodeType() == webots::Node::CAMERA) {
          webots::Camera *camera = static_cast<webots::Camera *>(device);
          double robot_rendering_quota = team_rendering_quota / nb_robots_in_team;
          double requested_bandwidth = getRenderingBandwidth(camera, sensor_time_step);
          // Adding bandwidth of other cameras
          for (const auto &entry : sensors)
            if (entry.first->getNodeType() == webots::Node::CAMERA && entry.first->getName() != sensorTimeStep.name())
              requested_bandwidth += getRenderingBandwidth(static_cast<webots::Camera *>(entry.first), entry.second);
          for (const auto &entry : new_sensors)
            if (entry.first->getNodeType() == webots::Node::CAMERA && entry.first->getName() != sensorTimeStep.name())
              requested_bandwidth += getRenderingBandwidth(static_cast<webots::Camera *>(entry.first), entry.second);
          // Only allowing to set time_step if the rendering bandwidth is not exceeded
          if (requested_bandwidth > robot_rendering_quota)
            warn(sensor_measurements, "requested rendering bandwidth is above the limit (" +
                                        std::to_string((int)std::round(requested_bandwidth)) + "MB/s > " +
                                        std::to_string((int)std::round(robot_rendering_quota)) + "MB/s)");
          else
            add_sensor = true;
        } else
          add_sensor = true;
        if (add_sensor) {
          // only add device if device isn't added
          if (sensors.count(device) == 0)
            new_sensors[device] = sensor_time_step;
          // Avoiding to enable again if the request is not making any change to the sensor
          else if (sensors.at(device) == sensor_time_step)
            continue;
          enableSensor(device, sensor_time_step);
        }
      } else
        warn(sensor_measurements, "Device \"" + sensorTimeStep.name() + "\" not found, time step command, ignored.");
    }
  }

  void prepareSensorMessage() {
    sensor_measurements.set_time(controller_time);
    struct timeval tp;
    gettimeofday(&tp, NULL);
    uint64_t real_time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    sensor_measurements.set_real_time(real_time);
    if (!devices_enabled)  // if devices are disabled (because robot got a red card), no sensor data is sent to the controller
      return;
    std::string active_sensor;
    std::chrono::time_point<sc> sensor_start;
    for (const auto &entry : sensors) {
      if (benchmark_level >= 4 && active_sensor != "") {
        std::cout << "\t\t" << active_sensor << " update time " << duration(sc::now() - sensor_start).count() << "ms"
                  << std::endl;
      }
      sensor_start = sc::now();
      active_sensor = entry.first->getName();
      webots::Device *dev = entry.first;
      webots::Accelerometer *accelerometer = dynamic_cast<webots::Accelerometer *>(dev);
      if (accelerometer) {
        if ((controller_time - start_sensoring_time[dev]) % accelerometer->getSamplingPeriod())
          continue;
        AccelerometerMeasurement *measurement = sensor_measurements.add_accelerometers();
        measurement->set_name(accelerometer->getName());
        const double *values = accelerometer->getValues();
        Vector3 *vector3 = measurement->mutable_value();
        vector3->set_x(values[0]);
        vector3->set_y(values[1]);
        vector3->set_z(values[2]);
        continue;
      }
      webots::Camera *camera = dynamic_cast<webots::Camera *>(dev);
      if (camera) {
        if ((controller_time - start_sensoring_time[dev]) % camera->getSamplingPeriod())
          continue;
        CameraMeasurement *measurement = sensor_measurements.add_cameras();
        const int width = camera->getWidth();
        const int height = camera->getHeight();
        measurement->set_name(camera->getName());
        measurement->set_width(width);
        measurement->set_height(height);
        measurement->set_quality(-1);  // raw image (JPEG compression not yet supported)
        const unsigned char *rgba_image = camera->getImage();
        const int rgb_image_size = width * height * 3;
        unsigned char *rgb_image = new unsigned char[rgb_image_size];
        for (int i = 0; i < width * height; i++) {
          rgb_image[3 * i] = rgba_image[4 * i];
          rgb_image[3 * i + 1] = rgba_image[4 * i + 1];
          rgb_image[3 * i + 2] = rgba_image[4 * i + 2];
        }
        measurement->set_image(rgb_image, rgb_image_size);
        delete[] rgb_image;

#ifdef JPEG_COMPRESSION
        // testing JPEG compression (impacts the performance)
        unsigned char *buffer = NULL;
        long unsigned int bufferSize = 0;
        encode_jpeg(rgba_image, width, height, 95, &bufferSize, &buffer);
        free_jpeg(buffer);
        buffer = NULL;
#endif

        continue;
      }
      webots::Gyro *gyro = dynamic_cast<webots::Gyro *>(dev);
      if (gyro) {
        if ((controller_time - start_sensoring_time[dev]) % gyro->getSamplingPeriod())
          continue;
        GyroMeasurement *measurement = sensor_measurements.add_gyros();
        measurement->set_name(gyro->getName());
        const double *values = gyro->getValues();
        Vector3 *vector3 = measurement->mutable_value();
        vector3->set_x(values[0]);
        vector3->set_y(values[1]);
        vector3->set_z(values[2]);
        continue;
      }
      webots::PositionSensor *position_sensor = dynamic_cast<webots::PositionSensor *>(dev);
      if (position_sensor) {
        if ((controller_time - start_sensoring_time[dev]) % position_sensor->getSamplingPeriod())
          continue;
        PositionSensorMeasurement *measurement = sensor_measurements.add_position_sensors();
        measurement->set_name(position_sensor->getName());
        measurement->set_value(position_sensor->getValue());
        continue;
      }
      webots::TouchSensor *touch_sensor = dynamic_cast<webots::TouchSensor *>(dev);
      if (touch_sensor) {
        if ((controller_time - start_sensoring_time[dev]) % touch_sensor->getSamplingPeriod())
          continue;
        webots::TouchSensor::Type type = touch_sensor->getType();
        switch (type) {
          case webots::TouchSensor::BUMPER: {
            BumperMeasurement *measurement = sensor_measurements.add_bumpers();
            measurement->set_name(touch_sensor->getName());
            measurement->set_value(touch_sensor->getValue() == 1.0);
            continue;
          }
          case webots::TouchSensor::FORCE: {
            ForceMeasurement *measurement = sensor_measurements.add_forces();
            measurement->set_name(touch_sensor->getName());
            measurement->set_value(touch_sensor->getValue());
            continue;
          }
          case webots::TouchSensor::FORCE3D: {
            Force3DMeasurement *measurement = sensor_measurements.add_force3ds();
            measurement->set_name(touch_sensor->getName());
            const double *values = touch_sensor->getValues();
            Vector3 *vector3 = measurement->mutable_value();
            vector3->set_x(values[0]);
            vector3->set_y(values[1]);
            vector3->set_z(values[2]);
            continue;
          }
        }
      }
    }
    if (benchmark_level >= 4 && active_sensor != "") {
      std::cout << "\t\t" << active_sensor << " update time " << duration(sc::now() - sensor_start).count() << "ms"
                << std::endl;
    }
  }

  void updateDevices() {
    for (const auto &entry : new_sensors)
      sensors.insert(entry);
    new_sensors.clear();
  }

  void sendSensorMessage() {
    uint32_t size = sensor_measurements.ByteSizeLong();
    uint64_t new_msg_real_time = sensor_measurements.real_time();
    // Clearing old messages from history
    while (message_size_history.size() > 0) {
      uint64_t history_start = message_size_history.back().first;
      if ((new_msg_real_time - history_start) / 1000.0 < window_duration)
        break;
      message_size_history.pop_back();
    }
    uint64_t history_size = 0;
    for (const auto &entry : message_size_history)
      history_size += entry.second;
    double robot_quota = team_network_quota / nb_robots_in_team;
    if (size + history_size > robot_quota * window_duration * std::pow(2, 20)) {
      sensor_measurements.Clear();
      sensor_measurements.set_time(controller_time);
      sensor_measurements.set_real_time(new_msg_real_time);
      Message *message = sensor_measurements.add_messages();
      message->set_message_type(Message::ERROR_MESSAGE);
      message->set_text(std::to_string(robot_quota) + " MB/s quota exceeded.");
      size = sensor_measurements.ByteSizeLong();
    }
    message_size_history.push_front({new_msg_real_time, size});
    char *output = new char[sizeof(uint32_t) + size];
    uint32_t *output_size = (uint32_t *)output;
    *output_size = htonl(size);
    sensor_measurements.SerializeToArray(&output[sizeof(uint32_t)], size);
    if (!send_all(client_fd, output, sizeof(uint32_t) + size)) {
      std::cerr << "Failed to send a message to client" << std::endl;
      closeClientSocket();
    }
    delete[] output;
    sensor_measurements.Clear();
  }

  void benchmarkPrint(const std::string &msg, const time_point &end, const time_point &start) {
    double elapsed_ms = duration(end - start).count();
    printMessage(msg + " " + std::to_string(elapsed_ms) + " ms");
  }

  void printMessage(const std::string &msg) {
    const char *team_name = team == RED ? "RED" : "BLUE";
    printf("%s %d: %s\n", team_name, player_id, msg.c_str());
  }

  void closeClientSocket() {
    close_socket(client_fd);
    client_fd = -1;
    content_size = 0;
  }

  /**
   * Returns the rendering bandwidth in MegaBytes per second for the given camera at the chosen camera_time_step (ms)
   */
  static double getRenderingBandwidth(webots::Camera *camera, int camera_time_step) {
    return camera->getWidth() * camera->getHeight() * 3 * 1000.0 / camera_time_step / std::pow(2, 20);
  }

private:
  std::vector<std::string> allowed_hosts;
  int port;
  int player_id;
  int team;
  int server_fd;
  int client_fd;
  bool actuators_enabled;
  bool devices_enabled;

  /// Keys are adresses of the devices and values are timestep
  std::map<webots::Device *, int> sensors;
  // sensors that have just been added but that were previously disabled.
  // It's required to store them to avoid sending values of unitialized sensors
  std::map<webots::Device *, int> new_sensors;
  std::vector<MotorCommand *> motor_commands;
  uint32_t controller_time;
  std::map<webots::Device *, uint32_t> start_sensoring_time;
  char *recv_buffer;
  int recv_index;
  int recv_size;
  int content_size;

  webots::Robot *robot;
  int basic_time_step;
  SensorMeasurements sensor_measurements;

  /// Stores pair with {real_timestamp_ms, msg_size}
  std::deque<std::pair<uint64_t, uint32_t>> message_size_history;

  // 0: silent
  // 1: print global step cost and details if budget is exceeded
  // 2: additionally to 1: print global cost systematically
  // 3: print costs recap at each step
  // 4: print sensor by sensor recap
  // WARNING: any value higher than 1 significantly impacts simulation speed
  static int benchmark_level;
  // The allowed ms per step before producing a warning
  static double budget_ms;
  /// The network bandwidth allowed for a team [MB/s] (per real-time second and not simulated second)
  static double team_network_quota;
  /// The duration of the time window used to average the bandwidth (seconds)
  static double window_duration;
  /// The minimal value authorized for camera time steps in milliseconds
  static int camera_min_time_step;
  /// The rendering bandwidth allowed for a team [MB/s] (per simulated second)
  static double team_rendering_quota;

public:
  static int nb_robots_in_team;
};

int PlayerServer::benchmark_level = 0;
double PlayerServer::budget_ms = 1.0;
double PlayerServer::team_network_quota = 350.0;
double PlayerServer::window_duration = 1.0;
int PlayerServer::nb_robots_in_team = 4;
int PlayerServer::camera_min_time_step = 16;
double PlayerServer::team_rendering_quota = 350.0;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <port> <nb_players> <host1> <host2> ...", argv[0]);
    return 1;
  }
  const int port = atoi(argv[1]);
  PlayerServer::nb_robots_in_team = atoi(argv[2]);
  n_allowed_hosts = argc - 3;
  for (int i = 0; i < n_allowed_hosts; i++)
    allowed_hosts.push_back(argv[i + 3]);

  webots::Robot *robot = new webots::Robot();
  const int basic_time_step = robot->getBasicTimeStep();
  const std::string name = robot->getName();
  const int player_id = std::stoi(name.substr(name.find_last_of(' ') + 1));
  const int player_team = name[0] == 'r' ? RED : BLUE;

  PlayerServer server(allowed_hosts, port, player_id, player_team, robot);

  while (robot->step(basic_time_step) != -1)
    server.step();

  delete robot;
  return 0;
}
