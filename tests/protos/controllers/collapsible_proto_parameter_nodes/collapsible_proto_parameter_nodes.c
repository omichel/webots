#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/emitter.h>
#include <webots/motor.h>
#include <webots/position_sensor.h>
#include <webots/robot.h>

#define TIME_STEP 32

int main(int argc, char **argv) {
  wb_robot_init();

  WbDeviceTag motors[5];
  WbDeviceTag sensors[5];

  for (int i = 0; i < 5; ++i) {
    char motor_name[10];
    char sensor_name[10];
    sprintf(motor_name, "motor%d", i);
    sprintf(sensor_name, "sensor%d", i);
    motors[i] = wb_robot_get_device(motor_name);
    sensors[i] = wb_robot_get_device(sensor_name);

    wb_motor_set_position(motors[i], INFINITY);
    wb_motor_set_velocity(motors[i], 0.1 * (i + 1));

    wb_position_sensor_enable(sensors[i], TIME_STEP);
  }

  WbDeviceTag emitter = wb_robot_get_device("emitter");

  wb_robot_step(TIME_STEP);

  while (1) {
    double positions[5];
    for (int i = 0; i < 5; ++i)
      positions[i] = wb_position_sensor_get_value(sensors[i]);

    wb_robot_step(TIME_STEP);
    char outbuffer[100];

    snprintf(outbuffer, sizeof(outbuffer), "%.4f %.4f %.4f %.4f %.4f %s\n", positions[0], positions[1], positions[2],
             positions[3], positions[4], wb_robot_get_name());

    wb_emitter_send(emitter, outbuffer, strlen(outbuffer) + 1);

    wb_robot_step(50 * TIME_STEP);
  }

  wb_robot_cleanup();
  return 0;
}
