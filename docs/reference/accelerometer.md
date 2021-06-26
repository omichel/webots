## Accelerometer

Derived from [Device](device.md) and [Solid](solid.md).

```
Accelerometer {
  MFVec3f lookupTable [ ]    # lookup table
  SFBool  xAxis       TRUE   # {TRUE, FALSE}
  SFBool  yAxis       TRUE   # {TRUE, FALSE}
  SFBool  zAxis       TRUE   # {TRUE, FALSE}
  SFFloat resolution  -1     # [0, inf)
}
```

### Description

The [Accelerometer](#accelerometer) node can be used to model accelerometer devices such as those commonly found in mobile electronics, robots and game input devices.
The [Accelerometer](#accelerometer) node measures acceleration and gravity induced reaction forces over 1, 2 or 3 axes.
It can be used for example to detect fall, the up/down direction, etc.
The parent node of an [Accelerometer](#accelerometer) node should have a [Physics](physics.md) node defined in its `physics` field, so that correct measurements can be performed.

### Field Summary

- `lookupTable`: This field optionally specifies a lookup table that can be used for mapping the raw acceleration values [m/s²] to device specific output values.
By default the lookup table is empty and therefore the raw acceleration values are returned (no mapping).
See the section on the [DistanceSensor](distancesensor.md#lookup-table) node for more explanation on how a `lookupTable` works.

- `xAxis, yAxis, zAxis`: Each of these boolean fields enables or disables computation for the specified axis.
If one of these fields is set to FALSE, then the corresponding vector element will not be computed and will return *NaN* (Not a Number).
For example, if `zAxis ` is FALSE, then second element of the array returned by the `wb_accelerometer_get_values` function will always be *NaN*.
The default is that all three axes are enabled (TRUE).
Modifying these fields makes it possible to choose between a single, dual or three-axis accelerometer and to specify which axes will be used.

- `resolution`: This field allows to define the resolution of the sensor, the resolution is the smallest change that it is able to measure.
The raw measurement is first interpolated according to the lookup table and subsequently sampled with respect to the specified resolution, if one is defined.
For example, if `resolution` is 0.2 instead of returning 1.767 the sensor will return 1.8.
Setting this field to -1 (default) means that the sensor has an 'infinite' resolution (it can measure any infinitesimal change).
This field accepts any value in the interval (0.0, inf).

### Accelerometer Functions

#### `wb_accelerometer_enable`
#### `wb_accelerometer_disable`
#### `wb_accelerometer_get_sampling_period`
#### `wb_accelerometer_get_values`
#### `wb_accelerometer_get_lookup_table_size`
#### `wb_accelerometer_get_lookup_table`

%tab-component "language"

%tab "C"

```c
#include <webots/accelerometer.h>

void wb_accelerometer_enable(WbDeviceTag tag, int sampling_period)
void wb_accelerometer_disable(WbDeviceTag tag)
int wb_accelerometer_get_sampling_period(WbDeviceTag tag)
const double *wb_accelerometer_get_values(WbDeviceTag tag)
int wb_accelerometer_get_lookup_table_size(WbDeviceTag tag)
const double *wb_accelerometer_get_lookup_table(WbDeviceTag tag)
```
%tab-end

%tab "C++"

```cpp
#include <webots/Accelerometer.hpp>

namespace webots {
  class Accelerometer : public Device {
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
from controller import Accelerometer

class Accelerometer (Device):
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
import com.cyberbotics.webots.controller.Accelerometer;

public class Accelerometer extends Device {
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
wb_accelerometer_enable(tag, sampling_period)
wb_accelerometer_disable(tag)
period = wb_accelerometer_get_sampling_period(tag)
x_y_z_array = wb_accelerometer_get_values(tag)
lookup_table_array = wb_accelerometer_get_lookup_table(tag)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/<device_name>/values` | `topic` | [`sensor_msgs::Imu`](http://docs.ros.org/api/sensor_msgs/html/msg/Imu.html) | [`Header`](http://docs.ros.org/api/std_msgs/html/msg/Header.html) `header`<br/>[`geometry_msgs/Quaternion`](http://docs.ros.org/api/geometry_msgs/html/msg/Quaternion.html) `orientation`<br/>`float64[9] orientation_covariance`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `angular_velocity`<br/>`float64[9] angular_velocity_covariance`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `linear_acceleration`<br/>`float64[9] linear_acceleration_covariance`<br/><br/>Note: only the linear_acceleration is filled in |
| `/<device_name>/enable` | `service` | [`webots_ros::set_int`](ros-api.md#common-services) | |
| `/<device_name>/get_sampling_period` | `service` | [`webots_ros::get_int`](ros-api.md#common-services) | |
| `/<device_name>/get_lookup_table` | `service` | [`webots_ros::get_float_array`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*enable, disable and read the output of the accelerometer*

The `wb_accelerometer_enable` function allows the user to enable the acceleration measurements.
The `sampling_period` argument specifies the sampling period of the sensor and is expressed in milliseconds.
Note that the first measurement will be available only after the first sampling period elapsed.

The `wb_accelerometer_disable` function turns the accelerometer off, saving computation time.

The `wb_accelerometer_get_sampling_period` function returns the sampling period given to the `wb_accelerometer_enable` function, or 0 if the device is disabled.

The `wb_accelerometer_get_values` function returns the current values measured by the [Accelerometer](#accelerometer).
These values are returned as a 3D-vector, therefore only the indices 0, 1, and 2 are valid for accessing the vector.
Each element of the vector represents the acceleration along the corresponding axis of the [Accelerometer](#accelerometer) node, expressed in meters per second squared [m/s²].
The first element corresponds to the x-axis, the second element to the y-axis, etc.
An [Accelerometer](#accelerometer) at rest with earth's gravity will indicate 1 g (9.81 m/s²) along the vertical axis.
Note that the gravity can be specified in the `gravity` field in the [WorldInfo](worldinfo.md) node.
To obtain the acceleration due to motion alone, this offset must be subtracted.
The device's output will be zero during free fall when no offset is substracted.

The `wb_accelerometer_get_lookup_table_size` function returns the number of rows in the lookup table.

The `wb_accelerometer_get_lookup_table` function returns the values of the lookup table.
This function returns a matrix containing exactly N * 3 values (N represents the number of mapped values optained with the `wb_accelerometer_get_lookup_table_size` function) that shall be interpreted as a N x 3 table.

> **Note** [C, C++]: The returned vector is a pointer to the internal values managed by the [Accelerometer](#accelerometer) node, therefore it is illegal to free this pointer.
Furthermore, note that the pointed values are only valid until the next call to the `wb_robot_step` or `Robot::step` functions.
If these values are needed for a longer period they must be copied.

<!-- -->

> **Note** [Python]: The `getValues` function returns the 3D-vector as a list containing three floats.
