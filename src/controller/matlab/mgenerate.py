#!/usr/bin/env python3

# Copyright 1996-2021 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Generate the matlab files.

Execute with '-update' to update the list of generated functions"""

import os
import re
import sys

PROC = 0
FUNC = 1
FOLDER = os.environ['WEBOTS_HOME'] + '/lib/controller/matlab/'
GITIGNORE = FOLDER + ".gitignore"


def gen_with_doc(type, line, doc_url=None):
    match = re.match(r'(.*)\((.*?)\)', line)
    assert match
    function = match.group(1)
    arguments = match.group(2)
    with open(FOLDER + function + '.m', 'w', newline='\n') as file:
        result = 'result = ' if type == FUNC else ''
        file.write('function %s%s(%s)\n' % (result, function, arguments))
        file.write('%% Usage: %s(%s)\n' % (function, arguments))
        file.write('% Matlab API for Webots\n')
        if doc_url is not None:
            file.write(
                '%% Online documentation is available <a href=\"%s\">here</a>\n' % doc_url)
        file.write('\n')
        file.write("%scalllib('libController', '%s'%s%s);\n" %
                   (result, function, ', ' if arguments else '', arguments))
        if UPDATE:
            GITIGNOREFILE.write(function + '.m\n')


def gen(type, line, doc_page=None):
    gen_with_doc(type, line, 'https://www.cyberbotics.com/doc/reference/' +
                 doc_page if doc_page is not None else None)


def gen_const(name, value):
    with open(FOLDER + name + '.m', 'w', newline='\n') as file:
        file.write('function value = %s\n' % name)
        file.write('value = %s;\n' % value)
        if UPDATE:
            GITIGNOREFILE.write(name + '.m\n')


def gen_consts_from_list(list):
    items = list.split(',')
    for i in range(len(items)):
        gen_const(items[i].strip(), i)


def main(args=None):
    global UPDATE
    if args:
        UPDATE = (args[0] == "-update")
    else:
        UPDATE = False
    if UPDATE:
        if os.path.exists(GITIGNORE):
            os.remove(GITIGNORE)
        global GITIGNOREFILE
        GITIGNOREFILE = open(GITIGNORE, 'w')

    # accelerometer.h
    gen(PROC, "wb_accelerometer_enable(tag, sampling_period))", "accelerometer")
    gen(PROC, "wb_accelerometer_disable(tag)", "accelerometer")
    gen(FUNC, "wb_accelerometer_get_sampling_period(tag)", "accelerometer")
    # gen(FUNC, "wb_accelerometer_get_lookup_table_size(tag)", "accelerometer")
    # gen(FUNC, "wb_accelerometer_get_lookup_table(tag)", "accelerometer")
    # gen(FUNC, "wb_accelerometer_get_values(tag)", "accelerometer")

    # brake.h
    gen(FUNC, "wb_brake_get_type(tag)", "brake")
    gen(PROC, "wb_brake_set_damping_constant(tag, damping_constant)", "brake")
    gen(FUNC, "wb_brake_get_motor(tag)", "brake")
    gen(FUNC, "wb_brake_get_position_sensor(tag)", "brake")

    # camera.h
    gen(PROC, "wb_camera_enable(tag, sampling_period)", "camera")
    gen(PROC, "wb_camera_disable(tag)", "camera")
    gen(FUNC, "wb_camera_get_sampling_period(tag)", "camera")
    gen(FUNC, "wb_camera_get_width(tag)", "camera")
    gen(FUNC, "wb_camera_get_height(tag)", "camera")
    # gen(FUNC, "wb_camera_get_image(tag)", "camera")
    gen(FUNC, "wb_camera_get_fov(tag)", "camera")
    gen(FUNC, "wb_camera_get_max_fov(tag)", "camera")
    gen(FUNC, "wb_camera_get_min_fov(tag)", "camera")
    gen(PROC, "wb_camera_set_fov(tag, fov)", "camera")
    gen(FUNC, "wb_camera_get_exposure(tag)", "camera")
    gen(PROC, "wb_camera_set_exposure(tag, exposure)", "camera")
    gen(FUNC, "wb_camera_get_focal_length(tag)", "camera")
    gen(FUNC, "wb_camera_get_focal_distance(tag)", "camera")
    gen(FUNC, "wb_camera_get_max_focal_distance(tag)", "camera")
    gen(FUNC, "wb_camera_get_min_focal_distance(tag)", "camera")
    gen(PROC, "wb_camera_set_focal_distance(tag, focal_distance)", "camera")
    gen(FUNC, "wb_camera_get_near(tag)", "camera")
    gen(FUNC, "wb_camera_has_recognition(tag)", "camera")
    gen(PROC, "wb_camera_recognition_disable(tag)", "camera")
    gen(PROC, "wb_camera_recognition_enable(tag, sampling_period)", "camera")
    gen(FUNC, "wb_camera_recognition_get_number_of_objects(tag)", "camera")
    #  gen(FUNC, "wb_camera_recognition_get_objects(tag)", "camera")
    gen(FUNC, "wb_camera_recognition_get_sampling_period(tag)", "camera")
    # gen(FUNC, "wb_camera_recognition_get_segmentation_image(tag)", "camera")")
    gen(PROC, "wb_camera_recognition_disable_segmentation(tag)", "camera")
    gen(PROC, "wb_camera_recognition_enable_segmentation(tag)", "camera")
    gen(PROC, "wb_camera_recognition_is_segmentation_enabled(tag)", "camera")
    gen(FUNC, "wb_camera_recognition_has_segmentation(tag)", "camera")
    gen(FUNC, "wb_camera_recognition_save_segmentation_image(tag, filename, quality)", "camera")
    gen(FUNC, "wb_camera_save_image(tag, filename, quality)", "camera")

    # compass.h
    gen(PROC, "wb_compass_enable(tag, sampling_period)", "compass")
    gen(PROC, "wb_compass_disable(tag)", "compass")
    gen(FUNC, "wb_compass_get_sampling_period(tag)", "compass")
    # gen(FUNC, "wb_compass_get_lookup_table_size(tag)", "compass")
    # gen(FUNC, "wb_compass_get_lookup_table(tag)", "compass")
    # gen(FUNC, "wb_compass_get_values(tag)", "compass")

    # connector.h
    gen(PROC, "wb_connector_enable_presence(tag, sampling_period)", "connector")
    gen(PROC, "wb_connector_disable_presence(tag)", "connector")
    gen(FUNC, "wb_connector_get_presence(tag)", "connector")
    gen(FUNC, "wb_connector_get_presence_sampling_period(tag)", "connector")
    gen(FUNC, "wb_connector_is_locked(tag)", "connector")
    gen(PROC, "wb_connector_lock(tag)", "connector")
    gen(PROC, "wb_connector_unlock(tag)", "connector")

    # console.h
    gen_with_doc(PROC, "wb_console_print(txt, stream)",
                 "https://www.cyberbotics.com/doc/guide/using-matlab")

    # display
    gen(FUNC, "wb_display_get_width(tag)", "display")
    gen(FUNC, "wb_display_get_height(tag)", "display")
    # gen(PROC, "wb_display_set_color(tag, color)", "display")
    gen(PROC, "wb_display_set_alpha(tag, alpha)", "display")
    gen(PROC, "wb_display_set_opacity(tag, opacity)", "display")
    gen(PROC, "wb_display_set_font(tag, font, size, anti_aliasing)", "display")
    gen(PROC, "wb_display_attach_camera(tag, camera_tag)", "display")
    gen(PROC, "wb_display_detach_camera(tag)", "display")
    gen(PROC, "wb_display_draw_pixel(tag, x, y)", "display")
    gen(PROC, "wb_display_draw_line(tag, x1, y1, x2, y2)", "display")
    gen(PROC, "wb_display_draw_rectangle(tag, x, y, width, height)", "display")
    gen(PROC, "wb_display_draw_oval(tag, cx, cy, a, b)", "display")
    # gen(PROC, "wb_display_draw_polygon(tag, x, y)", "display")
    gen(PROC, "wb_display_draw_text(tag, txt, x, y)", "display")
    gen(PROC, "wb_display_fill_rectangle(tag, x, y, width, height)", "display")
    gen(PROC, "wb_display_fill_oval(tag, cx, cy, a, b)", "display")
    # gen(PROC, "wb_display_fill_polygon(tag, x, y)", "display")
    gen(FUNC, "wb_display_image_copy(tag, x, y, width, height)", "display")
    # gen(PROC, "wb_display_image_new(tag, width, height, rgb, format)", "display")
    gen(PROC, "wb_display_image_paste(tag, imageref, x, y, blend)", "display")
    gen(FUNC, "wb_display_image_load(tag, filename)", "display")
    gen(PROC, "wb_display_image_save(tag, imageref, filename)", "display")
    gen(PROC, "wb_display_image_delete(tag, imageref)", "display")

    # device.h
    gen(FUNC, "wb_device_get_model(tag)", "device")
    gen(FUNC, "wb_device_get_name(tag)", "device")
    gen(FUNC, "wb_device_get_node_type(tag)", "device")

    # distance_sensor.h
    gen(PROC, "wb_distance_sensor_enable(tag, sampling_period)", "distancesensor")
    gen(PROC, "wb_distance_sensor_disable(tag)", "distancesensor")
    gen(FUNC, "wb_distance_sensor_get_sampling_period(tag)", "distancesensor")
    gen(FUNC, "wb_distance_sensor_get_value(tag)", "distancesensor")
    gen(FUNC, "wb_distance_sensor_get_max_value(tag)", "distancesensor")
    gen(FUNC, "wb_distance_sensor_get_min_value(tag)", "distancesensor")
    gen(FUNC, "wb_distance_sensor_get_aperture(tag)", "distancesensor")
    # gen(FUNC, "wb_distance_sensor_get_lookup_table_size(tag)", "distancesensor")
    # gen(FUNC, "wb_distance_sensor_get_lookup_table(tag)", "distancesensor")
    gen(FUNC, "wb_distance_sensor_get_type(tag)", "distancesensor")

    # emitter.h
    # gen(FUNC, "wb_emitter_send(tag, data, size)", "emitter")
    gen(FUNC, "wb_emitter_get_buffer_size(tag)", "emitter")
    gen(PROC, "wb_emitter_set_channel(tag, channel)", "emitter")
    gen(FUNC, "wb_emitter_get_channel(tag)", "emitter")
    gen(FUNC, "wb_emitter_get_range(tag)", "emitter")
    gen(PROC, "wb_emitter_set_range(tag, range)", "emitter")

    # gps.h
    gen(PROC, "wb_gps_enable(tag, sampling_period)", "gps")
    gen(FUNC, "wb_gps_convert_to_degrees_minutes_seconds(decimal_degrees)", "gps")
    gen(PROC, "wb_gps_disable(tag)", "gps")
    gen(FUNC, "wb_gps_get_coordinate_system(tag)", "gps")
    gen(FUNC, "wb_gps_get_sampling_period(tag)", "gps")
    gen(FUNC, "wb_gps_get_speed(tag)", "gps")
    # gen(FUNC, "wb_gps_get_values(tag)", "gps")

    # gyro.h
    gen(PROC, "wb_gyro_enable(tag, sampling_period)", "gyro")
    gen(PROC, "wb_gyro_disable(tag)", "gyro")
    gen(FUNC, "wb_gyro_get_sampling_period(tag)", "gyro")
    # gen(FUNC, "wb_gyro_get_lookup_table_size(tag)", "gyro")
    # gen(FUNC, "wb_gyro_get_lookup_table(tag)", "gyro")
    # gen(FUNC, "wb_gyro_get_values(tag)", "gyro")

    # inertial_unit.h
    gen(PROC, "wb_inertial_unit_enable(tag, sampling_period)", "inertialunit")
    gen(PROC, "wb_inertial_unit_disable(tag)", "inertialunit")
    gen(FUNC, "wb_inertial_unit_get_sampling_period(tag)", "inertialunit")
    gen(FUNC, "wb_inertial_unit_get_noise(tag)", "inertialunit")
    # gen(FUNC, "wb_inertial_unit_get_quaternion(tag)", "inertialunit")
    # gen(FUNC, "wb_inertial_unit_get_roll_pitch_yaw(tag)", "inertialunit")

    # joystick.h
    gen(PROC, "wb_joystick_enable(sampling_period)", "joystick")
    gen(PROC, "wb_joystick_disable()", "joystick")
    gen(FUNC, "wb_joystick_get_sampling_period()", "joystick")
    gen(FUNC, "wb_joystick_is_connected()", "joystick")
    gen(FUNC, "wb_joystick_get_model()", "joystick")
    gen(FUNC, "wb_joystick_get_number_of_axes()", "joystick")
    gen(FUNC, "wb_joystick_get_axis_value(axis)", "joystick")
    gen(FUNC, "wb_joystick_get_number_of_povs()", "joystick")
    gen(FUNC, "wb_joystick_get_pov_value(pov)", "joystick")
    gen(FUNC, "wb_joystick_get_pressed_button()", "joystick")
    gen(PROC, "wb_joystick_set_constant_force(level)", "joystick")
    gen(PROC, "wb_joystick_set_constant_force_duration(duration)", "joystick")
    gen(PROC, "wb_joystick_set_auto_centering_gain(gain)", "joystick")
    gen(PROC, "wb_joystick_set_resistance_gain(gain)", "joystick")
    gen(PROC, "wb_joystick_set_force_axis(axis)", "joystick")

    # keyboard.h
    gen(PROC, "wb_keyboard_enable(sampling_period)", "keyboard")
    gen(PROC, "wb_keyboard_disable()", "keyboard")
    gen(FUNC, "wb_keyboard_get_sampling_period()", "keyboard")
    gen(FUNC, "wb_keyboard_get_key()", "keyboard")

    # led.h
    gen(PROC, "wb_led_set(tag, value)", "led")
    gen(FUNC, "wb_led_get(tag)", "led")

    # lidar.h
    gen(PROC, "wb_lidar_enable(tag, sampling_period)", "lidar")
    gen(PROC, "wb_lidar_enable_point_cloud(tag)", "lidar")
    gen(PROC, "wb_lidar_disable(tag)", "lidar")
    gen(PROC, "wb_lidar_disable_point_cloud(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_sampling_period(tag)", "lidar")
    gen(FUNC, "wb_lidar_is_point_cloud_enabled(tag)", "lidar")
    # gen(FUNC, "wb_lidar_get_range_image(tag)", "lidar")
    # gen(FUNC, "wb_lidar_get_layer_range_image(tag,layer)", "lidar")
    # gen(FUNC, "wb_lidar_get_point_cloud(tag)", "lidar")
    # gen(FUNC, "wb_lidar_get_layer_point_cloud(tag,layer)", "lidar")
    gen(FUNC, "wb_lidar_get_horizontal_resolution(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_number_of_layers(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_min_frequency(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_max_frequency(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_frequency(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_number_of_points(tag)", "lidar")
    gen(PROC, "wb_lidar_set_frequency(tag,frequency)", "lidar")
    gen(FUNC, "wb_lidar_get_fov(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_vertical_fov(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_min_range(tag)", "lidar")
    gen(FUNC, "wb_lidar_get_max_range(tag)", "lidar")

    # light_sensor.h
    gen(PROC, "wb_light_sensor_enable(tag, sampling_period)", "lightsensor")
    gen(PROC, "wb_light_sensor_disable(tag)", "lightsensor")
    gen(FUNC, "wb_light_sensor_get_sampling_period(tag)", "lightsensor")
    gen(FUNC, "wb_light_sensor_get_value(tag)", "lightsensor")
    # gen(FUNC, "wb_light_sensor_get_lookup_table_size(tag)", "lightsensor")
    # gen(FUNC, "wb_light_sensor_get_lookup_table(tag)", "lightsensor")

    # motor.h
    gen(PROC, "wb_motor_set_acceleration(tag, acceleration)", "motor")
    gen(PROC, "wb_motor_set_velocity(tag, velocity)", "motor")
    gen(PROC, "wb_motor_set_force(tag, force)", "motor")
    gen(PROC, "wb_motor_set_available_force(tag, force)", "motor")
    gen(PROC, "wb_motor_set_control_pid(tag, p, i, d)", "motor")
    gen(PROC, "wb_motor_enable_force_feedback(tag, sampling_period)", "motor")
    gen(PROC, "wb_motor_disable_force_feedback(tag)", "motor")
    gen(FUNC, "wb_motor_get_force_feedback_sampling_period(tag)", "motor")
    gen(FUNC, "wb_motor_get_force_feedback(tag)", "motor")
    gen(PROC, "wb_motor_set_torque(tag, torque)", "motor")
    gen(PROC, "wb_motor_set_available_torque(tag, torque)", "motor")
    gen(PROC, "wb_motor_enable_torque_feedback(tag, sampling_period)", "motor")
    gen(PROC, "wb_motor_disable_torque_feedback(tag)", "motor")
    gen(FUNC, "wb_motor_get_torque_feedback_sampling_period(tag)", "motor")
    gen(FUNC, "wb_motor_get_torque_feedback(tag)", "motor")
    gen(PROC, "wb_motor_set_position(tag, position)", "motor")
    gen(FUNC, "wb_motor_get_type(tag)", "motor")
    gen(FUNC, "wb_motor_get_target_position(tag)", "motor")
    gen(FUNC, "wb_motor_get_min_position(tag)", "motor")
    gen(FUNC, "wb_motor_get_max_position(tag)", "motor")
    gen(FUNC, "wb_motor_get_velocity(tag)", "motor")
    gen(FUNC, "wb_motor_get_max_velocity(tag)", "motor")
    gen(FUNC, "wb_motor_get_acceleration(tag)", "motor")
    gen(FUNC, "wb_motor_get_available_force(tag)", "motor")
    gen(FUNC, "wb_motor_get_max_force(tag)", "motor")
    gen(FUNC, "wb_motor_get_available_torque(tag)", "motor")
    gen(FUNC, "wb_motor_get_max_torque(tag)", "motor")
    gen(FUNC, "wb_motor_get_multiplier(tag)", "motor")
    gen(FUNC, "wb_motor_get_brake(tag)", "motor")
    gen(FUNC, "wb_motor_get_position_sensor(tag)", "motor")

    # mouse.h
    gen(PROC, "wb_mouse_enable(sampling_period)", "mouse")
    gen(PROC, "wb_mouse_enable_3d_position()", "mouse")
    gen(PROC, "wb_mouse_disable()", "mouse")
    gen(PROC, "wb_mouse_disable_3d_position()", "mouse")
    gen(FUNC, "wb_mouse_is_3d_position_enabled()", "mouse")
    gen(FUNC, "wb_mouse_get_sampling_period()", "mouse")
    # gen(FUNC, "wb_mouse_get_state()", "mouse")

    # pen.h
    gen(PROC, "wb_pen_write(tag, write)", "pen")
    # gen(PROC, "wb_pen_set_ink_color(tag, color, density)", "pen")

    # position_sensor.h
    gen(PROC, "wb_position_sensor_enable(tag, sampling_period)", "positionsensor")
    gen(PROC, "wb_position_sensor_disable(tag)", "positionsensor")
    gen(FUNC, "wb_position_sensor_get_sampling_period(tag)", "positionsensor")
    gen(FUNC, "wb_position_sensor_get_value(tag)", "positionsensor")
    gen(FUNC, "wb_position_sensor_get_type(tag)", "positionsensor")
    gen(FUNC, "wb_position_sensor_get_brake(tag)", "positionsensor")
    gen(FUNC, "wb_position_sensor_get_motor(tag)", "positionsensor")

    # radar.h
    gen(PROC, "wb_radar_enable(tag, sampling_period)", "radar")
    gen(PROC, "wb_radar_disable(tag)", "radar")
    gen(FUNC, "wb_radar_get_sampling_period(tag)", "radar")
    gen(FUNC, "wb_radar_get_number_of_targets(tag)", "radar")
    # gen(FUNC, "wb_radar_get_targets(tag)", "radar")
    gen(FUNC, "wb_radar_get_min_range(tag)", "radar")
    gen(FUNC, "wb_radar_get_max_range(tag)", "radar")
    gen(FUNC, "wb_radar_get_horizontal_fov(tag)", "radar")
    gen(FUNC, "wb_radar_get_vertical_fov(tag)", "radar")

    # range_finder.h
    gen(PROC, "wb_range_finder_enable(tag, sampling_period)", "rangefinder")
    gen(PROC, "wb_range_finder_disable(tag)", "rangefinder")
    gen(FUNC, "wb_range_finder_get_sampling_period(tag)", "rangefinder")
    gen(FUNC, "wb_range_finder_get_width(tag)", "rangefinder")
    gen(FUNC, "wb_range_finder_get_height(tag)", "rangefinder")
    # gen(FUNC, "wb_range_finder_get_range_image(tag)", "rangefinder")
    gen(FUNC, "wb_range_finder_get_fov(tag)", "rangefinder")
    gen(FUNC, "wb_range_finder_get_min_range(tag)", "rangefinder")
    gen(FUNC, "wb_range_finder_get_max_range(tag)", "rangefinder")
    gen(FUNC, "wb_range_finder_save_image(tag, filename, quality)", "rangefinder")
    # gen(FUNC, "wb_range_finder_image_get_depth(image, width, x, y)", "rangefinder")

    # receiver.h
    gen(PROC, "wb_receiver_enable(tag, sampling_period)", "receiver")
    gen(PROC, "wb_receiver_disable(tag)", "receiver")
    gen(FUNC, "wb_receiver_get_sampling_period(tag)", "receiver")
    gen(PROC, "wb_receiver_set_channel(tag, channel)", "receiver")
    gen(FUNC, "wb_receiver_get_channel(tag)", "receiver")
    gen(FUNC, "wb_receiver_get_queue_length(tag)", "receiver")
    gen(PROC, "wb_receiver_next_packet(tag)", "receiver")
    # gen(FUNC, "wb_receiver_get_data(tag)", "receiver")
    # gen(FUNC, "wb_receiver_get_emitter_direction(tag)", "receiver")
    gen(FUNC, "wb_receiver_get_data_size(tag)", "receiver")
    gen(FUNC, "wb_receiver_get_signal_strength(tag)", "receiver")
    gen(FUNC, "wb_robot_get_number_of_devices()", "receiver")
    gen(FUNC, "wb_robot_get_device_by_index(index)", "receiver")
    gen(FUNC, "wb_robot_get_type()", "receiver")

    # robot.h
    # gen(PROC, "wb_robot_init()", "robot")
    # gen(PROC, "wb_robot_cleanup()", "robot")
    gen(FUNC, "wb_robot_step(duration)", "robot")
    gen(FUNC, "wb_robot_wait_for_user_input_event(event_type, timeout)", "robot")
    gen(PROC, "wb_robot_battery_sensor_enable(sampling_period)", "robot")
    gen(PROC, "wb_robot_battery_sensor_disable()", "robot")
    gen(FUNC, "wb_robot_battery_sensor_get_sampling_period()", "robot")
    gen(FUNC, "wb_robot_battery_sensor_get_value()", "robot")
    gen(FUNC, "wb_robot_get_mode()", "robot")
    gen(PROC, "wb_robot_set_mode(mode, arg)", "robot")
    gen(FUNC, "wb_robot_get_custom_data()", "robot")
    gen(FUNC, "wb_robot_get_data()", "robot")
    gen(FUNC, "wb_robot_get_time()", "robot")
    gen(FUNC, "wb_robot_get_name()", "robot")
    gen(FUNC, "wb_robot_get_synchronization()", "robot")
    gen(FUNC, "wb_robot_get_supervisor()", "robot")
    gen(FUNC, "wb_robot_get_project_path()", "robot")
    gen(FUNC, "wb_robot_get_basic_time_step()", "robot")
    gen(FUNC, "wb_robot_get_device(name)", "robot")
    gen(FUNC, "wb_robot_get_urdf(prefix)", "robot")
    gen(FUNC, "wb_robot_get_world_path()", "robot")
    gen(PROC, "wb_robot_set_custom_data(data)", "robot")
    gen(PROC, "wb_robot_set_data(data)", "robot")
    gen(FUNC, "wb_robot_wwi_receive_text()", "robot")
    gen(PROC, "wb_robot_wwi_send_text(text)", "robot")

    # skin.h
    # gen(PROC, "wb_skin_set_bone_orientation(tag, index, values, absolute)")
    # gen(PROC, "wb_skin_set_bone_position(tag, index, values, absolute)")
    gen(FUNC, "wb_skin_get_bone_count(tag)")
    gen(FUNC, "wb_skin_get_bone_name(tag, index)")
    # gen(FUNC, "wb_skin_get_bone_orientation(tag, index, absolute)")
    # gen(FUNC, "wb_skin_get_bone_position(tag, index, absolute)")

    # speaker.h
    gen(FUNC, "wb_speaker_get_engine(tag)", "speaker")
    gen(FUNC, "wb_speaker_get_language(tag)", "speaker")
    gen(FUNC, "wb_speaker_is_sound_playing(tag, sound)", "speaker")
    gen(FUNC, "wb_speaker_is_speaking(tag)", "speaker")
    gen(PROC, "wb_speaker_play_sound(left, right, sound, volume, pitch, balance, loop)", "speaker")
    gen(PROC, "wb_speaker_stop(tag, sound)", "speaker")
    gen(FUNC, "wb_speaker_set_engine(tag, language)", "speaker")
    gen(FUNC, "wb_speaker_set_language(tag, language)", "speaker")
    gen(PROC, "wb_speaker_speak(tag, text, volume)", "speaker")

    # supervisor.h
    gen(PROC, "wb_supervisor_simulation_quit(status)", "supervisor")
    # gen(PROC, "wb_supervisor_simulation_revert()", "supervisor"); # DEPRECATED
    gen(PROC, "wb_supervisor_simulation_reset()", "supervisor")
    gen(PROC, "wb_supervisor_simulation_reset_physics()", "supervisor")
    gen(PROC, "wb_supervisor_simulation_set_mode(mode)", "supervisor")
    gen(FUNC, "wb_supervisor_simulation_get_mode()", "supervisor")
    # gen(PROC, "wb_supervisor_simulation_physics_reset()", "supervisor"); # DEPRECATED
    # gen(FUNC, "wb_supervisor_save_world(filename)", "supervisor"); # DEPRECATED
    # gen(PROC, "wb_supervisor_load_world(filename)", "supervisor"); # DEPRECATED
    gen(PROC, "wb_supervisor_world_load(filename)", "supervisor")
    gen(PROC, "wb_supervisor_world_reload()", "supervisor")
    # gen(FUNC, "wb_supervisor_world_save(filename)", "supervisor"); # implemented due to the default arguments management
    gen(PROC, "wb_supervisor_export_image(filename, quality)", "supervisor")
    gen(FUNC, "wb_supervisor_animation_start_recording(filename)", "supervisor")
    gen(FUNC, "wb_supervisor_animation_stop_recording()", "supervisor")
    gen(PROC, "wb_supervisor_movie_start_recording(filename, width, height, codec, quality, acceleration, caption)",
        "supervisor")
    # gen(PROC, "wb_supervisor_start_movie(filename, width, height, codec, quality, acceleration, caption)");  # DEPRECATED
    gen(PROC, "wb_supervisor_movie_stop_recording()", "supervisor")
    gen(FUNC, "wb_supervisor_movie_is_ready()", "supervisor")
    gen(FUNC, "wb_supervisor_movie_failed()", "supervisor")
    # gen(PROC, "wb_supervisor_stop_movie()", "supervisor"); # DEPRECATED
    # gen(FUNC, "wb_supervisor_movie_get_status()", "supervisor"); # DEPRECATED
    # gen(FUNC, "wb_supervisor_get_movie_status()", "supervisor"); # DEPRECATED
    # gen(PROC, "wb_supervisor_set_label()", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_root()", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_self()", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_from_def(defname)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_from_proto_def(noderef, defname)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_from_id(id)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_from_device(tag)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_def(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_id(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_type(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_type_name(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_base_type_name(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_field(noderef, fieldname)", "supervisor")
    gen(FUNC, "wb_supervisor_field_enable_sf_tracking(field, sampling_period)", "supervisor")
    gen(FUNC, "wb_supervisor_field_disable_sf_tracking(field)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_proto_field(noderef, fieldname)", "supervisor")
    # gen(FUNC, "wb_supervisor_node_get_center_of_mass(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_number_of_contact_points(noderef, include_descendants)", "supervisor")
    # gen(FUNC, "wb_supervisor_node_get_contact_point(noderef, index)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_contact_point_node(noderef, index)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_parent_node(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_selected()", "supervisor")
    # gen(FUNC, "wb_supervisor_node_get_position(noderef)", "supervisor")
    # gen(FUNC, "wb_supervisor_node_get_orientation(noderef)", "supervisor")
    # gen(FUNC, "wb_supervisor_node_get_pose(noderef, noderef_from)", "supervisor")
    gen(FUNC, "wb_supervisor_node_get_static_balance(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_enable_pose_tracking(sampling_period, node, from_node)", "supervisor")
    gen(FUNC, "wb_supervisor_node_disable_pose_tracking(node, from_node)", "supervisor")
    # gen(FUNC, "wb_supervisor_node_get_velocity(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_is_proto(noderef)", "supervisor")
    gen(PROC, "wb_supervisor_node_move_viewpoint(node)", "supervisor")
    gen(PROC, "wb_supervisor_node_set_visibility(node, from, visible)", "supervisor")
    gen(PROC, "wb_supervisor_node_set_velocity(noderef, velocity)", "supervisor")
    gen(PROC, "wb_supervisor_node_add_force(noderef, force, relative)", "supervisor")
    gen(PROC, "wb_supervisor_node_add_force_with_offset(noderef, force, offset, relative)", "supervisor")
    gen(PROC, "wb_supervisor_node_add_torque(noderef, torque, relative)", "supervisor")
    gen(FUNC, "wb_supervisor_node_reset_physics(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_restart_controller(noderef)", "supervisor")
    gen(PROC, "wb_supervisor_node_remove(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_export_string(noderef)", "supervisor")
    gen(FUNC, "wb_supervisor_node_save_state(noderef, state_name)", "supervisor")
    gen(FUNC, "wb_supervisor_node_load_state(noderef, state_name)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_type(fieldref)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_type_name(fieldref)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_count(fieldref)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_sf_bool(fieldref)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_sf_int32(fieldref)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_sf_float(fieldref)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_sf_string(fieldref)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_sf_node(fieldref)", "supervisor")
    # gen(FUNC, "wb_supervisor_field_get_sf_color(fieldref)", "supervisor")
    # gen(FUNC, "wb_supervisor_field_get_sf_rotation(fieldref)", "supervisor")
    # gen(FUNC, "wb_supervisor_field_get_sf_vec2f(fieldref)", "supervisor")
    # gen(FUNC, "wb_supervisor_field_get_sf_vec3f(fieldref)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_mf_bool(fieldref, index)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_mf_int32(fieldref, index)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_mf_float(fieldref, index)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_mf_string(fieldref, index)", "supervisor")
    gen(FUNC, "wb_supervisor_field_get_mf_node(fieldref, index)", "supervisor")
    # gen(FUNC, "wb_supervisor_field_get_mf_color(fieldref, index)", "supervisor")
    # gen(FUNC, "wb_supervisor_field_get_mf_rotation(fieldref, index)", "supervisor")
    # gen(FUNC, "wb_supervisor_field_get_mf_vec2f(fieldref, index)", "supervisor")
    # gen(FUNC, "wb_supervisor_field_get_mf_vec3f(fieldref, index)", "supervisor")
    gen(PROC, "wb_supervisor_field_set_sf_bool(fieldref, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_set_sf_int32(fieldref, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_set_sf_float(fieldref, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_set_sf_string(fieldref, string)", "supervisor")
    # gen(PROC, "wb_supervisor_field_set_sf_color(fieldref, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_set_sf_rotation(fieldref, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_set_sf_vec2f(fieldref, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_set_sf_vec3f(fieldref, values)", "supervisor")
    gen(PROC, "wb_supervisor_field_set_mf_bool(fieldref, index, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_set_mf_int32(fieldref, index, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_set_mf_float(fieldref, index, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_set_mf_string(fieldref, index, string)", "supervisor")
    # gen(PROC, "wb_supervisor_field_set_mf_color(fieldref, index, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_set_mf_rotation(fieldref, index, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_set_mf_vec2f(fieldref, index, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_set_mf_vec3f(fieldref, index, values)", "supervisor")
    gen(PROC, "wb_supervisor_field_insert_mf_bool(fieldref, index, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_insert_mf_int32(fieldref, index, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_insert_mf_float(fieldref, index, value)", "supervisor")
    gen(PROC, "wb_supervisor_field_insert_mf_string(fieldref, index, string)", "supervisor")
    # gen(PROC, "wb_supervisor_field_insert_mf_vec2f(fieldref, index, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_insert_mf_vec3f(fieldref, index, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_insert_mf_rotation(fieldref, index, values)", "supervisor")
    # gen(PROC, "wb_supervisor_field_insert_mf_color(fieldref, index, values)", "supervisor")
    gen(PROC, "wb_supervisor_field_remove_mf(fieldref, index)", "supervisor")
    gen(PROC, "wb_supervisor_field_import_mf_node(fieldref, position, filename)", "supervisor")
    gen(PROC, "wb_supervisor_field_import_mf_node_from_string(fieldref, position, node_string)", "supervisor")
    gen(PROC, "wb_supervisor_field_remove_mf_node(fieldref, position)", "supervisor")
    gen(PROC, "wb_supervisor_field_remove_sf(fieldref)", "supervisor")
    gen(PROC, "wb_supervisor_field_import_sf_node(fieldref, filename)", "supervisor")
    gen(PROC, "wb_supervisor_field_import_sf_node_from_string(fieldref, node_string)", "supervisor")
    # gen(FUNC, "wb_supervisor_virtual_reality_headset_get_position()", "supervisor")
    # gen(FUNC, "wb_supervisor_virtual_reality_headset_get_orientation()", "supervisor")
    gen(FUNC, "wb_supervisor_virtual_reality_headset_is_used()", "supervisor")

    # touch_sensor.h
    gen(PROC, "wb_touch_sensor_enable(tag, sampling_period)", "touchsensor")
    gen(PROC, "wb_touch_sensor_disable(tag)", "touchsensor")
    gen(FUNC, "wb_touch_sensor_get_sampling_period(tag)", "touchsensor")
    gen(FUNC, "wb_touch_sensor_get_value(tag)", "touchsensor")
    gen(FUNC, "wb_touch_sensor_get_type(tag)", "touchsensor")
    # gen(FUNC, "wb_touch_sensor_get_lookup_table_size(tag)", "touchsensor")
    # gen(FUNC, "wb_touch_sensor_get_lookup_table(tag)", "touchsensor")
    # gen(FUNC, "wb_touch_sensor_get_values(tag)", "touchsensor")

    # utils/motion.h
    gen(FUNC, "wbu_motion_new(filename)", "motion")
    gen(PROC, "wbu_motion_delete(motionref)", "motion")
    gen(PROC, "wbu_motion_play(motionref)", "motion")
    gen(PROC, "wbu_motion_stop(motionref)", "motion")
    gen(PROC, "wbu_motion_set_loop(motionref, loop)", "motion")
    gen(PROC, "wbu_motion_set_reverse(motionref, reverse)", "motion")
    gen(FUNC, "wbu_motion_is_over(motionref)", "motion")
    gen(FUNC, "wbu_motion_get_duration(motionref)", "motion")
    gen(FUNC, "wbu_motion_get_time(motionref)", "motion")
    gen(FUNC, "wbu_motion_set_time(motionref, time)", "motion")

    # utils/system.h
    gen(FUNC, "wbu_system_getenv(variable)")
    gen(FUNC, "wbu_system_short_path(path)")

    # constants
    gen_const("WB_STDOUT", "0")
    gen_const("WB_STDERR", "1")

    gen_const("WB_CHANNEL_BROADCAST", "-1")

    gen_const("WB_IMAGE_RGB",  "3")
    gen_const("WB_IMAGE_RGBA", "4")
    gen_const("WB_IMAGE_ARGB", "5")
    gen_const("WB_IMAGE_BGRA", "6")
    gen_const("WB_IMAGE_ABGR", "7")

    gen_const("WB_KEYBOARD_KEY",        "65535")
    gen_const("WB_KEYBOARD_SHIFT",      "65536")
    gen_const("WB_KEYBOARD_CONTROL",   "131072")
    gen_const("WB_KEYBOARD_ALT",       "262144")
    gen_const("WB_KEYBOARD_LEFT",         "314")
    gen_const("WB_KEYBOARD_UP",           "315")
    gen_const("WB_KEYBOARD_RIGHT",        "316")
    gen_const("WB_KEYBOARD_DOWN",         "317")
    gen_const("WB_KEYBOARD_PAGEUP",       "366")
    gen_const("WB_KEYBOARD_PAGEDOWN",     "367")
    gen_const("WB_KEYBOARD_HOME",         "313")
    gen_const("WB_KEYBOARD_END",          "312")
    gen_const("WB_KEYBOARD_NUMPAD_UP",    "377")
    gen_const("WB_KEYBOARD_NUMPAD_DOWN",  "379")
    gen_const("WB_KEYBOARD_NUMPAD_LEFT",  "376")
    gen_const("WB_KEYBOARD_NUMPAD_RIGHT", "378")
    gen_const("WB_KEYBOARD_NUMPAD_HOME",  "375")
    gen_const("WB_KEYBOARD_NUMPAD_END",   "382")

    gen_const("WB_NO_FIELD",    "0")
    gen_const("WB_SF_BOOL",     "1")
    gen_const("WB_SF_INT32",    "2")
    gen_const("WB_SF_FLOAT",    "3")
    gen_const("WB_SF_VEC2F",    "4")
    gen_const("WB_SF_VEC3F",    "5")
    gen_const("WB_SF_ROTATION", "6")
    gen_const("WB_SF_COLOR",    "7")
    gen_const("WB_SF_STRING",   "8")
    gen_const("WB_SF_NODE",     "9")
    gen_const("WB_MF",         "16")
    gen_const("WB_MF_BOOL",    "17")
    gen_const("WB_MF_INT32",   "18")
    gen_const("WB_MF_FLOAT",   "19")
    gen_const("WB_MF_VEC2F",   "20")
    gen_const("WB_MF_VEC3F",   "21")
    gen_const("WB_MF_COLOR",   "23")
    gen_const("WB_MF_STRING",  "24")
    gen_const("WB_MF_NODE",    "25")

    gen_const("WB_EVENT_QUIT", "-1")
    gen_const("WB_EVENT_NO_EVENT", "0")
    gen_const("WB_EVENT_MOUSE_CLICK", "1")
    gen_const("WB_EVENT_MOUSE_MOVE", "2")
    gen_const("WB_EVENT_KEYBOARD", "4")
    gen_const("WB_EVENT_JOYSTICK_BUTTON", "8")
    gen_const("WB_EVENT_JOYSTICK_AXIS", "16")
    gen_const("WB_EVENT_JOYSTICK_POV", "32")

    # ANSI codes
    gen_const("ANSI_RESET", "strcat(27, '[0m')")
    gen_const("ANSI_BOLD", "strcat(27, '[1m')")
    gen_const("ANSI_UNDERLINE", "strcat(27, '[4m')")
    gen_const("ANSI_BLACK_FOREGROUND", "strcat(27, '[30m')")
    gen_const("ANSI_RED_FOREGROUND", "strcat(27, '[31m')")
    gen_const("ANSI_GREEN_FOREGROUND", "strcat(27, '[32m')")
    gen_const("ANSI_YELLOW_FOREGROUND", "strcat(27, '[33m')")
    gen_const("ANSI_BLUE_FOREGROUND", "strcat(27, '[34m')")
    gen_const("ANSI_MAGENTA_FOREGROUND", "strcat(27, '[35m')")
    gen_const("ANSI_CYAN_FOREGROUND", "strcat(27, '[36m')")
    gen_const("ANSI_WHITE_FOREGROUND", "strcat(27, '[37m')")
    gen_const("ANSI_BLACK_BACKGROUND", "strcat(27, '[40m')")
    gen_const("ANSI_RED_BACKGROUND", "strcat(27, '[41m')")
    gen_const("ANSI_GREEN_BACKGROUND", "strcat(27, '[42m')")
    gen_const("ANSI_YELLOW_BACKGROUND", "strcat(27, '[43m')")
    gen_const("ANSI_BLUE_BACKGROUND", "strcat(27, '[44m')")
    gen_const("ANSI_MAGENTA_BACKGROUND", "strcat(27, '[45m')")
    gen_const("ANSI_CYAN_BACKGROUND", "strcat(27, '[46m')")
    gen_const("ANSI_WHITE_BACKGROUND", "strcat(27, '[47m')")
    gen_const("ANSI_CLEAR_SCREEN", "strcat(27, '[2J')")

    # these lists are exact copy paste from the doc (without the commented lines and with addition of the experimental nodes)
    gen_consts_from_list(
        'WB_DISTANCE_SENSOR_GENERIC, WB_DISTANCE_SENSOR_INFRA_RED, WB_DISTANCE_SENSOR_SONAR, WB_DISTANCE_SENSOR_LASER')
    gen_consts_from_list('WB_GPS_LOCAL_COORDINATE, WB_GPS_WGS84_COORDINATE')
    gen_consts_from_list(
        'WB_MODE_SIMULATION, WB_MODE_CROSS_COMPILATION, WB_MODE_REMOTE_CONTROL')
    gen_consts_from_list("""
        WB_NODE_NO_NODE,
        WB_NODE_APPEARANCE, WB_NODE_BACKGROUND, WB_NODE_BILLBOARD, WB_NODE_BOX, WB_NODE_CAPSULE,
        WB_NODE_COLOR, WB_NODE_CONE, WB_NODE_COORDINATE,
        WB_NODE_CYLINDER, WB_NODE_DIRECTIONAL_LIGHT, WB_NODE_ELEVATION_GRID,
        WB_NODE_FOG, WB_NODE_GROUP, WB_NODE_IMAGE_TEXTURE, WB_NODE_INDEXED_FACE_SET,
        WB_NODE_INDEXED_LINE_SET, WB_NODE_MATERIAL, WB_NODE_MESH, WB_NODE_MUSCLE, WB_NODE_NORMAL,
        WB_NODE_PBR_APPEARANCE, WB_NODE_PLANE, WB_NODE_POINT_LIGHT, WB_NODE_POINT_SET, WB_NODE_SHAPE,
        WB_NODE_SPHERE, WB_NODE_SPOT_LIGHT, WB_NODE_TEXTURE_COORDINATE,
        WB_NODE_TEXTURE_TRANSFORM, WB_NODE_TRANSFORM, WB_NODE_VIEWPOINT,
        WB_NODE_ROBOT,
        WB_NODE_ACCELEROMETER, WB_NODE_BRAKE, WB_NODE_CAMERA, WB_NODE_COMPASS,
        WB_NODE_CONNECTOR, WB_NODE_DISPLAY, WB_NODE_DISTANCE_SENSOR, WB_NODE_EMITTER,
        WB_NODE_GPS, WB_NODE_GYRO, WB_NODE_INERTIAL_UNIT, WB_NODE_LED, WB_NODE_LIDAR,
        WB_NODE_LIGHT_SENSOR, WB_NODE_LINEAR_MOTOR, WB_NODE_PEN,
        WB_NODE_POSITION_SENSOR, WB_NODE_PROPELLER, WB_NODE_RADAR,
        WB_NODE_RANGE_FINDER, WB_NODE_RECEIVER, WB_NODE_ROTATIONAL_MOTOR,
        WB_NODE_SPEAKER, WB_NODE_TOUCH_SENSOR,
        WB_NODE_BALL_JOINT, WB_NODE_BALL_JOINT_PARAMETERS, WB_NODE_CHARGER,
        WB_NODE_CONTACT_PROPERTIES, WB_NODE_DAMPING, WB_NODE_FLUID,
        WB_NODE_FOCUS, WB_NODE_HINGE_JOINT, WB_NODE_HINGE_JOINT_PARAMETERS,
        WB_NODE_HINGE_2_JOINT, WB_NODE_IMMERSION_PROPERTIES, WB_NODE_JOINT_PARAMETERS,
        WB_NODE_LENS, WB_NODE_LENS_FLARE, WB_NODE_PHYSICS, WB_NODE_RECOGNITION,
        WB_NODE_SLIDER_JOINT, WB_NODE_SLOT, WB_NODE_SOLID, WB_NODE_SOLID_REFERENCE,
        WB_NODE_TRACK, WB_NODE_TRACK_WHEEL, WB_NODE_WORLD_INFO, WB_NODE_ZOOM,
        WB_NODE_MICROPHONE, WB_NODE_RADIO, WB_NODE_SKIN
        """)
    gen_consts_from_list(
        'WB_SUPERVISOR_SIMULATION_MODE_PAUSE, WB_SUPERVISOR_SIMULATION_MODE_REAL_TIME, WB_SUPERVISOR_SIMULATION_MODE_FAST')
    gen_consts_from_list('WB_ROTATIONAL, WB_LINEAR')
    gen_consts_from_list(
        'WB_TOUCH_SENSOR_BUMPER, WB_TOUCH_SENSOR_FORCE, WB_TOUCH_SENSOR_FORCE3D')
    if UPDATE:
        GITIGNOREFILE.close()


if __name__ == '__main__':
    main(sys.argv[1:])
