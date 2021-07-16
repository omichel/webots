## Shape

```
Shape {
  SFNode appearance  NULL   # {Appearance, PROTO}
  SFNode geometry    NULL   # {Geometry Primitive, PROTO}
  SFBool castShadows TRUE   # {TRUE, FALSE}
  SFBool isPickable  TRUE   # {TRUE, FALSE}
}
```

### Description

The [Shape](#shape) node is used to create rendered objects in the world.
Visible objects are constituted by a geometry and an appearance.

### Field Summary

- The `appearance` field contains an [Appearance](appearance.md) or [PBRAppearance](pbrappearance.md) node that specifies the visual attributes (e.g., material and texture) to be applied to the geometry.

- The `geometry` field contains a `Geometry` node: [Box](box.md), [Capsule](capsule.md), [Cone](cone.md), [Cylinder](cylinder.md), [ElevationGrid](elevationgrid.md), [IndexedFaceSet](indexedfaceset.md), [IndexedLineSet](indexedlineset.md), [Mesh](mesh.md), [Plane](plane.md), [PointSet](pointset.md) or [Sphere](sphere.md).
The specified `Geometry` node is rendered with the specified appearance nodes applied.

- The `castShadows` field allows the user to turn on (TRUE) or off (FALSE) shadows casted by this shape.
However, shapes containing more than 65535 vertices will ignore this field and won't cast any shadow to save performance.

- The `isPickable` field defines if the object is detected (TRUE) or not (FALSE) when clicking on the 3D scene.

> **Note**: Objects cast shadows only if the world contains at least one [Light](light.md) node with `castShadows` field set to TRUE and if shadows are not disabled in the application preferences.
