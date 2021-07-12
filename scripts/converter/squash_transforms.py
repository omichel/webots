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

"""Remove as many Transform nodes as possible, updating IndexedFaceSet nodes accordingly."""

import argparse
import numpy as np
import transforms3d
from webots_parser import WebotsParser


def get_translation(node):
    vector3_field = get_field(node, 'translation')
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
    value = [f'{round(float(v), 5):.5g}' for v in value]
    if not rotation_field:
        if all([float(v) < 1e-4 for v in value]):  # Ignore [0, 0, 0] arrays
            return
        node['fields'].insert(0, {'name': 'rotation', 'value': value, 'type': 'SFRotation'})
        return
    rotation_field['value'] = value


def set_translation(node, value):
    vector3_field = get_field(node, 'translation')
    value = [f'{round(float(v), 5):.5g}' for v in value]
    if not vector3_field:
        if all([float(v) < 1e-4 for v in value]):  # Ignore [0, 0, 0] arrays
            return
        node['fields'].insert(0, {'name': 'translation', 'value': value, 'type': 'SFVec3f'})
        return
    vector3_field['value'] = value


def squash_points(points, transform):
    rotation_angle_axis = get_rotation(transform)
    rotation = transforms3d.axangles.axangle2mat(rotation_angle_axis[:3], -rotation_angle_axis[3])
    translation = np.array(get_translation(transform))

    n = len(points)
    n3 = int(n / 3)
    if n3 != n / 3:
        print('Error: IndexedFaceSet.coords.Coordinates.point doesn\'t contain 3D coordinates.')
        return
    for i in range(n3):
        i3 = 3 * i
        point = np.array([float(points[i3]), float(points[i3 + 1]), float(points[i3 + 2])])
        point = point @ rotation + translation
        for j in range(3):
            points[i3 + j] = f'{round(float(point[j]), 5):.5g}'
    set_translation(transform, [0, 0, 0])
    set_rotation(transform, [0, 0, 1, 0])


def squash_indexed_face_set(node, transform):
    print(f'Squashing {node["name"]}')
    coord = get_field(node, 'coord')
    if coord is None:
        return
    point = get_field(coord['value'], 'point')
    squash_points(point['value'][0], transform)


def squash_shape(node, transform):
    print(f'Squashing {node["name"]}')
    geometry = get_field(node, 'geometry')
    if geometry is None:
        return
    if geometry['value']['name'] == 'IndexedFaceSet':
        squash_indexed_face_set(geometry['value'], transform)


def squash_transform(node, transform=None):
    if node['name'] == 'Shape' and transform is not None:
        squash_shape(node, transform)
    else:
        print('Skiping ' + node['name'])
    children = get_field(node, 'children')
    if children:
        squash_transforms(children['value'], node if node['name'] == 'Transform' else None)


def squash_transforms(nodes, transform=None):
    for node in nodes:
        squash_transform(node, transform)


def find(function, elements, default=None):
    return next((x for x in elements if function(x)), default)


def get_field(node, field_name, default=None):
    return find(lambda x: x['name'] == field_name, node['fields'], default)


def main():
    parser = argparse.ArgumentParser(
        description='Remove as many Transform nodes as possible'
    )
    parser.add_argument('proto_file', type=str, help='Path to the PROTO file')
    args = parser.parse_args()

    proto = WebotsParser()
    proto.load(args.proto_file)

    # Find the root children field
    # root > PROTO (`[0]['root']`) > Solid (`[0]`) > fields (`['fields']`)
    root = proto.content['root'][0]['root'][0]

    # Convert robot's children
    children = get_field(root, 'children')['value']
    squash_transforms(children)

    proto.save(args.proto_file)


if __name__ == '__main__':
    main()
