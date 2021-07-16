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

# WEBOTS_HOME is a sine qua non condition to run this Makefile
ifndef WEBOTS_HOME
 $(error The WEBOTS_HOME environment variable is not defined.)
endif

ifndef WEBOTS_HOME_PATH
 null :=
 space := $(null) $(null)
 WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
endif

include $(WEBOTS_HOME_PATH)/resources/Makefile.os.include

# compiler
ifeq ($(CC),cc)
 CC = gcc
endif

ifeq ($(OSTYPE),darwin)
WEBOTS_DISTRO_EXE = webots_distro
WEBOTS = $(WEBOTS_HOME_PATH)/webots
# we want to avoid a deprecation warning related to the MD5 library
CFLAGS = -Wno-deprecated-declarations
endif

ifeq ($(OSTYPE),linux)
LIBRARIES = -lcrypto
WEBOTS_DISTRO_EXE = webots_distro
WEBOTS = $(WEBOTS_HOME_PATH)/webots
ifeq ($(UBUNTU_VERSION),16.04)
CFLAGS = -DWEBOTS_UBUNTU_16_04
endif
ifeq ($(UBUNTU_VERSION),18.04)
CFLAGS = -DWEBOTS_UBUNTU_18_04
endif
ifeq ($(UBUNTU_VERSION),20.04)
CFLAGS = -DWEBOTS_UBUNTU_20_04
endif
endif

ifneq ($(SNAPCRAFT_PROJECT_NAME),)
ifeq ($(DISPLAY),)
export DISPLAY = :99
XVFB = xvfb
endif
endif

ifeq ($(OSTYPE),windows)
LIBRARIES = -L$(WEBOTS_HOME_PATH)/lib -L/mingw64/bin -lcrypto
WEBOTS_DISTRO_EXE = webots_distro.exe
INNO_SETUP_HOME ?= /C/Program Files (x86)/Inno Setup 6
WEBOTS = webots
ISCC_FORMATTER = iscc_formatter.exe
endif

TARGET_URL = $(shell cat $(WEBOTS_HOME_PATH)/scripts/packaging/webots_version.txt)
TAGS := $(shell git fetch --all --tags)
ifeq ($(shell git tag --points-at HEAD | grep $(TARGET_URL)),)
TARGET_URL = $(shell cat $(WEBOTS_HOME_PATH)/resources/commit.txt)
endif

all: $(WEBOTS_DISTRO_EXE) $(ISCC_FORMATTER) $(XVFB)
	+@echo "# removing files and folders not in git repository"
	+@cd $(WEBOTS_HOME_PATH) ; git clean -fdf | sed -e 's/^/# - /' ; cd scripts/packaging
	+@echo "# generating project files"
	+@./generate_projects_files.py > files_projects.txt
	+@echo "# changing URLs in world, proto and controller files"
	+@python3 update_urls.py $(TARGET_URL)
	+@echo "# recompiling controller files"
	+@make -s -f Makefile.controllers
	+@echo "# updating proto cache"
	+@$(WEBOTS) --update-proto-cache
ifeq ($(OSTYPE),darwin)
	+@echo "# checking RPATH system"
	+@./check_rpath.py
endif
	+@echo "# checking submodules update"
	+@./check_submodules_update.sh
ifeq ($(OSTYPE),windows)
	+@echo "# preparing msys64 folders and files"
	+@./copy_msys64_files.py
endif
	+@echo "# building distribution source"
	+@./webots_distro
	+@rm files_projects.txt
ifeq ($(OSTYPE),windows)
	+@echo "# creating webots_setup.exe (takes long)"
	+@"$(INNO_SETUP_HOME)/iscc" -Qp webots.iss | ./iscc_formatter.exe `tput cols`
	+@echo
endif
	+@echo "# done for" $(OSTYPE)

install: all
	./webots.snap

$(WEBOTS_DISTRO_EXE): webots_distro.c
	+@echo "# compiling webots_distro.c"
	+@$(CC) $(CFLAGS) -Wall -o $@ webots_distro.c $(LIBRARIES)

iscc_formatter.exe: iscc_formatter.c
	+@echo "# compiling iscc_formatter.c"
	+@$(CC) -Wall -o $@ iscc_formatter.c

xvfb:
	+@echo "# starting Xvfb: X Virtual Frame Buffer"
	+@Xvfb :99 -screen 0 1024x768x16 &

clean:
	+@rm -rf $(WEBOTS_DISTRO_EXE) webots.deb webots.snap webots.iss webots.mac files.txt iscc_formatter.exe
	+@find $(WEBOTS_HOME_PATH)/projects $(WEBOTS_HOME_PATH)/resources -name .*.cache | xargs rm -f
