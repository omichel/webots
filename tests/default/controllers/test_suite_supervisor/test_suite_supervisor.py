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

"""Supervisor used by the test suite."""

import sys
import shlex
import os.path
import time
from controller import Supervisor


class FileManager:
    """Encapsulate a file."""

    def __init__(self, filename):
        """Constructor. Check the validity of the file."""
        self.filename = filename
        if not os.path.isfile(self.filename):
            self.fatalError('File does not exist')

    def fatalError(self, msg):
        """Exit the controller with an error message."""
        sys.exit(self.__class__.__name__ + ' Error: ' + msg)


class IndexFileManager(FileManager):
    """Manage the index file."""

    def readIndex(self):
        """Get the current index."""
        file = open(self.filename)
        firstLine = file.readline().strip()
        index = int(firstLine)
        file.close()
        return index

    def incrementIndex(self):
        """Increment the index."""
        newIndex = self.readIndex() + 1
        assert newIndex >= 0
        file = open(self.filename, 'w')
        file.write(str(newIndex) + '\n')
        file.close()
        return newIndex


class SimulationFileManager(FileManager):
    """Manage the simulation file."""

    def countSimulations(self):
        """Get the simulation counter."""
        # count only lines having some content
        file = open(self.filename)
        counter = 0
        for line in file:
            if line.strip():
                counter += 1
        file.close()
        return counter

    def filenameAtLine(self, index):
        """Get the filename at a given line."""
        file = open(self.filename)
        lines = file.readlines()
        file.close()
        line = lines[index].strip()
        relativeFilename = line
        return relativeFilename


class OutputFileManager(FileManager):
    """Manage the output file."""

    def write(self, txt):
        """Write a result."""
        file = open(self.filename, 'a')
        file.write(txt)
        file.close()


class StdFileManager(FileManager):
    """Manage the stdout or stderr text redirector to a file."""

    def reset(self):
        """Empty the stdout or stderr file."""
        file = open(self.filename, 'w')
        file.close()

    def isEmpty(self):
        """Check if the file is empty."""
        return os.stat(self.filename).st_size == 0

    def contains(self, expectedString):
        """Check if the file contains the expected string."""
        file = open(self.filename)
        found = False
        line = file.readline()
        while line:
            if expectedString in line:
                found = True
                break
            line = file.readline()
        file.close()
        return found

    def dump(self):
        """Dumps the contents of the file."""
        file = open(self.filename)
        line = file.readline()
        string = ''
        while line:
            string = string + line
            line = file.readline()
        file.close()
        return string


class TestSuite (Supervisor):
    """Supervisor class."""

    if 'WEBOTS_TEST_SUITE' not in os.environ:
        sys.exit(0)

    # file path based on api, proto, parser, etc. folders
    indexFilename = 'worlds_index.txt'
    simulationFilename = 'worlds.txt'
    outputFilename = '../output.txt'
    tempWorldCounterFilename = '../world_counter.txt'

    def __init__(self, *args, **kwargs):
        """Supervisor constructor."""
        self.lastSimulation = False
        Supervisor.__init__(self, *args, **kwargs)

        self.isParserTest = "parser" in self.getCustomData()
        isDefaultWorld = "empty" in self.getCustomData()
        if isDefaultWorld:
            self.cwdPrefix = '../../../parser/'
        else:
            self.cwdPrefix = '../../'
        self.lastSimulation = False

        # prepare the next simulation
        self.indexFileManager = IndexFileManager(self.cwdPrefix + self.indexFilename)
        self.simulationFileManager = SimulationFileManager(self.cwdPrefix + self.simulationFilename)
        self.outputFileManager = OutputFileManager(self.cwdPrefix + self.outputFilename)

        currentIndex = self.indexFileManager.readIndex()
        self.currentSimulationFilename = self.simulationFileManager.filenameAtLine(currentIndex)
        print('RUN: ' + self.currentSimulationFilename)

        nSimulations = self.simulationFileManager.countSimulations()
        if currentIndex + 1 >= nSimulations:
            self.lastSimulation = True

    def initParserTest(self):
        """Prepare for the parser tests."""
        self.stdoutFileManager = StdFileManager(self.cwdPrefix + '../webots_stdout.txt')
        self.stderrFileManager = StdFileManager(self.cwdPrefix + '../webots_stderr.txt')
        expectedStringFile = open(self.cwdPrefix + 'expected_results.txt')
        content = expectedStringFile.readlines()
        self.expectedString = ""
        found = False
        for line in content:
            line.strip()
            if line:
                [world, expected] = shlex.split(line)
                localWorldPath = self.currentSimulationFilename.replace(os.path.dirname(os.path.abspath(self.cwdPrefix)), '')
                localWorldPath = localWorldPath.strip(os.sep)
                if os.path.normpath(world) == localWorldPath:
                    found = True
                    if expected != 'VOID':
                        self.expectedString = expected
                        break
            line = expectedStringFile.readline()
        expectedStringFile.close()

        if not found:
            self.outputFileManager.write(
                'FAILURE with ' + self.currentSimulationFilename +
                ': world name not found in \'expected_results.txt\'\n'
            )
        return found

    def assessParserTest(self):
        success = False
        if self.expectedString:
            success = self.stderrFileManager.contains(self.expectedString) or \
                self.stdoutFileManager.contains(self.expectedString)
            if not success:
                self.outputFileManager.write(
                    'FAILURE with ' + self.currentSimulationFilename +
                    ': Expected message not found \"' + self.expectedString + '\"\n'
                )
        else:
            success = self.stderrFileManager.isEmpty()
            if not success:
                self.outputFileManager.write(
                    'FAILURE with ' + self.currentSimulationFilename +
                    ': Some error messages detected:\n' + self.stderrFileManager.dump() + '\n'
                )

        self.parserTestStepCount = 0

        if success:
            self.outputFileManager.write(
                'OK: ' + os.path.splitext(os.path.basename(self.currentSimulationFilename))[0] + '\n'
            )

    def loadNextWorld(self):
        IndexFileManager(self.cwdPrefix + self.tempWorldCounterFilename).incrementIndex()
        if self.lastSimulation:
            self.simulationQuit(0)
        else:
            newIndex = self.indexFileManager.incrementIndex()
            self.worldLoad(self.simulationFileManager.filenameAtLine(newIndex))

    def run(self):
        """Supervisor run function."""

        basicTimeStep = int(self.getBasicTimeStep())
        if self.isParserTest:
            if not self.initParserTest():
                self.loadNextWorld()
                return
        else:
            receiver = self.getDevice("ts_receiver")
            receiver.enable(basicTimeStep)

        # 30 seconds before executing the next world
        timeout = time.time() + 30

        running_controllers_pid = []
        test_started = False

        self.parserTestStepCount = 0
        while self.step(basicTimeStep) != -1:
            testCompleted = False
            if self.isParserTest:
                self.parserTestStepCount = self.parserTestStepCount + 1
                if self.parserTestStepCount > 5:
                    self.assessParserTest()
                    testCompleted = True
                    print("testCompleted " + str(testCompleted))
            else:
                # wait for controllers start or termination messages
                while receiver.getQueueLength() > 0:
                    data = receiver.getData()
                    dataList = data.split(b" ")
                    if dataList[0] == b"ts":
                        if dataList[1] == b'1':
                            if dataList[2] not in running_controllers_pid:
                                running_controllers_pid.append(dataList[2])
                                test_started = True
                        elif dataList[2] in running_controllers_pid:
                            running_controllers_pid.remove(dataList[2])
                    receiver.nextPacket()
                testCompleted = test_started and \
                    len(running_controllers_pid) == 0

            if testCompleted:
                self.loadNextWorld()
                if self.isParserTest:
                    self.stdoutFileManager.reset()
                    self.stderrFileManager.reset()
                return
            elif time.time() > timeout:
                self.outputFileManager.write(
                    'FAILURE with ' + self.currentSimulationFilename +
                    ': Timeout the results file has not been written ' +
                    'quickly enough\n'
                )
                self.loadNextWorld()
                return


controller = TestSuite()
controller.run()
