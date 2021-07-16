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

"""Contains the Road class."""

from webots_objects.speed_limit import SpeedLimit
from webots_objects.webots_object import WebotsObject

from osm_objects import OSMCoord
from osm_objects import OSMNode

from shapely.geometry import LineString
from shapely.geometry import Point

from utils.shapely_utils import convert_polygon_to_vector2d_list
from utils.shapely_utils import cut_line_string
from utils.shapely_utils import cut_piece_line_string
from utils.shapely_utils import invert_line_string
from utils.shapely_utils import simplify_polygon

from utils.vector import Vector2D
from utils.shapely_utils import intersects

from settings import Settings

import copy
import math
import re


# vertical offset lifting the Roads and Crossroads in order to not be coplanar with the floor.
vOffset = 0.01

ROAD_LINE_DASHED_TEXTURE = 'https://raw.githubusercontent.com/cyberbotics/webots/R2021b/projects/objects/road/protos/textures/road_line_dashed.png'  # noqa: E501
ROAD_LINE_TRIANGLE_TEXTURE = 'https://raw.githubusercontent.com/cyberbotics/webots/R2021b/projects/objects/road/protos/textures/road_line_triangle.png'  # noqa: E501


class Road(WebotsObject):
    """Road class representing a Webots Road."""

    noIntersectionRoadLines = False
    speedLimit = None

    roadNameIndex = 1
    pedestrianCrossingNameIndex = 1
    nameList = []
    roads = []  # Road instances container
    crossroads = {}  # Crossroad instances container
    allRoadWayPoints = []  # Stores a copy of all the exported waypoints

    def __init__(self):
        """Initialize the road."""
        self.osmid = ''
        self.id = ''
        self.refs = 0
        self.tags = {}
        self.crossings = []
        self.originalPath = None
        self.finalPath = None
        self.shape = None
        self.startingAngle = None
        self.endingAngle = None
        self.startJunction = None
        self.endJunction = None

    def parse_tags(self):
        """Extract road data from the OSM tags and the Settings. Returns the validity of this road."""
        self.name = ""
        self.streetName = ""
        self.lanes = 2
        self.forwardLanes = 1
        self.backwardLanes = 1
        self.turnLanesForward = None
        self.turnLanesBackward = None
        self.noBorder = False
        self.layer = 0.0
        self.maxSpeed = -1.0

        self.type = self.tags['highway']
        settingsSection = 'road_' + self.type

        if Settings.has_option('road', 'filter'):
            ids = [str(x) for x in Settings.get('road', 'filter').split(', ')]
            if self.osmid not in ids:
                return False
        else:
            if settingsSection not in Settings.sections():
                settingsSection = 'road'
                if settingsSection not in Settings.sections():
                    return False

            if Settings.has_option(settingsSection, 'ignore') and Settings.get(settingsSection, 'ignore') == 'TRUE':
                return False

        if 'name' in self.tags:
            self.streetName = self.tags['name']

        if 'lanes' in self.tags:
            self.lanes = int(self.tags['lanes'])
            self.forwardLanes = self.lanes - self.backwardLanes
            assert self.lanes > 0
        elif Settings.has_option(settingsSection, 'laneNumber'):
            self.lanes = Settings.getint(settingsSection, 'laneNumber')

        if 'layer' in self.tags:
            self.layer = float(self.tags['layer'])

        try:
            if 'lanes:forward' in self.tags:
                forwardLanes = int(self.tags['lanes:forward'])
                assert forwardLanes > 0
                assert self.lanes - forwardLanes >= 0
                self.forwardLanes = forwardLanes
                self.backwardLanes = self.lanes - self.forwardLanes
        except (ValueError, AssertionError):
            print('Invalid "lanes:forward" tag for "%s"' % self.name)

        try:
            if 'lanes:backward' in self.tags:
                backwardLanes = int(self.tags['lanes:backward'])
                lanes = self.lanes
                forwardLanes = self.forwardLanes
                # note: "lanes" is overriden if "lanes:backward" and "lanes:forward" are both defined.
                if 'lanes:forward' in self.tags:
                    lanes = forwardLanes + backwardLanes
                else:
                    forwardLanes = lanes - backwardLanes
                assert backwardLanes > 0
                assert lanes - backwardLanes >= 0
                self.backwardLanes = backwardLanes
                self.forwardLanes = forwardLanes
                self.lanes = lanes
        except (ValueError, AssertionError):
            print('Invalid "lanes:backward" tag for "%s"' % self.name)

        # Note: "turn:lanes" seems to have no influence on the lanes in JOSM.

        self.insideARoundAbout = 'junction' in self.tags and self.tags['junction'] == 'roundabout'
        self.oneway = 'oneway' in self.tags and self.tags['oneway'] == 'yes'

        if self.oneway or self.insideARoundAbout:
            self.backwardLanes = 0
            self.forwardLanes = self.lanes

        if self.backwardLanes == 0 and self.forwardLanes > 0:
            self.oneway = True

        if 'turn:lanes:forward' in self.tags:
            self.turnLanesForward = self.tags['turn:lanes:forward']
            if len(self.turnLanesForward.split('|')) != self.forwardLanes:
                print('Invalid "turn:lanes:forward" tag for "%s"' % self.name)
                self.turnLanesForward = None

        if 'turn:lanes:backward' in self.tags:
            self.turnLanesBackward = self.tags['turn:lanes:backward']
            if len(self.turnLanesBackward.split('|')) != self.backwardLanes:
                print('Invalid "turn:lanes:backward" tag for "%s"' % self.name)
                self.turnLanesBackward = None

        self.width = None
        if 'width' in self.tags:
            # ref. http://wiki.openstreetmap.org/wiki/Key:width
            # Note: only the documented "width" values are supported,
            #       even if a lot of other values could be present:
            #       https://taginfo.openstreetmap.org/keys/?key=width#values
            m = re.match(r'(\d+)\s*\'\s*(\d*)\s*"?', self.tags['width'])
            if m:
                # Attempt to extract these cases: "3'" or "4'12\""
                self.width = int(m.group(1)) * 0.3048  # foot to meters
                if m.group(2):
                    self.width += int(m.group(2)) * 0.0254  # inch to meters
            else:
                m = re.match(r'(\d*\.\d+|\d+)\s*(.*)', self.tags['width'])
                if m:
                    # Attempt to extract these cases: "3", "3.3", "3 mi" or "3 km"
                    self.width = float(m.group(1))
                    if m.group(2):
                        if m.group(2) == 'mi':
                            self.width *= 1609.34  # miles to meters
                        elif m.group(2) == 'km':
                            self.width *= 1000.0  # km to meters
                        else:
                            self.width = None
                else:
                    self.width = None
            if self.width is None:
                # Failed to extract width.
                print('Warning: Invalid "width" tag for "%s" OSMID.: "%s"' % (self.osmid, self.tags['width']))

        if self.width is None:
            laneWidth = 3.5
            if Settings.has_option(settingsSection, 'laneWidth'):
                laneWidth = Settings.getfloat(settingsSection, 'laneWidth')
            self.width = laneWidth * self.lanes

        if Settings.has_option(settingsSection, 'border') and Settings.get(settingsSection, 'border') == 'FALSE':
            self.noBorder = True

        return True

    def is_similar(self, other):
        """Determine if a road has the same graphical shape than another in order to be merged."""
        if (self.oneway and not other.oneway) or (not self.oneway and other.oneway):
            return False  # reject if the oneway attribute is not matching

        if self.oneway:
            # both roads are oneway
            return (
                self.lanes == other.lanes and
                self.forwardLanes == other.forwardLanes and
                self.backwardLanes == other.backwardLanes and
                abs(self.width - other.width) <= 0.01
            )
        else:
            # both roads are bidirectional
            return (
                self.lanes == other.lanes and
                self.forwardLanes == other.backwardLanes and
                self.backwardLanes == other.forwardLanes and
                abs(self.width - other.width) <= 0.01
            )

    def is_inside_a_city(self):
        """Return True if the road is inside a city."""
        # The best found criteria to determine if a road is inside a city or not
        # is the fact that this road or its connecting roads are tagged as residential.
        # cf. https://help.openstreetmap.org/questions/56876/how-to-determine-if-a-road-is-outside-or-inside-place
        if 'residential' in self.type.lower():
            return True

        connectedJunctions = []
        if self.startJunction is not None:
            connectedJunctions.append(self.startJunction)
        if self.endJunction is not None:
            connectedJunctions.append(self.endJunction)
        for crossroad in connectedJunctions:
            for connectedRoad in crossroad.roads:
                if self != connectedRoad and 'residential' in connectedRoad.type.lower():
                    return True

        return False

    def process_path(self):
        """Convert the OSM way to a shapely geometry."""
        points = []
        for ref in self.refs:
            if ref in OSMCoord.coordDictionnary:
                p = Point(OSMCoord.coordDictionnary[ref].x, OSMCoord.coordDictionnary[ref].z)
                points.append(p)
        if len(self.refs) >= 2:
            self.originalPath = LineString(points)
            self.finalPath = self.originalPath
        else:
            self.originalPath = None
            self.finalPath = None

    def smooth_sharp_angles(self):
        """Smooth sharp angles from self."""
        if self.originalPath is None or not isinstance(self.originalPath, LineString) or len(self.originalPath.coords) < 3:
            return

        angleThreshold = 1.3  # angle threshlod in radian from which an angle is considered as sharp
        distanceThreshold = 5.0  # distance threshlod in meters from which a distance can be considered as sharp
        newPointsDistance = 3.0  # distance in meters where to add new points

        path = []
        path.append([self.originalPath.coords[0][0], self.originalPath.coords[0][1]])
        for i in range(len(self.originalPath.coords) - 2):
            p1 = Vector2D(self.originalPath.coords[i][0], self.originalPath.coords[i][1])
            p2 = Vector2D(self.originalPath.coords[i + 1][0], self.originalPath.coords[i + 1][1])
            p3 = Vector2D(self.originalPath.coords[i + 2][0], self.originalPath.coords[i + 2][1])

            v1to2 = p2 - p1
            v2to3 = p3 - p2

            angle = v1to2.angle(v2to3)

            if abs(angle) > angleThreshold and v1to2.norm() > distanceThreshold and v2to3.norm() > distanceThreshold:
                # sharp angle detected in p2
                p2before = p2 + v1to2.normalize() * (- newPointsDistance)
                p2after = p2 + v2to3.normalize() * newPointsDistance
                p2new = ((p2before + p2after) * 0.5 + p2) * 0.5

                path.append([p2before.x, p2before.y])
                path.append([p2new.x, p2new.y])
                path.append([p2after.x, p2after.y])
            else:
                path.append([p2.x, p2.y])
        path.append([self.originalPath.coords[-1][0], self.originalPath.coords[-1][1]])

        self.originalPath = LineString(path)
        self.finalPath = self.originalPath

    def process_shape(self):
        """Create the shapely polygon from the shapely path."""
        if self.originalPath is not None:
            self.shape = self.originalPath.buffer(0.5 * self.width, cap_style=2, join_style=2)

    def split_at(self, node):
        """Split a road in 2 at some node."""
        assert node in self.refs
        a = copy.deepcopy(self)
        cut = next((i for i, v in enumerate(a.refs) if v == node))
        assert cut != 0 and cut != len(self.refs) - 1
        a.refs = a.refs[:(cut + 1)]
        b = copy.deepcopy(self)
        b.refs = b.refs[cut:]
        return [a, b]

    @staticmethod
    def add_to_list(osmid, tags, refs):
        """Add a new road to the list of roads."""
        settingsSection = Settings.get_section('road', tags['highway'])
        if settingsSection is None:
            return
        road = Road()
        road.osmid = osmid
        road.id = str(osmid)
        road.refs = refs
        road.tags = tags
        if road.parse_tags():
            # Only add the road if valid.
            Road.roads.append(road)
        for ref in road.refs:
            node = OSMNode.nodeDictionnary[ref]
            if 'highway' in node.tags and node.tags['highway'] == 'crossing':
                road.crossings.append(node)

    @classmethod
    def initialize_speed_limit(cls, country):
        """Create the SpeedLimit instance."""
        cls.speedLimit = SpeedLimit(country)

    @classmethod
    def export(cls, f):
        """Export all the roads from the roads list."""
        # Export crossroads
        sortedCrossroads = sorted(cls.crossroads.values(), key=lambda x: x.id)
        for crossroad in sortedCrossroads:
            translation = Vector2D(OSMCoord.coordDictionnary[crossroad.nodeRef].x,
                                   OSMCoord.coordDictionnary[crossroad.nodeRef].z)

            f.write('Crossroad {\n')
            f.write('  translation %f %f %f\n' % (translation.x, vOffset, translation.y))
            if crossroad.name is not None:
                name = crossroad.name
                i = 1
                while name in Crossroad.nameList:
                    name = crossroad.name + '(%d)' % i
                    i += 1
                Crossroad.nameList.append(name)
                f.write('  name "%s"\n' % name)
            else:
                f.write('  name "crossroad(%d)"\n' % Crossroad.nameIndex)
                Crossroad.nameIndex += 1
            f.write('  id "%s"\n' % crossroad.id)
            f.write('  shape [\n')
            if crossroad.shape is not None:
                coords = convert_polygon_to_vector2d_list(crossroad.shape)
                for coord in coords:
                    height = 0
                    if WebotsObject.enable3D:
                        osmCoord = cls._convert_webots_to_osm_coord([coord.x, coord.y])
                        height += WebotsObject.elevation.interpolate_height(osmCoord[0], osmCoord[1])
                    f.write('    %f %f %f\n' % (coord.x - translation.x, height, coord.y - translation.y))
            f.write('  ]\n')
            f.write('  connectedRoadIDs [\n')
            sortedRoads = sorted(crossroad.roads, key=lambda x: x.id)
            for road in sortedRoads:
                f.write('    "%s"\n' % (road.id))
            f.write('  ]\n')
            f.write('}\n')
        # Export roads
        for road in cls.roads:
            if not isinstance(road.finalPath, LineString) or not road.finalPath.coords:
                continue
            coords = road.finalPath.coords

            f.write('Road {\n')
            f.write('  translation %f %f %f\n' % (coords[0][0], vOffset + road.layer * WebotsObject.layerHeight, coords[0][1]))
            if road.streetName:
                name = road.streetName
                i = 1
                while name in Road.nameList:
                    name = road.streetName + '(%d)' % i
                    i += 1
                Road.nameList.append(name)
                f.write('  name "%s"\n' % name.replace('"', ''))
            else:
                f.write('  name "road(%d)"\n' % Road.roadNameIndex)
                Road.roadNameIndex += 1
            f.write('  id "%s"\n' % road.id)
            if road.startJunction is not None:
                f.write('  startJunction "%s"\n' % road.startJunction.id)
            if road.endJunction is not None:
                f.write('  endJunction "%s"\n' % road.endJunction.id)
            f.write('  splineSubdivision 0\n')
            f.write('  numberOfLanes %d\n' % road.lanes)
            f.write('  numberOfForwardLanes %d\n' % road.forwardLanes)
            if road.maxSpeed > 0.0:
                f.write('  speedLimit %f\n' % road.maxSpeed)
            f.write('  width %f\n' % road.width)
            if road.noBorder:
                f.write('  leftBorder FALSE\n')
                f.write('  rightBorder FALSE\n')
            if road.startingAngle:
                if road.startingAngle > math.pi:
                    road.startingAngle -= 2 * math.pi
                elif road.startingAngle < -math.pi:
                    road.startingAngle += 2 * math.pi
                f.write('  startingAngle %f\n' % road.startingAngle)
            if road.endingAngle:
                if road.endingAngle > math.pi:
                    road.endingAngle -= 2 * math.pi
                elif road.endingAngle < -math.pi:
                    road.endingAngle += 2 * math.pi
                f.write('  endingAngle %f\n' % road.endingAngle)
            f.write('  lines [\n')
            for i in range(road.lanes - 1):
                f.write('    RoadLine {\n')
                f.write('      type "%s"\n' % ('continuous' if (i == road.forwardLanes - 1) else 'dashed'))
                f.write('    }\n')
            f.write('  ]\n')
            if road.turnLanesForward:
                f.write('  turnLanesForward "%s"\n' % road.turnLanesForward)
            if road.turnLanesBackward:
                f.write('  turnLanesBackward "%s"\n' % road.turnLanesBackward)
            if not Road.noIntersectionRoadLines and not road.insideARoundAbout:
                if road.startJunction is not None and len(road.startJunction.roads) > 2:
                    f.write('  startLine [\n')
                    for l in range(road.forwardLanes):
                        f.write(f'    "{ROAD_LINE_DASHED_TEXTURE}"\n')
                    for l in range(road.backwardLanes):
                        f.write(f'    "{ROAD_LINE_TRIANGLE_TEXTURE}"\n')
                    f.write('  ]\n')
                if road.endJunction is not None and len(road.endJunction.roads) > 2:
                    f.write('  endLine [\n')
                    for l in range(road.forwardLanes):
                        f.write(f'    "{ROAD_LINE_TRIANGLE_TEXTURE}"\n')
                    for l in range(road.backwardLanes):
                        f.write(f'    "{ROAD_LINE_DASHED_TEXTURE}"\n')
                    f.write('  ]\n')
            f.write('  wayPoints [\n')
            for coord in coords:
                height = 0
                if WebotsObject.enable3D:
                    osmCoord = cls._convert_webots_to_osm_coord(coord)
                    height += WebotsObject.elevation.interpolate_height(osmCoord[0], osmCoord[1])
                f.write('    %f %f %f\n' % (coord[0] - coords[0][0], height, coord[1] - coords[0][1]))
                Road.allRoadWayPoints.append([coord[0], coord[1]])
            f.write('  ]\n')
            f.write('}\n')
            # Export pedestrian crossings
            for crossingNode in road.crossings:
                for i in range(len(road.refs)):
                    ref = road.refs[i]
                    translation = Vector2D(OSMCoord.coordDictionnary[crossingNode.OSMID].x,
                                           OSMCoord.coordDictionnary[crossingNode.OSMID].z)
                    if (translation.x == OSMCoord.coordDictionnary[ref].x and
                            translation.y == OSMCoord.coordDictionnary[ref].z):
                        if i == len(road.refs) - 1:
                            otherRef = road.refs[i - 1]
                        else:
                            otherRef = road.refs[i + 1]
                        alpha = math.atan((OSMCoord.coordDictionnary[otherRef].x - OSMCoord.coordDictionnary[ref].x) /
                                          (OSMCoord.coordDictionnary[otherRef].z - OSMCoord.coordDictionnary[ref].z))
                        width = road.width
                        for crossroad in cls.crossroads.values():
                            # If the pedestrian crossing is on crossroad, it is moved.
                            if (translation.x == OSMCoord.coordDictionnary[crossroad.nodeRef].x and
                                    translation.y == OSMCoord.coordDictionnary[crossroad.nodeRef].z):
                                # Crossing must be parallel to this vector.
                                vector = Vector2D(OSMCoord.coordDictionnary[otherRef].x - OSMCoord.coordDictionnary[ref].x,
                                                  OSMCoord.coordDictionnary[otherRef].z - OSMCoord.coordDictionnary[ref].z)
                                normVector = math.sqrt(vector.x ** 2 + vector.y ** 2)
                                distanceFromCenter = 6.0  # distance to add to the translation in the good direction
                                if crossroad.shape is not None:
                                    bounds = crossroad.shape.bounds
                                    # center of the crossroad
                                    centre = Vector2D((bounds[2] + bounds[0]) / 2, (bounds[3] + bounds[1]) / 2)
                                    # difference between crossroad point and center
                                    difference = Vector2D(centre.x - OSMCoord.coordDictionnary[crossroad.nodeRef].x,
                                                          centre.y - OSMCoord.coordDictionnary[crossroad.nodeRef].z)
                                    beta = math.atan(difference.x / difference.y)
                                    normDifference = math.sqrt(difference.x ** 2 + difference.y ** 2)
                                    # radius of the crossroad
                                    radius = math.sqrt(((bounds[2] - bounds[0]) / 2) ** 2 + ((bounds[3] - bounds[1]) / 2) ** 2)
                                    # We need to add a correction distance to crossing translation because the center of the
                                    # crossroad is not the same point as the crossroad point.
                                    corrector = normDifference * abs(math.cos(beta - alpha))
                                    if normVector > math.sqrt((centre.x - OSMCoord.coordDictionnary[otherRef].x) ** 2 +
                                                              (centre.y - OSMCoord.coordDictionnary[otherRef].z) ** 2):
                                        corrector = -corrector
                                    distanceFromCenter = radius - corrector
                                translation = Vector2D(OSMCoord.coordDictionnary[ref].x +
                                                       (distanceFromCenter / normVector * vector.x),
                                                       OSMCoord.coordDictionnary[ref].z +
                                                       (distanceFromCenter / normVector * vector.y))
                                break
                        f.write('PedestrianCrossing {\n')
                        f.write('  translation %f %f %f\n' % (translation.x, -0.07, translation.y))
                        f.write('  rotation 0 1 0 %f\n' % (alpha))
                        f.write('  name "pedestrian crossing(%d)"\n' % (Road.pedestrianCrossingNameIndex))
                        Road.pedestrianCrossingNameIndex += 1
                        f.write('  size %f %f\n' % (width, width * 0.4))
                        f.write('}\n')
                        break

    @classmethod
    def process(cls):
        """Process the roads before exportation."""
        # split the roads at crossroads
        while True:
            modified = False

            for roadA in Road.roads:
                for refA in roadA.refs:
                    for roadB in Road.roads:
                        for refB in roadB.refs:
                            if refA == refB and not roadA == roadB:
                                ref = refA
                                for r in [roadA, roadB]:
                                    if r.refs[0] != ref and r.refs[-1] != ref:
                                        [r1, r2] = r.split_at(ref)
                                        Road.roads.remove(r)
                                        r1.id = Road.generate_unique_road_id(r.osmid)
                                        Road.roads.append(r1)
                                        r2.id = Road.generate_unique_road_id(r.osmid)
                                        Road.roads.append(r2)
                                        modified = True
                                if modified:
                                    break
                        if modified:
                            break
                    if modified:
                        break
                if modified:
                    break

            if not modified:
                break

        # create crossroads, and link road <-> crossroad.
        cls.crossroads = {}
        for roadA in Road.roads:
            for refA in roadA.refs:
                for roadB in Road.roads:
                    for refB in roadB.refs:
                        if refA == refB and not roadA == roadB:
                            crossroad = None
                            if refA in cls.crossroads:
                                crossroad = cls.crossroads[refA]
                            else:
                                crossroad = Crossroad(refA)
                                cls.crossroads[refA] = crossroad
                            crossroad.add_road(roadA)
                            crossroad.add_road(roadB)
        for crossroad in cls.crossroads.values():
            for road in crossroad.roads:
                if road.refs[0] == crossroad.nodeRef:
                    road.startJunction = crossroad
                if road.refs[-1] == crossroad.nodeRef:
                    road.endJunction = crossroad

        # The speed limit can only be computed at this point, because the final network need to be known
        # to evaluate if the road is inside or outside a city.
        if cls.speedLimit is not None:
            for road in Road.roads:
                road.maxSpeed = cls.speedLimit.compute_speed_limit(road)

        # process the path of each road, and the corresponding shapely geometry.
        for road in Road.roads:
            road.process_path()
            road.smooth_sharp_angles()
            road.process_shape()
        # process shapely geometry for each crossroad, and substract them to the road paths.
        for crossroad in cls.crossroads.values():
            crossroad.process_shape()
            crossroad.cut_roads()

    @classmethod
    def generate_unique_road_id(cls, osmid):
        """Create a new road unique ID."""
        counter = 0
        while True:
            counter += 1
            candidate = '%s_%d' % (osmid, counter)
            unique = True
            for road in cls.roads:
                if road.id == candidate:
                    unique = False
                    break
            if unique:
                return candidate

    @classmethod
    def are_coords_close_to_some_road_waypoint(cls, coordinates, areOSMReferences=True):
        """Return if a list of coordinates close to some road waypoints."""
        if areOSMReferences:
            for ref in coordinates:
                if ref in OSMCoord.coordDictionnary:
                    coordPos = [OSMCoord.coordDictionnary[ref].x, OSMCoord.coordDictionnary[ref].z]
                    if Road.is_coord_close_to_some_road_waypoint_way_point(coordPos):
                        return True
        else:
            for ref in coordinates:
                if Road.is_coord_close_to_some_road_waypoint_way_point([ref[0], ref[1]]):
                    return True
        return False

    @classmethod
    def is_coord_close_to_some_road_waypoint_way_point(cls, pos):
        """Check if the given pos is close from one road waypoint."""
        for roadWayPoint in cls.allRoadWayPoints:
            if math.sqrt(math.pow(roadWayPoint[0] - pos[0], 2) + math.pow(roadWayPoint[1] - pos[1], 2)) < Road.removalRadius:
                return True
        return False

    @classmethod
    def _convert_webots_to_osm_coord(cls, coord):
        return [-coord[0] + WebotsObject.xOffset, coord[1] + WebotsObject.zOffset]


class Crossroad:
    """Crossroad class representing a Webots Crossroad."""

    nameIndex = 1
    nameList = []

    def __init__(self, nodeRef):
        """Constuctor: create an empty crossroad."""
        self.nodeRef = nodeRef
        self.roads = set()
        self.shape = None
        self.name = None
        self.id = self.nodeRef
        if self.nodeRef in OSMNode.nodeDictionnary:
            node = OSMNode.nodeDictionnary[self.nodeRef]
            if 'name' in node.tags:
                self.name = node.tags['name']

    def add_road(self, road):
        """Add a road."""
        self.roads.add(road)

    def process_shape(self):
        """Create the crossroad shape using shapely and the touching road shapes."""
        if self.nodeRef not in OSMCoord.coordDictionnary or len(self.roads) < 2:
            return

        # Do not create a shape if the junctions have only 2 "similar" roads
        roadsList = list(self.roads)
        if len(roadsList) == 2 and roadsList[0].is_similar(roadsList[1]):
            return

        # Strategy: increase the influency length of each touching roads
        # individually until there is no collision. Create an union of the
        # resulting segments.

        increment = 3.0
        distData = []
        for road in self.roads:
            distData.append({
                'road': road,
                'distance': 1.0,
                'intersect': True,
                'shape': None
            })
        for i in range(0, 60):
            for d in distData:
                if not d['intersect']:
                    continue
                path = None
                if self.nodeRef == d['road'].refs[0]:
                    path = d['road'].originalPath
                elif self.nodeRef == d['road'].refs[-1]:
                    path = invert_line_string(d['road'].originalPath)
                else:
                    assert 0, "Cannot occur if the roads have been cut correctly."

                path = cut_piece_line_string(path, d['distance'], increment)
                d['shape'] = path.buffer(0.5 * d['road'].width, cap_style=2, join_style=2)

            for d1 in distData:
                if not d1['intersect']:
                    continue
                intersect = False
                for d2 in distData:
                    if d1 != d2 and d1['shape'].intersects(d2['shape']):
                        intersect = True
                        break
                if not intersect:
                    d1['intersect'] = False

            intersect = False
            for d in distData:
                if d['intersect']:
                    intersect = True
            if not intersect:
                break

            for d in distData:
                if d['intersect']:
                    d['distance'] += increment

        self.shape = None
        for data in distData:
            path = None
            if self.nodeRef == data['road'].refs[0]:
                path = data['road'].originalPath
            elif self.nodeRef == data['road'].refs[-1]:
                path = invert_line_string(data['road'].originalPath)
            else:
                assert 0, "Cannot occur if the roads have been cut correctly."

            path = cut_line_string(path, data['distance'])
            newShape = path[0].buffer(0.5 * data['road'].width, cap_style=2, join_style=2)
            if self.shape is None:
                self.shape = newShape
            else:
                self.shape = self.shape.union(newShape)
        # Remove doubled coordinates.
        self.shape = simplify_polygon(self.shape.convex_hull)

    def cut_roads(self):
        """Remove the roads touching this junction using the junction shape and
           compute the start/end angles at these intersections."""
        if self.shape is None:
            if len(self.roads) == 2:
                # When there is a crossroad containing 2 similar roads and
                # the crossroad has no shape, the roads start/end angles should match.
                r0 = list(self.roads)[0]
                r1 = list(self.roads)[1]
                if r0.is_similar(r1) and len(r0.originalPath.coords) >= 2 and len(r1.originalPath.coords) >= 2:
                    v00 = None
                    v01 = None
                    if r0.startJunction == self:
                        v00 = Vector2D(r0.originalPath.coords[0][0], r0.originalPath.coords[0][1])
                    else:
                        v00 = Vector2D(r0.originalPath.coords[-1][0], r0.originalPath.coords[-1][1])
                    if r0.startJunction == self:
                        v01 = Vector2D(r0.originalPath.coords[1][0], r0.originalPath.coords[1][1])
                    else:
                        v01 = Vector2D(r0.originalPath.coords[-2][0], r0.originalPath.coords[-2][1])

                    alpha = (v00 - v01).angle()

                    if r0.startJunction == self:
                        r0.startingAngle = alpha + 0.5 * math.pi
                    elif r0.endJunction == self:
                        r0.endingAngle = alpha - 0.5 * math.pi
                    if r1.startJunction == self:
                        r1.startingAngle = alpha - 0.5 * math.pi
                    elif r1.endJunction == self:
                        r1.endingAngle = alpha + 0.5 * math.pi
            return

        for road in self.roads:
            # Substract this crossroad shape to the road path
            road.shape = road.shape.difference(self.shape)

            if not isinstance(road.finalPath, LineString) or not road.finalPath.coords:
                road.finalPath = road.finalPath.difference(self.shape)
                continue
            roadPathBeforeEndsRemoval = [Vector2D(x, y) for (x, y) in road.finalPath.coords]

            firstPoint = roadPathBeforeEndsRemoval[0]
            lastPoint = roadPathBeforeEndsRemoval[-1]

            road.finalPath = road.finalPath.difference(self.shape)
            if not isinstance(road.finalPath, LineString) or not road.finalPath.coords:
                continue

            roadPath = [Vector2D(x, y) for (x, y) in road.finalPath.coords]

            # Search if the first or last point are still present in the cut path
            firstPointPresent = False
            lastPointPresent = False
            for c in road.finalPath.coords:
                v = Vector2D(c[0], c[1])
                if (v - firstPoint).norm() < 0.1:
                    firstPointPresent = True
                elif (v - lastPoint).norm() < 0.1:
                    lastPointPresent = True

            # Compute the starting and ending angles thanks to the intersection
            # point between the crossroad shape and the road path.
            crossroadCoords = convert_polygon_to_vector2d_list(self.shape)
            if not firstPointPresent:
                roadSegment = (roadPath[1], roadPath[0])
                for i in range(len(crossroadCoords) - 1):
                    crossroadShapeSegment = (crossroadCoords[i], crossroadCoords[i + 1])
                    intersection = intersects(roadSegment[0], roadSegment[1], crossroadShapeSegment[0],
                                              crossroadShapeSegment[1])
                    if intersection and intersection != roadPath[1]:
                        road.startingAngle = (crossroadShapeSegment[1] - crossroadShapeSegment[0]).angle() % (2.0 * math.pi)
            if not lastPointPresent:
                nRoadPath = len(roadPath)
                roadSegment = (roadPath[nRoadPath - 2], roadPath[nRoadPath - 1])
                for i in range(len(crossroadCoords) - 1):
                    crossroadShapeSegment = (crossroadCoords[i], crossroadCoords[i + 1])
                    intersection = intersects(roadSegment[0], roadSegment[1], crossroadShapeSegment[0],
                                              crossroadShapeSegment[1])
                    if intersection and intersection != roadPath[nRoadPath - 2]:
                        road.endingAngle = (crossroadShapeSegment[0] - crossroadShapeSegment[1]).angle() % (2.0 * math.pi)
