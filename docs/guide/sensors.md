## Sensors

Webots can simulate a lot of standard robotic sensors.
Using the nodes described [below](#generic-sensor-list) and their corresponding API is sufficient in most of the cases.

More specific sensors are built on the top of these generic nodes, thanks to the [PROTO system](../reference/proto.md).
These sensors are listed by category [in the section below](#commercially-available-sensors).

### Generic Sensor List

| Icon | Device | Description |
| :---: | --- | --- |
| ![Accelerometer.png](images/sensors/Accelerometer.png) | [Accelerometer](../reference/accelerometer.md) | *Simulates an accelerometer sensor which measures the relative accelerations.* |
| ![Camera.png](images/sensors/Camera.png) | [Camera](../reference/camera.md) | *Simulates an RGB camera, a linear camera, a gray-scale camera, a fish-eye camera or a smart camera with multiple special effects including noise, depth of field, motion blur or lense flares.* |
| ![Compass.png](images/sensors/Compass.png) | [Compass](../reference/compass.md) | *Simulates a magnetic sensor which measures the relative direction to the north.* |
| ![DistanceSensor.png](images/sensors/DistanceSensor.png) | [DistanceSensor](../reference/distancesensor.md) | *Simulates a distance measuring sensor based on infra-red light, sonar echo, or laser beam.* |
| ![GPS.png](images/sensors/GPS.png) | [GPS](../reference/gps.md) | *Simulates a positioning sensor which measures the absolute position in Webots coordinate system or in the WGS84 coordinate system.* |
| ![Gyro.png](images/sensors/Gyro.png) | [Gyro](../reference/gyro.md) | *Simulates a gyroscope sensor which measures the relative angular velocities.* |
| ![InertialUnit.png](images/sensors/InertialUnit.png) | [InertialUnit](../reference/inertialunit.md) | *Simulates a sensor which measures the relative roll, pitch and yaw angles.* |
| ![Lidar.png](images/sensors/Lidar.png) | [Lidar](../reference/lidar.md) | *Simulates a laser-scanner sensor also known as lidar.* |
| ![PositionSensor.png](images/sensors/PositionSensor.png) | [PositionSensor](../reference/positionsensor.md) | *Simulates a sensor which can monitor a joint position, such as an encoder or a potentiometer.* |
| ![Radar.png](images/sensors/Radar.png) | [Radar](../reference/radar.md) | *Simulates a radar sensor.* |
| ![RangeFinder.png](images/sensors/RangeFinder.png) | [RangeFinder](../reference/rangefinder.md) | *Simulates a depth camera also known as range-finder.* |
| ![Receiver.png](images/sensors/Receiver.png) | [Receiver](../reference/receiver.md) | *Simulates radio, serial or infra-red receiver receiving data from other robots.* |
| ![TouchSensor.png](images/sensors/TouchSensor.png) | [TouchSensor](../reference/touchsensor.md) | *Simulates a bumper or a force sensor.* |

### Commercially Available Sensors

Please [contact us](https://cyberbotics.com/#contact) if you would like to see your favorite sensor here.

#### Camera Sensors

| Preview | Name |  Manufacturer |
| :---: | --- | --- | --- |
| ![multisense_s21_icon.png](images/sensors/multisense_s21_icon.png) | [S21](camera-sensors.md#multisense-s21) | MultiSense |

#### DistanceSensor Sensors

| Preview | Name |  Manufacturer |
| :---: | --- | --- | --- |
| ![sharp_GP2D120_icon.png](images/sensors/sharp_GP2D120_icon.png) | [GP2D120](distancesensor-sensors.md#sharp-gp2d120) | Sharp |
| ![sharp_GP2Y0A02YK0F_icon.png](images/sensors/sharp_GP2Y0A02YK0F_icon.png) | [GP2Y0A02YK0F](distancesensor-sensors.md#sharp-gp2y0a02yk0f) | Sharp |
| ![sharp_GP2Y0A41SK0F_icon.png](images/sensors/sharp_GP2Y0A41SK0F_icon.png) | [GP2Y0A41SK0F](distancesensor-sensors.md#sharp-gp2y0a41sk0f) | Sharp |
| ![sharp_GP2Y0A710K0F_icon.png](images/sensors/sharp_GP2Y0A710K0F_icon.png) | [GP2Y0A710K0F](distancesensor-sensors.md#sharp-gp2y0a710k0f) | Sharp |

#### Lidar Sensors

| Preview | Name |  Manufacturer |
| :---: | --- | --- | --- |
| ![ibeo_icon.png](images/sensors/ibeo_icon.png) | [LUX](lidar-sensors.md#ibeo-lux) | Ibeo |
| ![hokuyo_urg_04lx_icon.png](images/sensors/hokuyo_urg_04lx_icon.png) | [Hokuyo URG-04LX](lidar-sensors.md#hokuyo-urg-04lx) | Hokuyo |
| ![hokuyo_urg_04lx_ug01_icon.png](images/sensors/hokuyo_urg_04lx_ug01_icon.png) | [Hokuyo URG-04LX-UG01](lidar-sensors.md#hokuyo-urg-04lx-ug01) | Hokuyo |
| ![hokuyo_utm_30lx_icon.png](images/sensors/hokuyo_utm_30lx_icon.png) | [Hokuyo UTM-30LX](lidar-sensors.md#hokuyo-utm-30lx) | Hokuyo |
| ![robotis_lds01_icon.png](images/sensors/robotis_lds01_icon.png) | [LDS-01](lidar-sensors.md#robotis-lds-01) | Robotis |
| ![sick_lms291_icon.png](images/sensors/sick_lms291_icon.png) | [LMS 291](lidar-sensors.md#sick-lms-291) | SICK |
| ![sick_ld_mrs_icon.png](images/sensors/sick_ld_mrs_icon.png) | [LD-MRS](lidar-sensors.md#sick-ld-mrs) | SICK |
| ![sick_s300_icon.png](images/sensors/sick_s300_icon.png) | [S300](lidar-sensors.md#sick-s300) | SICK |
| ![velodyne_vpl_16_icon.png](images/sensors/velodyne_vpl_16_icon.png) | [VLP Puck](lidar-sensors.md#velodyne-puck) | Velodyne |
| ![velodyne_hdl_32e_icon.png](images/sensors/velodyne_hdl_32e_icon.png) | [HDL 32E](lidar-sensors.md#velodyne-hdl-32e) | Velodyne |
| ![velodyne_hdl_64e_icon.png](images/sensors/velodyne_hdl_64e_icon.png) | [HDL 64E](lidar-sensors.md#velodyne-hdl-64e) | Velodyne |

#### Radar Sensors

| Preview | Name |  Manufacturer |
| :---: | --- | --- | --- |
| ![delphi_icon.png](images/sensors/delphi_icon.png) | [ESR](radar-sensors.md#delphi-esr) | Delphi |
| ![smartmicro_icon.png](images/sensors/smartmicro_icon.png) | [UMRR-0a](radar-sensors.md#smartmicro-umrr-0a) | Smartmicro |

#### RangeFinder Sensors

| Preview | Name |  Manufacturer |
| :---: | --- | --- | --- |
| ![kinect_icon.png](images/sensors/kinect_icon.png) | [Kinect](range-finder-sensors.md#microsoft-kinect) | Microsoft |
