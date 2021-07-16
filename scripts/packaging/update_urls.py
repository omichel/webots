#!/usr/bin/env python

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

"""Replace the webots:// URLs with https://raw.githubusercontent.com/cyberbotics/webots/<version>/
   in world, proto and controller files."""


import os
import sys
from pathlib import Path


def replace_url(file, tag):
    with open(file, 'r') as fd:
        content = fd.read()
    content = content.replace('webots://', 'https://raw.githubusercontent.com/cyberbotics/webots/' + tag + '/')
    with open(file, 'w', newline='\n') as fd:
        fd.write(content)


if 'WEBOTS_HOME' in os.environ:
    WEBOTS_HOME = os.environ['WEBOTS_HOME']
else:
    WEBOTS_HOME = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

if len(sys.argv) != 2:
    sys.exit('Missing argument: commit sha or tag.')
else:
    tag = sys.argv[1]
paths = []
paths.extend(Path(WEBOTS_HOME + '/projects').rglob('*.proto'))
paths.extend(Path(WEBOTS_HOME + '/projects').rglob('*.wbt'))
paths.extend(Path(WEBOTS_HOME + '/tests').rglob('*.wbt'))

with open(WEBOTS_HOME + '/scripts/packaging/controllers_with_urls.txt', 'r') as files:
    paths.extend(list(map(lambda path: WEBOTS_HOME + path, files.read().splitlines())))

for path in paths:
    replace_url(path, tag)
