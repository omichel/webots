#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/emitter.h>
#include <webots/position_sensor.h>
#include <webots/robot.h>

#define TIME_STEP 64

int main(int argc, char **argv) {
  wb_robot_init();

  WbDeviceTag emitter = wb_robot_get_device("emitter");

  const int nb_devices = atoi(argv[1]);
  WbDeviceTag sensors[nb_devices];

  for (int i = 0; i < nb_devices; ++i) {
    char sensor_name[20];
    sprintf(sensor_name, "sensor%d", i + 1);
    sensors[i] = wb_robot_get_device(sensor_name);
    wb_position_sensor_enable(sensors[i], TIME_STEP);
  }

  wb_robot_step(TIME_STEP);

  double positions[3];
  for (int i = 0; i < 3; ++i) {
    if (i < nb_devices)
      positions[i] = wb_position_sensor_get_value(sensors[i]);
    else
      positions[i] = -1.0;
  }

  char outbuffer[50];

  snprintf(outbuffer, sizeof(outbuffer), "%.4f %.4f %.4f %s\n", positions[0], positions[1], positions[2], wb_robot_get_name());

  while (wb_robot_step(TIME_STEP) != -1.0)
    wb_emitter_send(emitter, outbuffer, strlen(outbuffer) + 1);

  wb_robot_cleanup();
  return 0;
}
