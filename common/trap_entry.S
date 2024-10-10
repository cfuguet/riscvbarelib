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
 *  @file   common/trap_entry.S
 *  @author Cesar Fuguet
 */
#include <common/cpu_context.h>
#include <common/cpu_defs.h>

.extern trap_handler

.section .text,"ax",@progbits

#if (__riscv_xlen == 32)
#define __LD lw
#define __ST sw
#elif (__riscv_xlen == 64)
#define __LD ld
#define __ST sd
#endif

.macro CPU_CONTEXT_SAVE
addi    sp,    sp,   -CONTEXT_FRAME_SIZE ;

__ST      a0,    CONTEXT_FRAME_A0(sp) ;
__ST      a1,    CONTEXT_FRAME_A1(sp) ;
__ST      a2,    CONTEXT_FRAME_A2(sp) ;
__ST      a3,    CONTEXT_FRAME_A3(sp) ;

csrrs     a0,    mcause,  zero ;
__ST      a0,    CONTEXT_FRAME_MCAUSE(sp) ;
csrrs     a1,    mstatus, zero ;
__ST      a1,    CONTEXT_FRAME_MSTATUS(sp) ;
csrrs     a2,    mepc,    zero ;
__ST      a2,    CONTEXT_FRAME_MEPC(sp) ;
csrrs     a3,    mtval,   zero ;
__ST      a3,    CONTEXT_FRAME_MTVAL(sp) ;

__ST      a4,    CONTEXT_FRAME_A4(sp) ;
__ST      a5,    CONTEXT_FRAME_A5(sp) ;
__ST      a6,    CONTEXT_FRAME_A6(sp) ;
__ST      a7,    CONTEXT_FRAME_A7(sp) ;
__ST      s0,    CONTEXT_FRAME_S0(sp) ;
__ST      s1,    CONTEXT_FRAME_S1(sp) ;
__ST      s2,    CONTEXT_FRAME_S2(sp) ;
__ST      s3,    CONTEXT_FRAME_S3(sp) ;
__ST      s4,    CONTEXT_FRAME_S4(sp) ;
__ST      s5,    CONTEXT_FRAME_S5(sp) ;
__ST      s6,    CONTEXT_FRAME_S6(sp) ;
__ST      s7,    CONTEXT_FRAME_S7(sp) ;
__ST      s8,    CONTEXT_FRAME_S8(sp) ;
__ST      s9,    CONTEXT_FRAME_S9(sp) ;
__ST      s10,   CONTEXT_FRAME_S10(sp) ;
__ST      s11,   CONTEXT_FRAME_S11(sp) ;
__ST      t0,    CONTEXT_FRAME_T0(sp) ;
__ST      t1,    CONTEXT_FRAME_T1(sp) ;
__ST      t2,    CONTEXT_FRAME_T2(sp) ;
__ST      t3,    CONTEXT_FRAME_T3(sp) ;
__ST      t4,    CONTEXT_FRAME_T4(sp) ;
__ST      t5,    CONTEXT_FRAME_T5(sp) ;
__ST      t6,    CONTEXT_FRAME_T6(sp) ;
__ST      ra,    CONTEXT_FRAME_RA(sp) ;
__ST      gp,    CONTEXT_FRAME_GP(sp) ;
__ST      tp,    CONTEXT_FRAME_TP(sp) ;
.endm

.macro CPU_CONTEXT_RESTORE
__LD    a0,    CONTEXT_FRAME_MCAUSE(sp) ;
csrrw   zero,  mcause,  a0 ;
__LD    a0,    CONTEXT_FRAME_MSTATUS(sp) ;
csrrw   zero,  mstatus, a0 ;
__LD    a0,    CONTEXT_FRAME_MEPC(sp) ;
csrrw   zero,  mepc,    a0 ;
__LD    a0,    CONTEXT_FRAME_MTVAL(sp) ;
csrrw   zero,  mtval,    a0 ;

__LD    a0,    CONTEXT_FRAME_A0(sp) ;
__LD    a1,    CONTEXT_FRAME_A1(sp) ;
__LD    a2,    CONTEXT_FRAME_A2(sp) ;
__LD    a3,    CONTEXT_FRAME_A3(sp) ;
__LD    a4,    CONTEXT_FRAME_A4(sp) ;
__LD    a5,    CONTEXT_FRAME_A5(sp) ;
__LD    a6,    CONTEXT_FRAME_A6(sp) ;
__LD    a7,    CONTEXT_FRAME_A7(sp) ;
__LD    s0,    CONTEXT_FRAME_S0(sp) ;
__LD    s1,    CONTEXT_FRAME_S1(sp) ;
__LD    s2,    CONTEXT_FRAME_S2(sp) ;
__LD    s3,    CONTEXT_FRAME_S3(sp) ;
__LD    s4,    CONTEXT_FRAME_S4(sp) ;
__LD    s5,    CONTEXT_FRAME_S5(sp) ;
__LD    s6,    CONTEXT_FRAME_S6(sp) ;
__LD    s7,    CONTEXT_FRAME_S7(sp) ;
__LD    s8,    CONTEXT_FRAME_S8(sp) ;
__LD    s9,    CONTEXT_FRAME_S9(sp) ;
__LD    s10,   CONTEXT_FRAME_S10(sp) ;
__LD    s11,   CONTEXT_FRAME_S11(sp) ;
__LD    t0,    CONTEXT_FRAME_T0(sp) ;
__LD    t1,    CONTEXT_FRAME_T1(sp) ;
__LD    t2,    CONTEXT_FRAME_T2(sp) ;
__LD    t3,    CONTEXT_FRAME_T3(sp) ;
__LD    t4,    CONTEXT_FRAME_T4(sp) ;
__LD    t5,    CONTEXT_FRAME_T5(sp) ;
__LD    t6,    CONTEXT_FRAME_T6(sp) ;
__LD    ra,    CONTEXT_FRAME_RA(sp) ;
__LD    gp,    CONTEXT_FRAME_GP(sp) ;
__LD    tp,    CONTEXT_FRAME_TP(sp) ;

addi    sp,    sp,    CONTEXT_FRAME_SIZE ;
.endm

    .globl trap_entry
    .align 3

    trap_entry:
    CPU_CONTEXT_SAVE
    move  a0,sp // trap frame as sole arg of trap_handler func
    call trap_handler /* (mcause, mstatus, mepc) */
    CPU_CONTEXT_RESTORE
    mret
