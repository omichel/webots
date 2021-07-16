## Appearance

```
Appearance {
  SFNode   material         NULL           # {Material, PROTO}
  SFNode   texture          NULL           # {ImageTexture, PROTO}
  SFNode   textureTransform NULL           # {TextureTransform, PROTO}
  SFString name             "appearance"   # any string
}
```

### Description

The [Appearance](#appearance) node (like the [PBRAppearance](pbrappearance.md) node) specifies the visual properties of a geometric node. It should be set inside a [Shape](shape.md) node.
The value for each of the fields in this node may be NULL.
However, if the field is non-NULL, it shall contain one node of the appropriate type.

### Field Summary

- The `material` field, if specified, shall contain a [Material](material.md) node.
If the `material` field is NULL, lighting is off (all lights are ignored during the rendering of the object that references this [Appearance](#appearance)) and the unlit object color is (1,1,1).

- The `texture` field, if specified, shall contain an [ImageTexture](imagetexture.md) node.
If the `texture` node is NULL, the object that references this [Appearance](#appearance) is not textured.

- The `textureTransform` field, if specified, shall contain a [TextureTransform](texturetransform.md) node.
If the `textureTransform` is NULL, the `textureTransform` field has no effect.
Otherwise, the [TextureTransform](texturetransform.md) is applied to the [ImageTexture](imagetexture.md) in the `texture` field when shading the object.

- The `name` field is used to give a unique identifier to an [Appearance](#appearance) node such that if it is included in an MFNode field it can be retrieved by this name.
This name is not required to be unique if the [Appearance](#appearance) node is inserted into an SFNode field.
