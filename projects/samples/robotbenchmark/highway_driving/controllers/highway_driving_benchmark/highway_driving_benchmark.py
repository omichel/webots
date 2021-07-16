"""Controller program to manage the benchmark."""
import os
import random
import socket
import sys

from controller import Supervisor
try:
    from shapely.geometry import Point
    from shapely.geometry import LineString
except ImportError:
    sys.stderr.write("Error: 'shapely' module not found.\n")
    sys.exit(0)

VEHICLE_DEF_NAME = 'WEBOTS_VEHICLE0'
SUMO_PORTS_RANGE = [1024, 1998]
MAX_TIME = 60.0
sensorVisualizationNodes = []

try:
    includePath = os.environ.get("WEBOTS_HOME") + "/projects/samples/robotbenchmark/include"
    includePath.replace('/', os.sep)
    sys.path.append(includePath)
    from robotbenchmark import robotbenchmarkRecord
except ImportError:
    sys.stderr.write("Warning: 'robotbenchmark' module not found.\n")
    sys.exit(0)


def apply_spline_subdivison_to_path(path, subdivision):
    """Apply spline subdivision to the list of points."""
    spline = []
    points = list(path)
    pointsNumber = len(points)
    # extend the points array
    points.append((points[pointsNumber - 1][0] + (points[pointsNumber - 1][0] - points[pointsNumber - 2][0]),
                   points[pointsNumber - 1][1] + (points[pointsNumber - 1][1] - points[pointsNumber - 2][1])))
    points.append((points[pointsNumber][0] + (points[pointsNumber][0] - points[pointsNumber - 1][0]),
                   points[pointsNumber][1] + (points[pointsNumber][1] - points[pointsNumber - 1][1])))
    pointMinus1 = (points[0][0] + (points[0][0] - points[1][0]),
                   points[0][1] + (points[0][1] - points[1][1]))
    pointMinus2 = (pointMinus1[0] + (pointMinus1[0] - points[0][0]),
                   pointMinus1[1] + (pointMinus1[1] - points[0][1]))
    # adding to the end is equivalent to index -1 and -2
    points.append(pointMinus2)
    points.append(pointMinus1)

    # interpolation
    spline.append(points[0])  # first point
    for i in range(1, pointsNumber):
        # compute the third order coefficients
        coefficients = []
        for j in range(2):
            coefficients.append([
                (-points[i - 2][j] + 3 * points[i - 1][j] - 3 * points[i][j] + points[i + 1][j]) / 6.0,
                (3 * points[i - 2][j] - 6 * points[i - 1][j] + 3 * points[i][j]) / 6.0,
                (-3 * points[i - 2][j] + 3 * points[i][j]) / 6.0,
                (points[i - 2][j] + 4 * points[i - 1][j] + points[i][j]) / 6.0
            ])
        for j in range(subdivision):
            t = float(j + 1) / subdivision
            spline.append(((coefficients[0][2] + t * (coefficients[0][1] + t * coefficients[0][0])) * t + coefficients[0][3],
                           (coefficients[1][2] + t * (coefficients[1][1] + t * coefficients[1][0])) * t + coefficients[1][3]))
    return spline


def hide_sensors_visualization(supervisor):
    """Hide all the sensors visualization nodes to the camera."""
    vehicleNode = supervisor.getFromDef(VEHICLE_DEF_NAME)
    slotNames = [
        'sensorsSlotFront',
        'sensorsSlotRear',
        'sensorsSlotTop',
        'sensorsSlotCenter'
    ]
    renderingDevicesTypeName = [
        'Camera',
        'RangeFinder',
        'Lidar'
    ]
    renderingDevicesNodes = []
    # look for rendering devices nodes in slot fields
    for slotName in slotNames:
        field = vehicleNode.getField(slotName)
        for i in range(field.getCount()):
            node = field.getMFNode(i)
            if node.getBaseTypeName() in renderingDevicesTypeName:
                renderingDevicesNodes.append(node)
    # look for visualization nodes
    field = supervisor.getFromDef('SENSOR_VISUALIZATION').getField('children')
    for i in range(field.getCount()):
        node = field.getMFNode(i)
        if field.getMFNode(i).getTypeName() == 'DistanceSensorVisualization':
            sensorVisualizationNodes.append(node)
    # hide visualization nodes to rendering devices
    for renderingDevicesNode in renderingDevicesNodes:
        for sensorsVisualizationNode in sensorVisualizationNodes:
            sensorsVisualizationNode.setVisibility(renderingDevicesNode, False)


def enable_sensors_visualization(supervisor, enable):
    """Hide or show the sensors visualization nodes."""
    for node in sensorVisualizationNodes:
        node.setVisibility(supervisor.getFromDef('VIEWPOINT'), enable)


def add_sumo(supervisor):
    """Add the SUMO interface with a free port (in the range [1024;1998]) as argument."""
    freePortFound = False
    port = 8873
    while not freePortFound:
        port = random.randint(SUMO_PORTS_RANGE[0], SUMO_PORTS_RANGE[1])

        # check if this port is already used
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(3.0)
        try:
            s.connect(('localhost', port))
        except socket.error:
            freePortFound = True
        s.close()
    nodeString = 'SumoInterface {'
    nodeString += '  gui FALSE'
    nodeString += '  useNetconvert FALSE'
    nodeString += '  radius 200'
    nodeString += '  laneChangeDelay 4'
    nodeString += '  enableWheelsRotation TRUE'
    nodeString += '  port %d' % port
    nodeString += '  seed 0'  # equivalent to 'random' for SUMO
    nodeString += '}'
    supervisor.getRoot().getField('children').importMFNodeFromString(-1, nodeString)


supervisor = Supervisor()
hide_sensors_visualization(supervisor)
add_sumo(supervisor)

timestep = int(supervisor.getBasicTimeStep())

# get main vehicle node and position
vehicleNode = supervisor.getFromDef(VEHICLE_DEF_NAME)
initialPosition = vehicleNode.getPosition()
initialPoint = Point((initialPosition[0], initialPosition[2]))

# get first SUMO vehicle node and position
sumoFirstVehicle = supervisor.getFromDef("SUMO_VEHICLE0")
sumoFirstVehicleInitialPosition = sumoFirstVehicle.getPosition()
sumoInitializationChecked = False

# parse the road node to extract road path
roadNode = supervisor.getFromDef("ROAD")
coordinates = []
splineSubdivision = roadNode.getField("splineSubdivision").getSFInt32()
waypoints = roadNode.getField("wayPoints")
laneNumber = roadNode.getField("numberOfLanes").getSFInt32()
width = roadNode.getField("width").getSFFloat()
laneWidth = width / laneNumber
for i in range(waypoints.getCount()):
    point = waypoints.getMFVec3f(i)
    coordinates.append((point[0], point[2]))
subdividedCoordinates = coordinates
if splineSubdivision > 0:
    subdividedCoordinates = apply_spline_subdivison_to_path(coordinates, splineSubdivision)
roadPath = LineString(subdividedCoordinates)
emergencyLanePath = roadPath.parallel_offset(1.5 * laneWidth, 'left')
initialDistance = roadPath.project(initialPoint)

# main loop
inEmergencyLane = False
collided = False
sumoFailure = False
time = 0.0
while time < MAX_TIME and not inEmergencyLane and not collided and not sumoFailure and supervisor.step(timestep) != -1:
    # check that SUMO is running
    if time >= 10.0 and not sumoInitializationChecked:
        sumoInitializationChecked = True
        position = sumoFirstVehicle.getPosition()
        # check if first sumo vehicle has moved
        if (position[0] == sumoFirstVehicleInitialPosition[0] and
                position[1] == sumoFirstVehicleInitialPosition[1] and
                position[2] == sumoFirstVehicleInitialPosition[2]):
            sumoFailure = True
    # check if robot is inside the emergency lane
    position = vehicleNode.getPosition()
    positionPoint = Point((position[0], position[2]))
    if emergencyLanePath.distance(positionPoint) < 0.5 * laneWidth:
        inEmergencyLane = True
    # check for collision
    contactPoints = vehicleNode.getContactPoints()
    if len(contactPoints) > 0:
        collided = True
    time = supervisor.getTime()
    distance = roadPath.project(positionPoint) - initialDistance
    supervisor.wwiSendText("update: " + str(time) + " {0:.3f}".format(distance))
    # check if sensors visualization should be enabled/disabled
    message = supervisor.wwiReceiveText()
    if message and message.startswith("sensors visualization:"):
        if message.endswith("false"):
            enable_sensors_visualization(supervisor, False)
        elif message.endswith("true"):
            enable_sensors_visualization(supervisor, True)

supervisor.wwiSendText("stop")

if inEmergencyLane:
    supervisor.setLabel(0, "The vehicle should not enter in the emergency lane.",
                        0.01, 0.85, 0.09, 0xFF0000, 0, "Lucida Console")
elif collided:
    supervisor.setLabel(0, "The vehicle should not collide.",
                        0.01, 0.85, 0.09, 0xFF0000, 0, "Lucida Console")
elif sumoFailure:
    supervisor.setLabel(0, "Problem with traffic generation using SUMO, we can't take this performance into account.",
                        0.01, 0.85, 0.09, 0xFF0000, 0, "Lucida Console")

position = vehicleNode.getPosition()
distance = round(roadPath.project(Point((position[0], position[2]))) - initialDistance, 3)

while supervisor.step(timestep) != -1:
    # wait for record message
    message = supervisor.wwiReceiveText()
    if message:
        if message.startswith("record:"):
            record = robotbenchmarkRecord(message, "highway_driving", distance)
            supervisor.wwiSendText(record)
            break
        elif message == "exit":
            break

supervisor.simulationSetMode(Supervisor.SIMULATION_MODE_PAUSE)
