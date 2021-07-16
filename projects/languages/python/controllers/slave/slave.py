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

"""
This controller gives to its robot the following behavior:
According to the messages it receives, the robot change its
behavior.
"""

from controller import AnsiCodes
from controller import Robot
from common import common_print


class Enumerate(object):
    def __init__(self, names):
        for number, name in enumerate(names.split()):
            setattr(self, name, number)


class Slave (Robot):

    Mode = Enumerate('STOP MOVE_FORWARD AVOIDOBSTACLES TURN')
    timeStep = 32
    maxSpeed = 10.0
    mode = Mode.AVOIDOBSTACLES
    motors = []
    distanceSensors = []

    def boundSpeed(self, speed):
        return max(-self.maxSpeed, min(self.maxSpeed, speed))

    def __init__(self):
        super(Slave, self).__init__()
        self.mode = self.Mode.AVOIDOBSTACLES
        self.camera = self.getDevice('camera')
        self.camera.enable(4 * self.timeStep)
        self.receiver = self.getDevice('receiver')
        self.receiver.enable(self.timeStep)
        self.motors.append(self.getDevice("left wheel motor"))
        self.motors.append(self.getDevice("right wheel motor"))
        self.motors[0].setPosition(float("inf"))
        self.motors[1].setPosition(float("inf"))
        self.motors[0].setVelocity(0.0)
        self.motors[1].setVelocity(0.0)
        for dsnumber in range(0, 2):
            self.distanceSensors.append(self.getDevice('ds' + str(dsnumber)))
            self.distanceSensors[-1].enable(self.timeStep)

    def run(self):
        while True:
            # Read the supervisor order.
            if self.receiver.getQueueLength() > 0:
                message = self.receiver.getData().decode('utf-8')
                self.receiver.nextPacket()
                print('I should ' + AnsiCodes.RED_FOREGROUND + message + AnsiCodes.RESET + '!')
                if message == 'avoid obstacles':
                    self.mode = self.Mode.AVOIDOBSTACLES
                elif message == 'move forward':
                    self.mode = self.Mode.MOVE_FORWARD
                elif message == 'stop':
                    self.mode = self.Mode.STOP
                elif message == 'turn':
                    self.mode = self.Mode.TURN
            delta = self.distanceSensors[0].getValue() - self.distanceSensors[1].getValue()
            speeds = [0.0, 0.0]

            # Send actuators commands according to the mode.
            if self.mode == self.Mode.AVOIDOBSTACLES:
                speeds[0] = self.boundSpeed(self.maxSpeed / 2 + 0.1 * delta)
                speeds[1] = self.boundSpeed(self.maxSpeed / 2 - 0.1 * delta)
            elif self.mode == self.Mode.MOVE_FORWARD:
                speeds[0] = self.maxSpeed
                speeds[1] = self.maxSpeed
            elif self.mode == self.Mode.TURN:
                speeds[0] = self.maxSpeed / 2
                speeds[1] = -self.maxSpeed / 2
            self.motors[0].setVelocity(speeds[0])
            self.motors[1].setVelocity(speeds[1])

            # Perform a simulation step, quit the loop when
            # Webots is about to quit.
            if self.step(self.timeStep) == -1:
                break


controller = Slave()
common_print('slave')
controller.run()
