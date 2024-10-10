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
#  @file   common/objects.mk
#  @author Cesar Fuguet
##
common-objs-y =
common-objs-y += $(O)/common/bitset.o
common-objs-y += $(O)/common/fifobuf.o
common-objs-y += $(O)/common/mem.o
common-objs-y += $(O)/common/mp.o
common-objs-y += $(O)/common/spin_mutex.o
common-objs-y += $(O)/common/syscall.o
common-objs-y += $(O)/common/threads.o
common-objs-y += $(O)/common/ticket_mutex.o
common-objs-y += $(O)/common/trap_entry.o
common-objs-y += $(O)/common/trap_handler.o
