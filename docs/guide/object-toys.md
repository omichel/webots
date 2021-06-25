# Toys

## PaperBoat

A paper boat (42.8 x 14.2 x 10.6 cm).

%figure

![PaperBoat](images/objects/toys/PaperBoat/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
PaperBoat {
  SFVec3f    translation         0 0 0
  SFRotation rotation            0 1 0 0
  SFString   name                "paper boat"
  SFFloat    scale               1
  MFString   textureUrl          "PaperBoat/textures/newspaper.jpg"
  SFFloat    mass                0.25
  MFNode     immersionProperties [
      ImmersionProperties {
        fluidName "water"
        dragForceCoefficients 0.05 0 0
        viscousResistanceForceCoefficient 200
      }
    ]
  MFColor    recognitionColors   []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/toys/protos/PaperBoat.proto]({{ url.github_tree }}/projects/objects/toys/protos/PaperBoat.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### PaperBoat Field Summary

- `scale`: Defines the global scaling factor.

- `textureUrl`: Defines the texture used for the paper boat.

- `mass`: Defines the mass of the paper boat in kg.

## RubberDuck

A rubber yellow duck (7.42 x 10.66 x 10.13 cm).

%figure

![RubberDuck](images/objects/toys/RubberDuck/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
RubberDuck {
  SFVec3f    translation         0 0 0
  SFRotation rotation            0 1 0 0
  SFString   name                "rubber duck"
  SFFloat    scale               1
  SFColor    bodyColor           0.87451 0.756863 0.113725
  SFFloat    density             400
  MFNode     immersionProperties [
      ImmersionProperties {
        fluidName "water"
        viscousResistanceForceCoefficient 400
        viscousResistanceTorqueCoefficient 0.5
      }
    ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/toys/protos/RubberDuck.proto]({{ url.github_tree }}/projects/objects/toys/protos/RubberDuck.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### RubberDuck Field Summary

- `scale`: Defines the global scaling factor.

- `bodyColor`: Defines the color of the rubber duck.

- `density`: Defines density of the rubber duck in kg / m^3.

