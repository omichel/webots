# Bedroom

## Bed

Customizable bed with feet and one or 2 pillows.

%figure

![Bed](images/objects/bedroom/Bed/model.thumbnail.png)

%end

Derived from [Solid](../reference/solid.md).

```
Bed {
  SFVec3f    translation        0 0 0
  SFRotation rotation           0 1 0 0
  SFString   name               "bed"
  SFNode     frameAppearance    PaintedWood { colorOverride 0.11 0.11 0.11 }
  SFNode     mattressAppearance PBRAppearance { roughness 1 metalness 0 }
  SFNode     pillowAppearance   PBRAppearance { roughness 1 metalness 0 }
  SFNode     blanketAppearance  PBRAppearance { baseColorMap ImageTexture { url "textures/duvet.jpg" } roughness 1 metalness 0 }
  MFColor    recognitionColors  []
}
```

> **File location**: "[WEBOTS\_HOME/projects/objects/bedroom/protos/Bed.proto]({{ url.github_tree }}/projects/objects/bedroom/protos/Bed.proto)"

> **License**: Copyright Cyberbotics Ltd. Licensed for use only with Webots.
[More information.](https://cyberbotics.com/webots_assets_license)

### Bed Field Summary

- `frameAppearance`: Defines the appearance of the frame.

- `mattressAppearance`: Defines the appearance of the mattress.

- `pillowAppearance`: Defines the appearance of the pillows.

- `blanketAppearance`: Defines the appearance of the blanket.

