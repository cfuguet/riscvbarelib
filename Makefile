##
#  Copyright 2023,2024 CEA*
#  Commissariat a l'Energie Atomique et aux Energies Alternatives
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
##
##
#  @file   Makefile
#  @author Cesar Fuguet
##
ifndef BSP
$(error "Set BSP to the path of the directory of the Board Support Package")
endif

MAKEFILE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
MKDIR = mkdir -p
ECHO = echo
RM = rm -f
CP = cp -f

## ==================================================================
#  Output Directory
O = build

## ==================================================================
#  Include the BSP makefile definitions
include $(BSP)/makefile.bsp.include

## ==================================================================
#  Include paths
COMMON_INCDIRS = include
INCDIRS        = $(COMMON_INCDIRS) $(BSP_INCDIRS)
INCLUDES       = $(addprefix -I,$(INCDIRS))

## ==================================================================
#  Optimization flags
#  OPT_DEBUG = 1 compiles with debugging symbols -Og -g
#  OPT_SPEED = 1 compiles with -O2
#  By default, it compiles for size -Os
OPT_DEBUG = 0
OPT_SPEED = 0

## ==================================================================
#  Compilation flags
CFLAGS  = -ffreestanding \
          -ffunction-sections \
          -fdata-sections \
          -Wall \
          $(BSP_CFLAGS)

CFLAGS += $(if $(filter 1,$(OPT_DEBUG)),-Og -g, \
          $(if $(filter 1,$(OPT_SPEED)),-O2,-Os))

#  Stack size (default size may be overriden from the BSP definitions)
CFLAGS += -DSTACK_SIZE=$(BSP_STACK_SIZE)

## ==================================================================
#  Include objects definition
include common/objects.mk
include $(BSP)/objects.mk

## ==================================================================
#  Target static library
target = $(O)/librvb.a

## ==================================================================
#  Build rules
.PHONY: all
all: $(target) gen-build-mk

.PHONY: gen-build-mk
gen-build-mk:
	sed -e 's|<<__BSP__>>|$(abspath $(BSP))|g' \
		makefile.include.template > $(O)/makefile.include
	$(CP) linkcmds.include $(O)/

#  Build rule for the static library
$(target): $(common-objs-y) $(bsp-objs-y)
	@$(MKDIR) $(dir $@)
	$(AR) rs $@ $^

#  Generic build rule for C files
$(O)/%.o: %.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $^

#  Generic build rule for assembly files
$(O)/%.o: %.S
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $^

#  Generic build rule for C++ files
$(O)/%.o: %.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CFLAGS) $(INCLUDES) -c -o $@ $^

## ==================================================================
#  Clean rule

.PHONY: clean
clean:
	$(RM) -r $(O)
