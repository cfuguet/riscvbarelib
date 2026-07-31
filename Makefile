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
RVB_HOME = $(MAKEFILE_DIR)
XLEN ?= 64
RISCV_PREFIX ?= riscv$(XLEN)-unknown-elf-
BSP_FLOAT ?= 1
BSP_COMPRESSED ?= 1
BSP_ATOMIC ?= 1
BSP_ZICBOM ?= 0
BSP_NCPUS ?= 1
ifdef BSP
include $(BSP)/makefile.bsp.include
endif

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
ifdef BSP
include $(BSP)/objects.mk
endif

## ==================================================================
#  Target static library
target = $(O)/librvb.a

## ==================================================================
#  Build rules
.PHONY: all
ifdef BSP
all: $(target) gen-build-mk
	@echo "[INFO] RISC-V Bare Library (librvb) generated into $(abspath $(O))"
	@echo "[INFO]   XLEN=$(XLEN)"
	@echo "[INFO]   BSP_FLOAT=$(BSP_FLOAT)"
	@echo "[INFO]   BSP_COMPRESSED=$(BSP_COMPRESSED)"
	@echo "[INFO]   BSP_ATOMIC=$(BSP_ATOMIC)"
	@echo "[INFO]   BSP_ZICBOM=$(BSP_ZICBOM)"
	@echo "[INFO]   BSP_NCPUS=$(BSP_NCPUS)"
else
all: help
endif

.PHONY: help
help:
	@echo "make BSP=<path> [<options>]"
	@echo "options:"
	@echo "  BSP=<path>               Path to the Board Support Package directory (mandatory)"
	@echo "  [O=<path>]               Output path for output library and binaries"
	@echo "                           (default: $(O))"
	@echo "  [XLEN=<xlen_value>]      32/64/128 (default: $(XLEN))"
	@echo "  [RISCV_PREFIX=<prefix>]  RISC-V cross-compiler prefix"
	@echo "                           (default: $(RISCV_PREFIX))"
	@echo "  [BSP_FLOAT=<0|1>]        Support float F and D extensions"
	@echo "                           (default: $(BSP_FLOAT))"
	@echo "  [BSP_COMPRESSED=<0|1>]   Support compress C extension"
	@echo "                           (default: $(BSP_COMPRESSED))"
	@echo "  [BSP_ATOMIC=<0|1>]       Support atomic A extension"
	@echo "                           (default: $(BSP_ATOMIC))"
	@echo "  [BSP_ZICBOM=<0|1>]       Support zicbom extension / cache block management operations"
	@echo "                           (default: $(BSP_ZICBOM))"
	@echo "  [BSP_NCPUS=<n>]          Number of RISC-V harts in the platform"
	@echo "                           (default: $(BSP_NCPUS))"

.PHONY: gen-build-mk
gen-build-mk:
	@sed -e 's|<<__RVB_HOME__>>|$(RVB_HOME)|g' \
	    -e 's|<<__XLEN__>>|$(XLEN)|g' \
	    -e 's|<<__RISCV_PREFIX__>>|$(RISCV_PREFIX)|g' \
	    -e 's|<<__BSP__>>|$(abspath $(BSP))|g' \
	    -e 's|<<__BSP_FLOAT__>>|$(BSP_FLOAT)|g' \
	    -e 's|<<__BSP_COMPRESSED__>>|$(BSP_COMPRESSED)|g' \
	    -e 's|<<__BSP_ATOMIC__>>|$(BSP_ATOMIC)|g' \
	    -e 's|<<__BSP_ZICBOM__>>|$(BSP_ZICBOM)|g' \
	    -e 's|<<__BSP_NCPUS__>>|$(BSP_NCPUS)|g' \
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
