## Festo's Robotino 3

%robot robotino3 images/robots/robotino3/model.thumbnail.png

Designed by Festo, [Robotino 3](robotino3.md) is a three-wheeled (mecanum) robot.
The model is a modular mobile platform used for educational, training and research purposes.
It is controlled by an industry-standard PC system (ROS compatible) and it is also customizable with accessories to adapt to any specific need.

More information on the Robotino 3 robot can be found on the [constructor website](https://ip.festo-didactic.com/InfoPortal/Robotino3/Overview/EN/index.html).

### Robotino3 PROTO

Derived from [Robot](../reference/robot.md).

```
Robotino3 {
  SFVec3f     translation          0 0 0
  SFRotation  rotation             1 0 0 -1.5708
  SFString    name                 "Robotino 3"
  SFString    model                "Festo - Robotino 3"
  SFString    controller           "robotino3"
  MFString    controllerArgs       []
  SFString    customData           ""
  SFBool      supervisor           FALSE
  SFBool      synchronization      TRUE
  SFBool      selfCollision        FALSE   # Enables/disables the detection of collisions within the robot.
  MFNode      bodySlot             []
  MFNode      cameraSlot           []
  MFNode      topSlot              []
  MFNode      middleSlot           []
  MFNode      bottomSlot           []
  SFString    infraredSensorModel  "SharpGP2D120"
}
```

> **File location**: "[WEBOTS\_HOME/projects/robots/festo/robotino3/protos/Robotino3.proto]({{ url.github_tree }}/projects/robots/festo/robotino3/protos/Robotino3.proto)"

#### Robotino 3 Field Summary

- `bodySlot` : Extends the robot with new nodes (such as the `Robotino3Platform` for example).
- `cameraSlot` : Extends the robot with a camera (such as the `Robotino3Webcam` for example).
- `topSlot`: Extends the robot with new parts such as lidar.
- `middleSlot`: Extends the robot with new parts such as lidar.
- `bottomSlot`: Extends the robot with new parts such as lidar.
- `infraredSensorModel`: Defines the infrared sensors used (it should be one of `SharpGP2D120`, `SharpGP2Y0A41SK0F` or `SharpGP2Y0A02YK0F`).


### Sample

You will find the following sample in this folder: "[WEBOTS\_HOME/projects/robots/festo/robotino3/worlds]({{ url.github_tree }}/projects/robots/festo/robotino3/worlds)".

> **Note:** For the mecanum wheels to behave correctly, the following [ContactProperties](../reference/contactproperties.md) should be added in the `contactProperties` field of the [WorldInfo](../reference/worldinfo.md) node:
```
  contactProperties [
    ContactProperties {
      material1 "WheelMat"
      coulombFriction [
        0, 2, 0
      ]
      bounce 0
    }
  ]
```

#### robotino3.wbt

![robotino3.wbt.png](images/robots/robotino3/robotino3.wbt.thumbnail.jpg) This simulation shows a Robotino 3 moving in an industrial environment using a Braitenberg algorithm using the information received by its nine infrared sensors.
