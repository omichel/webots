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

"""Test that all the required Matlab functions are defined."""
import unittest
import os
import sys
sys.path.append(os.environ['WEBOTS_HOME'] + "/src/controller/matlab/")
import mgenerate  # noqa: E402


class TestMatlabFunctions(unittest.TestCase):
    """Unit test for checking that all the required Matlab functions are defined."""

    def setUp(self):
        if sys.version_info[0] >= 3:
            mgenerate.UPDATE = False
            mgenerate.main()
            """Get all the required function."""
            skippedLines = [
                'wbr',
                'microphone',
                'remote_control',
                'robot',
                'wb_device_get_type',
                'wb_node_get_name',
                'wbu_string',
                'lookup_table_size',
                'EXPORTS'
            ]
            self.functions = []
            filename = os.environ['WEBOTS_HOME'] + '/src/controller/c/Controller.def'
            self.assertTrue(
                os.path.isfile(filename),
                msg='Missing "%s" file.' % filename
            )
            with open(filename) as file:
                for line in file:
                    if not any(skippedLine in line for skippedLine in skippedLines):
                        self.functions.append(line.replace('\n', ''))

    @unittest.skipIf(sys.version_info[0] < 3, "not supported by Python 2.7")
    def test_matlab_function_exists(self):
        """Test that the function file exists."""
        for function in self.functions:
            filename = os.environ['WEBOTS_HOME'] + '/lib/controller/matlab/' + function + '.m'
            self.assertTrue(
                os.path.isfile(filename),
                msg='Missing "%s" file.' % filename
            )


if __name__ == '__main__':
    unittest.main()
