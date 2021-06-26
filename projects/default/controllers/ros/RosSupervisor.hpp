// Copyright 1996-2021 Cyberbotics Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ROS_SUPERVISOR_HPP
#define ROS_SUPERVISOR_HPP

#include <webots/Supervisor.hpp>
#include "Ros.hpp"

#include <webots_ros/get_bool.h>
#include <webots_ros/get_int.h>
#include <webots_ros/get_uint64.h>
#include <webots_ros/set_bool.h>
#include <webots_ros/set_int.h>
#include <webots_ros/set_string.h>

#include <webots_ros/save_image.h>
#include <webots_ros/supervisor_get_from_def.h>
#include <webots_ros/supervisor_get_from_id.h>
#include <webots_ros/supervisor_get_from_string.h>
#include <webots_ros/supervisor_movie_start_recording.h>
#include <webots_ros/supervisor_set_label.h>
#include <webots_ros/supervisor_virtual_reality_headset_get_orientation.h>
#include <webots_ros/supervisor_virtual_reality_headset_get_position.h>

#include <webots_ros/node_add_force_or_torque.h>
#include <webots_ros/node_add_force_with_offset.h>
#include <webots_ros/node_disable_pose_tracking.h>
#include <webots_ros/node_enable_pose_tracking.h>
#include <webots_ros/node_get_center_of_mass.h>
#include <webots_ros/node_get_contact_point.h>
#include <webots_ros/node_get_contact_point_node.h>
#include <webots_ros/node_get_field.h>
#include <webots_ros/node_get_id.h>
#include <webots_ros/node_get_name.h>
#include <webots_ros/node_get_number_of_contact_points.h>
#include <webots_ros/node_get_orientation.h>
#include <webots_ros/node_get_parent_node.h>
#include <webots_ros/node_get_pose.h>
#include <webots_ros/node_get_position.h>
#include <webots_ros/node_get_static_balance.h>
#include <webots_ros/node_get_status.h>
#include <webots_ros/node_get_string.h>
#include <webots_ros/node_get_type.h>
#include <webots_ros/node_get_velocity.h>
#include <webots_ros/node_is_proto.h>
#include <webots_ros/node_move_viewpoint.h>
#include <webots_ros/node_remove.h>
#include <webots_ros/node_reset_functions.h>
#include <webots_ros/node_set_string.h>
#include <webots_ros/node_set_velocity.h>
#include <webots_ros/node_set_visibility.h>

#include <webots_ros/field_disable_sf_tracking.h>
#include <webots_ros/field_enable_sf_tracking.h>
#include <webots_ros/field_get_bool.h>
#include <webots_ros/field_get_color.h>
#include <webots_ros/field_get_count.h>
#include <webots_ros/field_get_float.h>
#include <webots_ros/field_get_int32.h>
#include <webots_ros/field_get_node.h>
#include <webots_ros/field_get_rotation.h>
#include <webots_ros/field_get_string.h>
#include <webots_ros/field_get_type.h>
#include <webots_ros/field_get_type_name.h>
#include <webots_ros/field_get_vec2f.h>
#include <webots_ros/field_get_vec3f.h>
#include <webots_ros/field_import_node.h>
#include <webots_ros/field_import_node_from_string.h>
#include <webots_ros/field_remove.h>
#include <webots_ros/field_remove_node.h>
#include <webots_ros/field_set_bool.h>
#include <webots_ros/field_set_color.h>
#include <webots_ros/field_set_float.h>
#include <webots_ros/field_set_int32.h>
#include <webots_ros/field_set_rotation.h>
#include <webots_ros/field_set_string.h>
#include <webots_ros/field_set_vec2f.h>
#include <webots_ros/field_set_vec3f.h>

using namespace webots;

class RosSupervisor {
public:
  RosSupervisor(Ros *ros, Supervisor *supervisor);
  virtual ~RosSupervisor();

  bool simulationQuitCallback(webots_ros::set_int::Request &req, webots_ros::set_int::Response &res);
  bool simulationRevertCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res);
  bool simulationResetCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res);
  bool simulationResetPhysicsCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res);
  bool simulationGetModeCallback(webots_ros::get_int::Request &req, webots_ros::get_int::Response &res);
  bool simulationSetModeCallback(webots_ros::set_int::Request &req, webots_ros::set_int::Response &res);
  bool worldLoadCallback(webots_ros::set_string::Request &req, webots_ros::set_string::Response &res);
  bool worldSaveCallback(webots_ros::set_string::Request &req, webots_ros::set_string::Response &res);
  bool worldReloadCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res);
  bool exportImageCallback(webots_ros::save_image::Request &req, webots_ros::save_image::Response &res);
  bool animationStartRecordingCallback(webots_ros::set_string::Request &req, webots_ros::set_string::Response &res);
  bool animationStopRecordingCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res);
  bool movieStartRecordingCallback(webots_ros::supervisor_movie_start_recording::Request &req,
                                   webots_ros::supervisor_movie_start_recording::Response &res);
  bool movieStopRecordingCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res);
  bool movieFailedCallback(webots_ros::node_get_status::Request &req, webots_ros::node_get_status::Response &res);
  bool movieIsReadyCallback(webots_ros::node_get_status::Request &req, webots_ros::node_get_status::Response &res);
  bool virtualRealityHeadsetGetOrientationCallback(
    webots_ros::supervisor_virtual_reality_headset_get_orientation::Request &req,
    webots_ros::supervisor_virtual_reality_headset_get_orientation::Response &res);
  bool virtualRealityHeadsetGetPositionCallback(webots_ros::supervisor_virtual_reality_headset_get_position::Request &req,
                                                webots_ros::supervisor_virtual_reality_headset_get_position::Response &res);
  bool virtualRealityHeadsetIsUsedCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res);

  bool setLabelCallback(webots_ros::supervisor_set_label::Request &req, webots_ros::supervisor_set_label::Response &res);
  bool getRootCallback(webots_ros::get_uint64::Request &req, webots_ros::get_uint64::Response &res);
  bool getSelfCallback(webots_ros::get_uint64::Request &req, webots_ros::get_uint64::Response &res);
  bool getFromDefCallback(webots_ros::supervisor_get_from_def::Request &req,
                          webots_ros::supervisor_get_from_def::Response &res);
  bool getFromIdCallback(webots_ros::supervisor_get_from_id::Request &req, webots_ros::supervisor_get_from_id::Response &res);
  bool getFromDeviceCallback(webots_ros::supervisor_get_from_string::Request &req,
                             webots_ros::supervisor_get_from_string::Response &res);
  bool getSelectedCallback(webots_ros::get_uint64::Request &req, webots_ros::get_uint64::Response &res);

  bool nodeGetIdCallback(webots_ros::node_get_id::Request &req, webots_ros::node_get_id::Response &res);
  bool nodeGetTypeCallback(webots_ros::node_get_type::Request &req, webots_ros::node_get_type::Response &res);
  bool nodeGetTypeNameCallback(webots_ros::node_get_name::Request &req, webots_ros::node_get_name::Response &res);
  bool nodeGetDefCallback(webots_ros::node_get_name::Request &req, webots_ros::node_get_name::Response &res);
  bool nodeGetBaseTypeNameCallback(webots_ros::node_get_name::Request &req, webots_ros::node_get_name::Response &res);
  bool nodeGetParentNodeCallback(webots_ros::node_get_parent_node::Request &req,
                                 webots_ros::node_get_parent_node::Response &res);
  bool nodeIsProtoCallback(webots_ros::node_is_proto::Request &req, webots_ros::node_is_proto::Response &res);
  bool nodeGetPositionCallback(webots_ros::node_get_position::Request &req, webots_ros::node_get_position::Response &res);
  bool nodeGetOrientationCallback(webots_ros::node_get_orientation::Request &req,
                                  webots_ros::node_get_orientation::Response &res);
  bool nodeGetPoseCallback(webots_ros::node_get_pose::Request &req, webots_ros::node_get_pose::Response &res);
  bool nodeGetCenterOfMassCallback(webots_ros::node_get_center_of_mass::Request &req,
                                   webots_ros::node_get_center_of_mass::Response &res);
  bool nodeGetNumberOfContactPointsCallback(webots_ros::node_get_number_of_contact_points::Request &req,
                                            webots_ros::node_get_number_of_contact_points::Response &res);
  bool nodeGetContactPointCallback(webots_ros::node_get_contact_point::Request &req,
                                   webots_ros::node_get_contact_point::Response &res);
  bool nodeGetContactPointNodeCallback(webots_ros::node_get_contact_point_node::Request &req,
                                       webots_ros::node_get_contact_point_node::Response &res);
  bool nodeGetStaticBalanceCallback(webots_ros::node_get_static_balance::Request &req,
                                    webots_ros::node_get_static_balance::Response &res);
  bool nodeGetVelocityCallback(webots_ros::node_get_velocity::Request &req, webots_ros::node_get_velocity::Response &res);
  bool nodeSetVelocityCallback(webots_ros::node_set_velocity::Request &req, webots_ros::node_set_velocity::Response &res);
  bool nodeAddForceCallback(webots_ros::node_add_force_or_torque::Request &req,
                            webots_ros::node_add_force_or_torque::Response &res);
  bool nodeAddForceWithOffsetCallback(webots_ros::node_add_force_with_offset::Request &req,
                                      webots_ros::node_add_force_with_offset::Response &res);
  bool nodeAddTorqueCallback(webots_ros::node_add_force_or_torque::Request &req,
                             webots_ros::node_add_force_or_torque::Response &res);
  bool nodeGetFieldCallback(webots_ros::node_get_field::Request &req, webots_ros::node_get_field::Response &res);
  bool nodeMoveViewpointCallback(webots_ros::node_move_viewpoint::Request &req, webots_ros::node_move_viewpoint::Response &res);
  bool nodeSetVisibilityCallback(webots_ros::node_set_visibility::Request &req, webots_ros::node_set_visibility::Response &res);
  bool nodeRemoveCallback(webots_ros::node_remove::Request &req, webots_ros::node_remove::Response &res);
  bool nodeExportStringCallback(webots_ros::node_get_string::Request &req, webots_ros::node_get_string::Response &res);
  bool nodeResetPhysicsCallback(webots_ros::node_reset_functions::Request &req,
                                webots_ros::node_reset_functions::Response &res);
  bool nodeSaveStateCallback(webots_ros::node_set_string::Request &req, webots_ros::node_set_string::Response &res);
  bool nodeLoadStateCallback(webots_ros::node_set_string::Request &req, webots_ros::node_set_string::Response &res);
  bool nodeRestartControllerCallback(webots_ros::node_reset_functions::Request &req,
                                     webots_ros::node_reset_functions::Response &res);
  bool nodeEnablePoseTrackingCallback(webots_ros::node_enable_pose_tracking::Request &req,
                                      webots_ros::node_enable_pose_tracking::Response &res);
  bool nodeDisablePoseTrackingCallback(webots_ros::node_disable_pose_tracking::Request &req,
                                       webots_ros::node_disable_pose_tracking::Response &res);

  bool fieldGetTypeCallback(webots_ros::field_get_type::Request &req, webots_ros::field_get_type::Response &res);
  bool fieldGetTypeNameCallback(webots_ros::field_get_type_name::Request &req, webots_ros::field_get_type_name::Response &res);
  bool fieldGetCountCallback(webots_ros::field_get_count::Request &req, webots_ros::field_get_count::Response &res);
  bool fieldGetBoolCallback(webots_ros::field_get_bool::Request &req, webots_ros::field_get_bool::Response &res);
  bool fieldGetInt32Callback(webots_ros::field_get_int32::Request &req, webots_ros::field_get_int32::Response &res);
  bool fieldGetFloatCallback(webots_ros::field_get_float::Request &req, webots_ros::field_get_float::Response &res);
  bool fieldGetVec2fCallback(webots_ros::field_get_vec2f::Request &req, webots_ros::field_get_vec2f::Response &res);
  bool fieldGetVec3fCallback(webots_ros::field_get_vec3f::Request &req, webots_ros::field_get_vec3f::Response &res);
  bool fieldGetRotationCallback(webots_ros::field_get_rotation::Request &req, webots_ros::field_get_rotation::Response &res);
  bool fieldGetColorCallback(webots_ros::field_get_color::Request &req, webots_ros::field_get_color::Response &res);
  bool fieldGetStringCallback(webots_ros::field_get_string::Request &req, webots_ros::field_get_string::Response &res);
  bool fieldGetNodeCallback(webots_ros::field_get_node::Request &req, webots_ros::field_get_node::Response &res);
  bool fieldSetBoolCallback(webots_ros::field_set_bool::Request &req, webots_ros::field_set_bool::Response &res);
  bool fieldSetInt32Callback(webots_ros::field_set_int32::Request &req, webots_ros::field_set_int32::Response &res);
  bool fieldSetFloatCallback(webots_ros::field_set_float::Request &req, webots_ros::field_set_float::Response &res);
  bool fieldSetVec2fCallback(webots_ros::field_set_vec2f::Request &req, webots_ros::field_set_vec2f::Response &res);
  bool fieldSetVec3fCallback(webots_ros::field_set_vec3f::Request &req, webots_ros::field_set_vec3f::Response &res);
  bool fieldSetRotationCallback(webots_ros::field_set_rotation::Request &req, webots_ros::field_set_rotation::Response &res);
  bool fieldSetColorCallback(webots_ros::field_set_color::Request &req, webots_ros::field_set_color::Response &res);
  bool fieldSetStringCallback(webots_ros::field_set_string::Request &req, webots_ros::field_set_string::Response &res);
  bool fieldInsertBoolCallback(webots_ros::field_set_bool::Request &req, webots_ros::field_set_bool::Response &res);
  bool fieldInsertInt32Callback(webots_ros::field_set_int32::Request &req, webots_ros::field_set_int32::Response &res);
  bool fieldInsertFloatCallback(webots_ros::field_set_float::Request &req, webots_ros::field_set_float::Response &res);
  bool fieldInsertVec2fCallback(webots_ros::field_set_vec2f::Request &req, webots_ros::field_set_vec2f::Response &res);
  bool fieldInsertVec3fCallback(webots_ros::field_set_vec3f::Request &req, webots_ros::field_set_vec3f::Response &res);
  bool fieldInsertRotationCallback(webots_ros::field_set_rotation::Request &req, webots_ros::field_set_rotation::Response &res);
  bool fieldInsertColorCallback(webots_ros::field_set_color::Request &req, webots_ros::field_set_color::Response &res);
  bool fieldInsertStringCallback(webots_ros::field_set_string::Request &req, webots_ros::field_set_string::Response &res);
  bool fieldRemoveCallback(webots_ros::field_remove::Request &req, webots_ros::field_remove::Response &res);
  bool fieldImportNodeCallback(webots_ros::field_import_node::Request &req, webots_ros::field_import_node::Response &res);
  bool fieldImportNodeFromStringCallback(webots_ros::field_import_node_from_string::Request &req,
                                         webots_ros::field_import_node_from_string::Response &res);
  bool fieldRemoveNodeCallback(webots_ros::field_remove_node::Request &req, webots_ros::field_remove_node::Response &res);
  bool fieldEnableSFTrackingCallback(webots_ros::field_enable_sf_tracking::Request &req,
                                     webots_ros::field_enable_sf_tracking::Response &res);
  bool fieldDisableSFTrackingCallback(webots_ros::field_disable_sf_tracking::Request &req,
                                      webots_ros::field_disable_sf_tracking::Response &res);

private:
  Supervisor *mSupervisor;
  Ros *mRos;
  ros::ServiceServer mSimulationQuitServer;
  ros::ServiceServer mSimulationRevertServer;
  ros::ServiceServer mSimulationResetServer;
  ros::ServiceServer mSimulationResetPhysicsServer;
  ros::ServiceServer mSimulationGetModeServer;
  ros::ServiceServer mSimulationSetModeServer;
  ros::ServiceServer mWorldLoadServer;
  ros::ServiceServer mWorldSaveServer;
  ros::ServiceServer mWorldReloadServer;
  ros::ServiceServer mExportImageServer;
  ros::ServiceServer mAnimationStartRecordingServer;
  ros::ServiceServer mAnimationStopRecordingServer;
  ros::ServiceServer mMovieStartRecordingServer;
  ros::ServiceServer mMovieStopRecordingServer;
  ros::ServiceServer mMovieFailedServer;
  ros::ServiceServer mMovieIsReadyServer;
  ros::ServiceServer mSetLabelServer;
  ros::ServiceServer mGetRootServer;
  ros::ServiceServer mGetSelfServer;
  ros::ServiceServer mGetFromDefServer;
  ros::ServiceServer mGetFromIdServer;
  ros::ServiceServer mGetFromDeviceServer;
  ros::ServiceServer mGetSelectedServer;
  ros::ServiceServer mVirtualRealityHeadsetGetOrientationServer;
  ros::ServiceServer mVirtualRealityHeadsetGetPositionServer;
  ros::ServiceServer mVirtualRealityHeadsetIsUsedServer;

  ros::ServiceServer mNodeGetIdServer;
  ros::ServiceServer mNodeGetTypeServer;
  ros::ServiceServer mNodeGetTypeNameServer;
  ros::ServiceServer mNodeGetDefServer;
  ros::ServiceServer mNodeGetBaseTypeNameServer;
  ros::ServiceServer mNodeGetParentNodeServer;
  ros::ServiceServer mNodeIsProtoServer;
  ros::ServiceServer mNodeGetPositionServer;
  ros::ServiceServer mNodeGetOrientationServer;
  ros::ServiceServer mNodeGetPoseServer;
  ros::ServiceServer mNodeGetCenterOfMassServer;
  ros::ServiceServer mNodeGetNumberOfContactPointsServer;
  ros::ServiceServer mNodeGetContactPointServer;
  ros::ServiceServer mNodeGetContactPointNodeServer;
  ros::ServiceServer mNodeGetStaticBalanceServer;
  ros::ServiceServer mNodeGetVelocityServer;
  ros::ServiceServer mNodeSetVelocityServer;
  ros::ServiceServer mNodeAddForceServer;
  ros::ServiceServer mNodeAddForceWithOffsetServer;
  ros::ServiceServer mNodeAddTorqueServer;
  ros::ServiceServer mNodeGetFieldServer;
  ros::ServiceServer mNodeMoveViewpointServer;
  ros::ServiceServer mNodeSetVisibilityServer;
  ros::ServiceServer mNodeRemoveServer;
  ros::ServiceServer mNodeExportStringServer;
  ros::ServiceServer mNodeResetPhysicsServer;
  ros::ServiceServer mNodeRestartControllerServer;
  ros::ServiceServer mNodeSaveStateServer;
  ros::ServiceServer mNodeLoadStateServer;
  ros::ServiceServer mNodeEnablePoseTrackingServer;
  ros::ServiceServer mNodeDisablePoseTrackingServer;

  ros::ServiceServer mFieldGetTypeServer;
  ros::ServiceServer mFieldGetTypeNameServer;
  ros::ServiceServer mFieldGetCountServer;
  ros::ServiceServer mFieldGetBoolServer;
  ros::ServiceServer mFieldGetInt32Server;
  ros::ServiceServer mFieldGetFloatServer;
  ros::ServiceServer mFieldGetVec2fServer;
  ros::ServiceServer mFieldGetVec3fServer;
  ros::ServiceServer mFieldGetRotationServer;
  ros::ServiceServer mFieldGetColorServer;
  ros::ServiceServer mFieldGetStringServer;
  ros::ServiceServer mFieldGetNodeServer;
  ros::ServiceServer mFieldSetBoolServer;
  ros::ServiceServer mFieldSetInt32Server;
  ros::ServiceServer mFieldSetFloatServer;
  ros::ServiceServer mFieldSetVec2fServer;
  ros::ServiceServer mFieldSetVec3fServer;
  ros::ServiceServer mFieldSetRotationServer;
  ros::ServiceServer mFieldSetColorServer;
  ros::ServiceServer mFieldSetStringServer;
  ros::ServiceServer mFieldInsertBoolServer;
  ros::ServiceServer mFieldInsertInt32Server;
  ros::ServiceServer mFieldInsertFloatServer;
  ros::ServiceServer mFieldInsertVec2fServer;
  ros::ServiceServer mFieldInsertVec3fServer;
  ros::ServiceServer mFieldInsertRotationServer;
  ros::ServiceServer mFieldInsertColorServer;
  ros::ServiceServer mFieldInsertStringServer;
  ros::ServiceServer mFieldRemoveServer;
  ros::ServiceServer mFieldImportNodeServer;
  ros::ServiceServer mFieldImportNodeFromStringServer;
  ros::ServiceServer mFieldRemoveNodeServer;
  ros::ServiceServer mFieldEnableSFTrackingServer;
  ros::ServiceServer mFieldDisableSFTrackingServer;
};

#endif  // ROS_SUPERVISOR_HPP
