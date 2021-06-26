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

ifeq ($(WEBOTS_HOME),)
WEBOTS_HOME_PATH = ../../..
else
null :=
space := $(null) $(null)
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
endif

include $(WEBOTS_HOME_PATH)/resources/Makefile.os.include

PYTHON_OK := $(shell which python3)
release debug profile:
	@echo "# generating matlab"
ifeq ('$(PYTHON_OK)','')
	@echo "# \033[0;33mUnable to find python3, skipping matlab API\033[0m"
else
	$(shell python3 mgenerate.py;)
endif 

clean:
	for name in $$(cat $(WEBOTS_CONTROLLER_LIB_PATH)/matlab/.gitignore); do rm -f $(WEBOTS_CONTROLLER_LIB_PATH)/matlab/$$name; done;
