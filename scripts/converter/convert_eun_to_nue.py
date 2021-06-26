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

"""Convert R2020b world file from the EUN (northDirection = '0 0 1') to the NUE coordinate system."""

import math
import sys

from transforms3d import quaternions

from webots_parser import WebotsParser


def rotation(value, r):
    q0 = quaternions.axangle2quat([float(value[0]), float(value[1]), float(value[2])], float(value[3]))
    q1 = quaternions.axangle2quat([r[0], r[1], r[2]], r[3])
    qr = quaternions.qmult(q1, q0)
    v, theta = quaternions.quat2axangle(qr)
    return [WebotsParser.str(v[0]), WebotsParser.str(v[1]), WebotsParser.str(v[2]), WebotsParser.str(theta)]


def convert_to_nue(filename):
    world = WebotsParser()
    world.load(filename)

    for node in world.content['root']:
        if node['name'] == 'WorldInfo':
            is_nue = False
            for field in node['fields']:
                if field['name'] == 'northDirection':
                    assert field['value'] == ['0', '0', '1']
                    # remove the 'northDirection 0 0 1'
                    del node['fields'][node['fields'].index(field)]
                if field['name'] == 'coordinateSystem':
                    field['value'] = 'NUE'
                    is_nue = True
            if not is_nue:
                node['fields'].append({'name': 'coordinateSystem',
                                       'value': 'NUE',
                                       'type': 'SFString'})
        elif node['name'] == 'Viewpoint':
            print('Rotating', node['name'])
            for field in node['fields']:
                if field['name'] in ['orientation']:
                    field['value'] = rotation(field['value'], [0, 1, 0, 0.5 * math.pi])
                elif field['name'] in ['position']:
                    field['value'] = [field['value'][2], field['value'][1], str(-float(field['value'][0]))]
        elif node['name'] not in ['TexturedBackground', 'TexturedBackgroundLight', 'PointLight']:
            print('Rotating', node['name'])
            rotation_found = False
            for field in node['fields']:
                if field['name'] in ['rotation']:
                    rotation_found = True
                    field['value'] = rotation(field['value'], [0, 1, 0, 0.5 * math.pi])
                elif field['name'] in ['translation']:
                    field['value'] = [field['value'][2], field['value'][1], str(-float(field['value'][0]))]
            if not rotation_found:
                node['fields'].append({'name': 'rotation',
                                       'value': ['0', '1', '0', str(0.5 * math.pi)],
                                       'type': 'SFRotation'})
    world.save(filename)


if __name__ == "__main__":
    # execute only if run as a script
    for filename in sys.argv:
        if not filename.endswith('.wbt'):
            continue
        print(filename)
        convert_to_nue(filename)
