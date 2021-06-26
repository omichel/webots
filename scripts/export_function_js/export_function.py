#!/usr/bin/env python3

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

import os
try:
    from pyclibrary import CParser

    listheaders = os.listdir("../../include/wren/")
    buggyheaders = {"config.h", "drawable_texture.h", "file_import.h", "font.h", "overlay.h"}
    listheaders = ["../../include/wren/" + header for header in listheaders if header
                   [len(header)-2:len(header)] == '.h' and not(header in buggyheaders)]

    parser = CParser(listheaders)

    parser.process_all()

    # FUNCTIONS
    functionSignatures = parser.defs['functions']
    functionName = functionSignatures.keys()
    functionName = map(lambda name: "_" + name + ", ", functionName)

    if os.path.exists("../../src/wren/functions_to_export.txt"):
        os.remove("../../src/wren/functions_to_export.txt")

    functionName = list(functionName) + ["_wr_config_enable_point_size, _wr_config_get_line_scale, " +
                                         "_wr_config_get_max_active_directional_light_count, " +
                                         " _wr_config_get_max_active_point_light_count, " +
                                         "_wr_config_get_max_active_spot_light_count, _wr_config_enable_shadows"]

    # The next lines are not needed as long as we add manually a last function
    # lastIndex= len(functionName) - 1
    # lastName = functionName[lastIndex];
    # lastName = lastName[:len(lastName)-2]
    # functionName[lastIndex] = lastName;

    f = open("../../src/wren/functions_to_export.txt", 'w')

    f.write(''.join(functionName))

    f.close()

    # ENUM

    all_values = parser.defs['values']

    # Eliminate the include guard
    all_values = [value[0] + " : " + str(value[1]) + ", \n" for value in all_values.items() if not ("_H" in value[0])]

    if os.path.exists("../../resources/web/wwi/enum.js"):
        os.remove("../../resources/web/wwi/enum.js")

    f = open("../../resources/web/wwi/enum.js", 'w')

    values_string = ''.join(all_values)
    values_string = values_string[:len(values_string) - 3]
    f.write("const Enum = {\n" + values_string + "}")

    f.close()

    print("OK")

except ImportError:
    print("Fail to import pyclibrary")
