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

'''List of protos converted to the FLU orientation.'''

_rub2flu = [0.577350269189626, -0.577350269189626, -0.577350269189626, -2.094395102393196]
_luf2flu = [0.577350269189626, 0.577350269189626, 0.577350269189626, -2.094395102393196]
_other = [0, 0.7071067811865476, 0.7071067811865476, 3.1415926535897932]

converted_protos = {'Book': _luf2flu,
                    'BunchOfSunFlowers': _luf2flu,
                    'CardboardBox': _luf2flu,
                    'CircleArena': _rub2flu,
                    'E-puck': _rub2flu,
                    'Floor': _rub2flu,
                    'Pioneer3dx': _rub2flu,
                    'RectangleArena': _rub2flu,
                    'TurtleBot3Burger': _rub2flu,
                    'WoodenBox': _rub2flu}
