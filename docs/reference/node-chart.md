# Node Chart

The Webots Node Chart outlines all the nodes available to build Webots worlds.

## Inheritance

In the chart, an arrow between two nodes represents an inheritance relationship.
The inheritance relationship indicates that a derived node (at the arrow head) inherits all the fields and API functions of a base node (at the arrow tail).
For example, the [Solid](solid.md) node inherits from the [Transform](transform.md) node, and therefore all the fields and functions available in the [Transform](transform.md) node are also available in the [Solid](solid.md) node.

## Abstract Nodes

Boxes depicted with a dashed line (like [Light](light.md), [Device](device.md) or `Geometry`) represent *abstract* nodes, that is, nodes that cannot be instantiated (either using the SceneTree or in a ".wbt" file).
Abstract nodes are used to group common fields and functions that are shared by derived nodes.

## Bounding Objects

A box with a green background indicates a node that can be used directly (or composed using [Group](group.md) and [Transform](transform.md) nodes) to build a *boundingObject* used to detect collisions between [Solid](solid.md) objects.
Note that not all geometry nodes can be used as boundingObjects, and that although [Group](group.md) and [Transform](transform.md) can be used, not every combination of these will work correctly.

## Insertion Rules

In general, a [Device](device.md) node should lie within the `children` hierarchy of a [Robot](robot.md) node.
There are some exceptions though, like the [Connector](connector.md) node which can be inserted outside of a [Robot](robot.md) node.
Please refer to the [Nodes and API functions](nodes-and-api-functions.md) section for more details.

%figure "Webots Nodes Chart"
%chart
graph LR
  subgraph ""
    AbstractClass(Abstract Class) --- AbstractClassDefinition[cannot be instantiated]
    BoundingObject[Bounding Object] --- BoundingObjectDefinition[can be used as BoundingObject<br>in a Solid node]
    VRML97[VRML97] --- VRML97Definition[VRML97 node]
  end
  PositionSensor --- Appearance
  AbstractClassDefinition --- JointDevice
  linkStyle 0 stroke-width:0px;
  linkStyle 1 stroke-width:0px;
  linkStyle 2 stroke-width:0px;
  linkStyle 3 stroke-width:0px;
  linkStyle 4 stroke-width:0px;

  Geometry(Geometry) -.-> Box[[Box](box.md)]
  Geometry -.-> Capsule[[Capsule](capsule.md)]
  Geometry -.-> Cone[[Cone](cone.md)]
  Geometry -.-> Cylinder[[Cylinder](cylinder.md)]
  Geometry -.-> ElevationGrid[[ElevationGrid](elevationgrid.md)]
  Geometry -.-> IndexedFaceSet[[IndexedFaceSet](indexedfaceset.md)]
  Geometry -.-> IndexedLineSet[[IndexedLineSet](indexedlineset.md)]
  Geometry -.-> Mesh[[Mesh](mesh.md)]
  Geometry -.-> Plane[[Plane](plane.md)]
  Geometry -.-> PointSet[[PointSet](pointset.md)]
  Geometry -.-> Sphere[[Sphere](sphere.md)]

  Device([Device](device.md)) -.-> JointDevice(Joint Device)
  Device -.-> SolidDevice(Solid Device)

  Group[[Group](group.md)] --> Transform[[Transform](transform.md)]
  Group --> Billboard[[Billboard](billboard.md)]
    Transform --> Solid[[Solid](solid.md)]
      Solid --> SolidDevice
        SolidDevice --> Accelerometer[[Accelerometer](accelerometer.md)]
        SolidDevice --> Camera[[Camera](camera.md)]
        SolidDevice --> Compass[[Compass](compass.md)]
        SolidDevice --> Connector[[Connector](connector.md)]
        SolidDevice --> Display[[Display](display.md)]
        SolidDevice --> DistanceSensor[[DistanceSensor](distancesensor.md)]
        SolidDevice --> Emitter[[Emitter](emitter.md)]
        SolidDevice --> GPS[[GPS](gps.md)]
        SolidDevice --> Gyro[[Gyro](gyro.md)]
        SolidDevice --> InertialUnit[[InertialUnit](inertialunit.md)]
        SolidDevice --> LED[[LED](led.md)]
        SolidDevice --> Lidar[[Lidar](lidar.md)]
        SolidDevice --> LightSensor[[LightSensor](lightsensor.md)]
        SolidDevice --> Pen[[Pen](pen.md)]
        SolidDevice --> Propeller[[Propeller](propeller.md)]
        SolidDevice --> Radar[[Radar](radar.md)]
        SolidDevice --> RangeFinder[[RangeFinder](rangefinder.md)]
        SolidDevice --> Receiver[[Receiver](receiver.md)]
        SolidDevice --> Speaker[[Speaker](speaker.md)]
        SolidDevice --> TouchSensor[[TouchSensor](touchsensor.md)]
      Solid --> Track[[Track](track.md)]
      Solid --> Charger[[Charger](charger.md)]
      Solid --> Robot[[Robot](robot.md)]
    Transform --> Fluid[[Fluid](fluid.md)]
    Transform --> TrackWheel[[TrackWheel](trackwheel.md)]

  JointDevice -.-> Motor([Motor](motor.md))
    Motor -.-> LinearMotor[[LinearMotor](linearmotor.md)]
    Motor -.-> RotationalMotor[[RotationalMotor](rotationalmotor.md)]
  JointDevice -.-> Brake[[Brake](brake.md)]
  JointDevice -.-> PositionSensor[[PositionSensor](positionsensor.md)]

  Light([Light](light.md)) -.-> DirectionalLight[[DirectionalLight](directionallight.md)]
  Light -.-> PointLight[[PointLight](pointlight.md)]
  Light -.-> SpotLight[[SpotLight](spotlight.md)]

  Joint([Joint](joint.md)) -.-> HingeJoint[[HingeJoint](hingejoint.md)]
    HingeJoint --> Hinge2Joint[[Hinge2Joint](hinge2joint.md)]
      Hinge2Joint --> BallJoint[[BallJoint](balljoint.md)]
  Joint -.-> SliderJoint[[SliderJoint](sliderjoint.md)]

  JointParameters[[JointParameters](jointparameters.md)] --> HingeJointParameters[[HingeJointParameters](hingejointparameters.md)]
  JointParameters --> BallJointParameters[[BallJointParameters](balljointparameters.md)]

  subgraph other Nodes
    Appearance[[Appearance](appearance.md)]
    Background[[Background](background.md)]
    Color[[Color](color.md)]
    ContactProperties[[ContactProperties](contactproperties.md)]
    Coordinate[[Coordinate](coordinate.md)]
    Damping[[Damping](damping.md)]
    Focus[[Focus](focus.md)]
    Fog[[Fog](fog.md)]
    ImageTexture[[ImageTexture](imagetexture.md)]
    ImmersionProperties[[ImmersionProperties](immersionproperties.md)]
    Lens[[Lens](lens.md)]
    LensFlare[[LensFlare](lensflare.md)]
    Material[[Material](material.md)]
    Muscle[[Muscle](muscle.md)]
    Normal[[Normal](normal.md)]
    PBRAppearance[[PBRAppearance](pbrappearance.md)]
    Physics[[Physics](physics.md)]
    Recognition[[Recognition](recognition.md)]
    Shape[[Shape](shape.md)]
    Slot[[Slot](slot.md)]
    SolidReference[[SolidReference](solidreference.md)]
    TextureCoordinate[[TextureCoordinate](texturecoordinate.md)]
    TextureTransform[[TextureTransform](texturetransform.md)]
    Viewpoint[[Viewpoint](viewpoint.md)]
    WorldInfo[[WorldInfo](worldinfo.md)]
    Zoom[[Zoom](zoom.md)]
  end

  classDef AbstractClassStyle stroke-width:3px,stroke-dasharray:5,5;
  classDef DefinitionStyle fill:#ddd,stroke-width:0px;
  style VRML97 fill:#ddd,stroke:#444444,stroke-width:3px;

  class AbstractClass,Device,Geometry,Joint,JointDevice,Light,Motor,SolidDevice AbstractClassStyle;
  class BoundingObject,Capsule,Mesh,Plane secondaryNode;
  class Box,Cylinder,ElevationGrid,IndexedFaceSet,Sphere highlightedSecondaryNode;
  class Appearance,Background,Color,Cone,Coordinate,DirectionalLight,Fog,Group,ImageTexture,IndexedLineSet,Material,Normal,PointLight,PointSet,Shape,SpotLight,TextureCoordinate,TextureTransform,Transform,Viewpoint,WorldInfo highlightedNode;
  class AbstractClassDefinition,BoundingObjectDefinition,VRML97Definition DefinitionStyle;
%end
%end
