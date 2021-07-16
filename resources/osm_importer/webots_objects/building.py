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

"""Contains the Building class."""

from webots_objects.webots_object import WebotsObject
from webots_objects.road import Road

from utils.misc_utils import extract_float_from_string
from settings import Settings
from osm_objects import OSMCoord
from webcolors import name_to_rgb

import random


class Building(WebotsObject):
    """Building class representing one building."""

    list = []
    nameIndex = 1
    nameList = []
    wallTypes = ["glass building", "classic building", "orange building", "gray glass building", "blue glass building",
                 "arcade-style building", "transparent highrise", "windowed building", "old brick building",
                 "red and white building", "construction building", "stone brick",
                 "stone wall", "glass highrise", "old house", "old building", "highrise", "brick building",
                 "residential building", "old office building", "factory building", "tall house", "office building",
                 "concrete building"]
    coloredWallTypes = ["old house", "brick building", "factory building", "tall house", "office building", "concrete building"]
    coloredRoofTypes = ["bitumen", "tiled"]
    importedWallTypes = {
        "wood": "tall house",
        "old house": "brick building",
        "plaster": "plaster",
        "cement_block": "factory building",
        "concrete": "concrete building",
        "glass": "transparent highrise",
        "mirror": "transparent highrise"
    }
    importedRoofTypes = {
        "tile": "tiled",
        "roof_tiles": "tiled",
        "concrete": "bitumen",
        "slate": "slate",
        "asbestos": "sheet metal",
        "metal": "sheet metal"
    }
    importedRoofShapes = {
        "pyramidal": "pyramidal roof",
        "flat": "flat roof",
        "gabled": "gabled roof",
        "hipped": "hipped roof"
    }

    def __init__(self):
        """Initialize the building."""
        self.OSMID = 0
        self.type = ""
        self.material = ""
        self.roofMaterial = ""
        self.color = ""
        self.roofColor = ""
        self.ref = []
        self.name = ""
        self.number = ""
        self.layer = 0.0
        self.levels = -1
        self.minLevel = None
        self.height = -1
        self.minHeight = -1
        self.roofHeight = -1
        self.roofShape = ""

    @staticmethod
    def add_to_list(osmid, tags, ref):
        """Add a new building to the list of buildings."""
        if 'building' in tags or 'building:part' in tags:
            building = Building()
            building.OSMID = osmid
            if 'building' in tags:
                building.type = tags['building']
            if 'building:material' in tags:
                building.material = tags['building:material']
            if 'roof:material' in tags:
                building.roofMaterial = tags['roof:material']
            elif 'material' in tags:
                building.material = tags['material']
            if 'building:colour' in tags:
                if tags['building:colour'].startswith("#"):
                    building.color = int(tags['building:colour'][1:], 16)
                    building.red = float((building.color & 0xFF0000) >> 16) / 255.0
                    building.green = float((building.color & 0x00FF00) >> 8) / 255.0
                    building.blue = float(building.color & 0x0000FF) / 255.0
                else:
                    try:
                        building.color = tags['building:colour']
                        red, green, blue = name_to_rgb(building.color, spec='css3')
                        building.red = float(red) / 255.0
                        building.green = float(green) / 255.0
                        building.blue = float(blue) / 255.0
                    except:
                        building.color = ""
            if 'roof:colour' in tags:
                if tags['roof:colour'].startswith("#"):
                    building.roofColor = int(tags['roof:colour'][1:], 16)
                    building.roofRed = float((building.roofColor & 0xFF0000) >> 16) / 255.0
                    building.roofGreen = float((building.roofColor & 0x00FF00) >> 8) / 255.0
                    building.roofBlue = float(building.roofColor & 0x0000FF) / 255.0
                else:
                    try:
                        building.roofColor = tags['roof:colour']
                        roofRed, roofGreen, roofBlue = name_to_rgb(building.roofColor, spec='css3')
                        building.roofRed = float(roofRed) / 255.0
                        building.roofGreen = float(roofGreen) / 255.0
                        building.roofBlue = float(roofBlue) / 255.0
                    except:
                        building.roofColor = ""
            if 'name' in tags:
                building.name = tags['name']
            elif 'ref' in tags:
                building.name = tags['ref']
            if 'roof:height' in tags:
                building.roofHeight = extract_float_from_string(tags['roof:height'])
            if 'min_height' in tags:
                building.minHeight = extract_float_from_string(tags['min_height'])
            if 'height' in tags:
                if building.roofHeight > 0:
                    building.height = extract_float_from_string(tags['height']) - building.roofHeight
                else:
                    building.height = extract_float_from_string(tags['height'])
            if 'roof:shape' in tags:
                building.roofShape = tags['roof:shape']
            if 'layer' in tags:
                building.layer = extract_float_from_string(tags['layer'])
            if 'building:levels' in tags:
                building.levels = int(extract_float_from_string(tags['building:levels']))
            if 'building:min_level' in tags:
                building.minLevel = int(extract_float_from_string(tags['building:min_level']))
            building.ref = ref
            if building.ref and building.ref[0] == building.ref[-1]:
                # often last and first reference are the same => this is useless for us
                del building.ref[-1]
            Building.list.append(building)

    @classmethod
    def export(cls, file):
        """Export all the buildings from the buildings list."""
        for building in Building.list[:]:
            settingsSection = Settings.get_section('building', building.type)
            if settingsSection is None or len(building.ref) < 1:
                Building.list.remove(building)
                continue

            if WebotsObject.removalRadius > 0.0:
                # Check that the building is inside the scope of a road,
                # otherwise, remove it from the building list.
                if not Road.are_coords_close_to_some_road_waypoint(building.ref):
                    Building.list.remove(building)
                    continue

            if building.height > 0 and building.levels < 0:
                building.levels = int(building.height / 3.0)

            file.write("SimpleBuilding {\n")
            # set the height of the building to be the height of the minimum corner
            if WebotsObject.enable3D:
                height = float('inf')
                for ref in building.ref:
                    if ref in OSMCoord.coordDictionnary and OSMCoord.coordDictionnary[ref].y < height:
                        height = OSMCoord.coordDictionnary[ref].y
                if height == float('inf'):
                    height = 0
                height = height + building.layer * WebotsObject.layerHeight
                file.write("  translation %.2lf %.2lf %.2lf\n" %
                           (OSMCoord.coordDictionnary[building.ref[0]].x, height, OSMCoord.coordDictionnary[building.ref[0]].z))
            else:
                file.write("  translation %.2lf %.2lf %.2lf\n" %
                           (OSMCoord.coordDictionnary[building.ref[0]].x, building.layer * WebotsObject.layerHeight,
                            OSMCoord.coordDictionnary[building.ref[0]].z))

            name = building.name
            if name != '' and building.number != '':
                name += ' ' + building.number
            if name == '':
                name = "building(%d)" % Building.nameIndex
                Building.nameIndex += 1
            else:
                newName = name
                i = 1
                while newName in Building.nameList:
                    newName = name + '(%d)' % i
                    i += 1
                Building.nameList.append(newName)
                name = newName
            name = name.replace('"', '')
            file.write('  name "%s"\n' % name)

            file.write("  corners [\n")
            for ref in building.ref:
                if ref in OSMCoord.coordDictionnary:
                    file.write("    %.2f %.2f,\n" % (OSMCoord.coordDictionnary[ref].x -
                                                     OSMCoord.coordDictionnary[building.ref[0]].x,
                                                     OSMCoord.coordDictionnary[ref].z -
                                                     OSMCoord.coordDictionnary[building.ref[0]].z))
                else:
                    print("Warning: node " + str(ref) + " not referenced.")
            file.write("  ]\n")
            # Set the roofShape
            if building.roofShape in building.importedRoofShapes:
                file.write('  roofShape "%s"\n' % building.importedRoofShapes[building.roofShape])
            elif Settings.has_option(settingsSection, 'roofShape'):
                file.write("  roofShape " + Settings.get(settingsSection, 'roofShape') + "\n")
            else:
                if len(building.ref) <= 4:
                    file.write("  roofShape \"pyramidal roof\"\n")
                else:
                    file.write("  roofShape \"flat roof\"\n")
            # Set the wallType
            if (building.material in building.importedWallTypes and
                    building.importedWallTypes[building.material] in building.coloredWallTypes and building.color != ""):
                file.write("  wallType \"%s\"\n" % building.importedWallTypes[building.material])
            elif building.color != "":
                file.write('  wallType "%s"\n' % random.choice(building.coloredWallTypes))
            elif building.material in building.importedWallTypes:
                file.write("  wallType \"%s\"\n" % building.importedWallTypes[building.material])
            elif Settings.has_option(settingsSection, 'wallType'):
                file.write("  wallType " + Settings.get(settingsSection, 'wallType') + "\n")
            else:
                file.write('  wallType "%s"\n' % random.choice(building.wallTypes))
            # Set the roofType
            if (building.roofMaterial in building.coloredRoofTypes and
                    building.importedRoofTypes[building.roofMaterial] in building.coloredRoofTypes and
                    building.roofColor != ""):
                file.write('  roofType "%s"\n' % building.importedRoofTypes[building.roofMaterial])
            elif building.roofColor != "":
                file.write(' roofType "%s"\n' % random.choice(building.coloredRoofTypes))
            elif building.roofMaterial in building.importedRoofTypes:
                file.write('  roofType "%s"\n' % building.importedRoofTypes[building.roofMaterial])
            elif Settings.has_option(settingsSection, 'roofType'):
                file.write("  roofType " + Settings.get(settingsSection, 'roofType') + "\n")
            # Set the wallColor
            if building.color != '':
                file.write("  wallColor [ %.2f %.2f %.2f ]\n" % (building.red, building.green, building.blue))
            # Set the roofColor
            if building.roofColor != '':
                file.write("  wallColor [ %.2f %.2f %.2f ]\n" % (building.roofRed, building.roofGreen, building.roofBlue))
            # Set the roofHeight
            if building.roofHeight > 0:
                file.write("  roofHeight %.2f\n" % building.roofHeight)
            elif Settings.has_option(settingsSection, 'roofHeight'):
                file.write("  roofHeight " + Settings.getfloat(settingsSection, 'roofHeight') + "\n")
            # Set the levels
            if building.levels > 0:
                file.write("  floorNumber " + str(building.levels) + "\n")
            elif Settings.has_option(settingsSection, 'floorNumber'):
                building.levels = Settings.getfloat(settingsSection, 'floorNumber')
                file.write("  floorNumber " + Settings.get(settingsSection, 'floorNumber') + "\n")
            # Set the startingFloor
            if building.minHeight > 0 and building.levels > 0:
                file.write("  startingFloor %d\n" % int(building.minHeight / building.height * building.levels))
                file.write("  bottom TRUE\n")
            elif building.minLevel is not None:
                file.write("  startingFloor " + str(building.minLevel) + "\n")
                file.write("  bottom TRUE\n")
            elif building.layer > 0:
                file.write("  bottom TRUE\n")
            # Set the floorHeight
            if building.height > 0:
                if building.levels > 0:
                    file.write("  floorHeight %.2f\n" % (building.height / building.levels))
                else:
                    file.write("  floorHeight %.2f\n" % (building.height / 3))
            elif Settings.has_option(settingsSection, 'floorHeight'):
                file.write("  floorHeight " + Settings.get(settingsSection, 'floorHeight') + "\n")
            file.write("}\n")
