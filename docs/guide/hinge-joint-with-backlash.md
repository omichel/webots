## HingeJointWithBacklash

%figure

![HingeJointWithBacklash](images/joints/HingeJointWithBacklash/HingeJointWithBacklash.png)

%end

Derived from [HingeJoint](../reference/hingejoint.md).

### HingeJointWithBacklash Model

```
HingeJointWithBacklash [
  SFNode       jointParameters   HingeJointParameters {}
  SFFloat      backlash          0.01   # (0, inf)
  SFFloat      gearMass          0.1    # [0, inf)
  MFNode       device            [ ]    # {RotationalMotor, PositionSensor, Brake}
  MFNode       outputSensor      [ ]    # {PositionSensor}
  MFNode       startPoint        NULL   # {Group, Transform, or Shape}
  SFNode       endPoint          NULL   # {Solid, SolidReference, or Slot}
]
```

The backlash effect is modeled by concatenating two consecutive [HingeJoint](../reference/hingejoint.md) where the first axis can freely turn while the second's range of movement is limited by setting the values of `minStop` and `maxStop` according to the desired `backlash`.
By doing so, the second hinge will begin to move only when either limit is reached.
Given the nature of the implementation, this [PROTO](../reference/proto.md) won't achieve the desired effect if used in kinematic mode and therefore it requires the `endPoint` to have physics enabled.

### Field Summary

- `backlash`: this field specifies the amount of clearance intrinsic to the transmission. The backlash is defined as the maximum distance that the driving gear tooth can travel without any actual contact or transfer of power to the driven gear.
If set to zero, a single [HingeJoint](../reference/hingejoint.md) is used instead and the fields `gearMass`, `outputSensor` and `startPoint` are ignored.

- `gearMass`: this field specifies the mass of the gear on the input side, namely the object that is represented by the `startPoint` field. By default it's assumed as being negligible but can be adjusted accordingly.

- `device`: this field optionally specifies a [RotationalMotor](../reference/rotationalmotor.md), an angular [PositionSensor](../reference/positionsensor.md) and/or a [Brake](../reference/brake.md) device.
If no motor is specified, the joint is a passive joint.

- `outputSensor`: this field optionally specifies an angular [PositionSensor](../reference/positionsensor.md) fixed on the output axis of the joint.

> **Note**: what this sensor returns is the current position of the output axis in the range [-backlash/2, backlash/2]. When either limit is reached, the axis will begin to move.

- `startPoint`: this field optionally specifies the shape of the object attached to the axis at the input of the joint. It must be either a [Transform](../reference/transform.md), a [Group](../reference/group.md) or a [Shape](../reference/shape.md).
This object doesn't affect the joint in any way but can be useful in order to better visualize the effect of the backlash.

- `endPoint`: this field specifies which [Solid](../reference/solid.md) is subject to the joint constraints. It must be either a [Solid](../reference/solid.md) child, or a reference to an existing [Solid](../reference/solid.md), i.e. a [SolidReference](../reference/solidreference.md).
Alternatively, a [Slot](../reference/slot.md) node can be inserted in the `endPoint` field, but this [Slot](../reference/slot.md) should be connected to another [Slot](../reference/slot.md) whose `endPoint` is either a [Solid](../reference/solid.md) or a [SolidReference](../reference/solidreference.md).

> **Note**: **Important**, due to the way the backlash is modeled, it is **necessary** for the `endPoint` solid to have physics **enabled** for the backlash effect to be active.
If the physics node isn't activated, the simulation will work without giving any error but the resulting behavior will not model any backlash as it will be simulated in kinematic mode and the contact between the two hidden joints will be ignored.
