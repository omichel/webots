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

WEBOTS_HOME ?= ../..

null :=
space := $(null) $(null)
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
include $(WEBOTS_HOME_PATH)/resources/Makefile.os.include

WEBOTS_DEPENDENCY_PATH ?= $(WEBOTS_HOME_PATH)/dependencies

STATIC_LIBRARY = true
CFLAGS = -std=c++11 -DGLM_ENABLE_EXPERIMENTAL
INCLUDE = -I$(WEBOTS_HOME_PATH)/include -I$(WEBOTS_HOME_PATH)/include/glad -I../glm -I../webots/external/siphash/

ifeq ($(OSTYPE),darwin)
INCLUDE += -I$(WEBOTS_DEPENDENCY_PATH)/freetype-2.9/include -I$(WEBOTS_DEPENDENCY_PATH)/assimp/include
else ifeq ($(OSTYPE),linux)
INCLUDE += -I/usr/include/freetype2 -I$(WEBOTS_HOME_PATH)/include/libassimp/include
else ifeq ($(OSTYPE),windows)
INCLUDE += -I/mingw64/include/freetype2 -I/mingw64/include
endif

CXX_SOURCES = $(wildcard *.cpp) $(wildcard mesh/*.cpp)

PYTHON3_OK = $(shell type python3;)
ifneq ($(PYTHON3_OK),)
ifneq ($(EMSDK),)
# Check if pyclibrary is present
PYC = $(shell \
	pip list 2>/dev/null >> temp.txt; \
	grep pyclibrary temp.txt | awk '{print $$1}'; \
	rm temp.txt; \
	)
ifneq ($(PYC), pyclibrary) 
PYC = $(shell \
	pip3 list 2>/dev/null >> temp.txt; \
	grep pyclibrary temp.txt | awk '{print $$1}'; \
	rm temp.txt; \
	)
endif
ifeq ($(PYC), pyclibrary) 
WREN=1
# Parse the headers for WRENJS
ENUM := $(shell python $(WEBOTS_HOME_PATH)/scripts/export_function_js/export_function.py;)
ifneq ($(ENUM), OK)
ENUM := $(shell python3 $(WEBOTS_HOME_PATH)/scripts/export_function_js/export_function.py;)
endif
COPY:= $(shell mkdir $(WEBOTS_HOME_PATH)/resources/web/wwi/images/post_processing/ > /dev/null 2>&1) 
COPY:= $(shell cp $(WEBOTS_HOME_PATH)/resources/wren/textures/gtao_noise_texture.png $(WEBOTS_HOME_PATH)/resources/web/wwi/images/post_processing/)
COPY:= $(shell cp $(WEBOTS_HOME_PATH)/resources/wren/textures/smaa_area_texture.png $(WEBOTS_HOME_PATH)/resources/web/wwi/images/post_processing/)
COPY:= $(shell cp $(WEBOTS_HOME_PATH)/resources/wren/textures/smaa_search_texture.png $(WEBOTS_HOME_PATH)/resources/web/wwi/images/post_processing/)
endif
endif
endif

include $(WEBOTS_HOME_PATH)/resources/Makefile.include
