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

from abc import ABC, abstractmethod

import numpy as np


POS_ABS_TOL = 0.03  # [m]

VALID_STATES = ["INITIAL", "READY", "SET", "PLAYING", "FINISHED"]
VALID_SEC_STATES = ["NORMAL", "PENALTYSHOOT", "OVERTIME", "TIMEOUT", "DIRECT_FREEKICK", "INDIRECT_FREEKICK", "PENALTYKICK",
                    "CORNERKICK", "GOALKICK", "THROWIN", "DROPBALL", "UNKNOWN"]


class StatusInformation:
    """Contains basic information over the Game Controller state and time properties

    TODO make gc_status private and provide more accessors. Writing gc_status is dangerous, since setting up its value
    without using the update method might compromise the getStateStart results.

    Attributes
    ----------
    system_time : float
        The number of seconds elapsed since the test supervisor was started
    simulated_time : float
        The number of seconds elapsed in the simulation world
    gc_status : GameState
        A structure containing the most recent information received from the Game Controller

    Methods
    -------
    update(system_time, simulated_time, gc_status)
        Updates the internal structure according to the provided parameters.
    getFormattedTime()
        Returns a string describing the current time in a pretty format
    getGameState()
        Returns the current game state, or None if no GameState has been made available yet.
    getSecondaryState()
        Returns the current secondary state (e.g. DIRECT_FREEKICK)
    getSecondaryTeamId()
        Returns the Id of the team concerned by the secondary state
    getSecondaryPhase()
        Returns the state phase of the secondary state
    getKickOffTeam()
        Returns the id of the team currently having kick_off
    getStateStart(state, clock_type, state_count)
        Returns the time at which the provided situation was reached first, or -1 if it has never been reached.
    getSecStateStart(sec_state, phase, clock_type, state_count)
        Returns the time at which the provided situation was reached first, or -1 if it has never been reached.
    """

    def __init__(self, system_time, simulated_time, gc_status):
        self.system_time = system_time
        self.simulated_time = simulated_time
        self._state_starts = {}
        for s in VALID_STATES:
            self._state_starts[s] = []
        self._sec_state_starts = {}
        for s in VALID_SEC_STATES:
            self._sec_state_starts[s] = []
            # sec_state_starts store one entry per phase
            for i in range(3):
                self._sec_state_starts[s].append([])

        self.gc_status = None
        self._updateGC(system_time, simulated_time, gc_status)

    def update(self, system_time, simulated_time, gc_status):
        self.system_time = system_time
        self.simulated_time = simulated_time
        self._updateGC(system_time, simulated_time, gc_status)

    def getFormattedTime(self):
        return "[{:08.3f}|{:08.3f}]".format(self.system_time, self.simulated_time)

    def getGameState(self):
        if self.gc_status is None:
            return None
        return self.gc_status.game_state.split("_", 1)[1]

    def getSecondaryState(self):
        if self.gc_status is None:
            return None
        return self.gc_status.secondary_state[6:]

    def getSecondaryTeamId(self):
        if self.gc_status is None:
            return None
        return int(self.gc_status.secondary_state_info[0])

    def getSecondaryPhase(self):
        if self.gc_status is None:
            return None
        return int(self.gc_status.secondary_state_info[1])

    def getKickOffTeam(self):
        if self.gc_status is None:
            return None
        return int(self.gc_status.kickoff_team)

    def getStateStart(self, state, clock_type, state_count=1):
        """
        Returns the time elapsed since the provided state was reached for the n-th time, returns -1 if unreached

        Parameters
        ----------
        state : string
            The concerned state (e.g. PLAYING)
        clock_type : string
            Either Simulated or System
        state_count : int
            The number of time we expect the provided state to have been reached
        """
        if len(self._state_starts[state]) < state_count:
            return -1
        return self._state_starts[state][state_count-1][clock_type]

    def getSecStateStart(self, sec_state, phase, clock_type, state_count=1):
        """
        Returns the time elapsed since the provided state was reached for the n-th time, returns -1 if unreached

        Parameters
        ----------
        sec_state : string
            The concerned secondary state (e.g. CORNERKICK)
        phase : int
            The phase of the GameInterruption (e.g. READY)
        clock_type : string
            Either Simulated or System
        state_count : int
            The number of time we expect the provided exact state (sec_state + phase) to have been reached
        """
        if len(self._sec_state_starts[sec_state]) < phase:
            raise RuntimeError(f"Unexpected phase: {phase}")
        if len(self._sec_state_starts[sec_state][phase]) < state_count:
            return -1
        return self._sec_state_starts[sec_state][phase][state_count-1][clock_type]

    def _updateGC(self, system_time, simulated_time, gc_status):
        if gc_status is not None:
            update_state_start = (self.gc_status is None or
                                  gc_status.game_state != self.gc_status.game_state)
            update_sec_state_start = (self.gc_status is None or
                                      gc_status.secondary_state != self.gc_status.secondary_state or
                                      gc_status.secondary_state_info[1] != self.gc_status.secondary_state_info[1])
            if update_state_start:
                state = gc_status.game_state.split("_", 1)[1]
                self._state_starts[state].append({
                    "System": system_time,
                    "Simulated": simulated_time
                })
                print(f"Adding new state start for state {state}: count: {len(self._state_starts[state])}")
            if update_sec_state_start:
                sec_state = gc_status.secondary_state.split("_", 1)[1]
                phase = gc_status.secondary_state_info[1]
                if phase > len(self._sec_state_starts[sec_state]):
                    raise RuntimeError(f"Unexpected phase: {phase}")
                self._sec_state_starts[sec_state][phase].append({
                    "System": system_time,
                    "Simulated": simulated_time
                })
                count = len(self._sec_state_starts[sec_state][phase])
                print(f"Adding new sec state start for {sec_state}:{phase}: count: {count}")
        self.gc_status = gc_status


class TimeSpecification(ABC):
    """Defines when an event is active or finished based on clocks and GameController states

    This class can be used to express times either based on the simulation time or the system time.
    It is possible to define time at which an event occured based on the time since a specific state of the Game
    Controller has been reached.

    Methods
    -------
    isActive(status)
        Return true if the event should be actived based on status, false otherwise.
    isFinished(status)
        Return true if the event has finished being processed and will never be active again.
    """

    @abstractmethod
    def isActive(self, status):
        pass

    @abstractmethod
    def isFinished(self, status):
        pass

    def getCurrentTime(self, status):
        offset = 0.0
        if self._state is not None:
            offset = status.getStateStart(self._state, self._clock_type, self._state_count)
            if offset < 0:
                return -1
        elif self._secondary_state is not None:
            offset = status.getSecStateStart(self._secondary_state, self._phase, self._clock_type, self._state_count)
            if offset < 0:
                return -1
        if self._clock_type == "Simulated":
            return status.simulated_time - offset
        elif self._clock_type == "System":
            return status.system_time - offset
        raise RuntimeError(f"Unexpected value for _clock_type: {self._clock_type}")

    def buildFromDictionary(properties):
        t = properties["time"]
        clock_type = properties.get("clock_type", "Simulated")
        state = properties.get("state")
        state_count = properties.get("state_count", 1)
        secondary_state = properties.get("secondary_state")
        phase = properties.get("phase")
        if clock_type not in ["Simulated", "System"]:
            raise RuntimeError(f"clock_type: '{clock_type}' unknown")
        if isinstance(t, float) or isinstance(t, int):
            return TimePoint(t, clock_type, state, state_count, secondary_state, phase)
        elif isinstance(t, list):
            if len(t) == 2:
                return TimeInterval(t[0], t[1], clock_type, state, state_count, secondary_state, phase)
            raise RuntimeError(f"Invalid size for time: {len(t)}")
        raise RuntimeError("Invalid type for time")


class TimeInterval(TimeSpecification):
    """An implementation of TimeSpecification which is active over an interval of time."""

    def __init__(self, start, end, clock_type, state, state_count, secondary_state, phase):
        self._start = start
        self._end = end
        self._clock_type = clock_type
        if state is not None and state not in VALID_STATES:
            raise RuntimeError("Invalid state: {state}")
        self._state = state
        self._state_count = state_count
        self._secondary_state = secondary_state
        self._phase = phase

    def isActive(self, status):
        t = self.getCurrentTime(status)
        return t > self._start and t <= self._end

    def isFinished(self, status):
        t = self.getCurrentTime(status)
        return t > self._end


class TimePoint(TimeSpecification):
    """An implementation of TimeSpecification which is finished as soon as it is activated."""

    def __init__(self, t, clock_type, state, state_count, secondary_state, phase):
        self._t = t
        self._clock_type = clock_type
        if state is not None and state not in VALID_STATES:
            raise RuntimeError("Invalid state: {state}")
        self._state = state
        self._state_count = state_count
        self._secondary_state = secondary_state
        self._phase = phase

    def isActive(self, status):
        t = self.getCurrentTime(status)
        return t > self._t

    def isFinished(self, status):
        t = self.getCurrentTime(status)
        return t > self._t

    def __str__(self):
        return f"t:{self._t}, clock_type:{self._clock_type}, state: {self._state}"


class Test:
    """Defines a set of properties on the game that have to be compared with the game status.

    This class allows to test properties of objects (position,orientation), but also properties
    based on the status of the game controller (current game state, penalty status for robots).
    It provides simple access to the result of the test and allows to easily print the cause of failure.
    To avoid spamming messages, if one of the internal verifications fail once, a failure message is saved and the test
    is not performed anymore.

    Methods
    -------
    perform(status, supervisor)
        Runs all appropriated checks based on the latest status information, using the supervisor in read only.
    hasPassed()
        Returns True if no test has failed until now.
    isCritical()
        Returns True if a failed test should stop execution of the simulator and end the test immediately
    printResult(skipped)
        A pretty print version including a status message and the causes of failure, if applicable.
    """

    def __init__(self, name, target=None, position=None, rotation=None,
                 state=None, penalty=None, yellow_cards=None, secondary_state=None, secondary_team_id=None,
                 secondary_phase=None, score=None, kick_off_team=None, critical=False, red_cards=None, warnings=None):
        self._name = name
        self._target = target
        self._position = position
        self._rotation = rotation
        self._state = state
        self._penalty = penalty
        self._warnings = warnings
        self._yellow_cards = yellow_cards
        self._red_cards = red_cards
        self._secondary_state = secondary_state
        self._secondary_team_id = secondary_team_id
        self._secondary_phase = secondary_phase
        self._score = score
        self._kick_off_team = kick_off_team
        self._critical = critical
        self._abs_tol = POS_ABS_TOL
        self._msg = []
        self._success = True

    def perform(self, status, supervisor):
        if self._position is not None:
            self._testTargetPosition(status, supervisor)
        if self._rotation is not None:
            self._testTargetRotation(status, supervisor)
        if self._penalty is not None:
            self._testTargetPenalty(status, supervisor)
        if self._warnings is not None:
            self._testWarnings(status, supervisor)
        if self._yellow_cards is not None:
            self._testYellowCards(status, supervisor)
        if self._red_cards is not None:
            self._testRedCards(status, supervisor)
        if self._state is not None:
            self._testState(status, supervisor)
        if self._secondary_state is not None:
            self._testSecondaryState(status, supervisor)
        if self._secondary_team_id is not None:
            self._testSecondaryTeamId(status, supervisor)
        if self._secondary_phase is not None:
            self._testSecondaryPhase(status, supervisor)
        if self._score is not None:
            self._testScore(status, supervisor)
        if self._kick_off_team is not None:
            self._testKickOffTeam(status, supervisor)

    def hasPassed(self):
        return self._success

    def isCritical(self):
        return self._critical

    def printResult(self, skipped=True):
        status = "PASS"
        if not self._success:
            if self._critical:
                status = "CRIT"
            else:
                status = "FAIL"
        elif skipped:
            status = "SKIP"
        print(f"[{status}] Test {self._name}")
        for m in self._msg:
            print(f"\tCaused by {m}")

    def buildFromDictionary(dic):
        """Returns a Test based on the provided dictionary.

        Only 'name' field is required, consistency of the values is not checked currently.
        """
        t = Test(dic["name"])
        t._target = dic.get("target")
        t._position = dic.get("position")
        t._rotation = dic.get("rotation")
        t._state = dic.get("state")
        t._secondary_state = dic.get("secondary_state")
        t._secondary_team_id = dic.get("secondary_team_id")
        t._secondary_phase = dic.get("secondary_phase")
        t._penalty = dic.get("penalty")
        t._warnings = dic.get("warnings")
        t._yellow_cards = dic.get("yellow_cards")
        t._red_cards = dic.get("red_cards")
        t._score = dic.get("score")
        t._kick_off_team = dic.get("kick_off_team")
        t._critical = dic.get("critical", False)
        t._abs_tol = dic.get("abs_tol", POS_ABS_TOL)
        return t

    def _getAbsTol(self):
        return self._abs_tol

    def _getTargetGCData(self, status):
        splitted_target = self._target.split("_")
        team_color = splitted_target[0]
        for i in range(2):
            if status.gc_status.teams[i].team_color == team_color:
                if len(splitted_target) == 1:
                    return status.gc_status.teams[i]
                if len(splitted_target) == 3:
                    player_idx = int(splitted_target[2]) - 1
                    return status.gc_status.teams[i].players[player_idx]
        raise RuntimeError(f"Invalid target to get GameController data from {self._target}")

    def _testTargetPosition(self, status, supervisor):
        if self._target is None:
            raise RuntimeError("{self._name} tests position and has no target")
        target = supervisor.getFromDef(self._target)
        received_pos = target.getField('translation').getSFVec3f()
        if not np.allclose(self._position, received_pos, atol=self._getAbsTol()):
            failure_msg = f"Position Invalid at {status.getFormattedTime()}: "\
                f"expecting {self._position}, received {received_pos}"
            print(f"Failure in test {self._name}\n\t{failure_msg}")
            self._msg.append(failure_msg)
            self._success = False
            # Each test can only fail once to avoid spamming
            self._position = None

    def _testTargetRotation(self, status, supervisor):
        if self._target is None:
            raise RuntimeError("{self._name} tests position and has no target")
        # TODO Implement of rotation check is currently disabled, to be
        # properly introduced, it will require to be able to compare properly
        # the axis-angle obtained. Moreover, after reset, it does not seem that
        # the robot respects has the exact orientation used in pose

    def _testTargetPenalty(self, status, supervisor):
        if self._target is None:
            raise RuntimeError("{self._name} tests position and has no target")
        gc_data = self._getTargetGCData(status)
        if gc_data.penalty != self._penalty:
            failure_msg = f"Invalid penalty at {status.getFormattedTime()}: "\
                f"for {self._target}: received {gc_data.penalty},"\
                f"expecting {self._penalty}"
            self._msg.append(failure_msg)
            self._penalty = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testState(self, status, supervisor):
        received_state = status.getGameState()
        expected_state = self._state
        if expected_state != received_state:
            failure_msg = f"Invalid state at {status.getFormattedTime()}: "\
                f"received {received_state}, expecting {expected_state}"
            self._msg.append(failure_msg)
            self._state = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testSecondaryState(self, status, supervisor):
        received_state = status.getSecondaryState()
        expected_state = self._secondary_state
        if expected_state != received_state:
            failure_msg = f"Invalid secondary state at {status.getFormattedTime()}: "\
                f"received {received_state}, expecting {expected_state}"
            self._msg.append(failure_msg)
            self._secondary_state = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testSecondaryTeamId(self, status, supervisor):
        received_team_id = status.getSecondaryTeamId()
        expected_team_id = self._secondary_team_id
        if expected_team_id != received_team_id:
            failure_msg = f"Invalid secondary team_id at {status.getFormattedTime()}: "\
                f"received {received_team_id}, expecting {expected_team_id}"
            self._msg.append(failure_msg)
            self._secondary_team_id = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testSecondaryPhase(self, status, supervisor):
        received_phase = status.getSecondaryPhase()
        expected_phase = self._secondary_phase
        if expected_phase != received_phase:
            failure_msg = f"Invalid secondary phase at {status.getFormattedTime()}: "\
                f"received {received_phase}, expecting {expected_phase}"
            self._msg.append(failure_msg)
            self._secondary_phase = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testWarnings(self, status, supervisor):
        if self._target is None:
            raise RuntimeError("{self._name} tests warnings and has no target")
        gc_data = self._getTargetGCData(status)
        received = gc_data.number_of_warnings
        if received != self._warnings:
            failure_msg = \
                f"Invalid number of warnings at {status.getFormattedTime()}: "\
                f"for {self._target}: received {received},"\
                f"expecting {self._warnings}"
            self._msg.append(failure_msg)
            self._warnings = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testYellowCards(self, status, supervisor):
        if self._target is None:
            raise RuntimeError("{self._name} tests yellow_cards and has no target")
        gc_data = self._getTargetGCData(status)
        received = gc_data.number_of_yellow_cards
        if received != self._yellow_cards:
            failure_msg = \
                f"Invalid number of yellow cards at {status.getFormattedTime()}: "\
                f"for {self._target}: received {received},"\
                f"expecting {self._yellow_cards}"
            self._msg.append(failure_msg)
            self._yellow_cards = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testRedCards(self, status, supervisor):
        if self._target is None:
            raise RuntimeError("{self._name} tests red_cards and has no target")
        gc_data = self._getTargetGCData(status)
        received = gc_data.number_of_red_cards
        if received != self._red_cards:
            failure_msg = \
                f"Invalid number of red cards at {status.getFormattedTime()}: "\
                f"for {self._target}: received {received},"\
                f"expecting {self._red_cards}"
            self._msg.append(failure_msg)
            self._red_cards = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testScore(self, status, supervisor):
        team_data = self._getTargetGCData(status)
        received = team_data.score
        if received != self._score:
            failure_msg = \
                f"Invalid score at {status.getFormattedTime()}: "\
                f"for {self._target}: received {received},"\
                f"expecting {self._score}"
            self._msg.append(failure_msg)
            self._score = None
            # Each test can only fail once to avoid spamming
            self._success = False

    def _testKickOffTeam(self, status, supervisor):
        received = status.getKickOffTeam()
        if received != self._kick_off_team:
            failure_msg = \
                f"Invalid kick_off team at {status.getFormattedTime()}: "\
                f"received {received}, expecting {self._kick_off_team}"
            self._msg.append(failure_msg)
            self._kick_off_team = None
            # Each test can only fail once to avoid spamming
            self._success = False


class Action:
    """Defines a way to interact with the current status of the game

    Currently, only moving objects has been tested.

    Methods
    -------
    buildFromDictionary()
        This class method builds an Action from a dictionary
    perform(supervisor)
        Applies the required modifications to the supervisor
    """

    def __init__(self, target, position=None, orientation=None, force=None, velocity=None):
        self._target = target
        self._position = position
        self._orientation = orientation
        self._force = force
        if velocity is not None and len(velocity) == 3:
            velocity.extend((0, 0, 0))  # add null angular velocity
        self._velocity = velocity

    def buildFromDictionary(dic):
        """Returns an Action based on the provided dictionary.

        Only the 'target' field is required, consistency of the values is not checked currently.
        """
        a = Action(dic["target"])
        a._position = dic.get("position")
        a._orientation = dic.get("orientation")
        a._force = dic.get("force")
        a._velocity = dic.get("velocity")
        if a._velocity is not None and len(a._velocity) == 3:
            a._velocity.extend((0, 0, 0))  # add null angular velocity
        return a

    def perform(self, supervisor):
        obj = supervisor.getFromDef(self._target)
        if obj is None:
            print(f"Invalid target for action: {self._target}")
        if self._position is not None:
            self._setPosition(obj)
        if self._orientation is not None:
            self._setOrientation(obj)
        if self._force is not None:
            self._setForce(obj)
        if self._velocity is not None:
            self._setVelocity(obj)

    def _setPosition(self, obj):
        obj.resetPhysics()
        obj.getField("translation").setSFVec3f(self._position)

    def _setOrientation(self, obj):
        obj.getField("rotation").setSFRotation(self._orientation)

    def _setForce(self, obj):
        obj.addForce(self._force, False)

    def _setVelocity(self, obj):
        obj.setVelocity(self._velocity)


class Event:
    """An event is a set of tests and actions that occur over a period of time.

    Methods
    -------
    isActive(status)
        Is the event active given the provided status?
    isFinished(status)
        Is the event finished given the provided status?
    hasCriticalFailure()
        Returns True if a test has failed in a critical way that should lead to interrupting a scenario, False otherwise
    getNbTests()
        Returns the number of tests in the event
    getNbTestsPassed()
        Returns the number of tests that haven't failed
    printResults(skipped)
        Print result of the tests including appropriate mention if the test was skipped
    perform(status, supervisor)
        Apply all tests and actions on the simulation
    """

    def __init__(self, time_spec, tests=[], actions=[]):
        self._time_spec = time_spec
        self._tests = tests
        self._actions = actions

    def isActive(self, status):
        return self._time_spec.isActive(status)

    def isFinished(self, status):
        # TODO: an event might also been 'finished' if all tests have failed and no action is there
        return self._time_spec.isFinished(status)

    def perform(self, status, supervisor):
        """Applies all tests of the event and, apply actions *afterwards*
        """
        for c in self._tests:
            c.perform(status, supervisor)
        for a in self._actions:
            a.perform(supervisor)

    def getNbTests(self):
        return len(self._tests)

    """
    Return the number of tests that have not failed. It is the caller
    responsibility to make sure event is finished before calling this function.
    """
    def getNbTestsPassed(self):
        return sum([1 for c in self._tests if c.hasPassed()])

    def printResults(self, skipped):
        for c in self._tests:
            c.printResult(skipped)

    def hasCriticalFailure(self):
        for t in self._tests:
            if t.isCritical() and not t.hasPassed():
                return True
        return False

    def buildFromDictionary(dic):
        tests_str = dic.get("tests")
        tests = []
        if tests_str is not None:
            for test_dic in tests_str:
                tests.append(Test.buildFromDictionary(test_dic))
        actions_str = dic.get("actions")
        actions = []
        if actions_str is not None:
            for action_dic in actions_str:
                actions.append(Action.buildFromDictionary(action_dic))
        event = Event(TimeSpecification.buildFromDictionary(dic["timing"]), tests, actions)
        return event


class Scenario:
    """A scenario is composed of multiple events, it provides a global access to all of them.

    Methods
    -------
    step(status, supervisor)
        Perform all the events that are currently active.
    isFinished()
        Returns True if all the events of the scenario have been executed completely.
    getNbTests()
        Returns the number of tests in the scenario.
    getNbTestsPassed()
        Returns the number of tests that passed successfully.
    printResults()
        Print the results of all the tests and a final summary.
    hasCriticalFailure()
        Returns True if a test has failed in a critical way that should lead to interrupting a scenario, False otherwise
    buildFromList()
        Build a scenario from a list of events.
    """
    def __init__(self, events=[]):
        self._waiting_events = events
        self._finished_events = []

    def step(self, status, supervisor):
        finished_events = []
        for i in range(len(self._waiting_events)):
            e = self._waiting_events[i]
            if e.isActive(status):
                e.perform(status, supervisor)
            if e.isFinished(status):
                finished_events.insert(0, i)
        for i in finished_events:
            print(f"{status.getFormattedTime()} Finished treating an event")
            e = self._waiting_events.pop(i)
            e.printResults(False)
            self._finished_events.append(e)

    def isFinished(self):
        return len(self._waiting_events) == 0

    def getNbTests(self):
        waiting = sum([e.getNbTests() for e in self._waiting_events])
        finished = sum([e.getNbTests() for e in self._finished_events])
        return waiting + finished

    def getNbTestsPassed(self):
        return sum([e.getNbTestsPassed() for e in self._finished_events])

    def printResults(self):
        for e in self._waiting_events:
            e.printResults(True)
        for e in self._finished_events:
            e.printResults(False)
        nb_tests = self.getNbTests()
        nb_tests_passed = self.getNbTestsPassed()
        print(f"TEST RESULTS: {nb_tests_passed}/{nb_tests}")

    def hasCriticalFailure(self):
        for e in self._waiting_events:
            if e.hasCriticalFailure():
                return True
        for e in self._finished_events:
            if e.hasCriticalFailure():
                return True
        return False

    def buildFromList(event_list):
        s = Scenario()
        for e in event_list:
            s._waiting_events.append(Event.buildFromDictionary(e))
        return s
