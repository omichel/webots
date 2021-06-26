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

import numpy as np


class ForcefulContactMatrix:
    def __init__(self, red_team_size, blue_team_size, observation_duration, foul_duration, time_step):
        self.red_team_size = red_team_size
        self.blue_team_size = blue_team_size
        self.foul_duration = int((1000 * foul_duration) / time_step)
        self.time_window_size = int((1000 * observation_duration) / time_step)
        self.time_step = time_step
        self.matrix = np.zeros([red_team_size, blue_team_size, self.time_window_size], dtype=bool)

    def clear(self, time_count):
        index = int(time_count / self.time_step) % self.time_window_size
        for i in range(self.red_team_size):
            for j in range(self.blue_team_size):
                self.matrix[i][j][index] = False

    def clear_all(self):
        self.matrix.fill(False)

    def set_contact(self, red_number, blue_number, time_count, value=True):
        index = int(time_count / self.time_step) % self.time_window_size
        self.matrix[int(red_number) - 1][int(blue_number) - 1][index] = value

    def contact(self, red_number, blue_number, time_count):
        index = int(time_count / self.time_step) % self.time_window_size
        return self.matrix[int(red_number) - 1][int(blue_number) - 1][index]

    def get_collision_time(self, red_number, blue_number):
        """Return collision time in seconds of the collision between both robots"""
        sum = 0
        for touch in self.matrix[int(red_number) - 1][int(blue_number) - 1]:
            if touch:
                sum += 1
        return sum * self.time_step / 1000

    def long_collision(self, red_number, blue_number):
        sum = 0
        for touch in self.matrix[int(red_number) - 1][int(blue_number) - 1]:
            if touch:
                sum += 1
        return sum > self.foul_duration
