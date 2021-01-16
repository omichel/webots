# Appearances
This chapter describes the list of available appearance PROTO nodes based on the [PBRAppearance](../reference/pbrappearance.md) node.

## Asphalt

An asphalt material. The color can be overridden using the `colorOverride` field. Useful with the `Road` PROTO.

%figure

![Asphalt](images/appearances/Asphalt.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Asphalt {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Asphalt.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Asphalt.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Asphalt Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## BakelitePlastic

A bakelite plastic.

%figure

|     |     |
|:---:|:---:|
| ![BakelitePlastic_a.png](images/appearances/BakelitePlastic_a.thumbnail.png) |![BakelitePlastic_b.png](images/appearances/BakelitePlastic_b.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
BakelitePlastic {
  SFString  color            "red"
  SFNode    textureTransform NULL
  SFFloat   IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/BakelitePlastic.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/BakelitePlastic.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### BakelitePlastic Field Summary

- `color`: Defines the color of the plastic. This field accepts the following values: `"red"` and `"braun"`.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## BlanketFabric

A blanket fabric material.

%figure

![BlanketFabric](images/appearances/BlanketFabric.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
BlanketFabric {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/BlanketFabric.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/BlanketFabric.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### BlanketFabric Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## BrushedAluminium

A brushed aluminum material.

%figure

![BrushedAluminium](images/appearances/BrushedAluminium.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
BrushedAluminium {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/BrushedAluminium.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/BrushedAluminium.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### BrushedAluminium Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## BrushedSteel

A brushed steel material.

%figure

![BrushedSteel](images/appearances/BrushedSteel.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
BrushedSteel {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/BrushedSteel.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/BrushedSteel.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### BrushedSteel Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## CarpetFibers

A woolen carpet material.

%figure

|     |     |
|:---:|:---:|
| ![CarpetFibers_a.png](images/appearances/CarpetFibers_a.thumbnail.png) |![CarpetFibers_b.png](images/appearances/CarpetFibers_b.thumbnail.png) |
| ![CarpetFibers_c.png](images/appearances/CarpetFibers_c.thumbnail.png) | |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
CarpetFibers {
  SFColor   colorOverride    1 1 1
  SFString  type             "wooly"
  SFNode    textureTransform NULL
  SFFloat   IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/CarpetFibers.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/CarpetFibers.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### CarpetFibers Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `type`: Defines the carpet type. This field accepts the following values: `"wooly"`, `"synthetic"` and `"pattern"`.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## CementTiles

A cement tiles material.

%figure

![CementTiles](images/appearances/CementTiles.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
CementTiles {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/CementTiles.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/CementTiles.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### CementTiles Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Copper

A copper material.

%figure

![Copper](images/appearances/Copper.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Copper {
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Copper.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Copper.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Copper Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## CorrodedMetal

A corroded metal material. The color can be overridden using the `colorOverride` field.

%figure

![CorrodedMetal](images/appearances/CorrodedMetal.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
CorrodedMetal {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/CorrodedMetal.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/CorrodedMetal.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### CorrodedMetal Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## CorrugatedMetal

A corrugated metal material. The color can be overridden using the `colorOverride` field.

%figure

![CorrugatedMetal](images/appearances/CorrugatedMetal.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
CorrugatedMetal {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/CorrugatedMetal.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/CorrugatedMetal.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### CorrugatedMetal Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## CorrugatedPlates

A corrugated plates material. The color can be overridden using the `colorOverride` field.

%figure

![CorrugatedPlates](images/appearances/CorrugatedPlates.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
CorrugatedPlates {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/CorrugatedPlates.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/CorrugatedPlates.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### CorrugatedPlates Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## CorrugatedPvc

A corrugated PVC material. The color can be overridden using the `colorOverride` field.

%figure

![CorrugatedPvc](images/appearances/CorrugatedPvc.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
CorrugatedPvc {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/CorrugatedPvc.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/CorrugatedPvc.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### CorrugatedPvc Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## DamascusSteel

A damascus steel material. The color can be overridden using the `colorOverride` field.

%figure

![DamascusSteel](images/appearances/DamascusSteel.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
DamascusSteel {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/DamascusSteel.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/DamascusSteel.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### DamascusSteel Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## DryMud

A dry mud material with cracks.

%figure

![DryMud](images/appearances/DryMud.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
DryMud {
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/DryMud.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/DryMud.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### DryMud Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## ElectricConduit

An electric conduit material. The color can be overridden using the `colorOverride` field.

%figure

![ElectricConduit](images/appearances/ElectricConduit.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
ElectricConduit {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/ElectricConduit.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/ElectricConduit.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### ElectricConduit Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## FlexibleAluminiumDuct

A flexible aluminium duct material. The color can be overridden using the `colorOverride` field.

%figure

![FlexibleAluminiumDuct](images/appearances/FlexibleAluminiumDuct.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
FlexibleAluminiumDuct {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/FlexibleAluminiumDuct.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/FlexibleAluminiumDuct.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### FlexibleAluminiumDuct Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## FormedConcrete

A formed concrete material.

%figure

![FormedConcrete](images/appearances/FormedConcrete.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
FormedConcrete {
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/FormedConcrete.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/FormedConcrete.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### FormedConcrete Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## GalvanizedMetal

A galvanized metal material. The color can be overridden using the `colorOverride` field.

%figure

![GalvanizedMetal](images/appearances/GalvanizedMetal.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
GalvanizedMetal {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/GalvanizedMetal.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/GalvanizedMetal.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### GalvanizedMetal Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## GlossyPaint

A glossy paint material. The color can be selected in the `baseColor` field.

%figure

![GlossyPaint](images/appearances/GlossyPaint.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
GlossyPaint {
  SFColor baseColor        1 1 1
  SFNode  textureTransform TextureTransform { scale 10 10 }
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/GlossyPaint.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/GlossyPaint.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### GlossyPaint Field Summary

- `baseColor`: Defines the paint's color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Grass

A grass material containing several grass types.

%figure

|     |     |
|:---:|:---:|
| ![Grass_a.png](images/appearances/Grass_a.thumbnail.png) |![Grass_b.png](images/appearances/Grass_b.thumbnail.png) |
| ![Grass_c.png](images/appearances/Grass_c.thumbnail.png) |![Grass_d.png](images/appearances/Grass_d.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Grass {
  SFColor  colorOverride    1 1 1
  SFString type             "maintained"
  SFNode   textureTransform NULL
  SFFloat  IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Grass.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Grass.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Grass Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `type`: Defines the grass type. This field accepts the following values: `"maintained"`, `"artificial"`, `"prickly"` and `"mossy"`.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## HammeredCopper

An hammered copper steel material. The color can be overridden using the `colorOverride` field.

%figure

![HammeredCopper](images/appearances/HammeredCopper.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
HammeredCopper {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/HammeredCopper.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/HammeredCopper.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### HammeredCopper Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Leather

A leather material. The color can be overridden using the `colorOverride` field.

%figure

|     |     |
|:---:|:---:|
| ![Leather_a.png](images/appearances/Leather_a.thumbnail.png) |![Leather_b.png](images/appearances/Leather_b.thumbnail.png) |
| ![Leather_c.png](images/appearances/Leather_c.thumbnail.png) | |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Leather {
  SFColor   colorOverride     1 1 1
  SFString  tone              "dark"
  SFString  type              "dollaro"
  SFNode    textureTransform  NULL
  SFFloat   IBLStrength       1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Leather.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Leather.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Leather Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `tone`: Defines the tone of the leather. This field accepts the following values: `"dark"` and `"light"`.

- `type`: Defines the type of the leather. This field accepts the following values: `"dollaro"` and `"grain"`.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## LedStrip

A led strip material.

%figure

![LedStrip](images/appearances/LedStrip.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
LedStrip {
  SFColor colorOverride     1 1 1
  SFNode  textureTransform  NULL
  SFFloat IBLStrength       1
  SFFloat emissiveIntensity 1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/LedStrip.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/LedStrip.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### LedStrip Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

- `emissiveIntensity`: Defines the intensity of the leds.

## Marble

A beige marble material.

%figure

![Marble](images/appearances/Marble.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Marble {
  SFColor colorOverride     1 1 1
  SFNode  textureTransform  NULL
  SFFloat IBLStrength       1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Marble.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Marble.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Marble Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## MarbleTiles

A marble tiles material.

%figure

![MarbleTiles](images/appearances/MarbleTiles.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
MarbleTiles {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/MarbleTiles.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/MarbleTiles.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### MarbleTiles Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## MattePaint

A matte paint material. The color can be selected in the `baseColor` field.

%figure

![MattePaint](images/appearances/MattePaint.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
MattePaint {
  SFColor baseColor        1 1 1
  SFNode  textureTransform TextureTransform { scale 10 10 }
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/MattePaint.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/MattePaint.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### MattePaint Field Summary

- `baseColor`: Defines the paint's color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## MetalPipePaint

A worn paint layer on a metal surface. Useful with the `PipeSection` PROTO or any painted industrial surfaces.

%figure

![MetalPipePaint](images/appearances/MetalPipePaint.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
MetalPipePaint {
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/MetalPipePaint.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/MetalPipePaint.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### MetalPipePaint Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## MetalStainlessSteelCable

A stainless steel cable appearance.

%figure

![MetalStainlessSteelCable](images/appearances/MetalStainlessSteelCable.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
MetalStainlessSteelCable {
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/MetalStainlessSteelCable.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/MetalStainlessSteelCable.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### MetalStainlessSteelCable Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## OldPlywood

An old plywood material.

%figure

![OldPlywood](images/appearances/OldPlywood.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
OldPlywood {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/OldPlywood.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/OldPlywood.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### OldPlywood Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## OldSteel

An old battered steel material.

%figure

![OldSteel](images/appearances/OldSteel.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
OldSteel {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/OldSteel.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/OldSteel.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### OldSteel Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## OsbWood

An OSB wood material. The color can be overridden using the `colorOverride` field.

%figure

![OsbWood](images/appearances/OsbWood.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
OsbWood {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/OsbWood.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/OsbWood.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### OsbWood Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## PaintedWood

A painted wood material. The color can be overridden using the `colorOverride` field.

%figure

![PaintedWood](images/appearances/PaintedWood.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
PaintedWood {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/PaintedWood.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/PaintedWood.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### PaintedWood Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Parquetry

A parquetry material.
The type of parquetry can be set using the `type` field.
The color can be overridden using the `colorOverride` field.

%figure

|     |     |
|:---:|:---:|
| ![Parquetry_a.png](images/appearances/Parquetry_a.thumbnail.png) |![Parquetry_b.png](images/appearances/Parquetry_b.thumbnail.png) |
| ![Parquetry_c.png](images/appearances/Parquetry_c.thumbnail.png) |![Parquetry_d.png](images/appearances/Parquetry_d.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Parquetry {
  SFString type             "mosaic"
  SFColor  colorOverride    1 1 1
  SFNode   textureTransform NULL
  SFFloat  IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Parquetry.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Parquetry.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Parquetry Field Summary

- `type`: Defines the parquetry type. This field accepts the following values: `"chequered"`, `"dark strip"`, `"light strip"` and `"mosaic"`.

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Pavement

A pavement material.
The type of pavement can be set using the `type` field.
The color can be overridden using the `colorOverride` field.

%figure

|     |     |
|:---:|:---:|
| ![Pavement_a.png](images/appearances/Pavement_a.thumbnail.png) |![Pavement_b.png](images/appearances/Pavement_b.thumbnail.png) |
| ![Pavement_c.png](images/appearances/Pavement_c.thumbnail.png) |![Pavement_d.png](images/appearances/Pavement_d.thumbnail.png) |
| ![Pavement_e.png](images/appearances/Pavement_e.thumbnail.png) |![Pavement_f.png](images/appearances/Pavement_f.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Pavement {
  SFString type             "braun square stone"
  SFColor  colorOverride     1 1 1
  SFNode   textureTransform  NULL
  SFFloat  IBLStrength       1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Pavement.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Pavement.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Pavement Field Summary

- `type`: Defines the pavement type. This field accepts the following values: `"black stone"`, `"braun stone"`, `"braun square stone"`, `"grid"`, `"slate"` and `"tiles"`.

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Pcb

A PCB material.

%figure

![Pcb](images/appearances/Pcb.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Pcb {
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Pcb.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Pcb.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Pcb Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## PerforatedMetal

A perforated metal material.

%figure

![PerforatedMetal](images/appearances/PerforatedMetal.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
PerforatedMetal {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/PerforatedMetal.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/PerforatedMetal.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### PerforatedMetal Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Plaster

A plaster material. The color can be overridden using the `colorOverride` field.

%figure

![Plaster](images/appearances/Plaster.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Plaster {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Plaster.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Plaster.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Plaster Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Plastic

A plastic material containing several types.

%figure

|     |     |
|:---:|:---:|
| ![Plastic_a.png](images/appearances/Plastic_a.thumbnail.png) |![Plastic_b.png](images/appearances/Plastic_b.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Plastic {
  SFColor  colorOverride    1 1 1
  SFString type             "rough"
  SFNode   textureTransform NULL
  SFFloat  IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Plastic.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Plastic.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Plastic Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `type`: Defines the plastic type. This field accepts the following values: `"rough"` and `"padded"`.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## PorcelainChevronTiles

A porcelain chevron tiles material.

%figure

![PorcelainChevronTiles](images/appearances/PorcelainChevronTiles.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
PorcelainChevronTiles {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/PorcelainChevronTiles.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/PorcelainChevronTiles.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### PorcelainChevronTiles Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## RedBricks

A tiled brick material. Useful with the `Wall` PROTO.

%figure

![RedBricks](images/appearances/RedBricks.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
RedBricks {
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/RedBricks.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/RedBricks.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### RedBricks Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## ReflectiveSurface

A reflective surface material.

%figure

![ReflectiveSurface](images/appearances/ReflectiveSurface.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
ReflectiveSurface {
  SFFloat metalness         0.5
  SFColor emissiveColor     0 0 0
  SFFloat emissiveIntensity 1
  SFColor baseColorMap      1 1 1
  SFBool  useBaseColorMap   TRUE
  SFNode  textureTransform  NULL
  SFFloat IBLStrength       1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/ReflectiveSurface.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/ReflectiveSurface.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### ReflectiveSurface Field Summary

- `useBaseColorMap`: Defines wheather the base color texture should be used or not.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## RoughConcrete

A rough concrete material. The color can be overridden using the `colorOverride` field.

%figure

![RoughConcrete](images/appearances/RoughConcrete.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
RoughConcrete {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/RoughConcrete.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/RoughConcrete.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### RoughConcrete Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## RoughOak

A rough oak material. The color can be overridden using the `colorOverride` field.

%figure

![RoughOak](images/appearances/RoughOak.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
RoughOak {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/RoughOak.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/RoughOak.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### RoughOak Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## RoughPine

A pine wood material. The color can be overridden using the `colorOverride` field.

%figure

![RoughPine](images/appearances/RoughPine.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
RoughPine {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/RoughPine.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/RoughPine.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### RoughPine Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## RoughPolymer

A rough polymer material.

%figure

![RoughPolymer](images/appearances/RoughPolymer.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
RoughPolymer {
  SFColor baseColor        1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/RoughPolymer.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/RoughPolymer.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### RoughPolymer Field Summary

- `baseColor`: Defines the base color of the material.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Roughcast

A rough plaster material. Useful with the `Wall` PROTO.

%figure

![Roughcast](images/appearances/Roughcast.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Roughcast {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Roughcast.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Roughcast.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Roughcast Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Rubber

A rubber material.
The type of rubber can be set using the `type` field.

%figure

|     |     |
|:---:|:---:|
| ![Rubber_a.png](images/appearances/Rubber_a.thumbnail.png) |![Rubber_b.png](images/appearances/Rubber_b.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Rubber {
  SFString  type              "flat"
  SFNode    textureTransform  NULL
  SFFloat   IBLStrength       1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Rubber.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Rubber.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Rubber Field Summary

- `type`: Defines the rubber type. This field accepts the following values: `"flat"` and `"dotted"`.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## RustyMetal

An old battered steel material.
The color can be overridden using the `colorOverride` field and the amount of rust can be set using the `rustLevel` field.

%figure

![RustyMetal](images/appearances/RustyMetal.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
RustyMetal {
  SFInt32  rustLevel        1
  SFColor  colorOverride    1 1 1
  SFNode   textureTransform NULL
  SFFloat  IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/RustyMetal.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/RustyMetal.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### RustyMetal Field Summary

- `rustLevel`: Defines how much the metal is rusted. This field accepts the following values: `1` and `2`.

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Sand

A sand material.

%figure

![Sand](images/appearances/Sand.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Sand {
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Sand.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Sand.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Sand Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## SandyGround

A sandy ground material. The color can be selected using the `colorOverride` field. Useful with the UnevenTerrain PROTO.

%figure

![SandyGround](images/appearances/SandyGround.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
SandyGround {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform TextureTransform { scale 4 4 }
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/SandyGround.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/SandyGround.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### SandyGround Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## ScratchedPaint

A scratched paint material.

%figure

![ScratchedPaint](images/appearances/ScratchedPaint.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
ScratchedPaint {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
  SFBool  metalScratch     TRUE
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/ScratchedPaint.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/ScratchedPaint.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### ScratchedPaint Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

- `metalScratch`: Defines if the scratches should uncover metal or not.

## ScrewThread

A screw thread material. The color can be overridden using the `colorOverride` field.

%figure

![ScrewThread](images/appearances/ScrewThread.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
ScrewThread {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/ScrewThread.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/ScrewThread.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### ScrewThread Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## ScuffedPlastic

A worn scuffed plastic material. The color can be selected using the `colorOverride` field.

%figure

![ScuffedPlastic](images/appearances/ScuffedPlastic.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
ScuffedPlastic {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform TextureTransform { scale 4 4 }
  SFNode  environmentMap   NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/ScuffedPlastic.proto](https://github.com/omichel/webots/tree/master/projects/appearances/protos/ScuffedPlastic.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### ScuffedPlastic Field Summary

- `colorOverride`: Defines the plastic's color.

- `textureTransform`: Defines an optional 2d texture transform.

- `environmentMap`: Defines a `Cubemap` node overriding the skybox for this object.

- `IBLStrength`: Defines the strength of ambient lighting from the Cubemap node.

## ShinyLeather

A shiny leather material. The color can be selected using the `colorOverride` field. Useful with any of the vehicle PROTOs.

%figure

![ShinyLeather](images/appearances/ShinyLeather.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
ShinyLeather {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform TextureTransform { scale 4 4 }
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/ShinyLeather.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/ShinyLeather.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### ShinyLeather Field Summary

- `colorOverride`: Defines the leather's color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## Soil

A soil material.

%figure

|     |     |
|:---:|:---:|
| ![Soil_a.png](images/appearances/Soil_a.thumbnail.png) |![Soil_b.png](images/appearances/Soil_b.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
Soil {
  SFString  type              "braun"
  SFNode    textureTransform  NULL
  SFColor   color             1 1 1
  SFFloat   IBLStrength       1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/Soil.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/Soil.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### Soil Field Summary

- `type`: Defines the soil type. This field accepts the following values: `"braun"` and `"grey"`.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## SolarCell

A solar panel cell material.

%figure

![SolarCell](images/appearances/SolarCell.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
SolarCell {
  SFNode  textureTransform  NULL
  SFFloat IBLStrength       1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/SolarCell.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/SolarCell.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### SolarCell Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## ThreadMetalPlate

A worn paint layer on a metal surface.
The type of metal surface can be set using the `type` field.

%figure

|     |     |
|:---:|:---:|
| ![ThreadMetalPlate_a.png](images/appearances/ThreadMetalPlate_a.thumbnail.png) |![ThreadMetalPlate_b.png](images/appearances/ThreadMetalPlate_b.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
ThreadMetalPlate {
  SFString  type               "oval"
  SFColor   colorOverride      1 1 1
  SFNode    textureTransform   NULL
  SFFloat   IBLStrength        1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/ThreadMetalPlate.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/ThreadMetalPlate.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### ThreadMetalPlate Field Summary

- `type`: Defines the motif type on the material. This field accepts the following values: `"oval"` and `"square"`.

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## TireRubber

A tire rubber material.
It can be a car tire or a bike tire.

%figure

|     |     |
|:---:|:---:|
| ![TireRubber_a.png](images/appearances/TireRubber_a.thumbnail.png) |![TireRubber_b.png](images/appearances/TireRubber_b.thumbnail.png) |

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
TireRubber {
  SFNode    textureTransform NULL
  SFFloat   IBLStrength      1
  SFString  type             "car"
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/TireRubber.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/TireRubber.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### TireRubber Field Summary

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

- `type`: Defines the type of tire. This field accepts the following values: `"bike"` and `"car"`.

## VarnishedPine

A pine wood material covered with a layer of varnish. The color can be overridden using the `colorOverride` field.

%figure

![VarnishedPine](images/appearances/VarnishedPine.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
VarnishedPine {
  SFColor colorOverride    0.8039 0.6745 0.5764
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/VarnishedPine.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/VarnishedPine.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### VarnishedPine Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## WireFence

A wire fence material. The color can be overridden using the `colorOverride` field.

%figure

![WireFence](images/appearances/WireFence.thumbnail.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
WireFence {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/WireFence.proto](https://github.com/cyberbotics/webots/tree/released/projects/appearances/protos/WireFence.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### WireFence Field Summary

- `colorOverride`: Defines the default color multiplied with the texture color.

- `textureTransform`: Defines an optional 2d texture transform.

- `IBLStrength`: Defines the strength of ambient lighting from the [Background](../reference/background.md) node.

## WornBurlap

A worn burlap material. The color can be selected using the `colorOverride` field.

%figure

![WornBurlap](images/appearances/WornBurlap.png)

%end

Derived from [PBRAppearance](../reference/pbrappearance.md).

```
WornBurlap {
  SFColor colorOverride    1 1 1
  SFNode  textureTransform TextureTransform { scale 4 4 }
  SFNode  environmentMap   NULL
  SFFloat IBLStrength      1
}
```

> **File location**: "[WEBOTS\_HOME/projects/appearances/protos/WornBurlap.proto](https://github.com/omichel/webots/tree/master/projects/appearances/protos/WornBurlap.proto)"

> **License**: Apache License 2.0
[More information.](http://www.apache.org/licenses/LICENSE-2.0)

### WornBurlap Field Summary

- `colorOverride`: Defines the burlap's color.

- `textureTransform`: Defines an optional 2d texture transform.

- `environmentMap`: Defines a `Cubemap` node overriding the skybox for this object.

- `IBLStrength`: Defines the strength of ambient lighting from the Cubemap node.

