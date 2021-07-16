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

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/nodes.h>
#include <webots/robot.h>
#include <webots/supervisor.h>
#include "device_private.h"
#include "file.h"
#include "messages.h"
#include "robot_private.h"
#include "supervisor_private.h"

enum FIELD_REQUEST_TYPE { GET = 1, SET, IMPORT, IMPORT_FROM_STRING, REMOVE };

static struct Label {
  int id;
  char *text;
  double x;
  double y;
  double size;
  unsigned int color;
  char *font;
  struct Label *next;
} *supervisor_label = NULL;

union WbFieldData {
  bool sf_bool;
  int sf_int32;
  double sf_float;
  double sf_vec2f[2];
  double sf_vec3f[3];
  double sf_rotation[4];
  char *sf_string;
  int sf_node_uid;  // 0 => NULL node
};

typedef struct WbFieldStructPrivate {
  const char *name;
  WbFieldType type;  // WB_SF_* or WB_MT_* as defined in supervisor.h
  int count;         // used in MF fields only
  int node_unique_id;
  int id;                  // attributed by Webots
  bool is_proto_internal;  // internal field can't be changed
  union WbFieldData data;
  WbFieldRef next;
  double last_update;
} WbFieldStruct;

typedef struct WbPoseStructPrivate {
  double pose[16];
  double last_update;
  WbNodeRef from_node;
  WbNodeRef to_node;
  struct WbPoseStructPrivate *next;
} WbPoseStruct;

typedef struct WbFieldRequestPrivate {
  enum FIELD_REQUEST_TYPE type;
  int index;
  bool is_string;
  union WbFieldData data;
  WbFieldStruct *field;
  struct WbFieldRequestPrivate *next;
} WbFieldRequest;

static WbFieldStruct *field_list = NULL;
static WbFieldRequest *field_requests_list_head = NULL;
static WbFieldRequest *field_requests_list_tail = NULL;
static WbFieldRequest *field_requests_garbage_list = NULL;
static WbFieldRequest *sent_field_get_request = NULL;
static bool is_field_immediate_message = false;

typedef struct WbNodeWbContactPointListStructPrivate {
  int n;
  WbContactPoint *points;
  double timestamp;  // TODO: Delete with `wb_supervisor_node_get_contact_point`
  int sampling_period;
  double last_update;
} WbNodeWbContactPointListStruct;

typedef struct WbNodeStructPrivate {
  int id;
  WbNodeType type;
  char *model_name;
  char *def_name;
  char *content;
  int parent_id;
  double *position;                                  // double[3]
  double *orientation;                               // double[9]
  double *center_of_mass;                            // double[3]
  WbNodeWbContactPointListStruct contact_points[2];  // 0 -> without descendants, 1 -> with descendants
  bool contact_points_include_descendants;           // TODO: Delete with `wb_supervisor_node_get_contact_point`
  bool static_balance;
  double *solid_velocity;  // double[6] (linear[3] + angular[3])
  bool is_proto;
  bool is_proto_internal;
  WbNodeRef parent_proto;
  int tag;
  WbNodeRef next;
} WbNodeStruct;

static WbNodeStruct *node_list = NULL;

typedef struct WbFieldChangeTrackingPrivate {
  WbFieldStruct *field;
  int sampling_period;
  bool enable;
} WbFieldChangeTracking;

typedef struct WbPoseChangeTrackingPrivate {
  WbNodeRef node;
  WbNodeRef from_node;
  int sampling_period;
  bool enable;
} WbPoseChangeTracking;

typedef struct WbWbContactPointChangeTrackingPrivate {
  WbNodeRef node;
  bool include_descendants;
  int sampling_period;
  bool enable;
} WbWbContactPointChangeTracking;

static const double invalid_vector[16] = {NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN};

// These functions may be used for debugging:
//
// static int compute_node_list_size() {
//   WbNodeStruct *node = node_list;
//   int count = 0;
//   while (node) {
//     node = node->next;
//     count++;
//   }
//   return count;
// }
//
// static int compute_field_list_size() {
//   WbFieldStruct *field = field_list;
//   int count = 0;
//   while (field) {
//     field = field->next;
//     count++;
//   }
//   return count;
// }

static char *supervisor_strdup(const char *src) {
  if (src == NULL)
    return NULL;
  const int l = strlen(src) + 1;
  char *dst = malloc(l);
  memcpy(dst, src, l);
  return dst;
}

// find field in field_list
static WbFieldStruct *find_field_by_name(const char *field_name, int node_id) {
  // TODO: Hash map needed
  WbFieldStruct *field = field_list;
  while (field) {
    if (field->node_unique_id == node_id && strcmp(field_name, field->name) == 0)
      return field;
    field = field->next;
  }
  return NULL;
}

static WbFieldStruct *find_field_by_id(int node_id, int field_id) {
  // TODO: Hash map needed
  WbFieldStruct *field = field_list;
  while (field) {
    if (field->node_unique_id == node_id && field->id == field_id)
      return field;
    field = field->next;
  }
  return NULL;
}

// find node in node_list
static WbNodeRef find_node_by_id(int id) {
  WbNodeRef node = node_list;
  while (node) {
    if (node->id == id)
      return node;
    node = node->next;
  }
  return NULL;
}

static WbNodeRef find_node_by_def(const char *def_name, WbNodeRef parent_proto) {
  WbNodeRef node = node_list;
  while (node) {
    if (node->parent_proto == parent_proto && (parent_proto || !node->is_proto_internal) && node->def_name &&
        strcmp(def_name, node->def_name) == 0)
      return node;
    node = node->next;
  }
  return NULL;
}

static WbNodeRef find_node_by_tag(int tag) {
  WbNodeRef node = node_list;
  while (node) {
    if (node->tag == tag)
      return node;
    node = node->next;
  }
  return NULL;
}

static bool is_node_ref_valid(WbNodeRef n) {
  if (!n)
    return false;

  WbNodeRef node = node_list;
  while (node) {
    if (node == n)
      return true;
    node = node->next;
  }
  return false;
}

static void delete_node(WbNodeRef node) {
  // clean the node
  free(node->model_name);
  free(node->def_name);
  free(node->content);
  free(node->position);
  free(node->orientation);
  free(node->center_of_mass);
  free(node->contact_points[0].points);
  free(node->contact_points[1].points);
  free(node->solid_velocity);
  free(node);
}

static void remove_node_from_list(int uid) {
  WbNodeRef node = find_node_by_id(uid);
  if (node) {  // make sure this node is in the list
    // look for the previous node in the list
    if (node_list == node)  // the node is the first of the list
      node_list = node->next;
    else {
      WbNodeRef previous_node_in_list = node_list;
      while (previous_node_in_list) {
        if (previous_node_in_list->next && previous_node_in_list->next->id == uid) {
          // connect previous and next node in the list
          previous_node_in_list->next = node->next;
          break;
        }
        previous_node_in_list = previous_node_in_list->next;
      }
    }
    delete_node(node);
  }

  WbNodeRef n = node_list;
  while (n) {
    if (n->parent_id == uid)
      n->parent_id = -1;
    n = n->next;
  }
}

// extract node DEF name from dot expression
static const char *extract_node_def(const char *def_name_expression) {
  if (!def_name_expression)
    return NULL;

  const unsigned int def_length = strlen(def_name_expression);
  if (def_length == 0)
    return "";

  // search for first '.' from back of def_name
  int i = 0;
  for (i = def_length - 1; i >= 0; --i) {
    if (def_name_expression[i] == '.')
      break;
  }

  // i = -1 || i == '.''s position, so use i + 1 to get first character position
  return (const char *)&(def_name_expression[i + 1]);
}

static void remove_internal_proto_nodes_and_fields_from_list() {
  WbNodeRef node = node_list;
  WbNodeRef previous_node = NULL;
  while (node) {
    if (node->is_proto_internal) {
      if (previous_node)
        previous_node->next = node->next;
      else
        node_list = node->next;
      WbNodeRef current_node = node;
      node = node->next;
      delete_node(current_node);
    } else {
      previous_node = node;
      node = node->next;
    }
  }

  WbFieldStruct *field = field_list;
  WbFieldStruct *previous_field = NULL;
  while (field) {
    if (field->is_proto_internal) {
      if (previous_field)
        previous_field->next = field->next;
      else
        field_list = field->next;
      WbFieldStruct *current_field = field;
      field = field->next;
      // clean the field
      if (current_field->type == WB_SF_STRING || current_field->type == WB_MF_STRING)
        free(current_field->data.sf_string);
      free((char *)current_field->name);
      free(current_field);
    } else {
      previous_field = field;
      field = field->next;
    }
  }
}

static void add_node_to_list(int uid, WbNodeType type, const char *model_name, const char *def_name, int tag, int parent_id,
                             bool is_proto) {
  WbNodeRef nodeInList = find_node_by_id(uid);
  if (nodeInList) {
    // already in the list, update DEF name if needed
    if (def_name && strcmp(nodeInList->def_name, def_name) != 0) {
      free(nodeInList->def_name);
      nodeInList->def_name = supervisor_strdup(extract_node_def(def_name));
    }
    return;
  }

  WbNodeRef n = malloc(sizeof(WbNodeStruct));
  n->id = uid;
  n->type = type;
  const char *base_name = wb_node_get_name(type);
  if (!base_name || !model_name || strcmp(base_name, model_name) == 0)
    n->model_name = NULL;
  else
    n->model_name = (char *)model_name;
  n->def_name = supervisor_strdup(extract_node_def(def_name));
  n->content = NULL;
  n->parent_id = parent_id;
  n->position = NULL;
  n->orientation = NULL;
  n->center_of_mass = NULL;
  n->contact_points[0].points = NULL;
  n->contact_points[0].n = 0;
  n->contact_points[0].timestamp = -1.0;
  n->contact_points[1].points = NULL;
  n->contact_points[1].timestamp = -1.0;
  n->contact_points[1].n = 0;
  n->contact_points_include_descendants = false;
  n->static_balance = false;
  n->solid_velocity = NULL;
  n->is_proto = is_proto;
  n->is_proto_internal = false;
  n->parent_proto = NULL;
  n->tag = tag;
  n->next = node_list;
  node_list = n;
}

static void clean_field_request_garbage_collector() {
  while (field_requests_garbage_list) {
    WbFieldRequest *request = field_requests_garbage_list;
    field_requests_garbage_list = request->next;
    if (request->is_string)
      free(request->data.sf_string);
    free(request);
  }
}

// Private fields
static WbPoseStruct *pose_collection;
static WbPoseStruct pose;
static bool pose_requested = false;
static WbFieldChangeTracking field_change_tracking;
static bool field_change_tracking_requested = false;
static WbPoseChangeTracking pose_change_tracking;
static bool pose_change_tracking_requested = false;
static char *export_image_filename = NULL;
static int export_image_quality = 0;
static bool simulation_quit = false;
static int simulation_quit_status = 0;
static bool simulation_reset = false;
static bool world_reload = false;
static bool simulation_reset_physics = false;
static bool simulation_change_mode = false;
static int imported_node_id = -1;
static const char *world_to_load = NULL;
static char movie_stop = false;
static char movie_status = WB_SUPERVISOR_MOVIE_READY;
static char *movie_filename = NULL;
static int movie_quality = 0;
static int movie_codec = 0;
static int movie_width = 0;
static int movie_height = 0;
static int movie_acceleration = 1;
static int movie_caption = false;
static char animation_stop = false;
static char *animation_filename = NULL;
static bool animation_start_status = true;
static bool animation_stop_status = true;
static bool save_status = true;
static bool save_request = false;
static char *save_filename = NULL;
static int node_id = -1;
static int node_tag = -1;
static WbNodeRef node_to_remove = NULL;
static bool allow_search_in_proto = false;
static const char *node_def_name = NULL;
static int proto_id = -1;
static const char *requested_field_name = NULL;
static bool requested_node_number_of_fields = false;
static int node_number_of_fields = -1;
static int requested_field_index = -1;
static bool node_get_selected = false;
static int node_ref = 0;
static WbNodeRef root_ref = NULL;
static WbNodeRef self_node_ref = NULL;
static WbNodeRef position_node_ref = NULL;
static WbNodeRef export_string_node_ref = NULL;
static WbNodeRef orientation_node_ref = NULL;
static WbNodeRef center_of_mass_node_ref = NULL;
static WbWbContactPointChangeTracking contact_point_change_tracking;
static bool contact_point_change_tracking_requested = false;
static WbNodeRef contact_points_node_ref = NULL;
static bool contact_points_include_descendants = false;
static WbNodeRef static_balance_node_ref = NULL;
static WbNodeRef reset_physics_node_ref = NULL;
static WbNodeRef restart_controller_node_ref = NULL;
static bool node_visible = true;
static WbNodeRef move_viewpoint_node_ref = NULL;
static WbNodeRef set_visibility_node_ref = NULL;
static WbNodeRef set_visibility_from_node_ref = NULL;
static WbNodeRef get_velocity_node_ref = NULL;
static WbNodeRef set_velocity_node_ref = NULL;
static const double *solid_velocity = NULL;
static WbNodeRef add_force_node_ref = NULL;
static WbNodeRef add_force_with_offset_node_ref = NULL;
static WbNodeRef add_torque_node_ref = NULL;
static const double *add_force_or_torque = NULL;
static bool add_force_or_torque_relative = false;
static const double *add_force_offset = NULL;
static WbNodeRef set_joint_node_ref = NULL;
static double set_joint_position = 0.0;
static int set_joint_index = 0;
static bool virtual_reality_headset_is_used_request = false;
static bool virtual_reality_headset_is_used = false;
static bool virtual_reality_headset_position_request = false;
static double *virtual_reality_headset_position = NULL;
static bool virtual_reality_headset_orientation_request = false;
static double *virtual_reality_headset_orientation = NULL;
static WbNodeRef save_node_state_node_ref = NULL;
static const char *save_node_state_name = NULL;
static WbNodeRef reset_node_state_node_ref = NULL;
static const char *reset_node_state_name = NULL;

static void supervisor_cleanup(WbDevice *d) {
  clean_field_request_garbage_collector();
  while (field_list) {
    WbFieldStruct *f = field_list->next;
    if (field_list->type == WB_SF_STRING || field_list->type == WB_MF_STRING)
      free(field_list->data.sf_string);
    free((char *)field_list->name);
    free(field_list);
    field_list = f;
  }
  while (field_requests_list_head) {
    WbFieldRequest *r = field_requests_list_head->next;
    if (field_requests_list_head->is_string)
      free(field_requests_list_head->data.sf_string);
    free(field_requests_list_head);
    field_requests_list_head = r;
  }
  field_requests_list_tail = NULL;
  if (sent_field_get_request) {
    if (sent_field_get_request->is_string)
      free(sent_field_get_request->data.sf_string);
    free(sent_field_get_request);
    sent_field_get_request = NULL;
  }
  while (pose_collection) {
    WbPoseStruct *r = pose_collection->next;
    free(pose_collection);
    pose_collection = r;
  }
  while (node_list) {
    WbNodeStruct *n = node_list->next;
    delete_node(node_list);
    node_list = n;
  }

  free(export_image_filename);
  free(animation_filename);
  free(movie_filename);
  free(save_filename);
}

static void supervisor_write_request(WbDevice *d, WbRequest *r) {
  // chain with base class
  robot_write_request(d, r);

  if (simulation_change_mode) {
    request_write_uchar(r, C_SUPERVISOR_SIMULATION_CHANGE_MODE);
    request_write_int32(r, robot_get_simulation_mode());
    simulation_change_mode = false;
  } else if (simulation_quit) {
    request_write_uchar(r, C_SUPERVISOR_SIMULATION_QUIT);
    request_write_int32(r, simulation_quit_status);
    simulation_quit = false;
  } else if (simulation_reset) {
    request_write_uchar(r, C_SUPERVISOR_SIMULATION_RESET);
    simulation_reset = false;
  } else if (world_reload) {
    request_write_uchar(r, C_SUPERVISOR_RELOAD_WORLD);
    world_reload = false;
  } else if (simulation_reset_physics) {
    request_write_uchar(r, C_SUPERVISOR_SIMULATION_RESET_PHYSICS);
    simulation_reset_physics = false;
  } else if (world_to_load) {
    request_write_uchar(r, C_SUPERVISOR_LOAD_WORLD);
    request_write_string(r, world_to_load);
    world_to_load = NULL;
  }

  if (node_id >= 0) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_FROM_ID);
    request_write_int32(r, node_id);
  } else if (node_def_name) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_FROM_DEF);
    request_write_string(r, node_def_name);
    request_write_int32(r, proto_id);
  } else if (node_tag > 0) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_FROM_TAG);
    request_write_int32(r, node_tag);
  } else if (node_get_selected) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_SELECTED);
  } else if (requested_field_name) {
    request_write_uchar(r, C_SUPERVISOR_FIELD_GET_FROM_NAME);
    request_write_uint32(r, node_ref);
    request_write_string(r, requested_field_name);
    request_write_uchar(r, allow_search_in_proto ? 1 : 0);
  } else if (requested_field_index >= 0) {
    request_write_uchar(r, C_SUPERVISOR_FIELD_GET_FROM_INDEX);
    request_write_uint32(r, node_ref);
    request_write_uint32(r, requested_field_index);
    request_write_uchar(r, allow_search_in_proto ? 1 : 0);
  } else if (requested_node_number_of_fields) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_FIELD_COUNT);
    request_write_uint32(r, node_ref);
    request_write_uchar(r, allow_search_in_proto ? 1 : 0);
  } else if (pose_change_tracking_requested) {
    request_write_uchar(r, C_SUPERVISOR_POSE_CHANGE_TRACKING_STATE);
    request_write_int32(r, pose_change_tracking.from_node ? pose_change_tracking.from_node->id : 0);
    request_write_int32(r, pose_change_tracking.node->id);
    request_write_uchar(r, pose_change_tracking.enable);
    if (pose_change_tracking.enable)
      request_write_int32(r, pose_change_tracking.sampling_period);
  } else if (field_change_tracking_requested) {
    request_write_uchar(r, C_SUPERVISOR_FIELD_CHANGE_TRACKING_STATE);
    request_write_int32(r, field_change_tracking.field->node_unique_id);
    request_write_int32(r, field_change_tracking.field->id);
    request_write_uchar(r, field_change_tracking.field->is_proto_internal ? 1 : 0);
    request_write_uchar(r, field_change_tracking.enable);
    if (field_change_tracking.enable)
      request_write_int32(r, field_change_tracking.sampling_period);
  } else if (!robot_is_immediate_message() || is_field_immediate_message) {
    is_field_immediate_message = false;
    WbFieldRequest *request;
    if (sent_field_get_request)
      request = sent_field_get_request;
    else {
      request = field_requests_list_head;
      field_requests_list_tail = NULL;
      field_requests_list_head = NULL;
    }
    while (request) {
      WbFieldStruct *f = request->field;
      if (request->type == GET) {
        request_write_uchar(r, C_SUPERVISOR_FIELD_GET_VALUE);
        request_write_uint32(r, f->node_unique_id);
        request_write_uint32(r, f->id);
        request_write_uchar(r, f->is_proto_internal ? 1 : 0);
        if (request->index != -1)
          request_write_uint32(r, request->index);  // MF fields only
      } else if (request->type == SET) {
        request_write_uchar(r, C_SUPERVISOR_FIELD_SET_VALUE);
        request_write_uint32(r, f->node_unique_id);
        request_write_uint32(r, f->id);
        request_write_uint32(r, f->type);
        request_write_uint32(r, request->index);
        switch (f->type) {
          case WB_SF_BOOL:
          case WB_MF_BOOL:
            request_write_uchar(r, request->data.sf_bool ? 1 : 0);
            break;
          case WB_SF_INT32:
          case WB_MF_INT32:
            request_write_int32(r, request->data.sf_int32);
            break;
          case WB_SF_FLOAT:
          case WB_MF_FLOAT:
            request_write_double(r, request->data.sf_float);
            break;
          case WB_SF_VEC2F:
          case WB_MF_VEC2F:
            request_write_double(r, request->data.sf_vec2f[0]);
            request_write_double(r, request->data.sf_vec2f[1]);
            break;
          case WB_SF_VEC3F:
          case WB_MF_VEC3F:
          case WB_SF_COLOR:
          case WB_MF_COLOR:
            request_write_double(r, request->data.sf_vec3f[0]);
            request_write_double(r, request->data.sf_vec3f[1]);
            request_write_double(r, request->data.sf_vec3f[2]);
            break;
          case WB_SF_ROTATION:
          case WB_MF_ROTATION:
            request_write_double(r, request->data.sf_rotation[0]);
            request_write_double(r, request->data.sf_rotation[1]);
            request_write_double(r, request->data.sf_rotation[2]);
            request_write_double(r, request->data.sf_rotation[3]);
            break;
          case WB_SF_STRING:
          case WB_MF_STRING:
            assert(request->data.sf_string);
            request_write_string(r, request->data.sf_string);
            break;
          default:
            assert(0);
        }
      } else if (request->type == IMPORT) {
        request_write_uchar(r, C_SUPERVISOR_FIELD_INSERT_VALUE);
        request_write_uint32(r, f->node_unique_id);
        request_write_uint32(r, f->id);
        request_write_uint32(r, request->index);
        switch (f->type) {
          case WB_MF_BOOL:
            request_write_uchar(r, request->data.sf_bool ? 1 : 0);
            break;
          case WB_MF_INT32:
            request_write_int32(r, request->data.sf_int32);
            break;
          case WB_MF_FLOAT:
            request_write_double(r, request->data.sf_float);
            break;
          case WB_MF_VEC2F:
            request_write_double(r, request->data.sf_vec2f[0]);
            request_write_double(r, request->data.sf_vec2f[1]);
            break;
          case WB_MF_VEC3F:
          case WB_MF_COLOR:
            request_write_double(r, request->data.sf_vec3f[0]);
            request_write_double(r, request->data.sf_vec3f[1]);
            request_write_double(r, request->data.sf_vec3f[2]);
            break;
          case WB_MF_ROTATION:
            request_write_double(r, request->data.sf_rotation[0]);
            request_write_double(r, request->data.sf_rotation[1]);
            request_write_double(r, request->data.sf_rotation[2]);
            request_write_double(r, request->data.sf_rotation[3]);
            break;
          case WB_MF_STRING:
            request_write_string(r, request->data.sf_string);
            break;
          case WB_MF_NODE:
            request_write_string(r, request->data.sf_string);
            break;
          default:
            assert(false);
        }
      } else if (request->type == IMPORT_FROM_STRING) {
        request_write_uchar(r, C_SUPERVISOR_FIELD_IMPORT_NODE_FROM_STRING);
        request_write_uint32(r, f->node_unique_id);
        request_write_uint32(r, f->id);
        request_write_uint32(r, request->index);
        request_write_string(r, request->data.sf_string);
      } else if (request->type == REMOVE) {
        request_write_uchar(r, C_SUPERVISOR_FIELD_REMOVE_VALUE);
        request_write_uint32(r, f->node_unique_id);
        request_write_uint32(r, f->id);
        request_write_uint32(r, request->index);
      } else
        assert(false);
      if (request->type != GET) {
        // add request to garbage collector so that it is deleted later
        WbFieldRequest *next = request->next;
        request->next = field_requests_garbage_list;
        field_requests_garbage_list = request;
        request = next;
      } else {
        // get requests are handled immediately, so only one request has to be sent at a time
        // request is required when getting back the answer from Webots
        assert(sent_field_get_request == request);
        assert(request->next == NULL);
        request = request->next;
      }
    }
  }
  while (supervisor_label) {
    request_write_uchar(r, C_SUPERVISOR_SET_LABEL);
    request_write_uint16(r, supervisor_label->id);
    request_write_double(r, supervisor_label->x);
    request_write_double(r, supervisor_label->y);
    request_write_double(r, supervisor_label->size);
    request_write_uint32(r, supervisor_label->color);
    request_write_string(r, supervisor_label->text);
    request_write_string(r, supervisor_label->font);
    free(supervisor_label->text);
    free(supervisor_label->font);
    struct Label *old_label = supervisor_label;
    supervisor_label = supervisor_label->next;
    free(old_label);
  }
  if (node_to_remove) {
    request_write_uchar(r, C_SUPERVISOR_NODE_REMOVE_NODE);
    request_write_uint32(r, node_to_remove->id);
    node_to_remove = NULL;
  }
  if (position_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_POSITION);
    request_write_uint32(r, position_node_ref->id);
  }
  if (orientation_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_ORIENTATION);
    request_write_uint32(r, orientation_node_ref->id);
  }
  if (pose_requested) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_POSE);
    request_write_uint32(r, pose.from_node ? pose.from_node->id : 0);
    request_write_uint32(r, pose.to_node->id);
  }
  if (center_of_mass_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_CENTER_OF_MASS);
    request_write_uint32(r, center_of_mass_node_ref->id);
  }
  if (contact_point_change_tracking_requested) {
    request_write_uchar(r, C_SUPERVISOR_CONTACT_POINTS_CHANGE_TRACKING_STATE);
    request_write_uint32(r, contact_point_change_tracking.node->id);
    request_write_uchar(r, contact_point_change_tracking.include_descendants ? 1 : 0);
    request_write_uchar(r, contact_point_change_tracking.enable);
    if (contact_point_change_tracking.enable)
      request_write_int32(r, contact_point_change_tracking.sampling_period);
  }
  if (contact_points_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_CONTACT_POINTS);
    request_write_uint32(r, contact_points_node_ref->id);
    request_write_uchar(r, contact_points_include_descendants ? 1 : 0);
  }
  if (static_balance_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_STATIC_BALANCE);
    request_write_uint32(r, static_balance_node_ref->id);
  }
  if (get_velocity_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_GET_VELOCITY);
    request_write_uint32(r, get_velocity_node_ref->id);
  }
  if (set_velocity_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_SET_VELOCITY);
    request_write_uint32(r, set_velocity_node_ref->id);
    request_write_double(r, solid_velocity[0]);
    request_write_double(r, solid_velocity[1]);
    request_write_double(r, solid_velocity[2]);
    request_write_double(r, solid_velocity[3]);
    request_write_double(r, solid_velocity[4]);
    request_write_double(r, solid_velocity[5]);
  }
  if (export_string_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_EXPORT_STRING);
    request_write_uint32(r, export_string_node_ref->id);
  }
  if (reset_physics_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_RESET_PHYSICS);
    request_write_uint32(r, reset_physics_node_ref->id);
  }
  if (restart_controller_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_RESTART_CONTROLLER);
    request_write_uint32(r, restart_controller_node_ref->id);
  }
  if (set_visibility_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_SET_VISIBILITY);
    request_write_uint32(r, set_visibility_node_ref->id);
    request_write_uint32(r, set_visibility_from_node_ref->id);
    request_write_uchar(r, node_visible ? 1 : 0);
  }
  if (move_viewpoint_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_MOVE_VIEWPOINT);
    request_write_uint32(r, move_viewpoint_node_ref->id);
  }
  if (add_force_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_ADD_FORCE);
    request_write_uint32(r, add_force_node_ref->id);
    request_write_double(r, add_force_or_torque[0]);
    request_write_double(r, add_force_or_torque[1]);
    request_write_double(r, add_force_or_torque[2]);
    request_write_uchar(r, add_force_or_torque_relative ? 1 : 0);
  }
  if (add_force_with_offset_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_ADD_FORCE_WITH_OFFSET);
    request_write_uint32(r, add_force_with_offset_node_ref->id);
    request_write_double(r, add_force_or_torque[0]);
    request_write_double(r, add_force_or_torque[1]);
    request_write_double(r, add_force_or_torque[2]);
    request_write_double(r, add_force_offset[0]);
    request_write_double(r, add_force_offset[1]);
    request_write_double(r, add_force_offset[2]);
    request_write_uchar(r, add_force_or_torque_relative ? 1 : 0);
  }
  if (add_torque_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_ADD_TORQUE);
    request_write_uint32(r, add_torque_node_ref->id);
    request_write_double(r, add_force_or_torque[0]);
    request_write_double(r, add_force_or_torque[1]);
    request_write_double(r, add_force_or_torque[2]);
    request_write_uchar(r, add_force_or_torque_relative ? 1 : 0);
  }
  if (export_image_filename) {
    request_write_uchar(r, C_SUPERVISOR_EXPORT_IMAGE);
    request_write_uchar(r, export_image_quality);
    request_write_string(r, export_image_filename);
    free(export_image_filename);
    export_image_filename = NULL;
  }
  if (movie_filename) {
    request_write_uchar(r, C_SUPERVISOR_START_MOVIE);
    request_write_int32(r, movie_width);
    request_write_int32(r, movie_height);
    request_write_uchar(r, movie_codec);
    request_write_uchar(r, movie_quality);
    request_write_uchar(r, movie_acceleration);
    request_write_uchar(r, movie_caption ? 1 : 0);
    request_write_string(r, movie_filename);
    free(movie_filename);
    movie_filename = NULL;
  }
  if (movie_stop) {
    request_write_uchar(r, C_SUPERVISOR_STOP_MOVIE);
    movie_stop = false;
  }
  if (animation_filename) {
    request_write_uchar(r, C_SUPERVISOR_START_ANIMATION);
    request_write_string(r, animation_filename);
    free(animation_filename);
    animation_filename = NULL;
  }
  if (animation_stop) {
    request_write_uchar(r, C_SUPERVISOR_STOP_ANIMATION);
    animation_stop = false;
  }
  if (save_request) {
    request_write_uchar(r, C_SUPERVISOR_SAVE_WORLD);
    request_write_uchar(r, save_filename ? 1 : 0);
    if (save_filename) {
      request_write_string(r, save_filename);
      free(save_filename);
      save_filename = NULL;
    }
    save_request = false;
  }
  if (virtual_reality_headset_is_used_request)
    request_write_uchar(r, C_SUPERVISOR_VIRTUAL_REALITY_HEADSET_IS_USED);
  if (virtual_reality_headset_position_request)
    request_write_uchar(r, C_SUPERVISOR_VIRTUAL_REALITY_HEADSET_GET_POSITION);
  if (virtual_reality_headset_orientation_request)
    request_write_uchar(r, C_SUPERVISOR_VIRTUAL_REALITY_HEADSET_GET_ORIENTATION);
  if (save_node_state_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_SAVE_STATE);
    request_write_uint32(r, save_node_state_node_ref->id);
    request_write_string(r, save_node_state_name);
  }
  if (reset_node_state_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_RESET_STATE);
    request_write_uint32(r, reset_node_state_node_ref->id);
    request_write_string(r, reset_node_state_name);
  }
  if (set_joint_node_ref) {
    request_write_uchar(r, C_SUPERVISOR_NODE_SET_JOINT_POSITION);
    request_write_uint32(r, set_joint_node_ref->id);
    request_write_double(r, set_joint_position);
    request_write_uint32(r, set_joint_index);
  }
}

static void supervisor_read_answer(WbDevice *d, WbRequest *r) {
  int i;

  switch (request_read_uchar(r)) {
    case C_CONFIGURE: {
      const int self_uid = request_read_uint32(r);
      const int parent_uid = request_read_uint32(r);
      const bool is_proto = request_read_uchar(r) == 1;
      const bool is_proto_internal = request_read_uchar(r) == 1;
      const char *model_name = request_read_string(r);
      const char *def_name = request_read_string(r);
      add_node_to_list(self_uid, WB_NODE_ROBOT, model_name, def_name, 0, parent_uid, is_proto);  // add self node
      self_node_ref = node_list;
      self_node_ref->is_proto_internal = is_proto_internal;
    } break;
    case C_SUPERVISOR_NODE_GET_FROM_DEF: {
      const int uid = request_read_uint32(r);
      const WbNodeType type = request_read_uint32(r);
      const int tag = request_read_int32(r);
      const int parent_uid = request_read_uint32(r);
      const bool is_proto = request_read_uchar(r) == 1;
      const char *model_name = request_read_string(r);
      if (uid) {
        add_node_to_list(uid, type, model_name, node_def_name, tag, parent_uid, is_proto);
        node_id = uid;
      }
    } break;
    case C_SUPERVISOR_NODE_GET_SELECTED:
    case C_SUPERVISOR_NODE_GET_FROM_ID:
    case C_SUPERVISOR_NODE_GET_FROM_TAG: {
      const int uid = request_read_uint32(r);
      const WbNodeType type = request_read_uint32(r);
      const int tag = request_read_int32(r);
      const int parent_uid = request_read_uint32(r);
      const bool is_proto = request_read_uchar(r) == 1;
      const bool is_proto_internal = request_read_uchar(r) == 1;
      const char *model_name = request_read_string(r);
      const char *def_name = request_read_string(r);
      if (uid && (!is_proto_internal || allow_search_in_proto)) {
        add_node_to_list(uid, type, model_name, def_name, tag, parent_uid, is_proto);
        node_id = uid;
      }
    } break;
    case C_SUPERVISOR_FIELD_GET_FROM_INDEX:
    case C_SUPERVISOR_FIELD_GET_FROM_NAME: {
      const int field_ref = request_read_int32(r);
      const WbFieldType field_type = request_read_int32(r);
      const bool is_proto_internal = request_read_uchar(r) == 1;
      const int field_count = ((field_type & WB_MF) == WB_MF) ? request_read_int32(r) : -1;
      const char *name = request_read_string(r);
      if (field_ref == -1) {
        requested_field_name = NULL;
        break;
      }
      WbFieldStruct *f = malloc(sizeof(WbFieldStruct));
      f->next = field_list;
      f->id = field_ref;
      f->type = field_type;
      f->count = field_count;
      f->node_unique_id = node_ref;
      f->name = name;
      f->is_proto_internal = is_proto_internal;
      f->last_update = -DBL_MAX;
      f->data.sf_string = NULL;
      field_list = f;
    } break;
    case C_SUPERVISOR_FIELD_GET_VALUE: {
      const WbFieldType field_type = request_read_int32(r);

      // field_type == 0 if node was deleted
      if (field_type != 0) {
        const int field_node_id = request_read_int32(r);
        const int field_id = request_read_int32(r);
        const bool is_field_get_request = sent_field_get_request && sent_field_get_request->field &&
                                          sent_field_get_request->field->node_unique_id == field_node_id &&
                                          sent_field_get_request->field->id == field_id;

        WbFieldStruct *f = (is_field_get_request) ? sent_field_get_request->field : find_field_by_id(field_node_id, field_id);
        if (f) {
          switch (f->type) {
            case WB_SF_BOOL:
            case WB_MF_BOOL:
              f->data.sf_bool = request_read_uchar(r) == 1;
              break;
            case WB_SF_INT32:
            case WB_MF_INT32:
              f->data.sf_int32 = request_read_int32(r);
              break;
            case WB_SF_FLOAT:
            case WB_MF_FLOAT:
              f->data.sf_float = request_read_double(r);
              break;
            case WB_SF_VEC2F:
            case WB_MF_VEC2F:
              f->data.sf_vec2f[0] = request_read_double(r);
              f->data.sf_vec2f[1] = request_read_double(r);
              break;
            case WB_SF_VEC3F:
            case WB_MF_VEC3F:
            case WB_SF_COLOR:
            case WB_MF_COLOR:
              f->data.sf_vec3f[0] = request_read_double(r);
              f->data.sf_vec3f[1] = request_read_double(r);
              f->data.sf_vec3f[2] = request_read_double(r);
              break;
            case WB_SF_ROTATION:
            case WB_MF_ROTATION:
              f->data.sf_rotation[0] = request_read_double(r);
              f->data.sf_rotation[1] = request_read_double(r);
              f->data.sf_rotation[2] = request_read_double(r);
              f->data.sf_rotation[3] = request_read_double(r);
              break;
            case WB_SF_STRING:
            case WB_MF_STRING:
              free(f->data.sf_string);
              f->data.sf_string = request_read_string(r);
              break;
            case WB_SF_NODE:
            case WB_MF_NODE:
              f->data.sf_node_uid = request_read_uint32(r);  // 0 => NULL node
              if (f->data.sf_node_uid) {
                const WbNodeType type = request_read_uint32(r);
                const int tag = request_read_int32(r);
                const int parent_uid = request_read_uint32(r);
                const bool is_proto = request_read_uchar(r) == 1;
                const char *model_name = request_read_string(r);
                const char *def_name = request_read_string(r);
                add_node_to_list(f->data.sf_node_uid, type, model_name, def_name, tag, parent_uid, is_proto);
              }
              break;
            default:
              assert(0);
          }
          f->last_update = wb_robot_get_time();
        }
      }
      if (sent_field_get_request) {
        if (sent_field_get_request->is_string)
          free(sent_field_get_request->data.sf_string);
        free(sent_field_get_request);
        sent_field_get_request = NULL;
      }
      break;
    }
    case C_SUPERVISOR_NODE_GET_FIELD_COUNT:
      node_number_of_fields = request_read_int32(r);
      break;
    case C_SUPERVISOR_NODE_REGENERATED:
      remove_internal_proto_nodes_and_fields_from_list();
      break;
    case C_SUPERVISOR_FIELD_INSERT_VALUE:
      imported_node_id = request_read_int32(r);
      break;
    case C_SUPERVISOR_FIELD_COUNT_CHANGED: {
      const int parent_node_id = request_read_int32(r);
      const char *field_name = request_read_string(r);
      const int field_count = request_read_int32(r);
      if (parent_node_id >= 0) {
        WbFieldStruct *field = find_field_by_name(field_name, parent_node_id);
        if (field)
          field->count = field_count;
      }
      break;
    }
    case C_SUPERVISOR_NODE_REMOVE_NODE:
      // Remove the deleted node from the internal reference list
      remove_node_from_list(request_read_uint32(r));
      break;
    case C_SUPERVISOR_NODE_GET_POSITION:
      free(position_node_ref->position);
      position_node_ref->position = malloc(3 * sizeof(double));
      for (i = 0; i < 3; i++)
        position_node_ref->position[i] = request_read_double(r);
      break;
    case C_SUPERVISOR_NODE_EXPORT_STRING:
      free(export_string_node_ref->content);
      export_string_node_ref->content = request_read_string(r);
      break;
    case C_SUPERVISOR_NODE_GET_ORIENTATION:
      free(orientation_node_ref->orientation);
      orientation_node_ref->orientation = malloc(9 * sizeof(double));
      for (i = 0; i < 9; i++)
        orientation_node_ref->orientation[i] = request_read_double(r);
      break;
    case C_SUPERVISOR_NODE_GET_POSE: {
      const int from_node_id = request_read_int32(r);
      const int to_node_id = request_read_int32(r);
      double *node_pose = NULL;
      if (pose_requested && pose.to_node->id == to_node_id &&
          ((!pose.from_node && !from_node_id) || pose.from_node->id == from_node_id))
        node_pose = pose.pose;
      else {
        WbPoseStruct *tmp_pose = pose_collection;
        while (tmp_pose) {
          if (tmp_pose->to_node->id == to_node_id && (!tmp_pose->from_node || tmp_pose->from_node->id == from_node_id)) {
            node_pose = tmp_pose->pose;
            tmp_pose->last_update = wb_robot_get_time();
            break;
          }
          tmp_pose = tmp_pose->next;
        }
      }
      if (!node_pose)
        node_pose = pose.pose;
      for (i = 0; i < 16; i++)
        node_pose[i] = request_read_double(r);
      break;
    }
    case C_SUPERVISOR_NODE_GET_CENTER_OF_MASS:
      free(center_of_mass_node_ref->center_of_mass);
      center_of_mass_node_ref->center_of_mass = malloc(3 * sizeof(double));
      for (i = 0; i < 3; i++)
        center_of_mass_node_ref->center_of_mass[i] = request_read_double(r);
      break;
    case C_SUPERVISOR_NODE_GET_CONTACT_POINTS: {
      const int contact_point_node_id = request_read_int32(r);
      const int include_descendants = (int)request_read_uchar(r);

      WbNodeRef contact_point_node = NULL;
      if (contact_points_node_ref && contact_points_node_ref->id == contact_point_node_id &&
          contact_points_node_ref->contact_points_include_descendants == include_descendants)
        contact_point_node = contact_points_node_ref;
      else {
        WbNodeRef tmp_node = node_list;
        while (tmp_node) {
          if (tmp_node->id == contact_point_node_id) {
            contact_point_node = tmp_node;
            break;
          }
          tmp_node = tmp_node->next;
        }
      }
      assert(contact_point_node);

      const int n_points = request_read_int32(r);

      free(contact_point_node->contact_points[include_descendants].points);
      contact_point_node->contact_points[include_descendants].points = NULL;
      contact_point_node->contact_points[include_descendants].n = n_points;
      if (n_points > 0) {
        WbContactPoint *points = malloc(n_points * sizeof(WbContactPoint));
        contact_point_node->contact_points[include_descendants].points = points;
        for (i = 0; i < n_points; i++) {
          points[i].point[0] = request_read_double(r);
          points[i].point[1] = request_read_double(r);
          points[i].point[2] = request_read_double(r);
          points[i].node_id = request_read_int32(r);
        }
      }
      contact_point_node->contact_points[include_descendants].last_update = wb_robot_get_time();
      break;
    }
    case C_SUPERVISOR_NODE_GET_STATIC_BALANCE:
      static_balance_node_ref->static_balance = request_read_uchar(r) == 1;
      break;
    case C_SUPERVISOR_NODE_GET_VELOCITY:
      free(get_velocity_node_ref->solid_velocity);
      get_velocity_node_ref->solid_velocity = malloc(6 * sizeof(double));
      for (i = 0; i < 6; i++)
        get_velocity_node_ref->solid_velocity[i] = request_read_double(r);
      break;
    case C_SUPERVISOR_ANIMATION_START_STATUS:
      animation_start_status = request_read_uchar(r);
      break;
    case C_SUPERVISOR_ANIMATION_STOP_STATUS:
      animation_stop_status = request_read_uchar(r);
      break;
    case C_SUPERVISOR_MOVIE_STATUS:
      movie_status = request_read_uchar(r);
      break;
    case C_SUPERVISOR_SAVE_WORLD:
      save_status = request_read_uchar(r);
      break;
    case C_SUPERVISOR_VIRTUAL_REALITY_HEADSET_IS_USED:
      virtual_reality_headset_is_used = request_read_uchar(r) == 1;
      break;
    case C_SUPERVISOR_VIRTUAL_REALITY_HEADSET_GET_POSITION:
      virtual_reality_headset_position = malloc(3 * sizeof(double));
      for (i = 0; i < 3; i++)
        virtual_reality_headset_position[i] = request_read_double(r);
      break;
    case C_SUPERVISOR_VIRTUAL_REALITY_HEADSET_GET_ORIENTATION:
      virtual_reality_headset_orientation = malloc(9 * sizeof(double));
      for (i = 0; i < 9; i++)
        virtual_reality_headset_orientation[i] = request_read_double(r);
      break;
    default:
      r->pointer--;  // unread last value
      robot_read_answer(NULL, r);
      break;
  }
  // free requests previously sent to Webots
  // cannot be freed immediately because the string memory pointer is used to build the message
  clean_field_request_garbage_collector();
}

static void create_and_append_field_request(WbFieldStruct *f, int action, int index, union WbFieldData data, bool clamp_index) {
  if (clamp_index) {
    int offset = 0;
    if (action == IMPORT || action == IMPORT_FROM_STRING)
      offset = 1;
    if (f->count != -1 && (index >= (f->count + offset) || index < 0)) {
      index = 0;
      fprintf(stderr, "Warning wb_supervisor_field_get/set_mf_*() called with index out of range.\n");
    }
  }
  WbFieldRequest *request = malloc(sizeof(WbFieldRequest));
  request->type = action;
  request->index = index;
  request->data = data;
  request->is_string = f->type == WB_SF_STRING || f->type == WB_MF_STRING || action == IMPORT_FROM_STRING ||
                       (action == IMPORT && f->type == WB_MF_NODE);
  request->field = f;
  request->next = NULL;
  if (request->type == GET)
    sent_field_get_request = request;
  else if (field_requests_list_tail) {
    // append
    field_requests_list_tail->next = request;
    field_requests_list_tail = request;
  } else {
    field_requests_list_tail = request;
    field_requests_list_head = field_requests_list_tail;
  }
  is_field_immediate_message = request->type != SET;  // set operations are postponed
}

static void field_operation_with_data(WbFieldStruct *f, int action, int index, union WbFieldData data) {
  robot_mutex_lock_step();
  WbFieldRequest *r;
  for (r = field_requests_list_head; r; r = r->next) {
    if (r->field == f && r->type == SET && r->index == index) {
      if (action == GET) {
        if (!r->is_string)
          f->data = r->data;
        else {
          free(f->data.sf_string);
          f->data.sf_string = supervisor_strdup(r->data.sf_string);
        }
      } else if (action == SET) {
        if (!r->is_string)
          r->data = data;
        else {
          free(r->data.sf_string);
          r->data.sf_string = data.sf_string;
          f->data.sf_string = NULL;
        }
      }
      robot_mutex_unlock_step();
      return;
    }
  }
  // If a field tracking is used we don't have to send the request
  if (action == GET && f->count == -1 && f->last_update == wb_robot_get_time()) {
    robot_mutex_unlock_step();
    return;
  }
  assert(action != GET || sent_field_get_request == NULL);  // get requests have to be processed immediately so no
                                                            // pending get request should remain
  create_and_append_field_request(f, action, index, data, true);
  if (action != SET)  // Only setter can be postponed. The getter, import and remove actions have to be applied immediately.
    wb_robot_flush_unlocked();
  assert(action != GET || sent_field_get_request == NULL);
  robot_mutex_unlock_step();
}

static void field_operation(WbFieldStruct *f, int action, int index) {
  union WbFieldData data;
  data.sf_string = NULL;
  field_operation_with_data(f, action, index, data);
}

static bool check_field(WbFieldRef f, const char *func, WbFieldType type, bool check_type, int *index, bool is_importing,
                        bool check_type_internal) {
  if (!robot_check_supervisor(func))
    return false;

  if (!f) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with NULL 'field' argument.\n", func);
    return false;
  }

  // check field reference is valid
  WbFieldStruct *field = field_list;
  bool found = false;
  while (field) {
    if (field == f) {
      found = true;
      break;
    }
    field = field->next;
  }
  if (!found) {
    fprintf(stderr, "Error: %s() called with invalid 'field' argument.\n", func);
    return false;
  }

  if (check_type_internal && ((WbFieldStruct *)f)->is_proto_internal) {
    fprintf(stderr, "Error: %s() called on a read-only PROTO internal field.\n", func);
    return false;
  }

  if (check_type && ((WbFieldStruct *)f)->type != type) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with wrong field type: %s.\n", func, wb_supervisor_field_get_type_name(f));
    return false;
  }

  if (type & WB_MF) {
    assert(index != NULL);
    int count = ((WbFieldStruct *)f)->count;
    int offset = is_importing ? 0 : -1;

    if (*index < -(count + 1 + offset) || *index > (count + offset)) {
      fprintf(stderr, "Error: %s() called with an out-of-bound index: %d (should be between %d and %d).\n", func, *index,
              -count - 1 - offset, count + offset);
      return false;
    }

    // resolve negative index value
    if (*index < 0)
      *index += count + 1 + offset;
  }
  return true;
}

static bool check_float(const char *function, double value) {
  if (isnan(value)) {
    fprintf(stderr, "Error: %s() called with a NaN value.\n", function);
    return false;
  }
  if (value > FLT_MAX) {
    fprintf(stderr, "Error: %s() called with a value greater than FLX_MAX: %g > %g.\n", function, value, FLT_MAX);
    return false;
  }
  if (value < -FLT_MAX) {
    fprintf(stderr, "Error: %s() called with a value smaller than -FLX_MAX): %g < %g.\n", function, value, -FLT_MAX);
    return false;
  }
  return true;
}

static bool check_vector(const char *function, const double values[], int n) {
  if (!values) {
    fprintf(stderr, "Error: %s() called with NULL argument.\n", function);
    return false;
  }
  int i;
  for (i = 0; i < n; i++) {
    if (!check_float(function, values[i]))
      return false;
  }
  return true;  // ok
}

// Protected functions

void wb_supervisor_init(WbDevice *d) {
  d->write_request = supervisor_write_request;
  d->read_answer = supervisor_read_answer;
  d->cleanup = supervisor_cleanup;
  add_node_to_list(0, WB_NODE_GROUP, wb_node_get_name(WB_NODE_GROUP), NULL, 0, -1, false);  // create root node
  root_ref = node_list;
}

// Public functions available from the user API

void wb_supervisor_set_label(int id, const char *text, double x, double y, double size, int color, double transparency,
                             const char *font) {
  unsigned int color_and_transparency = (unsigned int)color;
  color_and_transparency += (unsigned int)(transparency * 0xff) << 24;

  if (x < 0 || x > 1) {
    fprintf(stderr, "Error: %s() called with x parameter outside of [0,1] range.\n", __FUNCTION__);
    return;
  }

  if (y < 0 || y > 1) {
    fprintf(stderr, "Error: %s() called with y parameter outside of [0,1] range.\n", __FUNCTION__);
    return;
  }

  if (size < 0 || size > 1) {
    fprintf(stderr, "Error: %s() called with size parameter outside of [0,1] range.\n", __FUNCTION__);
    return;
  }

  if (transparency < 0 || transparency > 1) {
    fprintf(stderr,
            "Error: %s() called with transparency parameter outside of [0,1] "
            "range.\n",
            __FUNCTION__);
    return;
  }

  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!text) {
    fprintf(stderr, "Error: %s() called with NULL 'text' argument.\n", __FUNCTION__);
    return;
  }

  if (!font) {
    fprintf(stderr, "Error: %s() called with NULL 'font' argument.\n", __FUNCTION__);
    return;
  }

  struct Label *l;
  robot_mutex_lock_step();
  for (l = supervisor_label; l; l = l->next) {
    if (l->id == id) {  // found, delete it
      free(l->text);
      free(l->font);
      break;
    }
  }
  if (l == NULL) {  // not found, insert first
    l = malloc(sizeof(struct Label));
    l->id = id;
    l->next = supervisor_label;
    supervisor_label = l;
  }
  l->text = supervisor_strdup(text);
  l->font = supervisor_strdup(font);
  l->x = x;
  l->y = y;
  l->size = size;
  l->color = color_and_transparency;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_node_save_state(WbNodeRef node, const char *state_name) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  save_node_state_node_ref = node;
  save_node_state_name = state_name;
  wb_robot_flush_unlocked();
  save_node_state_node_ref = NULL;
  save_node_state_name = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_load_state(WbNodeRef node, const char *state_name) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  reset_node_state_node_ref = node;
  reset_node_state_name = state_name;
  wb_robot_flush_unlocked();
  reset_node_state_node_ref = NULL;
  reset_node_state_name = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_export_image(const char *filename, int quality) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'filename' argument.\n", __FUNCTION__);
    return;
  }

  if (quality < 1 || quality > 100) {
    fprintf(stderr, "Error: %s(): 'quality' argument (%d) must be between 1 and 100.\n", __FUNCTION__, quality);
    return;
  }

  robot_mutex_lock_step();
  free(export_image_filename);
  export_image_filename = supervisor_strdup(filename);
  export_image_quality = quality;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_movie_start_recording(const char *filename, int width, int height, int codec, int quality, int acceleration,
                                         bool caption) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!wb_supervisor_movie_is_ready()) {
    fprintf(stderr, "Error: %s(): movie recording has already been started.\n", __FUNCTION__);
    return;
  }

  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'filename' argument.\n", __FUNCTION__);
    return;
  }
  if (width <= 0 || height <= 0) {
    fprintf(stderr, "Error: %s(): 'width' and 'height' arguments must be postitive.\n", __FUNCTION__);
    return;
  }
  if (quality < 1 || quality > 100) {
    fprintf(stderr, "Error: %s(): 'quality' argument (%d) must be between 1 and 100.\n", __FUNCTION__, quality);
    return;
  }
  if (acceleration < 1) {
    fprintf(stderr, "Error: %s(): 'acceleration' argument must be greater than or equal to 1.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  free(movie_filename);
  movie_filename = supervisor_strdup(filename);
  movie_width = width;
  movie_height = height;
  movie_codec = codec;
  movie_quality = quality;
  movie_acceleration = acceleration;
  movie_caption = caption;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_movie_stop_recording() {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  robot_mutex_lock_step();
  movie_stop = true;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

bool wb_supervisor_movie_is_ready() {
  if (!robot_check_supervisor(__FUNCTION__))
    return false;

  robot_mutex_lock_step();
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();

  return movie_status == WB_SUPERVISOR_MOVIE_READY || movie_status > WB_SUPERVISOR_MOVIE_SAVING;
}

bool wb_supervisor_movie_failed() {
  if (!robot_check_supervisor(__FUNCTION__))
    return true;

  robot_mutex_lock_step();
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();

  return movie_status > WB_SUPERVISOR_MOVIE_SAVING;
}

int wb_supervisor_movie_get_status() {
  static bool deprecation_warning = true;
  if (deprecation_warning) {
    fprintf(stderr,
            "Warning: %s() is deprecated, use wb_supervisor_movie_is_ready() and wb_supervisor_movie_failed() instead.\n",
            __FUNCTION__);
    deprecation_warning = false;
  }
  return movie_status;
}

void wb_supervisor_start_movie(const char *file, int width, int height, int codec, int quality, int acceleration,
                               bool caption) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  wb_supervisor_movie_start_recording(file, width, height, codec, quality, acceleration, caption);
}

void wb_supervisor_stop_movie() {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  wb_supervisor_movie_stop_recording();
}

int wb_supervisor_get_movie_status() {
  if (!robot_check_supervisor(__FUNCTION__))
    return WB_SUPERVISOR_MOVIE_SIMULATION_ERROR;

  return wb_supervisor_movie_get_status();
}

bool wb_supervisor_animation_start_recording(const char *filename) {
  animation_start_status = true;

  if (!robot_check_supervisor(__FUNCTION__))
    return false;

  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'filename' argument.\n", __FUNCTION__);
    return false;
  }

  if (strcmp("html", wb_file_get_extension(filename)) != 0) {
    fprintf(stderr, "Error: the target file given to %s() should have the '.html' extension.\n", __FUNCTION__);
    return false;
  }

  robot_mutex_lock_step();
  free(animation_filename);
  animation_filename = supervisor_strdup(filename);
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();

  return animation_start_status;
}

bool wb_supervisor_animation_stop_recording() {
  animation_stop_status = true;

  if (!robot_check_supervisor(__FUNCTION__))
    return false;

  robot_mutex_lock_step();
  animation_stop = true;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();

  return animation_stop_status;
}

void wb_supervisor_simulation_quit(int status) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  robot_mutex_lock_step();
  simulation_quit = true;
  simulation_quit_status = status;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_simulation_reset() {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  robot_mutex_lock_step();
  simulation_reset = true;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_simulation_revert() {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  wb_supervisor_world_reload();
}

void wb_supervisor_simulation_physics_reset() {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  wb_supervisor_simulation_reset_physics();
}

WbSimulationMode wb_supervisor_simulation_get_mode() {
  return robot_get_simulation_mode();
}

void wb_supervisor_simulation_set_mode(WbSimulationMode mode) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  robot_mutex_lock_step();
  robot_set_simulation_mode(mode);
  simulation_change_mode = true;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_simulation_reset_physics() {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  robot_mutex_lock_step();
  simulation_reset_physics = true;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_load_world(const char *filename) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  wb_supervisor_world_load(filename);
}

void wb_supervisor_world_load(const char *filename) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'filename' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  world_to_load = filename;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

bool wb_supervisor_save_world(const char *filename) {
  if (!robot_check_supervisor(__FUNCTION__))
    return false;

  return wb_supervisor_world_save(filename);
}

bool wb_supervisor_world_save(const char *filename) {
  if (!robot_check_supervisor(__FUNCTION__))
    return false;

  if (filename) {
    if (!filename[0]) {
      fprintf(stderr, "Error: %s() called with an empty 'filename' argument.\n", __FUNCTION__);
      return false;
    }

    if (strcmp("wbt", wb_file_get_extension(filename)) != 0) {
      fprintf(stderr, "Error: the target file given to %s() ends with the '.wbt' extension.\n", __FUNCTION__);
      return false;
    }
  } else {
    fprintf(stderr, "Error: %s() called with a NULL 'filename' argument.\n", __FUNCTION__);
    return false;
  }

  free(save_filename);
  save_filename = NULL;

  save_status = true;
  save_request = true;

  robot_mutex_lock_step();
  save_filename = supervisor_strdup(filename);
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();

  return save_status;
}

void wb_supervisor_world_reload() {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  robot_mutex_lock_step();
  world_reload = true;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

WbNodeRef wb_supervisor_node_get_root() {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  return root_ref;
}

WbNodeRef wb_supervisor_node_get_self() {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  return self_node_ref;
}

int wb_supervisor_node_get_id(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return -1;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return -1;
  }

  if (node->is_proto_internal) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called for an internal PROTO node.\n", __FUNCTION__);
    return -1;
  }

  return node->id;
}

static WbNodeRef node_get_from_id(int id) {
  robot_mutex_lock_step();

  WbNodeRef result = find_node_by_id(id);
  if (!result) {
    WbNodeRef node_list_before = node_list;
    node_id = id;
    wb_robot_flush_unlocked();
    if (node_list != node_list_before)
      result = node_list;
    else
      result = find_node_by_id(id);
    node_id = -1;
  }
  robot_mutex_unlock_step();
  return result;
}

WbNodeRef wb_supervisor_node_get_from_id(int id) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (id < 0) {
    fprintf(stderr, "Error: %s() called with a negative 'id' argument.\n", __FUNCTION__);
    return NULL;
  }

  return node_get_from_id(id);
}

WbNodeRef wb_supervisor_node_get_from_def(const char *def) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!def || !def[0]) {
    fprintf(stderr, "Error: %s() called with a NULL or empty 'def' argument.\n", __FUNCTION__);
    return NULL;
  }

  robot_mutex_lock_step();

  // search if node is already present in node_list
  WbNodeRef result = find_node_by_def(def, NULL);
  if (!result) {
    // otherwise: need to talk to Webots
    node_def_name = def;
    node_id = -1;
    wb_robot_flush_unlocked();
    if (node_id >= 0)
      result = find_node_by_id(node_id);
    node_def_name = NULL;
    node_id = -1;
  }
  robot_mutex_unlock_step();
  return result;
}

WbNodeRef wb_supervisor_node_get_from_device(WbDeviceTag tag) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (tag >= robot_get_number_of_devices()) {
    fprintf(stderr, "Error: %s() called with an invalid 'tag' argument.\n", __FUNCTION__);
    return NULL;
  }

  robot_mutex_lock_step();

  // search if node is already present in node_list
  WbNodeRef result = find_node_by_tag(tag);
  if (!result) {
    // otherwise: need to talk to Webots
    allow_search_in_proto = true;
    node_tag = tag;
    node_id = -1;
    wb_robot_flush_unlocked();
    if (node_id >= 0)
      result = find_node_by_id(node_id);
    node_tag = -1;
    node_id = -1;
    allow_search_in_proto = false;
  }
  robot_mutex_unlock_step();
  return result;
}

bool wb_supervisor_node_is_proto(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return false;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return false;
  }

  return node->is_proto;
}

WbNodeRef wb_supervisor_node_get_from_proto_def(WbNodeRef node, const char *def) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!def || !def[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'def' argument.\n", __FUNCTION__);
    return NULL;
  }

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return NULL;
  }

  if (!node->is_proto) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s(): 'node' is not a PROTO node.\n", __FUNCTION__);
    return NULL;
  }

  robot_mutex_lock_step();

  // search if node is already present in node_list
  WbNodeRef result = find_node_by_def(def, node);
  if (!result) {
    // otherwise: need to talk to Webots
    node_def_name = def;
    node_id = -1;
    proto_id = node->id;
    wb_robot_flush_unlocked();
    if (node_id >= 0) {
      result = find_node_by_id(node_id);
      if (result) {
        result->is_proto_internal = true;
        result->parent_proto = node;
      }
    }
    node_def_name = NULL;
    node_id = -1;
    proto_id = -1;
  }
  robot_mutex_unlock_step();
  return result;
}

WbNodeRef wb_supervisor_node_get_parent_node(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return NULL;
  }

  allow_search_in_proto = true;
  WbNodeRef parent_node = node_get_from_id(node->parent_id);
  allow_search_in_proto = false;
  return parent_node;
}

WbNodeRef wb_supervisor_node_get_selected() {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  robot_mutex_lock_step();

  WbNodeRef result = NULL;
  node_get_selected = true;
  node_id = -1;
  wb_robot_flush_unlocked();
  if (node_id >= 0)
    result = find_node_by_id(node_id);
  node_id = -1;
  node_get_selected = false;

  robot_mutex_unlock_step();
  return result;
}

const double *wb_supervisor_node_get_position(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return invalid_vector;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return invalid_vector;
  }

  robot_mutex_lock_step();
  position_node_ref = node;
  wb_robot_flush_unlocked();
  position_node_ref = NULL;
  robot_mutex_unlock_step();
  return node->position ? node->position : invalid_vector;  // will be (NaN, NaN, NaN) if n is not derived from Transform
}

const double *wb_supervisor_node_get_orientation(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return invalid_vector;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return invalid_vector;
  }

  robot_mutex_lock_step();
  orientation_node_ref = node;
  wb_robot_flush_unlocked();
  orientation_node_ref = NULL;
  robot_mutex_unlock_step();
  return node->orientation ? node->orientation : invalid_vector;  // will be (NaN, ..., NaN) if n is not derived from Transform
}

const double *wb_supervisor_node_get_pose(WbNodeRef node, WbNodeRef from_node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return invalid_vector;

  if (from_node != NULL && !is_node_ref_valid(from_node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'from_node' argument.\n", __FUNCTION__);
    return invalid_vector;
  }

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return invalid_vector;
  }

  WbPoseStruct *tmp_pose = pose_collection;
  while (tmp_pose)
    if (tmp_pose->from_node == from_node && tmp_pose->to_node == node) {
      if (tmp_pose->last_update == wb_robot_get_time())
        return tmp_pose->pose;
      else
        break;

      tmp_pose = tmp_pose->next;
    }

  robot_mutex_lock_step();
  pose_requested = true;
  pose.from_node = from_node;
  pose.to_node = node;
  wb_robot_flush_unlocked();
  pose_requested = false;
  robot_mutex_unlock_step();
  return pose.pose;
}

const double *wb_supervisor_node_get_center_of_mass(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return invalid_vector;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return invalid_vector;
  }

  robot_mutex_lock_step();
  center_of_mass_node_ref = node;
  wb_robot_flush_unlocked();
  center_of_mass_node_ref = NULL;
  robot_mutex_unlock_step();
  return node->center_of_mass ? node->center_of_mass : invalid_vector;  // will be NULL if n is not a Solid
}

const double *wb_supervisor_node_get_contact_point(WbNodeRef node, int index) {
  static bool deprecation_warning = true;
  if (deprecation_warning) {
    fprintf(stderr, "Warning: %s() is deprecated, use wb_supervisor_node_get_contact_points() instead.\n", __FUNCTION__);
    deprecation_warning = false;
  }
  if (!robot_check_supervisor(__FUNCTION__))
    return invalid_vector;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return invalid_vector;
  }

  const double t = wb_robot_get_time();
  const int descendants = node->contact_points_include_descendants;

  if (t <= node->contact_points[descendants].timestamp && node->contact_points[descendants].points)
    return (node->contact_points[descendants].points && index < node->contact_points[descendants].n) ?
             node->contact_points[descendants].points[index].point :
             invalid_vector;  // will be (NaN, NaN, NaN) if n is not a Solid or if there is no contact

  node->contact_points[descendants].timestamp = t;

  robot_mutex_lock_step();
  contact_points_node_ref = node;
  wb_robot_flush_unlocked();
  contact_points_node_ref = NULL;
  robot_mutex_unlock_step();

  return (node->contact_points[descendants].points && index < node->contact_points[descendants].n) ?
           node->contact_points[descendants].points[index].point :
           invalid_vector;  // will be (NaN, NaN, NaN) if n is not a Solid or if there is no contact
}

WbNodeRef wb_supervisor_node_get_contact_point_node(WbNodeRef node, int index) {
  static bool deprecation_warning = true;
  if (deprecation_warning) {
    fprintf(stderr, "Warning: %s() is deprecated, use wb_supervisor_node_get_contact_points() instead.\n", __FUNCTION__);
    deprecation_warning = false;
  }
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return NULL;
  }

  const double t = wb_robot_get_time();
  const int descendants = node->contact_points_include_descendants;

  if (t > node->contact_points[descendants].timestamp ||
      contact_points_include_descendants != node->contact_points_include_descendants) {
    node->contact_points[descendants].timestamp = t;
    node->contact_points_include_descendants = contact_points_include_descendants;
    robot_mutex_lock_step();
    contact_points_node_ref = node;
    wb_robot_flush_unlocked();
    contact_points_node_ref = NULL;
    robot_mutex_unlock_step();
  }

  if (!node->contact_points[descendants].points || index >= node->contact_points[descendants].n)
    return NULL;
  allow_search_in_proto = true;
  WbNodeRef result = node_get_from_id(node->contact_points[descendants].points[index].node_id);
  allow_search_in_proto = false;
  return result;
}

int wb_supervisor_node_get_number_of_contact_points(WbNodeRef node, bool include_descendants) {
  static bool deprecation_warning = true;
  if (deprecation_warning) {
    fprintf(stderr, "Warning: %s() is deprecated, use wb_supervisor_node_get_contact_points() instead.\n", __FUNCTION__);
    deprecation_warning = false;
  }
  if (!robot_check_supervisor(__FUNCTION__))
    return -1;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return -1;
  }

  const double t = wb_robot_get_time();
  const int descendants = include_descendants ? 1 : 0;

  if (t <= node->contact_points[descendants].timestamp && node->contact_points[descendants].points)
    return node->contact_points[descendants].n;

  node->contact_points[descendants].timestamp = t;
  node->contact_points_include_descendants = include_descendants;

  robot_mutex_lock_step();
  contact_points_node_ref = node;
  contact_points_include_descendants = include_descendants;
  wb_robot_flush_unlocked();
  contact_points_node_ref = NULL;
  robot_mutex_unlock_step();

  return node->contact_points[descendants].n;  // will be -1 if n is not a Solid
}

WbContactPoint *wb_supervisor_node_get_contact_points(WbNodeRef node, bool include_descendants, int *size) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return NULL;
  }

  const double t = wb_robot_get_time();
  const int descendants = include_descendants ? 1 : 0;

  if (t == node->contact_points[descendants].last_update) {
    *size = node->contact_points[descendants].n;
    return node->contact_points[descendants].points;
  }

  // TODO: Delete with `wb_supervisor_node_get_contact_point`
  node->contact_points[descendants].timestamp = t;

  robot_mutex_lock_step();
  contact_points_node_ref = node;
  contact_points_include_descendants = include_descendants;
  wb_robot_flush_unlocked();
  contact_points_node_ref = NULL;
  robot_mutex_unlock_step();

  *size = node->contact_points[descendants].n;
  return node->contact_points[descendants].points;
}

bool wb_supervisor_node_get_static_balance(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return false;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return false;
  }

  robot_mutex_lock_step();
  static_balance_node_ref = node;
  wb_robot_flush_unlocked();
  static_balance_node_ref = NULL;
  robot_mutex_unlock_step();

  return node->static_balance;  // will be false if n is not a top Solid
}

const char *wb_supervisor_node_get_def(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return "";

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return "";
  }

  return node->def_name ? node->def_name : "";
}

WbNodeType wb_supervisor_node_get_type(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return 0;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return 0;
  }

  return node->type;
}

const char *wb_supervisor_node_get_type_name(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return "";

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return "";
  }

  if (!node->model_name)
    return wb_node_get_name(node->type);
  return node->model_name;
}

const char *wb_supervisor_node_get_base_type_name(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return "";

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return "";
  }

  return wb_node_get_name(node->type);
}

WbFieldRef wb_supervisor_node_get_field_by_index(WbNodeRef node, int index) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return NULL;
  }
  if (index < 0) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with an negative 'index' argument: %d.\n", __FUNCTION__, index);
    return NULL;
  }

  robot_mutex_lock_step();
  // search if field is already present in field_list
  WbFieldRef result = find_field_by_id(index, node->id);
  if (!result) {
    // otherwise: need to talk to Webots
    WbFieldRef field_list_before = field_list;
    requested_field_index = index;
    node_ref = node->id;
    wb_robot_flush_unlocked();
    requested_field_index = -1;
    if (field_list != field_list_before)
      result = field_list;
    else
      result = find_field_by_id(index, node->id);
    if (result && node->is_proto_internal)
      result->is_proto_internal = true;
  }
  robot_mutex_unlock_step();
  return result;
}

WbFieldRef wb_supervisor_node_get_proto_field_by_index(WbNodeRef node, int index) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return NULL;
  }
  if (index < 0) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a negative 'index' argument: %d.\n", __FUNCTION__, index);
    return NULL;
  }

  robot_mutex_lock_step();
  // search if field is already present in field_list
  WbFieldRef result = find_field_by_id(index, node->id);
  if (!result) {
    // otherwise: need to talk to Webots
    WbFieldRef field_list_before = field_list;
    requested_field_index = index;
    node_ref = node->id;
    allow_search_in_proto = true;
    wb_robot_flush_unlocked();
    requested_field_index = -1;
    if (field_list != field_list_before)
      result = field_list;
    else
      result = find_field_by_id(index, node->id);
    if (result)
      result->is_proto_internal = true;
    allow_search_in_proto = false;
  }
  robot_mutex_unlock_step();
  return result;
}

WbFieldRef wb_supervisor_node_get_field(WbNodeRef node, const char *field_name) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return NULL;
  }

  if (!field_name || !field_name[0]) {
    fprintf(stderr, "Error: %s() called with a NULL or empty 'field_name' argument.\n", __FUNCTION__);
    return NULL;
  }

  robot_mutex_lock_step();

  WbFieldRef result = find_field_by_name(field_name, node->id);
  if (!result) {
    // otherwise: need to talk to Webots
    requested_field_name = field_name;
    node_ref = node->id;
    wb_robot_flush_unlocked();
    if (requested_field_name) {
      requested_field_name = NULL;
      result = field_list;  // was just inserted at list head
      if (result && node->is_proto_internal)
        result->is_proto_internal = true;
    }
  }
  robot_mutex_unlock_step();
  return result;
}

int wb_supervisor_node_get_number_of_fields(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return -1;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with NULL or invalid 'node' argument.\n", __FUNCTION__);
    return -1;
  }

  robot_mutex_lock_step();
  requested_node_number_of_fields = true;
  node_ref = node->id;
  node_number_of_fields = -1;
  wb_robot_flush_unlocked();
  requested_node_number_of_fields = false;
  robot_mutex_unlock_step();
  if (node_number_of_fields > 0)
    return node_number_of_fields;
  return -1;
}

int wb_supervisor_node_get_proto_number_of_fields(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return -1;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with NULL or invalid 'node' argument.\n", __FUNCTION__);
    return -1;
  }

  robot_mutex_lock_step();
  requested_node_number_of_fields = true;
  node_ref = node->id;
  node_number_of_fields = -1;
  allow_search_in_proto = true;
  wb_robot_flush_unlocked();
  requested_node_number_of_fields = false;
  allow_search_in_proto = false;
  robot_mutex_unlock_step();
  if (node_number_of_fields > 0)
    return node_number_of_fields;
  return -1;
}

WbFieldRef wb_supervisor_node_get_proto_field(WbNodeRef node, const char *field_name) {
  if (!robot_check_supervisor(__FUNCTION__))
    return NULL;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with NULL or invalid 'node' argument.\n", __FUNCTION__);
    return NULL;
  }

  if (!node->is_proto) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s(): 'node' is not a PROTO node.\n", __FUNCTION__);
    return NULL;
  }

  if (!field_name || !field_name[0]) {
    fprintf(stderr, "Error: %s() called with NULL or empty 'field_name' argument.\n", __FUNCTION__);
    return NULL;
  }

  robot_mutex_lock_step();

  // search if field is already present in field_list
  WbFieldRef result = find_field_by_name(field_name, node->id);
  if (!result) {
    // otherwise: need to talk to Webots
    requested_field_name = field_name;
    node_ref = node->id;
    allow_search_in_proto = true;
    wb_robot_flush_unlocked();
    if (requested_field_name) {
      requested_field_name = NULL;
      result = field_list;  // was just inserted at list head
      if (result)
        result->is_proto_internal = true;
    }
    allow_search_in_proto = false;
  }
  robot_mutex_unlock_step();
  return result;
}

void wb_supervisor_node_remove(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node) || node->id == 0) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  if (node->type == WB_NODE_VIEWPOINT || node->type == WB_NODE_WORLD_INFO) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a Viewpoint or WorldInfo node.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  node_to_remove = node;
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

const char *wb_supervisor_node_export_string(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return "";

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return "";
  }

  robot_mutex_lock_step();
  export_string_node_ref = node;
  wb_robot_flush_unlocked();
  export_string_node_ref = NULL;
  robot_mutex_unlock_step();

  return node->content;
}

const double *wb_supervisor_node_get_velocity(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return invalid_vector;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return invalid_vector;
  }

  robot_mutex_lock_step();
  free(node->solid_velocity);
  node->solid_velocity = NULL;
  get_velocity_node_ref = node;
  wb_robot_flush_unlocked();
  get_velocity_node_ref = NULL;
  robot_mutex_unlock_step();
  // cppcheck-suppress knownConditionTrueFalse
  return node->solid_velocity ? node->solid_velocity : invalid_vector;  // will be NULL if n is not a Solid
}

void wb_supervisor_node_set_velocity(WbNodeRef node, const double velocity[6]) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  if (!check_vector(__FUNCTION__, velocity, 6))
    return;

  robot_mutex_lock_step();
  set_velocity_node_ref = node;
  solid_velocity = velocity;
  wb_robot_flush_unlocked();
  set_velocity_node_ref = NULL;
  solid_velocity = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_reset_physics(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  reset_physics_node_ref = node;
  wb_robot_flush_unlocked();
  reset_physics_node_ref = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_restart_controller(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  restart_controller_node_ref = node;
  wb_robot_flush_unlocked();
  restart_controller_node_ref = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_set_visibility(WbNodeRef node, WbNodeRef from, bool visible) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  if (!is_node_ref_valid(from)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'from' argument.\n", __FUNCTION__);
    return;
  }

  if (from->type != WB_NODE_VIEWPOINT && from->type != WB_NODE_CAMERA && from->type != WB_NODE_LIDAR &&
      from->type != WB_NODE_RANGE_FINDER) {
    fprintf(stderr,
            "Error: %s() called with a 'from' argument which is not the viewpoint or a camera, lidar or range-finder device.\n",
            __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  set_visibility_node_ref = node;
  set_visibility_from_node_ref = from;
  node_visible = visible;
  wb_robot_flush_unlocked();
  set_visibility_node_ref = NULL;
  set_visibility_from_node_ref = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_move_viewpoint(WbNodeRef node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  move_viewpoint_node_ref = node;
  wb_robot_flush_unlocked();
  move_viewpoint_node_ref = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_add_force(WbNodeRef node, const double force[3], bool relative) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  if (!check_vector(__FUNCTION__, force, 3))
    return;

  robot_mutex_lock_step();
  add_force_node_ref = node;
  add_force_or_torque = force;
  add_force_or_torque_relative = relative;
  wb_robot_flush_unlocked();
  add_force_node_ref = NULL;
  add_force_or_torque = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_add_force_with_offset(WbNodeRef node, const double force[3], const double offset[3], bool relative) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  if (!check_vector(__FUNCTION__, force, 3))
    return;

  if (!check_vector(__FUNCTION__, offset, 3))
    return;

  robot_mutex_lock_step();
  add_force_with_offset_node_ref = node;
  add_force_or_torque = force;
  add_force_offset = offset;
  add_force_or_torque_relative = relative;
  wb_robot_flush_unlocked();
  add_force_with_offset_node_ref = NULL;
  add_force_or_torque = NULL;
  add_force_offset = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_add_torque(WbNodeRef node, const double torque[3], bool relative) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  if (!check_vector(__FUNCTION__, torque, 3))
    return;

  robot_mutex_lock_step();
  add_torque_node_ref = node;
  add_force_or_torque = torque;
  add_force_or_torque_relative = relative;
  wb_robot_flush_unlocked();
  add_torque_node_ref = NULL;
  add_force_or_torque = NULL;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_set_joint_position(WbNodeRef node, double position, int index) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  if (index < 1) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with an invalid 'index'. Only values greater than or equal to 1 are supported.\n",
              __FUNCTION__);
    return;
  }
  bool valid_node = false;
  if (node->type == WB_NODE_SLIDER_JOINT || node->type == WB_NODE_HINGE_JOINT) {
    if (index != 1) {
      if (!robot_is_quitting())
        fprintf(stderr, "Error: %s() called with an invalid 'index'. SliderJoint and HingeJoint only support index 1.\n",
                __FUNCTION__);
      return;
    }
    valid_node = true;
  } else if (node->type == WB_NODE_HINGE_2_JOINT) {
    if (index > 2) {
      if (!robot_is_quitting())
        fprintf(stderr, "Error: %s() called with an invalid 'index'. Hinge2Joint only supports index 1 or 2.\n", __FUNCTION__);
      return;
    }
    valid_node = true;
  } else if (node->type == WB_NODE_BALL_JOINT) {
    if (index > 3) {
      if (!robot_is_quitting())
        fprintf(stderr, "Error: %s() called with an invalid 'index'. BallJoint only supports index 1, 2, or 3.\n",
                __FUNCTION__);
      return;
    }
    valid_node = true;
  }

  if (!valid_node) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a 'node' argument which is not a joint node.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  set_joint_node_ref = node;
  set_joint_position = position;
  set_joint_index = index;
  wb_robot_flush_unlocked();
  set_joint_node_ref = NULL;
  robot_mutex_unlock_step();
}

bool wb_supervisor_virtual_reality_headset_is_used() {
  if (!robot_check_supervisor(__FUNCTION__))
    return false;

  robot_mutex_lock_step();
  virtual_reality_headset_is_used_request = true;
  wb_robot_flush_unlocked();
  virtual_reality_headset_is_used_request = false;
  robot_mutex_unlock_step();
  return virtual_reality_headset_is_used;
}

const double *wb_supervisor_virtual_reality_headset_get_position() {
  if (!robot_check_supervisor(__FUNCTION__))
    return invalid_vector;

  robot_mutex_lock_step();
  virtual_reality_headset_position_request = true;
  free(virtual_reality_headset_position);
  virtual_reality_headset_position = NULL;
  wb_robot_flush_unlocked();
  virtual_reality_headset_position_request = false;
  robot_mutex_unlock_step();
  return virtual_reality_headset_position ? virtual_reality_headset_position : invalid_vector;
}

const double *wb_supervisor_virtual_reality_headset_get_orientation() {
  if (!robot_check_supervisor(__FUNCTION__))
    return invalid_vector;

  robot_mutex_lock_step();
  virtual_reality_headset_orientation_request = true;
  free(virtual_reality_headset_orientation);
  virtual_reality_headset_orientation = NULL;
  wb_robot_flush_unlocked();
  virtual_reality_headset_orientation_request = false;
  robot_mutex_unlock_step();
  return virtual_reality_headset_orientation ? virtual_reality_headset_orientation : invalid_vector;
}

const char *wb_supervisor_field_get_name(WbFieldRef field) {
  return field->name;
}

WbFieldType wb_supervisor_field_get_type(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, false))
    return WB_NO_FIELD;

  return ((WbFieldStruct *)field)->type;
}

int wb_supervisor_field_get_count(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, false))
    return false;

  if (((((WbFieldStruct *)field)->type) & WB_MF) != WB_MF) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() can only be used with multiple fields (MF).\n", __FUNCTION__);
    return -1;
  }

  return ((WbFieldStruct *)field)->count;
}

void wb_supervisor_node_enable_contact_point_tracking(WbNodeRef node, int sampling_period, bool include_descendants) {
  if (sampling_period < 0) {
    fprintf(stderr, "Error: %s() called with negative sampling period.\n", __FUNCTION__);
    return;
  }

  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  const int descendants = include_descendants ? 1 : 0;

  robot_mutex_lock_step();
  contact_point_change_tracking_requested = true;
  contact_point_change_tracking.node = node;
  contact_point_change_tracking.enable = true;
  contact_point_change_tracking.include_descendants = include_descendants;
  node->contact_points[descendants].last_update = -DBL_MAX;
  node->contact_points[descendants].sampling_period = sampling_period;
  wb_robot_flush_unlocked();
  pose_change_tracking_requested = false;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_disable_contact_point_tracking(WbNodeRef node, bool include_descendants) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  contact_point_change_tracking_requested = true;
  contact_point_change_tracking.node = node;
  contact_point_change_tracking.enable = false;
  contact_point_change_tracking.include_descendants = include_descendants;
  wb_robot_flush_unlocked();
  pose_change_tracking_requested = false;
  robot_mutex_unlock_step();
}

void wb_supervisor_field_enable_sf_tracking(WbFieldRef field, int sampling_period) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, false))
    return;

  if (sampling_period < 0) {
    fprintf(stderr, "Error: %s() called with negative sampling period.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  field_change_tracking.field = field;
  field_change_tracking.sampling_period = sampling_period;
  field_change_tracking.enable = true;
  field_change_tracking_requested = true;
  wb_robot_flush_unlocked();
  field_change_tracking_requested = false;
  robot_mutex_unlock_step();
}

void wb_supervisor_field_disable_sf_tracking(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, false))
    return;

  robot_mutex_lock_step();
  field_change_tracking.field = field;
  field_change_tracking.enable = false;
  field_change_tracking_requested = true;
  wb_robot_flush_unlocked();
  field_change_tracking_requested = false;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_enable_pose_tracking(WbNodeRef node, int sampling_period, WbNodeRef from_node) {
  if (sampling_period < 0) {
    fprintf(stderr, "Error: %s() called with negative sampling period.\n", __FUNCTION__);
    return;
  }

  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (from_node != NULL && !is_node_ref_valid(from_node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'from_node' argument.\n", __FUNCTION__);
    return;
  }

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  pose_change_tracking_requested = true;
  pose_change_tracking.node = node;
  pose_change_tracking.from_node = from_node;
  pose_change_tracking.enable = true;

  // Create a new pose item
  WbPoseStruct *const new_pose = malloc(sizeof(WbPoseStruct));
  new_pose->from_node = from_node;
  new_pose->to_node = node;
  new_pose->next = NULL;
  new_pose->last_update = -DBL_MAX;

  // Add the pose to the list
  if (!pose_collection)
    pose_collection = new_pose;
  else {
    WbPoseStruct *tmp_pose = pose_collection;
    while (tmp_pose->next)
      tmp_pose = tmp_pose->next;
    tmp_pose->next = new_pose;
  }

  assert(pose_collection);

  wb_robot_flush_unlocked();
  pose_change_tracking_requested = false;
  robot_mutex_unlock_step();
}

void wb_supervisor_node_disable_pose_tracking(WbNodeRef node, WbNodeRef from_node) {
  if (!robot_check_supervisor(__FUNCTION__))
    return;

  if (!is_node_ref_valid(node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'node' argument.\n", __FUNCTION__);
    return;
  }

  if (!is_node_ref_valid(from_node)) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a NULL or invalid 'from_node' argument.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  pose_change_tracking.node = node;
  pose_change_tracking.from_node = from_node;
  pose_change_tracking.enable = false;
  wb_robot_flush_unlocked();
  pose_change_tracking.node = NULL;
  robot_mutex_unlock_step();
}

bool wb_supervisor_field_get_sf_bool(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_BOOL, true, NULL, false, false))
    return false;

  field_operation(field, GET, -1);
  return ((WbFieldStruct *)field)->data.sf_bool;
}

int wb_supervisor_field_get_sf_int32(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_INT32, true, NULL, false, false))
    return 0;

  field_operation(field, GET, -1);
  return ((WbFieldStruct *)field)->data.sf_int32;
}

double wb_supervisor_field_get_sf_float(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_FLOAT, true, NULL, false, false))
    return 0.0;

  field_operation(field, GET, -1);
  return ((WbFieldStruct *)field)->data.sf_float;
}

const double *wb_supervisor_field_get_sf_vec2f(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_VEC2F, true, NULL, false, false))
    return NULL;

  field_operation(field, GET, -1);
  return ((WbFieldStruct *)field)->data.sf_vec2f;
}

const double *wb_supervisor_field_get_sf_vec3f(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_VEC3F, true, NULL, false, false))
    return NULL;

  field_operation(field, GET, -1);
  return ((WbFieldStruct *)field)->data.sf_vec3f;
}

const double *wb_supervisor_field_get_sf_rotation(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_ROTATION, true, NULL, false, false))
    return NULL;

  field_operation(field, GET, -1);
  return ((WbFieldStruct *)field)->data.sf_rotation;
}

const double *wb_supervisor_field_get_sf_color(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_COLOR, true, NULL, false, false))
    return NULL;

  field_operation(field, GET, -1);
  return ((WbFieldStruct *)field)->data.sf_vec3f;
}

const char *wb_supervisor_field_get_sf_string(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_STRING, true, NULL, false, false))
    return "";

  field_operation(field, GET, -1);
  return ((WbFieldStruct *)field)->data.sf_string;
}

WbNodeRef wb_supervisor_field_get_sf_node(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_SF_NODE, true, NULL, false, false))
    return NULL;

  field_operation(field, GET, -1);
  int id = ((WbFieldStruct *)field)->data.sf_node_uid;
  if (id <= 0)
    return NULL;
  WbNodeRef result = find_node_by_id(id);
  if (result && ((WbFieldStruct *)field)->is_proto_internal)
    result->is_proto_internal = true;
  return result;
}

bool wb_supervisor_field_get_mf_bool(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_BOOL, true, &index, false, false))
    return 0;

  field_operation(field, GET, index);
  return ((WbFieldStruct *)field)->data.sf_bool;
}

int wb_supervisor_field_get_mf_int32(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_INT32, true, &index, false, false))
    return 0;

  field_operation(field, GET, index);
  return ((WbFieldStruct *)field)->data.sf_int32;
}

double wb_supervisor_field_get_mf_float(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_FLOAT, true, &index, false, false))
    return 0.0;

  field_operation(field, GET, index);
  return ((WbFieldStruct *)field)->data.sf_float;
}

const double *wb_supervisor_field_get_mf_vec2f(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_VEC2F, true, &index, false, false))
    return NULL;

  field_operation(field, GET, index);
  return ((WbFieldStruct *)field)->data.sf_vec2f;
}

const double *wb_supervisor_field_get_mf_vec3f(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_VEC3F, true, &index, false, false))
    return NULL;

  field_operation(field, GET, index);
  return ((WbFieldStruct *)field)->data.sf_vec3f;
}

const double *wb_supervisor_field_get_mf_color(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_COLOR, true, &index, false, false))
    return NULL;

  field_operation(field, GET, index);
  return ((WbFieldStruct *)field)->data.sf_vec3f;
}

const double *wb_supervisor_field_get_mf_rotation(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_ROTATION, true, &index, false, false))
    return NULL;

  field_operation(field, GET, index);
  return ((WbFieldStruct *)field)->data.sf_rotation;
}

const char *wb_supervisor_field_get_mf_string(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_STRING, true, &index, false, false))
    return "";

  field_operation(field, GET, index);
  return ((WbFieldStruct *)field)->data.sf_string;
}

WbNodeRef wb_supervisor_field_get_mf_node(WbFieldRef field, int index) {
  if (!check_field(field, __FUNCTION__, WB_MF_NODE, true, &index, false, false))
    return NULL;

  field_operation(field, GET, index);
  WbNodeRef result = find_node_by_id(((WbFieldStruct *)field)->data.sf_node_uid);
  if (result && ((WbFieldStruct *)field)->is_proto_internal)
    result->is_proto_internal = true;
  return result;
}

void wb_supervisor_field_set_sf_bool(WbFieldRef field, bool value) {
  if (!check_field(field, __FUNCTION__, WB_SF_BOOL, true, NULL, false, true))
    return;

  union WbFieldData data;
  data.sf_bool = value;
  field_operation_with_data(field, SET, -1, data);
}

void wb_supervisor_field_set_sf_int32(WbFieldRef field, int value) {
  if (!check_field(field, __FUNCTION__, WB_SF_INT32, true, NULL, false, true))
    return;

  union WbFieldData data;
  data.sf_int32 = value;
  field_operation_with_data(field, SET, -1, data);
}

void wb_supervisor_field_set_sf_float(WbFieldRef field, double value) {
  if (!check_field(field, __FUNCTION__, WB_SF_FLOAT, true, NULL, false, true))
    return;

  if (!check_float(__FUNCTION__, value))
    return;

  union WbFieldData data;
  data.sf_float = value;
  field_operation_with_data(field, SET, -1, data);
}

void wb_supervisor_field_set_sf_vec2f(WbFieldRef field, const double values[2]) {
  if (!check_field(field, __FUNCTION__, WB_SF_VEC2F, true, NULL, false, true))
    return;

  if (!check_vector(__FUNCTION__, values, 2))
    return;

  union WbFieldData data;
  data.sf_vec2f[0] = values[0];
  data.sf_vec2f[1] = values[1];
  field_operation_with_data(field, SET, -1, data);
}

void wb_supervisor_field_set_sf_vec3f(WbFieldRef field, const double values[3]) {
  if (!check_field(field, __FUNCTION__, WB_SF_VEC3F, true, NULL, false, true))
    return;

  if (!check_vector(__FUNCTION__, values, 3))
    return;

  union WbFieldData data;
  data.sf_vec3f[0] = values[0];
  data.sf_vec3f[1] = values[1];
  data.sf_vec3f[2] = values[2];
  field_operation_with_data(field, SET, -1, data);
}

static bool isValidRotation(const double r[4]) {
  return !(r[0] == 0.0 && r[1] == 0.0 && r[2] == 0.0);
}

void wb_supervisor_field_set_sf_rotation(WbFieldRef field, const double values[4]) {
  if (!check_field(field, __FUNCTION__, WB_SF_ROTATION, true, NULL, false, true))
    return;

  if (!check_vector(__FUNCTION__, values, 4))
    return;

  if (!isValidRotation(values)) {
    fprintf(stderr, "Error: %s() called with invalid values for the [x y z] axis.\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_rotation[0] = values[0];
  data.sf_rotation[1] = values[1];
  data.sf_rotation[2] = values[2];
  data.sf_rotation[3] = values[3];
  field_operation_with_data(field, SET, -1, data);
}

static bool isValidColor(const double rgb[3]) {
  return rgb[0] >= 0.0 && rgb[0] <= 1.0 && rgb[1] >= 0.0 && rgb[1] <= 1.0 && rgb[2] >= 0.0 && rgb[2] <= 1.0;
}

void wb_supervisor_field_set_sf_color(WbFieldRef field, const double values[3]) {
  if (!check_field(field, __FUNCTION__, WB_SF_COLOR, true, NULL, false, true))
    return;

  if (!values) {
    fprintf(stderr, "Error: %s() called with a NULL 'values' argument.\n", __FUNCTION__);
    return;
  }

  if (!isValidColor(values)) {
    fprintf(stderr, "Error: %s() called with invalid RGB values (outside [0,1] range).\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_vec3f[0] = values[0];
  data.sf_vec3f[1] = values[1];
  data.sf_vec3f[2] = values[2];
  field_operation_with_data(field, SET, -1, data);
}

void wb_supervisor_field_set_sf_string(WbFieldRef field, const char *value) {
  if (!check_field(field, __FUNCTION__, WB_SF_STRING, true, NULL, false, true))
    return;

  if (!value) {
    fprintf(stderr, "Error: %s() called with a NULL string argument.\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_string = supervisor_strdup(value);
  field_operation_with_data(field, SET, -1, data);
}

void wb_supervisor_field_set_mf_bool(WbFieldRef field, int index, bool value) {
  if (!check_field(field, __FUNCTION__, WB_MF_BOOL, true, &index, false, true))
    return;

  union WbFieldData data;
  data.sf_bool = value;
  field_operation_with_data(field, SET, index, data);
}

void wb_supervisor_field_set_mf_int32(WbFieldRef field, int index, int value) {
  if (!check_field(field, __FUNCTION__, WB_MF_INT32, true, &index, false, true))
    return;

  union WbFieldData data;
  data.sf_int32 = value;
  field_operation_with_data(field, SET, index, data);
}

void wb_supervisor_field_set_mf_float(WbFieldRef field, int index, double value) {
  if (!check_field(field, __FUNCTION__, WB_MF_FLOAT, true, &index, false, true))
    return;

  if (!check_float(__FUNCTION__, value))
    return;

  union WbFieldData data;
  data.sf_float = value;
  field_operation_with_data(field, SET, index, data);
}

void wb_supervisor_field_set_mf_vec2f(WbFieldRef field, int index, const double values[2]) {
  if (!check_field(field, __FUNCTION__, WB_MF_VEC2F, true, &index, false, true))
    return;

  if (!check_vector(__FUNCTION__, values, 2))
    return;

  union WbFieldData data;
  data.sf_vec2f[0] = values[0];
  data.sf_vec2f[1] = values[1];
  field_operation_with_data(field, SET, index, data);
}

void wb_supervisor_field_set_mf_vec3f(WbFieldRef field, int index, const double values[3]) {
  if (!check_field(field, __FUNCTION__, WB_MF_VEC3F, true, &index, false, true))
    return;

  if (!check_vector(__FUNCTION__, values, 3))
    return;

  union WbFieldData data;
  data.sf_vec3f[0] = values[0];
  data.sf_vec3f[1] = values[1];
  data.sf_vec3f[2] = values[2];
  field_operation_with_data(field, SET, index, data);
}

void wb_supervisor_field_set_mf_rotation(WbFieldRef field, int index, const double values[4]) {
  if (!check_field(field, __FUNCTION__, WB_MF_ROTATION, true, &index, false, true))
    return;

  if (!check_vector(__FUNCTION__, values, 4))
    return;

  if (!isValidRotation(values)) {
    fprintf(stderr, "Error: %s() called with invalid values for the [x y z] axis.\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_rotation[0] = values[0];
  data.sf_rotation[1] = values[1];
  data.sf_rotation[2] = values[2];
  data.sf_rotation[3] = values[3];
  field_operation_with_data(field, SET, index, data);
}

void wb_supervisor_field_set_mf_color(WbFieldRef field, int index, const double values[3]) {
  if (!check_field(field, __FUNCTION__, WB_MF_COLOR, true, &index, false, true))
    return;

  if (!values) {
    fprintf(stderr, "Error: %s() called with a NULL 'values' argument.\n", __FUNCTION__);
    return;
  }

  if (!isValidColor(values)) {
    fprintf(stderr, "Error: %s() called with invalid RGB values (outside [0,1] range).\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_vec3f[0] = values[0];
  data.sf_vec3f[1] = values[1];
  data.sf_vec3f[2] = values[2];
  field_operation_with_data(field, SET, index, data);
}

void wb_supervisor_field_set_mf_string(WbFieldRef field, int index, const char *value) {
  if (!check_field(field, __FUNCTION__, WB_MF_STRING, true, &index, false, true))
    return;

  if (!value) {
    fprintf(stderr, "Error: %s() called with a NULL string argument.\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_string = supervisor_strdup(value);
  field_operation_with_data(field, SET, index, data);
}

void wb_supervisor_field_insert_mf_bool(WbFieldRef field, int index, bool value) {
  if (!check_field(field, __FUNCTION__, WB_MF_BOOL, true, &index, true, true))
    return;

  union WbFieldData data;
  data.sf_bool = value;
  field_operation_with_data((WbFieldStruct *)field, IMPORT, index, data);
}

void wb_supervisor_field_insert_mf_int32(WbFieldRef field, int index, int value) {
  if (!check_field(field, __FUNCTION__, WB_MF_INT32, true, &index, true, true))
    return;

  union WbFieldData data;
  data.sf_int32 = value;
  field_operation_with_data((WbFieldStruct *)field, IMPORT, index, data);
}

void wb_supervisor_field_insert_mf_float(WbFieldRef field, int index, double value) {
  if (!check_field(field, __FUNCTION__, WB_MF_FLOAT, true, &index, true, true))
    return;

  if (!check_float(__FUNCTION__, value))
    return;

  union WbFieldData data;
  data.sf_float = value;
  field_operation_with_data((WbFieldStruct *)field, IMPORT, index, data);
}

void wb_supervisor_field_insert_mf_vec2f(WbFieldRef field, int index, const double values[2]) {
  if (!check_field(field, __FUNCTION__, WB_MF_VEC2F, true, &index, true, true))
    return;

  if (!check_vector(__FUNCTION__, values, 2))
    return;

  union WbFieldData data;
  data.sf_vec2f[0] = values[0];
  data.sf_vec2f[1] = values[1];
  field_operation_with_data((WbFieldStruct *)field, IMPORT, index, data);
}

void wb_supervisor_field_insert_mf_vec3f(WbFieldRef field, int index, const double values[3]) {
  if (!check_field(field, __FUNCTION__, WB_MF_VEC3F, true, &index, true, true))
    return;

  if (!check_vector(__FUNCTION__, values, 3))
    return;

  union WbFieldData data;
  data.sf_vec3f[0] = values[0];
  data.sf_vec3f[1] = values[1];
  data.sf_vec3f[2] = values[2];
  field_operation_with_data((WbFieldStruct *)field, IMPORT, index, data);
}

void wb_supervisor_field_insert_mf_rotation(WbFieldRef field, int index, const double values[4]) {
  if (!check_field(field, __FUNCTION__, WB_MF_ROTATION, true, &index, true, true))
    return;

  if (!check_vector(__FUNCTION__, values, 4))
    return;

  if (!isValidRotation(values)) {
    fprintf(stderr, "Error: %s() called with invalid values for the [x y z] axis.\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_rotation[0] = values[0];
  data.sf_rotation[1] = values[1];
  data.sf_rotation[2] = values[2];
  data.sf_rotation[3] = values[3];
  field_operation_with_data((WbFieldStruct *)field, IMPORT, index, data);
}

void wb_supervisor_field_insert_mf_color(WbFieldRef field, int index, const double values[3]) {
  if (!check_field(field, __FUNCTION__, WB_MF_COLOR, true, &index, true, true))
    return;

  if (!values) {
    fprintf(stderr, "Error: %s() called with a NULL 'values' argument.\n", __FUNCTION__);
    return;
  }

  if (!isValidColor(values)) {
    fprintf(stderr, "Error: %s() called with invalid RGB values (outside [0,1] range).\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_vec3f[0] = values[0];
  data.sf_vec3f[1] = values[1];
  data.sf_vec3f[2] = values[2];
  field_operation_with_data((WbFieldStruct *)field, IMPORT, index, data);
}

void wb_supervisor_field_insert_mf_string(WbFieldRef field, int index, const char *value) {
  if (!check_field(field, __FUNCTION__, WB_MF_STRING, true, &index, true, true))
    return;

  if (!value) {
    fprintf(stderr, "Error: %s() called with a NULL string argument.\n", __FUNCTION__);
    return;
  }

  union WbFieldData data;
  data.sf_string = supervisor_strdup(value);
  field_operation_with_data((WbFieldStruct *)field, IMPORT, index, data);
}

void wb_supervisor_field_remove_mf(WbFieldRef field, int index) {
  if (field->count == 0) {
    fprintf(stderr, "Error: %s() called for an empty field.\n", __FUNCTION__);
    return;
  }

  if (!check_field(field, __FUNCTION__, WB_MF, false, &index, false, true))
    return;

  field_operation(field, REMOVE, index);
}

void wb_supervisor_field_import_mf_node(WbFieldRef field, int position, const char *filename) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, true))
    return;

  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with a NULL or empty 'filename' argument.\n", __FUNCTION__);
    return;
  }

  // check extension
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename) {
    fprintf(stderr, "Error: %s() called with a 'filename' argument without extension.\n", __FUNCTION__);
    return;
  }

  const bool isWbo = strcmp(dot, ".wbo") == 0;
  const bool isWrl = strcmp(dot, ".wrl") == 0;
  if (!isWbo && !isWrl) {
    fprintf(stderr, "Error: %s() supports only '*.wbo' and '*.wrl' files.\n", __FUNCTION__);
    return;
  }

  if (isWrl && field != wb_supervisor_node_get_field(root_ref, "children")) {
    fprintf(stderr, "Error: %s() '*.wrl' import is supported only at the root children field level.\n", __FUNCTION__);
    return;
  }

  WbFieldStruct *f = (WbFieldStruct *)field;
  if (f->type != WB_MF_NODE) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with wrong field type: %s.\n", __FUNCTION__,
              wb_supervisor_field_get_type_name(field));
    return;
  }

  int count = f->count;
  if (position < -(count + 1) || position > count) {
    fprintf(stderr, "Error: %s() called with an out-of-bound index: %d (should be between %d and %d).\n", __FUNCTION__,
            position, -(count + 1), count);
    return;
  }

  // resolve negative position value
  if (position < 0)
    position = count + position + 1;

  if (isWrl && position != f->count) {
    fprintf(stderr, "Error: %s() '*.wrl' import is supported only at the end of the root node children field.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  union WbFieldData data;
  data.sf_string = supervisor_strdup(filename);
  create_and_append_field_request(f, IMPORT, position, data, false);
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_field_import_mf_node_from_string(WbFieldRef field, int position, const char *node_string) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, true))
    return;

  WbFieldStruct *f = (WbFieldStruct *)field;
  if (f->type != WB_MF_NODE) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a wrong field type: %s.\n", __FUNCTION__,
              wb_supervisor_field_get_type_name(field));
    return;
  }

  if (!node_string || !node_string[0]) {
    fprintf(stderr, "Error: %s() called with a NULL or empty 'node_string' argument.\n", __FUNCTION__);
    return;
  }

  int count = f->count;
  if (position < -(count + 1) || position > count) {
    fprintf(stderr, "Error: %s() called with an out-of-bound index: %d (should be between %d and %d).\n", __FUNCTION__,
            position, -(count + 1), count);
    return;
  }

  // resolve negative position value
  if (position < 0)
    position = count + position + 1;

  robot_mutex_lock_step();
  union WbFieldData data;
  data.sf_string = supervisor_strdup(node_string);
  create_and_append_field_request(f, IMPORT_FROM_STRING, position, data, false);
  wb_robot_flush_unlocked();
  robot_mutex_unlock_step();
}

void wb_supervisor_field_remove_mf_node(WbFieldRef field, int position) {
  wb_supervisor_field_remove_mf(field, position);
}

void wb_supervisor_field_remove_sf(WbFieldRef field) {
  if (field->data.sf_node_uid == 0) {
    fprintf(stderr, "Error: %s() called for an empty field.\n", __FUNCTION__);
    return;
  }

  if (!check_field(field, __FUNCTION__, WB_SF_NODE, true, NULL, false, true))
    return;

  field_operation(field, REMOVE, -1);
  field->count = 0;
}

void wb_supervisor_field_import_sf_node(WbFieldRef field, const char *filename) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, true))
    return;

  if (!filename || !filename[0]) {
    fprintf(stderr, "Error: %s() called with a NULL or empty 'filename' argument.\n", __FUNCTION__);
    return;
  }

  // check extension
  const char *dot = strrchr(filename, '.');
  if (!dot || dot == filename) {
    fprintf(stderr, "Error: %s() called with a 'filename' argument without extension.\n", __FUNCTION__);
    return;
  }

  if (strcmp(dot, ".wbo") == 0) {
    fprintf(stderr, "Error: %s() supports only '*.wbo' files.\n", __FUNCTION__);
    return;
  }

  WbFieldStruct *f = (WbFieldStruct *)field;
  if (f->type != WB_SF_NODE) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with wrong field type: %s.\n", __FUNCTION__,
              wb_supervisor_field_get_type_name(field));
    return;
  }

  if (field->data.sf_node_uid != 0) {
    fprintf(stderr, "Error: %s() called with a non-empty field.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  union WbFieldData data;
  data.sf_string = supervisor_strdup(filename);
  create_and_append_field_request(f, IMPORT, -1, data, false);
  imported_node_id = -1;
  wb_robot_flush_unlocked();
  if (imported_node_id >= 0)
    field->data.sf_node_uid = imported_node_id;
  robot_mutex_unlock_step();
}

void wb_supervisor_field_import_sf_node_from_string(WbFieldRef field, const char *node_string) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, true))
    return;

  WbFieldStruct *f = (WbFieldStruct *)field;
  if (f->type != WB_SF_NODE) {
    if (!robot_is_quitting())
      fprintf(stderr, "Error: %s() called with a wrong field type: %s.\n", __FUNCTION__,
              wb_supervisor_field_get_type_name(field));
    return;
  }

  if (!node_string || !node_string[0]) {
    fprintf(stderr, "Error: %s() called with a NULL or empty 'node_string' argument.\n", __FUNCTION__);
    return;
  }

  if (field->data.sf_node_uid != 0) {
    fprintf(stderr, "Error: %s() called with a non-empty field.\n", __FUNCTION__);
    return;
  }

  robot_mutex_lock_step();
  union WbFieldData data;
  data.sf_string = supervisor_strdup(node_string);
  create_and_append_field_request(f, IMPORT_FROM_STRING, -1, data, false);
  imported_node_id = -1;
  wb_robot_flush_unlocked();
  if (imported_node_id >= 0)
    field->data.sf_node_uid = imported_node_id;
  robot_mutex_unlock_step();
}

const char *wb_supervisor_field_get_type_name(WbFieldRef field) {
  if (!check_field(field, __FUNCTION__, WB_NO_FIELD, false, NULL, false, false))
    return "";

  switch (field->type) {
    case WB_SF_BOOL:
      return "SFBool";
    case WB_SF_INT32:
      return "SFInt32";
    case WB_SF_FLOAT:
      return "SFFloat";
    case WB_SF_VEC2F:
      return "SFVec2f";
    case WB_SF_VEC3F:
      return "SFVec3f";
    case WB_SF_ROTATION:
      return "SFRotation";
    case WB_SF_COLOR:
      return "SFColor";
    case WB_SF_STRING:
      return "SFString";
    case WB_SF_NODE:
      return "SFNode";
    case WB_MF_BOOL:
      return "MFBool";
    case WB_MF_INT32:
      return "MFInt32";
    case WB_MF_FLOAT:
      return "MFFloat";
    case WB_MF_VEC2F:
      return "MFVec2f";
    case WB_MF_VEC3F:
      return "MFVec3f";
    case WB_MF_COLOR:
      return "MFColor";
    case WB_MF_ROTATION:
      return "MFRotation";
    case WB_MF_STRING:
      return "MFString";
    case WB_MF_NODE:
      return "MFNode";
    default:
      return "";
  }
}
