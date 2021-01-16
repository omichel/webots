# Copyright 1996-2019 Cyberbotics Ltd.
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

"""Referee supervisor controller for the Robot Wrestling Tournament."""

import math
from controller import Supervisor


class Referee (Supervisor):

    def init(self):
        self.digit = [[0] * 10 for i in range(3)]  # create an array of size [3][10] filled in with zeros
        for j in range(3):
            for i in range(10):
                self.digit[j][i] = self.getMotor("digit " + str(j) + str(i))
        self.currentDigit = [0, 0, 0]  # 0:00
        self.robot = [0] * 2
        self.robot[0] = self.getFromDef("WRESTLER_RED")
        self.robot[1] = self.getFromDef("WRESTLER_BLUE")
        self.min = [[0] * 3 for i in range(2)]
        self.max = [[0] * 3 for i in range(2)]
        for i in range(2):
            self.min[i] = self.robot[i].getPosition()
            self.max[i] = self.robot[i].getPosition()
        self.coverage = [0] * 2
        self.koCount = [0] * 2
        self.indicator = [0] * 2
        self.indicator[0] = self.getMotor("red indicator")
        self.indicator[1] = self.getMotor("blue indicator")

    def displayTime(self, minutes, seconds):
        for j in range(3):
            self.digit[j][self.currentDigit[j]].setPosition(1000)  # far away, not visible
        self.currentDigit[0] = minutes
        self.currentDigit[1] = seconds // 10
        self.currentDigit[2] = seconds % 10
        for j in range(3):
            self.digit[j][self.currentDigit[j]].setPosition(0)  # visible

    def run(self):
        matchDuration = 3 * 60 * 1000  # a match lasts 3 minutes
        timeStep = int(self.getBasicTimeStep())  # retrieves the WorldInfo.basicTimeTime (ms) from the world file
        time = 0
        seconds = -1
        ko = -1
        while True:
            if time % 200 == 0:
                s = int(time / 1000) % 60
                if seconds != s:
                    seconds = s
                    minutes = int(time / 60000)
                    self.displayTime(minutes, seconds)
                box = [0] * 3
                for i in range(2):
                    position = self.robot[i].getPosition()
                    if abs(position[0]) > 1 or abs(position[1]) > 1:  # outside of the ring
                        continue
                    coverage = 0
                    for j in range(3):
                        if position[j] < self.min[i][j]:
                            self.min[i][j] = position[j]
                        elif position[j] > self.max[i][j]:
                            self.max[i][j] = position[j]
                        box[j] = self.max[i][j] - self.min[i][j]
                        coverage += box[j] * box[j]
                    coverage = math.sqrt(coverage)
                    self.coverage[i] = coverage
                    self.indicator[i].setPosition(self.coverage[i] / 7)
                if position[1] < 0.75:  # low position threshold
                    self.koCount[i] = self.koCount[i] + 200
                    if self.koCount[i] > 10000:  # 10 seconds
                        ko = i
                else:
                    self.koCount[i] = 0
                if self.koCount[0] > self.koCount[1]:
                    print("\fred KO: %d" % (10 - self.koCount[0] // 1000))
                elif self.koCount[1] > self.koCount[0]:
                    print("\fblue KO: %d" % (10 - self.koCount[1] // 1000))
                #  print("\fred: %1.3f - blue: %1.3f" % (self.coverage[0], self.coverage[1]))
            if self.step(timeStep) == -1 or time > matchDuration or ko != -1:
                break
            time += timeStep
        if ko == 0:
            print("Wrestler red is KO. Wrestler blue wins!")
        elif ko == 1:
            print("Wrestler blue is KO. Wrestler red wins!")
        elif self.coverage[0] >= self.coverage[1]:  # in case of coverage equality, red wins
            print("Wresler red wins: %s >= %s" % (self.coverage[0], self.coverage[1]))
        else:
            print("Wresler blue wins: %s > %s" % (self.coverage[1], self.coverage[0]))


# create the referee instance and run main loop
referee = Referee()
referee.init()
referee.run()
