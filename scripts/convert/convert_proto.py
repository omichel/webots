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

'''Convert world file from NUE to ENU coordinate system.'''

import math
import numpy
import sys


def translation(value):
    return [value[2], value[0], value[1]]


def axis_angle_to_quaternion(axis, theta):
    axis = numpy.array(axis) / numpy.linalg.norm(axis)
    return numpy.append([numpy.cos(theta/2)], numpy.sin(theta/2) * axis)


def quaternion_multiply(q1, q2):
    q3 = numpy.copy(q1)
    q3[0] = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3]
    q3[1] = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2]
    q3[2] = q1[0]*q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1]
    q3[3] = q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0]
    return q3


def normalize(v, tolerance=0.00001):
    mag2 = sum(n * n for n in v)
    if abs(mag2 - 1.0) > tolerance:
        mag = math.sqrt(mag2)
        v = tuple(n / mag for n in v)
    return v


def quaternion_to_axis_angle(q):
    w, v = q[0], q[1:]
    theta = math.acos(w) * 2.0
    return normalize(v), theta


def rotation(value):
    q0 = axis_angle_to_quaternion([value[2], value[0], value[1]], value[3])
    qr = quaternion_multiply(q0, [0.5, 0.5, 0.5, 0.5])
    (v, theta) = quaternion_to_axis_angle(qr)
    return [v[0], v[1], v[2], theta]


input_filename = sys.argv[1]

output_filename = input_filename.replace('.proto', '-FLU.proto')


def swap_translation(line, count):
    if '%{=' in line:
        print('line ' + str(count) + ': %{ case not handled')
        return
    if '#' in line:
        print('line ' + str(count) + ': # case not handled')
        return
    beginning_of_field_name = len(line) - len(line.lstrip())
    lenght_of_field_name = len(line[beginning_of_field_name:].split(' ', 1)[0])
    beginning_of_field_value = beginning_of_field_name + lenght_of_field_name + 1
    output = line[0:beginning_of_field_value]
    fields = line[beginning_of_field_value:].rstrip().split(' ')
    swap = fields[2]
    fields[2] = fields[1]
    fields[1] = fields[0]
    fields[0] = swap
    output += ' '.join(fields) + '\n'
    return output


translation_names = ['anchor', 'axis', 'position', 'translation']
indented_translation_names = []
for name in translation_names:
    indented_translation_names.append(' ' * 8 + name + ' ')

with open(output_filename, 'w', newline='\n') as output:
    with open(input_filename, 'r') as input:
        count = 0
        for line in input:
            count += 1
            if line.startswith('PROTO '):
                words = line.split(' ')
                line = 'PROTO ' + words[1]
                # line += '-FLU'
                line += ' [\n'
            if line.startswith('  field SFRotation '):
                line = line.replace('0 1 0 0', '0 0 1 0')
            elif ' IS ' not in line and any(line.strip().startswith(s) for s in translation_names):
                line = swap_translation(line, count)
            elif ' IS ' not in line and line.strip().startswith('rotation'):
                words = line.strip().split(' ')
                r = rotation([float(words[1]), float(words[2]), float(words[3]), float(words[4])])
                line = ' ' * line.find('rotation') + 'rotation '
                line += str(r[0]) + ' ' + str(r[1]) + ' ' + str(r[2]) + ' ' + str(r[3]) + '\n'
            elif line.strip().startswith('coord Coordinate {'):  # enter Coordinate point swap
                output.write(line)
                values = []
                offset = 0
                for line in input:
                    o = line.find('point [')
                    if o != -1:
                        offset = o
                    if line.strip() == '}':
                        break
                    words = line.strip().split(' ')
                    for word in words:
                        if word.endswith(','):
                            word = word[:-1]
                        try:
                            f = float(word)
                            values.append(word)
                        except ValueError:
                            pass
                line = ' ' * offset + 'point [\n' + ' ' * (offset + 1)
                max = len(values)
                if max % 3 != 0:
                    print('Error: coordinates number ({}) is not divisible by 3'.format(max))
                i = 0
                while i < max:
                    line += ' ' + values[i + 2] + ' ' + values[i] + ' ' + values[i + 1]
                    i += 3
                line += '\n' + ' ' * offset + ']\n' + ' ' * (offset - 2) + '}\n'
            output.write(line)
