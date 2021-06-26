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

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include "messages.pb.h"
#if GOOGLE_PROTOBUF_VERSION < 3006001
#define ByteSizeLong ByteSize
#endif

#include "robot_client.hpp"

static void usage(const std::string &error_msg = "") {
  if (error_msg.length() > 0)
    fprintf(stderr, "Invalid call: %s\n", error_msg.c_str());
  fprintf(stderr, "Usage: client [-v verbosity_level --camera camera_name time_step] <host> <port>\n");
}

int main(int argc, char *argv[]) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  int port = -1;
  std::string host;
  std::string camera;
  int camera_time_step = -1;
  int arg_idx = 1;
  int verbosity = 3;
  while (arg_idx < argc) {
    std::string current_arg(argv[arg_idx]);
    // Treating arguments
    if (current_arg[0] == '-') {
      if (current_arg == "-v") {
        if (arg_idx + 1 >= argc)
          usage("Missing value for verbosity");
        verbosity = std::stoi(argv[arg_idx + 1]);
        arg_idx++;
      } else if (current_arg == "--camera") {
        if (arg_idx + 2 >= argc)
          usage("Missing parameters for camera");
        camera = argv[arg_idx + 1];
        camera_time_step = std::stoi(argv[arg_idx + 2]);
        arg_idx += 2;
      } else  // if current_arg == "-h" or "--help" or anything else
        usage();
    } else if (host.length() == 0)
      host = current_arg;
    else if (port == -1) {
      port = std::stoi(current_arg);
      if (port < 0)
        usage("Unexpected negative value for port: " + current_arg);
    } else
      usage("Unexpected additional argument: " + current_arg);
    arg_idx++;
  }
  if (port == -1)
    usage("Missing arguments");

  RobotClient client(host, port, verbosity);
  client.connectClient();
  bool first_step = true;
  while (client.isOk()) {
    try {
      ActuatorRequests request;
      if (first_step) {
        SensorTimeStep *sensor = request.add_sensor_time_steps();
        sensor->set_name("NeckS");
        sensor->set_timestep(8);
        if (camera.length() > 0) {
          SensorTimeStep *camera_sensor = request.add_sensor_time_steps();
          camera_sensor->set_name(camera);
          camera_sensor->set_timestep(camera_time_step);
        }
        first_step = false;
      }
      MotorPosition *motor = request.add_motor_positions();
      motor->set_name("Neck");
      motor->set_position(1.6);
      client.sendRequest(request);
      SensorMeasurements sensors = client.receive();
      std::string printout;
      google::protobuf::TextFormat::PrintToString(sensors, &printout);
    } catch (const std::runtime_error &exc) {
      std::cerr << "Runtime error: " << exc.what() << std::endl;
    }
  }

  return 0;
}
