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

"""Convert Webots PROTO file from the RUB (x-Right, y-Up, z-Back) or similar to FLU (x-Forward, y-Left, z-Up)."""

import argparse
import numpy as np
import transforms3d
from webots_parser import WebotsParser


TO_FLU_FROM = {
    'RUB': transforms3d.axangles.axangle2mat(
        [-0.5773516025189619, 0.5773476025217157, 0.5773516025189619], -2.094405307179586),
    'LUF': transforms3d.axangles.axangle2mat(
        [0.577351, 0.57735, 0.57735], 2.0944)
}


def find(function, elements, default=None):
    return next((x for x in elements if function(x)), default)


def get_field(node, field_name, default=None):
    return find(lambda x: x['name'] == field_name, node['fields'], default)


def get_vector3(node, name='translation'):
    vector3_field = get_field(node, name)
    if not vector3_field:
        return [0, 0, 0]
    return [float(value) for value in vector3_field['value']]


def get_rotation(node):
    rotation_field = get_field(node, 'rotation')
    if not rotation_field:
        return [1, 0, 0, 0]
    return [float(value) for value in rotation_field['value']]


def set_rotation(node, value):
    rotation_field = get_field(node, 'rotation')
    if not rotation_field:
        # Ignore [0, 0, 0] arrays
        if all([float(v) < 1e-4 for v in value]):
            return

        node['fields'].insert(0,
                              {'name': 'rotation',
                               'value': value,
                               'type': 'SFRotation'}
                              )
        return
    rotation_field['value'] = value


def set_vector3(node, value, name='translation'):
    vector3_field = get_field(node, name)
    if not vector3_field:
        # Ignore [0, 0, 0] arrays
        if all([float(v) < 1e-4 for v in value]):
            return

        node['fields'].insert(0,
                              {'name': name,
                               'value': value,
                               'type': 'SFVec3f'}
                              )
        return
    vector3_field['value'] = value


def vector_to_string(vector, decimals):
    new_str = []
    for value in vector:
        if abs(value) > 1 / (10**decimals):
            new_str.append(str(round(value, decimals)))
        else:
            new_str.append('0')
    return new_str


def convert_pose(rotation_angle_axis, translation, z_offset, initial_orientation):
    assert len(rotation_angle_axis) == 4, f'Rotation {rotation_angle_axis} is not the correct length'
    assert len(translation) == 3, f'Translation {translation} is not the correct length'

    # Convert
    rotation = transforms3d.axangles.axangle2mat(rotation_angle_axis[:3], rotation_angle_axis[3])
    new_rotation = TO_FLU_FROM[initial_orientation] @ rotation
    new_rotation_axis, new_rotation_angle = transforms3d.axangles.mat2axangle(new_rotation)
    new_translation = TO_FLU_FROM[initial_orientation] @ np.array(translation)
    new_translation[2] += z_offset

    # Convert to string array
    new_rotation_str = vector_to_string(new_rotation_axis.tolist() + [new_rotation_angle], 4)
    new_translation_str = vector_to_string(new_translation, 3)

    return new_rotation_str, new_translation_str


def convert_physics(node, z_offset, initial_orientation):
    physics_field = get_field(node, 'physics')
    new_center_of_masses_str = []
    if physics_field:
        physics_node = physics_field['value']
        center_of_mass_field = get_field(physics_node, 'centerOfMass')
        if center_of_mass_field:
            for ceneter_of_mass_str in center_of_mass_field['value']:
                center_of_mass = [float(value) for value in ceneter_of_mass_str]
                new_center_of_mass = TO_FLU_FROM[initial_orientation] @ np.array(center_of_mass)
                new_center_of_mass[2] += z_offset
                new_center_of_mass_str = [f'{round(v, 5):.5}' for v in new_center_of_mass]
                new_center_of_masses_str.append(new_center_of_mass_str)
            center_of_mass_field['value'] = new_center_of_masses_str


def convert_nodes(nodes, z_offset, initial_orientation):
    for node in nodes:
        if 'Joint' in node['name']:
            joint_parameters_node = get_field(node, 'jointParameters')['value']

            anchor = get_vector3(joint_parameters_node, name='anchor')
            new_anchor = TO_FLU_FROM[initial_orientation] @ np.array(anchor)
            new_anchor[2] += z_offset
            new_anchor_str = [f'{round(v, 5):.5}' for v in new_anchor]
            set_vector3(joint_parameters_node, new_anchor_str, name='anchor')

            axis = get_vector3(joint_parameters_node, name='axis')
            new_axis = TO_FLU_FROM[initial_orientation] @ np.array(axis)
            new_axis_str = [f'{int(round(v))}' for v in new_axis]
            set_vector3(joint_parameters_node, new_axis_str, name='axis')

            endpoint_node = get_field(node, 'endPoint')['value']
            convert_nodes([endpoint_node], z_offset, initial_orientation)

        elif node['name'] == 'Group':
            children = get_field(node, 'children')
            if children and children['type'] != 'IS':
                convert_nodes(children['value'], z_offset, initial_orientation)
        elif node['name'] == 'Shape':
            # TODO: It would be better to convert it pixel by pixel
            current_node = node.copy()
            node.clear()
            node['name'] = 'Transform'
            node['fields'] = [
                {'name': 'children', 'type': 'MFNode', 'value': [current_node]}
            ]
            translation = get_vector3(node)
            rotation = get_rotation(node)
            new_rotation, new_translaton = convert_pose(rotation, translation, z_offset, initial_orientation)
            set_rotation(node, new_rotation)
            set_vector3(node, new_translaton)
        elif node['name'] == 'Slot':
            # Ignore
            pass
        else:
            # Handle `Solid` nodes
            translation = get_vector3(node)
            rotation = get_rotation(node)
            new_rotation, new_translaton = convert_pose(rotation, translation, z_offset, initial_orientation)
            set_rotation(node, new_rotation)
            set_vector3(node, new_translaton)

            convert_physics(node, z_offset, initial_orientation)


def convert_bounding_object(node, z_offset, initial_orientation):
    bounding_object = get_field(node, 'boundingObject')['value']
    if bounding_object:
        convert_nodes([bounding_object], z_offset, initial_orientation)


def main():
    parser = argparse.ArgumentParser(
        description='Convert a Webots PROTO file from the RUB or similar to FLU (x-Forward, y-Left, z-Up).'
    )
    parser.add_argument('--z-offset', dest='z_offset', type=float, default=0.0,
                        help='Change z-offset to match the ground level')
    parser.add_argument('proto_file', type=str, help='Path to the PROTO file')
    parser.add_argument('--initial-orientation', dest='initial_orientation',
                        choices=['RUB', 'LUF'], default='RUB', help='Initial PROTO orientation')
    args = parser.parse_args()

    proto = WebotsParser()
    proto.load(args.proto_file)

    # Find the Robot's children field
    # root > PROTO (`[0]['root']`) > Robot (`[0]`) > fields (`['fields']`)
    robot_node = proto.content['root'][0]['root'][0]

    # Convert robot's children
    robot_children = get_field(robot_node, 'children')['value']
    convert_nodes(robot_children, args.z_offset, args.initial_orientation)

    # Convert physics
    convert_physics(robot_node, args.z_offset, args.initial_orientation)

    # Convert bounding objects
    convert_bounding_object(robot_node, args.z_offset, args.initial_orientation)

    proto.save(args.proto_file)


if __name__ == '__main__':
    main()
