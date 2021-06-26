#ifndef WR_STATIC_MESH_H
#define WR_STATIC_MESH_H

#ifdef __cplusplus
extern "C" {
#endif

struct WrStaticMesh;
typedef struct WrStaticMesh WrStaticMesh;

/* Creates an axis-aligned box mesh with edge length 1.0f, centered on the origin. */
WrStaticMesh *wr_static_mesh_unit_box_new(bool outline);
/* Creates a cone with radius 1.0f and height 1.0f, centered on the origin with its main axis on +Y. */
WrStaticMesh *wr_static_mesh_unit_cone_new(int subdivision, bool has_side, bool has_bottom);
/* Creates a cylinder mesh with radius 1.0f and height 1.0f, centered on the origin with its main axis on +Y. */
WrStaticMesh *wr_static_mesh_unit_cylinder_new(int subdivision, bool has_side, bool has_top, bool has_bottom, bool outline);
/* Creates a elevation grid mesh with specified dimension, heights and unit spacing, starting at the origin and extending in +X
 * and +Z. */
WrStaticMesh *wr_static_mesh_unit_elevation_grid_new(int dimension_x, int dimension_z, const float *height_data,
                                                     float thickness, bool outline);
/* Creates a rectangle mesh with an edge length of 1.0f, centered on the origin and perpendicular to +Y. */
WrStaticMesh *wr_static_mesh_unit_rectangle_new(bool outline);
/* Creates a screen-sized quad useful for post-processing */
WrStaticMesh *wr_static_mesh_quad_new();
/* Creates a sphere mesh with a radius of 1.0f, centered on the origin. */
WrStaticMesh *wr_static_mesh_unit_sphere_new(int subdivision, bool ico, bool outline);
/* Creates a capsule with given radius and height, centered on the origin with its main axis on +Y.
   Since a capsule cannot simply be scaled when its radius or height changes, they have to be passed as parameters. */
WrStaticMesh *wr_static_mesh_capsule_new(int subdivision, float radius, float height, bool has_side, bool has_top,
                                         bool has_bottom, bool outline);
/* Creates a line set. Each pair of coordinates defines a line. */
WrStaticMesh *wr_static_mesh_line_set_new(int coord_count, const float *coord_data, const float *color_data);
/* Creates a point set. Each coordinate defines a point. */
WrStaticMesh *wr_static_mesh_point_set_new(int coord_count, const float *coord_data, const float *color_data);
/* Creates a triangle mesh using the provided vertex and index data. */
WrStaticMesh *wr_static_mesh_new(int vertex_count, int index_count, const float *coord_data, const float *normal_data,
                                 const float *tex_coord_data, const float *unwrapped_tex_coord_data,
                                 const unsigned int *index_data, bool outline);

void wr_static_mesh_delete(WrStaticMesh *mesh);

/* Returns the bounding sphere in mesh space.
   sphere[0]..[2] = center, sphere[3] = radius */
void wr_static_mesh_get_bounding_sphere(WrStaticMesh *mesh, float *sphere);
/* Returns mesh data, NULL can be passed for unwanted data.
   This function should be avoided in performance sensitive code as it involves heavy memory operations. */
void wr_static_mesh_read_data(WrStaticMesh *mesh, float *coord_data, float *normal_data, float *tex_coord_data,
                              unsigned int *index_data);
int wr_static_mesh_get_vertex_count(WrStaticMesh *mesh);
int wr_static_mesh_get_index_count(WrStaticMesh *mesh);

#ifdef __cplusplus
}
#endif

#endif  // WR_STATIC_MESH_H
