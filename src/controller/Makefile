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

include ../../resources/Makefile.os.include

.PHONY: release debug profile

release debug profile clean:
	@echo "#"
	@echo "# C controller library ("$@")"
	@echo "#"
	@+make -s -C c $@
	@echo "#"
	@echo "# C++ controller library ("$@")"
	@echo "#"
	@+make -s -C cpp $@
	@echo "#"
	@echo "# Java controller library ("$@")"
	@echo "#"
	@+make -s -C java $@
	@echo "# Matlab controller library ("$@")"
	@echo "#"
	@+make -s -C matlab $@
ifeq ($(OSTYPE),linux)
	@echo "#"
	@echo "# Python 2.7 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=python2.7 make -s -C python $@
ifeq ($(UBUNTU_VERSION), 16.04)
	@echo "#"
	@echo "# Python 3.5 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=python3.5 make -s -C python $@
endif
	@echo "#"
	@echo "# Python 3.6 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=python3.6 make -s -C python $@
	@echo "#"
	@echo "# Python 3.7 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=python3.7 make -s -C python $@
	@echo "#"
	@echo "# Python 3.8 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=python3.8 make -s -C python $@
	@echo "#"
	@echo "# Python 3.9 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=python3.9 make -s -C python $@
endif
ifeq ($(OSTYPE),windows)
	@echo "#"
	@echo "# Python 3.9 controller library ("$@")"
	@echo "#"
	@+PATH="$(PYTHON39_HOME):$(PATH)" make -s -C python $@
	@echo "#"
	@echo "# Python 3.8 controller library ("$@")"
	@echo "#"
	@+PATH="$(PYTHON38_HOME):$(PATH)" make -s -C python $@
	@echo "#"
	@echo "# Python 3.7 controller library ("$@")"
	@echo "#"
	@+PATH="$(PYTHON37_HOME):$(PATH)" make -s -C python $@
	@echo "#"
	@echo "# Python 2.7 controller library ("$@")"
	@echo "#"
	@+PATH="$(PYTHON27_HOME):$(PATH)" make -s -C python $@
endif
ifeq ($(OSTYPE),darwin)
	@echo "#"
	@echo "# Python 2.7 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=/usr/bin/python2.7 make -s -C python $@
	@echo "#"
	@echo "# Python 3.7 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=/Library/Frameworks/Python.framework/Versions/3.7/bin/python3.7 make -s -C python $@
	@echo "#"
	@echo "# Python 3.8 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=/Library/Frameworks/Python.framework/Versions/3.8/bin/python3.8 make -s -C python $@
	@echo "#"
	@echo "# Python 3.9 controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=/Library/Frameworks/Python.framework/Versions/3.9/bin/python3.9 make -s -C python $@
	@echo "#"
	@echo "# Python 3.7 (brew) controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=/usr/local/opt/python@3.7/bin/python3.7 SUFFIX=_brew make -s -C python $@
	@echo "#"
	@echo "# Python 3.8 (brew) controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=/usr/local/opt/python@3.8/bin/python3.8 SUFFIX=_brew make -s -C python $@
	@echo "#"
	@echo "# Python 3.9 (brew) controller library ("$@")"
	@echo "#"
	@+PYTHON_COMMAND=/usr/local/opt/python@3.9/bin/python3.9 SUFFIX=_brew make -s -C python $@
endif
