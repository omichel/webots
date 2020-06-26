## Tutorial 7: Your First PROTO (20 Minutes)

The aim of this tutorial is to create a PROTO file corresponding to the four wheels robot from the previous tutorial.

### Copy the Robot Definition

For now, the definition of the robot is completely contained in the world file.
Moving this definition in a PROTO file will allow you to use this robot without the need to completely copy and paste its definition (either several times in the same world or in different worlds).

> **Hands-on #1**: Open the world `4_wheels_robot.wbt` created in the previous tutorial in your favorite text editor.
Create a new empty text file in the `protos` folder of your project called `FourWheelsRobot.proto` and open this file in the text editor too, this file will contain the definition of your robot.

Any PROTO file should at least respect the following structure:
```
PROTO protoName [
  protoFields
]
{
  protoBody
}
```

The `protoName` should be the name of the PROTO file (i.e. `FourWheelsRobot` in this case), `protoFields` defines the modifiable fields of the PROTO node (this part should be empty for now) and the `protoBody` is the definition of the root node (i.e. the [Robot](../reference/robot.md) node in this case).

> **Hands-on #2**: Write this default structure in your PROTO file with the correct `protoName`.
From the `4_wheels_robot.wbt` world file copy the robot node (starting with `Robot {` and ending with the final `}`) and paste it in your PROTO file instead of the `protoBody`.
Finally, save the PROTO file.


  **Solution**: You should have something like this:

```
  PROTO FourWheelsRobot [

  ]
  {
    Robot {
      # list of fields
    }
  }
```

### Use the PROTO Node.

This new PROTO node is now available for each world in your current project.

%figure "The PROTO is now visible in the Add a node window."

![tutorial_proto.png](images/tutorial_proto.thumbnail.jpg)

%end

> **Hands-on #3**: Open the `4_wheels_robot.wbt` world in Webots and add the `FourWheelsRobot` node (that you just defined).
The node is located in `PROTO nodes (Current Project) / FourWheelsRobot (Robot)`.

A second 4 wheels robot should have been added at the exact same location as the already existing one.

### Adding Fields

As you probably noticed, this new PROTO node doesn't have any open field, it is therefore impossible to translate, rotate or change the controller for example.
It is very easy to add new fields to a PROTO node and to link them with internal fields.
This should be done in the PROTO interface part (part between the `[` and the `]`).

> **Hands-on #4**: Edit your PROTO file in your text editor and add the definition of the `translation`, `rotation` and `bodyMass` field in the PROTO interface part:
```
  field SFVec3f    translation  0 0 0
  field SFRotation rotation     0 0 1 0
  field SFFloat    bodyMass     1
```
Your PROTO node has now two open fields but they are not linked to any internal field.
To link the fields you should use the IS keyword, simply replace the `translation x y z` and `rotation x y z angle` fields of the [Robot](../reference/robot.md) node by:
```
  translation IS translation
  rotation IS rotation
```

And the `mass` field of the [Physics](../reference/physics.md) node of the [Robot](../reference/robot.md) node by:
```
  mass IS bodyMass
```
Save your PROTO file, it should now look like this:
```
PROTO FourWheelsRobot [
  field SFVec3f    translation  0 0 0
  field SFRotation rotation     0 0 1 0
  field SFFloat    bodyMass     1
]
{
  Robot {
    translation IS translation
    rotation IS rotation
    children [
      # list of children nodes
    ]
    boundingObject USE BODY
    physics Physics {
      density -1
      mass IS bodyMass
    }
    controller "four_wheels_collision_avoidance"
  }
}
```

You can now save your simulation in Webots and revert it. The `translation`, `rotation` and `bodyMass` of the `FourWheelsRobot` node can now be changed (either in the [scene tree](the-scene-tree.md) or using the handles in the [3D view](the-3d-window.md)).

The same mechanism could also be used to expose the `controller` field of the [Robot](../reference/robot.md) node.

### Solution: PROTO File

To compare your PROTO file with [the solution](https://github.com/cyberbotics/webots/blob/master/projects/samples/tutorials/protos/FourWheelsRobot.proto), go to your files and find the folder named "my\_first\_simulation" created in [Tutorial 1](tutorial-1-your-first-simulation-in-webots.md), then go to the "protos" folder and open with a text editor the right PROTO.

### Conclusion

You are now able to create PROTO nodes from any nodes you created in Webots.

More specifically, you learnt how to copy the node definition in the PROTO file and how to open and link PROTO fields to internal node fields.

To go further, the [PROTO chapter](../reference/proto.md) of the reference manual explains in detail all the possibilities of the PROTO mechanism.
