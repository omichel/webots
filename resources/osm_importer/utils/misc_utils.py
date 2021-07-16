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

"""This module gather various utility functions."""

import math
import re
from projection import Projection


GRASS_TEXTURE = 'https://raw.githubusercontent.com/cyberbotics/webots/R2021b/projects/default/worlds/textures/grass.jpg'


def get_world_size(minlat, minlon, maxlat, maxlon):
    """Return the world size in X-Z coordinates."""
    x1, z1 = Projection.project(minlon, minlat)
    x2, z2 = Projection.project(maxlon, maxlat)
    xSize = math.fabs(x2 - x1)
    zSize = math.fabs(z2 - z1)
    return (xSize, zSize)


def print_header(file, minlat, minlon, maxlat, maxlon, elevation=None):
    """Print the 'WorldInfo', 'Viewpoint', 'TexturedBackground', 'TexturedBackgroundLight' and 'Floor' nodes."""
    xSize, zSize = get_world_size(minlat=minlat, minlon=minlon, maxlat=maxlat, maxlon=maxlon)
    file.write("#VRML_SIM R2021c utf8\n")
    file.write("WorldInfo {\n")
    file.write("  info [\n")
    file.write("    \"World generated using the Open Street Map to Webots importer\"\n")
    file.write("    \"Author: David Mansolino <david.mansolino@epfl.ch>\"\n")
    file.write("  ]\n")
    file.write("  coordinateSystem \"NUE\"\n")
    longitude = (float(maxlon) + float(minlon)) / 2
    latitude = (float(maxlat) + float(minlat)) / 2
    x, z = Projection.project(longitude, latitude)
    height = 0
    if elevation is not None:
        height = elevation.interpolate_height(x, z)
    file.write("  gpsCoordinateSystem \"WGS84\"\n")
    file.write("  gpsReference " + str(latitude) + " " + str(longitude) + " " + str(height) + "\n")
    file.write("  lineScale " + str(round(max(xSize, zSize) / 200.0)) + "\n")
    file.write("}\n")
    file.write("Viewpoint {\n")
    file.write("  orientation 0.305 0.902 0.305 4.609\n")
    position = round(xSize * math.cos(0.785) * 1.5 + zSize * math.cos(0.785) * 1.5)
    file.write("  position " + str(-position * 1.25) + " " + str(position) + " 0\n")
    file.write("  near 3\n")
    file.write("}\n")
    file.write("TexturedBackground {\n")
    file.write("}\n")
    file.write("TexturedBackgroundLight {\n")
    file.write("}\n")
    if elevation is None:
        file.write("Floor {\n")
        file.write("  translation 0 -0.02 0\n")
        file.write("  size " + str(round(1.5 * xSize)) + " " + str(round(1.5 * zSize)) + "\n")
        file.write("  appearance PBRAppearance {\n")
        file.write("    baseColorMap ImageTexture {\n")
        file.write("      url [\n")
        file.write(f"        \"{GRASS_TEXTURE}\"\n")
        file.write("      ]\n")
        file.write("    }\n")
        file.write("    roughness 1\n")
        file.write("    metalness 0\n")
        file.write("  }\n")
        file.write("}\n")
    else:
        file.write(elevation.floorString)


def length2D(x, y):
    """Return the 2D length."""
    return math.sqrt(math.pow(x, 2) + math.pow(y, 2))


def clean_string(string):
    """Removed unwanted characters from string."""
    return string.replace(" ", "_").replace("'", "_").replace(".", "").replace(",", "_") \
                 .encode('ascii', errors='ignore').decode()


def get_intersection(x1, y1, x2, y2, x3, y3, x4, y4):
    """Return intersection between segment 1 (p1-p2) and segment 2 (p3-p4)."""
    p1x = min(x1, x2)
    p2x = max(x1, x2)
    p3x = min(x3, x4)
    p4x = max(x3, x4)
    if p1x == x1:
        p1y = y1
        p2y = y2
    else:
        p1y = y2
        p2y = y1
    if p3x == x3:
        p3y = y3
        p4y = y4
    else:
        p3y = y4
        p4y = y3

    d = (p4y - p3y) * (p2x - p1x) - (p2y - p1y) * (p4x - p3x)
    if d == 0.0:
        return (None, None)

    x = (p1y - p3y) * (p4x - p3x) * (p2x - p1x) / d
    if not (p2x - p1x) == 0.0:
        y = ((p2y - p1y) / (p2x - p1x)) * x + p1y
    elif not (p4x - p3x) == 0.0:
        y = ((p4y - p3y) / (p4x - p3x)) * x + p3y
    else:
        return (None, None)
    x = x + p1x
    # detect if x and y are in the 'bounds' defined by the two vectors
    if x < min(p1x, p2x) or x > max(p1x, p2x):
        return (None, None)
    if x < min(p3x, p4x) or x > max(p3x, p4x):
        return (None, None)
    if y < min(p1y, p2y) or y > max(p1y, p2y):
        return (None, None)
    if y < min(p3y, p4y) or y > max(p3y, p4y):
        return (None, None)
    return (x, y)


def extract_float_from_string(str):
    """Extract a float from a given string.

    Algorithm from: http://stackoverflow.com/questions/4703390/how-to-extract-a-floating-number-from-a-string-in-python
    Examples:
      - "2" returns 2
      - "3m" returns 3
      - "Current Level: -13.2 db or 14.2 or -3" returns -13.2
    """
    rx = re.compile(r"""
        [-+]? # optional sign
        (?:
            (?: \d* \. \d+ ) # .1 .12 .123 etc 9.1 etc 98.1 etc
            |
            (?: \d+ \.? ) # 1. 12. 123. etc 1 12 123 etc
        )
        # followed by optional exponent part if desired
        (?: [Ee] [+-]? \d+ ) ?
    """, re.VERBOSE)
    floats = rx.findall(str)
    if not floats:
        return 0.0
    return float(floats[0])


def protect_def_name(defName):
    """Convert a DEF name to be supported in Webots."""
    protectedDefName = clean_string(defName)
    if protectedDefName and protectedDefName[0].isdigit():
        protectedDefName = "_" + protectedDefName
    return protectedDefName
