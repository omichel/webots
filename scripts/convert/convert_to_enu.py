#!/usr/bin/env python3

# Copyright 1996-2020 Cyberbotics Ltd.
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

'''Convert world file from R2020a to R2020b switching from NUE to ENU coordinate system.'''

import math
import sys

from transforms3d import quaternions

from webots_parser import WebotsParser
from converted_protos import converted_protos


def translation(value):
    return [value[2], value[0], value[1]]


def rotation_axis(value, node_name):
    rotation = value[3] + math.pi if node_name == 'E-puck' else value[3]
    return [value[2], value[0], value[1], rotation]


def rotation(value, r):
    q0 = quaternions.axangle2quat([float(value[0]), float(value[1]), float(value[2])], float(value[3]))
    q1 = quaternions.axangle2quat([r[0], r[1], r[2]], r[3])
    qr = quaternions.qmult(q0, q1)
    v, theta = quaternions.quat2axangle(qr)
    return [WebotsParser.str(v[0]), WebotsParser.str(v[1]), WebotsParser.str(v[2]), WebotsParser.str(theta)]


filename = sys.argv[1]
world = WebotsParser()
world.load(filename)

transform_nodes = ['Tranform', 'Solid', 'Robot']

for node in world.content['root']:
    if node['name'] == 'WorldInfo':
        for field in node['fields']:
            if field['name'] == 'gravity':
                field['value'] = -field['value'][1]
                field['type'] = 'SFFloat'
    elif node['name'] in converted_protos:
        for field in node['fields']:
            if field['name'] in ['translation', 'location', 'direction']:
                field['value'] = translation(field['value'])
            elif field['name'] in ['rotation', 'orientation']:
                field['value'] = rotation_axis(field['value'], node['name'])
    else:
        default_direction = True
        default_position = True
        default_rotation = True
        default_translation = True
        for field in node['fields']:
            if field['name'] in ['translation', 'position', 'location', 'direction']:
                field['value'] = translation(field['value'])
                if field['name'] == 'translation':
                    default_translation = False
                if field['name'] == 'direction':
                    default_direction = False
                elif field['name'] == 'position':  # Viewpoint
                    default_position = False
            elif field['name'] in ['rotation', 'orientation']:
                field['value'] = rotation(field['value'])
                if field['name'] == 'rotation':
                    default_rotation = False

        if node['name'] in ['DirectionalLight', 'SpotLight'] and default_direction:  # fix default direction for lights
            node['fields'].insert(0, {'name': 'direction', 'type': 'SFVec3f', 'value': [0, -1, 0]})
        elif node['name'] == 'Viewpoint' and default_position:  # fix default position for Viewpoint
            node['fields'].insert(0, {'name': 'position', 'type': 'SFVec3f', 'value': [0, 10, 0]})
        elif (node['name'] in ['Robot', 'Solid', 'Transform'] or not default_translation) and default_rotation:
            node['fields'].insert(0 if default_translation else 1,
                                  {'name': 'rotation', 'type': 'SFRotation', 'value': rotation([0, 1, 0, 0])})

world.save(filename[:-4] + '_enu.wbt')
