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
#  @file   ariane_testharness/objects.mk
#  @author Cesar Fuguet
##
bsp-objs-y += $(O)/bsp_init.o
bsp-objs-y += $(O)/bsp_irq.o
bsp-objs-y += $(O)/bsp_tohost.o
bsp-objs-y += $(O)/bsp/shared/crt0.o
bsp-objs-y += $(O)/bsp/shared/bsp_start.o
bsp-objs-y += $(O)/drivers/clint/clint.o
bsp-objs-y += $(O)/drivers/uart16550/uart16550.o

VPATH += $(BSP)
VPATH += $(RVB_HOME)/bsp/shared
VPATH += $(RVB_HOME)/drivers
