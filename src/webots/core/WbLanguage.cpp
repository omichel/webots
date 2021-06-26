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

#include "WbLanguage.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <cassert>

static const char *C_PREPROCESSOR_KEYWORDS = "#assert #define #elif #else #endif #error #ident #if #ifdef #ifndef "
                                             "#import #include #include_next #line #pragma #sccs #unassert #undef "
                                             "#warning";

static const char *C_KEYWORDS = "asm auto bool break case char const continue default do double else enum extern "
                                "false float for goto if inline int long register restrict return short signed "
                                "sizeof static struct switch true typedef typeof union unsigned void volatile "
                                "while";

// C++98 keywords coming in addition to C keywords
static const char *CPP_KEYWORDS = "and and_eq bitand bitor catch class compl const_cast delete "
                                  "dynamic_cast explicit export friend mutable namespace new not not_eq "
                                  "operator or or_eq private protected public reinterpret_cast static_cast "
                                  "template this throw try typeid typename using virtual wchar_t xor "
                                  "xor_eq";

static const char *JAVA_KEYWORDS = "abstract assert boolean break byte case catch char class continue default "
                                   "do double else enum extends final finally float for if implements import "
                                   "instanceof int interface long native new package private protected public "
                                   "return short static strictfp super switch synchronized this throw throws "
                                   "transient try void volatile while";

static const char *MATLAB_KEYWORDS = "break case catch continue else elseif end for function global if otherwise "
                                     "persistent return switch try while";

static const char *PYTHON_KEYWORDS = "and as assert break class continue def "
                                     "del elif else except exec finally for "
                                     "from global if import in is lambda "
                                     "not or pass print raise return try "
                                     "while with yield";

static const char *MAKEFILE_KEYWORDS = "if ifdef ifndef ifeq ifneq else endif include";

static const char *WBT_KEYWORDS = "USE DEF PROTO IS FALSE TRUE NULL "
                                  "field deprecatedField hiddenField unconnectedField vrmlField "
                                  "SFVec3f SFBool SFInt32 SFFloat SFVec2f SFVec3f SFRotation SFColor SFString SFNode "
                                  "MFBool MFInt32 MFFloat MFVec2f MFVec3f MFRotation MFColor MFString MFNode";

static const char *LUA_KEYWORDS = "and break do else elseif end false fields for function if in local nil "
                                  "not or repeat return then true until while";

static const char *C_API_FUNCTIONS = "wb_accelerometer_enable "
                                     "wb_accelerometer_disable "
                                     "wb_accelerometer_get_sampling_period "
                                     "wb_accelerometer_get_values "
                                     "wb_accelerometer_get_lookup_table_size "
                                     "wb_accelerometer_get_lookup_table "
                                     "wb_brake_get_type "
                                     "wb_brake_get_motor "
                                     "wb_brake_get_position_sensor "
                                     "wb_brake_set_damping_constant "
                                     "wb_camera_enable "
                                     "wb_camera_disable "
                                     "wb_camera_get_exposure "
                                     "wb_camera_get_sampling_period "
                                     "wb_camera_get_image "
                                     "wb_camera_get_width "
                                     "wb_camera_get_height "
                                     "wb_camera_get_fov "
                                     "wb_camera_get_max_fov "
                                     "wb_camera_get_min_fov "
                                     "wb_camera_set_fov "
                                     "wb_camera_get_focal_length "
                                     "wb_camera_get_focal_distance "
                                     "wb_camera_get_min_focal_distance "
                                     "wb_camera_get_max_focal_distance "
                                     "wb_camera_set_exposure "
                                     "wb_camera_set_focal_distance "
                                     "wb_camera_get_near "
                                     "wb_camera_save_image "
                                     "wb_camera_image_get_red "
                                     "wb_camera_image_get_green "
                                     "wb_camera_image_get_blue "
                                     "wb_camera_image_get_gray "
                                     "wb_compass_enable "
                                     "wb_compass_disable "
                                     "wb_camera_has_recognition "
                                     "wb_camera_recognition_disable "
                                     "wb_camera_recognition_disable_segmentation "
                                     "wb_camera_recognition_enable "
                                     "wb_camera_recognition_enable_segmentation "
                                     "wb_camera_recognition_get_number_of_objects "
                                     "wb_camera_recognition_get_objects "
                                     "wb_camera_recognition_get_sampling_period "
                                     "wb_camera_recognition_get_segmentation_image "
                                     "wb_camera_recognition_has_segmentation "
                                     "wb_camera_recognition_is_segmentation_enabled "
                                     "wb_camera_recognition_save_segmentation_image "
                                     "wb_compass_get_sampling_period "
                                     "wb_compass_get_values "
                                     "wb_compass_get_lookup_table_size "
                                     "wb_compass_get_lookup_table "
                                     "wb_connector_lock "
                                     "wb_connector_unlock "
                                     "wb_connector_disable_presence "
                                     "wb_connector_enable_presence "
                                     "wb_connector_get_presence "
                                     "wb_connector_get_presence_sampling_period "
                                     "wb_connector_is_locked "
                                     "wb_device_get_model "
                                     "wb_device_get_name "
                                     "wb_device_get_node_type "
                                     "wb_display_get_width "
                                     "wb_display_get_height "
                                     "wb_display_set_alpha "
                                     "wb_display_set_color "
                                     "wb_display_set_font "
                                     "wb_display_set_opacity "
                                     "wb_display_attach_camera "
                                     "wb_display_detach_camera "
                                     "wb_display_draw_pixel "
                                     "wb_display_draw_line "
                                     "wb_display_draw_rectangle "
                                     "wb_display_draw_oval "
                                     "wb_display_draw_polygon "
                                     "wb_display_draw_text "
                                     "wb_display_fill_rectangle "
                                     "wb_display_fill_oval "
                                     "wb_display_fill_polygon "
                                     "wb_display_image_copy "
                                     "wb_display_image_paste "
                                     "wb_display_image_load "
                                     "wb_display_image_new "
                                     "wb_display_image_save "
                                     "wb_display_image_delete "
                                     "wb_distance_sensor_disable "
                                     "wb_distance_sensor_enable "
                                     "wb_distance_sensor_get_aperture "
                                     "wb_distance_sensor_get_max_value "
                                     "wb_distance_sensor_get_min_value "
                                     "wb_distance_sensor_get_sampling_period "
                                     "wb_distance_sensor_get_lookup_table_size "
                                     "wb_distance_sensor_get_lookup_table "
                                     "wb_distance_sensor_get_type "
                                     "wb_distance_sensor_get_value "
                                     "wb_emitter_send "
                                     "wb_emitter_get_buffer_size "
                                     "wb_emitter_set_channel "
                                     "wb_emitter_get_channel "
                                     "wb_emitter_get_range "
                                     "wb_emitter_set_range "
                                     "wb_gps_enable "
                                     "wb_gps_convert_to_degrees_minutes_seconds "
                                     "wb_gps_disable "
                                     "wb_gps_get_coordinate_system "
                                     "wb_gps_get_sampling_period "
                                     "wb_gps_get_speed "
                                     "wb_gps_get_values "
                                     "wb_gyro_enable "
                                     "wb_gyro_disable "
                                     "wb_gyro_get_sampling_period "
                                     "wb_gyro_get_values "
                                     "wb_gyro_get_lookup_table_size "
                                     "wb_gyro_get_lookup_table "
                                     "wb_inertial_unit_enable "
                                     "wb_inertial_unit_disable "
                                     "wb_inertial_unit_get_sampling_period "
                                     "wb_inertial_unit_get_roll_pitch_yaw "
                                     "wb_inertial_unit_get_quaternion "
                                     "wb_inertial_unit_get_noise "
                                     "wb_joystick_disable "
                                     "wb_joystick_enable "
                                     "wb_joystick_get_axis_value "
                                     "wb_joystick_get_model "
                                     "wb_joystick_get_number_of_axes "
                                     "wb_joystick_get_number_of_povs "
                                     "wb_joystick_get_pov_value "
                                     "wb_joystick_get_pressed_button "
                                     "wb_joystick_get_sampling_period "
                                     "wb_joystick_is_connected "
                                     "wb_joystick_set_auto_centering_gain "
                                     "wb_joystick_set_constant_force "
                                     "wb_joystick_set_constant_force_duration "
                                     "wb_joystick_set_force_axis "
                                     "wb_joystick_set_resistance_gain "
                                     "wb_keyboard_enable "
                                     "wb_keyboard_disable "
                                     "wb_keyboard_get_key "
                                     "wb_keyboard_get_sampling_period "
                                     "wb_led_set "
                                     "wb_led_get "
                                     "wb_lidar_disable "
                                     "wb_lidar_enable "
                                     "wb_lidar_enable_point_cloud "
                                     "wb_lidar_disable_point_cloud "
                                     "wb_lidar_get_fov "
                                     "wb_lidar_get_frequency "
                                     "wb_lidar_get_horizontal_resolution "
                                     "wb_lidar_get_layer_point_cloud "
                                     "wb_lidar_get_layer_range_image "
                                     "wb_lidar_get_max_frequency "
                                     "wb_lidar_get_max_range "
                                     "wb_lidar_get_min_frequency "
                                     "wb_lidar_get_min_range "
                                     "wb_lidar_get_number_of_layers "
                                     "wb_lidar_get_point_cloud "
                                     "wb_lidar_get_number_of_points "
                                     "wb_lidar_get_range_image "
                                     "wb_lidar_get_sampling_period "
                                     "wb_lidar_get_vertical_fov "
                                     "wb_lidar_is_point_cloud_enabled "
                                     "wb_lidar_set_frequency "
                                     "wb_light_sensor_enable "
                                     "wb_light_sensor_disable "
                                     "wb_light_sensor_get_sampling_period "
                                     "wb_light_sensor_get_value "
                                     "wb_light_sensor_get_lookup_table_size "
                                     "wb_light_sensor_get_lookup_table "
                                     "wb_motor_disable_force_feedback "
                                     "wb_motor_disable_torque_feedback "
                                     "wb_motor_enable_force_feedback "
                                     "wb_motor_enable_torque_feedback "
                                     "wb_motor_get_force_feedback "
                                     "wb_motor_get_force_feedback_sampling_period "
                                     "wb_motor_get_acceleration "
                                     "wb_motor_get_available_force "
                                     "wb_motor_get_available_torque "
                                     "wb_motor_get_max_force "
                                     "wb_motor_get_max_torque "
                                     "wb_motor_get_max_position "
                                     "wb_motor_get_max_velocity "
                                     "wb_motor_get_min_position "
                                     "wb_motor_get_target_position "
                                     "wb_motor_get_torque_feedback "
                                     "wb_motor_get_torque_feed_back_sampling_period "
                                     "wb_motor_get_type "
                                     "wb_motor_get_velocity "
                                     "wb_motor_get_multiplier "
                                     "wb_motor_get_brake "
                                     "wb_motor_get_position_sensor "
                                     "wb_motor_set_acceleration "
                                     "wb_motor_set_available_force "
                                     "wb_motor_set_available_torque "
                                     "wb_motor_set_control_pid "
                                     "wb_motor_set_force "
                                     "wb_motor_set_position "
                                     "wb_motor_set_torque "
                                     "wb_motor_set_velocity "
                                     "wb_mouse_enable "
                                     "wb_mouse_enable_3d_position "
                                     "wb_mouse_disable "
                                     "wb_mouse_disable_3d_position "
                                     "wb_mouse_get_sampling_period "
                                     "wb_mouse_get_state "
                                     "wb_mouse_is_3d_position_enabled "
                                     "wb_pen_write "
                                     "wb_pen_set_ink_color "
                                     "wb_position_sensor_enable "
                                     "wb_position_sensor_disable "
                                     "wb_position_sensor_sampling_period "
                                     "wb_position_sensor_get_value "
                                     "wb_position_sensor_get_type "
                                     "wb_position_sensor_get_brake "
                                     "wb_position_sensor_get_motor "
                                     "wb_radar_disable "
                                     "wb_radar_enable "
                                     "wb_radar_get_horizontal_fov "
                                     "wb_radar_get_max_range "
                                     "wb_radar_get_min_range "
                                     "wb_radar_get_number_of_targets "
                                     "wb_radar_get_sampling_period "
                                     "wb_radar_get_targets "
                                     "wb_radar_get_vertical_fov "
                                     "wb_range_finder_enable "
                                     "wb_range_finder_disable "
                                     "wb_range_finder_get_sampling_period "
                                     "wb_range_finder_get_range_image "
                                     "wb_range_finder_get_width "
                                     "wb_range_finder_get_height "
                                     "wb_range_finder_get_fov "
                                     "wb_range_finder_get_min_range "
                                     "wb_range_finder_get_max_range "
                                     "wb_range_finder_image_get_depth "
                                     "wb_range_finder_save_image "
                                     "wb_receiver_enable "
                                     "wb_receiver_disable "
                                     "wb_receiver_get_sampling_period "
                                     "wb_receiver_set_channel "
                                     "wb_receiver_get_channel "
                                     "wb_receiver_get_queue_length "
                                     "wb_receiver_next_packet "
                                     "wb_receiver_get_data_size "
                                     "wb_receiver_get_data "
                                     "wb_receiver_get_signal_strength "
                                     "wb_receiver_get_emitter_direction "
                                     "wb_robot_init "
                                     "wb_robot_cleanup "
                                     "wb_robot_battery_sensor_enable "
                                     "wb_robot_battery_sensor_disable "
                                     "wb_robot_battery_sensor_get_sampling_period "
                                     "wb_robot_battery_sensor_get_value "
                                     "wb_robot_get_custom_data "
                                     "wb_robot_get_device "
                                     "wb_robot_get_device_by_index "
                                     "wb_robot_get_number_of_devices "
                                     "wb_robot_get_type "
                                     "wb_robot_get_model "
                                     "wb_robot_get_mode "
                                     "wb_robot_get_name "
                                     "wb_robot_get_time "
                                     "wb_robot_get_supervisor "
                                     "wb_robot_get_synchronization "
                                     "wb_robot_get_project_path "
                                     "wb_robot_get_basic_time_step "
                                     "wb_robot_get_urdf "
                                     "wb_robot_get_world_path "
                                     "wb_robot_mutex_new "
                                     "wb_robot_mutex_lock "
                                     "wb_robot_mutex_unlock "
                                     "wb_robot_mutex_delete "
                                     "wb_robot_set_custom_data "
                                     "wb_robot_set_mode "
                                     "wb_robot_step "
                                     "wb_robot_task_new "
                                     "wb_robot_wait_for_user_input_event "
                                     "wb_robot_window_custom_function "
                                     "wb_skin_get_bone_count "
                                     "wb_skin_get_bone_name "
                                     "wb_skin_get_bone_orientation "
                                     "wb_skin_get_bone_position "
                                     "wb_skin_set_bone_orientation "
                                     "wb_skin_set_bone_position "
                                     "wb_speaker_get_engine "
                                     "wb_speaker_get_language "
                                     "wb_speaker_is_sound_playing "
                                     "wb_speaker_is_speaking "
                                     "wb_speaker_play_sound "
                                     "wb_speaker_set_engine "
                                     "wb_speaker_set_language "
                                     "wb_speaker_speak "
                                     "wb_speaker_stop "
                                     "wb_supervisor_animation_start_recording "
                                     "wb_supervisor_animation_stop_recording "
                                     "wb_supervisor_export_image "
                                     "wb_supervisor_field_disable_sf_tracking "
                                     "wb_supervisor_field_enable_sf_tracking "
                                     "wb_supervisor_field_get_type "
                                     "wb_supervisor_field_get_type_name "
                                     "wb_supervisor_field_get_count "
                                     "wb_supervisor_field_get_sf_bool "
                                     "wb_supervisor_field_get_sf_int32 "
                                     "wb_supervisor_field_get_sf_float "
                                     "wb_supervisor_field_get_sf_vec2f "
                                     "wb_supervisor_field_get_sf_vec3f "
                                     "wb_supervisor_field_get_sf_rotation "
                                     "wb_supervisor_field_get_sf_color "
                                     "wb_supervisor_field_get_sf_string "
                                     "wb_supervisor_field_get_sf_node "
                                     "wb_supervisor_field_get_mf_bool "
                                     "wb_supervisor_field_get_mf_int32 "
                                     "wb_supervisor_field_get_mf_float "
                                     "wb_supervisor_field_get_mf_vec2f "
                                     "wb_supervisor_field_get_mf_vec3f "
                                     "wb_supervisor_field_get_mf_color "
                                     "wb_supervisor_field_get_mf_rotation "
                                     "wb_supervisor_field_get_mf_string "
                                     "wb_supervisor_field_get_mf_node "
                                     "wb_supervisor_field_import_mf_node "
                                     "wb_supervisor_field_import_mf_node_from_string "
                                     "wb_supervisor_field_import_sf_node "
                                     "wb_supervisor_field_import_sf_node_from_string "
                                     "wb_supervisor_field_insert_mf_bool "
                                     "wb_supervisor_field_insert_mf_color "
                                     "wb_supervisor_field_insert_mf_float "
                                     "wb_supervisor_field_insert_mf_int32 "
                                     "wb_supervisor_field_insert_mf_rotation "
                                     "wb_supervisor_field_insert_mf_string "
                                     "wb_supervisor_field_insert_mf_vec2f "
                                     "wb_supervisor_field_insert_mf_vec3f "
                                     "wb_supervisor_field_remove_mf "
                                     "wb_supervisor_field_remove_sf "
                                     "wb_supervisor_field_set_sf_bool "
                                     "wb_supervisor_field_set_sf_int32 "
                                     "wb_supervisor_field_set_sf_float "
                                     "wb_supervisor_field_set_sf_vec2f "
                                     "wb_supervisor_field_set_sf_vec3f "
                                     "wb_supervisor_field_set_sf_rotation "
                                     "wb_supervisor_field_set_sf_color "
                                     "wb_supervisor_field_set_sf_string "
                                     "wb_supervisor_field_set_mf_bool "
                                     "wb_supervisor_field_set_mf_int32 "
                                     "wb_supervisor_field_set_mf_float "
                                     "wb_supervisor_field_set_mf_vec2f "
                                     "wb_supervisor_field_set_mf_vec3f "
                                     "wb_supervisor_field_set_mf_color "
                                     "wb_supervisor_field_set_mf_rotation "
                                     "wb_supervisor_field_set_mf_string "
                                     "wb_supervisor_movie_failed "
                                     "wb_supervisor_movie_is_ready "
                                     "wb_supervisor_movie_start_recording "
                                     "wb_supervisor_movie_stop_recording "
                                     "wb_supervisor_node_add_force "
                                     "wb_supervisor_node_add_force_with_offset "
                                     "wb_supervisor_node_add_torque "
                                     "wb_supervisor_node_disable_pose_tracking "
                                     "wb_supervisor_node_enable_pose_tracking "
                                     "wb_supervisor_node_export_string "
                                     "wb_supervisor_node_get_base_type_name "
                                     "wb_supervisor_node_get_center_of_mass "
                                     "wb_supervisor_node_get_contact_point "
                                     "wb_supervisor_node_get_def "
                                     "wb_supervisor_node_get_field "
                                     "wb_supervisor_node_get_from_def "
                                     "wb_supervisor_node_get_from_id "
                                     "wb_supervisor_node_get_from_proto_def "
                                     "wb_supervisor_node_get_from_device "
                                     "wb_supervisor_node_get_id "
                                     "wb_supervisor_node_get_number_of_contact_points "
                                     "wb_supervisor_node_get_orientation "
                                     "wb_supervisor_node_get_parent_node "
                                     "wb_supervisor_node_get_pose "
                                     "wb_supervisor_node_get_position "
                                     "wb_supervisor_node_get_proto_field "
                                     "wb_supervisor_node_get_root "
                                     "wb_supervisor_node_get_selected "
                                     "wb_supervisor_node_get_self "
                                     "wb_supervisor_node_get_static_balance "
                                     "wb_supervisor_node_get_type "
                                     "wb_supervisor_node_get_type_name "
                                     "wb_supervisor_node_get_velocity "
                                     "wb_supervisor_node_is_proto "
                                     "wb_supervisor_node_load_state "
                                     "wb_supervisor_node_move_viewpoint "
                                     "wb_supervisor_node_remove "
                                     "wb_supervisor_node_reset_physics "
                                     "wb_supervisor_node_restart_controller "
                                     "wb_supervisor_node_save_state "
                                     "wb_supervisor_node_set_velocity "
                                     "wb_supervisor_node_set_visibility "
                                     "wb_supervisor_set_label "
                                     "wb_supervisor_simulation_get_mode "
                                     "wb_supervisor_simulation_quit "
                                     "wb_supervisor_simulation_reset "
                                     "wb_supervisor_simulation_reset_physics "
                                     "wb_supervisor_simulation_set_mode "
                                     "wb_supervisor_virtual_reality_headset_get_position "
                                     "wb_supervisor_virtual_reality_headset_get_orientation "
                                     "wb_supervisor_virtual_reality_headset_is_used "
                                     "wb_supervisor_world_load "
                                     "wb_supervisor_world_reload "
                                     "wb_supervisor_world_save "
                                     "wb_touch_sensor_enable "
                                     "wb_touch_sensor_disable "
                                     "wb_touch_sensor_get_sampling_period "
                                     "wb_touch_sensor_get_type "
                                     "wb_touch_sensor_get_value "
                                     "wb_touch_sensor_get_lookup_table_size "
                                     "wb_touch_sensor_get_lookup_table "
                                     "wbu_car_cleanup "
                                     "wbu_car_enable_indicator_auto_disabling "
                                     "wbu_car_init "
                                     "wbu_car_get_backwards_lights "
                                     "wbu_car_get_brake_lights "
                                     "wbu_car_get_engine_type "
                                     "wbu_car_get_front_wheel_radius "
                                     "wbu_car_get_indicator_period "
                                     "wbu_car_get_left_steering_angle "
                                     "wbu_car_enable_limited_slip_differential "
                                     "wbu_car_get_rear_wheel_radius "
                                     "wbu_car_get_track_front "
                                     "wbu_car_get_track_rear "
                                     "wbu_car_get_type "
                                     "wbu_car_get_right_steering_angle "
                                     "wbu_car_get_wheelbase "
                                     "wbu_car_get_wheel_encoder "
                                     "wbu_car_get_wheel_speed "
                                     "wbu_car_set_indicator_period "
                                     "wbu_driver_cleanup "
                                     "wbu_driver_init "
                                     "wbu_driver_get_brake_intensity "
                                     "wbu_driver_get_current_speed "
                                     "wbu_driver_get_gear "
                                     "wbu_driver_get_hazard_flashers "
                                     "wbu_driver_get_indicator "
                                     "wbu_driver_get_rpm "
                                     "wbu_driver_get_steering_angle "
                                     "wbu_driver_get_target_cruising_speed "
                                     "wbu_driver_get_throttle "
                                     "wbu_driver_set_antifog_lights "
                                     "wbu_driver_set_brake_intensity "
                                     "wbu_driver_get_control_mode "
                                     "wbu_driver_get_wiper_mode "
                                     "wbu_driver_set_cruising_speed "
                                     "wbu_driver_set_dipped_beams "
                                     "wbu_driver_set_gear "
                                     "wbu_driver_get_gear_number "
                                     "wbu_driver_set_hazard_flashers "
                                     "wbu_driver_set_indicator "
                                     "wbu_driver_set_steering_angle "
                                     "wbu_driver_set_throttle "
                                     "wbu_driver_set_wiper_mode "
                                     "wbu_driver_step "
                                     "wbu_motion_new "
                                     "wbu_motion_play "
                                     "wbu_motion_stop "
                                     "wbu_motion_get_duration "
                                     "wbu_motion_get_time "
                                     "wbu_motion_set_time "
                                     "wbu_motion_set_reverse "
                                     "wbu_motion_set_loop "
                                     "wbu_motion_is_over "
                                     "wbu_motion_delete";

static const char *C_API_DATA_TYPES = "WbCameraRecognitionObject "
                                      "WbDeviceTag "
                                      "WbDistanceSensorType "
                                      "WbFieldRef "
                                      "WbFieldType "
                                      "WbGpsCoordinateSystem "
                                      "WbImageRef "
                                      "WbLidarPoint "
                                      "WbMotionRef "
                                      "WbMouseState "
                                      "WbNodeRef "
                                      "WbNodeType "
                                      "WbRadarTarget "
                                      "WbRobotMode "
                                      "WbSimulationMode "
                                      "WbTouchSensorType "
                                      "WbUserInputEvent "
                                      "WbuCarEngineType "
                                      "WbuCarType "
                                      "WbuCarWheelIndex "
                                      "WbuDriverControlMode "
                                      "WbuDriverIndicatorState "
                                      "WbuDriverWiperMode";

static const char *C_API_CONSTANTS = "INFINITY "
                                     "WB_ROTATIONAL "
                                     "WB_LINEAR "
                                     "WB_CHANNEL_BROADCAST "
                                     "WB_DISTANCE_SENSOR_GENERIC "
                                     "WB_DISTANCE_SENSOR_INFRA_RED "
                                     "WB_DISTANCE_SENSOR_SONAR "
                                     "WB_DISTANCE_SENSOR_LASER "
                                     "WB_IMAGE_RGB "
                                     "WB_IMAGE_RGBA "
                                     "WB_IMAGE_ARGB "
                                     "WB_IMAGE_BGRA "
                                     "WB_IMAGE_ABGR "
                                     "WB_KEYBOARD_KEY "
                                     "WB_KEYBOARD_SHIFT "
                                     "WB_KEYBOARD_CONTROL "
                                     "WB_KEYBOARD_ALT "
                                     "WB_KEYBOARD_LEFT "
                                     "WB_KEYBOARD_UP "
                                     "WB_KEYBOARD_RIGHT "
                                     "WB_KEYBOARD_DOWN "
                                     "WB_KEYBOARD_PAGEUP "
                                     "WB_KEYBOARD_PAGEDOWN "
                                     "WB_KEYBOARD_HOME "
                                     "WB_KEYBOARD_END "
                                     "WB_MODE_SIMULATION "
                                     "WB_MODE_CROSS_COMPILATION "
                                     "WB_MODE_REMOTE_CONTROL "
                                     "WB_TOUCH_SENSOR_BUMPER "
                                     "WB_TOUCH_SENSOR_FORCE "
                                     "WB_TOUCH_SENSOR_FORCE3D "
                                     "WB_SF_BOOL "
                                     "WB_SF_INT32 "
                                     "WB_SF_FLOAT "
                                     "WB_SF_VEC2F "
                                     "WB_SF_VEC3F "
                                     "WB_SF_ROTATION "
                                     "WB_SF_COLOR "
                                     "WB_SF_STRING "
                                     "WB_SF_NODE "
                                     "WB_MF "
                                     "WB_MF_INT32 "
                                     "WB_MF_FLOAT "
                                     "WB_MF_VEC2F "
                                     "WB_MF_VEC3F "
                                     "WB_MF_COLOR "
                                     "WB_MF_STRING "
                                     "WB_MF_NODE "
                                     "WB_NODE_NO_NODE "
                                     "WB_NODE_APPEARANCE "
                                     "WB_NODE_ACCELEROMETER "
                                     "WB_NODE_BACKGROUND "
                                     "WB_NODE_BALL_JOINT "
                                     "WB_NODE_BALL_JOINT_PARAMETERS "
                                     "WB_NODE_BILLBOARD"
                                     "WB_NODE_BOX "
                                     "WB_NODE_BRAKE "
                                     "WB_NODE_CAMERA "
                                     "WB_NODE_CAPSULE "
                                     "WB_NODE_CHARGER "
                                     "WB_NODE_COLOR "
                                     "WB_NODE_COMPASS "
                                     "WB_NODE_CONE "
                                     "WB_NODE_CONNECTOR "
                                     "WB_NODE_CONTACT_PROPERTIES "
                                     "WB_NODE_COORDINATE "
                                     "WB_NODE_CYLINDER "
                                     "WB_NODE_DAMPING "
                                     "WB_NODE_FLUID "
                                     "WB_NODE_FOCUS "
                                     "WB_NODE_DIRECTIONAL_LIGHT "
                                     "WB_NODE_DISPLAY "
                                     "WB_NODE_DISTANCE_SENSOR "
                                     "WB_NODE_ELEVATION_GRID "
                                     "WB_NODE_EMITTER "
                                     "WB_NODE_FOG "
                                     "WB_NODE_GPS "
                                     "WB_NODE_GROUP "
                                     "WB_NODE_GYRO "
                                     "WB_NODE_HINGE_JOINT "
                                     "WB_NODE_HINGE_JOINT_PARAMETERS "
                                     "WB_NODE_HINGE_2_JOINT "
                                     "WB_NODE_IMAGE_TEXTURE "
                                     "WB_NODE_IMMERSION_PROPERTIES "
                                     "WB_NODE_INDEXED_FACE_SET "
                                     "WB_NODE_INDEXED_LINE_SET "
                                     "WB_NODE_INERTIAL_UNIT "
                                     "WB_NODE_JOINT_PARAMETERS "
                                     "WB_NODE_LED "
                                     "WB_NODE_LENS "
                                     "WB_NODE_LENS_FLARE "
                                     "WB_NODE_LIDAR "
                                     "WB_NODE_LIGHT_SENSOR "
                                     "WB_NODE_LINEAR_MOTOR "
                                     "WB_NODE_MATERIAL "
                                     "WB_NODE_MESH "
                                     "WB_NODE_PBR_APPEARANCE "
                                     "WB_NODE_PEN "
                                     "WB_NODE_PHYSICS "
                                     "WB_NODE_PLANE "
                                     "WB_NODE_POINT_LIGHT "
                                     "WB_NODE_POINT_SET "
                                     "WB_NODE_POSITION_SENSOR "
                                     "WB_NODE_PROPELLER "
                                     "WB_NODE_RADAR "
                                     "WB_NODE_RANGE_FINDER "
                                     "WB_NODE_RECEIVER "
                                     "WB_NODE_RECOGNITION "
                                     "WB_NODE_ROBOT "
                                     "WB_NODE_ROTATIONAL_MOTOR "
                                     "WB_NODE_SHAPE "
                                     "WB_NODE_SKIN "
                                     "WB_NODE_SLIDER_JOINT "
                                     "WB_NODE_SLOT "
                                     "WB_NODE_SOLID "
                                     "WB_NODE_SOLID_REFERENCE "
                                     "WB_NODE_SPEAKER "
                                     "WB_NODE_SPHERE "
                                     "WB_NODE_SPOT_LIGHT "
                                     "WB_NODE_TEXTURE_COORDINATE "
                                     "WB_NODE_TEXTURE_TRANSFORM "
                                     "WB_NODE_TOUCH_SENSOR "
                                     "WB_NODE_TRACK "
                                     "WB_NODE_TRACK_WHEEL "
                                     "WB_NODE_TRANSFORM "
                                     "WB_NODE_VIEWPOINT "
                                     "WB_NODE_WORLD_INFO "
                                     "WB_NODE_ZOOM";
//  "WB_NODE_MICROPHONE "  // private
//  "WB_NODE_RADIO "       // private
//  "WB_NODE_SWITCH "      // private

static const char *API_CLASSES =
  "Accelerometer Brake Camera CameraRecognitionObject Car Compass Connector ControlMode "
  "CoordinateSystem Driver Display DistanceSensor Emitter EngineType Field GPS Gyro Keyboard "
  "InertialUnit ImageRef IndicatorState Joystick LED Lidar LidarPoint LightSensor LinearMotor Mode Motion Motor "
  "MouseState Node Pen PositionSensor Radar RadarTarget RangeFinder Receiver Robot RotationalMotor "
  "SimulationMode Skin Speaker Supervisor TouchSensor Type UserInputEvent WiperMode WheelIndex";

static const char *WBT_OBJECTS =
  "Appearance Background BallJoint BallJointParameters Billboard Box "
  "Capsule Charger Color Cone ContactProperties Coordinate Cylinder Damping "
  "DirectionalLight ElevationGrid Fluid Focus Fog Group HingeJoint "
  "HingeJointParameters Hinge2Joint ImageTexture ImmersionProperties "
  "IndexedFaceSet IndexedLineSet JointParameters Lens LensFlare Material Mesh Normal PBRAppearance "
  "Physics Plane PointLight Propeller Recognition Shape SliderJoint Slot Solid SolidReference "
  "Sphere SpotLight TextureCoordinate TextureTransform Track TrackWheel "
  "Transform Viewpoint WorldInfo Zoom";

static WbLanguage **gLanguages = NULL;

void WbLanguage::cleanup() {
  for (int i = 0; i < MAX; i++)
    delete gLanguages[i];
}

WbLanguage::WbLanguage(int code, const QString &name, const QString &defaultFileSuffix, const QString &commentPrefix,
                       bool isCompilable) :
  mCode(code),
  mName(name),
  mDefaultFileSuffix(defaultFileSuffix),
  mCommentPrefix(commentPrefix),
  mIsCompilable(isCompilable) {
}

WbLanguage::~WbLanguage() {
}

void WbLanguage::addKeywords(QString words) {
  mKeywords += words.split(" ");
}

void WbLanguage::addApiWords(QString words) {
  mApiWords += words.split(" ");
}

void WbLanguage::addPreprocessorWords(QString words) {
  mPreprocessorWords += words.split(" ");
}

WbLanguage *WbLanguage::findByCode(int code) {
  assert(code >= 0 && code < MAX);

  if (gLanguages == NULL) {
    gLanguages = new WbLanguage *[MAX];

    for (int i = 0; i < MAX; i++)
      gLanguages[i] = NULL;

    qAddPostRoutine(cleanup);
  }

  if (gLanguages[code] != NULL)
    return gLanguages[code];

  WbLanguage *language = NULL;
  switch (code) {
    case PLAIN_TEXT:
      language = new WbLanguage(PLAIN_TEXT, "Plain text", "txt", "", false);
      break;
    case C:
      language = new WbLanguage(C, "C", "c", "//", true);
      language->addKeywords(C_KEYWORDS);
      language->addApiWords(C_API_FUNCTIONS);
      language->addApiWords(C_API_DATA_TYPES);
      language->addApiWords(C_API_CONSTANTS);
      language->addPreprocessorWords(C_PREPROCESSOR_KEYWORDS);
      break;
    case CPP:
      language = new WbLanguage(CPP, "C++", "cpp", "//", true);
      language->addKeywords(C_KEYWORDS);
      language->addKeywords(CPP_KEYWORDS);
      language->addApiWords(C_API_FUNCTIONS);
      language->addApiWords(C_API_DATA_TYPES);
      language->addApiWords(C_API_CONSTANTS);
      language->addApiWords(API_CLASSES);
      language->addPreprocessorWords(C_PREPROCESSOR_KEYWORDS);
      break;
    case JAVA:
      language = new WbLanguage(JAVA, "Java", "java", "//", true);
      language->addKeywords(JAVA_KEYWORDS);
      language->addApiWords(API_CLASSES);
      break;
    case PYTHON:
      language = new WbLanguage(PYTHON, "Python", "py", "#", false);
      language->addKeywords(PYTHON_KEYWORDS);
      language->addApiWords(API_CLASSES);
      break;
    case MATLAB:
      language = new WbLanguage(MATLAB, "Matlab", "m", "%", false);
      language->addKeywords(MATLAB_KEYWORDS);
      language->addApiWords(C_API_FUNCTIONS);
      language->addApiWords(C_API_CONSTANTS);
      break;
    case WBT:
      language = new WbLanguage(WBT, "Webots", "wbt", "#", false);
      language->addKeywords(WBT_KEYWORDS);
      language->addApiWords(API_CLASSES);
      language->addApiWords(WBT_OBJECTS);
      break;
    case PROTO:
      language = new WbLanguage(PROTO, "Webots", "proto", "#", false);
      language->addKeywords(WBT_KEYWORDS);
      language->addApiWords(API_CLASSES);
      language->addApiWords(WBT_OBJECTS);
      break;
    case LUA:
      language = new WbLanguage(LUA, "Lua", "lua", "--", false);
      language->addKeywords(LUA_KEYWORDS);
      break;
    case MAKEFILE:
      language = new WbLanguage(MAKEFILE, "Makefile", "", "#", true);
      language->addKeywords(MAKEFILE_KEYWORDS);
      break;
    default:
      assert(0);
  }

  gLanguages[code] = language;
  return language;
}

WbLanguage *WbLanguage::findByFileName(const QString &fileName) {
  QFileInfo fi(fileName);
  QString ext = fi.suffix().toLower();

  int code = PLAIN_TEXT;
  if (ext == "c" || ext == "h")
    code = C;
  else if (ext == "java")
    code = JAVA;
  else if (ext == "py")
    code = PYTHON;
  else if (ext == "cpp" || ext == "cc" || ext == "c++" || ext == "hpp" || ext == "hh" || ext == "h++")
    code = CPP;
  else if (ext == "m")
    code = MATLAB;
  else if (ext == "wbt" || ext == "wrl" || ext == "wbo")
    code = WBT;
  else if (ext == "proto")
    code = PROTO;
  else if (ext == "lua")
    code = LUA;

  QString baseName = fi.baseName().toLower();
  if (baseName == "makefile")
    code = MAKEFILE;

  return findByCode(code);
}

const QStringList &WbLanguage::sourceFileExtensions() {
  static const QStringList EXTENSIONS = QStringList() << ".c"
                                                      << ".cc"
                                                      << ".cpp"
                                                      << ".c++"
                                                      << ".cxx"
                                                      << ".java"
                                                      << ".py"
                                                      << ".m"
                                                      << ".js";
  return EXTENSIONS;
}

const QStringList &WbLanguage::headerFileExtensions() {
  static const QStringList EXTENSIONS = QStringList() << ".h"
                                                      << ".hh"
                                                      << ".hpp"
                                                      << ".h++"
                                                      << ".hxx";
  return EXTENSIONS;
}

const QStringList &WbLanguage::dataFileExtensions() {
  static const QStringList EXTENSIONS = QStringList() << ".txt"
                                                      << ".xml"
                                                      << ".json"
                                                      << ".yaml"
                                                      << ".ini"
                                                      << ".csv"
                                                      << ".motion"
                                                      << ".html"
                                                      << ".css";
  return EXTENSIONS;
}

const QStringList &WbLanguage::exectuableExtensions() {
  static const QStringList EXTENSIONS = QStringList() << ""
                                                      << ".exe"
                                                      << ".class"
                                                      << ".jar"
                                                      << ".so"
                                                      << ".dylib"
                                                      << ".dll";
  return EXTENSIONS;
}

bool WbLanguage::isUnixLibraryExtension(const QString &extension) {
  return extension == ".so" || extension == ".dylib";
}

const QString &WbLanguage::compilationExtension() const {
  static QString COMPILATION_EXTENSION = "";
  switch (mCode) {
    case C:
    case CPP:
      COMPILATION_EXTENSION = ".o";
      break;
    case JAVA:
      COMPILATION_EXTENSION = ".class";
      break;
    default:
      break;
  }
  return COMPILATION_EXTENSION;
}
