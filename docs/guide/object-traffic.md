# Traffic

## CautionPanel

Traffic panel: Caution panel.

%figure

![CautionPanel](images/objects/traffic/CautionPanel/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
CautionPanel {
  SFVec3f    translation        0 0 0
  SFRotation rotation           0 1 0 0
  SFString   name               "caution panel"
  MFString   signImage          "textures/signs/us/traffic_signals_ahead.jpg"
  SFColor    color              0.8 0.8 0.8
  MFColor    recognitionColors  [ 1 0.82 0.2, 0.8 0.8 0.8 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/CautionPanel.proto]({{ url.github_tree }}/projects/objects/traffic/protos/CautionPanel.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### CautionPanel Field Summary

- `signImage`: Defines the texture used for the sign.

- `color`: Defines the color of the panel.

## CautionSign

Traffic sign: Caution sign.

%figure

![CautionSign](images/objects/traffic/CautionSign/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
CautionSign {
  SFVec3f    translation  0 0 0
  SFRotation rotation     0 1 0 0
  SFString   name         "caution sign"
  SFFloat    height       2
  SFFloat    radius       0.03
  SFColor    color        0.576471 0.576471 0.576471
  MFNode     signBoards   [ CautionPanel { translation 0 -0.17 0 } ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/CautionSign.proto]({{ url.github_tree }}/projects/objects/traffic/protos/CautionSign.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### CautionSign Field Summary

- `height`: Defines the height of the sign.

- `radius`: Defines the radius of the sign pole.

- `color`: Defines the color of the sign.

- `signBoards`: Defines the boards.

## ControlledStreetLight

Simple model of a controlled street light including a customizable SpotLight.

%figure

![ControlledStreetLight](images/objects/traffic/ControlledStreetLight/model.thumbnail.png)

%end

Derived from [Robot](../reference/robot.md).

```
ControlledStreetLight {
  SFVec3f    translation  0 0 0
  SFRotation rotation     0 1 0 0
  SFString   name         "street light"
  SFString   controller   "defective_street_light"
  SFBool     supervisor   FALSE
  SFFloat    beamWidth    1.1
  MFColor    color        [ 1 0.9 0.8 ]
  SFFloat    cutOffAngle  1.4
  SFVec3f    direction    0 -1 -0.1
  SFFloat    radius       1000
  SFBool     castShadows  FALSE
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/ControlledStreetLight.proto]({{ url.github_tree }}/projects/objects/traffic/protos/ControlledStreetLight.proto)"

> **License**: Creative Commons Attribution 3.0 United States License (original model by Andrew Kator & Jennifer Legaz).
[More information.](https://creativecommons.org/licenses/by/3.0/legalcode)

### ControlledStreetLight Field Summary

- `controller`: Defines the controller used to make the [LED](../reference/led.md) blink.

- `beamWidth`: Defines the beam width of the spot light.

- `color`: Defines the color of the spot light.

- `cutOffAngle`: Defines the cut-off angle of the spot light.

- `direction`: Defines the direction of the spot light.

- `radius`: Defines the radius of the spot light.

- `castShadows`: Defines whether the spot light casts shadows.

## CrossRoadsTrafficLight

Four roads crossing traffic light.

%figure

![CrossRoadsTrafficLight](images/objects/traffic/CrossRoadsTrafficLight/model.thumbnail.png)

%end

Derived from [Robot](../reference/robot.md).

```
CrossRoadsTrafficLight {
  SFVec3f    translation 0 0 0
  SFRotation rotation    0 1 0 0
  SFString   name        "cross road traffic light"
  SFString   controller  "crossroads_traffic_lights"
  SFBool     supervisor  FALSE
  SFVec2f    size        20.8 20.8
  MFColor    recognitionColors []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/CrossRoadsTrafficLight.proto]({{ url.github_tree }}/projects/objects/traffic/protos/CrossRoadsTrafficLight.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### CrossRoadsTrafficLight Field Summary

- `size`: Defines the size of the traffic lights.

## DirectionPanel

Resizable direction panel with two customizable sides.
The 'textSize' fields defines the height (in meter) of one line of text.
List of supported fonts:
- 'Arial'
- 'Arial Black'
- 'Comic Sans MS'
- 'Courier New'
- 'Georgia',
- 'Impact'
- 'Lucida Console'
- 'Lucida Sans Unicode'
- 'Palatino Linotype'
- 'Tahoma'
- 'Times New Roman'
- 'Trebuchet MS'
- 'Verdana'
Alternatively it is possible to add other *.ttf files in the PROJECT\_HOME/fonts directory.

%figure

![DirectionPanel](images/objects/traffic/DirectionPanel/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
DirectionPanel {
  SFVec3f     translation      0 0 0
  SFRotation  rotation         0 1 0 0
  SFString    name             "direction panel"
  SFFloat     width            1.3
  SFFloat     height           0.4
  SFFloat     thickness        0.04
  SFColor     backgroundColor  0.8 0.8 0.8
  SFColor     textColor        0 0 0
  SFFloat     textSize         0.1
  SFString    font             "Arial"
  SFString    text             "INPUT YOUR\nTEXT HERE"
  SFBool      arrow            TRUE
  SFBool      right            TRUE
  SFBool      left             FALSE
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/DirectionPanel.proto]({{ url.github_tree }}/projects/objects/traffic/protos/DirectionPanel.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### DirectionPanel Field Summary

- `width`: Defines the width of the panel.

- `height`: Defines the height of the panel.

- `thickness`: Defines the thickness of the panel.

- `backgroundColor`: Defines the background color of the panel.

- `textColor`: Defines the color of the text displayed on the panel.

- `textSize`: Defines the size of the text displayed on the panel.

- `font`: Defines the font used by the text displayed on the panel.

- `text`: Defines the text displayed on the panel.

- `arrow`: Defines whether the panel should display an arraw.

- `right`: Defines whether the pannel should draw text on the side pointing toward the right.

- `left`: Defines whether the pannel should draw text on the side pointing toward the left.

## DivergentIndicator

A divergent indicator block.

%figure

![DivergentIndicator](images/objects/traffic/DivergentIndicator/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
DivergentIndicator {
  SFVec3f    translation     0 0 0
  SFRotation rotation        0 1 0 0
  SFString   name            "divergent indicator"
  MFString   texture         "textures/divergent_indicator.jpg"
  SFFloat    height          1
  SFFloat    radius          0.5
  SFInt32    subdivision     24
  MFColor    recognitionColors []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/DivergentIndicator.proto]({{ url.github_tree }}/projects/objects/traffic/protos/DivergentIndicator.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### DivergentIndicator Field Summary

- `texture`: Defines the texture used by the divergent indicator.

- `height`: Defines the height of the divergent indicator.

- `radius`: Defines the radius of the divergent indicator.

- `subdivision`: Defines the number of polygons used to represent the divergent indicator cylindrical part and so its resolution .

## ExitPanel

Traffic panel: Exit panel.

%figure

![ExitPanel](images/objects/traffic/ExitPanel/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
ExitPanel {
  SFVec3f    translation        0 0 0
  SFRotation rotation           0 1 0 0
  SFString   name               "exit panel"
  MFString   signImage          "textures/signs/exit.jpg"
  SFColor    color              0.8 0.8 0.8
  MFColor    recognitionColors  [ 0 0.4 0.3, 0.8 0.8 0.8 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/ExitPanel.proto]({{ url.github_tree }}/projects/objects/traffic/protos/ExitPanel.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### ExitPanel Field Summary

- `signImage`: Defines the texture used for the sign.

- `color`: Defines the color of the panel.

## ExitSign

Traffic sign: Exit sign.

%figure

![ExitSign](images/objects/traffic/ExitSign/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
ExitSign {
  SFVec3f    translation  0 0 0
  SFRotation rotation     0 1 0 0
  SFString   name         "exit sign"
  SFFloat    height       2
  SFFloat    radius       0.03
  SFColor    color        0.576471 0.576471 0.576471
  MFNode     signBoards   [ ExitPanel { translation 0 -0.051 0 } ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/ExitSign.proto]({{ url.github_tree }}/projects/objects/traffic/protos/ExitSign.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### ExitSign Field Summary

- `height`: Defines the height of the sign.

- `radius`: Defines the radius of the sign pole.

- `color`: Defines the color of the panel.

- `signBoards`: Defines the boards.

## GenericTrafficLight

A generic traffic light with configurable timming.

%figure

![GenericTrafficLight](images/objects/traffic/GenericTrafficLight/model.thumbnail.png)

%end

Derived from [Robot](../reference/robot.md).

```
GenericTrafficLight {
  SFVec3f    translation 0 0 0
  SFRotation rotation    0 1 0 0
  SFString   name        "generic traffic light"
  SFBool     startGreen  TRUE
  SFFloat    greenTime   60
  SFFloat    redTime     15
  SFString   state       "off"
  MFColor    recognitionColors []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/GenericTrafficLight.proto]({{ url.github_tree }}/projects/objects/traffic/protos/GenericTrafficLight.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### GenericTrafficLight Field Summary

- `startGreen`: Defines whether the traffic light should start in a 'green' state or 'red' state.

- `greenTime`: Defines how long should be the green state in seconds.

- `redTime`: Defines how long should be the red state in seconds.

- `state`: Defines the current state of the traffic light, this field is automatically updated when the state change.

## HighwayPole

Customizable highway pole with the possibility of adding another stand and more signs along the vertical or horizontal parts of the pole.
Different types of poles are available: "cylinder", "box", or "H-shape".

%figure

![HighwayPole](images/objects/traffic/HighwayPole/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
HighwayPole {
  SFVec3f     translation           0 0 0
  SFRotation  rotation              0 1 0 0
  SFString    name                  "highway pole"
  SFString    type                  "cylinder"
  SFInt32     stand                 1
  SFFloat     height                6
  SFFloat     length                8
  SFFloat     thickness             0.2
  SFColor     color                 0.258824 0.258824 0.258824
  SFFloat     curveRadius           0.4
  MFNode      rightHorizontalSigns  [ HighwaySign { name "vertical sign" } ]
  MFNode      rightVerticalSigns    [ HighwaySign { name "horizontal sign" height 2.1 length 3.2 texture "textures/highway_sign_la_ciotat.jpg" } ]
  MFNode      leftHorizontalSigns   [ ]
  MFNode      leftVerticalSigns     [ ]
  MFColor     recognitionColors     []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/HighwayPole.proto]({{ url.github_tree }}/projects/objects/traffic/protos/HighwayPole.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### HighwayPole Field Summary

- `type`: Defines the shape of the pole. This field accepts the following values: `"cylinder"`, `"box"`, and `"H-shape"`.

- `stand`: Defines the number of stands. This field accepts the following values: `1` and `2`.

- `height`: Defines the height of the pole.

- `length`: Defines the length of the pole.

- `thickness`: Defines the thickness of the pole.

- `color`: Defines the color of the pole.

- `curveRadius`: Defines the curvature radius of the pole.

- `rightHorizontalSigns`: Defines the horizontal signs on the right side of the pole.

- `rightVerticalSigns`: Defines the vertical signs on the right side of the pole.

- `leftHorizontalSigns`: Defines the horizontal signs on the left side of the pole.

- `leftVerticalSigns`: Defines the vertical signs on the left side of the pole.

## HighwaySign

Resizable sign with front texture option.

%figure

![HighwaySign](images/objects/traffic/HighwaySign/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
HighwaySign {
  SFVec3f     translation        0 0 0
  SFRotation  rotation           0 1 0 0
  SFString    name               "highway sign"
  SFFloat     height             3
  SFFloat     length             4.5
  SFFloat     thickness          0.2
  SFColor     color              0.258824 0.258824 0.258824
  MFString    texture            "textures/highway_sign_bordeaux.jpg"
  MFColor     recognitionColors  [ 0.08 0.22 0.75, 0.26 0.26 0.26 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/HighwaySign.proto]({{ url.github_tree }}/projects/objects/traffic/protos/HighwaySign.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### HighwaySign Field Summary

- `height`: Defines the height of the sign.

- `length`: Defines the lenght of the sign.

- `thickness`: Defines the thickness of the sign.

- `color`: Defines the color of the sign.

- `texture`: Defines the texture used for the sign.

## OrderPanel

Traffic panel: Order panel.

%figure

![OrderPanel](images/objects/traffic/OrderPanel/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
OrderPanel {
  SFVec3f    translation        0 0 0
  SFRotation rotation           0 1 0 0
  SFString   name               "order panel"
  MFString   signImage          "textures/signs/do_not_enter.jpg"
  SFColor    color              0.8 0.8 0.8
  MFColor    recognitionColors  [ 0.75 0.17 0.22, 0.8 0.8 0.8 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/OrderPanel.proto]({{ url.github_tree }}/projects/objects/traffic/protos/OrderPanel.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### OrderPanel Field Summary

- `signImage`: Defines the texture used for the sign.

- `color`: Defines the color of the panel.

## OrderSign

Traffic sign: Order sign.

%figure

![OrderSign](images/objects/traffic/OrderSign/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
OrderSign {
  SFVec3f    translation  0 0 0
  SFRotation rotation     0 1 0 0
  SFString   name         "order sign"
  SFFloat    height       2
  SFFloat    radius       0.03
  SFColor    color        0.576471 0.576471 0.576471
  MFNode     signBoards   [ OrderPanel { translation 0 -0.175 -0.026 } ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/OrderSign.proto]({{ url.github_tree }}/projects/objects/traffic/protos/OrderSign.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### OrderSign Field Summary

- `height`: Defines the height of the sign.

- `radius`: Defines the radius of the sign pole.

- `color`: Defines the color of the sign.

- `signBoards`: Defines the boards.

## ParkingLines

Parking lines for several consecutive cars.

%figure

![ParkingLines](images/objects/traffic/ParkingLines/model.thumbnail.png)

%end

Derived from [Transform](../reference/transform.md).

```
ParkingLines {
  SFVec3f    translation      0 0 0
  SFRotation rotation         0 1 0 0
  SFInt32    numberOfCarParks 5
  SFFloat    carParkLength    4.8
  SFFloat    carParkWidth     2.4
  MFString   texture          "textures/parking_lines.png"
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/ParkingLines.proto]({{ url.github_tree }}/projects/objects/traffic/protos/ParkingLines.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### ParkingLines Field Summary

- `numberOfCarParks`: Defines the number of parks.

- `carParkLength`: Defines the length of one park.

- `carParkWidth`: Defines the width of one park.

- `texture`: Defines the texture used for the lines.

## ParkingMeter

A parking meter.

%figure

![ParkingMeter](images/objects/traffic/ParkingMeter/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
ParkingMeter {
  SFVec3f    translation 0 0 0
  SFRotation rotation    0 1 0 0
  SFString   name        "parking meter"
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/ParkingMeter.proto]({{ url.github_tree }}/projects/objects/traffic/protos/ParkingMeter.proto)"

> **License**: Creative Commons Attribution 4.0 International License.
[More information.](https://creativecommons.org/licenses/by/4.0/legalcode)

## PedestrianCrossing

A pedestrian crossing 20 x 8 meters (0.1m thick).

%figure

![PedestrianCrossing](images/objects/traffic/PedestrianCrossing/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
PedestrianCrossing {
  SFVec3f    translation          0 0 0
  SFRotation rotation             0 1 0 0
  SFString   name                 "pedestrian crossing"
  SFVec2f    size                 20 8
  SFInt32    textureFiltering     4
  SFBool     enableBoundingObject TRUE
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/PedestrianCrossing.proto]({{ url.github_tree }}/projects/objects/traffic/protos/PedestrianCrossing.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### PedestrianCrossing Field Summary

- `size`: Defines the size of the pedestrian crossing.

- `textureFiltering`: Defines the filtering level for the texture used for the pedestrian crossing.

- `enableBoundingObject`: Defines whether the pedestrian crossing should have a bounding object.

## Pole

A metallic pole for the traffic lights.

%figure

![Pole](images/objects/traffic/Pole/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
Pole {
  SFVec3f    translation 0 0 0
  SFRotation rotation    0 1 0 0
  SFString   name        "pole"
  MFNode     slot        []
  MFColor    recognitionColors []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/Pole.proto]({{ url.github_tree }}/projects/objects/traffic/protos/Pole.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### Pole Field Summary

- `slot`: Extends the pole with panels or traffic lights.

## RectangularPanel

A simple rectangular traffic pannel.

%figure

![RectangularPanel](images/objects/traffic/RectangularPanel/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
RectangularPanel {
  SFVec3f     translation        0 0 0
  SFRotation  rotation           0 1 0 0
  SFString    name               "rectangular panel"
  MFString    signImage          "textures/signs/eu/dead_end.jpg"
  SFColor     color              0.8 0.8 0.8
  SFVec2f     size               0.5 0.5
  MFColor     recognitionColors  [ 0 0.5 0.76, 0.8 0.8 0.8 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/RectangularPanel.proto]({{ url.github_tree }}/projects/objects/traffic/protos/RectangularPanel.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### RectangularPanel Field Summary

- `signImage`: Defines the texture used for the sign.

- `color`: Defines the color of the panel.

- `size`: Defines the size of the panel.

## SignPole

Customizable direction panel on pole with the possibility of adding more panels.

%figure

![SignPole](images/objects/traffic/SignPole/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
SignPole {
  SFVec3f     translation   0 0 0
  SFRotation  rotation      0 1 0 0
  SFString    name          "sign pole"
  SFFloat     height        2.2
  SFFloat     radius        0.02
  SFColor     color         0.258824 0.258824 0.258824
  MFNode      signBoards    [ DirectionPanel {} ]
  MFColor     recognitionColors []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/SignPole.proto]({{ url.github_tree }}/projects/objects/traffic/protos/SignPole.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### SignPole Field Summary

- `height`: Defines the height of the sign.

- `radius`: Defines the radius of the sign.

- `color`: Defines the color of the pole.

- `signBoards`: Defines the boards.

## SpeedLimitPanel

Traffic panel: Speed limit panel (5mph to 80mph) or one way panel.

%figure

![SpeedLimitPanel](images/objects/traffic/SpeedLimitPanel/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
SpeedLimitPanel {
  SFVec3f     translation        0 0 0
  SFRotation  rotation           0 1 0 0
  SFString    name               "speed limit panel"
  MFString    signImage          "textures/signs/us/speed_limit_45.jpg"
  SFColor     color              0.8 0.8 0.8
  MFColor     recognitionColors  [ 1 1 1, 0.8 0.8 0.8 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/SpeedLimitPanel.proto]({{ url.github_tree }}/projects/objects/traffic/protos/SpeedLimitPanel.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### SpeedLimitPanel Field Summary

- `signImage`: Defines the texture used for the sign.

- `color`: Defines the color of the panel.

## SpeedLimitSign

Traffic sign: Speed limit sign (5mph to 80mph) or one way sign.

%figure

![SpeedLimitSign](images/objects/traffic/SpeedLimitSign/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
SpeedLimitSign {
  SFVec3f    translation  0 0 0
  SFRotation rotation     0 1 0 0
  SFString   name         "speed limit"
  SFFloat    height       2
  SFFloat    radius       0.03
  SFColor    color        0.576471 0.576471 0.576471
  MFNode     signBoards   [ SpeedLimitPanel { translation 0 0 -0.023 } ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/SpeedLimitSign.proto]({{ url.github_tree }}/projects/objects/traffic/protos/SpeedLimitSign.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### SpeedLimitSign Field Summary

- `height`: Defines the height of the sign.

- `radius`: Defines the radius of the sign pole.

- `color`: Defines the color of the sign.

- `signBoards`: Defines the boards.

## StopPanel

Traffic panel: Stop panel.

%figure

![StopPanel](images/objects/traffic/StopPanel/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
StopPanel {
  SFVec3f    translation        0 0 0
  SFRotation rotation           0 1 0 0
  SFString   name               "stop panel"
  MFString   signImage          "textures/signs/stop.jpg"
  SFColor    color              0.576471 0.576471 0.576471
  MFColor    recognitionColors  [ 0.75 0.25 0.12, 0.58 0.58 0.58 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/StopPanel.proto]({{ url.github_tree }}/projects/objects/traffic/protos/StopPanel.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### StopPanel Field Summary

- `signImage`: Defines the texture used for the sign.

- `color`: Defines the color of the panel.

## StopSign

Traffic sign: Stop sign.

%figure

![StopSign](images/objects/traffic/StopSign/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
StopSign {
  SFVec3f    translation  0 0 0
  SFRotation rotation     0 1 0 0
  SFString   name         "stop sign"
  SFFloat    height       2
  SFFloat    radius       0.03
  SFColor    color        0.576471 0.576471 0.576471
  MFNode     signBoards   [ StopPanel { translation 0 -0.097 0 } ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/StopSign.proto]({{ url.github_tree }}/projects/objects/traffic/protos/StopSign.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### StopSign Field Summary

- `height`: Defines the height of the sign.

- `radius`: Defines the radius of the sign pole.

- `color`: Defines the color of the sign.

- `signBoards`: Defines the boards.

## StreetLight

Simple model of a street light including a customizable SpotLight.
This model was sponsored by the CTI project RO2IVSim ([http://transport.epfl.ch/simulator-for-mobile-robots-and-intelligent-vehicles](http://transport.epfl.ch/simulator-for-mobile-robots-and-intelligent-vehicles)).

%figure

![StreetLight](images/objects/traffic/StreetLight/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
StreetLight {
  SFVec3f    translation   0 0 0
  SFRotation rotation      0 1 0 0
  SFString   name          "street light"
  SFFloat    beamWidth     1.1
  SFColor    color         1 1 1
  SFFloat    cutOffAngle   1.4
  SFVec3f    direction     0 -1 -0.1
  SFBool     on            TRUE
  SFFloat    radius        1000
  SFFloat    intensity     30
  SFBool     castShadows   FALSE
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/StreetLight.proto]({{ url.github_tree }}/projects/objects/traffic/protos/StreetLight.proto)"

> **License**: Creative Commons Attribution 3.0 United States License (original model by Andrew Kator & Jennifer Legaz).
[More information.](https://creativecommons.org/licenses/by/3.0/legalcode)

### StreetLight Field Summary

- `beamWidth`: Defines the beam width of the spot light.

- `color`: Defines the color of the spot light.

- `cutOffAngle`: Defines the cut-off angle of the spot light.

- `direction`: Defines the direction of the spot light.

- `on`: Defines whether the spot light is on or off.

- `radius`: Defines the radius of the spot light.

- `intensity`: Defines the radius of the spot light.

- `castShadows`: Defines whether the spot light casts shadows.

## TrafficCone

Traffic cone.

%figure

![TrafficCone](images/objects/traffic/TrafficCone/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
TrafficCone {
  SFVec3f    translation 0 0 0
  SFRotation rotation    0 1 0 0
  SFString   name        "traffic cone"
  SFNode     physics     NULL
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/TrafficCone.proto]({{ url.github_tree }}/projects/objects/traffic/protos/TrafficCone.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

## TrafficLight

Basic traffic light without lamp.

%figure

![TrafficLight](images/objects/traffic/TrafficLight/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
TrafficLight {
  SFVec3f    translation        0 0 0
  SFRotation rotation           0 1 0 0
  SFString   name               "traffic light"
  SFString   red_light          "red light"
  SFString   orange_light       "orange light"
  SFString   green_light        "green light"
  SFNode     lamp_geometry      NULL
  SFRotation lamp_rotation      0 0 1 0
  MFColor    recognitionColors  [ 0.25 0.25 0.25, 0 0 0 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/TrafficLight.proto]({{ url.github_tree }}/projects/objects/traffic/protos/TrafficLight.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### TrafficLight Field Summary

- `red_light`: Defines the name of the red [LED](../reference/led.md) device.

- `orange_light`: Defines the name of the orange [LED](../reference/led.md) device.

- `green_light`: Defines the name of the green [LED](../reference/led.md) device.

- `lamp_geometry`: Defines the geometry of the lamps.

- `lamp_rotation`: Defines the rotation of the lamps.

## TrafficLightArrowLampGeometry

A traffic light lamp with an adjustable arrow.

%figure

![TrafficLightArrowLampGeometry](images/objects/traffic/TrafficLightArrowLampGeometry/model.thumbnail.png)

%end

Derived from [IndexedFaceSet](../reference/indexedfaceset.md).

```
TrafficLightArrowLampGeometry {
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/TrafficLightArrowLampGeometry.proto]({{ url.github_tree }}/projects/objects/traffic/protos/TrafficLightArrowLampGeometry.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

## TrafficLightBigPole

A big metallic pole above the road for traffic lights.

%figure

![TrafficLightBigPole](images/objects/traffic/TrafficLightBigPole/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
TrafficLightBigPole {
  SFVec3f    translation 0 0 0
  SFRotation rotation    0 1 0 0
  SFString   name        "traffic light big pole"
  MFNode     slot1       []
  MFNode     slot2       []
  MFNode     slot3       []
  MFColor    recognitionColors []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/TrafficLightBigPole.proto]({{ url.github_tree }}/projects/objects/traffic/protos/TrafficLightBigPole.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### TrafficLightBigPole Field Summary

- `slot1`: Extends the pole with a traffic light located in the middle of the vertical part of the pole.

- `slot2`: Extends the pole with a traffic light located in the begining of the horizontal part of the pole.

- `slot3`: Extends the pole with a traffic light located in the end of the horizontal part of the pole.

## TrafficLightHorizontal

Horizontal traffic light without lamp to put on the big pole above the road.

%figure

![TrafficLightHorizontal](images/objects/traffic/TrafficLightHorizontal/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
TrafficLightHorizontal {
  SFVec3f    translation        0 0 0
  SFRotation rotation           0 1 0 0
  SFString   name               "horizontal traffic light"
  SFString   red_light          "red light"
  SFString   orange_light       "orange light"
  SFString   green_light        "green light"
  SFNode     lamp_geometry      NULL
  SFRotation lamp_rotation      0 0 1 0
  MFColor    recognitionColors  [ 0.25 0.25 0.25, 0 0 0 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/TrafficLightHorizontal.proto]({{ url.github_tree }}/projects/objects/traffic/protos/TrafficLightHorizontal.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### TrafficLightHorizontal Field Summary

- `red_light`: Defines the name of the red [LED](../reference/led.md) device.

- `orange_light`: Defines the name of the orange [LED](../reference/led.md) device.

- `green_light`: Defines the name of the green [LED](../reference/led.md) device.

- `lamp_geometry`: Defines the geometry of the lamps.

- `lamp_rotation`: Defines the rotation of the lamps.

## TrafficLightStandardLampGeometry

A traffic light lamp with a standard geometry.

%figure

![TrafficLightStandardLampGeometry](images/objects/traffic/TrafficLightStandardLampGeometry/model.thumbnail.png)

%end

Derived from [Sphere](../reference/sphere.md).

```
TrafficLightStandardLampGeometry {
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/TrafficLightStandardLampGeometry.proto]({{ url.github_tree }}/projects/objects/traffic/protos/TrafficLightStandardLampGeometry.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

## WorkBarrier

A work barrier with optional physics.

%figure

![WorkBarrier](images/objects/traffic/WorkBarrier/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
WorkBarrier {
  SFVec3f    translation   0 0 0
  SFRotation rotation      0 1 0 0
  SFString   name          "work barrier"
  SFBool     enablePhysics FALSE
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/WorkBarrier.proto]({{ url.github_tree }}/projects/objects/traffic/protos/WorkBarrier.proto)"

> **License**: Creative Commons Attribution 4.0 International License.
[More information.](https://creativecommons.org/licenses/by/4.0/legalcode)

### WorkBarrier Field Summary

- `enablePhysics`: Defines whether the barrier should have physics.

## YieldPanel

Traffic panel: Yield panel.

%figure

![YieldPanel](images/objects/traffic/YieldPanel/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
YieldPanel {
  SFVec3f     translation        0 0 0
  SFRotation  rotation           0 1 0 0
  SFString    name               "yield panel"
  MFString    signImage          "textures/signs/us/yield.jpg"
  SFColor     color              0.8 0.8 0.8
  SFFloat     textureRotation    0
  MFColor     recognitionColors  [ 0.7 0.12 0.18, 0.8 0.8 0.8 ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/YieldPanel.proto]({{ url.github_tree }}/projects/objects/traffic/protos/YieldPanel.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### YieldPanel Field Summary

- `signImage`: Defines the texture used for the sign.

- `color`: Defines the color of the panel.

- `textureRotation`: Defines the rotation of the texture used for the sign.

## YieldSign

Traffic sign: Yield sign.

%figure

![YieldSign](images/objects/traffic/YieldSign/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
YieldSign {
  SFVec3f    translation  0 0 0
  SFRotation rotation     0 1 0 0
  SFString   name         "yield sign"
  SFFloat    height       2
  SFFloat    radius       0.03
  SFColor    color        0.576471 0.576471 0.576471
  MFNode     signBoards   [ YieldPanel { } ]
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/traffic/protos/YieldSign.proto]({{ url.github_tree }}/projects/objects/traffic/protos/YieldSign.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### YieldSign Field Summary

- `height`: Defines the height of the sign.

- `radius`: Defines the radius of the sign pole.

- `color`: Defines the color of the sign.

- `signBoards`: Defines the boards.

