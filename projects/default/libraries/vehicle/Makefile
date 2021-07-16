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

null :=
space := $(null) $(null)
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
include $(WEBOTS_HOME_PATH)/resources/Makefile.os.include

.PHONY: release debug profile clean c/car c/driver cpp/car cpp/driver python java

release debug profile clean: java python

java: cpp/driver cpp/car
	+@echo "# make" $(MAKECMDGOALS) $@
	+@make -s -C $@ $(MAKECMDGOALS)

python: cpp/driver cpp/car
ifeq ($(OSTYPE),linux)
	@echo "# make" $@ python3.6;
	+PYTHON_COMMAND=python3.6 make -s -C python $(MAKECMDGOALS)
	@echo "# make" $@ python3.7;
	+PYTHON_COMMAND=python3.7 make -s -C python $(MAKECMDGOALS)
	@echo "# make" $@ python3.8;
	+PYTHON_COMMAND=python3.8 make -s -C python $(MAKECMDGOALS)
	@echo "# make" $@ python3.9;
	+PYTHON_COMMAND=python3.9 make -s -C python $(MAKECMDGOALS)
endif
ifeq ($(OSTYPE),windows)
	@echo "# make" $@ python3.9;
	+PATH="$(PYTHON39_HOME):$(PATH)" make -s -C python $(MAKECMDGOALS)
	@echo "# make" $@ python3.8;
	+PATH="$(PYTHON38_HOME):$(PATH)" make -s -C python $(MAKECMDGOALS)
	@echo "# make" $@ python3.7;
	+PATH="$(PYTHON37_HOME):$(PATH)" make -s -C python $(MAKECMDGOALS)
endif
ifeq ($(OSTYPE),darwin)
	@echo "# make" $@ python3.7;
	+PYTHON_COMMAND=python3.7 make -s -C python $(MAKECMDGOALS)
	@echo "# make" $@ python3.8;
	+PYTHON_COMMAND=python3.8 make -s -C python $(MAKECMDGOALS)
	@echo "# make" $@ python3.9;
	+PYTHON_COMMAND=python3.9 make -s -C python $(MAKECMDGOALS)
endif


cpp/driver: c/driver
	+@echo "# make" $(MAKECMDGOALS) $@
	+@make -s -C $@ $(MAKECMDGOALS)

cpp/car c/driver: c/car
	+@echo "# make" $(MAKECMDGOALS) $@
	+@make -s -C $@ $(MAKECMDGOALS)

c/car:
	+@echo "# make" $(MAKECMDGOALS) $@
	+@make -s -C c/car $(MAKECMDGOALS)
