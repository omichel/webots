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

"""Contains the Barrier class."""

from webots_objects.webots_object import WebotsObject
from webots_objects.road import Road

from settings import Settings
from osm_objects import OSMCoord
from utils.misc_utils import extract_float_from_string, length2D


RED_BRICK_TEXTURE = 'https://raw.githubusercontent.com/cyberbotics/webots/R2021b/projects/default/worlds/textures/red_brick_wall.jpg'  # noqa: E501


class Barrier(WebotsObject):
    """Barrier class representing a barrier segment."""

    list = []
    fenceNameIndex = 1
    wallNameIndex = 1

    def __init__(self):
        """Initialize the barrier."""
        self.OSMID = 0
        self.type = ""
        self.ref = 0
        self.height = 1.5
        self.width = 0.2

    def length(self):
        """Return the length of a barrier."""
        length = 0
        for index in range(len(self.ref)):
            if index > 0:
                x = OSMCoord.coordDictionnary[self.ref[index]].x - OSMCoord.coordDictionnary[self.ref[index - 1]].x
                z = OSMCoord.coordDictionnary[self.ref[index]].z - OSMCoord.coordDictionnary[self.ref[index - 1]].z
                length = length + length2D(x, z)
        return length

    @staticmethod
    def add_to_list(osmid, tags, ref):
        """Add a new barrier to the list of barriers."""
        if 'barrier' in tags:
            settingsSection = Settings.get_section('barrier', tags['barrier'])
            if settingsSection is None:
                return
            if Settings.has_option(settingsSection, 'ignore') and Settings.get(settingsSection, 'ignore') == 'TRUE':
                return
            barrier = Barrier()
            barrier.OSMID = osmid
            barrier.type = tags['barrier']
            barrier.ref = ref
            if 'height' in tags:
                barrier.height = extract_float_from_string(tags['height'])
            elif Settings.has_option(settingsSection, 'height'):
                barrier.height = Settings.getfloat(settingsSection, 'height')
            if 'width' in tags:
                barrier.width = extract_float_from_string(tags['width'])
            elif Settings.has_option(settingsSection, 'width'):
                barrier.width = Settings.getfloat(settingsSection, 'width')
            if WebotsObject.enable3D:
                barrier.ref = Barrier.add_intermediate_point_where_needed(barrier.ref)
            Barrier.list.append(barrier)

    @classmethod
    def export(cls, file):
        """Export all the barriers from the barriers list."""
        for barrier in Barrier.list[:]:
            if not barrier.ref:
                Barrier.list.remove(barrier)
                continue

            if WebotsObject.removalRadius > 0.0:
                # Check that the barrier is inside the scope of a road,
                # otherwise, remove it from the barrier list.
                if not Road.are_coords_close_to_some_road_waypoint(barrier.ref):
                    Barrier.list.remove(barrier)
                    continue

            if barrier.type == 'fence':
                file.write('Fence {\n')
                file.write('  name "fence(%d)"\n' % Barrier.fenceNameIndex)
                Barrier.fenceNameIndex += 1
                file.write('  height %.2f\n' % barrier.height)
                file.write('  path [\n')
                for ref in barrier.ref:
                    if ref in OSMCoord.coordDictionnary:
                        file.write('    %.2f %.2f %.2f,\n' % (OSMCoord.coordDictionnary[ref].x,
                                                              OSMCoord.coordDictionnary[ref].y,
                                                              OSMCoord.coordDictionnary[ref].z))
                    else:
                        print('Warning: node ' + str(ref) + ' not referenced.')
                file.write('  ]\n')
                file.write('  splineSubdivision 0\n')
                file.write('}\n')
            elif barrier.type == 'wall':
                file.write('Solid {\n')
                file.write('  translation 0 %.2f 0\n' % (barrier.height / 2))
                file.write('  children [\n')
                file.write('    DEF SHAPE Shape {\n')
                file.write('      appearance PBRAppearance {\n')
                file.write('        baseColorMap ImageTexture {\n')
                file.write(f'          url [ "{RED_BRICK_TEXTURE}" ]\n')
                file.write('        }\n')
                file.write('        roughness 1\n')
                file.write('        metalness 0\n')
                file.write('        textureTransform TextureTransform {\n')
                file.write('          scale %.1f %.1f\n' % (barrier.length(), barrier.height))
                file.write('        }\n')
                file.write('      }\n')
                file.write('      geometry Extrusion {\n')
                file.write('        crossSection [\n')
                file.write('          %.2f %.2f\n' % ((barrier.width / 2), (barrier.height / 2)))
                file.write('          %.2f %.2f\n' % ((barrier.width / 2), (-barrier.height / 2)))
                file.write('          %.2f %.2f\n' % ((-barrier.width / 2), (-barrier.height / 2)))
                file.write('          %.2f %.2f\n' % ((-barrier.width / 2), (barrier.height / 2)))
                file.write('          %.2f %.2f\n' % ((barrier.width / 2), (barrier.height / 2)))
                file.write('        ]\n')
                file.write('        spine [\n')
                for ref in barrier.ref:
                    if ref in OSMCoord.coordDictionnary:
                        file.write('          %.2f %.2f %.2f,\n' % (OSMCoord.coordDictionnary[ref].x,
                                                                    OSMCoord.coordDictionnary[ref].y,
                                                                    OSMCoord.coordDictionnary[ref].z))
                    else:
                        print('Warning: node ' + str(ref) + ' not referenced.')
                file.write('        ]\n')
                file.write('        splineSubdivision 0\n')
                file.write('      }\n')
                file.write('    }\n')
                file.write('  ]\n')
                file.write('  name "wall(%d)"\n' % Barrier.wallNameIndex)
                Barrier.wallNameIndex += 1
                file.write('  boundingObject USE SHAPE\n')
                file.write('}\n')
