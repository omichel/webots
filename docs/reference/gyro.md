## Gyro

Derived from [Device](device.md) and [Solid](solid.md).

```
Gyro {
  MFVec3f lookupTable [ ]    # lookup table
  SFBool  xAxis       TRUE   # {TRUE, FALSE}
  SFBool  yAxis       TRUE   # {TRUE, FALSE}
  SFBool  zAxis       TRUE   # {TRUE, FALSE}
  SFFloat resolution  -1     # {-1, [0, inf)}
}
```

### Description

The [Gyro](#gyro) node is used to model 1, 2 and 3-axis angular velocity sensors (gyroscope).
The angular velocity is measured in radians per second [rad/s].

### Field Summary

- `lookupTable`: This field optionally specifies a lookup table that can be used for mapping the raw angular velocity values [rad/s] to device specific output values.
By default the lookup table is empty and therefore the raw values are returned (no mapping).
See the section on the [DistanceSensor](distancesensor.md#lookup-table) node for more explanation on how a `lookupTable` works.

- `xAxis, yAxis, zAxis`: Each of these boolean fields specifies if the computation should be enabled or disabled for the specified axis.
If one of these fields is set to FALSE, then the corresponding vector element will not be computed and it will return *NaN* (Not a Number).
For example if `zAxis` is FALSE, then the second element of the array returned by the `wb_gyro_get_values` function returns *NaN*.
The default is that all three axes are enabled (TRUE).

- `resolution`: This field allows to define the resolution of the sensor, the resolution is the smallest change that it is able to measure.
Setting this field to -1 (default) means that the sensor has an 'infinite' resolution (it can measure any infinitesimal change).
The raw measurement is first interpolated according to the lookup table and subsequently sampled with respect to the specified resolution, if one is defined.
This field accepts any value in the interval (0.0, inf).

### Gyro Functions

#### `wb_gyro_enable`
#### `wb_gyro_disable`
#### `wb_gyro_get_sampling_period`
#### `wb_gyro_get_values`
#### `wb_gyro_get_lookup_table_size`
#### `wb_gyro_get_lookup_table`

%tab-component "language"

%tab "C"

```c
#include <webots/gyro.h>

void wb_gyro_enable(WbDeviceTag tag, int sampling_period);
void wb_gyro_disable(WbDeviceTag tag);
int wb_gyro_get_sampling_period(WbDeviceTag tag);
const double *wb_gyro_get_values(WbDeviceTag tag);
int wb_gyro_get_lookup_table_size(WbDeviceTag tag);
const double *wb_gyro_get_lookup_table(WbDeviceTag tag);
```

%tab-end

%tab "C++"

```cpp
#include "<webots/Gyro.hpp>"

namespace webots {
  class Gyro : public Device {
    virtual void enable(int samplingPeriod);
    virtual void disable();
    int getSamplingPeriod() const;
    const double *getValues() const;
    int getLookupTableSize() const;
    const double *getLookupTable() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Gyro

class Gyro (Device):
    def enable(self, samplingPeriod):
    def disable(self):
    def getSamplingPeriod(self):
    def getValues(self):
    def getLookupTable(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Gyro;

public class Gyro extends Device {
  public void enable(int samplingPeriod);
  public void disable();
  public int getSamplingPeriod();
  public double[] getValues();
  public double[] getLookupTable();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_gyro_enable(tag, sampling_period)
wb_gyro_disable(tag)
period = wb_gyro_get_sampling_period(tag)
x_y_z_array = wb_gyro_get_values(tag)
lookup_table_array = wb_gyro_get_lookup_table(tag)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/<device_name>/values` | `topic` | [`sensor_msgs::Imu`](http://docs.ros.org/api/sensor_msgs/html/msg/Imu.html) | [`Header`](http://docs.ros.org/api/std_msgs/html/msg/Header.html) `header`<br/>[`geometry_msgs/Quaternion`](http://docs.ros.org/api/geometry_msgs/html/msg/Quaternion.html) `orientation`<br/>`float64[9] orientation_covariance`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `angular_velocity`<br/>`float64[9] angular_velocity_covariance`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `linear_acceleration`<br/>`float64[9] linear_acceleration_covariance`<br/><br/>Note: only the angular_velocity is filled in |
| `/<device_name>/enable` | `service` | [`webots_ros::set_int`](ros-api.md#common-services) | |
| `/<device_name>/get_sampling_period` | `service` | [`webots_ros::get_int`](ros-api.md#common-services) | |
| `/<device_name>/get_lookup_table` | `service` | [`webots_ros::get_float_array`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*enable, disable and read the output values of the gyro device*

The `wb_gyro_enable` function turns on the angular velocity measurements.
The `sampling_period` argument specifies the sampling period of the sensor and is expressed in milliseconds.
Note that the first measurement will be available only after the first sampling period elapsed.

The `wb_gyro_disable` function turns off the [Gyro](#gyro) device.

The `wb_gyro_get_sampling_period` function returns the period given into the `wb_gyro_enable` function, or 0 if the device is disabled.

The `wb_gyro_get_values` function returns the current measurement of the [Gyro](#gyro) device.
The values are returned as a 3D-vector therefore only the indices 0, 1, and 2 are valid for accessing the vector.
Each vector element represents the angular velocity about one of the axes of the [Gyro](#gyro) node, expressed in radians per second [rad/s].
The first element corresponds to the angular velocity about the x-axis, the second element to the y-axis, etc.

The `wb_gyro_get_lookup_table_size` function returns the number of rows in the lookup table.

The `wb_gyro_get_lookup_table` function returns the values of the lookup table.
This function returns a matrix containing exactly N * 3 values (N represents the number of mapped values optained with the `wb_gyro_get_lookup_table_size` function) that shall be interpreted as a N x 3 table.

> **Note** [C, C++]: The returned vector is a pointer to the internal values managed by the [Gyro](#gyro) node, therefore it is illegal to free this pointer.
Furthermore, note that the pointed values are only valid until the next call to the `wb_robot_step` or `Robot::step` functions.
If these values are needed for a longer period they must be copied.

<!-- -->

> **Note** [Python]: The `getValues` function returns the vector as a list containing three floats.
