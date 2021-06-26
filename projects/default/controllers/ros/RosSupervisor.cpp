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

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/LED.hpp>, etc.
// and/or to add some other includes
#include "RosSupervisor.hpp"

#include "RosMathUtils.hpp"

RosSupervisor::RosSupervisor(Ros *ros, Supervisor *supervisor) {
  mRos = ros;
  mSupervisor = supervisor;

  mSimulationQuitServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/simulation_quit",
                                                               &RosSupervisor::simulationQuitCallback, this);
  mSimulationRevertServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/simulation_revert",
                                                                 &RosSupervisor::simulationRevertCallback, this);
  mSimulationResetServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/simulation_reset",
                                                                &RosSupervisor::simulationResetCallback, this);
  mSimulationResetPhysicsServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/simulation_reset_physics",
                                                                       &RosSupervisor::simulationResetPhysicsCallback, this);
  mSimulationGetModeServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/simulation_get_mode",
                                                                  &RosSupervisor::simulationGetModeCallback, this);
  mSimulationSetModeServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/simulation_set_mode",
                                                                  &RosSupervisor::simulationSetModeCallback, this);
  mWorldLoadServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/world_load", &RosSupervisor::worldLoadCallback, this);
  mWorldSaveServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/world_save", &RosSupervisor::worldSaveCallback, this);
  mWorldReloadServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/world_reload", &RosSupervisor::worldReloadCallback, this);
  mExportImageServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/export_image", &RosSupervisor::exportImageCallback, this);
  mAnimationStartRecordingServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/animation_start_recording",
                                                                        &RosSupervisor::animationStartRecordingCallback, this);
  mAnimationStopRecordingServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/animation_stop_recording",
                                                                       &RosSupervisor::animationStopRecordingCallback, this);
  mMovieStartRecordingServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/movie_start_recording",
                                                                    &RosSupervisor::movieStartRecordingCallback, this);
  mMovieStopRecordingServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/movie_stop_recording",
                                                                   &RosSupervisor::movieStopRecordingCallback, this);
  mMovieFailedServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/" + "movie_failed",
                                                            &RosSupervisor::movieFailedCallback, this);
  mMovieIsReadyServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/" + "movie_is_ready",
                                                             &RosSupervisor::movieIsReadyCallback, this);
  mSetLabelServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/set_label", &RosSupervisor::setLabelCallback, this);
  mGetRootServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/get_root", &RosSupervisor::getRootCallback, this);
  mGetSelfServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/get_self", &RosSupervisor::getSelfCallback, this);
  mGetFromDefServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/get_from_def", &RosSupervisor::getFromDefCallback, this);
  mGetFromIdServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/get_from_id", &RosSupervisor::getFromIdCallback, this);
  mGetFromDeviceServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/get_from_device",
                                                              &RosSupervisor::getFromDeviceCallback, this);
  mGetSelectedServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/get_selected", &RosSupervisor::getSelectedCallback, this);
  mVirtualRealityHeadsetGetOrientationServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/vitual_reality_headset_get_orientation",
                                         &RosSupervisor::virtualRealityHeadsetGetOrientationCallback, this);
  mVirtualRealityHeadsetGetPositionServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/vitual_reality_headset_get_position",
                                         &RosSupervisor::virtualRealityHeadsetGetPositionCallback, this);
  mVirtualRealityHeadsetIsUsedServer = mRos->nodeHandle()->advertiseService(
    (ros->name()) + "/supervisor/vitual_reality_headset_is_used", &RosSupervisor::virtualRealityHeadsetIsUsedCallback, this);

  mNodeGetIdServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_id", &RosSupervisor::nodeGetIdCallback, this);
  mNodeGetTypeServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_type",
                                                            &RosSupervisor::nodeGetTypeCallback, this);
  mNodeGetTypeNameServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_type_name",
                                                                &RosSupervisor::nodeGetTypeNameCallback, this);
  mNodeGetDefServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_def", &RosSupervisor::nodeGetDefCallback, this);
  mNodeGetBaseTypeNameServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_base_type_name",
                                                                    &RosSupervisor::nodeGetBaseTypeNameCallback, this);
  mNodeGetParentNodeServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_parent_node",
                                                                  &RosSupervisor::nodeGetParentNodeCallback, this);
  mNodeIsProtoServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/is_proto",
                                                            &RosSupervisor::nodeIsProtoCallback, this);
  mNodeGetPositionServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_position",
                                                                &RosSupervisor::nodeGetPositionCallback, this);
  mNodeGetOrientationServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_orientation",
                                                                   &RosSupervisor::nodeGetOrientationCallback, this);
  mNodeGetPoseServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_pose",
                                                            &RosSupervisor::nodeGetPoseCallback, this);
  mNodeGetCenterOfMassServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_center_of_mass",
                                                                    &RosSupervisor::nodeGetCenterOfMassCallback, this);
  mNodeGetNumberOfContactPointsServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_number_of_contact_points",
                                         &RosSupervisor::nodeGetNumberOfContactPointsCallback, this);
  mNodeGetContactPointServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_contact_point",
                                                                    &RosSupervisor::nodeGetContactPointCallback, this);
  mNodeGetContactPointNodeServer = mRos->nodeHandle()->advertiseService(
    (ros->name()) + "/supervisor/node/get_contact_point_node", &RosSupervisor::nodeGetContactPointNodeCallback, this);
  mNodeGetStaticBalanceServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_static_balance",
                                                                     &RosSupervisor::nodeGetStaticBalanceCallback, this);
  mNodeGetVelocityServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_velocity",
                                                                &RosSupervisor::nodeGetVelocityCallback, this);
  mNodeSetVelocityServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/set_velocity",
                                                                &RosSupervisor::nodeSetVelocityCallback, this);
  mNodeAddForceServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/add_force",
                                                             &RosSupervisor::nodeAddForceCallback, this);
  mNodeAddForceWithOffsetServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/add_force_with_offset",
                                                                       &RosSupervisor::nodeAddForceWithOffsetCallback, this);
  mNodeAddTorqueServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/add_torque",
                                                              &RosSupervisor::nodeAddTorqueCallback, this);
  mNodeGetFieldServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/get_field",
                                                             &RosSupervisor::nodeGetFieldCallback, this);
  mNodeMoveViewpointServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/move_viewpoint",
                                                                  &RosSupervisor::nodeMoveViewpointCallback, this);
  mNodeSetVisibilityServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/set_visibility",
                                                                  &RosSupervisor::nodeSetVisibilityCallback, this);
  mNodeRemoveServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/remove", &RosSupervisor::nodeRemoveCallback, this);
  mNodeExportStringServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/export_string",
                                                                 &RosSupervisor::nodeExportStringCallback, this);
  mNodeResetPhysicsServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/reset_physics",
                                                                 &RosSupervisor::nodeResetPhysicsCallback, this);
  mNodeSaveStateServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/save_state",
                                                              &RosSupervisor::nodeSaveStateCallback, this);
  mNodeLoadStateServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/load_state",
                                                              &RosSupervisor::nodeLoadStateCallback, this);
  mNodeRestartControllerServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/restart_controller",
                                                                      &RosSupervisor::nodeRestartControllerCallback, this);
  mNodeEnablePoseTrackingServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/node/enable_pose_tracking",
                                                                       &RosSupervisor::nodeEnablePoseTrackingCallback, this);
  mNodeDisablePoseTrackingServer = mRos->nodeHandle()->advertiseService(
    (ros->name()) + "/supervisor/node/disable_pose_tracking", &RosSupervisor::nodeDisablePoseTrackingCallback, this);

  mFieldGetTypeServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_type",
                                                             &RosSupervisor::fieldGetTypeCallback, this);
  mFieldGetTypeNameServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_type_name",
                                                                 &RosSupervisor::fieldGetTypeNameCallback, this);
  mFieldGetCountServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_count",
                                                              &RosSupervisor::fieldGetCountCallback, this);
  mFieldGetBoolServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_bool",
                                                             &RosSupervisor::fieldGetBoolCallback, this);
  mFieldGetInt32Server = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_int32",
                                                              &RosSupervisor::fieldGetInt32Callback, this);
  mFieldGetFloatServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_float",
                                                              &RosSupervisor::fieldGetFloatCallback, this);
  mFieldGetVec2fServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_vec2f",
                                                              &RosSupervisor::fieldGetVec2fCallback, this);
  mFieldGetVec3fServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_vec3f",
                                                              &RosSupervisor::fieldGetVec3fCallback, this);
  mFieldGetRotationServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_rotation",
                                                                 &RosSupervisor::fieldGetRotationCallback, this);
  mFieldGetColorServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_color",
                                                              &RosSupervisor::fieldGetColorCallback, this);
  mFieldGetStringServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_string",
                                                               &RosSupervisor::fieldGetStringCallback, this);
  mFieldGetNodeServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/get_node",
                                                             &RosSupervisor::fieldGetNodeCallback, this);
  mFieldSetBoolServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/set_bool",
                                                             &RosSupervisor::fieldSetBoolCallback, this);
  mFieldSetInt32Server = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/set_int32",
                                                              &RosSupervisor::fieldSetInt32Callback, this);
  mFieldSetFloatServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/set_float",
                                                              &RosSupervisor::fieldSetFloatCallback, this);
  mFieldSetVec2fServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/set_vec2f",
                                                              &RosSupervisor::fieldSetVec2fCallback, this);
  mFieldSetVec3fServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/set_vec3f",
                                                              &RosSupervisor::fieldSetVec3fCallback, this);
  mFieldSetRotationServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/set_rotation",
                                                                 &RosSupervisor::fieldSetRotationCallback, this);
  mFieldSetColorServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/set_color",
                                                              &RosSupervisor::fieldSetColorCallback, this);
  mFieldSetStringServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/set_string",
                                                               &RosSupervisor::fieldSetStringCallback, this);
  mFieldInsertBoolServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/insert_bool",
                                                                &RosSupervisor::fieldInsertBoolCallback, this);
  mFieldInsertInt32Server = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/insert_int32",
                                                                 &RosSupervisor::fieldInsertInt32Callback, this);
  mFieldInsertFloatServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/insert_float",
                                                                 &RosSupervisor::fieldInsertFloatCallback, this);
  mFieldInsertVec2fServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/insert_vec2f",
                                                                 &RosSupervisor::fieldInsertVec2fCallback, this);
  mFieldInsertVec3fServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/insert_vec3f",
                                                                 &RosSupervisor::fieldInsertVec3fCallback, this);
  mFieldInsertRotationServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/insert_rotation",
                                                                    &RosSupervisor::fieldInsertRotationCallback, this);
  mFieldInsertColorServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/insert_color",
                                                                 &RosSupervisor::fieldInsertColorCallback, this);
  mFieldInsertStringServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/insert_string",
                                                                  &RosSupervisor::fieldInsertStringCallback, this);
  mFieldRemoveServer =
    mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/remove", &RosSupervisor::fieldRemoveCallback, this);
  mFieldImportNodeServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/import_node",
                                                                &RosSupervisor::fieldImportNodeCallback, this);
  mFieldImportNodeFromStringServer = mRos->nodeHandle()->advertiseService(
    (ros->name()) + "/supervisor/field/import_node_from_string", &RosSupervisor::fieldImportNodeFromStringCallback, this);
  mFieldRemoveNodeServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/remove_node",
                                                                &RosSupervisor::fieldRemoveNodeCallback, this);

  mFieldEnableSFTrackingServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/enable_sf_tracking",
                                                                      &RosSupervisor::fieldEnableSFTrackingCallback, this);
  mFieldDisableSFTrackingServer = mRos->nodeHandle()->advertiseService((ros->name()) + "/supervisor/field/disable_sf_tracking",
                                                                       &RosSupervisor::fieldDisableSFTrackingCallback, this);
}

RosSupervisor::~RosSupervisor() {
  mSimulationQuitServer.shutdown();
  mSimulationRevertServer.shutdown();
  mSimulationResetServer.shutdown();
  mSimulationResetPhysicsServer.shutdown();
  mWorldLoadServer.shutdown();
  mWorldSaveServer.shutdown();
  mWorldReloadServer.shutdown();
  mExportImageServer.shutdown();
  mAnimationStartRecordingServer.shutdown();
  mAnimationStopRecordingServer.shutdown();
  mMovieStartRecordingServer.shutdown();
  mMovieStopRecordingServer.shutdown();
  mMovieIsReadyServer.shutdown();
  mMovieFailedServer.shutdown();
  mSetLabelServer.shutdown();
  mGetRootServer.shutdown();
  mGetSelfServer.shutdown();
  mGetFromDefServer.shutdown();
  mGetFromIdServer.shutdown();
  mGetFromDeviceServer.shutdown();
  mGetSelectedServer.shutdown();
  mVirtualRealityHeadsetGetOrientationServer.shutdown();
  mVirtualRealityHeadsetGetPositionServer.shutdown();
  mVirtualRealityHeadsetIsUsedServer.shutdown();

  mNodeGetIdServer.shutdown();
  mNodeGetTypeServer.shutdown();
  mNodeGetTypeNameServer.shutdown();
  mNodeGetDefServer.shutdown();
  mNodeGetBaseTypeNameServer.shutdown();
  mNodeGetParentNodeServer.shutdown();
  mNodeGetPositionServer.shutdown();
  mNodeGetOrientationServer.shutdown();
  mNodeGetPoseServer.shutdown();
  mNodeGetCenterOfMassServer.shutdown();
  mNodeGetNumberOfContactPointsServer.shutdown();
  mNodeGetContactPointServer.shutdown();
  mNodeGetContactPointNodeServer.shutdown();
  mNodeGetStaticBalanceServer.shutdown();
  mNodeGetVelocityServer.shutdown();
  mNodeSetVelocityServer.shutdown();
  mNodeGetFieldServer.shutdown();
  mNodeMoveViewpointServer.shutdown();
  mNodeSetVisibilityServer.shutdown();
  mNodeRemoveServer.shutdown();
  mNodeExportStringServer.shutdown();
  mNodeResetPhysicsServer.shutdown();
  mNodeRestartControllerServer.shutdown();
  mNodeEnablePoseTrackingServer.shutdown();
  mNodeDisablePoseTrackingServer.shutdown();

  mFieldGetTypeServer.shutdown();
  mFieldGetTypeNameServer.shutdown();
  mFieldGetCountServer.shutdown();
  mFieldGetBoolServer.shutdown();
  mFieldGetInt32Server.shutdown();
  mFieldGetFloatServer.shutdown();
  mFieldGetVec2fServer.shutdown();
  mFieldGetVec3fServer.shutdown();
  mFieldGetRotationServer.shutdown();
  mFieldGetColorServer.shutdown();
  mFieldGetStringServer.shutdown();
  mFieldGetNodeServer.shutdown();
  mFieldSetBoolServer.shutdown();
  mFieldSetInt32Server.shutdown();
  mFieldSetFloatServer.shutdown();
  mFieldSetVec2fServer.shutdown();
  mFieldSetVec3fServer.shutdown();
  mFieldSetRotationServer.shutdown();
  mFieldSetColorServer.shutdown();
  mFieldSetStringServer.shutdown();
  mFieldInsertBoolServer.shutdown();
  mFieldInsertInt32Server.shutdown();
  mFieldInsertFloatServer.shutdown();
  mFieldInsertVec2fServer.shutdown();
  mFieldInsertVec3fServer.shutdown();
  mFieldInsertRotationServer.shutdown();
  mFieldInsertColorServer.shutdown();
  mFieldInsertStringServer.shutdown();
  mFieldRemoveServer.shutdown();
  mFieldImportNodeServer.shutdown();
  mFieldImportNodeFromStringServer.shutdown();
  mFieldRemoveNodeServer.shutdown();
  mFieldEnableSFTrackingServer.shutdown();
  mFieldDisableSFTrackingServer.shutdown();
}

bool RosSupervisor::simulationQuitCallback(webots_ros::set_int::Request &req, webots_ros::set_int::Response &res) {
  assert(mSupervisor);
  mSupervisor->simulationQuit(req.value);
  res.success = true;
  return true;
}

bool RosSupervisor::simulationRevertCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res) {
  assert(mSupervisor);
  mSupervisor->simulationRevert();
  res.value = true;
  return true;
}

bool RosSupervisor::simulationResetCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res) {
  assert(mSupervisor);
  mSupervisor->simulationReset();
  res.value = true;
  return true;
}

bool RosSupervisor::simulationResetPhysicsCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res) {
  assert(mSupervisor);
  mSupervisor->simulationResetPhysics();
  res.value = true;
  return true;
}

bool RosSupervisor::simulationGetModeCallback(webots_ros::get_int::Request &req, webots_ros::get_int::Response &res) {
  assert(mSupervisor);
  res.value = mSupervisor->simulationGetMode();
  return true;
}

bool RosSupervisor::simulationSetModeCallback(webots_ros::set_int::Request &req, webots_ros::set_int::Response &res) {
  assert(mSupervisor);
  mSupervisor->simulationSetMode(Supervisor::SimulationMode(req.value));
  res.success = true;
  return true;
}

bool RosSupervisor::worldLoadCallback(webots_ros::set_string::Request &req, webots_ros::set_string::Response &res) {
  assert(mSupervisor);
  mSupervisor->worldLoad(req.value);
  res.success = 1;
  return true;
}

bool RosSupervisor::worldSaveCallback(webots_ros::set_string::Request &req, webots_ros::set_string::Response &res) {
  assert(mSupervisor);
  if (req.value.data() != NULL)
    res.success = mSupervisor->worldSave(req.value);
  else
    res.success = mSupervisor->worldSave();
  return true;
}

bool RosSupervisor::worldReloadCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res) {
  assert(mSupervisor);
  mSupervisor->worldReload();
  res.value = true;
  return true;
}

bool RosSupervisor::exportImageCallback(webots_ros::save_image::Request &req, webots_ros::save_image::Response &res) {
  assert(mSupervisor);
  mSupervisor->exportImage(req.filename, req.quality);
  res.success = 1;
  return true;
}

bool RosSupervisor::animationStartRecordingCallback(webots_ros::set_string::Request &req,
                                                    webots_ros::set_string::Response &res) {
  assert(mSupervisor);
  res.success = mSupervisor->animationStartRecording(req.value);
  return true;
}

bool RosSupervisor::animationStopRecordingCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res) {
  assert(mSupervisor);
  res.value = mSupervisor->animationStopRecording();
  return true;
}

bool RosSupervisor::movieStartRecordingCallback(webots_ros::supervisor_movie_start_recording::Request &req,
                                                webots_ros::supervisor_movie_start_recording::Response &res) {
  assert(mSupervisor);
  mSupervisor->movieStartRecording(req.filename, req.width, req.height, req.codec, req.quality, req.acceleration, req.caption);
  res.success = 1;
  return true;
}

bool RosSupervisor::movieStopRecordingCallback(webots_ros::get_bool::Request &req, webots_ros::get_bool::Response &res) {
  assert(mSupervisor);
  mSupervisor->movieStopRecording();
  res.value = true;
  return true;
}

bool RosSupervisor::movieFailedCallback(webots_ros::node_get_status::Request &req, webots_ros::node_get_status::Response &res) {
  assert(mSupervisor);
  res.status = mSupervisor->movieFailed();
  return true;
}

bool RosSupervisor::movieIsReadyCallback(webots_ros::node_get_status::Request &req,
                                         webots_ros::node_get_status::Response &res) {
  assert(mSupervisor);
  res.status = mSupervisor->movieIsReady();
  return true;
}

bool RosSupervisor::setLabelCallback(webots_ros::supervisor_set_label::Request &req,
                                     webots_ros::supervisor_set_label::Response &res) {
  assert(mSupervisor);
  mSupervisor->setLabel(req.id, req.label, req.xpos, req.ypos, req.size, req.color, req.transparency, req.font);
  res.success = 1;
  return true;
}

bool RosSupervisor::getRootCallback(webots_ros::get_uint64::Request &req, webots_ros::get_uint64::Response &res) {
  assert(mSupervisor);
  res.value = reinterpret_cast<uint64_t>(mSupervisor->getRoot());
  return true;
}

bool RosSupervisor::getSelfCallback(webots_ros::get_uint64::Request &req, webots_ros::get_uint64::Response &res) {
  assert(mSupervisor);
  res.value = reinterpret_cast<uint64_t>(mSupervisor->getSelf());
  return true;
}

bool RosSupervisor::getFromDefCallback(webots_ros::supervisor_get_from_def::Request &req,
                                       webots_ros::supervisor_get_from_def::Response &res) {
  assert(mSupervisor);
  if (req.proto) {
    Node *node = reinterpret_cast<Node *>(req.proto);
    res.node = reinterpret_cast<uint64_t>(node->getFromProtoDef(req.name));
  } else
    res.node = reinterpret_cast<uint64_t>(mSupervisor->getFromDef(req.name));
  return true;
}

bool RosSupervisor::getFromIdCallback(webots_ros::supervisor_get_from_id::Request &req,
                                      webots_ros::supervisor_get_from_id::Response &res) {
  assert(mSupervisor);
  res.node = reinterpret_cast<uint64_t>(mSupervisor->getFromId(req.id));
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::getFromDeviceCallback(webots_ros::supervisor_get_from_string::Request &req,
                                          webots_ros::supervisor_get_from_string::Response &res) {
  assert(mSupervisor);
  const Device *device = mRos->getDevice(req.value);
  res.node = reinterpret_cast<uint64_t>(mSupervisor->getFromDevice(device));
  return true;
}

bool RosSupervisor::getSelectedCallback(webots_ros::get_uint64::Request &req, webots_ros::get_uint64::Response &res) {
  assert(mSupervisor);
  res.value = reinterpret_cast<uint64_t>(mSupervisor->getSelected());
  return true;
}

bool RosSupervisor::virtualRealityHeadsetGetOrientationCallback(
  webots_ros::supervisor_virtual_reality_headset_get_orientation::Request &req,
  webots_ros::supervisor_virtual_reality_headset_get_orientation::Response &res) {
  assert(mSupervisor);
  const double *matrix = mSupervisor->virtualRealityHeadsetGetOrientation();
  RosMathUtils::matrixToQuaternion(matrix, res.orientation);
  return true;
}

bool RosSupervisor::virtualRealityHeadsetGetPositionCallback(
  webots_ros::supervisor_virtual_reality_headset_get_position::Request &req,
  webots_ros::supervisor_virtual_reality_headset_get_position::Response &res) {
  assert(mSupervisor);
  const double *position = mSupervisor->virtualRealityHeadsetGetPosition();
  res.position.x = position[0];
  res.position.y = position[1];
  res.position.z = position[2];
  return true;
}

bool RosSupervisor::virtualRealityHeadsetIsUsedCallback(webots_ros::get_bool::Request &req,
                                                        webots_ros::get_bool::Response &res) {
  assert(mSupervisor);
  res.value = mSupervisor->virtualRealityHeadsetIsUsed();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetIdCallback(webots_ros::node_get_id::Request &req, webots_ros::node_get_id::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.id = node->getId();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetTypeCallback(webots_ros::node_get_type::Request &req, webots_ros::node_get_type::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.type = node->getType();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetTypeNameCallback(webots_ros::node_get_name::Request &req, webots_ros::node_get_name::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.name = node->getTypeName();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetDefCallback(webots_ros::node_get_name::Request &req, webots_ros::node_get_name::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.name = node->getDef();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetBaseTypeNameCallback(webots_ros::node_get_name::Request &req,
                                                webots_ros::node_get_name::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.name = node->getBaseTypeName();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetParentNodeCallback(webots_ros::node_get_parent_node::Request &req,
                                              webots_ros::node_get_parent_node::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.node = reinterpret_cast<uint64_t>(node->getParentNode());
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeIsProtoCallback(webots_ros::node_is_proto::Request &req, webots_ros::node_is_proto::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.value = node->isProto();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetPositionCallback(webots_ros::node_get_position::Request &req,
                                            webots_ros::node_get_position::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  const double *position;
  position = node->getPosition();
  res.position.x = position[0];
  res.position.y = position[1];
  res.position.z = position[2];
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetOrientationCallback(webots_ros::node_get_orientation::Request &req,
                                               webots_ros::node_get_orientation::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  const double *matrix;
  matrix = node->getOrientation();
  RosMathUtils::matrixToQuaternion(matrix, res.orientation);
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetPoseCallback(webots_ros::node_get_pose::Request &req, webots_ros::node_get_pose::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *fromNode = reinterpret_cast<Node *>(req.from_node);
  Node *toNode = reinterpret_cast<Node *>(req.node);
  const double *m = fromNode->getPose(toNode);
  const double rotation[9] = {m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]};
  RosMathUtils::matrixToQuaternion(rotation, res.pose.rotation);
  res.pose.translation.x = m[3];
  res.pose.translation.y = m[7];
  res.pose.translation.z = m[11];
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetCenterOfMassCallback(webots_ros::node_get_center_of_mass::Request &req,
                                                webots_ros::node_get_center_of_mass::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  const double *centerOfMass;
  centerOfMass = node->getCenterOfMass();
  res.centerOfMass.x = centerOfMass[0];
  res.centerOfMass.y = centerOfMass[1];
  res.centerOfMass.z = centerOfMass[2];
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetNumberOfContactPointsCallback(webots_ros::node_get_number_of_contact_points::Request &req,
                                                         webots_ros::node_get_number_of_contact_points::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.numberOfContactPoints = node->getNumberOfContactPoints(req.includeDescendants);
  return true;
}

bool RosSupervisor::nodeGetContactPointCallback(webots_ros::node_get_contact_point::Request &req,
                                                webots_ros::node_get_contact_point::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  const double *point;
  point = node->getContactPoint(req.index);
  res.point.x = point[0];
  res.point.y = point[1];
  res.point.z = point[2];
  return true;
}

bool RosSupervisor::nodeGetContactPointNodeCallback(webots_ros::node_get_contact_point_node::Request &req,
                                                    webots_ros::node_get_contact_point_node::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.node = reinterpret_cast<uint64_t>(node->getContactPointNode(req.index));
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetStaticBalanceCallback(webots_ros::node_get_static_balance::Request &req,
                                                 webots_ros::node_get_static_balance::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.balance = node->getStaticBalance();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeGetVelocityCallback(webots_ros::node_get_velocity::Request &req,
                                            webots_ros::node_get_velocity::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  const double *velocity;
  velocity = node->getVelocity();
  res.velocity.linear.x = velocity[0];
  res.velocity.linear.y = velocity[1];
  res.velocity.linear.z = velocity[2];
  res.velocity.angular.x = velocity[3];
  res.velocity.angular.y = velocity[4];
  res.velocity.angular.z = velocity[5];
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeSetVelocityCallback(webots_ros::node_set_velocity::Request &req,
                                            webots_ros::node_set_velocity::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  double velocity[6];
  velocity[0] = req.velocity.linear.x;
  velocity[1] = req.velocity.linear.y;
  velocity[2] = req.velocity.linear.z;
  velocity[3] = req.velocity.angular.x;
  velocity[4] = req.velocity.angular.y;
  velocity[5] = req.velocity.angular.z;
  node->setVelocity((const double *)velocity);
  res.success = 1;
  return true;
}

bool RosSupervisor::nodeAddForceCallback(webots_ros::node_add_force_or_torque::Request &req,
                                         webots_ros::node_add_force_or_torque::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  double force[3];
  force[0] = req.force.x;
  force[1] = req.force.y;
  force[2] = req.force.z;
  node->addForce((const double *)force, req.relative);
  res.success = 1;
  return true;
}

bool RosSupervisor::nodeAddForceWithOffsetCallback(webots_ros::node_add_force_with_offset::Request &req,
                                                   webots_ros::node_add_force_with_offset::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  double force[3];
  force[0] = req.force.x;
  force[1] = req.force.y;
  force[2] = req.force.z;
  double offset[3];
  offset[0] = req.offset.x;
  offset[1] = req.offset.y;
  offset[2] = req.offset.z;
  node->addForceWithOffset((const double *)force, (const double *)offset, req.relative);
  res.success = 1;
  return true;
}

bool RosSupervisor::nodeAddTorqueCallback(webots_ros::node_add_force_or_torque::Request &req,
                                          webots_ros::node_add_force_or_torque::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  double torque[3];
  torque[0] = req.force.x;
  torque[1] = req.force.y;
  torque[2] = req.force.z;
  node->addTorque((const double *)torque, req.relative);
  res.success = 1;
  return true;
}

bool RosSupervisor::nodeGetFieldCallback(webots_ros::node_get_field::Request &req, webots_ros::node_get_field::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  if (req.proto)
    res.field = reinterpret_cast<uint64_t>(node->getProtoField(req.fieldName));
  else
    res.field = reinterpret_cast<uint64_t>(node->getField(req.fieldName));
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeMoveViewpointCallback(webots_ros::node_move_viewpoint::Request &req,
                                              webots_ros::node_move_viewpoint::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  node->moveViewpoint();
  res.success = 1;
  return true;
}

bool RosSupervisor::nodeSetVisibilityCallback(webots_ros::node_set_visibility::Request &req,
                                              webots_ros::node_set_visibility::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  Node *from = reinterpret_cast<Node *>(req.from);
  node->setVisibility(from, req.visible);
  res.success = 1;
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeRemoveCallback(webots_ros::node_remove::Request &req, webots_ros::node_remove::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  node->remove();
  res.success = 1;
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeExportStringCallback(webots_ros::node_get_string::Request &req,
                                             webots_ros::node_get_string::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  res.value = node->exportString();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeResetPhysicsCallback(webots_ros::node_reset_functions::Request &req,
                                             webots_ros::node_reset_functions::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  node->resetPhysics();
  res.success = 1;
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeRestartControllerCallback(webots_ros::node_reset_functions::Request &req,
                                                  webots_ros::node_reset_functions::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  node->restartController();
  res.success = 1;
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeSaveStateCallback(webots_ros::node_set_string::Request &req,
                                          webots_ros::node_set_string::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  node->saveState(req.state_name);
  res.success = 1;
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeLoadStateCallback(webots_ros::node_set_string::Request &req,
                                          webots_ros::node_set_string::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  node->loadState(req.state_name);
  res.success = 1;
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeEnablePoseTrackingCallback(webots_ros::node_enable_pose_tracking::Request &req,
                                                   webots_ros::node_enable_pose_tracking::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  Node *fromNode = reinterpret_cast<Node *>(req.from_node);

  node->enablePoseTracking(req.sampling_period, fromNode);

  res.success = 1;
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::nodeDisablePoseTrackingCallback(webots_ros::node_disable_pose_tracking::Request &req,
                                                    webots_ros::node_disable_pose_tracking::Response &res) {
  assert(this);
  if (!req.node)
    return false;
  Node *node = reinterpret_cast<Node *>(req.node);
  Node *fromNode = reinterpret_cast<Node *>(req.from_node);
  res.success = 1;

  node->disablePoseTracking(fromNode);

  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::fieldGetTypeCallback(webots_ros::field_get_type::Request &req, webots_ros::field_get_type::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.type = field->getType();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::fieldGetTypeNameCallback(webots_ros::field_get_type_name::Request &req,
                                             webots_ros::field_get_type_name::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.name = field->getTypeName();
  return true;
}

// cppcheck-suppress constParameter
bool RosSupervisor::fieldGetCountCallback(webots_ros::field_get_count::Request &req,
                                          webots_ros::field_get_count::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.count = field->getCount();
  return true;
}

bool RosSupervisor::fieldGetBoolCallback(webots_ros::field_get_bool::Request &req, webots_ros::field_get_bool::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    res.value = field->getSFBool();
  else
    res.value = field->getMFBool(req.index);
  return true;
}

bool RosSupervisor::fieldGetInt32Callback(webots_ros::field_get_int32::Request &req,
                                          webots_ros::field_get_int32::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    res.value = field->getSFInt32();
  else
    res.value = field->getMFInt32(req.index);
  return true;
}

bool RosSupervisor::fieldGetFloatCallback(webots_ros::field_get_float::Request &req,
                                          webots_ros::field_get_float::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    res.value = field->getSFFloat();
  else
    res.value = field->getMFFloat(req.index);
  return true;
}

bool RosSupervisor::fieldGetVec2fCallback(webots_ros::field_get_vec2f::Request &req,
                                          webots_ros::field_get_vec2f::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  const double *value;
  if (field->getType() <= 0x09)
    value = field->getSFVec2f();
  else
    value = field->getMFVec2f(req.index);
  res.value.x = value[0];
  res.value.y = value[1];
  return true;
}

bool RosSupervisor::fieldGetVec3fCallback(webots_ros::field_get_vec3f::Request &req,
                                          webots_ros::field_get_vec3f::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  const double *value;
  if (field->getType() <= 0x09)
    value = field->getSFVec3f();
  else
    value = field->getMFVec3f(req.index);
  res.value.x = value[0];
  res.value.y = value[1];
  res.value.z = value[2];
  return true;
}

bool RosSupervisor::fieldGetRotationCallback(webots_ros::field_get_rotation::Request &req,
                                             webots_ros::field_get_rotation::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  const double *axisAngleValues;
  if (field->getType() <= 0x09)
    axisAngleValues = field->getSFRotation();
  else
    axisAngleValues = field->getMFRotation(req.index);
  RosMathUtils::axisAngleToQuaternion(axisAngleValues, res.value);
  return true;
}

bool RosSupervisor::fieldGetColorCallback(webots_ros::field_get_color::Request &req,
                                          webots_ros::field_get_color::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  const double *value;
  if (field->getType() <= 0x09)
    value = field->getSFColor();
  else
    value = field->getMFColor(req.index);
  res.value.r = value[0];
  res.value.g = value[1];
  res.value.b = value[2];
  res.value.a = 1.0;
  return true;
}

bool RosSupervisor::fieldGetStringCallback(webots_ros::field_get_string::Request &req,
                                           webots_ros::field_get_string::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    res.value = field->getSFString();
  else
    res.value = field->getMFString(req.index);
  return true;
}

bool RosSupervisor::fieldGetNodeCallback(webots_ros::field_get_node::Request &req, webots_ros::field_get_node::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    res.node = reinterpret_cast<uint64_t>(field->getSFNode());
  else
    res.node = reinterpret_cast<uint64_t>(field->getMFNode(req.index));
  return true;
}

bool RosSupervisor::fieldSetBoolCallback(webots_ros::field_set_bool::Request &req, webots_ros::field_set_bool::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    field->setSFBool(req.value);
  else
    field->setMFBool(req.index, req.value);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldSetInt32Callback(webots_ros::field_set_int32::Request &req,
                                          webots_ros::field_set_int32::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    field->setSFInt32(req.value);
  else
    field->setMFInt32(req.index, req.value);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldSetFloatCallback(webots_ros::field_set_float::Request &req,
                                          webots_ros::field_set_float::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    field->setSFFloat(req.value);
  else
    field->setMFFloat(req.index, req.value);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldSetVec2fCallback(webots_ros::field_set_vec2f::Request &req,
                                          webots_ros::field_set_vec2f::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  double values[2];
  values[0] = req.value.x;
  values[1] = req.value.y;
  if (field->getType() <= 0x09)
    field->setSFVec2f(values);
  else
    field->setMFVec2f(req.index, values);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldSetVec3fCallback(webots_ros::field_set_vec3f::Request &req,
                                          webots_ros::field_set_vec3f::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  double values[3];
  values[0] = req.value.x;
  values[1] = req.value.y;
  values[2] = req.value.z;
  if (field->getType() <= 0x09)
    field->setSFVec3f(values);
  else
    field->setMFVec3f(req.index, values);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldSetRotationCallback(webots_ros::field_set_rotation::Request &req,
                                             webots_ros::field_set_rotation::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  double axisAngleValues[4];
  RosMathUtils::quaternionToAxisAngle(req.value, axisAngleValues);
  if (field->getType() <= 0x09)
    field->setSFRotation(axisAngleValues);
  else
    field->setMFRotation(req.index, axisAngleValues);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldSetColorCallback(webots_ros::field_set_color::Request &req,
                                          webots_ros::field_set_color::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  double values[3];
  values[0] = req.value.r;
  values[1] = req.value.g;
  values[2] = req.value.b;
  if (field->getType() <= 0x09)
    field->setSFColor(values);
  else
    field->setMFColor(req.index, values);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldSetStringCallback(webots_ros::field_set_string::Request &req,
                                           webots_ros::field_set_string::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  if (field->getType() <= 0x09)
    field->setSFString(req.value);
  else
    field->setMFString(req.index, req.value);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldInsertBoolCallback(webots_ros::field_set_bool::Request &req,
                                            webots_ros::field_set_bool::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  if (field->getType() <= 0x09)
    res.success = 0;
  else
    field->insertMFBool(req.index, req.value);
  return true;
}

bool RosSupervisor::fieldInsertInt32Callback(webots_ros::field_set_int32::Request &req,
                                             webots_ros::field_set_int32::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  if (field->getType() <= 0x09)
    res.success = 0;
  else
    field->insertMFInt32(req.index, req.value);
  return true;
}

bool RosSupervisor::fieldInsertFloatCallback(webots_ros::field_set_float::Request &req,
                                             webots_ros::field_set_float::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  if (field->getType() <= 0x09)
    res.success = 0;
  else
    field->insertMFFloat(req.index, req.value);
  return true;
}

bool RosSupervisor::fieldInsertVec2fCallback(webots_ros::field_set_vec2f::Request &req,
                                             webots_ros::field_set_vec2f::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  double values[2];
  values[0] = req.value.x;
  values[1] = req.value.y;
  if (field->getType() <= 0x09)
    res.success = 0;
  else
    field->insertMFVec2f(req.index, values);
  return true;
}

bool RosSupervisor::fieldInsertVec3fCallback(webots_ros::field_set_vec3f::Request &req,
                                             webots_ros::field_set_vec3f::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  double values[3];
  values[0] = req.value.x;
  values[1] = req.value.y;
  values[2] = req.value.z;
  if (field->getType() <= 0x09)
    res.success = 0;
  else
    field->insertMFVec3f(req.index, values);
  return true;
}

bool RosSupervisor::fieldInsertRotationCallback(webots_ros::field_set_rotation::Request &req,
                                                webots_ros::field_set_rotation::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  double axisAngleValues[4];
  RosMathUtils::quaternionToAxisAngle(req.value, axisAngleValues);
  if (field->getType() <= 0x09)
    res.success = 0;
  else
    field->insertMFRotation(req.index, axisAngleValues);
  return true;
}

bool RosSupervisor::fieldInsertColorCallback(webots_ros::field_set_color::Request &req,
                                             webots_ros::field_set_color::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  double values[3];
  values[0] = req.value.r;
  values[1] = req.value.g;
  values[2] = req.value.b;
  if (field->getType() <= 0x09)
    res.success = 0;
  else
    field->insertMFColor(req.index, values);
  return true;
}

bool RosSupervisor::fieldInsertStringCallback(webots_ros::field_set_string::Request &req,
                                              webots_ros::field_set_string::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  if (field->getType() <= 0x09)
    res.success = 0;
  else
    field->insertMFString(req.index, req.value);
  return true;
}

bool RosSupervisor::fieldRemoveCallback(webots_ros::field_remove::Request &req, webots_ros::field_remove::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  if (field->getType() == Field::SF_NODE)
    field->removeSF();
  else if (field->getType() & Field::MF)
    field->removeMF(req.index);
  else
    res.success = 0;
  return true;
}

bool RosSupervisor::fieldImportNodeCallback(webots_ros::field_import_node::Request &req,
                                            webots_ros::field_import_node::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  if (field->getType() == Field::SF_NODE)
    field->importSFNode(req.filename);
  else if (field->getType() == Field::MF_NODE)
    field->importMFNode(req.position, req.filename);
  else
    res.success = 0;
  return true;
}

bool RosSupervisor::fieldImportNodeFromStringCallback(webots_ros::field_import_node_from_string::Request &req,
                                                      webots_ros::field_import_node_from_string::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  if (field->getType() == Field::SF_NODE)
    field->importSFNodeFromString(req.nodeString);
  else if (field->getType() == Field::MF_NODE)
    field->importMFNodeFromString(req.position, req.nodeString);
  else
    res.success = 0;

  return true;
}

bool RosSupervisor::fieldRemoveNodeCallback(webots_ros::field_remove_node::Request &req,
                                            webots_ros::field_remove_node::Response &res) {
  ROS_WARN("Deprecated '/supervisor/field/remove_node' service, use '/supervisor/field/remove' service instead.");
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  field->removeMF(req.position);
  res.success = 1;
  return true;
}

bool RosSupervisor::fieldEnableSFTrackingCallback(webots_ros::field_enable_sf_tracking::Request &req,
                                                  webots_ros::field_enable_sf_tracking::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  field->enableSFTracking(req.sampling_period);
  res.success = 1;

  return true;
}

bool RosSupervisor::fieldDisableSFTrackingCallback(webots_ros::field_disable_sf_tracking::Request &req,
                                                   webots_ros::field_disable_sf_tracking::Response &res) {
  assert(this);
  if (!req.field)
    return false;
  Field *field = reinterpret_cast<Field *>(req.field);
  res.success = 1;
  field->disableSFTracking();

  return true;
}
