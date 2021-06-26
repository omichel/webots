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

default:
	@echo "Type one of the following (OSTYPE is \""$(OSTYPE)"\"):"
	@echo "  make debug                 for debug (with gdb symbols)"
	@echo "  make profile               for profiling (with gprof information)"
	@echo "  make release               for finale release"
	@echo "  make clean                 remove build directory"
	@echo "  make count-lines           count the lines of code of C controller library"

count-lines:
	wc -l *.c* *.h*

include $(WEBOTS_HOME_PATH)/resources/Makefile.include

WEBOTS_DEPENDENCY_PATH ?= $(WEBOTS_HOME_PATH)/dependencies

ifeq ($(OSTYPE),darwin)
CFLAGS      = -mmacosx-version-min=$(MACOSX_MIN_SDK_VERSION) -fpascal-strings -Wno-deprecated-declarations -I/Developer/Headers/FlatCarbon -Wall -fno-common
INCLUDE     = -I$(WEBOTS_HOME_PATH)/include/controller/c -I$(WEBOTS_HOME_PATH)/include
LD_FLAGS    = -mmacosx-version-min=$(MACOSX_MIN_SDK_VERSION) -dynamiclib -install_name @rpath/lib/controller/libController.dylib -compatibility_version 1.0 -current_version 1.0.0
SHARED_LIBS = -lz
TARGET      = $(WEBOTS_CONTROLLER_LIB_PATH)/libController.dylib
CC          = gcc
endif

ifeq ($(OSTYPE),linux)
CFLAGS      = -fPIC -Wall -Wno-stringop-truncation
INCLUDE     = -I$(WEBOTS_HOME_PATH)/include/controller/c -I$(WEBOTS_HOME_PATH)/include
LD_FLAGS    = -shared -Wl,-soname,libController.so
SHARED_LIBS = -lm -lpthread -ldl -lrt
TARGET      = $(WEBOTS_CONTROLLER_LIB_PATH)/libController.so
endif

ifeq ($(OSTYPE),windows)
CFLAGS      = -Wall -mwindows
INCLUDE     = -I$(WEBOTS_HOME_PATH)/include/controller/c -I$(WEBOTS_HOME_PATH)/include -I/mingw64/include
LD_FLAGS    = -shared -mwindows --def Controller.def
PLATFORM    = win32
CC          = gcc
TARGET      = $(WEBOTS_CONTROLLER_LIB_PATH)/Controller.dll $(WEBOTS_CONTROLLER_LIB_PATH)/Controller.lib
ifneq (,$(wildcard /mingw32/bin/gcc))
TARGET     += $(WEBOTS_HOME_PATH)/msys64/mingw32/bin/Controller.dll $(WEBOTS_HOME_PATH)/msys64/mingw32/lib/Controller.lib
endif
endif

ifeq ($(MAKECMDGOALS),debug)
CFLAGS += -g
endif

ifeq ($(TREAT_WARNINGS_AS_ERRORS),1)
CFLAGS += -Werror
endif

ifeq ($(MAKECMDGOALS),profile)
CFLAGS += -pg -O3 -DNDEBUG
LD_FLAGS += -pg
endif

OBJDIR = build/$(MAKECMDGOALS)
ifneq (,$(findstring $(MAKECMDGOALS),debug profile release))
BUILD_PATH_SETUP := $(shell mkdir -p $(OBJDIR))
ifeq ($(OSTYPE),windows)
BUILD_PATH_SETUP := $(shell mkdir -p $(OBJDIR)/mingw32)
endif
endif

ifdef EXTCONTROLLER
CFLAGS += -DEXTCONTROLLER
endif

ifeq ($(MAKECMDGOALS),release)
CFLAGS   += -O3 -DNDEBUG
ifneq ($(OSTYPE),darwin)
LD_FLAGS += -s
endif
endif

vpath %.o $(OBJDIR)
vpath %.c .

INCLUDE += -isystem $(WEBOTS_HOME_PATH)/src/stb

SOURCES=$(shell ls *.c)

OBJECTS=$(notdir $(SOURCES:.c=.o))

# clear out all suffixes for implicit rules to speed up Makefile
.SUFFIXES:
%:: %,v
%:: RCS/%,v
%:: RCS/%
%:: s.%
%:: SCCS/s.%

ifneq (,$(filter $(MAKECMDGOALS),debug release profile)) # if $(MAKECMDGOALS) is either of those
-include $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.c=.d)))
endif

debug profile release: $(TARGET)

ifeq ($(OSTYPE),windows)

MINGW64_OBJECTS = $(addprefix $(OBJDIR)/, $(OBJECTS))
MINGW32_OBJECTS = $(addprefix $(OBJDIR)/mingw32/, $(OBJECTS))

$(WEBOTS_CONTROLLER_LIB_PATH)/Controller.dll: $(MINGW64_OBJECTS) Controller.def
	@echo "# linking  "$@
	@echo "# linking  "$(WEBOTS_CONTROLLER_LIB_PATH)"/libController.a"
	@$(CC) $(LD_FLAGS) -o $@ $(addprefix $(OBJDIR)/, $(OBJECTS)) -Wl,--out-implib,$(WEBOTS_CONTROLLER_LIB_PATH)/libController.a
ifeq ($(NO_GCC32),1)
	@echo -e "# \033[0;33mmissing 32 bit gcc or dependencies, skipping 32 bit version of Controller.dll\033[0m"
endif

$(WEBOTS_CONTROLLER_LIB_PATH)/Controller.lib: $(WEBOTS_CONTROLLER_LIB_PATH)/Controller.dll Controller.def
ifeq ($(VISUAL_STUDIO_PATH),)
	@echo -e "# \033[0;33m'VISUAL_STUDIO_PATH' not set, skipping Controller.lib\033[0m"
else
	@echo "# creating "$@
	@PATH="$(VISUAL_STUDIO_PATH)/BuildTools/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64":$PATH lib /machine:X64 /def:Controller.def /out:Controller.lib > /dev/null
	@mv Controller.lib "$(WEBOTS_CONTROLLER_LIB_PATH)"
	@rm Controller.exp
endif

$(WEBOTS_HOME_PATH)/msys64/mingw32/bin/Controller.dll: $(MINGW32_OBJECTS) Controller.def
	@echo "# linking  "$@" (32 bit)"
	@echo "# linking  "$(WEBOTS_HOME_PATH)"/msys64/mingw32/lib/libController.a (32 bit)"
	@PATH=/mingw32/bin $(CC) -L/mingw32/lib $(LD_FLAGS) -o $@ $(addprefix $(OBJDIR)/mingw32/, $(OBJECTS)) -Wl,--out-implib,$(WEBOTS_HOME_PATH)/msys64/mingw32/lib/libController.a

$(WEBOTS_HOME_PATH)/msys64/mingw32/lib/Controller.lib: $(WEBOTS_HOME_PATH)/msys64/mingw32/bin/Controller.dll Controller.def
ifeq ($(VISUAL_STUDIO_PATH),)
	@echo -e "# \033[0;33m'VISUAL_STUDIO_PATH' not set, skipping Controller.lib (32 bit)\033[0m"
else
	@echo "# creating "$@" (32 bit)"
	@PATH="$(VISUAL_STUDIO_PATH)/BuildTools/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64":$PATH lib /machine:X86 /def:Controller.def /out:Controller32.lib > /dev/null
	@mv Controller32.lib "$(WEBOTS_HOME_PATH)/msys64/mingw32/lib/Controller.lib"
	@rm Controller32.exp
endif

else

$(TARGET): $(OBJECTS)
	@echo "# linking "$@
	@$(CC) $(LD_FLAGS) -o $(TARGET) $(addprefix $(OBJDIR)/, $(OBJECTS)) $(SHARED_LIBS)
	@chmod a-x $(TARGET)
endif

$(OBJDIR)/%.o:%.c
	@echo "# compiling "$<
	@$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDE) $< -o $@

$(OBJDIR)/mingw32/%o:%c
	@echo "# compiling "$<" (32 bit)"
	@PATH=/mingw32/bin $(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDE) $< -o $(OBJDIR)/mingw32/$(notdir $@)

$(OBJDIR)/%.d:%.c
	@echo "# updating "$(notdir $@)
	@echo $(basename $(OBJDIR)/$(notdir $@)).o: $(basename $@).d > $(OBJDIR)/$(notdir $@)
	@printf "%s" "$(OBJDIR)/" >> $(OBJDIR)/$(notdir $@)
	@$(CC) $(CFLAGS) -MM -w $(INCLUDE) $< >> $(OBJDIR)/$(notdir $@)

FILES_TO_REMOVE = $(TARGET)
FILES_TO_REMOVE += $(WEBOTS_CONTROLLER_LIB_PATH)/libController.a
FILES_TO_REMOVE += $(WEBOTS_CONTROLLER_LIB_PATH)/libController.lib
FILES_TO_REMOVE += $(WEBOTS_HOME_PATH)/msys64/mingw32/bin/Controller.dll
FILES_TO_REMOVE += $(WEBOTS_HOME_PATH)/msys64/mingw32/lib/libController.a
FILES_TO_REMOVE += $(WEBOTS_HOME_PATH)/msys64/mingw32/lib/Controller.lib
