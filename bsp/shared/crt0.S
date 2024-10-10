/**
 * Copyright 2023,2024 CEA*
 * Commissariat a l'Energie Atomique et aux Energies Alternatives
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 *  @file   shared/crt0.c
 *  @author Cesar Fuguet
 */
#include <common/cpu_defs.h>
#include <bsp/bsp_config.h>

.section .start,"ax",@progbits

.extern bsp_start

.globl _start
.align 3

#if   (__riscv_xlen == 32)
#define AMOADD amoadd.w
#define LD     lw
#elif (__riscv_xlen == 64)
#define AMOADD amoadd.d
#define LD     ld
#endif

_start:
    #  Disable interrupts by default (global interrupt enable)
    csrrci  zero, mstatus, MSTATUS_MIE

    #  Specify the base address of the trap handler.
    #
    #  The trap_entry routine must be aligned to 4 bytes so the 2 lsb of the
    #  address are 0. The 2 lsb of mtvec are the MODE:
    #    MODE = 0 -> Direct mode (non-vectored).
    la      t0, trap_entry
    csrrw   x0, mtvec, t0

    #  Initialize the stack pointer
    #
    #  Each core atomically fetch and add a global pointer that indicates the
    #  base address of the stack.
    la       t0, _stack_pointer

#if BSP_CONFIG_NCPUS > 1
    li       t1, STACK_SIZE
    AMOADD   sp, t1, 0(t0)
#else
    LD       sp, 0(t0)
#endif

    #  Initialize the per-cpu description structure
    csrr    a0, mhartid  # arg[0]: hart ID
    la      t0, bsp_start
    jalr    ra, t0

    #  We should not get here
    j       .


.section .data,"aw",@progbits

.align 3

_stack_pointer:
    .dword  (_sstack + STACK_SIZE)


.section .data.cached,"aw",@progbits

.align 3

_sstack:
    .space  BSP_CONFIG_NCPUS*STACK_SIZE
_estack:
    .word   0xdeadbeef
