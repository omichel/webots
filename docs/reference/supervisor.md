## Supervisor

The [Supervisor](#supervisor) is not a node, it is a set of functions available for each [Robot](robot.md) node whose `supervisor` field is set to `TRUE`.
The [Supervisor API](#supervisor) can be used to access to extra functions that are not available to a regular [Robot](robot.md).

> **Note**: Note that in some special cases the [Supervisor](#supervisor) functions might return wrong values and it might not be possible to retrieve fields and nodes.
This occurs when closing a world and quitting its controllers, i.e. reloading the current world, opening a new world, or closing Webots.
In this case the return value will be a NULL pointer or a default value.
For functions returning a string, an empty string is returned instead of a NULL pointer.

### Supervisor Functions

As for a regular [Robot](robot.md) controller, the `wb_robot_init`, `wb_robot_step`, etc. functions must be used in a [Supervisor](#supervisor) controller.

#### `wb_supervisor_node_get_root`
#### `wb_supervisor_node_get_self`
#### `wb_supervisor_node_get_from_def`
#### `wb_supervisor_node_get_from_id`
#### `wb_supervisor_node_get_from_device`
#### `wb_supervisor_node_get_selected`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

WbNodeRef wb_supervisor_node_get_root();
WbNodeRef wb_supervisor_node_get_self();
WbNodeRef wb_supervisor_node_get_from_def(const char *def);
WbNodeRef wb_supervisor_node_get_from_id(int id);
WbNodeRef wb_supervisor_node_get_from_device(WbDeviceTag tag);
WbNodeRef wb_supervisor_node_get_selected();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    Node *getRoot();
    Node *getSelf();
    Node *getFromDef(const std::string &name);
    Node *getFromId(int id);
    Node *getFromDevice(const Device *device);
    Node *getSelected();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def getRoot(self):
    def getSelf(self):
    def getFromDef(self, name):
    def getFromId(self, id):
    def getFromDevice(self, device);
    def getSelected(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public Node getRoot();
  public Node getSelf();
  public Node getFromDef(String name);
  public Node getFromId(int id);
  public Node getFromDevice(Device device);
  public Node getSelected();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
node = wb_supervisor_node_get_root()
node = wb_supervisor_node_get_self()
node = wb_supervisor_node_get_from_def('def')
node = wb_supervisor_node_get_from_id(id)
node = wb_supervisor_node_get_from_device(tag)
node = wb_supervisor_node_get_selected()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/get_root` | `service` | [`webots_ros::get_uint64`](ros-api.md#common-services) | |
| `/supervisor/get_self` | `service` | [`webots_ros::get_uint64`](ros-api.md#common-services) | |
| `/supervisor/get_from_def` | `service` | `webots_ros::supervisor_get_from_def` | `string name`<br/>`uint64 proto`<br/>`---`<br/>`uint64 node` |
| `/supervisor/get_from_id` | `service` | `webots_ros::supervisor_get_from_id` | `int32 id`<br/>`---`<br/>`uint64 node` |
| `/supervisor/get_from_device` | `service` | `webots_ros::supervisor_get_from_string` | `string value`<br/>`---`<br/>`uint64 node` |
| `/supervisor/get_selected` | `service` | [`webots_ros::get_uint64`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*get a handle to a node in the world*

The `wb_supervisor_node_get_from_def` function returns a handle to a node in the world from its DEF name.
The return value can be used for subsequent calls to functions which require a `WbNodeRef` parameter.
If the requested node does not exist in the current world file or is an internal node of a PROTO, the function returns NULL.
If a handle to an internal node of a PROTO should be retrieved, the `wb_supervisor_node_get_from_proto_def` should be used instead.

Note that in case of [procedural PROTO nodes](procedural-proto-nodes.md) regeneration, nodes retrieved by `wb_supervisor_node_get_from_proto_def` might become invalid as well as all the descendant nodes and fields.
In this case, after triggering the PROTO regeneration, the nodes and fields should be retrieved again using the [Supervisor](#supervisor) API functions.

It is possible to use dots (.) as scoping operator in the DEF parameter.
Dots can be used when looking for a specific node path in the node hierarchy.
For example:

```c
WbNodeRef node = wb_supervisor_node_get_from_def("ROBOT.JOINT.SOLID");
```

This means that we are searching for a node named "SOLID" inside a node named "JOINT", inside a node named "ROBOT".

The `wb_supervisor_node_get_from_id` function retrieves a handle to a node, but from its unique identifier (the `id` parameter).
The function returns NULL if the given identifier doesn't match with any node of the current world.
It is recommended to use this function only when knowing formerly the identifier (rather than looping on this function to retrieve all the nodes of a world).
For example, when exporting an X3D file, its XML nodes are containing an `id` attribute which matches with the unique identifier described here.

The `wb_supervisor_node_get_from_device` function retrieves the node's handle for a [Device](device.md) object.
The function returns NULL if the given device is invalid or is an internal node of a PROTO.
Note that in the ROS API the device name has to be used to retrieve the handle to the node.

The `wb_supervisor_node_get_root` function returns a handle to the root node which is actually a [Group](group.md) node containing all the nodes visible at the top level in the scene tree window of Webots.
Like any [Group](group.md) node, the root node has a MFNode field called "children" which can be parsed to read each node in the scene tree.
An example of such a usage is provided in the "supervisor.wbt" sample worlds (located in the "projects/samples/devices/worlds" directory of Webots.

The `wb_supervisor_node_get_self` function returns a handle to the [Robot](robot.md) node itself on which the controller is run.
This is a utility function that simplifies the task of retrieving the base node without having to define a DEF name for it.

The `wb_supervisor_node_get_selected` function returns a handle to the currently selected node in the scene tree.
If no node is currently selected, the function returns NULL.

---

#### `wb_supervisor_node_get_def`
#### `wb_supervisor_node_get_id`
#### `wb_supervisor_node_get_parent_node`
#### `wb_supervisor_node_is_proto`
#### `wb_supervisor_node_get_from_proto_def`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

const char *wb_supervisor_node_get_def(WbNodeRef node);
int wb_supervisor_node_get_id(WbNodeRef node);
WbNodeRef wb_supervisor_node_get_parent_node(WbNodeRef node);
bool wb_supervisor_node_is_proto(WbNodeRef node);
WbNodeRef wb_supervisor_node_get_from_proto_def(WbNodeRef node, const char *def);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    int getId() const;
    std::string getDef() const;
    Node *getParentNode() const;
    bool isProto() const;
    Node *getFromProtoDef(const std::string &name);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def getId(self):
    def getDef(self):
    def getParentNode(self):
    def isProto(self):
    def getFromProtoDef(self, name):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public int getId();
  public String getDef();
  public Node getParentNode();
  public boolean isProto();
  public Node getFromProtoDef(String name);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
id = wb_supervisor_node_get_id(node)
s = wb_supervisor_node_get_def(node)
node = wb_supervisor_node_get_parent_node(node)
b = wb_supervisor_node_is_proto(node)
node = wb_supervisor_node_get_from_proto_def(node, 'def')
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/get_id` | `service` | `webots_ros::node_get_id` | `uint64 node`<br/>`---`<br/>`int32 id` |
| `/supervisor/node/get_def` | `service` | `webots_ros::node_get_name` | `uint64 node`<br/>`---`<br/>`string name` |
| `/supervisor/node/get_parent_node` | `service` | `webots_ros::node_get_parent_node` | `uint64 node`<br/>`---`<br/>`uint64 node` |
| `/supervisor/node/is_proto` | `service` | `webots_ros::node_is_proto` | `uint64 node`<br/>`---`<br/>`bool value` |
| `/supervisor/get_from_def` | `service` | `webots_ros::supervisor_get_from_def` | `string name`<br/>`uint64 proto`<br/>`---`<br/>`uint64 node` |

%tab-end

%end

##### Description

*get node info*

The `wb_supervisor_node_get_def` function retrieves the DEF name of the node passed as a parameter.
If no DEF name is specified, this function returns the empty string.

The `wb_supervisor_node_get_id` function retrieves the unique identifier of the node given in parameter.

The `wb_supervisor_node_get_parent_node` function retrieves the reference to the direct parent node of the node given in parameter.

The `wb_supervisor_node_is_proto` function returns `true` if the node given in the argument is a [PROTO node](proto.md).

The `wb_supervisor_node_get_from_proto_def` function returns a handle to a node defined in the [PROTO body](proto-definition.md) of the specified node from its DEF name.
Note that this function works only for PROTO nodes.

---

#### `wb_supervisor_node_get_type`
#### `wb_supervisor_node_get_type_name`
#### `wb_supervisor_node_get_base_type_name`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>
#include <webots/node.h>

typedef enum {
  WB_NODE_NO_NODE,
  /* 3D rendering */
  WB_NODE_APPEARANCE,
  WB_NODE_BACKGROUND,
  WB_NODE_BILLBOARD,
  WB_NODE_BOX,
  WB_NODE_CAPSULE,
  WB_NODE_COLOR,
  WB_NODE_CONE,
  WB_NODE_COORDINATE,
  WB_NODE_CYLINDER,
  WB_NODE_DIRECTIONAL_LIGHT,
  WB_NODE_ELEVATION_GRID,
  WB_NODE_FOG,
  WB_NODE_GROUP,
  WB_NODE_IMAGE_TEXTURE,
  WB_NODE_INDEXED_FACE_SET,
  WB_NODE_INDEXED_LINE_SET,
  WB_NODE_MATERIAL,
  WB_NODE_MESH,
  WB_NODE_MUSCLE,
  WB_NODE_NORMAL,
  WB_NODE_PBR_APPEARANCE,
  WB_NODE_PLANE,
  WB_NODE_POINT_LIGHT,
  WB_NODE_POINT_SET,
  WB_NODE_SHAPE,
  WB_NODE_SPHERE,
  WB_NODE_SPOT_LIGHT,
  WB_NODE_TEXTURE_COORDINATE,
  WB_NODE_TEXTURE_TRANSFORM,
  WB_NODE_TRANSFORM,
  WB_NODE_VIEWPOINT,
  /* robots */
  WB_NODE_ROBOT,
  /* devices */
  WB_NODE_ACCELEROMETER,
  WB_NODE_BRAKE,
  WB_NODE_CAMERA,
  WB_NODE_COMPASS,
  WB_NODE_CONNECTOR,
  WB_NODE_DISPLAY,
  WB_NODE_DISTANCE_SENSOR,
  WB_NODE_EMITTER,
  WB_NODE_GPS,
  WB_NODE_GYRO,
  WB_NODE_INERTIAL_UNIT,
  WB_NODE_LED,
  WB_NODE_LIDAR,
  WB_NODE_LIGHT_SENSOR,
  WB_NODE_LINEAR_MOTOR,
  WB_NODE_PEN,
  WB_NODE_POSITION_SENSOR,
  WB_NODE_PROPELLER,
  WB_NODE_RADAR,
  WB_NODE_RANGE_FINDER,
  WB_NODE_RECEIVER,
  WB_NODE_ROTATIONAL_MOTOR,
  WB_NODE_SPEAKER,
  WB_NODE_TOUCH_SENSOR,
  /* misc */
  WB_NODE_BALL_JOINT,
  WB_NODE_BALL_JOINT_PARAMETERS,
  WB_NODE_CHARGER,
  WB_NODE_CONTACT_PROPERTIES,
  WB_NODE_DAMPING,
  WB_NODE_FLUID,
  WB_NODE_FOCUS,
  WB_NODE_HINGE_JOINT,
  WB_NODE_HINGE_JOINT_PARAMETERS,
  WB_NODE_HINGE_2_JOINT,
  WB_NODE_IMMERSION_PROPERTIES,
  WB_NODE_JOINT_PARAMETERS,
  WB_NODE_LENS,
  WB_NODE_LENS_FLARE,
  WB_NODE_PHYSICS,
  WB_NODE_RECOGNITION,
  WB_NODE_SLIDER_JOINT,
  WB_NODE_SLOT,
  WB_NODE_SOLID,
  WB_NODE_SOLID_REFERENCE,
  WB_NODE_TRACK,
  WB_NODE_TRACK_WHEEL,
  WB_NODE_WORLD_INFO,
  WB_NODE_ZOOM
} WbNodeType;

WbNodeType wb_supervisor_node_get_type(WbNodeRef node);
const char *wb_supervisor_node_get_type_name(WbNodeRef node);
const char *wb_supervisor_node_get_base_type_name(WbNodeRef node);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    typedef enum {
      NO_NODE,
      // 3D rendering
      APPEARANCE, BACKGROUND, BILLBOARD, BOX, CAPSULE, COLOR, CONE, COORDINATE,
      CYLINDER, DIRECTIONAL_LIGHT, ELEVATION_GRID, FOG, GROUP, IMAGE_TEXTURE,
      INDEXED_FACE_SET, INDEXED_LINE_SET, MATERIAL, MESH, MUSCLE, NORMAL, PBR_APPEARANCE,
      PLANE, POINT_LIGHT, POINT_SET, SHAPE, SPHERE, SPOT_LIGHT, TEXTURE_COORDINATE,
      TEXTURE_TRANSFORM, TRANSFORM, VIEWPOINT,
      // robots
      ROBOT,
      // devices
      ACCELEROMETER, BRAKE, CAMERA, COMPASS, CONNECTOR, DISPLAY,
      DISTANCE_SENSOR, EMITTER, GPS, GYRO, INERTIAL_UNIT, LED, LIDAR,
      LIGHT_SENSOR, LINEAR_MOTOR, PEN, POSITION_SENSOR, PROPELLER,
      RADAR, RANGE_FINDER, RECEIVER, ROTATIONAL_MOTOR, SPEAKER, TOUCH_SENSOR,
      // misc
      BALL_JOINT, BALL_JOINT_PARAMETERS, CHARGER, CONTACT_PROPERTIES,
      DAMPING, FLUID, FOCUS, HINGE_JOINT, HINGE_JOINT_PARAMETERS, HINGE_2_JOINT,
      IMMERSION_PROPERTIES, JOINT_PARAMETERS, LENS, LENS_FLARE, PHYSICS, RECOGNITION,
      SLIDER_JOINT, SLOT, SOLID, SOLID_REFERENCE, TRACK, TRACK_WHEEL, WORLD_INFO, ZOOM
    } Type;

    Type getType() const;
    std::string getTypeName() const;
    std::string getBaseTypeName() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    NO_NODE,
    # 3D rendering
    APPEARANCE, BACKGROUND, BILLBOARD, BOX, CAPSULE, COLOR, CONE, COORDINATE,
    CYLINDER, DIRECTIONAL_LIGHT, ELEVATION_GRID, FOG, GROUP, IMAGE_TEXTURE,
    INDEXED_FACE_SET, INDEXED_LINE_SET, MATERIAL, MESH, MUSCLE, NORMAL, PBR_APPEARANCE,
    PLANE, POINT_LIGHT, POINT_SET, SHAPE, SPHERE, SPOT_LIGHT, TEXTURE_COORDINATE,
    TEXTURE_TRANSFORM, TRANSFORM, VIEWPOINT,
    # robots
    ROBOT,
    # devices
    ACCELEROMETER, BRAKE, CAMERA, COMPASS, CONNECTOR, DISPLAY,
    DISTANCE_SENSOR, EMITTER, GPS, GYRO, INERTIAL_UNIT, LED, LIDAR,
    LIGHT_SENSOR, LINEAR_MOTOR, PEN, POSITION_SENSOR, PROPELLER,
    RADAR, RANGE_FINDER, RECEIVER, ROTATIONAL_MOTOR, SPEAKER, TOUCH_SENSOR,
    # misc
    BALL_JOINT, BALL_JOINT_PARAMETERS, CHARGER, CONTACT_PROPERTIES,
    DAMPING, FLUID, FOCUS, HINGE_JOINT, HINGE_JOINT_PARAMETERS, HINGE_2_JOINT,
    IMMERSION_PROPERTIES, JOINT_PARAMETERS, LENS, LENS_FLARE, PHYSICS, RECOGNITION,
    SLIDER_JOINT, SLOT, SOLID, SOLID_REFERENCE, TRACK, TRACK_WHEEL, WORLD_INFO, ZOOM

    def getType(self):
    def getTypeName(self):
    def getBaseTypeName(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public final static int
    NO_NODE,
    // 3D rendering
    APPEARANCE, BACKGROUND, BILLBOARD, BOX, CAPSULE, COLOR, CONE, COORDINATE,
    CYLINDER, DIRECTIONAL_LIGHT, ELEVATION_GRID, FOG, GROUP, IMAGE_TEXTURE,
    INDEXED_FACE_SET, INDEXED_LINE_SET, MATERIAL, MESH, MUSCLE, NORMAL, PBR_APPEARANCE,
    PLANE, POINT_LIGHT, POINT_SET, SHAPE, SPHERE, SPOT_LIGHT, TEXTURE_COORDINATE,
    TEXTURE_TRANSFORM, TRANSFORM, VIEWPOINT,
    // robots
    ROBOT,
    // devices
    ACCELEROMETER, BRAKE, CAMERA, COMPASS, CONNECTOR, DISPLAY,
    DISTANCE_SENSOR, EMITTER, GPS, GYRO, INERTIAL_UNIT, LED, LIDAR,
    LIGHT_SENSOR, LINEAR_MOTOR, PEN, POSITION_SENSOR, PROPELLER,
    RADAR, RANGE_FINDER, RECEIVER, ROTATIONAL_MOTOR, SPEAKER, TOUCH_SENSOR,
    // misc
    BALL_JOINT, BALL_JOINT_PARAMETERS, CHARGER, CONTACT_PROPERTIES,
    DAMPING, FLUID, FOCUS, HINGE_JOINT, HINGE_JOINT_PARAMETERS, HINGE_2_JOINT,
    IMMERSION_PROPERTIES, JOINT_PARAMETERS, LENS, LENS_FLARE, PHYSICS, RECOGNITION,
    SLIDER_JOINT, SLOT, SOLID, SOLID_REFERENCE, TRACK, TRACK_WHEEL, WORLD_INFO, ZOOM;

  public int getType();
  public String getTypeName();
  public String getBaseTypeName();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
WB_NODE_NO_NODE,
% 3D rendering
WB_NODE_APPEARANCE, WB_NODE_BACKGROUND, WB_NODE_BILLBOARD, WB_NODE_BOX, WB_NODE_CAPSULE,
WB_NODE_COLOR, WB_NODE_CONE, WB_NODE_COORDINATE,
WB_NODE_CYLINDER, WB_NODE_DIRECTIONAL_LIGHT, WB_NODE_ELEVATION_GRID, WB_NODE_FOG,
WB_NODE_GROUP, WB_NODE_IMAGE_TEXTURE, WB_NODE_INDEXED_FACE_SET, WB_NODE_INDEXED_LINE_SET,
WB_NODE_MATERIAL, WB_NODE_MESH, WB_NODE_MUSCLE, WB_NODE_NORMAL,
WB_NODE_PBR_APPEARANCE, WB_NODE_PLANE, WB_NODE_POINT_LIGHT, WB_NODE_POINT_SET,
WB_NODE_SHAPE, WB_NODE_SPHERE, WB_NODE_SPOT_LIGHT, WB_NODE_TEXTURE_COORDINATE,
WB_NODE_TEXTURE_TRANSFORM, WB_NODE_TRANSFORM, WB_NODE_VIEWPOINT,
% robots
WB_NODE_ROBOT,
% devices
WB_NODE_ACCELEROMETER, WB_NODE_BRAKE, WB_NODE_CAMERA, WB_NODE_COMPASS,
WB_NODE_CONNECTOR, WB_NODE_DISPLAY, WB_NODE_DISTANCE_SENSOR, WB_NODE_EMITTER,
WB_NODE_GPS, WB_NODE_GYRO, WB_NODE_INERTIAL_UNIT, WB_NODE_LED, WB_NODE_LIDAR,
WB_NODE_LIGHT_SENSOR, WB_NODE_LINEAR_MOTOR, WB_NODE_PEN,
WB_NODE_POSITION_SENSOR, WB_NODE_PROPELLER, WB_NODE_RADAR,
WB_NODE_RANGE_FINDER, WB_NODE_RECEIVER, WB_NODE_ROTATIONAL_MOTOR,
WB_NODE_SPEAKER, WB_NODE_TOUCH_SENSOR,
% misc
WB_NODE_BALL_JOINT, WB_NODE_BALL_JOINT_PARAMETERS, WB_NODE_CHARGER,
WB_NODE_CONTACT_PROPERTIES, WB_NODE_DAMPING, WB_NODE_FLUID,
WB_NODE_FOCUS, WB_NODE_HINGE_JOINT, WB_NODE_HINGE_JOINT_PARAMETERS,
WB_NODE_HINGE_2_JOINT, WB_NODE_IMMERSION_PROPERTIES, WB_NODE_JOINT_PARAMETERS,
WB_NODE_LENS, WB_NODE_LENS_FLARE, WB_NODE_PHYSICS, WB_NODE_RECOGNITION,
WB_NODE_SLIDER_JOINT, WB_NODE_SLOT, WB_NODE_SOLID, WB_NODE_SOLID_REFERENCE,
WB_NODE_TRACK, WB_NODE_TRACK_WHEEL, WB_NODE_WORLD_INFO, WB_NODE_ZOOM

type = wb_supervisor_node_get_type(node)
name = wb_supervisor_node_get_type_name(node)
name = wb_supervisor_node_get_base_type_name(node)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/get_type` | `service` | `webots_ros::node_get_type` | `uint64 node`<br/>`---`<br/>`int32 type` |
| `/supervisor/node/get_type_name` | `service` | `webots_ros::node_get_name` | `uint64 node`<br/>`---`<br/>`string name` |
| `/supervisor/node/get_base_type_name` | `service` | `webots_ros::node_get_name` | `uint64 node`<br/>`---`<br/>`string name` |

%tab-end

%end

##### Description

*get information on a specified node*

The `wb_supervisor_node_get_type` function returns a symbolic value corresponding the type of the node specified as an argument.
If the argument is NULL, it returns `WB_NODE_NO_NODE`.
A list of all node types is provided in the "webots/nodes.h" include file.
Node types include `WB_NODE_APPEARANCE`, `WB_NODE_LIGHT_SENSOR`, etc.

The `wb_supervisor_node_get_type_name` function returns a text string corresponding to the name of the node.
If the argument node is a PROTO node, this function returns the PROTO name, like "E-puck", "RectangleArena", "Door", etc.
Otherwise if the argument node is not a PROTO node the returned value is the same as the output of `wb_supervisor_node_get_base_type_name` function, i.e. "Robot", "Appearance", "LightSensor", etc.
If the argument is NULL, the function returns the empty string.

The `wb_supervisor_node_get_base_type_name` function returns a text string corresponding to the base type name of the node, like "Robot", "Appearance", "LightSensor", etc.
If the argument is NULL, the function returns the empty string.

> **Note** [C++, Java, Python]: In the oriented-object APIs, the `WB_NODE_*` constants are available as static integers of the `Node` class (for example, Node::ROBOT).
These integers can be directly compared with the output of the `Node::getType` function.

---

#### `wb_supervisor_node_remove`
#### `wb_supervisor_node_export_string`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_node_remove(WbNodeRef node);
const char * wb_supervisor_node_export_string(WbNodeRef node);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    virtual void remove();
    std::string exportString() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def remove(self):
    def exportString(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public void remove();
  public String exportString();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_node_remove(node)
node_string = wb_supervisor_node_export_string(node)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/remove` | `service` | `webots_ros::node_remove` | `uint64 node`<br/>`---`<br/>`int8 success` |
| `/supervisor/node/export_string` | `service` | `webots_ros::node_get_string` | `uint64 node`<br/>`---`<br/>`string value` |

%tab-end

%end

##### Description

*Remove a specified node*

The `wb_supervisor_node_remove` function removes the node specified as an argument from the Webots scene tree.
If the node given in argument is the [Robot](robot.md) node itself, it is removed only at the end of the step.

The `wb_supervisor_node_export_string` function returns a string from which the node is constructed.
In conjunction with the `wb_supervisor_field_import_sf/mf_node_from_string` functions it can be used to duplicate the node.
A file with the equivalent content can be produced in the Webots user interface by selecting the node in the scene tree window and using the `Export` button.

---

#### `wb_supervisor_node_get_field`
#### `wb_supervisor_node_get_proto_field`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

WbFieldRef wb_supervisor_node_get_field(WbNodeRef node, const char *field_name);
WbFieldRef wb_supervisor_node_get_proto_field(WbNodeRef node, const char *field_name);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    Field *getField(const std::string &fieldName) const;
    Field *getProtoField(const std::string &fieldName) const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def getField(self, fieldName):
    def getProtoField(self, fieldName):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public Field getField(String fieldName);
  public Field getProtoField(String fieldName);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
field = wb_supervisor_node_get_field(node, 'field_name')
field = wb_supervisor_node_get_proto_field(node, 'field_name')
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/get_field` | `service` | `webots_ros::node_get_field` | `uint64 node`<br/>`string fieldName`<br/>`bool proto`<br/>`---`<br/>`uint64 field` |

%tab-end

%end

##### Description

*get a field reference from a node*

The `wb_supervisor_node_get_field` function retrieves a handler to a node field.
The field is specified by its name in `field_name` and the `node` it belongs to.
It can be a single field (SF) or a multiple field (MF).
If no such field name exists for the specified node or the field is an internal field of a PROTO, the return value is NULL.
Otherwise, it returns a handler to a field.

> **Note**: The `wb_supervisor_node_get_field` function will return a valid field handler if the field corresponding to the field name is an hidden field.

If the field is an internal field of a PROTO, the `wb_supervisor_node_get_proto_field` function should be used instead.

> **Note**: fields retrieved with the `wb_supervisor_node_get_proto_field` function are read-only. Which means that it is not possible to change them using any of the [`wb_supervisor_field_set_*`](#wb_supervisor_field_set_sf_bool) functions.

---

#### `wb_supervisor_node_get_position`
#### `wb_supervisor_node_get_orientation`
#### `wb_supervisor_node_get_pose`
#### `wb_supervisor_node_enable_pose_tracking`
#### `wb_supervisor_node_disable_pose_tracking`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

const double *wb_supervisor_node_get_position(WbNodeRef node);
const double *wb_supervisor_node_get_orientation(WbNodeRef node);
const double *wb_supervisor_node_get_pose(WbNodeRef node, WbNodeRef from_node);
void wb_supervisor_node_enable_pose_tracking(int sampling_period, WbNodeRef node, WbNodeRef from_node);
void wb_supervisor_node_disable_pose_tracking(WbNodeRef node, WbNodeRef from_node);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    const double *getPosition() const;
    const double *getOrientation() const;
    const double *getPose(const Node* fromNode = nullptr) const;
    void enablePoseTracking(int samplingPeriod, const Node *fromNode = nullprt) const;
    void disablePoseTracking(const Node *fromNode = nullptr) const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def getPosition(self):
    def getOrientation(self):
    def getPose(self, fromNode=None):
    def enablePoseTracking(self, samplingPeriod, fromNode=None):
    def disablePoseTracking(self, fromNode=None):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public double[] getPosition();
  public double[] getOrientation();
  public double[] getPose(Node fromNode=null);
  public void enablePoseTracking(int samplingPeriod, fromNode=null);
  public void disablePoseTracking(fromNode=null);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
position = wb_supervisor_node_get_position(node)
orientation = wb_supervisor_node_get_orientation(node)
pose = wb_supervisor_node_get_pose(node)
pose = wb_supervisor_node_get_pose(node, from_node)
wb_supervisor_node_enable_pose_tracking(sampling_period, node, from_node)
wb_supervisor_node_disable_pose_tracking(node, from_node)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/get_position` | `service` | `webots_ros::node_get_position` | `uint64 node`<br/>`---`<br/>[`geometry_msgs/Point`](http://docs.ros.org/api/geometry_msgs/html/msg/Point.html) position |
| `/supervisor/node/get_orientation` | `service` | `webots_ros::node_get_orientation` | `uint64 node`<br/>`---`<br/>[`geometry_msgs/Quaternion`](http://docs.ros.org/api/geometry_msgs/html/msg/Quaternion.html) orientation |
| `/supervisor/node/get_pose` | `service` | `webots_ros::node_get_pose` | `uint64 node`<br/>`uint64 from_node`<br/>`---`<br/>[`geometry_msgs/Pose`](http://docs.ros.org/en/api/geometry_msgs/html/msg/Pose.html) pose |
| `/supervisor/node/enable_pose_tracking` | `service` | `webots_ros::node_enable_pose_tracking` | `uint64 node`<br/>`uint64 sampling_period`<br/>`uint64 from_node`<br/>`---`<br/>`int8 success` |
| `/supervisor/node/disable_pose_tracking` | `service` | `webots_ros::node_disable_pose_tracking` | `uint64 node`<br/>`uint64 from_node`<br/>`---`<br/>`int8 success` |

%tab-end

%end

##### Description

*get the global (world) position/orientation of a node*

The `wb_supervisor_node_get_position` function returns the position of a node expressed in the global (world) coordinate system.
The `node` argument must be a [Transform](transform.md) node (or a derived node), otherwise the function will print a warning message and return 3 `NaN` (Not a Number) values.
This function returns a vector containing exactly 3 values.

The `wb_supervisor_node_get_orientation` function returns a matrix that represents the rotation of the node in the global (world) coordinate system.
The `node` argument must be a [Transform](transform.md) node (or a derived node), otherwise the function will print a warning message and return 9 `NaN` (Not a Number) values.
This function returns a matrix containing exactly 9 values that shall be interpreted as a 3 x 3 orthogonal rotation matrix:

```
[ R[0] R[1] R[2] ]
[ R[3] R[4] R[5] ]
[ R[6] R[7] R[8] ]
```

Each column of the matrix represents where each of the three main axes (*x*, *y* and *z*) is pointing in the node's coordinate system.
The columns (and the rows) of the matrix are pairwise orthogonal unit vectors (i.e., they form an orthonormal basis).
Because the matrix is orthogonal, its transpose is also its inverse.
So by transposing the matrix you can get the inverse rotation.
Please find more info [here](http://en.wikipedia.org/wiki/Rotation_representation).

By multiplying the rotation matrix on the right with a vector and then adding the position vector you can express the coordinates of a point in the global (world) coordinate system knowing its coordinates in a local (node) coordinate system.
For example:

```
p' = R * p + T
```

Where *p* is a point whose coordinates are given with respect to the local coordinate system of a node, *R* the rotation matrix returned by the `wb_supervisor_node_get_orientation` function, *T* is the position returned by the `wb_supervisor_node_get_position` function and *p'* represents the same point but this time with coordinates expressed in the global (world) coordinate system.

The `wb_supervisor_node_get_pose` function returns an array of 16 values.
The array shall be interpreted as a 4 x 4 [transformation matrix](https://en.wikipedia.org/wiki/Transformation_matrix) that represents an absolute transform of the node.
The function returns the relative pose of the node with respect to the node specified in `from_node`. If `from_node` is null, it returns the absolute pose of the node in the global coordinate system.
The resulting matrix looks as following:

```
[ M[0] = R[0,0]    M[1] = R[0,1]    M[2] = R[0,2]     M[3] = T[0] ]
[ M[4] = R[1,0]    M[5] = R[1,1]    M[6] = R[1,2]     M[7] = T[1] ]
[ M[8] = R[2,0]    M[9] = R[2,1]    M[10] = R[2,2]    M[11] = T[2]]
[ M[12] = 0        M[13] = 0        M[14] = 0         M[15] = 1   ]
```

Where `M[i]` is the result of the `wb_supervisor_node_get_pose` function at index `i`.
The matrix is composed of a rotation matrix `R` and a translation vector `T`.

The `wb_supervisor_node_enable_pose_tracking` function forces Webots to stream poses to the controller.
It improves the performance as the controller by default uses a request-response pattern to get pose data.
The `sampling_period` argument determines how often the pose data should be sent to the controller.

The `wb_supervisor_node_disable_pose_tracking` function disables pose data tracking.

The "[WEBOTS\_HOME/projects/robots/neuronics/ipr/worlds/ipr\_cube.wbt]({{ url.github_tree }}/projects/robots/neuronics/ipr/worlds/ipr_cube.wbt)" simulation shows how to use these functions to achieve this (see [the controller]({{ url.github_tree }}/projects/robots/neuronics/ipr/controllers/target_coordinates/target_coordinates.c)).

> **Note**: The returned pointers are valid during one time step only as memory will be deallocated at the next time step.

---

#### `wb_supervisor_node_get_center_of_mass`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

const double *wb_supervisor_node_get_center_of_mass(WbNodeRef node);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    const double *getCenterOfMass() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def getCenterOfMass(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public double[] getCenterOfMass();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
com = wb_supervisor_node_get_center_of_mass(node)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/get_center_of_mass` | `service` | `webots_ros::node_get_center_of_mass` | `uint64 node`<br/>`---`<br/>[`geometry_msgs/Point`](http://docs.ros.org/api/geometry_msgs/html/msg/Point.html) centerOfMass |

%tab-end

%end

##### Description

*get the global position of a solid's center of mass*

The `wb_supervisor_node_get_center_of_mass` function returns the position of the center of mass of a Solid node (including its descendant Solid nodes) expressed in the global (world) coordinate system.
The `node` argument must be a [Solid](solid.md) node (or a derived node), otherwise the function will print a warning message and return 3 `NaN` (Not a Number) values.
This function returns a vector containing exactly 3 values.
If the `node` argument has a `NULL` `physics` node, the return value is always the zero vector.

The "[WEBOTS\_HOME/projects/samples/howto/center\_of\_mass/worlds/center\_of\_mass.wbt]({{ url.github_tree }}/projects/samples/howto/center_of_mass/worlds/center_of_mass.wbt)" simulation shows how to use this function.

> **Note**: The returned pointer is valid during one time step only as memory will be deallocated at the next time step.

---

#### `wb_supervisor_node_get_contact_point`
#### `wb_supervisor_node_get_contact_point_node`
#### `wb_supervisor_node_get_number_of_contact_points`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

const double *wb_supervisor_node_get_contact_point(WbNodeRef node, int index);
WbNodeRef wb_supervisor_node_get_contact_point_node(WbNodeRef node, int index);
int wb_supervisor_node_get_number_of_contact_points(WbNodeRef node, bool include_descendants);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    const double *getContactPoint(int index) const;
    Node *getContactPointNode(int index) const;
    int getNumberOfContactPoints(bool includeDescendants = false) const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def getContactPoint(self, index):
    def getContactPointNode(self, index):
    def getNumberOfContactPoints(self, includeDescendants=False):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public double[] getContactPoint(int index);
  public Node getContactPointNode(int index);
  public int getNumberOfContactPoints(boolean includeDescendants);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
contact_point = wb_supervisor_node_get_contact_point(node, index)
node = wb_supervisor_node_get_contact_point_node(node, index);
number_of_contacts = wb_supervisor_node_get_number_of_contact_points(node, include_descendants)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/get_number_of_contact_points` | `service` | `webots_ros::node_get_number_of_contact_points` | `uint64 node`<br/>`bool includeDescendants`<br/>`---`<br/>`int32 numberOfContactPoints` |
| `/supervisor/node/get_contact_point` | `service` | `webots_ros::node_get_contact_point` | `uint64 node`<br/>`int32 index`<br/>`---`<br/>[`geometry_msgs/Point`](http://docs.ros.org/api/geometry_msgs/html/msg/Point.html) point |
| `/supervisor/node/get_contact_point_node` | `service` | `webots_ros::node_get_contact_point_node` | `uint64 node`<br/>`int32 index`<br/>`---`<br/>`uint64 node`` |

%tab-end

%end

##### Description

*get the contact point with given index in the contact point list of the given solid.*

The `wb_supervisor_node_get_contact_point` function returns the contact point with given index in the contact point list of the given `Solid`.
The `wb_supervisor_node_get_number_of_contact_points` function allows you to retrieve the length of this list.
Contact points are expressed in the global (world) coordinate system.
If the index is less than the number of contact points, then the x (resp. y, z) coordinate (expressed in the global (world) coordinate system) of the *index*th contact point is the element number *0* (resp. *1, 2*) in the returned array.
Otherwise the function returns a `NaN` (Not a Number) value for each of these numbers.
The `node` argument must be a [Solid](solid.md) node (or a derived node), which moreover has no `Solid` parent, otherwise the function will print a warning message and return `NaN` values on the first 3 array components.

The `wb_supervisor_node_get_contact_point_node` function allows you to retrieve the node associated to a contact point. This is useful when contact points of the descendants are included, to know which part the contact belongs to.

The `wb_supervisor_node_get_number_of_contact_points` function returns the number of contact points of the given `Solid`.
The `node` argument must be a [Solid](solid.md) node (or a derived node), which moreover has no `Solid` parent, otherwise the function will print a warning message and return `-1`.
The `include_descendants` argument defines whether the descendant nodes should also generate contact points or not. The descendant nodes are the nodes included within the node given as an argument.

The "[WEBOTS\_HOME/projects/samples/howto/cylinder_stack/worlds/cylinder\_stack.wbt]({{ url.github_tree }}/projects/samples/howto/cylinder_stack/worlds/cylinder_stack.wbt)" project shows how to use this function.

> **Note**: The returned pointer is valid during one time step only as memory will be deallocated at the next time step.

---

#### `wb_supervisor_node_get_static_balance`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

bool wb_supervisor_node_get_static_balance(WbNodeRef node);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    bool getStaticBalance() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def getStaticBalance(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public boolean getStaticBalance();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
balance = wb_supervisor_node_get_static_balance(node)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/get_static_balance` | `service` | `webots_ros::node_get_static_balance` | `uint64 node`<br/>`---`<br/>`uint8 balance` |

%tab-end

%end

##### Description

*return the boolean value of the static balance test based on the support polygon of a solid*

The `wb_supervisor_node_get_static_balance` function returns the boolean value of the static balance test based on the support polygon of a solid.
The `node` argument must be a [Solid](solid.md) node (or a derived node), which moreover has no `Solid` parent.
Otherwise the function will print a warning message and return `false`.
The support polygon of a solid is the convex hull of the solid's contact points projected onto a plane that is orthognal to the gravity direction.
The test consists in checking whether the projection of the center of mass onto this plane lies inside or outside the support polygon.

---

#### `wb_supervisor_node_get_velocity`
#### `wb_supervisor_node_set_velocity`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

const double *wb_supervisor_node_get_velocity(WbNodeRef node);
void wb_supervisor_node_set_velocity(WbNodeRef node, const double velocity[6]);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    const double *getVelocity() const;
    void setVelocity(const double velocity[6]);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def getVelocity(self):
    def setVelocity(self, velocity):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public double[] getVelocity();
  public void setVelocity(double velocity[6]);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
velocity = wb_supervisor_node_get_velocity(node)
wb_supervisor_node_set_velocity(node, velocity)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/get_velocity` | `service` | `webots_ros::node_get_velocity` | `uint64 node`<br/>`---`<br/>[`geometry_msgs/Twist`](http://docs.ros.org/api/geometry_msgs/html/msg/Twist.html) velocity |
| `/supervisor/node/set_velocity` | `service` | `webots_ros::node_set_velocity` | `uint64 node`<br/>[`geometry_msgs/Twist`](http://docs.ros.org/api/geometry_msgs/html/msg/Twist.html) `velocity`<br/>`---`<br/>`int32 success` |

%tab-end

%end

##### Description

*get/set the angular and linear velocities of a Solid node.*

The `wb_supervisor_node_get_velocity` function returns the absolute velocity (both linear and angular) of a node expressed in the global (world) coordinate system.
The `node` argument must be a [Solid](solid.md) node (or a derived node), otherwise the function will print a warning message and return 6 `NaN` (Not a Number) values.
This function returns a vector containing exactly 6 values.
The first three are respectively the linear velocities in the x, y and z direction.
The last three are respectively the angular velocities around the x, y and z axes.

The `wb_supervisor_node_set_velocity` function sets the absolute velocity (both linear and angular) of a node expressed in the global (world) coordinate system.
The `node` argument must be a [Solid](solid.md) node (or a derived node), otherwise the function will print a warning message and have no effect.
The `velocity` argument must be a vector containing exactly 6 values.
The first three are respectively the linear velocities in the x, y and z direction.
The last three are respectively the angular velocities around the x, y and z axes.

---

#### `wb_supervisor_node_save_state`
#### `wb_supervisor_node_load_state`
#### `wb_supervisor_node_reset_physics`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_node_save_state(WbNodeRef node, const char *state_name);
void wb_supervisor_node_load_state(WbNodeRef node, const char *state_name);
void wb_supervisor_node_reset_physics(WbNodeRef node);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    void saveState(const std::string &stateName);
    void loadState(const std::string &stateName);
    void resetPhysics();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def saveState(self, stateName):
    def loadState(self, stateName):
    def resetPhysics(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public void saveState(String stateName);
  public void loadState(String stateName);
  public void resetPhysics();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_node_save_state(node, state_name)
wb_supervisor_node_load_state(node, state_name)
wb_supervisor_node_reset_physics(node)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/save_state` | `service` | `webots_ros::node_set_string` | `uint64 node`<br/>`string state_name`<br/>`---`<br/>`int8 success` |
| `/supervisor/node/load_state` | `service` | `webots_ros::node_set_string` | `uint64 node`<br/>`string state_name`<br/>`---`<br/>`int8 success` |
| `/supervisor/node/reset_physics` | `service` | `webots_ros::node_reset_functions` | `uint64 node`<br/>`---`<br/>`int8 success` |

%tab-end

%end

##### Description

*resets state of the given solid*

The `wb_supervisor_node_save_state` function saves the state of the given node and all its descendant nodes.
The state is saved internally under a key equal to the `state_name` parameter.
Therefore, the state can be reverted using the `wb_supervisor_node_load_state` function afterward.
Please refer to [Reset/Reload Matrix](#wb_supervisor_simulation_reset) for more details.

The `wb_supervisor_node_load_state` function loads the state of the given node and all its descendant nodes.
The function parameter `state_name` has to be a valid value, or the controller will fail.
The valid `state_name` value means that the state is saved with the `wb_supervisor_node_load_state` function, or a magic name `__init__`.
The state in the beginning of the simulation is saved under the magic name `__init__`.

The `wb_supervisor_node_reset_physics` function stops the inertia of the given node and its descendants.
If the specified node is physics-enabled, i.e. it contains a [Physics](physics.md) node, then the linear and angular velocities of the corresonding body are reset to 0, hence the inertia is also zeroed.
This function could be useful for resetting the physics of a solid after changing its translation or rotation.
To stop the inertia of all available solids please refer to [this section](#wb_supervisor_simulation_reset_physics).

---

#### `wb_supervisor_node_restart_controller`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_node_restart_controller(WbNodeRef node);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    void restartController();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def restartController(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public void restartController();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_node_restart_controller(node)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/restart_controller` | `service` | `webots_ros::node_reset_functions` | `uint64 node`<br/>`---`<br/>`int8 success` |

%tab-end

%end

##### Description

*restarts the controller of the given robot*

The `wb_supervisor_node_restart_controller` function restarts the controller of the Robot passed to it.
If a node other than a [Robot](robot.md) is passed to this function, no change is effected, and a warning message is printed to the console.
Note that if a robot window is specified for the [Robot](robot.md) node, the robot window will be restarted as well.
If an extern controller is used, then this function will only terminate the process, it will not start the controller again.

---

#### `wb_supervisor_node_move_viewpoint`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_node_move_viewpoint(WbNodeRef node);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    void moveViewpoint() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def moveViewpoint(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public void moveViewpoint();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_node_move_viewpoint(node)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/move_viewpoint` | `service` | `webots_ros::node_move_viewpoint` | `uint64 node`<br/>---`<br/>`int8 success` |

%tab-end

%end

##### Description

*move the viewpoint to the node*

The `wb_supervisor_node_move_viewpoint` function moves the [Viewpoint](viewpoint.md) to the node given in argument.
Calling this function is equivalent to using the 'Move Viewpoint to Object' menu from the GUI.

---

#### `wb_supervisor_node_set_visibility`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_node_set_visibility(WbNodeRef node, WbNodeRef from, bool visible);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    void setVisibility(Node *from, bool visible);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def setVisibility(self, from, visible):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public void setVisibility(Node from, boolean visible);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_node_set_visibility(node, from, visible)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/set_visibility` | `service` | `webots_ros::node_hide_from_camera` | `uint64 node`<br/>`uint64 from`<br/>`uint8 visible`<br/>`---`<br/>`int8 success` |

%tab-end

%end

##### Description

*set the visibility of a node*

The `wb_supervisor_node_set_visibility` function sets the visibility of a node from the specified [Camera](camera.md), [Lidar](lidar.md), [RangeFinder](rangefinder.md) or [Viewpoint](viewpoint.md) node.
In particular it defines if the node is visible in the image recorded by the `from` device.
The `from` argument must be either a [Camera](camera.md), [Lidar](lidar.md), [RangeFinder](rangefinder.md) or [Viewpoint](viewpoint.md) node.
In case of the [Viewpoint](viewpoint.md) the node is hidden or shown in the main 3D scene.
The `node` argument is the node to hide or show, if the node has some children they all will be recursively hidden too, any type of node is allowed but it doesn't make sense to hide a node that has no visual appearance in the 3D scene.
The `visible` argument specifies whether the node should be hidden (false) or shown (true).
By default, all the nodes are visible.
It is relevant to show a node only if it was previously hidden using this function.

---

#### `wb_supervisor_node_add_force`
#### `wb_supervisor_node_add_force_with_offset`
#### `wb_supervisor_node_add_torque`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_node_add_force(WbNodeRef node, const double force[3], bool relative);
void wb_supervisor_node_add_force_with_offset(WbNodeRef node, const double force[3], const double offset[3], bool relative);
void wb_supervisor_node_add_torque(WbNodeRef node, const double torque[3], bool relative);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Node.hpp>

namespace webots {
  class Node {
    void addForce(const double force[3], bool relative);
    void addForceWithOffset(const double force[3], const double offset[3], bool relative);
    void addTorque(const double torque[3], bool relative);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Node

class Node:
    def addForce(self, force, relative)
    def addForceWithOffset(self, force, offset, relative)
    def addTorque(self, torque, relative)
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Node;

public class Node {
  public void addForce(double force[3], boolean relative);
  public void addForceWithOffset(double force[3], double offset[3], boolean relative);
  public void addTorque(double torque[3], boolean relative);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
void wb_supervisor_node_add_force(node, force, relative)
void wb_supervisor_node_add_force_with_offset(node, force, offset, relative)
void wb_supervisor_node_add_torque(node, torque, relative)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/node/add_force` | `service` | `webots_ros::node_add_force_or_torque` | `uint64 node`<br/>[`geometry_msgs/Twist`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) force<br/>`uint8 relative`<br/>`---`<br/>`int32 success` |
| `/supervisor/node/add_force_with_offset` | `service` | `webots_ros::node_add_force_with_offset` | `uint64 node`<br/>[`geometry_msgs/Twist`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) force<br/>[`geometry_msgs/Twist`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) offset<br/>`uint8 relative`<br/>`---`<br/>`int32 success` |
| `/supervisor/node/add_torque` | `service` | `webots_ros::node_add_force_or_torque` | `uint64 node`<br/>[`geometry_msgs/Twist`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) force<br/>`uint8 relative`<br/>`---`<br/>`int32 success` |

%tab-end

%end

##### Description

*add force or torque to a Solid node.*

The `wb_supervisor_node_add_force` function adds a force to the [Solid](solid.md) node at its center of mass, the `relative` argument defines if the force is expressed in world coordinate system (`relative` set to false) or relatively to the node (`relative` set to true).
The `wb_supervisor_node_add_force_with_offset` function adds a force to the [Solid](solid.md) node at the location (expressed in the node coordinate system) defined by the `offset` argument.
The `wb_supervisor_node_add_torque` function adds a torque to the [Solid](solid.md) node.

---

#### `wb_supervisor_set_label`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_set_label(int id, const char *text, double x, double y, double size, int color, double transparency, const char *font);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    virtual void setLabel(int id, const std::string &label, double xpos, double ypos,
      double size, int color, double transparency, const std::string &font="Arial");
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def setLabel(self, id, label, xpos, ypos, size, color, transparency, font="Arial"):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public void setLabel(int id, String label, double xpos, double ypos,
     double size, int color, double transparency, String font);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_set_label(id, 'text', x, y, size, [r g b], transparency)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/set_label` | `service` | `webots_ros::supervisor_set_label` | `int32 id`<br/>`string label`<br/>`float64 xpos`<br/>`float64 ypos`<br/>`float64 size`<br/>`int32 color`<br/>`float64 transparency`<br/>`string font`<br/>`---`<br/>`int8 success` |

%tab-end

%end

##### Description

*overlay a text label on the 3D scene*

The `wb_supervisor_set_label` function displays a text label overlaying the 3D scene in Webots' main window.
The `id` parameter is an identifier for the label; you can choose any value in the range 0 to 65534.
The same value may be used later if you want to change that label, or update the text.
Id value 65535 is reserved for automatic video caption.
The `text` parameter is a text string which should contain only displayable characters in the range 32-127.
The `x` and `y` parameters are the coordinates of the upper left corner of the text, relative to the upper left corner of the 3D window.
These floating point values are expressed in percent of the 3D window width and height, hence, they should lie in the range 0-1.
The `size` parameter defines the size of the font to be used.
It is expressed in the same unit as the `y` parameter.
The `color` parameter defines the color of the label.
It is expressed as a 3 bytes RGB integer, the most significant byte (leftmost byte in hexadecimal representation) represents the red component, the second most significant byte represents the green component and the third byte represents the blue component.
The `transparency` parameter defines the transparency of the label.
A transparency level of 0 means no transparency, while a transparency level of 1 means total transparency (the text will be invisible).
Intermediate values correspond to semi-transparent levels.
Finally, the `font` parameter defines the font used to draw the text, the following standard fonts are available:

- Arial
-  Arial Black
-  Comic Sans MS
-  Courier New
-  Georgia
-  Impact
-  Lucida Console
-  Lucida Sans Unicode
-  Palatino Linotype
-  Tahoma
-  Times New Roman
-  Trebuchet MS
-  Verdana

##### Examples

```c
wb_supervisor_set_label(0,"hello world",0,0,0.1,0xff0000,0,"Arial");
```

This will display the label "hello world" in red at the upper left corner of the 3D window.

```c
wb_supervisor_set_label(1,"hello Webots",0,0.1,0.1,0x00ff00,0.5,"Impact");
```

This will display the label "hello Webots" in semi-transparent green, just below.

```c
supervisor_set_label(0,"hello universe",0,0,0.1,0xffff00,0,"Times New Roman");
```

This will change the label "hello world" defined earlier into "hello universe", using a yellow color for the new text.

> **Note** [MATLAB]: In the MATLAB version of the `wb_supervisor_set_label` function, the `color` argument must be a vector containing the three RGB components: `[RED GREEN BLUE]`.
Each component must be a value between 0.0 and 1.0.
For example the vector `[1 0 1]` represents the magenta color.

---

#### `wb_supervisor_simulation_quit`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_simulation_quit(int status);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    virtual void simulationQuit(int status);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def simulationQuit(self, status):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public void simulationQuit(int status);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_simulation_quit(status)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/simulation_quit` | `service` | [`webots_ros::set_int`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*terminate the simulator and controller processes*

The `wb_supervisor_simulator_quit` function quits Webots, as if one was using the menu `File / Quit Webots`.
This function makes it easier to invoke a Webots simulation from a script because it allows to terminate the simulation automatically, without human intervention.
As a result of quitting the simulator process, all controller processes, including the calling supervisor controller, will terminate.
The `wb_supervisor_simulator_quit` function sends a request to quit the simulator and immediately returns to the controller process, it does not wait for the effective termination of the simulator.
After the call to the `wb_supervisor_simulator_quit` function, the controller should call the `wb_robot_cleanup` function and then exit.
The POSIX exit status returned by Webots can be defined by the status `status` parameter.
Some typical values for this are the `EXIT_SUCCESS` or `EXIT_FAILURE` macros defined into the "stdlib.h" file.
Here is a C example:

```c
#include <webots/robot.h>
#include <webots/supervisor.h>
#include <stdlib.h>

#define TIME_STEP 32

int main(int argc, char *argv[]) {
  wb_robot_init();
  ...
  while (! finished) {
    // your controller code here
    ...
    if (wb_robot_step(TIME_STEP) == -1)
      break;
  }
  saveExperimentsData();
  wb_supervisor_simulation_quit(EXIT_SUCCESS); // ask Webots to terminate
  wb_robot_cleanup(); // cleanup resources
  return 0;
}
```

In object-oriented languages, there is no `wb_robot_cleanup` function, in this case the controller should call its destructor.
Here is a C++ example:

```c
#include <webots/Robot.hpp>
#include <webots/Supervisor.hpp>
#include <cstdlib>

#define TIME_STEP 32

using namespace webots;

int main(int argc, char *argv[]) {
  Supervisor *controller = new Supervisor();

  ...
  while (! finished) {
    // your controller code here
    ...
    if (controller->step(TIME_STEP) == -1)
      break;
  }
  controller->simulationQuit(EXIT_SUCCESS);  // ask Webots to terminate

  delete controller; // cleanup resources
  return 0;
}
```

---

#### `wb_supervisor_simulation_get_mode`
#### `wb_supervisor_simulation_set_mode`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

typedef enum {
  WB_SUPERVISOR_SIMULATION_MODE_PAUSE,
  WB_SUPERVISOR_SIMULATION_MODE_REAL_TIME,
  WB_SUPERVISOR_SIMULATION_MODE_FAST
} WbSimulationMode;

WbSimulationMode wb_supervisor_simulation_get_mode();
void wb_supervisor_simulation_set_mode(WbSimulationMode mode);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    typedef enum {
      SIMULATION_MODE_PAUSE, SIMULATION_MODE_REAL_TIME, SIMULATION_MODE_FAST
    } SimulationMode;

    SimulationMode simulationGetMode() const;
    virtual void simulationSetMode(SimulationMode mode);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    SIMULATION_MODE_PAUSE, SIMULATION_MODE_REAL_TIME, SIMULATION_MODE_FAST

    def simulationGetMode(self):
    def simulationSetMode(self, mode):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public final static int SIMULATION_MODE_PAUSE, SIMULATION_MODE_REAL_TIME, SIMULATION_MODE_FAST;

  public int simulationGetMode();
  public void simulationSetMode(int mode);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
WB_SUPERVISOR_SIMULATION_MODE_PAUSE, WB_SUPERVISOR_SIMULATION_MODE_REAL_TIME, WB_SUPERVISOR_SIMULATION_MODE_FAST

mode = wb_supervisor_simulation_get_mode()
wb_supervisor_simulation_set_mode(mode)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/supervisor_simulation_get_mode` | `service` | [`webots_ros::get_int`](ros-api.md#common-services) | |
| `/supervisor/supervisor_simulation_set_mode` | `service` | [`webots_ros::set_int`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*get and set the simulation mode*

The `wb_supervisor_simulation_get_mode` function returns an integer matching with the current simulation mode, i.e., if the simulation is currently paused, is running in real-time, or in fast mode with or without the graphical renderings.
The macros described in the [table](#simulation-modes) are matching with the return value of this function.
The value returned by this function is updated during the previous function call of the `wb_robot_init` or `wb_robot_step` functions.

The `wb_supervisor_simulation_set_mode` function allows to set the simulation mode.
Note that if the `WB_SUPERVISOR_SIMULATION_MODE_PAUSE` is set by the current supervisor, then calling the `wb_robot_step(time_step)` function with a `time_step` argument different from 0 will make the controller wait until the simulation is resumed.
Calling `wb_robot_step(0)` could be useful to send information to Webots when it is paused (such as modifying, moving, adding or deleting objects).
The simulation can then go on by calling for example `wb_supervisor_simulation_set_mode(WB_SUPERVISOR_SIMULATION_MODE_FAST)`.

The current simulation mode can also be modified by the Webots user, when he's clicking on the corresponding buttons in the user interface.

%figure "Simulation modes"

| Mode                                         | Description                                                                     |
| -------------------------------------------- | ------------------------------------------------------------------------------- |
| `WB_SUPERVISOR_SIMULATION_MODE_PAUSE`        | The simulation is paused.                                                       |
| `WB_SUPERVISOR_SIMULATION_MODE_REAL_TIME`    | The simulation is running as close as possible to the real-time.                |
| `WB_SUPERVISOR_SIMULATION_MODE_FAST`         | The simulation is running as fast as possible without the graphical renderings. |

%end

---

#### `wb_supervisor_simulation_reset`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_simulation_reset();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    virtual void simulationReset();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def simulationReset(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public void simulationReset();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_simulation_reset()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/simulation_reset` | `service` | [`webots_ros::get_bool`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*reset the simulation*

The `wb_supervisor_simulation_reset` function sends a request to the simulator process, asking it to reset the simulation at the end of the step.
The reset process is explained in detail in the [User Guide](https://www.cyberbotics.com/doc/guide/the-user-interface#file-menu), the only difference is that the supervisor and robot controllers are not restarted, if needed, they have to be restarted with the `wb_supervisor_node_restart_controller` function.
You may wish to save some data in a file from your supervisor and robot controller programs in order to reload it when they restart.

##### Reset/Reload Matrix

The following table summarizes the behavior of different reset functions:
- **Reload**: The reload button in the user-interace and the `wb_supervisor_world_reload` funtion.
- **Reset from user interface**: The reset button in the user-interace.
- **Reset from Supervisor**: The `wb_supervisor_simulation_reset` function.
- **Load node's state from Supervisor**: The `wb_supervisor_node_save/load_state` functions.
- **Reset physics**: The `wb_supervisor_simulation_reset_physics` function.
- **Reset node's physics**: The `wb_supervisor_node_reset_physics` function.

<table>
  <thead>
    <tr>
      <th></th>
      <th><strong>Reload</strong></th>
      <th><strong>Reset from user interface</strong></th>
      <th><strong>Reset from Supervisor</strong></th>
      <th><strong>Load node&#39;s state from Supervisor</strong></th>
      <th><strong>Reset physics</strong></th>
      <th><strong>Reset node&#39;s physics</strong></th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><strong>Resets simulation time</strong></td>
      <td>Yes</td>
      <td>Yes</td>
      <td>Yes</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
    </tr>
    <tr>
      <td><strong>Removes nodes</strong></td>
      <td>Yes</td>
      <td>Yes</td>
      <td>Yes</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
    </tr>
    <tr>
      <td><strong>Restarts controller</strong></td>
      <td>Yes</td>
      <td>Yes</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
    </tr>
    <tr>
      <td><strong>Stops sounds</strong></td>
      <td>Yes</td>
      <td>Yes</td>
      <td>Yes</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
    </tr>
    <tr>
      <td><strong>Resets random seeds</strong></td>
      <td>Yes</td>
      <td>Yes</td>
      <td>Yes</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
    </tr>
    <tr>
      <td><strong>Resets physics</strong></td>
      <td>Yes</td>
      <td>Yes</td>
      <td>Yes</td>
      <td>Yes (scoped)</td>
      <td>Yes</td>
      <td>Yes (scoped)</td>
    </tr>
    <tr>
      <td><strong>Resets physics plugin</strong></td>
      <td>Yes</td>
      <td>Yes</td>
      <td>Yes</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
    </tr>
    <tr>
      <td><strong>Resets all fields</strong></td>
      <td>Yes</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
    </tr>
    <tr>
      <td><strong>Adds removed nodes</strong></td>
      <td>Yes</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
      <td>No</td>
    </tr>
    <tr>
      <td><strong><a href="brake.md">Brake</a></strong></td>
      <td>Resets</td>
      <td>Releases</td>
      <td>Releases</td>
      <td>Releases</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="charger.md">Charger</a></strong></td>
      <td>Resets</td>
      <td colspan=2>Resets the <code>battery</code> field and the <code>emissiveColor</code> field of the <a
          href="material.md">Material</a> node of the first <a href="shape.md">Shape</a> child node</td>
      <td>Loads the <code>battery</code> field and resets the emissiveColor field of the <a
          href="material.md">Material</a> node of the first <a href="shape.md">Shape</a> child node</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="display.md">Display</a></strong></td>
      <td>Resets</td>
      <td>Clears</td>
      <td>Clears</td>
      <td>Clears</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="emitter.md">Emitter</a>/<a href="receiver.md">Receiver</a></strong></td>
      <td>Resets</td>
      <td colspan=3>Clears the message queue</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="joint.md">Joint</a>/<a href="motor.md">Motor</a></strong></td>
      <td>Resets</td>
      <td colspan=2>Resets the position, velocity, acceleration, available torque, and available force</td>
      <td>Loads the position, resets the velocity, acceleration, available torque, and available force</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="led.md">LED</a></strong></td>
      <td>Resets</td>
      <td colspan=3>If the first child is a Light node, it resets the <code>color</code> field and it switches the LED off.
        If the first child is a <a href="shape.md">Shape</a> node, it resets the <code>emissiveColor</code>
        field of its <a href="material.md">Material</a> node.</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="lidar.md">Lidar</a></strong></td>
      <td>Resets</td>
      <td colspan=3>Resets the position of the rotating head</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="pen.md">Pen</a></strong></td>
      <td>Resets</td>
      <td colspan=3>Cleans the painted textures</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="propeller.md">Propeller</a></strong></td>
      <td>Resets</td>
      <td colspan=3>Resets the slow helix and it&#39;s initial position</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="robot.md">Robot</a></strong></td>
      <td>Resets</td>
      <td colspan=2>Resets the <code>battery</code> field, removes all the supervisor labels, resets the nodes visibility,
        and restarts the controller</td>
      <td>Loads the <code>battery</code> field, removes all the supervisor labels, and resets the nodes visibility
      </td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="solid.md">Solid</a></strong></td>
      <td>Resets</td>
      <td colspan=2>Resets the <code>translation</code> and <code>rotation</code> fields, and the physics</td>
      <td>Loads the <code>translation</code> and <code>rotation</code> fields, and resets the physics</td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="track.md">Track</a></strong></td>
      <td>Resets</td>
      <td colspan=2>Resets the motor position and the <code>translation</code> field of the textureTransform node of the <a
          href="appearance.md">Appearance</a> node of the first <a href="shape.md">Shape</a> children node
      </td>
      <td>Loads the motor position and the <code>translation</code> field of the textureTransform node of the <a
          href="appearance.md">Appearance</a> node of the first <a href="shape.md">Shape</a> children node
      </td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
    <tr>
      <td><strong><a href="viewpoint.md">Viewpoint</a></strong></td>
      <td>Resets</td>
      <td colspan=2>Resets <code>orientation</code>, <code>position</code>, <code>near</code>, <code>far</code>, and
        <code>fieldOfView</code> fields
      </td>
      <td>Loads <code>orientation</code>, <code>position</code>, <code>near</code>, <code>far</code>, and
        <code>fieldOfView</code> fields
      </td>
      <td>N/A</td>
      <td>N/A</td>
    </tr>
  </tbody>
</table>

---

#### `wb_supervisor_simulation_reset_physics`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_simulation_reset_physics();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    virtual void simulationResetPhysics();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def simulationResetPhysics(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public void simulationResetPhysics();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_simulation_reset_physics()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/simulation_reset_physics` | `service` | [`webots_ros::get_bool`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*stop the inertia of all solids in the world and reset the random number generator*

The `wb_supervisor_simulation_reset_physics` function sends a request to the simulator process, asking it to stop the movement of all physics-enabled solids in the world.
It means that for any [Solid](solid.md) node containing a [Physics](physics.md) node, the linear and angular velocities of the corresponding body are reset to 0, hence the inertia is also zeroed.
This is actually implemented by calling the ODE's `dBodySetLinearVel` and `dBodySetAngularVel` functions for all bodies with a zero velocity parameter.
This function is especially useful for resetting a robot to its initial position and inertia.
To stop the inertia of a single [Solid](solid.md) node please refer to [this section](#wb_supervisor_node_reset_physics).

---

#### `wb_supervisor_world_load`
#### `wb_supervisor_world_save`
#### `wb_supervisor_world_reload`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_world_load(const char *filename);
bool wb_supervisor_world_save(const char *filename);
void wb_supervisor_world_reload();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    virtual void worldLoad(const std::string &file);
    virtual bool worldSave();
    virtual bool worldSave(const std::string &file);
    virtual void worldReload();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def worldLoad(self, file):
    def worldSave(self, file=None):
    def worldReload(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public void worldLoad(String file);
  public boolean worldSave();
  public boolean worldSave(String file);
  public void worldReload();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_world_load('filename')
success = wb_supervisor_world_save()
success = wb_supervisor_world_save('filename')
wb_supervisor_world_reload()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/world_load` | `service` | [`webots_ros::set_string`](ros-api.md#common-services) | |
| `/supervisor/world_save` | `service` | [`webots_ros::set_string`](ros-api.md#common-services) | |
| `/supervisor/world_reload` | `service` | [`webots_ros::get_bool`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*Load, save or reload the current world.*

The `wb_supervisor_world_load` function sends a request to the simulator process, asking it to stop the current simulation and load the world given in argument immediately.
As a result of changing the current world, all the supervisor and robot controller processes are terminated and the new one are restarted with the new world.
You may wish to save some data in a file from your supervisor and robot controller programs in order to reload it from the new world.

The `wb_supervisor_world_save` function saves the current world.
The `filename` parameter defines the path to the target world file.
It should end with the `.wbt` extension.
It can be defined either as an absolute path, or as a path relative to the current supervisor controller.
If NULL, the current world path is used instead (e.g., a simple save operation).
The boolean return value indicates the success of the save operation.
Be aware that this function can overwrite silently existing files, so that the corresponding data may be lost.

> **Note** [C++, Java, Python, MATLAB]: In the other APIs, the `Robot.worldSave` function can be called without argument.
In this case, a simple save operation is performed.

The `wb_supervisor_world_reload` function sends a request to the simulator process, asking it to reload the current world immediately.
As a result of reloading the current world, all the supervisor and robot controller processes are terminated and restarted.
You may wish to save some data in a file from your supervisor and robot controller programs in order to reload it when they restart.

---

#### `wb_supervisor_export_image`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_export_image(const char *filename, int quality);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    void exportImage(const std::string &file, int quality) const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def exportImage(self, file, quality):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public void exportImage(String file, int quality);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_export_image('filename', quality)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/export_image` | `service` | `webots_ros::save_image` | `string filename`<br/>`int32 quality`<br/>`---`<br/>`int8 success` |

%tab-end

%end

##### Description

*save the current 3D image of the simulator into a JPEG file, suitable for building a webcam system*

The `wb_supervisor_export_image` function saves the current image of Webots main window into a JPEG file as specified by the `filename` parameter.
If the target file exists, it will be silently overwritten.
The `quality` parameter defines the JPEG quality (in the range 1 - 100).
The `filename` parameter should specify a valid (absolute or relative) file name, e.g., "snapshot.jpg" or "/var/www/html/images/snapshot.jpg".
In fact, a temporary file is first saved, and then renamed to the requested `filename`.
This avoids having a temporary unfinished (and hence corrupted) file for webcam applications.

---

#### `wb_supervisor_movie_start_recording`
#### `wb_supervisor_movie_stop_recording`
#### `wb_supervisor_movie_is_ready`
#### `wb_supervisor_movie_failed`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_movie_start_recording(const char *filename, int width, int height, int codec, int quality, int acceleration, bool caption);
void wb_supervisor_movie_stop_recording();
bool wb_supervisor_movie_is_ready();
bool wb_supervisor_movie_failed();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    virtual void movieStartRecording(const std::string &file, int width, int height, int codec, int quality, int acceleration, bool caption) const;
    virtual void movieStopRecording();
    bool movieIsReady() const;
    bool movieFailed() const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def movieStartRecording(self, file, width, height, codec, quality, acceleration, caption):
    def movieStopRecording(self):
    def movieIsReady(self):
    def movieFailed(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public void movieStartRecording(String file, int width, int height, int codec, int quality, int acceleration, boolean caption);
  public void movieStopRecording();
  public boolean movieIsReady();
  public boolean movieFailed();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_movie_start_recording('filename', width, height, codec, quality,
acceleration, caption)
wb_supervisor_movie_stop_recording()
status = wb_supervisor_movie_is_ready()
status = wb_supervisor_movie_failed()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/movie_start_recording` | `service` | `webots_ros::supervisor_movie_start_recording` | `string filename`<br/>`int32 width`<br/>`int32 height`<br/>`int32 codec`<br/>`int32 quality`<br/>`int32 acceleration`<br/>`uint8 caption`<br/>`---`<br/>`int8 success` |
| `/supervisor/movie_stop_recording` | `service` | [`webots_ros::get_bool`](ros-api.md#common-services) | |
| `/supervisor/movie_is_ready` | `service` | `webots_ros::node_get_status` | `uint8 ask`<br/>`---`<br/>`uint8 status` |
| `/supervisor/movie_failed` | `service` | `webots_ros::node_get_status` | `uint8 ask`<br/>`---`<br/>`uint8 status` |

%tab-end

%end

##### Description

*export the current simulation into a movie file*

The `wb_supervisor_movie_start_recording` function starts saving the current simulation into a movie file.
The movie creation process will complete after the `wb_supervisor_movie_stop_recording` function is called.
The movie is saved in the file defined by the `filename` parameter.
If the `filename` doesn't end with a ".mp4" extension, the file extension is completed automatically.
If the target file exists, it will be silently overwritten.
The `codec` parameter specify the codec used when creating the movie.
Currently only MPEG-4/AVC encoding is available and the `codec` value is ignored.
The `quality` corresponds to the movie compression factor that affects the movie quality and file size.
It should be a value between 1 and 100.
Beware, that choosing a too small value may cause the video encoding program to fail because of a too low bitrate.
The movie frame rate is automatically computed based on the `basicTimeStep` value of the simulation in order to produce a real-time movie.
The `acceleration` specifies the acceleration factor of the created movie with respect to the real simulation time.
Default value is 1, i.e. no acceleration.
If `caption` parameters is set to true, a default caption is printed on the top right corner of the movie showing the current `acceleration` value.

The `wb_supervisor_movie_is_ready` function returns `true` if the application is ready to start recording a movie, i.e. if another recording process is not already running.
So it could be used to check if the encoding process is completed and the file has been created.
Note that if the recording process failed, this function will return `true`.
In order to detect a failure the `wb_supervisor_movie_failed` function has to be called.

The `wb_supervisor_movie_failed` function returns `true` if the recording process failed.
After starting a new recording process the returned value is reset to `false`.

---

#### `wb_supervisor_animation_start_recording`
#### `wb_supervisor_animation_stop_recording`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

bool wb_supervisor_animation_start_recording(const char *filename);
bool wb_supervisor_animation_stop_recording();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    virtual bool animationStartRecording(const std::string &file);
    virtual bool animationStopRecording();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def animationStartRecording(self, file):
    def animationStopRecording(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public boolean animationStartRecording(String file);
  public boolean animationStopRecording();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
success = wb_supervisor_animation_start_recording('filename')
success = wb_supervisor_animation_stop_recording()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/animation_start_recording` | `service` | [`webots_ros::set_string`](ros-api.md#common-services) | |
| `/supervisor/animation_stop_recording` | `service` | [`webots_ros::get_bool`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*export the current simulation into an animation file*

The `wb_supervisor_animation_start_recording` function starts saving the current simulation into an animation file.
The animation creation process will complete after the `wb_supervisor_animation_stop_recording` function is called.
Only one animation can be created at the same time.
The animation is saved in the file defined by the `filename` parameter.
If the target file exists, it will be silently overwritten.
The `filename` should ends with the ".html" extension.
The animation frame rate is automatically computed based on the `basicTimeStep` value of the simulation in order to produce a real-time animation.

Both `wb_supervisor_animation_start_recording` and `wb_supervisor_animation_stop_recording` functions are returning a boolean indicating their success.

---

#### `wb_supervisor_field_get_type`
#### `wb_supervisor_field_get_type_name`
#### `wb_supervisor_field_get_count`
#### `wb_supervisor_field_enable_sf_tracking`
#### `wb_supervisor_field_disable_sf_tracking`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

typedef enum {
  WB_NO_FIELD, WB_SF_BOOL, WB_SF_INT32, WB_SF_FLOAT, WB_SF_VEC2F, WB_SF_VEC3F, WB_SF_ROTATION,
  WB_SF_COLOR, WB_SF_STRING, WB_SF_NODE, WB_MF, WB_MF_BOOL, WB_MF_INT32, WB_MF_FLOAT,
  WB_MF_VEC2F, WB_MF_VEC3F, WB_MF_ROTATION, WB_MF_COLOR, WB_MF_STRING, WB_MF_NODE
} WbFieldType;

WbFieldType wb_supervisor_field_get_type(WbFieldRef field);
const char *wb_supervisor_field_get_type_name(WbFieldRef field);
int wb_supervisor_field_get_count(WbFieldRef field);
void wb_supervisor_field_enable_sf_tracking(WbFieldRef field, int sampling_period);
void wb_supervisor_field_disable_sf_tracking(WbFieldRef field);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Field.hpp>

namespace webots {
  class Field {
    typedef enum {
      SF_BOOL, SF_INT32, SF_FLOAT, SF_VEC2F, SF_VEC3F, SF_ROTATION, SF_COLOR, SF_STRING,
      SF_NODE, MF, MF_BOOL, MF_INT32, MF_FLOAT, MF_VEC2F, MF_VEC3F, MF_ROTATION, MF_COLOR,
      MF_STRING, MF_NODE
    } Type;

    Type getType() const;
    std::string getTypeName() const;
    int getCount() const;
    void enableSFTracking(int samplingPeriod);
    void disableSFTracking();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Field

class Field:
    SF_BOOL, SF_INT32, SF_FLOAT, SF_VEC2F, SF_VEC3F, SF_ROTATION, SF_COLOR, SF_STRING,
    SF_NODE, MF, MF_BOOL, MF_INT32, MF_FLOAT, MF_VEC2F, MF_VEC3F, MF_ROTATION, MF_COLOR,
    MF_STRING, MF_NODE

    def getType(self):
    def getTypeName(self):
    def getCount(self):
    def enableSFTracking(self, samplingPeriod):
    def disableSFTracking(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Field;

public class Field {
  public final static int SF_BOOL, SF_INT32, SF_FLOAT, SF_VEC2F, SF_VEC3F, SF_ROTATION,
    SF_COLOR, SF_STRING, SF_NODE, MF, MF_BOOL, MF_INT32, MF_FLOAT, MF_VEC2F, MF_VEC3F,
    MF_ROTATION, MF_COLOR, MF_STRING, MF_NODE;

  public int getType();
  public String getTypeName();
  public int getCount();
  public void enableSFTracking(int samplingPeriod);
  public void disableSFTracking();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
WB_SF_BOOL, WB_SF_INT32, WB_SF_FLOAT, WB_SF_VEC2F, WB_SF_VEC3F, WB_SF_ROTATION, WB_SF_COLOR,
WB_SF_STRING, WB_SF_NODE, WB_MF, WB_BOOL, WB_MF_INT32, WB_MF_FLOAT, B_MF_VEC2F, WB_MF_VEC3F,
WB_ROTATION, WB_MF_COLOR, WB_MF_STRING, WB_MF_NODE

type = wb_supervisor_field_get_type(field)
name = wb_supervisor_field_get_type_name(field)
count = wb_supervisor_field_get_count(field)
wb_supervisor_field_enable_sf_tracking(field, sampling_period)
wb_supervisor_field_disable_sf_tracking(field)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/field/get_type` | `service` | `webots_ros::field_get_type` | `uint64 node`<br/>`---`<br/>`int8 success` |
| `/supervisor/field/get_type_name` | `service` | `webots_ros::field_get_type_name` | `uint64 field`<br/>`---`<br/>`string name` |
| `/supervisor/field/get_count` | `service` | `webots_ros::field_get_count` | `uint64 field`<br/>`---`<br/>`int32 count` |
| `/supervisor/field/enable_sf_tracking` | `service` | `webots_ros::field_enable_sf_tracking` | `uint64 field`<br/>`int32 sampling_period`<br/>`---`<br/>`int8 sampling_period` |
| `/supervisor/field/disable_sf_tracking` | `service` | `webots_ros::field_disable_sf_tracking` | `uint64 field`<br/>`---`<br/>`int8 success` |

%tab-end

%end

##### Description

*get a handler and more information on a field in a node*

The `wb_supervisor_field_get_type` function returns the data type of a field found previously from the `wb_supervisor_node_get_field` function, as a symbolic value.
If the argument is NULL, the function returns 0.
Field types are defined in "webots/supervisor.h" and include for example: `WB_SF_FLOAT`, `WB_MF_NODE`, `WB_SF_STRING`, etc.
The `WB_MF` value doesn't represent a type but it is a *bit mask* value that can be used to check if the type represents a single field (SF) or a multiple field (MF).

The `wb_supervisor_field_get_type_name` function returns a text string corresponding to the data type of a field found previously from the `wb_supervisor_node_get_field` function.
Field type names are defined in the VRML97 specifications and include for example: `"SFFloat"`, `"MFNode"`, `"SFString"`, etc.
If the argument is NULL, the function returns the empty string.

The `wb_supervisor_field_get_count` function returns the number of items of a multiple field (MF) passed as an argument to this function.
If a single field (SF) or NULL is passed as an argument to this function, it returns -1.
Hence, this function can also be used to test if a field is MF (like `WB_MF_INT32`) or SF (like `WB_SF_BOOL`).

The `wb_supervisor_field_enable_sf_tracking` function forces Webots to stream field data to the controller.
It improves the performance as the controller by default uses a request-response pattern to get data from the field.
The `sampling_period` argument determines how often the field data should be sent to the controller.
The function is limited only to single fields.

The `wb_supervisor_field_disable_sf_tracking` function disables field data tracking.
The function is limited only to single fields.

> **Note** [C++, Java, Python]: In the oriented-object APIs, the SF\_* and MF\_* constants are available as static integers of the `Field` class (for example, Field::SF\_BOOL).
These integers can be directly compared with the output of the `Field::getType` function.

---

#### `wb_supervisor_field_get_sf_bool`
#### `wb_supervisor_field_get_sf_int32`
#### `wb_supervisor_field_get_sf_float`
#### `wb_supervisor_field_get_sf_vec2f`
#### `wb_supervisor_field_get_sf_vec3f`
#### `wb_supervisor_field_get_sf_rotation`
#### `wb_supervisor_field_get_sf_color`
#### `wb_supervisor_field_get_sf_string`
#### `wb_supervisor_field_get_sf_node`
#### `wb_supervisor_field_get_mf_bool`
#### `wb_supervisor_field_get_mf_int32`
#### `wb_supervisor_field_get_mf_float`
#### `wb_supervisor_field_get_mf_vec2f`
#### `wb_supervisor_field_get_mf_vec3f`
#### `wb_supervisor_field_get_mf_rotation`
#### `wb_supervisor_field_get_mf_color`
#### `wb_supervisor_field_get_mf_string`
#### `wb_supervisor_field_get_mf_node`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

bool wb_supervisor_field_get_sf_bool(WbFieldRef field);
int wb_supervisor_field_get_sf_int32(WbFieldRef field);
double wb_supervisor_field_get_sf_float(WbFieldRef field);
const double *wb_supervisor_field_get_sf_vec2f(WbFieldRef sf_field);
const double *wb_supervisor_field_get_sf_vec3f(WbFieldRef field);
const double *wb_supervisor_field_get_sf_rotation(WbFieldRef field);
const double *wb_supervisor_field_get_sf_color(WbFieldRef field);
const char *wb_supervisor_field_get_sf_string(WbFieldRef field);
WbNodeRef wb_supervisor_field_get_sf_node(WbFieldRef field);
bool wb_supervisor_field_get_mf_bool(WbFieldRef field, int index);
int wb_supervisor_field_get_mf_int32(WbFieldRef field, int index);
double wb_supervisor_field_get_mf_float(WbFieldRef field, int index);
const double *wb_supervisor_field_get_mf_vec2f(WbFieldRef field, int index);
const double *wb_supervisor_field_get_mf_vec3f(WbFieldRef field, int index);
const double *wb_supervisor_field_get_mf_rotation(WbFieldRef field, int index);
const double *wb_supervisor_field_get_mf_color(WbFieldRef field, int index);
const char *wb_supervisor_field_get_mf_string(WbFieldRef field, int index);
WbNodeRef wb_supervisor_field_get_mf_node(WbFieldRef field, int index);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Field.hpp>

namespace webots {
  class Field {
    bool getSFBool() const;
    int getSFInt32() const;
    double getSFFloat() const;
    const double *getSFVec2f() const;
    const double *getSFVec3f() const;
    const double *getSFRotation() const;
    const double *getSFColor() const;
    std::string getSFString() const;
    Node *getSFNode() const;
    bool getMFBool(int index) const;
    int getMFInt32(int index) const;
    double getMFFloat(int index) const;
    const double *getMFVec2f(int index) const;
    const double *getMFVec3f(int index) const;
    const double *getMFRotation(int index) const;
    const double *getMFColor(int index) const;
    std::string getMFString(int index) const;
    Node *getMFNode(int index) const;
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Field

class Field:
    def getSFBool(self):
    def getSFInt32(self):
    def getSFFloat(self):
    def getSFVec2f(self):
    def getSFVec3f(self):
    def getSFRotation(self):
    def getSFColor(self):
    def getSFString(self):
    def getSFNode(self):
    def getMFBool(self, index):
    def getMFInt32(self, index):
    def getMFFloat(self, index):
    def getMFVec2f(self, index):
    def getMFVec3f(self, index):
    def getMFRotation(self, index):
    def getMFColor(self, index):
    def getMFString(self, index):
    def getMFNode(self, index):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Field;

public class Field {
  public boolean getSFBool();
  public int getSFInt32();
  public double getSFFloat();
  public double[] getSFVec2f();
  public double[] getSFVec3f();
  public double[] getSFRotation();
  public double[] getSFColor();
  public String getSFString();
  public Node getSFNode();
  public boolean getMFBool(int index);
  public int getMFInt32(int index);
  public double getMFFloat(int index);
  public double[] getMFVec2f(int index);
  public double[] getMFVec3f(int index);
  public double[] getMFColor(int index);
  public double[] getMFRotation(int index);
  public String getMFString(int index);
  public Node getMFNode(int index);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
b = wb_supervisor_field_get_sf_bool(field)
i = wb_supervisor_field_get_sf_int32(field)
f = wb_supervisor_field_get_sf_float(field)
x_y_array = wb_supervisor_field_get_sf_vec2f(field)
x_y_z_array = wb_supervisor_field_get_sf_vec3f(field)
x_y_z_alpha_array = wb_supervisor_field_get_sf_rotation(field)
r_g_b_array = wb_supervisor_field_get_sf_color(field)
s = wb_supervisor_field_get_sf_string(field)
node = wb_supervisor_field_get_sf_node(field)
b = wb_supervisor_field_get_mf_bool(field, index)
i = wb_supervisor_field_get_mf_int32(field, index)
f = wb_supervisor_field_get_mf_float(field, index)
x_y_array = wb_supervisor_field_get_mf_vec2f(field, index)
x_y_z_array = wb_supervisor_field_get_mf_vec3f(field, index)
x_y_z_a_array = wb_supervisor_field_get_mf_rotation(field, index)
r_g_b_array = wb_supervisor_field_get_mf_color(field, index)
s = wb_supervisor_field_get_mf_string(field, index)
node = wb_supervisor_field_get_mf_node(field, index)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/field/get_bool` | `service` | `webots_ros::field_get_bool` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>`uint8 value` |
| `/supervisor/field/get_int32` | `service` | `webots_ros::field_get_int32` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>`int32 value` |
| `/supervisor/field/get_float` | `service` | `webots_ros::field_get_float` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>`float64 value` |
| `/supervisor/field/get_vec2f` | `service` | `webots_ros::field_get_vec2f` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `value`<br/><br/>Note: the 'z' coordinate should be ignored. |
| `/supervisor/field/get_vec3f` | `service` | `webots_ros::field_get_vec3f` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) value |
| `/supervisor/field/get_rotation` | `service` | `webots_ros::field_get_rotation` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>[`geometry_msgs/Quaternion`](http://docs.ros.org/api/geometry_msgs/html/msg/Quaternion.html) value |
| `/supervisor/field/get_color` | `service` | `webots_ros::field_get_color` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>[`std_msgs/ColorRGBA`](http://docs.ros.org/api/std_msgs/html/msg/ColorRGBA.html) value |
| `/supervisor/field/get_string` | `service` | `webots_ros::field_get_string` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>`string value` |
| `/supervisor/field/get_node` | `service` | `webots_ros::field_get_node` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>`uint64 node` |

%tab-end

%end

##### Description

*get the value of a field*

The `wb_supervisor_field_get_sf_*` functions retrieve the value of a specified single `field` (SF).
The type of the field has to match the name of the function used, otherwise the return value is undefined (and a warning message is displayed).
If the `field` parameter is NULL, it has the wrong type, or the `index` is not valid, then a default value is returned.
Default values are defined as `0` and `0.0` for integer and double values, `false` in case of boolean values, NULL for vectors and pointers and the empty string `""` for strings.

The `wb_supervisor_field_get_mf_*` functions work the same way as the `wb_supervisor_field_get_sf_*` functions but with multiple `field` argument.
They take an additional `index` argument which refers to the index of the item in the multiple field (MF).

The type of the field has to match with the name of the function.
The index should be comprised between minus the total number of items and the total number of items minus one.
Using a negative index starts the count from the last element of the field until the first one.
Index -1 represents the last item and the first item is represented by index 0 (or minus number of items).

> **Note**: If a `wb_supervisor_field_set_*` function is executed just before a corresponding `wb_supervisor_field_get_*` function, in the same time step, the controller library will not send the query to Webots, but answer directly with the value that has just been set before.

---

#### `wb_supervisor_field_set_sf_bool`
#### `wb_supervisor_field_set_sf_int32`
#### `wb_supervisor_field_set_sf_float`
#### `wb_supervisor_field_set_sf_vec2f`
#### `wb_supervisor_field_set_sf_vec3f`
#### `wb_supervisor_field_set_sf_rotation`
#### `wb_supervisor_field_set_sf_color`
#### `wb_supervisor_field_set_sf_string`
#### `wb_supervisor_field_set_mf_bool`
#### `wb_supervisor_field_set_mf_int32`
#### `wb_supervisor_field_set_mf_float`
#### `wb_supervisor_field_set_mf_vec2f`
#### `wb_supervisor_field_set_mf_vec3f`
#### `wb_supervisor_field_set_mf_rotation`
#### `wb_supervisor_field_set_mf_color`
#### `wb_supervisor_field_set_mf_string`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_field_set_sf_bool(WbFieldRef field, bool value);
void wb_supervisor_field_set_sf_int32(WbFieldRef field, int value);
void wb_supervisor_field_set_sf_float(WbFieldRef field, double value);
void wb_supervisor_field_set_sf_vec2f(WbFieldRef sf_field, const double values[2]);
void wb_supervisor_field_set_sf_vec3f(WbFieldRef field, const double values[3]);
void wb_supervisor_field_set_sf_rotation(WbFieldRef field, const double values[4]);
void wb_supervisor_field_set_sf_color(WbFieldRef field, const double values[3]);
void wb_supervisor_field_set_sf_string(WbFieldRef field, const char *value);
void wb_supervisor_field_set_mf_bool(WbFieldRef field, int index, bool value);
void wb_supervisor_field_set_mf_int32(WbFieldRef field, int index, int value);
void wb_supervisor_field_set_mf_float(WbFieldRef field, int index, double value);
void wb_supervisor_field_set_mf_vec2f(WbFieldRef field, int index, const double values[2]);
void wb_supervisor_field_set_mf_vec3f(WbFieldRef field, int index, const double values[3]);
void wb_supervisor_field_set_mf_rotation(WbFieldRef field, int index, const double values[4]);
void wb_supervisor_field_set_mf_color(WbFieldRef field, int index, const double values[3]);
void wb_supervisor_field_set_mf_string(WbFieldRef field, int index, const char *value);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Field.hpp>

namespace webots {
  class Field {
    void setSFBool(bool value);
    void setSFInt32(int value);
    void setSFFloat(double value);
    void setSFVec2f(const double values[2]);
    void setSFVec3f(const double values[3]);
    void setSFRotation(const double values[4]);
    void setSFColor(const double values[3]);
    void setSFString(const std::string &value);
    void setMFBool(int index, bool value);
    void setMFInt32(int index, int value);
    void setMFFloat(int index, double value);
    void setMFVec2f(int index, const double values[2]);
    void setMFVec3f(int index, const double values[3]);
    void setMFRotation(int index, const double values[4]);
    void setMFColor(int index, const double values[3]);
    void setMFString(int index, const std::string &value);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Field

class Field:
    def setSFBool(self, value):
    def setSFInt32(self, value):
    def setSFFloat(self, value):
    def setSFVec2f(self, values):
    def setSFVec3f(self, values):
    def setSFRotation(self, values):
    def setSFColor(self, values):
    def setSFString(self, value):
    def setMFBool(self, index, value):
    def setMFInt32(self, index, value):
    def setMFFloat(self, index, value):
    def setMFVec2f(self, index, values):
    def setMFVec3f(self, index, values):
    def setMFRotation(self, index, values):
    def setMFColor(self, index, values):
    def setMFString(self, index, value):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Field;

public class Field {
  public void setSFBool(boolean value);
  public void setSFInt32(int value);
  public void setSFFloat(double value);
  public void setSFVec2f(double values[2]);
  public void setSFVec3f(double values[3]);
  public void setSFRotation(double values[4]);
  public void setSFColor(double values[3]);
  public void setSFString(String value);
  public void setMFBool(int index, boolean value);
  public void setMFInt32(int index, int value);
  public void setMFFloat(int index, double value);
  public void setMFVec2f(int index, double values[2]);
  public void setMFVec3f(int index, double values[3]);
  public void setMFRotation(int index, double values[4]);
  public void setMFColor(int index, double values[3]);
  public void setMFString(int index, String value);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_field_set_sf_bool(field, value)
wb_supervisor_field_set_sf_int32(field, value)
wb_supervisor_field_set_sf_float(field, value)
wb_supervisor_field_set_sf_vec2f(field, [x y])
wb_supervisor_field_set_sf_vec3f(field, [x y z])
wb_supervisor_field_set_sf_rotation(field, [x y z alpha])
wb_supervisor_field_set_sf_color(field, [r g b])
wb_supervisor_field_set_sf_string(field, 'value')
wb_supervisor_field_set_mf_bool(field, index, value)
wb_supervisor_field_set_mf_int32(field, index, value)
wb_supervisor_field_set_mf_float(field, index, value)
wb_supervisor_field_set_mf_vec2f(field, index, [x y])
wb_supervisor_field_set_mf_vec3f(field, index, [x y z])
wb_supervisor_field_set_mf_rotation(field, index, [x y z a])
wb_supervisor_field_set_mf_color(field, index, [r g b])
wb_supervisor_field_set_mf_string(field, index, 'value')
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/field/set_bool` | `service` | `webots_ros::field_set_bool` | `uint64 field`<br/>`int32 index`<br/>`uint8 value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/set_int32` | `service` | `webots_ros::field_set_int32` | `uint64 field`<br/>`int32 index`<br/>`int32 value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/set_float` | `service` | `webots_ros::field_set_float` | `uint64 field`<br/>`int32 index`<br/>`float64 value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/set_vec2f` | `service` | `webots_ros::field_set_vec2f` | `uint64 field`<br/>`int32 index`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `value`<br/>`---`<br/>`int32 success`<br/><br/>Note: the 'z' coordinate is ignored. |
| `/supervisor/field/set_vec3f` | `service` | `webots_ros::field_set_vec3f` | `uint64 field`<br/>`int32 index`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/set_rotation` | `service` | `webots_ros::field_set_rotation` | `uint64 field`<br/>`int32 index`<br/>[`geometry_msgs/Quaternion`](http://docs.ros.org/api/geometry_msgs/html/msg/Quaternion.html) `value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/set_color` | `service` | `webots_ros::field_set_color` | `uint64 field`<br/>`int32 index`<br/>[`std_msgs/ColorRGBA`](http://docs.ros.org/api/std_msgs/html/msg/ColorRGBA.html) `value`<br/>`---`<br/>`int32 success`<br/><br/>Note: the 'a' component is ignored. |
| `/supervisor/field/set_string` | `service` | `webots_ros::field_set_string` | `uint64 field`<br/>`int32 index`<br/>`string value`<br/>`---`<br/>`int32 success`<br/> |

%tab-end

%end

##### Description

*set the value of a field*

The `wb_supervisor_field_set_sf_*` functions assign a value to a specified single `field` (SF).
The type of the field has to match with the name of the function used, otherwise the value of the field remains unchanged (and a warning message is displayed).

The `wb_supervisor_field_set_mf_*` functions work the same way as the `wb_supervisor_field_set_sf_*` functions but with a multiple `field` (MF) argument.
They take an additional `index` argument which refers to the index of the item in the multiple field.

The type of the field has to match with the name of the function.
The index should be comprised between minus the total number of items and the total number of items minus one.
Using a negative index starts the count from the last element of the field until the first one.
Index -1 represents the last item and the first item is represented by index 0 (or minus number of items).

The set operations are received by Webots from possibly several supervisors running concurrently.
In order to ensure reproducible simulation results, they are executed only once all set operations are received, just before advancing the simulation time.
The order of execution of the set operations is defined by the order of the [Robot](robot.md) nodes in the scene tree.
As a consequence, if a supervisor sets the translation field of a node and immediately retrieves the absolute position of the same node using the `wb_supervisor_node_get_position` function, it will actually get the previous position of the node.
This is because the execution of the set operation is postponed to the beginning of the next simulation step.
In order to retrieve the new position of the node, a `wb_robot_step` function call with a non-zero argument should be executed before calling the `wb_supervisor_node_get_position` function.

An exception to this rule applies if one of the following functions is executed:
- [`wb_supervisor_field_insert_mf_*`](#wb_supervisor_field_insert_mf_bool)
- [`wb_supervisor_field_import_mf_*`](#wb_supervisor_field_import_mf_node)
- [`wb_supervisor_field_import_sf_*`](#wb_supervisor_field_import_sf_node)
- [`wb_supervisor_field_remove_mf`](#wb_supervisor_field_remove_mf)
- [`wb_supervisor_field_remove_sf`](#wb_supervisor_field_remove_sf)

In this case, the set operations queued in the current step of this controller only are executed just before the insert, import or remove operation.

> **Note**: Since Webots 7.4.4, the inertia of a solid is no longer automatically reset when changing its translation or rotation using `wb_supervisor_field_set_sf_vec2f` and `wb_supervisor_field_set_sf_rotation` functions.
If needed, the user has to explicitly call [this section](#wb_supervisor_node_reset_physics) function.

##### Examples

The "texture\_change.wbt" world, located in the "projects/samples/howto/texture\_change/worlds" directory, shows how to change a texture from the supervisor while the simulation is running.
The "soccer.wbt" world, located in the "projects/samples/demos/worlds" directory, provides a simple example for getting and setting fields with the above described functions.

---

#### `wb_supervisor_field_insert_mf_bool`
#### `wb_supervisor_field_insert_mf_int32`
#### `wb_supervisor_field_insert_mf_float`
#### `wb_supervisor_field_insert_mf_vec2f`
#### `wb_supervisor_field_insert_mf_vec3f`
#### `wb_supervisor_field_insert_mf_rotation`
#### `wb_supervisor_field_insert_mf_color`
#### `wb_supervisor_field_insert_mf_string`
#### `wb_supervisor_field_remove_mf`
#### `wb_supervisor_field_remove_sf`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_field_insert_mf_bool(WbFieldRef field, int index, bool value);
void wb_supervisor_field_insert_mf_int32(WbFieldRef field, int index, int value);
void wb_supervisor_field_insert_mf_float(WbFieldRef field, int index, double value);
void wb_supervisor_field_insert_mf_vec2f(WbFieldRef field, int index, const double values[2]);
void wb_supervisor_field_insert_mf_vec3f(WbFieldRef field, int index, const double values[3]);
void wb_supervisor_field_insert_mf_rotation(WbFieldRef field, int index, const double values[4]);
void wb_supervisor_field_insert_mf_color(WbFieldRef field, int index, const double values[3]);
void wb_supervisor_field_insert_mf_string(WbFieldRef field, int index, const char *value);

void wb_supervisor_field_remove_mf(WbFieldRef field, int index);
void wb_supervisor_field_remove_sf(WbFieldRef field);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Field.hpp>

namespace webots {
  class Field {
    void insertSFBool(bool value);
    void insertSFInt32(int value);
    void insertSFFloat(double value);
    void insertSFVec2f(const double values[2]);
    void insertSFVec3f(const double values[3]);
    void insertSFRotation(const double values[4]);
    void insertSFColor(const double values[3]);
    void insertSFString(const std::string &value);
    void insertMFBool(int index, bool value);
    void insertMFInt32(int index, int value);
    void insertMFFloat(int index, double value);
    void insertMFVec2f(int index, const double values[2]);
    void insertMFVec3f(int index, const double values[3]);
    void insertMFRotation(int index, const double values[4]);
    void insertMFColor(int index, const double values[3]);
    void insertMFString(int index, const std::string &value);
    void removeMF(int index);
    void removeSF();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Field

class Field:
    def insertMFBool(self, index, value):
    def insertMFInt32(self, index, value):
    def insertMFFloat(self, index, value):
    def insertMFVec2f(self, index, values):
    def insertMFVec3f(self, index, values):
    def insertMFRotation(self, index, values):
    def insertMFColor(self, index, values):
    def insertMFString(self, index, value):
    def removeMF(self, index):
    def removeSF(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Field;

public class Field {
  public void insertMFBool(int index, boolean value);
  public void insertMFInt32(int index, int value);
  public void insertMFFloat(int index, double value);
  public void insertMFVec2f(int index, double values[2]);
  public void insertMFVec3f(int index, double values[3]);
  public void insertMFRotation(int index, double values[4]);
  public void insertMFColor(int index, double values[3]);
  public void insertMFString(int index, String value);
  public void removeMF(int index);
  public void removeSF();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_field_insert_mf_bool(field, index, value)
wb_supervisor_field_insert_mf_int32(field, index, value)
wb_supervisor_field_insert_mf_float(field, index, value)
wb_supervisor_field_insert_mf_vec2f(field, index, [x y])
wb_supervisor_field_insert_mf_vec3f(field, index, [x y z])
wb_supervisor_field_insert_mf_rotation(field, index, [x y z a])
wb_supervisor_field_insert_mf_color(field, index, [r g b])
wb_supervisor_field_insert_mf_string(field, index, 'value')
wb_supervisor_field_remove_mf(field, index)
wb_supervisor_field_remove_sf(field)
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/field/insert_bool` | `service` | `webots_ros::field_set_bool` | `uint64 field`<br/>`int32 index`<br/>`uint8 value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/insert_int32` | `service` | `webots_ros::field_set_int32` | `uint64 field`<br/>`int32 index`<br/>`int32 value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/insert_float` | `service` | `webots_ros::field_set_float` | `uint64 field`<br/>`int32 index`<br/>`float64 value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/insert_vec2f` | `service` | `webots_ros::field_set_vec2f` | `uint64 field`<br/>`int32 index`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `value`<br/>`---`<br/>`int32 success`<br/><br/>Note: the 'z' coordinate is ignored. |
| `/supervisor/field/insert_vec3f` | `service` | `webots_ros::field_set_vec3f` | `uint64 field`<br/>`int32 index`<br/>[`geometry_msgs/Vector3`](http://docs.ros.org/api/geometry_msgs/html/msg/Vector3.html) `value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/insert_rotation` | `service` | `webots_ros::field_set_rotation` | `uint64 field`<br/>`int32 index`<br/>[`geometry_msgs/Quaternion`](http://docs.ros.org/api/geometry_msgs/html/msg/Quaternion.html) `value`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/insert_color` | `service` | `webots_ros::field_set_color` | `uint64 field`<br/>`int32 index`<br/>[`std_msgs/ColorRGBA`](http://docs.ros.org/api/std_msgs/html/msg/ColorRGBA.html) `value`<br/>`---`<br/>`int32 success`<br/><br/>Note: the 'a' component is ignored. |
| `/supervisor/field/insert_string` | `service` | `webots_ros::field_set_string` | `uint64 field`<br/>`int32 index`<br/>`string value`<br/>`---`<br/>`int32 success`<br/> |
| `/supervisor/field/remove` | `service` | `webots_ros::field_remove` | `uint64 field`<br/>`int32 index`<br/>`---`<br/>`int32 success` |

%tab-end

%end

##### Description

*insert or remove a value in a field*

The `wb_supervisor_field_insert_mf_*` functions insert an item to a specified multiple `field` (MF).
The type of the field has to match with the name of the function used, otherwise the field remains unchanged (and a warning message is displayed).
The `index` parameter defines the position in the MF field where the new item will be inserted.
It can be positive or negative.
Here are a few examples for the `index` parameter:

- 0: insert at the beginning of the field.
- 1: insert at the second index.
- 2: insert at the third index.
- -1: insert at the last index.
- -2: insert at the second index from the end of the field.
- -3: insert at the third index from the end.

The `wb_supervisor_field_remove_sf/mf` functions remove an item from a specified `field` (MF or SF).
If the item is the [Robot](robot.md) node itself, it is removed only at the end of the step.

---

#### `wb_supervisor_field_import_mf_node`
#### `wb_supervisor_field_import_mf_node_from_string`
#### `wb_supervisor_field_import_sf_node`
#### `wb_supervisor_field_import_sf_node_from_string`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

void wb_supervisor_field_import_mf_node(WbFieldRef field, int position, const char *filename);
void wb_supervisor_field_import_mf_node_from_string(WbFieldRef field, int position, const char *node_string);

void wb_supervisor_field_import_sf_node(WbFieldRef field, const char *filename);
void wb_supervisor_field_import_sf_node_from_string(WbFieldRef field, const char *node_string);
```

%tab-end

%tab "C++"

```cpp
#include <webots/Field.hpp>

namespace webots {
  class Field {
    void importMFNode(int position, const std::string &filename);
    void importMFNodeFromString(int position, const std::string &nodeString);
    void importSFNode(const std::string &filename);
    void importSFNodeFromString(const std::string &nodeString);
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Field

class Field:
    def importMFNode(self, position, filename):
    def importMFNodeFromString(self, position, nodeString):
    def importSFNode(self, filename):
    def importSFNodeFromString(self, nodeString):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Field;

public class Field {
  public void importMFNode(int position, String filename);
  public void importMFNodeFromString(int position, String nodeString);
  public void importSFNode(String filename);
  public void importSFNodeFromString(String nodeString);
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
wb_supervisor_field_import_mf_node(field, position, 'filename')
wb_supervisor_field_import_mf_node_from_string(field, position, 'node_string')
wb_supervisor_field_import_sf_node(field, 'filename')
wb_supervisor_field_import_sf_node_from_string(field, 'node_string')
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/field/import_node` | `service` | `webots_ros::field_import_node` | `uint64 field`<br/>`int32 position`<br/>`string filename`<br/>`---`<br/>`int32 success` |
| `/supervisor/field/import_node_from_string` | `service` | `webots_ros::field_import_node_from_string` | `uint64 field`<br/>`int32 position`<br/>`string nodeString`<br/>`---`<br/>`int32 success` |

%tab-end

%end

##### Description

*import a node into an MF\_NODE or SF\_NODE field (typically a "children" field)*

The `wb_supervisor_field_import_mf_node` and `wb_supervisor_field_import_sf_node` functions import a Webots node into an MF\_NODE or SF\_NODE field.
This node should be defined in a `.wbo` file referenced by the `filename` parameter.
Such a file can be produced easily from Webots by selecting a node in the scene tree window and using the `Export` button.

The `position` parameter defines the position in the MF\_NODE where the new node will be inserted.
It can be positive or negative.
Here are a few examples for the `position` parameter:

- 0: insert at the beginning of the scene tree.
- 1: insert at the second position.
- 2: insert at the third position.
- -1: insert at the last position.
- -2: insert at the second position from the end of the scene tree.
- -3: insert at the third position from the end.

The `filename` parameter can be specified as an absolute or a relative path.
In the later case, it is relative to the location of the supervisor controller.

This function is typically used in order to add a node into a "children" field.
Note that a node can be imported into the scene tree by calling this function with the "children" field of the root node.

The `wb_supervisor_field_import_sf/mf_node_from_string` functions are very similar to the `wb_supervisor_field_import_sf/mf_node` function, except that the node is constructed from the `node_string` string.
For example, if you want to create a new robot with a specific controller:

```c
#include <webots/robot.h>
#include <webots/supervisor.h>

#define TIME_STEP 32

int main(int argc, char **argv) {
  wb_robot_init();

  WbNodeRef root_node = wb_supervisor_node_get_root();
  WbFieldRef root_children_field = wb_supervisor_node_get_field(root_node, "children");
  wb_supervisor_field_import_mf_node_from_string(root_children_field, 4, "DEF MY_ROBOT Robot { controller \"my_controller\" }");

  ...
}
```

> **Note**: To remove a node use the `wb_supervisor_field_remove_mf` function.

---

#### `wb_supervisor_virtual_reality_headset_is_used`
#### `wb_supervisor_virtual_reality_headset_get_position`
#### `wb_supervisor_virtual_reality_headset_get_orientation`

%tab-component "language"

%tab "C"

```c
#include <webots/supervisor.h>

bool          wb_supervisor_virtual_reality_headset_is_used();
const double *wb_supervisor_virtual_reality_headset_get_position();
const double *wb_supervisor_virtual_reality_headset_get_orientation();
```

%tab-end

%tab "C++"

```cpp
#include <webots/Supervisor.hpp>

namespace webots {
  class Supervisor : public Robot {
    bool virtualRealityHeadsetIsUsed();
    const double *virtualRealityHeadsetGetPosition();
    const double *virtualRealityHeadsetGetOrientation();
    // ...
  }
}
```

%tab-end

%tab "Python"

```python
from controller import Supervisor

class Supervisor (Robot):
    def virtualRealityHeadsetIsUsed(self):
    def virtualRealityHeadsetGetPosition(self):
    def virtualRealityHeadsetGetOrientation(self):
    # ...
```

%tab-end

%tab "Java"

```java
import com.cyberbotics.webots.controller.Supervisor;

public class Supervisor extends Robot {
  public boolean virtualRealityHeadsetIsUsed();
  public double[] virtualRealityHeadsetGetPosition();
  public double[] virtualRealityHeadsetGetOrientation();
  // ...
}
```

%tab-end

%tab "MATLAB"

```MATLAB
used = wb_supervisor_virtual_reality_headset_is_used()
position = wb_supervisor_virtual_reality_headset_get_position()
orientation = wb_supervisor_virtual_reality_headset_get_orientation()
```

%tab-end

%tab "ROS"

| name | service/topic | data type | data type definition |
| --- | --- | --- | --- |
| `/supervisor/vitual_reality_headset_get_orientation` | `service` | `webots_ros::supervisor_virtual_reality_headset_get_orientation` | `uint8 ask`<br/>`---`<br/>[`geometry_msgs/Point`](http://docs.ros.org/api/geometry_msgs/html/msg/Point.html) position |
| `/supervisor/vitual_reality_headset_get_position` | `service` | `webots_ros::supervisor_virtual_reality_headset_get_position` | `uint8 ask`<br/>`---`<br/>[`geometry_msgs/Quaternion`](http://docs.ros.org/api/geometry_msgs/html/msg/Quaternion.html) orientation |
| `/supervisor/vitual_reality_headset_is_used` | `service` | [`webots_ros::get_bool`](ros-api.md#common-services) | |

%tab-end

%end

##### Description

*check if a virtual reality headset is used and get its position and orientation*

The `wb_supervisor_virtual_reality_headset_is_used` function returns true if a virtual reality headset is currently used to view the simulation.
For more information about how to use a virtual reality headset refer to the [User Guide](../guide/computer-peripherals.md#virtual-reality-headset).

The `wb_supervisor_virtual_reality_headset_get_position` and `wb_supervisor_virtual_reality_headset_get_orientation` functions return respectively the current position and orientation of the virtual reality headset as a vector of 3 doubles and a matrix containing 9 doubles that should be interpreted as a 3 x 3 orthogonal rotation matrix:
```
[ R[0] R[1] R[2] ]
[ R[3] R[4] R[5] ]
[ R[6] R[7] R[8] ]
```
If the position or the orientation of the virtual reality headset is not tracked or no virtual reality headset is currently used, these functions will return `NaN` (Not a Number) values.
