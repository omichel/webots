/*
 * Copyright 1996-2021 Cyberbotics Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**********************************************************************************/
/* Description:  Webots C programming interface for the Supervisor node           */
/**********************************************************************************/

#ifndef WB_SUPERVISOR_H
#define WB_SUPERVISOR_H

#define WB_USING_C_API
#include "nodes.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  WB_NO_FIELD = 0x00,
  WB_SF_BOOL = 0x01,
  WB_SF_INT32,
  WB_SF_FLOAT,
  WB_SF_VEC2F,
  WB_SF_VEC3F,
  WB_SF_ROTATION,
  WB_SF_COLOR,
  WB_SF_STRING,
  WB_SF_NODE,
  WB_MF = 0x10,
  WB_MF_BOOL,
  WB_MF_INT32,
  WB_MF_FLOAT,
  WB_MF_VEC2F,
  WB_MF_VEC3F,
  WB_MF_ROTATION,
  WB_MF_COLOR,
  WB_MF_STRING,
  WB_MF_NODE
} WbFieldType;

typedef enum {
  WB_SUPERVISOR_SIMULATION_MODE_PAUSE = 0,
  WB_SUPERVISOR_SIMULATION_MODE_REAL_TIME,
  WB_SUPERVISOR_SIMULATION_MODE_FAST
} WbSimulationMode;

void wb_supervisor_world_load(const char *filename);
bool wb_supervisor_world_save(const char *filename);
void wb_supervisor_world_reload();

void wb_supervisor_simulation_quit(int status);
void wb_supervisor_simulation_reset();
void wb_supervisor_simulation_reset_physics();

WbSimulationMode wb_supervisor_simulation_get_mode();
void wb_supervisor_simulation_set_mode(WbSimulationMode mode);

void wb_supervisor_set_label(int id, const char *text, double x, double y, double size, int color, double transparency,
                             const char *font);

void wb_supervisor_export_image(const char *filename, int quality);

void wb_supervisor_movie_start_recording(const char *filename, int width, int height, int codec, int quality, int acceleration,
                                         bool caption);
void wb_supervisor_movie_stop_recording();
bool wb_supervisor_movie_is_ready();
bool wb_supervisor_movie_failed();

bool wb_supervisor_animation_start_recording(const char *filename);
bool wb_supervisor_animation_stop_recording();

WbNodeRef wb_supervisor_node_get_root();
WbNodeRef wb_supervisor_node_get_self();
int wb_supervisor_node_get_id(WbNodeRef node);
WbNodeRef wb_supervisor_node_get_from_id(int id);
WbNodeRef wb_supervisor_node_get_from_device(WbDeviceTag tag);
WbNodeRef wb_supervisor_node_get_from_def(const char *def);
WbNodeRef wb_supervisor_node_get_from_proto_def(WbNodeRef node, const char *def);
WbNodeRef wb_supervisor_node_get_parent_node(WbNodeRef node);
WbNodeRef wb_supervisor_node_get_selected();
WbNodeType wb_supervisor_node_get_type(WbNodeRef node);
WbFieldRef wb_supervisor_node_get_field(WbNodeRef node, const char *field_name);
WbFieldRef wb_supervisor_node_get_field_by_index(WbNodeRef node, int index);
WbFieldRef wb_supervisor_node_get_proto_field(WbNodeRef node, const char *field_name);
WbFieldRef wb_supervisor_node_get_proto_field_by_index(WbNodeRef node, int index);
void wb_supervisor_node_remove(WbNodeRef node);
void wb_supervisor_node_save_state(WbNodeRef node, const char *state_name);
void wb_supervisor_node_load_state(WbNodeRef node, const char *state_name);

const char *wb_supervisor_node_get_def(WbNodeRef node);
const char *wb_supervisor_node_get_type_name(WbNodeRef node);
const char *wb_supervisor_node_get_base_type_name(WbNodeRef node);
bool wb_supervisor_node_is_proto(WbNodeRef node);
const double *wb_supervisor_node_get_center_of_mass(WbNodeRef node);
const double *wb_supervisor_node_get_contact_point(WbNodeRef node, int index);
WbNodeRef wb_supervisor_node_get_contact_point_node(WbNodeRef node, int index);
int wb_supervisor_node_get_number_of_contact_points(WbNodeRef node, bool include_descendants);
const double *wb_supervisor_node_get_orientation(WbNodeRef node);
const double *wb_supervisor_node_get_position(WbNodeRef node);
const double *wb_supervisor_node_get_pose(WbNodeRef node, WbNodeRef from_node);
bool wb_supervisor_node_get_static_balance(WbNodeRef node);
const double *wb_supervisor_node_get_velocity(WbNodeRef node);
void wb_supervisor_node_set_velocity(WbNodeRef node, const double velocity[6]);
void wb_supervisor_node_reset_physics(WbNodeRef node);
void wb_supervisor_node_restart_controller(WbNodeRef node);
const char *wb_supervisor_node_export_string(WbNodeRef node);

void wb_supervisor_node_move_viewpoint(WbNodeRef node);

void wb_supervisor_node_set_visibility(WbNodeRef node, WbNodeRef from, bool visible);

void wb_supervisor_node_add_force(WbNodeRef node, const double force[3], bool relative);
void wb_supervisor_node_add_force_with_offset(WbNodeRef node, const double force[3], const double offset[3], bool relative);
void wb_supervisor_node_add_torque(WbNodeRef node, const double torque[3], bool relative);

int wb_supervisor_node_get_number_of_fields(WbNodeRef node);
int wb_supervisor_node_get_proto_number_of_fields(WbNodeRef node);
const char *wb_supervisor_field_get_name(WbFieldRef field);
WbFieldType wb_supervisor_field_get_type(WbFieldRef field);
const char *wb_supervisor_field_get_type_name(WbFieldRef field);
int wb_supervisor_field_get_count(WbFieldRef field);

void wb_supervisor_field_enable_sf_tracking(WbFieldRef field, int sampling_period);
void wb_supervisor_field_disable_sf_tracking(WbFieldRef field);
void wb_supervisor_node_enable_pose_tracking(WbNodeRef node, int sampling_period, WbNodeRef from_node);
void wb_supervisor_node_disable_pose_tracking(WbNodeRef node, WbNodeRef from_node);

bool wb_supervisor_field_get_sf_bool(WbFieldRef field);
int wb_supervisor_field_get_sf_int32(WbFieldRef field);
double wb_supervisor_field_get_sf_float(WbFieldRef field);
const double *wb_supervisor_field_get_sf_vec2f(WbFieldRef field);
const double *wb_supervisor_field_get_sf_vec3f(WbFieldRef field);
const double *wb_supervisor_field_get_sf_rotation(WbFieldRef field);
const double *wb_supervisor_field_get_sf_color(WbFieldRef field);
const char *wb_supervisor_field_get_sf_string(WbFieldRef field);
WbNodeRef wb_supervisor_field_get_sf_node(WbFieldRef field);

bool wb_supervisor_field_get_mf_bool(WbFieldRef field, int index);
int wb_supervisor_field_get_mf_int32(WbFieldRef field, int index);
double wb_supervisor_field_get_mf_float(WbFieldRef field, int index);
const double *wb_supervisor_field_get_mf_vec2f(WbFieldRef field, int index);
const double *wb_supervisor_field_get_mf_vec3f(WbFieldRef field, int index);
const double *wb_supervisor_field_get_mf_color(WbFieldRef field, int index);
const double *wb_supervisor_field_get_mf_rotation(WbFieldRef field, int index);
const char *wb_supervisor_field_get_mf_string(WbFieldRef field, int index);
WbNodeRef wb_supervisor_field_get_mf_node(WbFieldRef field, int index);

void wb_supervisor_field_set_sf_bool(WbFieldRef field, bool value);
void wb_supervisor_field_set_sf_int32(WbFieldRef field, int value);
void wb_supervisor_field_set_sf_float(WbFieldRef field, double value);
void wb_supervisor_field_set_sf_vec2f(WbFieldRef field, const double values[2]);
void wb_supervisor_field_set_sf_vec3f(WbFieldRef field, const double values[3]);
void wb_supervisor_field_set_sf_rotation(WbFieldRef field, const double values[4]);
void wb_supervisor_field_set_sf_color(WbFieldRef field, const double values[3]);
void wb_supervisor_field_set_sf_string(WbFieldRef field, const char *value);

void wb_supervisor_field_set_mf_bool(WbFieldRef field, int index, bool value);
void wb_supervisor_field_set_mf_int32(WbFieldRef field, int index, int value);
void wb_supervisor_field_set_mf_float(WbFieldRef field, int index, double value);
void wb_supervisor_field_set_mf_vec2f(WbFieldRef field, int index, const double values[2]);
void wb_supervisor_field_set_mf_vec3f(WbFieldRef field, int index, const double values[3]);
void wb_supervisor_field_set_mf_rotation(WbFieldRef field, int index, const double values[4]);
void wb_supervisor_field_set_mf_color(WbFieldRef field, int index, const double values[3]);
void wb_supervisor_field_set_mf_string(WbFieldRef field, int index, const char *value);

void wb_supervisor_field_insert_mf_bool(WbFieldRef field, int index, bool value);
void wb_supervisor_field_insert_mf_int32(WbFieldRef field, int index, int value);
void wb_supervisor_field_insert_mf_float(WbFieldRef field, int index, double value);
void wb_supervisor_field_insert_mf_vec2f(WbFieldRef field, int index, const double values[2]);
void wb_supervisor_field_insert_mf_vec3f(WbFieldRef field, int index, const double values[3]);
void wb_supervisor_field_insert_mf_rotation(WbFieldRef field, int index, const double values[4]);
void wb_supervisor_field_insert_mf_color(WbFieldRef field, int index, const double values[3]);
void wb_supervisor_field_insert_mf_string(WbFieldRef field, int index, const char *value);

void wb_supervisor_field_remove_mf(WbFieldRef field, int index);

void wb_supervisor_field_import_mf_node(WbFieldRef field, int position, const char *filename);
void wb_supervisor_field_import_mf_node_from_string(WbFieldRef field, int position, const char *node_string);

void wb_supervisor_field_remove_sf(WbFieldRef field);

void wb_supervisor_field_import_sf_node(WbFieldRef field, const char *filename);
void wb_supervisor_field_import_sf_node_from_string(WbFieldRef field, const char *node_string);

bool wb_supervisor_virtual_reality_headset_is_used();
const double *wb_supervisor_virtual_reality_headset_get_position();
const double *wb_supervisor_virtual_reality_headset_get_orientation();

// Deprecated functions
// deprecated since Webots R2018b
void wb_supervisor_simulation_revert() WB_DEPRECATED;               // please use wb_supervisor_world_reload() instead
void wb_supervisor_load_world(const char *filename) WB_DEPRECATED;  // please use wb_supervisor_world_load() instead
bool wb_supervisor_save_world(const char *filename) WB_DEPRECATED;  // please use wb_supervisor_world_save() instead

// deprecated since Webots 8.6.0, plesae use wb_supervisor_field_remove_mf_item() instead
void wb_supervisor_field_remove_mf_node(WbFieldRef field, int position) WB_DEPRECATED;

// deprecated since Webots 8.0.0, plesae use wb_supervisor_simulation_reset_physics() instead
void wb_supervisor_simulation_physics_reset() WB_DEPRECATED;

// deprecated since Webots 8.4.0 please use wb_supervisor_movie_is_ready and wb_supervisor_movie_failed
#define WB_SUPERVISOR_MOVIE_READY 0
#define WB_SUPERVISOR_MOVIE_RECORDING 1
#define WB_SUPERVISOR_MOVIE_SAVING 2
#define WB_SUPERVISOR_MOVIE_WRITE_ERROR 3
#define WB_SUPERVISOR_MOVIE_ENCODING_ERROR 4
#define WB_SUPERVISOR_MOVIE_SIMULATION_ERROR 5
int wb_supervisor_movie_get_status();

// deprecated since webots 8.3.0: please use the wb_supervisor_movie_*() functions instead
void wb_supervisor_start_movie(const char *file, int width, int height, int codec, int quality, int acceleration,
                               bool caption) WB_DEPRECATED;
void wb_supervisor_stop_movie() WB_DEPRECATED;
int wb_supervisor_get_movie_status() WB_DEPRECATED;

#ifdef __cplusplus
}
#endif

#endif /* SUPERVISOR_H */
