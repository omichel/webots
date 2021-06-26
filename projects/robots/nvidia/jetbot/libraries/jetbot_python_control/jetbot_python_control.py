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

"""JetBot Robot class with basic motion control."""

from controller import Robot


class JetBot(Robot):
    def __init__(self):
        Robot.__init__(self)
        self.left_motor = self.getDevice('left_wheel_hinge')
        self.right_motor = self.getDevice('right_wheel_hinge')
        self.camera = self.getDevice('camera')
        self.display = self.getDevice('display')
        self.right_motor.setPosition(float('+inf'))
        self.left_motor.setPosition(float('+inf'))
        self.right_motor.setVelocity(0)
        self.left_motor.setVelocity(0)
        self.speed_factor = 10

    def left(self, speed):
        s = self.__convert_speed(speed)
        self.right_motor.setVelocity(-s)
        self.left_motor.setVelocity(s)

    def right(self, speed):
        s = self.__convert_speed(speed)
        self.right_motor.setVelocity(s)
        self.left_motor.setVelocity(-s)

    def forward(self, speed):
        s = self.__convert_speed(speed)
        self.right_motor.setVelocity(s)
        self.left_motor.setVelocity(s)

    def backward(self, speed):
        s = self.__convert_speed(speed)
        self.right_motor.setVelocity(-s)
        self.left_motor.setVelocity(-s)

    def set_motors(self, speed_left, speed_right):
        self.right_motor.setVelocity(self.__convert_speed(speed_left))
        self.left_motor.setVelocity(self.__convert_speed(speed_right))

    def stop(self):
        self.right_motor.setVelocity(0)
        self.left_motor.setVelocity(0)

    def __convert_speed(self, speed):
        if speed < 0.2:
            return 0.0

        converted_values = [0, 0.12, 0.2, 0.3, 0.4, 0.45, 0.55, 0.6, 0.65]
        max_speed = 0.3
        for i in range(1, 9):
            if speed <= max_speed:
                return self.speed_factor * ((converted_values[i] - converted_values[i - 1]) / 0.1 * (speed - max_speed + 0.1) +
                                            converted_values[i - 1])
            max_speed += 0.1
        return converted_values[8] * self.speed_factor
