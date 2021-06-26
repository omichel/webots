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

"""test_supervisor controller."""
from controller import Supervisor
from gamestate import GameState

import json
import os
import rc_testing
import socket
import time
import traceback
import sys


class GCListener:
    """A simple UDP socket listening to the GameController broadcast messages"""

    def __init__(self):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self._socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self._socket.bind(('0.0.0.0', 3838))
        self._socket.setblocking(False)
        self._state = None

    def receive(self):
        try:
            data, peer = self._socket.recvfrom(GameState.sizeof())
        except BlockingIOError:
            return
        except Exception as e:
            print(f'UDP input failure: {e}')
            pass
        if not data:
            print('No UDP data received')
            return
        self._state = GameState.parse(data)

    def getState(self):
        return self._state


scenario_config_file = os.environ['WEBOTS_ROBOCUP_TEST_SCENARIO']

supervisor = Supervisor()
time_step = int(supervisor.getBasicTimeStep())
system_start = time.time()

try:
    with open(scenario_config_file) as json_file:
        test_scenario = rc_testing.Scenario.buildFromList(json.load(json_file))
except EnvironmentError:
    print(f"FAILED TO LOAD SCENARIO at {scenario_config_file}", file=sys.stderr)
    traceback.print_exc()
    # Ticking once the supervisor to flush waiting and then exit simulation
    # This ensures that referee is not too busy to exit properly
    supervisor.step(time_step)
    time.sleep(4.0)
    supervisor.simulationQuit(0)

status = rc_testing.StatusInformation(0.0, 0.0, None)

gc_listener = GCListener()

finished = False
critical_failure = False
simulated_time = 0

while supervisor.step(time_step) != -1:
    if finished or critical_failure:
        if finished:
            print(f"{status.getFormattedTime()} END OF TESTING")
        else:
            print(f"{status.getFormattedTime()} PREMATURELY EXITING TESTING")
        test_scenario.printResults()
        break
    try:
        gc_listener.receive()
        simulated_time += time_step/1000
        status.update(time.time() - system_start, simulated_time, gc_listener.getState())
        if (status.gc_status is not None):
            test_scenario.step(status, supervisor)

        finished = test_scenario.isFinished()
        critical_failure = test_scenario.hasCriticalFailure()
    except Exception:
        traceback.print_exc()
        critical_failure = True

# TODO make exit status different if one of the tests did not pass or critical failure
supervisor.step(time_step)
supervisor.simulationQuit(0)
