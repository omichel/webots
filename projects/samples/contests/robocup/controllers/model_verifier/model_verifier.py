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

from controller import Supervisor
import trimesh
import traceback
import transforms3d
import math
import numpy as np
import trimesh.viewer
import trimesh.creation
import trimesh.boolean
import json
import time
import os
import itertools
import sys
import pandas as pd
import textwrap

JOINT_TYPES = ["HingeJoint", "HingeJointWithBacklash", "Hinge2Joint", "Hinge2JointWithBacklash"]
VALID_BOUNDING_OBJ_TYPES = ["Box", "Cylinder", "Sphere", "Capsule"]
FORBIDDEN_BOUNDING_OBJ_TYPES = ["IndexedFaceSet", "Mesh", "ElevationGrid", "Plane"]

# Nodes that should be allowed but are not implemented in the model verifier
UNSUPPORTED_NODES = ["SliderJoint", "LinearMotor"]
# Nodes that are forbidden by the rules
FORBIDDEN_NODES = ["Compass", "Display", "DistanceSensor", "GPS", "Lidar", "InertialUnit", "RangeFinder", "Propeller"]
MIN_BACKLASH = 0.01    # [rad]
MAX_SPAWN_TIME = 10.0  # [sec]
MAX_GYRO_VALUES = 10 * 2 * math.pi  # [rad/s], trigger warning if values are above this

# Visualization palette for bounding objects
DEFAULT_COLOR = [0.5, 0.5, 0.5]
ARM_COLOR = [0.8, 0.5, 0.0]
HAND_COLOR = [0.0, 0.5, 0.4]
FOOT_COLOR = [0.0, 0.0, 0.0]
FAKE_HAND_NODE = ("SFNode", {"__type": "Sphere", "radius": ("SFFloat", 0.01)})

# Giving a classical name to avoid robot invalid proto when spawned without
# color or id
ROBOT_NAME = "RED_PLAYER_1"
WARNINGS = []
ERRORS = []
MAX_LINE_WIDTH = 120

# Values initialized at the beginning of script
MODEL_NAME = None
ROBOT_PATH = None
ROBOT_DIR = None
DISPLAY_ENABLED = True
log_file = None
controller_start = None


def log(message, msg_type):
    global ERRORS, WARNINGS
    print(message, file=sys.stderr if msg_type == 'Error' else sys.stdout)
    if log_file:
        real_time = time.time() - controller_start
        log_file.write(f"[{real_time:08.3f}] {msg_type}: {message}\n")
    if msg_type == "Error":
        ERRORS += [message]
    elif msg_type == "Warning":
        WARNINGS += [message]


def info(message):
    log(message, 'Info')


def warning(message):
    log(message, 'Warning')


def error(message, fatal=False):
    log(message, 'Error')
    if fatal:
        s.simulationQuit(-1)


def spawn_robot():
    """Spawn and returns the robot that should be verified"""
    info("Spawning robot")
    string = f'DEF {ROBOT_NAME} {MODEL_NAME}' \
        '{name "red player 1" translation 0 0 0 rotation 0 0 1 0 controller "void"}'
    s.getRoot().getField('children').importMFNodeFromString(-1, string)
    return s.getFromDef(ROBOT_NAME)


def get_node_desc(node):
    # TODO ideally the node path should be provided but it's unclear if that can still be accessed from the dictionary
    # represenation
    node_type = node[1].get("__type")
    node_name = node[1].get("name", "unknown")
    return f"(name: {node_name}, type: {node_type})"


def build_dict_field(field):
    """

    :type field: Field
    """
    if field is None:
        return None

    type_name = field.getTypeName()
    value_s = None
    if type_name == "SFBool":
        value_s = field.getSFBool()
    elif type_name == "SFInt32":
        value_s = field.getSFInt32()
    elif type_name == "SFFloat":
        value_s = field.getSFFloat()
    elif type_name == "SFVec2f":
        value_s = field.getSFVec2f()
    elif type_name == "SFVec3f":
        value_s = field.getSFVec3f()
    elif type_name == "SFRotation":
        value_s = field.getSFRotation()
    elif type_name == "SFColor":
        value_s = field.getSFColor()
    elif type_name == "SFString":
        value_s = field.getSFString()
    elif type_name == "SFNode":
        value_s = build_dict_node(field.getSFNode())
    elif type_name == "MFNode":
        vals = []
        for i in range(field.getCount()):
            vals.append(build_dict_node(field.getMFNode(i)))
        value_s = vals
    elif EXPORT_MF:
        if type_name == "MFBool":
            vals = []
            for i in range(field.getCount()):
                vals.append(field.getMFBool(i))
            value_s = vals
        elif type_name == "MFInt32":
            vals = []
            for i in range(field.getCount()):
                vals.append(field.getMFInt32(i))
            value_s = vals
        elif type_name == "MFFloat":
            vals = []
            for i in range(field.getCount()):
                vals.append(field.getMFFloat(i))
            value_s = vals
        elif type_name == "MFVec2f":
            vals = []
            for i in range(field.getCount()):
                vals.append(field.getMFVec2f(i))
            value_s = vals
        elif type_name == "MFVec3f":
            vals = []
            for i in range(field.getCount()):
                vals.append(field.getMFVec3f(i))
            value_s = vals
        elif type_name == "MFColor":
            vals = []
            for i in range(field.getCount()):
                vals.append(field.getMFColor(i))
            value_s = vals
        elif type_name == "MFRotation":
            vals = []
            for i in range(field.getCount()):
                vals.append(field.getMFRotation(i))
            value_s = vals
        elif type_name == "MFString":
            vals = []
            for i in range(field.getCount()):
                vals.append(field.getMFString(i))
            value_s = vals
        else:
            warning(f"type {type_name} not known")

    return type_name, value_s


def build_dict_node(node):
    """
    :param node: Node
    :return:
    """
    if node is None:
        return {}
    local_fields = {}
    local_fields["__type"] = node.getTypeName()
    nb_fields = node.getProtoNumberOfFields()
    for i in range(nb_fields):
        field = node.getProtoFieldByIndex(i)
        if field is None:
            error(f"None field reached {i+1}/{nb_fields} in {get_node_desc(('SFNode',local_fields))}\n")
            continue
        local_fields[field.getName()] = build_dict_field(field)
    return local_fields


def check_self_collision(robot):
    self_collision_node = robot.get("selfCollision")
    if self_collision_node is None:
        error("Robot has no field selfCollision\n")
    elif not self_collision_node[1]:
        error("Self-collision is disabled\n")


def check_custom_data(robot):
    custom_data_node = robot.get("customData")
    if custom_data_node is None:
        error("Robot has no field customData\n")
    elif custom_data_node[0] != "SFString":
        error(f"CustomData has invalid type {custom_data_node[0]}, while expecting SFString\n")
    else:
        info("Robot has valid customData field")


def check_nodes_support(node):
    """Add errors if some nodes are unsupported or forbidden"""
    if node[0] == "SFNode":
        node_type = node[1].get("__type")
        if node_type in UNSUPPORTED_NODES:
            warning(f"Node {get_node_desc(node)} is allowed, but not supported, contact technical committee\n")
        if node_type in FORBIDDEN_NODES:
            warning(f"Node {get_node_desc(node)} is forbidden\n")
        for k, v in node[1].items():
            check_nodes_support(v)
    elif node[0] == "MFNode":
        for i, child_node in enumerate(node[1]):
            check_nodes_support(("SFNode", child_node))


def get_backlash(node):
    if "Backlash" not in node[1].get("__type"):
        error(f"Tried to get backlash in an invalid node {get_node_desc(node)}\n")
        return None
    end_point = node[1]["endPoint"]
    fake_joint_node = end_point[1]['children'][1]
    joint_parameters = fake_joint_node[1]['jointParameters'][1]
    return joint_parameters["maxStop"][1] - joint_parameters["minStop"][1]


def get_device_properties(node, device_index=1):
    """Return a tuple with (motor_device, resolution, joint_parameters)

    Parameters
    ----------
    node: (str, node)
        The joint node that contains the device to extract properties from
    index: int
        Use 1 for device and 2 for device2
    """
    if device_index < 1 or device_index > 2:
        raise RuntimeError(f"Invalid device_index: {device_index}")
    motor_device = None
    resolution = -1
    dev_key = "device"
    jp_key = "jointParameters"
    pos_key = "position"
    if device_index == 2:
        dev_key += "2"
        jp_key += "2"
        pos_key += "2"
    joint_position = node[1][pos_key][1]
    for field in node[1][dev_key][1]:
        if field["__type"] == "RotationalMotor":
            motor_device = field
        if field["__type"] == "PositionSensor" and field.get("resolution") is not None:
            resolution = field.get("resolution")[1]
    return (motor_device, resolution, node[1][jp_key][1], joint_position)


def get_devices(node):
    """Return the list of devices in the node with support for Hinge2Joint and Hinge2JointWithBacklash

    See get_device_properties
    """
    devices = []
    devices += [get_device_properties(node)]
    if node[1].get("device2") is not None:
        devices += [get_device_properties(node, 2)]
    return devices


def get_joints_list(node):
    """Perform a depth-first search to identify all the joints in the robot and returns a dictionary with there properties

    The dictionary has the following structure:
    - key are joints:
    - values are dictionary with the following properties:
      - jointType
      - maxTorque
      - maxVelocity
      - maxForce
      - springConstant
      - staticFriction
      - dampingConstant
      - backlash
    """
    joints = []
    if node[0] == "SFNode":
        node_type = node[1].get("__type")
        if node_type in JOINT_TYPES:
            devices = get_devices(node)
            for device in devices:
                motor_device = device[0]
                resolution = device[1]
                joint_parameters = device[2]
                if motor_device is None:  # Ignore passive joints
                    continue
                motor_name = motor_device["name"][1]
                motor_entry = {
                    "jointName": motor_name,
                    "jointType": node_type,
                    "maxTorque": "",
                    "maxVelocity": "",
                    "springConstant": "",
                    "staticFriction": "",
                    "dampingConstant": "",
                    "backlash": "",
                    "resolution": resolution
                }
                for joint_parameter in ["dampingConstant", "staticFriction", "springConstant"]:
                    if joint_parameter in joint_parameters:
                        motor_entry[joint_parameter] = joint_parameters[joint_parameter][1]
                for motor_parameter in ["maxTorque", "maxVelocity"]:
                    if motor_parameter in motor_device:
                        motor_entry[motor_parameter] = motor_device[motor_parameter][1]
                if resolution <= 0:
                    error(f"Sensor with infinite resolution in node {get_node_desc(node)}\n")
                if "Backlash" in node_type:
                    motor_entry["backlash"] = get_backlash(node)
                    if motor_entry["backlash"] is None:
                        error(f"Failed to find backlash in node {get_node_desc(node)}\n")
                    elif motor_entry["backlash"] < MIN_BACKLASH:
                        error(f"Backlash value ({motor_entry['backlash']}) below minimum ({MIN_BACKLASH})"
                              f"{get_node_desc(node)}\n")
                joints += [motor_entry]
        for k, v in node[1].items():
            joints += get_joints_list(v)
    elif node[0] == "MFNode":
        for i, child_node in enumerate(node[1]):
            joints += get_joints_list(("SFNode", child_node))
    return joints


def fill_sensors_information(node, dic={"Gyro": [], "Accelerometer": [], "TouchSensor": [], "Camera": []}):
    """Update dic with the sensors information from the given node and all its descendants"""
    if node[0] == "SFNode":
        node_type = node[1].get("__type")
        if node_type in ["Gyro", "Accelerometer", "TouchSensor"]:
            lut_node = node[1].get("lookupTable")
            entry = {"name": node[1].get("name")[1], "lutEntries": None, "minValue": None, "maxValue": None}
            if node_type == "TouchSensor":
                entry["type"] = node[1].get("type")[1]
            if lut_node is not None and len(lut_node[1]) > 0:
                entry['lutEntries'] = len(lut_node[1])
                entry['minValue'] = lut_node[1][0][0]
                entry['maxValue'] = lut_node[1][-1][0]
                if node_type == "Gyro":
                    if entry['minValue'] < -MAX_GYRO_VALUES:
                        warning(f"Surprisingly low min value for Gyro: {entry['minValue']} [rad/s]")
                    if entry['maxValue'] > MAX_GYRO_VALUES:
                        warning(f"Surprisingly high max value for Gyro: {entry['maxValue']} [rad/s]")
            else:
                error(f"No LUT specified for sensor: {get_node_desc(node)}")
            dic[node_type] += [entry]
        if node_type == "Camera":
            entry = {
                "name": node[1].get("name")[1],
                "width": node[1].get("width")[1],
                "height": node[1].get("height")[1],
                "fieldOfView": node[1].get("fieldOfView")[1],
                "spherical": node[1].get("spherical")[1],
                "noise": node[1].get("noise")[1],
                "radialCoefficients": None,
                "tangentialCoefficients": None
            }
            lens_node = node[1].get("lens")
            if lens_node is not None:
                for key in ["radialCoefficients", "tangentialCoefficients"]:
                    coeff_node = lens_node[1].get(key)
                    if coeff_node is not None:
                        entry[key] = coeff_node[1]
            dic[node_type] += [entry]
        for k, v in node[1].items():
            fill_sensors_information(v, dic)
    elif node[0] == "MFNode":
        for i, child_node in enumerate(node[1]):
            fill_sensors_information(("SFNode", child_node), dic)
    return dic


def check_bounding_objects(node, inside_bounding_object=False, parent_name=None):
    """Test if all bounding objects are made of geometric primitives"""
    if node[0] == "SFNode":
        node_type = node[1].get("__type")
        if node_type == "Shape":
            return
        elif node_type == "Solid":
            parent_name = node[1].get("name")
        elif node_type == "BoundingObject":
            inside_bounding_object = True
        elif inside_bounding_object:
            if node_type in FORBIDDEN_BOUNDING_OBJ_TYPES:
                error(f"Invalid BoundingObject of solid {parent_name}: {get_node_desc(node)}\n")
            elif node_type in VALID_BOUNDING_OBJ_TYPES:
                info(f"Valid BoundingObject of solid {parent_name}: {get_node_desc(node)}")
        for k, v in node[1].items():
            if k == "boundingObject":
                check_bounding_objects(v, True, parent_name)
            else:
                check_bounding_objects(v, inside_bounding_object, parent_name)
    elif node[0] == "MFNode":
        for i, child_node in enumerate(node[1]):
            check_bounding_objects(("SFNode", child_node), inside_bounding_object, parent_name)


def replace_primitives_with_indexed_face_set(node):
    if node[0] == "SFNode":
        node_type = node[1].get("__type")
        if node_type == "Box":
            node = box_to_proto_dict(node[1])
        elif node_type == "Cylinder":
            node = cylinder_to_proto_dict(node[1])
        elif node_type == "Sphere":
            node = sphere_to_proto_dict(node[1])
        elif node_type == "Capsule":
            node = capsule_to_proto_dict(node[1])
        else:
            # In case an empty solid was created for the hand, add a fake node
            if node_type == "Solid":
                name = node[1].get("name")[1]
                if name is not None and "[hand]" in name:
                    nb_children = len(node[1]["children"][1])
                    nb_bounding_objects = len(node[1]["boundingObject"][1])
                    if nb_children == 0 and nb_bounding_objects == 0:
                        node[1]["boundingObject"] = FAKE_HAND_NODE
            for k, v in node[1].items():
                node[1][k] = replace_primitives_with_indexed_face_set(v)
        # in case that we have found a primitive, we replace node[1] with an indexed face set
        # check type of each field, if sfnode or mf node pass recursively
    elif node[0] == "MFNode":
        for i, child_node in enumerate(node[1]):
            child_node_type, new_child_node = replace_primitives_with_indexed_face_set(("SFNode", child_node))
            node[1][i] = new_child_node
    return node


def cylinder_to_proto_dict(cylinder):
    cylinder_offset = np.identity(4)
    cylinder_offset[:3, :3] = transforms3d.euler.euler2mat(0.5 * np.pi, 0, 0)
    # todo wired webots rotation because z is not up
    mesh = trimesh.creation.cylinder(cylinder["radius"][1], cylinder["height"][1], transform=cylinder_offset)
    return mesh_to_indexed_face_set(mesh)


def box_to_proto_dict(box):
    mesh = trimesh.creation.box(box["size"][1])
    return mesh_to_indexed_face_set(mesh)


def sphere_to_proto_dict(sphere):
    mesh = trimesh.creation.icosphere(1, sphere["radius"][1])
    return mesh_to_indexed_face_set(mesh)


def capsule_to_proto_dict(capsule):
    capsule_offset = np.identity(4)
    capsule_offset[:3, :3] = transforms3d.euler.euler2mat(0.5 * np.pi, 0, 0)
    # In webots, height is along 'y-axis', in trimesh it's along 'z-axis'
    mesh = trimesh.creation.capsule(capsule["height"][1], capsule["radius"][1])
    mesh.apply_transform(capsule_offset)
    return mesh_to_indexed_face_set(mesh)


def mesh_to_indexed_face_set(mesh):
    coordIndex = []
    for face in mesh.faces:
        coordIndex.extend(face)
        coordIndex.append(-1)
    coordinates = []
    for vert in mesh.vertices:
        coordinates.append(vert)
    return "SFNode", \
           {"__type": "IndexedFaceSet",
            "coord": ("SFNode", {"__type": "Coordinate", "point": ("MFVec3f", coordinates)}),
            "coordIndex": ("MFInt32", coordIndex)}


def get_bounding_bot(node, transform_list=[], inside_bounding_object=False, parentSolidName=None, active_tag=None):
    bounding_objects = []
    if inside_bounding_object and node[0] == "SFNode" and node[1].get("__type") == "IndexedFaceSet":
        vertices = node[1]["coord"][1]["point"][1]
        faces_matrix = []
        faces_list = node[1]["coordIndex"][1]
        for i in range(0, len(faces_list), 4):
            faces_matrix.append([faces_list[i + 0], faces_list[i + 1], faces_list[i + 2]])
            # todo this should look for the end of face marker instead of assume that it is three
        color = DEFAULT_COLOR
        if 'arm' == active_tag:
            color = ARM_COLOR
        if 'foot' == active_tag:
            color = FOOT_COLOR
        if 'hand' == active_tag:
            color = HAND_COLOR
        mesh = trimesh.Trimesh(vertices=vertices, faces=faces_matrix, face_colors=color)
        bounding_objects.append([mesh, transform_list[:], parentSolidName])
    else:
        if node[0] == "SFNode":
            translation = node[1].get("translation")
            rotation = node[1].get("rotation")
            if translation is not None:
                transform_list.append((translation[1], rotation[1]))

            node_type = node[1].get("__type")
            if node_type in JOINT_TYPES:
                active_tag = None
                devices = get_devices(node)
                for device in devices:
                    motor_device = device[0]
                    joint_parameters = device[2]
                    joint_position = device[3]
                    motor_name = None
                    anchor = [0, 0, 0]  # For Hinge2Joint, the second joint has no anchor
                    if "anchor" in joint_parameters:
                        anchor = joint_parameters["anchor"][1]
                    if motor_device is not None:
                        motor_name = motor_device["name"][1]
                    transform_list.append((node_type, anchor, joint_parameters["axis"][1], joint_position, motor_name))
            name_field = node[1].get('name')
            if name_field is not None:
                name = name_field[1]
                tag_start = name.rfind('[')
                tag_end = name.rfind(']')
                if tag_start != -1 and tag_end != -1:
                    active_tag = name[tag_start+1:tag_end]
            bounding_object = node[1].get("boundingObject")
            if bounding_object is not None:
                bounding_objects.extend(get_bounding_bot(bounding_object, transform_list[:], True, node[1]["name"], active_tag))

            for k, v in node[1].items():
                bounding_objects.extend(get_bounding_bot(v, transform_list[:], inside_bounding_object, parentSolidName,
                                                         active_tag))
        elif node[0] == "MFNode":
            for i, child_node in enumerate(node[1]):
                bounding_objects.extend(get_bounding_bot(("SFNode", child_node), transform_list[:],
                                                         inside_bounding_object, parentSolidName, active_tag))

    return bounding_objects


def get_foot_bounding_objects(bounding_objects):
    """Filter the provided bounding objects and only return those belonging to the first foot"""
    # TODO: if multiple solids are used for the BoundingObject of one foot, this seems likely to be invalid
    # An easy check might be to return a dictionary of list of bounding objects and to assert that its size is 2
    result = []
    foot_name = None
    for bo in bounding_objects:
        parent_name = bo[2][1]
        if "[foot]" in parent_name and foot_name is None:  # only extract one of the feet, we assume both to be the same
            foot_name = parent_name
        if parent_name == foot_name:
            result += [bo]
    return result


def joint_and_tf_list_to_single_tf(joint_and_tf_list, joint_poses):
    """Returns the transform after setting the angles to dynamic transformations

    :param joint_and_tf_list: list
    :param joint_poses: dict
    :return: list
    """
    tf_list = []
    for i in range(len(joint_and_tf_list)):
        if type(joint_and_tf_list[i][0]) == list:  # static tf
            translation = joint_and_tf_list[i][0]
            rotation_axangle = joint_and_tf_list[i][1]
            rotation_mat = transforms3d.axangles.axangle2mat(axis=rotation_axangle[:3], angle=rotation_axangle[3])
            affine = transforms3d.affines.compose(translation, rotation_mat, [1, 1, 1])
            tf_list.append(affine)
        elif type(joint_and_tf_list[i][0]) == str and "Hinge" in joint_and_tf_list[i][0]:  # angular joint
            joint_origin = np.array(joint_and_tf_list[i][1])
            joint_axis = joint_and_tf_list[i][2]
            joint_position = joint_and_tf_list[i][3]
            joint_name = joint_and_tf_list[i][4]
            if joint_name is not None:  # skip fake hinge joints for hingejointwithbacklash since they dont have a motor name
                joint_rot_mat = transforms3d.axangles.axangle2mat(axis=joint_axis,
                                                                  angle=joint_poses[joint_name]-joint_position)
                new_affine2 = transforms3d.affines.compose(joint_origin, joint_rot_mat, [1, 1, 1])
                tf_list.append(new_affine2)
                new_affine = transforms3d.affines.compose(-joint_origin, np.identity(3), [1, 1, 1])
                tf_list.append(new_affine)

    final_affine = np.identity(4)
    for i in range(len(tf_list), 0, -1):  # todo maybe other way around
        final_affine = np.matmul(tf_list[i-1], final_affine)
    return final_affine


def find_joint_tag(node, joint_tag, transform_list=[]):
    joints = []
    if node[0] == "SFNode":
        translation = node[1].get("translation")
        rotation = node[1].get("rotation")
        if translation is not None:
            transform_list.append((translation[1], rotation[1]))
        node_type = node[1].get("__type")
        if node_type in JOINT_TYPES:
            motor_name = None
            for device in node[1]["device"][1]:
                if device["__type"] == "RotationalMotor":
                    motor_name = device["name"][1]
                    if joint_tag in motor_name:
                        translation = node[1]["jointParameters"][1]["anchor"][1]
                        rotation = [1, 0, 0, 0]
                        transform_list.append([translation, rotation])
                        return [transform_list]
            transform_list.append((node_type,
                                   node[1]["jointParameters"][1]["anchor"][1],
                                   node[1]["jointParameters"][1]["axis"][1],
                                   node[1]["position"][1],
                                   motor_name))
        for k, v in node[1].items():
            joints += find_joint_tag(v, joint_tag, transform_list[:])
    elif node[0] == "MFNode":
        for i, child_node in enumerate(node[1]):
            joints += find_joint_tag(("SFNode", child_node), joint_tag, transform_list[:])
    return joints


def find_solid_tag(node, solid_tag, transform_list=[]):
    """Returns a list of transform_list for all solids matching the given solid_tag"""
    res = []
    if node[0] == "SFNode":
        translation = node[1].get("translation")
        rotation = node[1].get("rotation")
        if translation is not None:
            transform_list.append((translation[1], rotation[1]))
        node_type = node[1].get("__type")
        if node_type in JOINT_TYPES:
            motor_name = None
            for device in node[1]["device"][1]:
                if device["__type"] == "RotationalMotor":
                    motor_name = device["name"][1]
            if 'position' not in node[1]:
                error(f"Missing position field in joint with motor {motor_name}: {get_node_desc(node)}\n")
            transform_list.append((node_type,
                                   node[1]["jointParameters"][1]["anchor"][1],
                                   node[1]["jointParameters"][1]["axis"][1],
                                   node[1]["position"][1],
                                   motor_name))
        if node_type == "Solid":
            name = node[1].get("name")
            if name is not None and solid_tag in name[1]:
                return [transform_list]
        for k, v in node[1].items():
            res += find_solid_tag(v, solid_tag, transform_list[:])
    elif node[0] == "MFNode":
        for i, child_node in enumerate(node[1]):
            res += find_solid_tag(("SFNode", child_node), solid_tag, transform_list[:])
    return res


def find_joint_pos(tag, posture):
    joints = find_joint_tag(("SFNode", robot), tag)
    positions = []
    for j in joints:
        affine = joint_and_tf_list_to_single_tf(j, posture)
        positions += [transforms3d.affines.decompose44(affine)[0]]
    return positions


def find_solid_pos(tag, posture):
    """Returns a list of position for all solids matching the given solid_tag"""
    solids = find_solid_tag(("SFNode", robot), tag)
    positions = []
    for s in solids:
        pos_affine = joint_and_tf_list_to_single_tf(s, posture)
        positions += [transforms3d.affines.decompose44(pos_affine)[0]]
    return positions


def check_structure(node):
    for tag in ["[hand]", "[foot]", "[arm]"]:
        nb_solids = len(find_solid_tag(node, tag))
        if nb_solids < 2 or tag == "[arm]" and nb_solids != 2:
            expected = "2 or more" if tag == "[arm]" else 2
            error(f"Invalid number of solids with tag {tag}, received {nb_solids} while expecting {expected}\n")
    for tag in ["[hip]", "[shoulder]"]:
        nb_joints = len(find_joint_tag(node, tag))
        if nb_joints != 2:
            expected = "2 or more" if tag == "[arm]" else 2
            error(f"Invalid number of joints with tag {tag}, received {nb_joints} expecting {expected}\n")


def compute_h_top(vertices):
    max_z = -9999
    min_z = +9999
    for vertex in vertices:
        if vertex[2] > max_z:
            max_z = vertex[2]
        if vertex[2] < min_z:
            min_z = vertex[2]
    return max_z - min_z


def compute_h_leg(robot, vertices):
    hip_positions = find_joint_pos("[hip]", postures["upright"])
    if len(hip_positions) == 0:
        error("Cannot compute hleg, because no joint is tagged [hip]\n")
        return None
    hip_pos = hip_positions[0]
    max_z_dist = 0
    for vertex in vertices:
        new_z_dist = hip_pos[2] - vertex[2]
        if new_z_dist > max_z_dist:
            max_z_dist = new_z_dist
    return max_z_dist


def compute_h_head(robot, vertices):
    shoulder_positions = find_joint_pos("[shoulder]", postures["upright"])
    if len(shoulder_positions) == 0:
        error("Cannot compute hhead, because no joint is tagged [shoulder]\n")
        return None
    shoulder_pos = shoulder_positions[0]
    max_z_dist = 0
    for vertex in vertices:
        new_z_dist = vertex[2] - shoulder_pos[2]
        if new_z_dist > max_z_dist:
            max_z_dist = new_z_dist
    return max_z_dist


def compute_arms_length(robot, vertices):
    # Since hand is required to be at the end of the arm, taking closest hand gives an appropriate value
    shoulder_positions = find_joint_pos("[shoulder]", postures["upright"])
    if len(shoulder_positions) == 0:
        error("Cannot compute armsLength, because no joint is tagged [shoulder]\n")
        return None
    shoulder_pos = shoulder_positions[0]
    hand_positions = find_solid_pos("[hand]", postures["upright"])
    if len(hand_positions) <= 0:
        error('No hands, cannot compute armsLength\n')
        return None
    return min([np.linalg.norm(h_pos - shoulder_pos) for h_pos in hand_positions])


def compute_physics(robot):
    """Returns mass, h_com"""
    physics_nodes = get_physics_nodes(("SFNode", robot))
    mass = 0
    com_absolute = np.array([0.0, 0.0, 0.0])

    for physics_node in physics_nodes:
        single_mass = physics_node[0]
        joint_and_tf = physics_node[1]
        if single_mass == -1:
            warning("error mass not defined, i am not dealing with density...")
        mass += single_mass
        single_center_of_mass_affine = joint_and_tf_list_to_single_tf(joint_and_tf, postures["upright"])
        single_com_translate, _, _, _ = transforms3d.affines.decompose44(single_center_of_mass_affine)
        com_absolute += (single_com_translate * single_mass)

    com_absolute = com_absolute/mass

    min_vertex_z = 9999
    for vertex in single_mesh_upright.vertices:
        if vertex[2] < min_vertex_z:
            min_vertex_z = vertex[2]

    h_com = com_absolute[2] - min_vertex_z
    return mass, h_com


def get_physics_nodes(node, transform_list=[],):
    """Return a list of tuples (mass, joint_and_tf, mass_name)"""
    physics_nodes = []
    if node[0] == "SFNode":
        translation = node[1].get("translation")
        rotation = node[1].get("rotation")
        if translation is not None:
            transform_list.append((translation[1], rotation[1]))

        if node[1].get("physics") is not None:
            physics = node[1]["physics"][1]
            if physics != {}:  # empty dict if null
                com = physics["centerOfMass"][1]
                if len(com) == 0:
                    name = node[1].get("name")
                    if name is None:
                        warning("Empty center of mass in unnamed node\n")
                    else:
                        warning(f"Empty center of mass in node {name}\n")
                else:
                    new_tf_list = transform_list[:]
                    new_tf_list.append((com[0], [0, 0, 1, 0]))
                    physics_nodes.append((physics["mass"][1], new_tf_list[:], node[1]["name"]))
        for mass_name in ['gearMass', 'gearMass2']:
            mass_node = node[1].get(mass_name)
            if mass_node is not None:
                physics_nodes.append((mass_node[1], transform_list[:], node[1]["name"]))
        node_type = node[1].get("__type")
        if node_type in JOINT_TYPES:
            motor_name = None
            for device in node[1]["device"][1]:
                if device["__type"] == "RotationalMotor":
                    motor_name = device["name"][1]

            transform_list.append((node_type,
                                   node[1]["jointParameters"][1]["anchor"][1],
                                   node[1]["jointParameters"][1]["axis"][1],
                                   node[1]["position"][1],
                                   motor_name))
        for k, v in node[1].items():
            physics_nodes.extend(get_physics_nodes(v, transform_list[:]))
    elif node[0] == "MFNode":
        for child_node in node[1]:
            physics_nodes.extend(get_physics_nodes(("SFNode", child_node), transform_list[:]))

    return physics_nodes


def find_max_and_min_yx(vertices):
    max_x = -9999
    min_x = 9999
    max_y = -9999
    min_y = 9999
    for v in vertices:
        max_x = max(max_x, v[0])
        min_x = min(min_x, v[0])
        max_y = max(max_y, v[1])
        min_y = min(min_y, v[1])
    return max_x, min_x, max_y, min_y


def compute_cylinder_diameter(vertices):
    """Returns the width of the robot"""
    # TODO: This implementation is an approximation
    # Easy solution: replace coordinaters of the vertices along z-axis by 0 and then use get_max_dist
    max_x, min_x, max_y, min_y = find_max_and_min_yx(vertices)
    width_x = max_x-min_x
    width_y = max_y-min_y
    width = max(width_x, width_y)
    info(f"robot is wider in {'x' if width_x>width_y else 'y'} direction\n")
    return width


def compute_foot_size():
    foot_bounds = single_mesh_foot.bounding_box.bounds
    foot_length = - foot_bounds[0][0] + foot_bounds[1][0]
    foot_width = - foot_bounds[0][1] + foot_bounds[1][1]
    return foot_length, foot_width


def compute_max_dist(vertices):
    """Return the maximal distance between the vertices in parameters"""
    # calculate from convex hull since convex hull is n*log(n) and max dist is n*n
    max_vert_distance = 0
    ch_verts = single_mesh_extended.convex_hull.vertices
    for i, (v1, v2) in enumerate(itertools.combinations(ch_verts, 2)):
        current_distance = np.linalg.norm(v1-v2)
        max_vert_distance = max(current_distance, max_vert_distance)
    return max_vert_distance


def get_single_mesh(bounding_objects, posture):
    """Return a mesh based on the bounding_objects given the posture or None if no bounding_objects are provided"""
    if len(bounding_objects) == 0:
        return None
    scene = trimesh.scene.Scene()
    for bo in bounding_objects:
        mesh = bo[0]
        tf_list = bo[1]
        composed_tf = joint_and_tf_list_to_single_tf(tf_list, posture)
        scene.add_geometry(mesh, transform=composed_tf)
    return trimesh.util.concatenate(scene.dump())


def compute_robot_properties():
    h_top = None
    h_leg = None
    h_head = None
    foot_length = None
    foot_width = None
    max_length = None
    mass, h_com = compute_physics(robot)
    if single_mesh_upright is not None:
        h_top = compute_h_top(single_mesh_upright.vertices)
        h_leg = compute_h_leg(robot, single_mesh_upright.vertices)
        h_head = compute_h_head(robot, single_mesh_upright.vertices)
        arms_length = compute_arms_length(robot, single_mesh_upright.vertices)
        bmi = mass/(h_top**2)
        diameter = compute_cylinder_diameter(single_mesh_upright.vertices)
    if single_mesh_foot is not None:
        foot_length, foot_width = compute_foot_size()
    if single_mesh_extended is not None:
        max_length = compute_max_dist(single_mesh_extended.vertices)
    measurements = [
        {"name": "htop", "unit": "[m]", "value": h_top},
        {"name": "hleg", "unit": "[m]", "value": h_leg},
        {"name": "hhead", "unit": "[m]", "value": h_head},
        {"name": "armsLength", "unit": "[m]", "value": arms_length},
        {"name": "hcom", "unit": "[m]", "value": h_com},
        {"name": "weight", "unit": "[kg]", "value": mass},
        {"name": "BMI", "unit": "ul", "value": bmi},
        {"name": "fittingCylinderDiameter", "unit": "[m]", "value": diameter},
        {"name": "footLength", "unit": "[m]", "value": foot_length},
        {"name": "footWidth", "unit": "[m]", "value": foot_width},
        {"name": "longestPossibleLength", "unit": "[m]", "value": max_length}
    ]
    return pd.DataFrame(measurements)


joints_list = None
robot_properties = None
sensors_dictionaries = None
EXPORT_MF = True
s = Supervisor()
log_file = open('model_verifier.log', 'w')
controller_start = time.time()
try:
    for required_variable in ["ROBOT_NAME", "ROBOT_PATH"]:
        if required_variable not in os.environ:
            raise RuntimeError(f"Environment variable {required_variable} is missing")
    MODEL_NAME = os.environ["ROBOT_NAME"]
    ROBOT_PATH = os.environ["ROBOT_PATH"]
    ROBOT_DIR = os.path.dirname(ROBOT_PATH)
    POSTURES_PATH = os.path.join(ROBOT_DIR, "postures.json")
    DISPLAY_ENABLED = False if "MODEL_VERIFIER_NO_DISPLAY" in os.environ else True

    try:
        with open(POSTURES_PATH) as f:
            postures = json.load(f)
    except FileNotFoundError:
        error(f"Posture file is not at expected location: {POSTURES_PATH}\n")
        postures = None

    before_spawning = time.time()
    robot_node = spawn_robot()
    spawn_time = time.time() - before_spawning
    if spawn_time > MAX_SPAWN_TIME:
        error(f"Model is too complex to be spawn in reasonable time: {spawn_time}>{MAX_SPAWN_TIME}\n")
    else:
        info(f"Model is spawn in reasonable time: {spawn_time}<={MAX_SPAWN_TIME}\n")
    if robot_node is None:
        raise RuntimeError(f"Failed to spawn robot {MODEL_NAME}")

    info("Loading Robot Model.... this might take a while")
    robot = build_dict_node(robot_node)
    check_self_collision(robot)
    check_custom_data(robot)
    check_nodes_support(("SFNode", robot))
    check_bounding_objects(("SFNode", robot))
    check_structure(("SFNode", robot))
    joints_list = get_joints_list(("SFNode", robot))
    sensors_dictionaries = fill_sensors_information(("SFNode", robot))
    meshed_robot = replace_primitives_with_indexed_face_set(("SFNode", robot))
    bounding_objects = get_bounding_bot(meshed_robot)
    if len(bounding_objects) == 0:
        error("No valid bounding object found for robot\n")
        foot_bounding_objects = []
    else:
        foot_bounding_objects = get_foot_bounding_objects(bounding_objects)
        if len(foot_bounding_objects) == 0:
            error("No valid bounding object found for foot\n")

    if postures is not None:
        single_mesh_upright = get_single_mesh(bounding_objects, postures["upright"])
        single_mesh_extended = get_single_mesh(bounding_objects, postures["extension"])
        single_mesh_foot = get_single_mesh(foot_bounding_objects, postures["upright"])
        if (DISPLAY_ENABLED):
            if single_mesh_upright is not None:
                single_mesh_upright.show()
            if single_mesh_extended is not None:
                single_mesh_extended.show()
        robot_properties = compute_robot_properties()
except Exception:
    error(f"Failed execution of model verifier with error:\n{traceback.format_exc()}\n")

try:
    if robot_properties is not None:
        prop_dic = {
            "robot": MODEL_NAME,
            "markersFront": 1000,
            "markersBack": 1000,
            "markersLeft": 1000,
            "markersRight": 1000,
            "canStandUp": 0,
            "canStandUpBack": 0,
            "emergencyButton": 1,
            "customReject": len(ERRORS) > 0,
            "comment": '\n'.join(ERRORS)
        }
        for index, row in robot_properties.iterrows():
            v = row["value"]
            # Convert to cm for robot_inspection tool
            if v is None or math.isnan(float(v)):
                v = 0.001  # If value is set to 0, import fails
            elif row["unit"] == "[m]":
                v *= 100
            prop_dic[row["name"]] = v
        with open("robot_properties.json", "w") as json_file:
            json.dump(prop_dic, json_file)
except Exception:
    error(f"Failed while writing robot_properties:\n{traceback.format_exc()}\n")


try:
    with open("report.md", "w") as f:
        f.write(f"---\ntitle: Semi-automated review of {MODEL_NAME}\n---\n")
        if len(ERRORS) > 0:
            f.write("\n# Errors\n```\n")
            for e in ERRORS:
                f.write('\n'.join(textwrap.wrap(f"{e}", MAX_LINE_WIDTH, replace_whitespace=False)) + "\n")
            f.write("```\n")
        if len(WARNINGS) > 0:
            f.write("\n# Warnings\n```\n")
            for w in WARNINGS:
                f.write('\n'.join(textwrap.wrap(f"{w}", MAX_LINE_WIDTH, replace_whitespace=False)) + "\n")
            f.write("```\n")
        if joints_list is not None:
            joint_properties = pd.DataFrame(joints_list).sort_values(["maxTorque", "jointName"])
            f.write("\n# Joint properties\n")
            f.write("\\tiny\n")
            f.write(joint_properties.to_markdown(index=False, tablefmt="pipe", floatfmt=".03f"))
            f.write("\n\\normalsize\n")
        if sensors_dictionaries is not None and len(sensors_dictionaries) > 0:
            f.write("\n# Sensors\n\n")
            for sensor_type, sensor_information in sensors_dictionaries.items():
                if len(sensor_information) == 0:
                    continue
                f.write(f"## {sensor_type}\n\n")
                sensor_data = pd.DataFrame(sensor_information)
                f.write(sensor_data.to_markdown(index=False, tablefmt="pipe", floatfmt=".03f"))
                f.write("\n")
        if robot_properties is not None:
            f.write("\n# Robot properties\n")
            f.write(robot_properties.to_markdown(index=False, tablefmt="pipe", floatfmt=".03f"))
            f.write("\n")
except Exception:
    error(f"Failed while writing report:\n{traceback.format_exc()}\n")

s.simulationQuit(0)

# todo visual model complexity (count vertices) (optional)
# todo collision model complexity (count primitives) (optional)
