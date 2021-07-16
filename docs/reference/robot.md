## Robot

Derived from [Solid](solid.md).

```
Robot {
  SFString controller      "void"   # any string
  MFString controllerArgs  []       # any string
  SFString customData      ""       # any string
  SFBool   supervisor      FALSE    # {TRUE, FALSE}
  SFBool   synchronization TRUE     # {TRUE, FALSE}
  MFFloat  battery         []       # see below
  SFFloat  cpuConsumption  10       # [0, inf)
  SFBool   selfCollision   FALSE    # {TRUE, FALSE}
  SFBool   showWindow      FALSE    # {TRUE, FALSE}
  SFString window          ""       # any string
  SFString remoteControl   ""       # any string
}
```

### Description

The [Robot](#robot) node can be used as basis for building a robot, e.g., an articulated robot, a humanoid robot, a wheeled robot.

### Field Summary

- `controller`: name of the controller program that the simulator must use to control the robot.
This program is located in a directory whose name is equal to the field's value.
This directory is in turn located in the "controllers" subdirectory of the current project directory.
For example, if the field value is "my\_controller" then the controller program should be located in "my\_project/controllers/my\_controller/my\_controller[.exe]".
The ".exe" extension is added on the Windows platforms only.
If this field is left empty, the robot will run no controller at all.
Doing so may lead to better performance than using the `void` controller.
Setting the value of this field to `<extern>` will make this robot runnable from an [extern robot controller](../guide/running-extern-robot-controllers.md).

> **Note**: If the controller is not started the robot window will not work.
If the robot window is required it is recommended to assign the `void` controller instead of an empty string.

- `controllerArgs`: string containing the arguments (separated by space characters) to be passed to the `main` function of the C/C++ controller program or the `main` method of the Java controller program.

- `customData`: this field may contain any user data, for example parameters corresponding to the configuration of the robot.
It can be read from the robot controller using the `wb_robot_get_custom_data` function and can be written using the `wb_robot_set_custom_data` function.
It may also be used as a convenience for communicating between a robot and a supervisor without implementing a Receiver / Emitter system: The supervisor can read and write in this field using the generic supervisor functions for accessing fields.

- `supervisor`: if the value is `TRUE` the robot will have [supervisor capabilities](supervisor.md).
You will have to save and reload the simulation if you change this field from the scene tree, so that the new value is actually taken into account.

- `synchronization`: if the value is `TRUE` (default value), the simulator is synchronized with the controller; if the value is `FALSE`, the simulator runs as fast as possible, without waiting for the controller.
The `wb_robot_get_synchronization` function can be used to read the value of this field from a controller program.

- `battery`: this field should contain three values: the first one corresponds to the present energy level of the robot in Joules (*J*), the second is the maximum energy the robot can hold in Joules, and the third is the energy recharge speed in Watts (*[W]=[J]/[s]*).
The simulator updates the first value, while the other two remain constant.
*Important:* when the current energy value reaches zero, the corresponding controller process terminates and the simulated robot stops all motion.

> **Note**: *[J]=[V].[A].[s] and [J]=[V].[A.h]/3600*

- `cpuConsumption`: power consumption of the CPU (central processing unit) of the robot in Watts.

- `selfCollision`: setting this field to TRUE will enable the detection of collisions within the robot and apply the corresponding contact forces, so that the robot limbs cannot cross each other (provided that they have a [Physics](physics.md) node).
This is useful for complex articulated robots for which the controller doesn't prevent inner collisions.
Enabling self collision is, however, likely to decrease the simulation speed, as more collisions will be generated during the simulation.
Note that only collisions between non-consecutive solids will be detected.
For consecutive solids, e.g., two solids attached to each other with a joint, no collision detection is performed, even if the self collision is enabled.
Collision detection is also ignored for longer chains of consecutive solids in the event that all intermediary joints connecting the two colliding bodies all share the same `anchor` point.
If even just one of them is different, standard rules apply.
The reason for these exceptions is that these types of collision detections are usually not wanted by the user, because a very accurate design of the bounding objects of the solids would be required.
To prevent two consecutive solid nodes from penetrating each other, the `minStop` and `maxStop` fields of the corresponding joint node should be adjusted accordingly.
Here is an example of a robot leg with self collision enabled:

        Thigh (solid)
          |
        Knee (joint)
          |
        Leg (solid)
          |
        Ankle (joint)
          |
        Foot (solid)

    In this example, no collision is detected between the "Thigh" and the "Leg"
    solids because they are consecutive, e.g., directly joined by the "Knee". In the
    same way no collision detection takes place between the "Leg" and the "Foot"
    solids because they are also consecutive, e.g., directly joined by the "Ankle".
    However, collisions may be detected between the "Thigh" and the "Foot" solids,
    because they are non-consecutive, e.g., they are attached to each other through
    an intermediate solid ("Leg"). In such an example, it is probably a good idea to
    set `minStop` and `maxStop` values for the "Knee" and "Ankle" joints.

- `showWindow`: defines whether the robot window should be shown at the startup of the controller.

- `window`: defines the path of the robot window controller plugin used to display the robot window.
If the `window` field is empty, the default generic robot window is loaded.
The search algorithm works as following: Let $(VALUE) be the value of the `window` field, let $(EXT) be the shared library file extension of the OS (".so", ".dll" or ".dylib"), let $(PREFIX) be the shared library file prefix of the OS ("" on windows and "lib" on other OS), let $(PROJECT) be the current project path, let $(WEBOTS) be the webots installation path, and let $(...) be a recursive search, then the first existing file will be used as absolute path:

    $(PROJECT)/plugins/robot\_windows/$(VALUE)/$(PREFIX)$(VALUE)$(EXT)

    $(WEBOTS)/resources/$(...)/plugins/robot\_windows/$(VALUE)/$(PREFIX)$(VALUE)$(EXT)

- `remoteControl`: defines the path of the remote-control controller plugin used to remote control the real robot.
The search algorithm is identical to the one used for the `window` field, except that the subdirectory of `plugins` is `remote_controls` rather than `robot_windows`.

### Synchronous versus Asynchronous Controllers

The `synchronization` field specifies if a robot controller must be synchronized with the simulator or not.

If `synchronization` is `TRUE` (the default), the simulator will wait for the controller's `wb_robot_step` function call whenever necessary to keep the simulation and the controller synchronized.
So for example if the simulation step (`WorldInfo.basicTimeStep`) is 16 ms and the control step is 64 ms, then Webots will always execute precisely 4 simulation steps during one control step.
After the 4th simulation step, Webots will wait for the controller's next control step (call to `wb_robot_step(64)`).

If `synchronization` is `FALSE`, the simulator will run as fast a possible without waiting for the control step.
So for example, with the same simulation step (16 ms) and control step (64 ms) as before, if the simulator has finished the 4th simulation step but the controller has not yet reached the call to `wb_robot_step(64)`, then Webots will not wait; instead it will continue the simulation using the latest actuation commands.
Hence, if `synchronization` is `FALSE`, the number of simulation steps that are executed during a control step may vary; this will depend on the current simulator and controller speeds and on the current CPU load, and hence the outcome of the simulation may also vary.
Note that if the number of simulation steps per control step varies, this will appear as a variations of the "speed of the physics" in the controller's point of view, and this will appear as a variation of the robot's reaction speed in the user's point of view.

So generally the `synchronization` field should be set to `TRUE` when robust control is required.
For example if a motion (or ".motion file") was designed in synchronous mode then it may appear completely different in asynchronous mode.
The asynchronous mode is currently used only for the robot competitions, because in this case it is necessary to limit the CPU time allocated to each participating controller.
Note that it is also possible to combine synchronous and asynchronous controllers, e.g., for the robot competitions generally the [Supervisor](supervisor.md) controller is synchronous while the contestants controllers are asynchronous.
Asynchronous controllers may also be recommended for networked simulations involving several robots distributed over a computer network with an unpredictable delay (like the Internet).

### Robot Functions

#### `wb_robot_step`
#### `wb_robot_init`
#### `wb_robot_cleanup`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

int wb_robot_step(int duration);
void wb_robot_init();
void wb_robot_cleanup();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    Robot();
    virtual ~Robot();
    virtual int step(int duration);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def __init__(self):
    def __del__(self):
    def step(self, duration):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public Robot();
  protected void finalize();
  public int step(int duration);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
period = wb_robot_step(duration)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/time_step` | `service` | [`webots_ros::set_int`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*controller step, initialization and cleanup functions*

The `wb_robot_step` function is crucial and must be used in every controller.
This function synchronizes the sensor and actuator data between Webots and the controllers.
If the `wb_robot_step` function is not called then there will be no actuation in Webots and no update of the sensors in the controller.

The `duration` parameter specifies the amount of time, expressed in milliseconds, that must be simulated until the `wb_robot_step` function returns.
Note that this is not real time but virtual (simulation) time, so this is not like calling the system's `sleep` function.
Depending on the complexity of the simulation and execution mode, the function may return quickly.
When it returns, the requested duration of simulation time is elapsed.
In other words the physics runs for the specified duration: objects may move, the motors may run, the sensor values may change, etc.
Note that the `duration` parameter must be a multiple of the `WorldInfo.basicTimeStep`.

If this function returns -1, this indicates that Webots is about to terminate the controller.
This happens when the user hits the `Reload` button or quits Webots.
So if your code needs to do some cleanup, e.g., flushing or closing data files, etc., it is necessary to test this return value and take proper action.
The controller termination cannot be vetoed: one second later the controller is killed by Webots.
So only one second is available to do the cleanup.

If the `synchronization` field is TRUE, this function always returns 0 (or -1 to indicate termination).
If the `synchronization` field is FALSE, the return value can be different from 0: Let `controller_time` be the current time of the controller and let `dt` be the return value.
Then `dt` may be interpreted as follows:

- if `dt` = 0, then the asynchronous behavior was equivalent to the synchronous behavior.
- if 0 <= `dt` <= `duration`, then the actuator values were set at `controller_time` + `dt`, and the sensor values were measured at `controller_time` + `duration`, as requested.
It means that the step actually lasted the requested number of milliseconds, but the actuator commands could not be executed on time.
- if `dt` > `duration`, then the actuators values were set at `controller_time` + `dt`, and the sensor values were also measured at `controller_time` + `dt`.
It means that the requested step duration could not be respected.

The C API has two additional functions: `wb_robot_init` and `wb_robot_cleanup`.
There is no equivalent of the `wb_robot_init` and `wb_robot_cleanup` functions in the Java, Python, C++ and MATLAB APIs.
In these languages the necessary initialization and cleanup of the controller library is done automatically.

The `wb_robot_init` function is used to initialize the Webots controller library and enable the communication with the Webots simulator.
Note that the `wb_robot_init` function must be called before any other Webots API function.

Calling the `wb_robot_cleanup` function is the clean way to terminate a C controller.
This function frees the various resources allocated by Webots on the controller side.
In addition the `wb_robot_cleanup` function signals the termination of the controller to the simulator.
As a consequence, Webots removes the controller from the simulation which can continue normally with the execution of the other controllers (if any).
If a C controller exits without calling the `wb_robot_cleanup` function, then its termination will not be signaled to Webots.
In this case the simulation will remain blocked (sleeping) on the current step (but only if this [Robot](#robot)'s `synchronization` field is TRUE).
Note that the call to the `wb_robot_cleanup` function must be the last API function call in a C controller.
Any subsequent Webots API function call will give unpredictable results.

**Simple controller Example**

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>

#define TIME_STEP 32

static WbDeviceTag my_sensor, my_led;

int main() {
  /* initialize the webots controller library */
  wb_robot_init();

  // get device tags
  my_sensor = wb_robot_get_device("my_distance_sensor");
  my_led = wb_robot_get_device("my_led");

  /* enable sensors to read data from them */
  wb_distance_sensor_enable(my_sensor, TIME_STEP);

  /* main control loop: perform simulation steps of 32 milliseconds */
  /* and leave the loop when the simulation is over */
  while (wb_robot_step(TIME_STEP) != -1) {

    /* Read and process sensor data */
    double val = wb_distance_sensor_get_value(my_sensor);

    /* Send actuator commands */
    wb_led_set(my_led, 1);
  }

  /* Add here your own exit cleanup code */

  wb_robot_cleanup();

  return 0;
}
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.h>
#include <webots/DistanceSensor.h>
#include <webots/Led.h>

class MyController : public Robot {
public:
  MyController() {
    timeStep = 32;  // set the control time step

    // get device tags
    distanceSensor = getDistanceSensor("my_distance_sensor");
    led = getLed("my_led");

    distanceSensor->enable(timeStep);  // enable sensors to read data from them
  }

  void run() {
    // main control loop: perform simulation steps of 32 milliseconds
    // and leave the loop when the simulation is over
    while (step(timeStep) != -1) {
      double val = distanceSensor->getValue();  // Read and process sensor data
      led->set(1);                              // Send actuator commands
    }
  }

private:
  int timeStep;
  DistanceSensor *distanceSensor;
  Led *led;
}

// main C++ program
int main() {
  MyController *controller = new MyController();
  controller->run();
  delete controller;
  return 0;
}

```

%tab-end

%tab "Python"

```python
from controller import Robot

class MyController(Robot):
    def __init__(self):
        super(MyController, self).__init__()
        self.timeStep = 32  # set the control time step

        # get device tags
        self.distanceSensor = self.getDistanceSensor('my_distance_sensor')
        self.led = self.getLed('my_led')
        self.distanceSensor.enable(timeStep)  # enable sensors to read data from them

    def run(self):
        # main control loop: perform simulation steps of 32 milliseconds
        # and leave the loop when the simulation is over
        while self.step(self.timeStep) != -1:
            val = self.distanceSensor.getValue()  # Read and process sensor data
            self.led.set(1)                       # Send actuator commands

# main Python program
controller = MyController()
controller.run()
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;
import com.cyberbotics.webots.controller.DistanceSensor;
import com.cyberbotics.webots.controller.Led;

public class MyController extends Robot {
  public MyController() {
    timeStep = 32;  // set the control time step

    // get device tags
    distanceSensor = getDistanceSensor("my_distance_sensor");
    led = getLed("my_led");

    distanceSensor.enable(timeStep);  // enable sensors to read data from them
  }

  public void run() {
    // main control loop: perform simulation steps of 32 milliseconds
    // and leave the loop when the simulation is over
    while (step(timeStep) != -1) {
      double val = distanceSensor.getValue();  // Read and process sensor data
      led.set(1);                              // Send actuator commands
    }
  }

  private int timeStep;
  private DistanceSensor distanceSensor;
  private Led led;

  // main Java program
  public static void main(String[] args) {
    MyController controller = new MyController();
    controller.run();
  }
}
```

%tab-end

%tab "MATLAB"

```MATLAB

TIME_STEP = 32; % control time step

% get device tags
distanceSensor = wb_robot_get_device("my_distance_sensor");
led = wb_robot_get_device("my_led");

% enable sensors to read data from them
wb_distance_sensor_enable(distanceSensor, TIME_STEP);

% main control loop: perform simulation steps of 32 milliseconds
% and leave the loop when the simulation is over
while wb_robot_step(TIME_STEP) ~= -1
  val = wb_distance_sensor_get_value(distanceSensor);  % Read and process sensor data
  wb_led_set(led, 1);                                  % Send actuator commands
end
```

%tab-end


%end

---

#### `wb_robot_get_device`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

WbDeviceTag wb_robot_get_device(const char *name);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    Accelerometer *getAccelerometer(const std::string &name);
    Brake *getBrake(const std::string &name);
    Camera *getCamera(const std::string &name);
    Compass *getCompass(const std::string &name);
    Connector *getConnector(const std::string &name);
    Display *getDisplay(const std::string &name);
    DistanceSensor *getDistanceSensor(const std::string &name);
    Emitter *getEmitter(const std::string &name);
    GPS *getGPS(const std::string &name);
    Gyro *getGyro(const std::string &name);
    InertialUnit *getInertialUnit(const std::string &name);
    Joystick *getJoystick();
    Keyboard *getKeyboard();
    LED *getLED(const std::string &name);
    Lidar *getLidar(const std::string &name);
    LightSensor *getLightSensor(const std::string &name);
    Motor *getMotor(const std::string &name);
    Mouse *getMouse();
    Pen *getPen(const std::string &name);
    PositionSensor *getPositionSensor(const std::string &name);
    Radar *getRadar(const std::string &name);
    RangeFinder *getRangeFinder(const std::string &name);
    Receiver *getReceiver(const std::string &name);
    Speaker *getSpeaker(const std::string &name);
    TouchSensor *getTouchSensor(const std::string &name);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getDevice(self, name):
    def getJoystick(self):
    def getKeyboard(self):
    def getMouse(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public Accelerometer getAccelerometer(String name);
  public Brake getBrake(String name);
  public Camera getCamera(String name);
  public Compass getCompass(String name);
  public Connector getConnector(String name);
  public Display getDisplay(String name);
  public DistanceSensor getDistanceSensor(String name);
  public Emitter getEmitter(String name);
  public GPS getGPS(String name);
  public Gyro getGyro(String name);
  public InertialUnit getInertialUnit(String name);
  public Joystick getJoystick();
  public Keyboard getKeyboard();
  public LED getLED(String name);
  public Lidar getLidar(String name);
  public LightSensor getLightSensor(String name);
  public Motor getMotor(String name);
  public Motor getMouse();
  public Pen getPen(String name);
  public PositionSensor getPositionSensor(String name);
  public Radar getRadar(String name);
  public RangeFinder getRangeFinder(String name);
  public Receiver getReceiver(String name);
  public Speaker getSpeaker(String name);
  public TouchSensor getTouchSensor(String name);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
tag = wb_robot_get_device('name')
```

%tab-end

%tab "ROS"

> **Note**: this function has no equivalent for ROS.
Devices are available through their services.

%tab-end

%end

##### Description

*get a unique identifier to a device*

The `wb_robot_get_device` function (available in C, Python and MATLAB) returns a unique identifier for a device corresponding to a specified `name`.
For example, if a robot contains a [DistanceSensor](distancesensor.md) node whose `name` field is "ds1", the function will return the unique identifier of that device.
This `WbDeviceTag` identifier will be used subsequently for enabling, sending commands to, or reading data from this device.
If the specified device is not found, the function returns 0 in C and MATLAB or `None` in Python.

In C++ or Java, users should use the device specific typed methods, for example `getDistanceSensor`.
These functions return a reference to an object corresponding to a specified `name`.
Depending on the called function, this object can be an instance of a `Device` subclass.
For example, if a robot contains a [DistanceSensor](distancesensor.md) node whose `name` field is "ds1", the function `getDistanceSensor` will return a reference to a [DistanceSensor](distancesensor.md) object.
If the specified device is not found, the function returns `NULL` in C++ or `null` in Java.

---

#### `wb_robot_get_device_by_index`
#### `wb_robot_get_number_of_devices`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

WbDeviceTag wb_robot_get_device_by_index(int index);
int wb_robot_get_number_of_devices();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    int getNumberOfDevices() const;
    Device *getDeviceByIndex(int index);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getNumberOfDevices(self):
    def getDeviceByIndex(self, index):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public int getNumberOfDevices();
  public Device getDeviceByIndex(int index);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
size = wb_robot_get_number_of_devices()
tag = wb_robot_get_device_by_index(index)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_number_of_devices` | `service` | [`webots_ros::get_int`](ros-api.md#common-services) | |
| `/robot/get_device_list` | `service` | `webots_ros::robot_get_device_list` | `uint8 ask`<br/>`---`<br/>`string[] list` |

%tab-end

%end

##### Description

*get the devices by introspection*

These functions allows to get the robot devices by introspection.
Indeed they allow to get the devices from an internal flat list storing the devices.
The size of this list matches with the number of devices.
The order of this list matches with their declaration in the scene tree.

If `index` is out of the bounds of the list index (from `0` to `wb_robot_get_number_of_devices() - 1`) then the returned WbDeviceTag is equal to `0`.

The following example shows a typical example of introspection.
It is used with the device API allowing to retrieve some information from a WbDeviceTag.

```c
int n_devices = wb_robot_get_number_of_devices();
int i;
for(i=0; i<n_devices; i++) {
  WbDeviceTag tag = wb_robot_get_device_by_index(i);

  const char *name = wb_device_get_name(tag);
  WbNodeType type = wb_device_get_node_type(tag);

  // do something with the device
  printf("Device #%d name = %s\n", i, name);

  if (type == WB_NODE_CAMERA) {
    // do something with the camera
    printf("Device #%d is a camera\n", i);
  }
}
```

---

#### `wb_robot_wait_for_user_input_event`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

typedef enum {
  WB_EVENT_QUIT,
  WB_EVENT_NO_EVENT,
  WB_EVENT_MOUSE_CLICK,
  WB_EVENT_MOUSE_MOVE,
  WB_EVENT_KEYBOARD,
  WB_EVENT_JOYSTICK_BUTTON,
  WB_EVENT_JOYSTICK_AXIS,
  WB_EVENT_JOYSTICK_POV
} WbUserInputEvent;

WbUserInputEvent wb_robot_wait_for_user_input_event(WbUserInputEvent event_type, int timeout);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    typedef enum {
      EVENT_QUIT, EVENT_NO_EVENT, EVENT_MOUSE_CLICK, EVENT_MOUSE_MOVE, EVENT_KEYBOARD,
      EVENT_JOYSTICK_BUTTON, EVENT_JOYSTICK_AXIS, EVENT_JOYSTICK_POV
    } UserInputEvent;

    UserInputEvent waitForUserInputEvent(UserInputEvent event_type, int timeout);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    EVENT_QUIT, EVENT_NO_EVENT, EVENT_MOUSE_CLICK, EVENT_MOUSE_MOVE, EVENT_KEYBOARD, EVENT_JOYSTICK_BUTTON, EVENT_JOYSTICK_AXIS, EVENT_JOYSTICK_POV

    def waitForUserInputEvent(self, event_type, timeout):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public final static int EVENT_QUIT, EVENT_NO_EVENT, EVENT_MOUSE_CLICK,
     EVENT_MOUSE_MOVE, EVENT_KEYBOARD, EVENT_JOYSTICK_BUTTON,
     EVENT_JOYSTICK_AXIS, EVENT_JOYSTICK_POV;

  public int waitForUserInputEvent(int event_type, int timeout);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
WB_EVENT_QUIT, WB_EVENT_NO_EVENT, WB_EVENT_MOUSE_CLICK, WB_EVENT_MOUSE_MOVE, WB_EVENT_KEYBOARD, WB_EVENT_JOYSTICK_BUTTON, WB_EVENT_JOYSTICK_AXIS, WB_EVENT_JOYSTICK_POV

event_type = wb_robot_wait_for_user_input_event(event_type, timeout)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/wait_for_user_input_event` | `service` | `webots_ros::robot_wait_for_user_input_event` | `int32 eventType`<br/>`int32 timeout`<br/>`---`<br/>`int32 event` |

%tab-end

%end

##### Description

*wait for a Joystick, Keyboard or Mouse input event*

This function can be used to get [Joystick](joystick.md), [Keyboard](keyboard.md) and [Mouse](mouse.md) input without calling the `wb_robot_step` function, this is useful to prevent the simulation from running until a specific user input event occurs.
This function blocks the simulation and will return:
  - as soon as an event which type is defined by the `event_type` argument occurs (the list of available types is defined in [this table](#helper-enumeration-to-interpret-the-event_type-argument-and-return-value-of-the-wb_robot_wait_for_user_input_event-function)).
  - when the amount of milliseconds specified by the `timeout` argument has passed. This timeout is expressed in real time and not in simulation time.
  - when Webots is about to terminate the controller (in that case `WB_EVENT_QUIT` is returned).

It is possible to combine event types in order to return as soon as one of the event occurs:

```
WbUserInputEvent returned_event = wb_robot_wait_for_user_input_event(WB_EVENT_KEYBOARD | WB_EVENT_JOYSTICK_BUTTON, 1000);
```

> **note**: The corresponding input devices should be enabled before calling the `wb_robot_wait_for_user_input_event` function.
In case of mouse move and joystick axis events, the sampling period is used to avoid producing too many events (at least one sampling period is required before returning).
In that case, the sampling period is expressed in real time and not in simulation time.

%figure "Helper enumeration to interpret the event_type argument and return value of the `wb_robot_wait_for_user_input_event` function"

| Event                       | Purpose                                                   |
| --------------------------- | --------------------------------------------------------- |
| `WB_EVENT_QUIT`             | returned when Webots is about to terminate the controller |
| `WB_EVENT_NO_EVENT`         | no event occurred or no event should cause a return       |
| `WB_EVENT_MOUSE_CLICK`      | used to detect a mouse click in the 3D window             |
| `WB_EVENT_MOUSE_MOVE`       | used to detect the motion of the mouse in the 3D window   |
| `WB_EVENT_KEYBOARD`         | used to detect a keyboard key press/release               |
| `WB_EVENT_JOYSTICK_BUTTON`  | used to detect a joystick button press/release            |
| `WB_EVENT_JOYSTICK_AXIS`    | used to detect the motion of a joystick axis              |
| `WB_EVENT_JOYSTICK_POV`     | used to detect state change of a joystick pov             |

%end

> **note**: Calling the `wb_robot_wait_for_user_input_event` function with `WB_EVENT_NO_EVENT` as the `event_type` argument causes the controller process to sleep for the specified `timeout` duration. If the controller is synchronous, this will also pause the simulation for the same duration.

---

#### `wb_robot_battery_sensor_enable`
#### `wb_robot_battery_sensor_disable`
#### `wb_robot_get_battery_sampling_period`
#### `wb_robot_battery_sensor_get_value`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

void wb_robot_battery_sensor_enable(int sampling_period);
void wb_robot_battery_sensor_disable();
double wb_robot_battery_sensor_get_value();
int wb_robot_get_battery_sampling_period(WbDeviceTag tag);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    virtual void batterySensorEnable(int sampling_period);
    virtual void batterySensorDisable();
    int batterySensorGetSamplingPeriod() const;
    double batterySensorGetValue() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def batterySensorEnable(self, sampling_period):
    def batterySensorDisable(self):
    def batterySensorGetSamplingPeriod(self):
    def batterySensorGetValue(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public void batterySensorEnable(int sampling_period);
  public void batterySensorDisable();
  public int batterySensorGetSamplingPeriod();
  public double batterySensorGetValue();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_robot_battery_sensor_enable(sampling_period)
wb_robot_battery_sensor_disable()
period = wb_robot_battery_sensor_get_sampling_period()
value = wb_robot_battery_sensor_get_value()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/battery_sensor/value` | `topic` | webots_ros::Float64Stamped | [`Header`](http://docs.ros.org/api/std_msgs/html/msg/Header.html) `header`<br/>`float64 data` |
| `/battery_sensor/enable` | `service` | [`webots_ros::set_int`](ros-api.md#common-services) | |
| `/battery_sensor/get_sampling_period` | `service` | [`webots_ros::get_int`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*battery sensor function*

These functions allow you to measure the present energy level of the robot battery.
First, it is necessary to enable battery sensor measurements by calling the `wb_robot_battery_sensor_enable` function.
The `sampling_period` parameter is expressed in milliseconds and defines how frequently measurements are performed.
After the battery sensor is enabled a value can be read from it by calling the `wb_robot_battery_sensor_get_value` function.
The returned value corresponds to the present energy level of the battery expressed in Joules (*J*), if the `battery` field is empty, this function will return `-1.0`.

The `wb_robot_battery_sensor_disable` function should be used to stop battery sensor measurements.

The `wb_robot_get_battery_sampling_period` function returns the period given into the `wb_robot_battery_sensor_enable` function, or 0 if the device is disabled.

---

#### `wb_robot_get_basic_time_step`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

double wb_robot_get_basic_time_step();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    double getBasicTimeStep() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getBasicTimeStep(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public double getBasicTimeStep();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
step = wb_robot_get_basic_time_step()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_basic_time_step` | `service` | [`webots_ros::get_float`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*returns the value of the basicTimeStep field of the WorldInfo node*

This function returns the value of the `basicTimeStep` field of the [WorldInfo](worldinfo.md) node.

---

#### `wb_robot_get_mode`
#### `wb_robot_set_mode`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

typedef enum {
  WB_MODE_SIMULATION,
  WB_MODE_CROSS_COMPILATION,
  WB_MODE_REMOTE_CONTROL
} WbRobotMode;

WbRobotMode wb_robot_get_mode();
void wb_robot_set_mode(WbRobotMode mode, const char *arg);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    enum {
      MODE_SIMULATION,
      MODE_CROSS_COMPILATION,
      MODE_REMOTE_CONTROL
    } RobotMode;

    RobotMode getMode() const;
    void setMode(RobotMode mode, const char *arg);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    MODE_SIMULATION, MODE_CROSS_COMPILATION, MODE_REMOTE_CONTROL

    def getMode(self):
    def setMode(self, mode, arg);
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public final static int MODE_SIMULATION, MODE_CROSS_COMPILATION, MODE_REMOTE_CONTROL;

  public int getMode();
  public void setMode(int mode, String arg);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
WB_MODE_SIMULATION, WB_MODE_CROSS_COMPILATION, WB_MODE_REMOTE_CONTROL

mode = wb_robot_get_mode()
wb_robot_set_mode(mode, arg)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_mode` | `service` | [`webots_ros::get_int`](ros-api.md#common-services) | |
| `/robot/set_mode` | `service` | `webots_ros::robot_set_mode` | `char[] arg`<br/>`int32 mode`<br/>`---`<br/>`int8 success` |

%tab-end

%end

##### Description

*get operating mode, simulation versus real robot*

The `wb_robot_get_mode` function returns an integer value indicating the current operating mode for the controller.

The `wb_robot_set_mode` function allows the user to switch between the simulation and the remote control mode.
When switching to the remote-control mode, the `wbr_start` function of the remote control plugin is called.
The argument `arg` is passed directly to the `wbr_start` function (more information in the user guide).

The WbRobotMode can be compared to the following enumeration items:

%figure "Helper enumeration to interpret the WbRobotMode argument and return value of the `wb_robot_[gs]et_mode` functions"

| Mode                         | Purpose                |
| ---------------------------- | ---------------------- |
| `WB_MODE_SIMULATION`         | simulation mode        |
| `WB_MODE_CROSS_COMPILATION`  | cross compilation mode |
| `WB_MODE_REMOTE_CONTROL`     | remote control mode    |

%end

---

#### `wb_robot_get_name`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

const char *wb_robot_get_name();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    std::string getName() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getName(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public String getName();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
name = wb_robot_get_name()
```

%tab-end

%end

##### Description

*return the name defined in the robot node*

This function returns the name as it is defined in the name field of the robot node (Robot, Supervisor, etc.) in the current world file.
The string returned should not be deallocated, as it was allocated by the "libController" shared library and will be deallocated when the controller terminates.
This function is very useful to pass some arbitrary parameter from a world file to a controller program.
For example, you can have the same controller code behave differently depending on the name of the robot.
This is illustrated in the "soccer.wbt" sample demo, where the goal keeper robot runs the same control code as the other soccer players, but its behavior is different because its name was tested to determine its behavior (in this sample world, names are "b3" for the blue goal keeper and "y3" for the yellow goal keeper, whereas the other players are named "b1", "b2", "y1" and "y2").
This sample world is located in the "projects/samples/demos/worlds" directory of Webots.

---

#### `wb_robot_get_model`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

const char *wb_robot_get_model();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    std::string getModel() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getModel(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public String getModel();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
model = wb_robot_get_model()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_model` | `service` | [`webots_ros::get_string`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*return the model defined in the robot node*

This function returns the model string as it is defined in the model field of the robot node (Robot, Supervisor, etc.) in the current world file.
The string returned should not be deallocated, as it was allocated by the "libController" shared library and will be deallocated when the controller terminates.

---

#### `wb_robot_get_custom_data`
#### `wb_robot_set_custom_data`

 - *set the data defined in the robot node*

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

const char *wb_robot_get_custom_data();
void wb_robot_set_custom_data(const char *data);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    std::string getCustomData() const;
    void setCustomData(const std::string &data);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getCustomData(self):
    def setCustomData(self, data):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public String getCustomData();
  public setCustomData(String data);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
data = wb_robot_get_custom_data()
wb_robot_set_custom_data('data')
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_custom_data` | `service` | [`webots_ros::get_string`](ros-api.md#common-services) | |
| `/robot/set_custom_data` | `service` | [`webots_ros::set_string`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*return the custom data defined in the robot node*

The `wb_robot_get_custom_data` function returns the string contained in the `customData` field of the robot node.

The `wb_robot_set_custom_data` function set the string contained in the `customData` field of the robot node.

---

#### `wb_robot_get_type`

%tab-component "language"

%tab "C"

```c
#include <webots/nodes.h>
#include <webots/robot.h>

WbNodeType wb_robot_get_type();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    int getType() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getType(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public int getType();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
type = wb_robot_get_type()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_type` | `service` | [`webots_ros::get_int`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*return the type of the robot node*

This function returns the type of the current mode (WB\_NODE\_ROBOT or WB\_NODE\_SUPERVISOR).

---

#### `wb_robot_get_project_path`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

const char *wb_robot_get_project_path();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    std::string getProjectPath() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getProjectPath(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public String getProjectPath();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
path = wb_robot_get_project_path()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_project_path` | `service` | [`webots_ros::get_string`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*return the full path of the current project*

This function returns the full path of the current project, that is the directory which contains the worlds and controllers subdirectories (among others) of the current simulation world.
It doesn't include the final directory separator char (slash or anti-slash).
The returned pointer is a UTF-8 encoded char string.
It should not be deallocated.

---

#### `wb_robot_get_world_path`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

const char *wb_robot_get_world_path();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    std::string getWorldPath() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getWorldPath(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public String getWorldPath();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
path = wb_robot_get_world_path()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_world_path` | `service` | [`webots_ros::get_string`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*return the full path of the current opened world file*

This function returns the full path of the current opened world.
The returned pointer is a UTF-8 encoded char string which does include the final ".wbt".
It should not be deallocated.

---

#### `wb_robot_get_supervisor`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

bool wb_robot_get_supervisor();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    bool getSupervisor() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getSupervisor(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public boolean getSupervisor();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
sync = wb_robot_get_supervisor()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_supervisor` | `service` | [`webots_ros::get_bool`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*return the value of the supervisor field of the Robot node*

This function returns the boolean value corresponding to the supervisor field of the Robot node.
This function can be used to determine whether it is allowed to use the [Supervisor API](supervisor.md) or not.

---

#### `wb_robot_get_synchronization`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

bool wb_robot_get_synchronization();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    bool getSynchronization() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getSynchronization(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public boolean getSynchronization();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
sync = wb_robot_get_synchronization()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_synchronization` | `service` | [`webots_ros::get_bool`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*return the value of the synchronization field of the Robot node*

This function returns the boolean value corresponding to the synchronization field of the Robot node.

---

#### `wb_robot_get_time`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

double wb_robot_get_time();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    double getTime() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getTime(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public double getTime();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
time = wb_robot_get_time()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_time` | `service` | [`webots_ros::get_float`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*return the current simulation time in seconds*

This function returns the current simulation time in seconds.
This correspond to the simulation time displayed in the speedometer located in the main toolbar.
It does not matter whether the controller is synchronized or not.

---

#### `wb_robot_task_new`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

void wb_robot_task_new(void (*task, void *param);
```

%tab-end

%end

##### Description

*start a new thread of execution*

This function creates and starts a new thread of execution for the robot controller.
The `task` function is immediately called using the `param` parameter.
It will end only when the `task` function returns.
The Webots controller API is thread safe, however, some API functions use or return pointers to data structures which are not protected outside the function against asynchronous access from a different thread.
Hence you should use mutexes (see below) to ensure that such data is not accessed by a different thread.

---

#### `wb_robot_mutex_new`
#### `wb_robot_mutex_delete`
#### `wb_robot_mutex_lock`
#### `wb_robot_mutex_unlock`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

WbMutexRef wb_robot_mutex_new();
void wb_robot_mutex_delete(WbMutexRef mutex);
void wb_robot_mutex_lock(WbMutexRef mutex);
void wb_robot_mutex_unlock(WBMutexRef mutex);
```

%tab-end

%end

##### Description

*mutex functions*

The `wb_robot_mutex_new` function creates a new mutex and returns a reference to that mutex to be used with other mutex functions.
A newly created mutex is always initially unlocked.
Mutexes (mutual excluders) are useful with multi-threaded controllers to protect some resources (typically variables or memory chunks) from being used simultaneously by different threads.

The `wb_robot_mutex_delete` function deletes the specified `mutex`.
This function should be used when a mutex is no longer in use.

The `wb_robot_mutex_lock` function attempts to lock the specified `mutex`.
If the mutex is already locked by another thread, this function waits until the other thread unlocks the mutex, and then locks it.
This function returns only after it has locked the specified `mutex`.

The `wb_robot_mutex_unlock` function unlocks the specified `mutex`, allowing other threads to lock it.

Users unfamiliar with the mutex concept may wish to consult a reference on multi-threaded programming techniques for further information.

---

#### `wb_robot_get_urdf`

%tab-component "language"

%tab "C"

```c
#include <webots/robot.h>

const char *wb_robot_get_urdf(const char *prefix);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    std::string getUrdf(std::string prefix="");
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def getUrdf(self, prefix=''):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public String getUrdf(String prefix);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_robot_get_urdf(prefix)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/get_urdf` | `service` | `webots_ros::get_urdf` | `string prefix`<br/>`---`<br/>`string value` |

%tab-end

%end

##### Description

The `wb_robot_get_urdf` function allows a robot controller to export [URDF](http://wiki.ros.org/urdf), an XML format for representing a robot model.
A prefix for URDF link and joint names can be specified by `prefix` (useful multi-robot systems to distinguish different robots).
The function is particularly useful for ROS applications in which URDF is widely used to describe robot models.
There are certain rules that are followed to create an efficient output.
Webots nodes are squashed into a single URDF link node whenever possible to simplify the exported robot model.
In case you want a Webots node to be shown as a separate URDF link it is enough to define its `name` field.
URDF links inherit the `name` field from Webots node except when there is no `name` field defined or there are two or more Webots nodes with the name.
URDF joints are named after position sensor in the corresponding Webots joints.

> **Note**: Exported URDF is not complete.
Currently, the generated URDF consists of a minimal number of elements to be used with [`robot_state_publisher`](http://wiki.ros.org/robot_state_publisher).
Only the [Box](box.md), [Capsule](capsule.md), [Cylinder](cylinder.md) and [Sphere](sphere.md) nodes in the `boundingObject` field of [Solid](solid.md) nodes will be exported (both for `visual` and `collision`), the URDF file can therefore be visualized in [RViz](http://wiki.ros.org/rviz).
Also note that [Hinge2Joint](./hinge2joint.md) and [BallJoint](./balljoint.md) are not supported.
The URDF joints are named according to the [Joint](joint.md) [Motor](motor.md) names (or [PositionSensor](positionsensor.md) if the [Joint](joint.md) has no [Motor](motor.md)).

---

#### `wb_robot_wwi_receive`
#### `wb_robot_wwi_receive_text`
#### `wb_robot_wwi_send`
#### `wb_robot_wwi_send_text`

%tab-component "language"

%tab "C"

```c
#include <webots/plugins/robot_window/default.h>

const char *wb_robot_wwi_receive(int *size);
const char *wb_robot_wwi_receive_text();
void wb_robot_wwi_send(const char *data, int size);
void wb_robot_wwi_send_text(const char *text);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Robot.hpp>

namespace webots {
  class Robot {
    const char *wwiReceive(int *size);
    std::string wwiReceiveText();
    void wwiSend(const char *data, int size);
    void wwiSendText(const std::string &text);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Robot

class Robot:
    def wwiSendText(self, text):
    def wwiReceiveText(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Robot;

public class Robot {
  public void wwiSendText(String text);
  public String wwiReceiveText();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_robot_wwi_send_text(text)
text = wb_robot_wwi_receive_text()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/robot/wwi_receive_text` | `service` | [`webots_ros::get_string`](ros-api.md#common-services) | |
| `/robot/wwi_send_text` | `service` | [`webots_ros::set_string`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*communication with a HTML robot window*

These functions allow the robot controller to communicate with a HTML robot window.
Such a window is embedded as a dockable sub-window in the Webots user interface.
The content of the window is written in HTML and JavaScript functions are used to communicate with the robot controller.

The `wb_robot_wwi_receive` and `wb_robot_wwi_receive_text` functions allow a robot controller to receive a message sent from a JavaScript function running in the HTML robot window.
The message is sent using the `webots.window("<robot window name>").send` method of the Webots JavaScript API.

The `wb_robot_window_send` and `wb_robot_wwi_send_text` functions allow a robot controller to send a message to a JavaScript function running in the HTML robot window.
The message is received using the `webots.window("<robot window name>").receive` method of the Webots JavaScript API.

> **note** [Java, Python, MATLAB, ROS]: `wb_robot_wwi_receive` and `wb_robot_window_send` functions are not available in the Java, Python, MATLAB, or ROS API.
