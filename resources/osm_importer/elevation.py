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

"""Sponsored by the CTI project RO2IVSim (http://transport.epfl.ch/simulator-for-mobile-robots-and-intelligent-vehicles)."""

import json
import math
import sys
import time
import urllib.parse
import urllib.request

from utils.misc_utils import length2D

GOOGLE_ELEVATION_BASE_URL = 'https://maps.googleapis.com/maps/api/elevation/json'
GEAONAMES_ELEVATION_BASE_URI = 'http://api.geonames.org/astergdemJSON'

GRASS_TEXTURE = 'https://raw.githubusercontent.com/cyberbotics/webots/R2021b/projects/default/worlds/textures/grass.jpg'


class Elevation(object):
    """Elevation class."""

    @staticmethod
    def get_elevation_from_google(locations, key=""):
        """Create a dictionary for each results[] object."""
        elevationArray = []

        offset = 0
        finished = False
        while not finished:
            finished = True
            locationString = ""
            for i in range(offset, len(locations)):
                locationString = locationString + str(locations[i][0]) + "," + str(locations[i][1]) + "|"
                # maximum of 512 locations per request and url max length of 2000
                if ((i - offset) > 500 or
                        len(GOOGLE_ELEVATION_BASE_URL + '?' + urllib.parse.urlencode({'locations': locationString})) > 1800):
                    offset = i + 1
                    finished = False
                    time.sleep(0.3)  # maximum 5 request per second
                    break
            locationString = locationString[:-1]  # remove last '|'

            elvtn_args = {
                'locations': locationString
            }
            if key:
                elvtn_args['key'] = key

            url = GOOGLE_ELEVATION_BASE_URL + '?' + urllib.parse.urlencode(elvtn_args)
            response = json.load(urllib.request.urlopen(url))

            if not response['status'] == 'OK':
                sys.stderr.write(response['status'])
                sys.stderr.write(response.get('error_message'))
                return []

            for result in response['results']:
                elevationArray.append(result['elevation'])
        if not len(elevationArray) == len(locations):
            sys.stderr.write("Error: not all the elevation point have been acquired")
            return []
        return elevationArray

    @staticmethod
    def get_elevation_from_geonames(locations, key=""):
        """Create a dictionary for each results[] object."""
        elevationArray = []

        print('Aquiring elevation data, please be patient\n')
        sys.stdout.flush()
        for location in locations:
            elvtn_args = {
                'lat': str(location[0]),
                'lng': str(location[1])
            }

            if not key == "":
                elvtn_args['username'] = key

            url = GEAONAMES_ELEVATION_BASE_URI + '?' + urllib.parse.urlencode(elvtn_args)
            response = json.load(urllib.request.urlopen(url))
            if 'status' in response:
                sys.stderr.write(response['status']['message'])
                return []
            elevationArray.append(response['astergdem'])

        return elevationArray

    def __init__(self, projection, minlat=46.5062, minlon=6.5506, maxlat=46.5264, maxlon=6.5903, useGoogle=True,
                 googleAPIKey=''):
        """Initialize the projection."""
        x1, z1 = projection(minlon, minlat)
        x2, z2 = projection(maxlon, maxlat)

        Xmin = min(x1, x2)
        Xmax = max(x1, x2)
        Zmin = min(z1, z2)
        Zmax = max(z1, z2)

        Xdiff = Xmax - Xmin
        Zdiff = Zmax - Zmin

        xDiv = int(Xdiff / 30) + 1
        zDiv = int(Zdiff / 30) + 1

        self.xStep = Xdiff / xDiv
        self.zStep = Zdiff / zDiv

        # generate all the locations (lat and long array) in function of the X, Z grid
        locations = []
        for z in range(0, zDiv + 1):
            for x in range(0, xDiv + 1):
                long, lat = projection(Xmax - x * self.xStep, z * self.zStep + Zmin, inverse=True)
                long = float(format(long, '.7f'))  # we want to reduce precision in order to reduce the request url
                lat = float(format(lat, '.7f'))
                locations.append([lat, long])

        result = []
        if useGoogle and googleAPIKey:
            result = Elevation.get_elevation_from_google(locations, key=googleAPIKey)
        else:
            result = Elevation.get_elevation_from_geonames(locations, "cyberbotics")

        if not result:
            sys.stderr.write("Warning: the acquisition of the elevation data failed.\n")
            return

        # shift the height to have the minimum height been at altitude 0
        offset = min(result)
        for index in range(len(result)):
            result[index] = result[index] - offset

        self.floorString = ""
        self.floorString += "DEF FLOOR Solid {\n"
        self.floorString += "  translation %.2f 0 %.2f\n" % (-Xdiff / 2, -Zdiff / 2)
        self.floorString += "  children [\n"
        self.floorString += "    DEF FLOOR_SHAPE Shape {\n"
        self.floorString += "      appearance PBRAppearance {\n"
        self.floorString += "        baseColorMap ImageTexture {\n"
        self.floorString += "          url [\n"
        self.floorString += f"            \"{GRASS_TEXTURE}\"\n"
        self.floorString += "          ]\n"
        self.floorString += "        }\n"
        self.floorString += "        roughness 1\n"
        self.floorString += "        metalness 0\n"
        scale = max(Xdiff, Zdiff)
        self.floorString += "        textureTransform TextureTransform {\n"
        self.floorString += "          scale %.2f %.2f\n" % (scale, scale)
        self.floorString += "        }\n"
        self.floorString += "      }\n"
        self.floorString += "      geometry ElevationGrid {\n"
        self.floorString += "        height [\n"
        for z in range(0, zDiv + 1):
            self.floorString += "        "
            for x in range(0, xDiv + 1):
                self.floorString += str(result[x + z * (xDiv + 1)]) + " "
            self.floorString += "\n"
        self.floorString += "        ]\n"
        self.floorString += "        xDimension " + str(xDiv + 1) + "\n"
        self.floorString += "        xSpacing " + str(self.xStep) + "\n"
        self.floorString += "        zDimension " + str(zDiv + 1) + "\n"
        self.floorString += "        zSpacing " + str(self.zStep) + "\n"
        self.floorString += "      }\n"
        self.floorString += "    }\n"
        self.floorString += "  ]\n"
        self.floorString += "  boundingObject USE FLOOR_SHAPE\n"
        self.floorString += "}\n"

        self.elevationArray = []
        for z in range(0, zDiv + 1):
            for x in range(0, xDiv + 1):
                self.elevationArray.append({
                    'x': Xmax - x * self.xStep,
                    'z': z * self.zStep + Zmin,
                    'height': result[x + z * (xDiv + 1)]
                })

    def interpolate_height(self, X, Z):
        """Interpolate the height at a given position."""
        xMinus = -float('inf')
        zMinus = -float('inf')
        xPlus = float('inf')
        zPlus = float('inf')
        heights = [0, 0, 0, 0]
        # get the 'boundary' box:
        #        zMinus
        #        0---1
        # xMinus | c | xPlus
        #        3---2
        #        zPlus
        distance = []
        for elevation in self.elevationArray:
            currentX = elevation['x']
            currentZ = elevation['z']
            distance.append(length2D(X - currentX, Z - currentZ))
            if currentX < X:
                if currentX > xMinus:
                    xMinus = currentX
            else:
                if currentX < xPlus:
                    xPlus = currentX
            if currentZ < Z:
                if currentZ > zMinus:
                    zMinus = currentZ
            else:
                if currentZ < zPlus:
                    zPlus = currentZ

        for elevation in self.elevationArray:
            if elevation['x'] == xMinus and elevation['z'] == zMinus:
                heights[0] = elevation['height']
            elif elevation['x'] == xMinus and elevation['z'] == zPlus:
                heights[3] = elevation['height']
            elif elevation['x'] == xPlus and elevation['z'] == zMinus:
                heights[1] = elevation['height']
            elif elevation['x'] == xPlus and elevation['z'] == zPlus:
                heights[2] = elevation['height']

        # compute the ration to determine in which of the two triangle of the box the point lies
        ratio1 = (zPlus - zMinus) / (xPlus - xMinus)
        ratio2 = (Z - zMinus) / (X - xMinus)

        # use a barycentric coordinate system in order to interpolate the value in the triangle
        # http://en.wikipedia.org/wiki/Barycentric_coordinate_system
        x1 = xMinus
        z1 = zMinus
        if ratio2 < ratio1:    # use triangle 0-1-2
            x2 = xPlus
            x3 = xPlus
            z2 = zMinus
            z3 = zPlus
        else:                              # use triangle 0-2-3
            x2 = xPlus
            x3 = xMinus
            z2 = zPlus
            z3 = zPlus
        denominator = (z2 - z3) * (x1 - x3) + (x3 - x2) * (z1 - z3)
        lambda1 = ((z2 - z3) * (X - x3) + (x3 - x2) * (Z - z3)) / denominator
        lambda2 = ((z3 - z1) * (X - x3) + (x1 - x3) * (Z - z3)) / denominator
        lambda3 = 1 - lambda1 - lambda2
        if ratio2 < ratio1:
            height = lambda1 * heights[0] + lambda2 * heights[1] + lambda3 * heights[2]
        else:
            height = lambda1 * heights[0] + lambda2 * heights[2] + lambda3 * heights[3]
        if math.isnan(height) or height == float('inf') or height == -float('inf'):
            return 0
        else:
            return height

    def get_tilt(self, p1, p2):
        """Return tilt at p1 in direction of p2."""
        x1 = p1['x'] - p2['x']
        z1 = p1['z'] - p2['z']
        x2 = -z1
        z2 = x1
        len = length2D(x2, z2)
        if len == 0:
            return 0
        x2 = x2 / len
        z2 = z2 / len
        h1 = self.interpolate_height(p1['x'], p2['z'])
        h2 = self.interpolate_height(p1['x'] + x2, p2['z'] + z2)
        return math.atan2(h1 - h2, 1)

    def get_grid_indexes(self, X, Z):
        """Return the indexes from the elevation array corresponding to the position X and Z."""
        xIndex = -1
        zIndex = -1
        for elevation in self.elevationArray:
            currentX = elevation['x']
            currentZ = elevation['z']
            if currentX < X and (currentX > self.elevationArray[xIndex]['x'] or xIndex < 0):
                xIndex = self.elevationArray.index(elevation)
            if currentZ < Z and (currentZ > self.elevationArray[zIndex]['z'] or zIndex < 0):
                zIndex = self.elevationArray.index(elevation)
        return (xIndex, zIndex)
