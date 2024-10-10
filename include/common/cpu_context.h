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
 *  @file   cpu_context.h
 *  @author Cesar Fuguet
 */
#ifndef __CPU_CONTEXT_H__
#define __CPU_CONTEXT_H__

#define CONTEXT_REGBYTES          ((__riscv_xlen)/8)
#define CONTEXT_FRAME_SIZE        (40*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_A0          ( 0*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_A1          ( 1*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_A2          ( 2*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_A3          ( 3*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_A4          ( 4*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_A5          ( 5*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_A6          ( 6*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_A7          ( 7*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S0          ( 8*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S1          ( 9*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S2          (10*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S3          (11*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S4          (12*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S5          (13*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S6          (14*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S7          (15*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S8          (16*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S9          (17*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S10         (18*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_S11         (19*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_T0          (20*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_T1          (21*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_T2          (22*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_T3          (23*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_T4          (24*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_T5          (25*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_T6          (26*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_RA          (27*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_GP          (28*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_TP          (29*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_MCAUSE      (30*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_MSTATUS     (31*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_MEPC        (32*CONTEXT_REGBYTES)
#define CONTEXT_FRAME_MTVAL       (33*CONTEXT_REGBYTES)

#endif /* __CPU_CONTEXT_H__ */
