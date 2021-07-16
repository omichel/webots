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

TARGETS=robotis-op2.Makefile managers.Makefile python.Makefile

.PHONY: release debug profile clean

release debug profile: $(TARGETS)

clean: $(TARGETS)
	# Remove python/*managers.* except python/managers.i
	ls -1 python/*managers.* | grep -v managers.i | xargs rm -f

managers.Makefile: robotis-op2.Makefile

managers.Makefile robotis-op2.Makefile:
	+@echo "# make" $(MAKECMDGOALS) $(@:.Makefile=)
	+@make -s -C $(@:.Makefile=) $(MAKECMDGOALS)

python.Makefile: managers.Makefile
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
	+make -f Makefile.mac $(MAKECMDGOALS)
	@echo "# make" $@ python3.9;
	+PYTHON_COMMAND=python3.9 make -s -C python $(MAKECMDGOALS)
	+make -f Makefile.mac $(MAKECMDGOALS)
endif
