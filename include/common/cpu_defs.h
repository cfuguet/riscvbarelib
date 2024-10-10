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
 *  @file   cpu_defs.h
 *  @author Cesar Fuguet
 */
#ifndef __CPU_DEFS_H__
#define __CPU_DEFS_H__

/*
 *  Machine status (mstatus) register masks
 */
#define MSTATUS_UIE         0x00000001
#define MSTATUS_SIE         0x00000002
#define MSTATUS_MIE         0x00000008
#define MSTATUS_UPIE        0x00000010
#define MSTATUS_SPIE        0x00000020
#define MSTATUS_MPIE        0x00000080
#define MSTATUS_SPP         0x00000100
#define MSTATUS_MPP         0x00001800
#define MSTATUS_FS          0x00006000
#define MSTATUS_XS          0x00018000
#define MSTATUS_MPRV        0x00020000
#define MSTATUS_SUM         0x00040000
#define MSTATUS_MXR         0x00080000
#define MSTATUS_TVM         0x00100000
#define MSTATUS_TW          0x00200000
#define MSTATUS_TSR         0x00400000
#if (__INTPTR_WIDTH__ == 64)
#define MSTATUS_SD          0x8000000000000000ULL
#else
#define MSTATUS_SD          0x80000000
#endif

#define MSTATUS_FS_OFF      0x00000000
#define MSTATUS_FS_INITIAL  0x00002000
#define MSTATUS_FS_CLEAN    0x00004000
#define MSTATUS_FS_DIRTY    0x00006000

#define MSTATUS_XS_OFF      0x00000000
#define MSTATUS_XS_INITIAL  0x00008000
#define MSTATUS_XS_CLEAN    0x00010000
#define MSTATUS_XS_DIRTY    0x00018000

/*
 *  Machine interrupt pending (mip) register masks
 */
#define MIP_USIP            0x00000001
#define MIP_SSIP            0x00000002
#define MIP_MSIP            0x00000008
#define MIP_UTIP            0x00000010
#define MIP_STIP            0x00000020
#define MIP_MTIP            0x00000080
#define MIP_UEIP            0x00000100
#define MIP_SEIP            0x00000200
#define MIP_MEIP            0x00000800

/*
 *  Machine interrupt enable (mie) register masks
 */
#define MIE_USIE            0x00000001
#define MIE_SSIE            0x00000002
#define MIE_MSIE            0x00000008
#define MIE_UTIE            0x00000010
#define MIE_STIE            0x00000020
#define MIE_MTIE            0x00000080
#define MIE_UEIE            0x00000100
#define MIE_SEIE            0x00000200
#define MIE_MEIE            0x00000800

/*
 *  Machine cause (mcause) register codes
 */
#if (__INTPTR_WIDTH__ == 64)
#define MCAUSE_INTERRUPT             (1ULL << 63)
#else
#define MCAUSE_INTERRUPT             (1UL  << 31)
#endif

#define MCAUSE_U_SOFTWARE_INTERRUPT  (MCAUSE_INTERRUPT |  0)
#define MCAUSE_S_SOFTWARE_INTERRUPT  (MCAUSE_INTERRUPT |  1)
#define MCAUSE_M_SOFTWARE_INTERRUPT  (MCAUSE_INTERRUPT |  3)
#define MCAUSE_U_TIMER_INTERRUPT     (MCAUSE_INTERRUPT |  4)
#define MCAUSE_S_TIMER_INTERRUPT     (MCAUSE_INTERRUPT |  5)
#define MCAUSE_M_TIMER_INTERRUPT     (MCAUSE_INTERRUPT |  7)
#define MCAUSE_U_EXTERNAL_INTERRUPT  (MCAUSE_INTERRUPT |  8)
#define MCAUSE_S_EXTERNAL_INTERRUPT  (MCAUSE_INTERRUPT |  9)
#define MCAUSE_M_EXTERNAL_INTERRUPT  (MCAUSE_INTERRUPT | 11)
#define MCAUSE_INSTR_ADDR_MISALIGNED (                    0)
#define MCAUSE_INSTR_ACCESS_FAULT    (                    1)
#define MCAUSE_INSTR_ILLEGAL         (                    2)
#define MCAUSE_BREAKPOINT            (                    3)
#define MCAUSE_LOAD_ADDR_MISALIGNED  (                    4)
#define MCAUSE_LOAD_ACCESS_FAULT     (                    5)
#define MCAUSE_STORE_ADDR_MISALIGNED (                    6)
#define MCAUSE_STORE_ACCESS_FAULT    (                    7)
#define MCAUSE_ENV_CALL_FROM_U       (                    8)
#define MCAUSE_ENV_CALL_FROM_S       (                    9)
#define MCAUSE_ENV_CALL_FROM_M       (                   11)
#define MCAUSE_INSTR_PAGE_FAULT      (                   12)
#define MCAUSE_LOAD_PAGE_FAULT       (                   13)
#define MCAUSE_STORE_PAGE_FAULT      (                   15)

#define CSR_MHPMEVENT3       0x323
#define CSR_MHPMEVENT4       0x324
#define CSR_MHPMEVENT5       0x325
#define CSR_MHPMEVENT6       0x326
#define CSR_MHPMEVENT7       0x327
#define CSR_MHPMEVENT8       0x328
#define CSR_MHPMEVENT9       0x329
#define CSR_MHPMEVENT10      0x32a
#define CSR_MHPMEVENT11      0x32b
#define CSR_MHPMEVENT12      0x32c
#define CSR_MHPMEVENT13      0x32d
#define CSR_MHPMEVENT14      0x32e
#define CSR_MHPMEVENT15      0x32f
#define CSR_MHPMEVENT16      0x330
#define CSR_MHPMEVENT17      0x331
#define CSR_MHPMEVENT18      0x332
#define CSR_MHPMEVENT19      0x333
#define CSR_MHPMEVENT20      0x334
#define CSR_MHPMEVENT21      0x335
#define CSR_MHPMEVENT22      0x336
#define CSR_MHPMEVENT23      0x337
#define CSR_MHPMEVENT24      0x338
#define CSR_MHPMEVENT25      0x339
#define CSR_MHPMEVENT26      0x33a
#define CSR_MHPMEVENT27      0x33b
#define CSR_MHPMEVENT28      0x33c
#define CSR_MHPMEVENT29      0x33d
#define CSR_MHPMEVENT30      0x33e
#define CSR_MHPMEVENT31      0x33f

#define CSR_MHPMCOUNTER3     0xb03
#define CSR_MHPMCOUNTER4     0xb04
#define CSR_MHPMCOUNTER5     0xb05
#define CSR_MHPMCOUNTER6     0xb06
#define CSR_MHPMCOUNTER7     0xb07
#define CSR_MHPMCOUNTER8     0xb08
#define CSR_MHPMCOUNTER9     0xb09
#define CSR_MHPMCOUNTER10    0xb0a
#define CSR_MHPMCOUNTER11    0xb0b
#define CSR_MHPMCOUNTER12    0xb0c
#define CSR_MHPMCOUNTER13    0xb0d
#define CSR_MHPMCOUNTER14    0xb0e
#define CSR_MHPMCOUNTER15    0xb0f
#define CSR_MHPMCOUNTER16    0xb10
#define CSR_MHPMCOUNTER17    0xb11
#define CSR_MHPMCOUNTER18    0xb12
#define CSR_MHPMCOUNTER19    0xb13
#define CSR_MHPMCOUNTER20    0xb14
#define CSR_MHPMCOUNTER21    0xb15
#define CSR_MHPMCOUNTER22    0xb16
#define CSR_MHPMCOUNTER23    0xb17
#define CSR_MHPMCOUNTER24    0xb18
#define CSR_MHPMCOUNTER25    0xb19
#define CSR_MHPMCOUNTER26    0xb1a
#define CSR_MHPMCOUNTER27    0xb1b
#define CSR_MHPMCOUNTER28    0xb1c
#define CSR_MHPMCOUNTER29    0xb1d
#define CSR_MHPMCOUNTER30    0xb1e
#define CSR_MHPMCOUNTER31    0xb1f

#endif /* __CPU_DEFS_H__ */
