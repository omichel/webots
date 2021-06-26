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

# Webots Makefile system
#
# You may add some variable definitions hereafter to customize the build process
# See documentation in $(WEBOTS_HOME_PATH)/resources/Makefile.include

null :=
space := $(null) $(null)
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
LIBRARIES = -lprotobuf -ljpeg
CXX_SOURCES = player.cpp messages.pb.cpp
CFLAGS = -Wno-array-bounds
FILES_TO_REMOVE = messages.pb.* client.exe client test_client.exe test_client
ifeq ($(OS),Windows_NT)
# on Windows, need to link with WinSock2
LIBRARIES += -lws2_32
endif
ifeq ($(MAKECMDGOALS),)
GOAL = release
else
GOAL := $(MAKECMDGOALS)
endif

defaut: release

release debug profile: clients

clients: build/$(GOAL)/messages.pb.o messages.pb.h build/$(GOAL)/robot_client.o
	$(MAKE) -f Makefile.client $(MAKECMDGOALS)
	$(MAKE) -f Makefile.test_client $(MAKECMDGOALS)

messages.pb.cpp message.pb.h:	messages.proto
	@echo "#" compiling $<
	@protoc $< --cpp_out=.
	@mv messages.pb.cc messages.pb.cpp

### Do not modify: this includes Webots global Makefile.include
include $(WEBOTS_HOME_PATH)/resources/Makefile.include
