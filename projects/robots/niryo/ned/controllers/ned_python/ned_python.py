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

# Ned_Controller controller in python.

# Webots controller for the Niryo Ned robot.
# With this controller, you can see the 6 different axis of the robot moving
# You can also control the robots with your keyboard and launch a Pick and Pack


from controller import Robot
from controller import Keyboard

robot = Robot()

robot_name = robot.getName()
print('Name of the robot: ' + robot_name + '\n')

# Init the motors - the Ned robot is a 6-axis robot arm
# You can find the name of the rotationalMotors is the device parameters of each HingeJoints
m1 = robot.getDevice('joint_1')
m2 = robot.getDevice('joint_2')
m3 = robot.getDevice('joint_3')
m4 = robot.getDevice('joint_4')
m5 = robot.getDevice('joint_5')
m6 = robot.getDevice('joint_6')
m7 = robot.getDevice('joint_base_to_jaw_1')
m8 = robot.getDevice('joint_base_to_jaw_2')

# Set the motor velocity
# First we make sure that every joints are at their initial positions
m1.setPosition(0)
m2.setPosition(0)
m3.setPosition(0)
m4.setPosition(0)
m5.setPosition(0)
m6.setPosition(0)
m7.setPosition(0)
m8.setPosition(0)

# Set the motors speed. Here we set it to 1 radian/second
m1.setVelocity(1)
m2.setVelocity(1)
m3.setVelocity(1)
m4.setVelocity(1)
m5.setVelocity(1)
m6.setVelocity(1)
m7.setVelocity(1)
m8.setVelocity(1)


# ----Function to realize a demo of the Ned robot moving----
def demo():
    m1.setVelocity(1)
    m2.setVelocity(1)
    m3.setVelocity(1)

    if robot.step(0) == -1:
        return
    m1.setPosition(1.6)
    m7.setPosition(0.01)
    m8.setPosition(0.01)

    if robot.step(1500) == -1:
        return
    m1.setPosition(0)

    if robot.step(1500) == -1:
        return
    m2.setPosition(0.5)

    if robot.step(700) == -1:
        return
    m2.setPosition(0)

    if robot.step(700) == -1:
        return
    m1.setPosition(-0.5)
    m4.setPosition(1.4)

    if robot.step(1500) == -1:
        return
    m4.setPosition(0)

    if robot.step(1500) == -1:
        return
    m5.setPosition(-1)

    if robot.step(700) == -1:
        return
    m5.setPosition(0)

    if robot.step(1000) == -1:
        return
    m3.setPosition(0)
    m1.setPosition(0)

    if robot.step(500) == -1:
        return
    m6.setPosition(1.5)

    if robot.step(1000) == -1:
        return
    m6.setPosition(0)

    if robot.step(1000) == -1:
        return
    m7.setPosition(0)
    m8.setPosition(0)

    if robot.step(500) == -1:
        return
    m7.setPosition(0.01)
    m8.setPosition(0.01)


def pick_place():
    m1.setVelocity(0.5)
    m2.setVelocity(0.5)
    m3.setVelocity(0.5)

    if robot.step(0) == -1:
        return
    m1.setPosition(1.6)
    m2.setPosition(0.69)
    m7.setPosition(0.01)
    m8.setPosition(0.01)

    if robot.step(4200) == -1:
        return
    m3.setPosition(0.5)

    if robot.step(1200) == -1:
        return
    m7.setPosition(0)
    m8.setPosition(0)

    if robot.step(1500) == -1:
        return
    m3.setPosition(0.3)

    if robot.step(1200) == -1:
        return
    m1.setPosition(0)

    if robot.step(5000) == -1:
        return
    m3.setPosition(0.5)

    if robot.step(500) == -1:
        return
    m7.setPosition(0.01)
    m8.setPosition(0.01)

    if robot.step(500) == -1:
        return
    m3.setPosition(0)
    m2.setPosition(0)

while True:

    print("------------COMMANDS--------------")
    print("Launch demo --> SHIFT+D")
    print("Move joint_1 --> SHIFT+A or SHIFT+Z")
    print("Move joint_2 --> SHIFT+Q or SHIFT+S")
    print("Move joint_3 --> SHIFT+W or SHIFT+X")
    print("Move joint_4 --> SHIFT+Y or SHIFT+U")
    print("Move joint_5 --> SHIFT+J or SHIFT+K")
    print("Move joint_6 --> SHIFT+B or SHIFT+N")
    print("Open/Close Gripper --> SHIFT+L or SHIFT+M")
    print("Launch Pick and Place --> SHIFT+P")
    print("----------------------------------")

    timestep = int(robot.getBasicTimeStep())
    keyboard = Keyboard()
    keyboard.enable(timestep)

    while robot.step(timestep) != -1:

        key = keyboard.getKey()

        if key == Keyboard.SHIFT + ord('A'):
            print("Move --> joint_1 left")
            m1.setPosition(-1.5)

        elif key == Keyboard.SHIFT + ord('Z'):
            print("Move --> joint_1 right")
            m1.setPosition(1.5)

        elif key == Keyboard.SHIFT + ord('Q'):
            print("Move --> joint_2 left")
            m2.setPosition(0.5)

        elif key == Keyboard.SHIFT + ord('S'):
            print("Move --> joint_2 right")
            m2.setPosition(-0.5)

        elif key == Keyboard.SHIFT + ord('W'):
            print("Move --> joint_3 left")
            m3.setPosition(0.5)

        elif key == Keyboard.SHIFT + ord('X'):
            print("Move --> joint_3 right")
            m3.setPosition(-0.5)

        elif key == Keyboard.SHIFT + ord('Y'):
            print("Move --> joint_4 left")
            m4.setPosition(1)

        elif key == Keyboard.SHIFT + ord('U'):
            print("Move --> joint_4 right")
            m4.setPosition(-1)

        elif key == Keyboard.SHIFT + ord('H'):
            print("Move --> joint_5 left")
            m5.setPosition(1.4)

        elif key == Keyboard.SHIFT + ord('J'):
            print("Move --> joint_5 right")
            m5.setPosition(-1.4)

        elif key == Keyboard.SHIFT + ord('B'):
            print("Move --> joint_6 left")
            m6.setPosition(1.5)

        elif key == Keyboard.SHIFT + ord('N'):
            print("Move --> joint_6 right")
            m6.setPosition(-1.5)

        elif key == Keyboard.SHIFT + ord('L'):
            print("Move --> Open Gripper")
            m7.setPosition(0.01)
            m8.setPosition(0.01)

        elif key == Keyboard.SHIFT + ord('M'):
            print("Move --> Close Gripper")
            m7.setPosition(0)
            m8.setPosition(0)

        elif key == Keyboard.SHIFT + ord('D'):
            print("Demonstrator: Move Joints")
            demo()

        elif key == Keyboard.SHIFT + ord('P'):
            print("Demonstrator: Pick And Place")
            pick_place()
