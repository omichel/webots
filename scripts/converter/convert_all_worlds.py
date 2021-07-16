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

"""Convert world files from R2020a to R2020b with either NUE or ENU coordinate system."""

# can be reverted with "git checkout -- *.wbt" at root level


import glob
import os
from convert_to_nue import convert_to_nue

WEBOTS_HOME = os.environ['WEBOTS_HOME']
nue_world_files = ['tests/api/worlds/*.wbt',
                   'tests/default/worlds/*.wbt',
                   'tests/manual_tests/worlds/*.wbt',
                   'tests/physics/worlds/*.wbt',
                   'tests/protos/worlds/*.wbt',
                   'tests/rendering/worlds/*.wbt',
                   'projects/humans/*/worlds/*.wbt',
                   'projects/languages/*/worlds/*.wbt',
                   'projects/robots/*/worlds/*.wbt',
                   'projects/robots/*/*/worlds/*.wbt',
                   'projects/samples/*/worlds/*.wbt',
                   'projects/samples/*/*/worlds/*.wbt',
                   'projects/vehicles/worlds/*.wbt',
                   'scripts/icon_studio/worlds/icon_studio.wbt'
                   ]
skip_world_files = []
skipped_world_files = []
for file in skip_world_files:
    skipped_world_files.append(os.path.join(WEBOTS_HOME, file.replace('/', os.sep)))
for file in nue_world_files:
    for f in glob.glob(os.path.join(WEBOTS_HOME, file.replace('/', os.sep))):
        if f in skipped_world_files:
            print('Skipping', f)
            continue
        print('Converting', f)
        convert_to_nue(f)
