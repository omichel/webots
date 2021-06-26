#!/usr/bin/env python

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

"""Test suite."""

import sys
import os
import shutil
import platform
import datetime
import getpass
import glob
import subprocess
import threading
import time
import multiprocessing

from command import Command

# monitor failures
failures = 0
systemFailures = []
whitelist = ['ContextResult::kTransientFailure: Failed to send GpuChannelMsg_CreateCommandBuffer']
# parse arguments
filesArguments = []
nomakeOption = False
ansiEscape = True
if len(sys.argv) > 1:
    for arg in sys.argv[1:]:
        if arg == '--nomake':
            nomakeOption = True
        elif arg == '--no-ansi-escape':
            ansiEscape = False
        elif os.path.exists(arg):
            filesArguments.append(arg)
        else:
            raise RuntimeError('Unknown option "' + arg + '"')

testGroups = ['api', 'other_api', 'physics', 'protos', 'parser', 'rendering']

# global files
testsFolderPath = os.path.dirname(os.path.abspath(__file__)) + os.sep
outputFilename = testsFolderPath + 'output.txt'
defaultProjectPath = testsFolderPath + 'default' + os.sep
supervisorControllerName = 'test_suite_supervisor'
protoFileNames = ['TestSuiteSupervisor.proto', 'TestSuiteEmitter.proto']
tempWorldCounterFilename = testsFolderPath + 'world_counter.txt'
webotsStdOutFilename = testsFolderPath + 'webots_stdout.txt'
webotsStdErrFilename = testsFolderPath + 'webots_stderr.txt'

# Webots setup (cf. setupWebots() below)
webotsFullPath = ''
webotsVersion = ''


def setupWebots():
    """Find webots binary thanks to WEBOTS_HOME."""
    os.putenv('WEBOTS_TEST_SUITE', 'TRUE')
    os.putenv('WEBOTS_EMPTY_PROJECT_PATH', defaultProjectPath)

    global webotsFullPath
    global webotsVersion
    global webotsSysInfo

    if sys.platform == 'win32':
        webotsFullPath = os.environ['WEBOTS_HOME'] + os.sep + 'msys64' + \
            os.sep + 'mingw64' + os.sep + 'bin' + os.sep + 'webots.exe'
    else:
        webotsBinary = 'webots'
        if 'WEBOTS_HOME' in os.environ:
            webotsFullPath = os.environ['WEBOTS_HOME'] + os.sep + webotsBinary
        else:
            webotsFullPath = '..' + os.sep + '..' + os.sep + webotsBinary
        if not os.path.isfile(webotsFullPath):
            sys.exit('Error: ' + webotsBinary + ' binary not found')
        webotsFullPath = os.path.normpath(webotsFullPath)

    command = Command(webotsFullPath + ' --version')
    command.run()
    if command.returncode != 0:
        raise RuntimeError('Error when getting the Webots version')
    webotsVersion = command.output.replace('\n', ' ').split(' ')[2].split('.')

    command = Command(webotsFullPath + ' --sysinfo')
    command.run()
    if command.returncode != 0:
        raise RuntimeError('Error when getting the Webots information of the system')
    webotsSysInfo = command.output.split('\n')


def findFirstWorldFilename(worldsFilename):
    """Get the first world file name."""
    file = open(worldsFilename)
    worldFilename = file.readline().strip()
    file.close()
    return worldFilename


def resetIndexFile(indexFilename):
    """Create the index file."""
    file = open(indexFilename, 'w')
    file.write('0\n')
    file.close()


def formatString(s):
    """Add a predefined number of spaces after the ':' character."""
    try:
        index = s.index(': ')
        s0 = '{:<20}'.format(s[0:index])
        s0 += s[index:]
        return s0
    except ValueError:  # can be thrown by string.index()
        return s


def resetOutputFile():
    """Create the output file."""
    file = open(outputFilename, 'w')
    file.write(formatString('Webots binary: ' + webotsFullPath) + '\n')
    file.write(formatString('Webots version: ' + str(webotsVersion)) + '\n')
    file.write(formatString(
        'Operating System: ' + platform.platform() +
        ' [' + platform.machine() + '] ' + platform.processor() +
        ' (' + platform.node() + ')') + '\n'
    )
    file.write(formatString('Date: ' + datetime.datetime.now().ctime()) + '\n')
    file.write(formatString('Tester: ' + getpass.getuser()) + '\n')
    for line in webotsSysInfo:
        file.write(formatString(line) + '\n')
    file.close()


def appendToOutputFile(txt):
    """Append txt to output file."""
    file = open(outputFilename, 'a')
    file.write(txt)
    file.close()


def executeMake():
    """Execute 'make release' to ensure every controller/plugin is compiled."""
    curdir = os.getcwd()
    os.chdir(testsFolderPath)
    command = Command('make release -j%d' % multiprocessing.cpu_count())
    command.run(silent=False)
    os.chdir(curdir)
    if command.returncode != 0:
        raise RuntimeError('Error when executing the Make command')


def generateWorldsList(groupName, worldsFilename):
    """Generate the list of worlds to run."""
    f = open(worldsFilename, 'w')
    worldsCount = 0
    # generate the list from the arguments
    if filesArguments:
        for file in filesArguments:
            if file.startswith(groupName):
                f.write(file + '\n')
        worldsCount = len(filesArguments)

    # generate the list from 'ls worlds/*.wbt'
    else:
        filenames = glob.glob(testsFolderPath + groupName + os.sep + 'worlds' + os.sep + '*.wbt')

        # remove the generic name
        for filename in filenames:
            if filename.endswith('test_suite'):
                filenames.remove(filename)

        # alphabetical order
        filenames.sort()

        # to file
        for filename in filenames:
            # speaker test not working on travis/github action because of missing sound drivers
            if (not filename.endswith('_temp.wbt') and
                    not (('TRAVIS' in os.environ or 'GITHUB_ACTIONS' in os.environ) and filename.endswith('speaker.wbt'))):
                f.write(filename + '\n')
                worldsCount += 1

    f.close()
    return worldsCount


def monitorOutputFile(finalMessage):
    """Display the output file on the console."""
    global monitorOutputCommand
    monitorOutputCommand = Command('tail -f ' + outputFilename, ansiEscape)
    monitorOutputCommand.run(expectedString=finalMessage, silent=False)


if not nomakeOption:
    executeMake()
setupWebots()
resetOutputFile()

finalMessage = 'Test suite complete'
thread = threading.Thread(target=monitorOutputFile, args=[finalMessage])
thread.start()

webotsArguments = '--mode=fast --no-rendering --stdout --stderr --minimize --batch'
if sys.platform != 'win32':
    webotsArguments += ' --no-sandbox'

for groupName in testGroups:

    testFailed = False

    appendToOutputFile('\n### ' + groupName + ' test\n\n')

    # clear stdout and stderr files
    open(webotsStdErrFilename, 'w').close()
    open(webotsStdOutFilename, 'w').close()

    worldsFilename = testsFolderPath + groupName + os.sep + 'worlds.txt'
    indexFilename = testsFolderPath + groupName + os.sep + 'worlds_index.txt'

    # init temporary world counter file
    tempFile = open(tempWorldCounterFilename, 'w')
    tempFile.write('0')
    tempFile.close()

    supervisorTargetDirectory = testsFolderPath + groupName + os.sep + 'controllers' + os.sep + \
        supervisorControllerName
    if not os.path.exists(supervisorTargetDirectory):
        os.makedirs(supervisorTargetDirectory)
    shutil.copyfile(
        defaultProjectPath + 'controllers' + os.sep +
        supervisorControllerName + os.sep +
        supervisorControllerName + '.py',
        supervisorTargetDirectory + os.sep + supervisorControllerName + '.py'
    )
    # parser tests uses a slightly different Supervisor PROTO
    protosTargetDirectory = testsFolderPath + groupName + os.sep + 'protos'
    protosSourceDirectory = defaultProjectPath + 'protos' + os.sep
    if not os.path.exists(protosTargetDirectory):
        os.makedirs(protosTargetDirectory)
    for protoFileName in protoFileNames:
        shutil.copyfile(protosSourceDirectory + protoFileName,
                        protosTargetDirectory + os.sep + protoFileName)
    worldsCount = generateWorldsList(groupName, worldsFilename)
    firstSimulation = findFirstWorldFilename(worldsFilename)
    if not os.path.exists(firstSimulation):
        continue

    resetIndexFile(indexFilename)

    # Here is an example to run webots in gdb and display the stack
    # when it crashes.
    # this is particuarliy useful to debug on the jenkins server
    #  command = Command('gdb -ex run --args ' + webotsFullPath + '-bin ' +
    #                    firstSimulation + ' --mode=fast --no-rendering --minimize')
    #  command.run(silent = False)

    command = Command(webotsFullPath + ' ' + firstSimulation + ' ' + webotsArguments)

    # redirect stdout and stderr to files
    command.runTest(timeout=10 * 60)  # 10 minutes

    if command.isTimeout or command.returncode != 0:
        if command.isTimeout:
            failures += 1
            appendToOutputFile(
                'FAILURE: Webots has been terminated ' +
                'by the test suite script\n')
        else:
            failures += 1
            appendToOutputFile(
                'FAILURE: Webots exits abnormally with this error code: ' +
                str(command.returncode) + '\n')
        testFailed = True
    else:
        # check count of executed worlds
        tempFile = open(tempWorldCounterFilename)
        counterString = tempFile.read()
        tempFile.close()
        if int(counterString) < worldsCount:
            testFailed = True
            appendToOutputFile('FAILURE: Some tests have not been executed\n')
            appendToOutputFile('- expected number of worlds: %d\n' % (worldsCount))
            appendToOutputFile('- number of worlds actually tested: %s)\n' % (counterString))
        else:
            lines = open(webotsStdErrFilename, 'r').readlines()
            for line in lines:
                if 'Failure' in line:
                    # check if it should be ignored
                    if not any(item in line for item in whitelist):
                        failures += 1
                        systemFailures.append(line)

    if testFailed:
        appendToOutputFile('\nWebots complete STDOUT log:\n')
        with open(webotsStdOutFilename) as f:
            for line in f:
                appendToOutputFile(line)
        appendToOutputFile('\nWebots complete STDERR log:\n')
        with open(webotsStdErrFilename) as f:
            for line in f:
                appendToOutputFile(line)
                if '(core dumped)' in line:
                    l = line[0:line.find(' Segmentation fault')]
                    pid = int(l[l.rfind(' ') + 1:])
                    core_dump_file = '/tmp/core_webots-bin.' + str(pid)
                    if os.path.exists(core_dump_file):
                        appendToOutputFile(subprocess.check_output([
                            'gdb', '--batch', '--quiet', '-ex', 'bt', '-ex',
                            'quit', '../bin/webots-bin', core_dump_file
                        ]))
                        os.remove(core_dump_file)
                    else:
                        appendToOutputFile(
                            'Cannot get the core dump file: "%s" does not exist.' % core_dump_file
                        )

appendToOutputFile('\n' + finalMessage + '\n')

if len(systemFailures) > 0:
    appendToOutputFile('\nSystem Failures:\n')
    for message in systemFailures:
        appendToOutputFile(message)

time.sleep(1)
if monitorOutputCommand.isRunning():
    monitorOutputCommand.terminate(force=True)

with open(outputFilename, 'r') as file:
    content = file.read()
    failures += content.count('FAILURE ')

sys.exit(failures)
