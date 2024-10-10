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
 *  @file   common/trap_handler.c
 *  @author Cesar Fuguet
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "common/cpu.h"
#include "common/trap_handler.h"
#include "bsp/bsp_config.h"

static irq_handler_t __per_core_irq_ipi_handler[BSP_CONFIG_NCPUS];
static irq_handler_t __per_core_irq_tim_handler[BSP_CONFIG_NCPUS];
static irq_handler_t __per_core_irq_ext_handler[BSP_CONFIG_NCPUS];
static exc_handler_t __per_core_exc_ld_flt_handler[BSP_CONFIG_NCPUS];
static exc_handler_t __per_core_exc_st_flt_handler[BSP_CONFIG_NCPUS];
static exc_handler_t __per_core_exc_flt_handler[BSP_CONFIG_NCPUS];

void set_irq_ipi_handler(int core, irq_handler_t handler)
{
    __per_core_irq_ipi_handler[core] = handler;
    cpu_dfence();
}

void set_irq_tim_handler(int core, irq_handler_t handler)
{
    __per_core_irq_tim_handler[core] = handler;
    cpu_dfence();
}

void set_irq_ext_handler(int core, irq_handler_t handler)
{
    __per_core_irq_ext_handler[core] = handler;
    cpu_dfence();
}

void set_exc_ld_flt_handler(int core, exc_handler_t handler)
{
    __per_core_exc_ld_flt_handler[core] = handler;
    cpu_dfence();
}

void set_exc_st_flt_handler(int core, exc_handler_t handler)
{
    __per_core_exc_st_flt_handler[core] = handler;
    cpu_dfence();
}

void set_exc_flt_handler(int core, exc_handler_t handler)
{
    __per_core_exc_flt_handler[core] = handler;
    cpu_dfence();
}

static void __irq_handler(uintptr_t mcause, uintptr_t mstatus, uintptr_t mepc)
{
    irq_handler_t handler;
    switch (mcause) {
        case MCAUSE_M_SOFTWARE_INTERRUPT:
            handler = __per_core_irq_ipi_handler[cpu_id()];
            if (handler) {
                handler(mcause, mstatus, mepc);
                break;
            }
            puts("PANIC ! SPURIOUS SOFTWARE INTERRUPT!\n");
            exit(EXIT_FAILURE);

        case MCAUSE_M_TIMER_INTERRUPT:
            handler = __per_core_irq_tim_handler[cpu_id()];
            if (handler) {
                handler(mcause, mstatus, mepc);
                break;
            }
            puts("PANIC ! SPURIOUS TIMER INTERRUPT!\n");
            exit(EXIT_FAILURE);

        case MCAUSE_M_EXTERNAL_INTERRUPT:
            handler = __per_core_irq_ext_handler[cpu_id()];
            if (handler) {
                handler(mcause, mstatus, mepc);
                break;
            }
            puts("PANIC ! SPURIOUS EXTERNAL INTERRUPT!\n");
            exit(EXIT_FAILURE);

        default:
            puts("PANIC ! SPURIOUS INTERRUPT!\n");
            exit(EXIT_FAILURE);
    }
}

static void __exc_handler(uintptr_t mcause, uintptr_t mstatus, uintptr_t mepc, uintptr_t mtval)
{
    exc_handler_t handler;

    switch (mcause) {
        case MCAUSE_INSTR_ADDR_MISALIGNED:
        case MCAUSE_INSTR_ACCESS_FAULT:
            handler = __per_core_exc_flt_handler[cpu_id()];
            if (handler) {
                handler(mcause, mstatus, mepc, mtval);
                return;
            }
            puts("PANIC ! INSTRUCTION ACCESS FAULT!\n");
            break;

        case MCAUSE_INSTR_ILLEGAL:
            handler = __per_core_exc_flt_handler[cpu_id()];
            if (handler) {
                handler(mcause, mstatus, mepc, mtval);
                return;
            }
            puts("PANIC ! ILLEGAL INSTRUCTION!\n");
            break;

        case MCAUSE_LOAD_ACCESS_FAULT:
        case MCAUSE_LOAD_ADDR_MISALIGNED:
            handler = __per_core_exc_ld_flt_handler[cpu_id()];
            if (handler) {
                handler(mcause, mstatus, mepc, mtval);
                return;
            }
            puts("PANIC ! SPURIOUS LOAD ACCESS FAULT!\n");
            break;

        case MCAUSE_STORE_ACCESS_FAULT:
        case MCAUSE_STORE_ADDR_MISALIGNED:
            handler = __per_core_exc_st_flt_handler[cpu_id()];
            if (handler) {
                handler(mcause, mstatus, mepc, mtval);
                return;
            }
            puts("PANIC ! SPURIOUS STORE ACCESS FAULT!\n");
            break;

        default:
            puts("PANIC ! INRECOVERABLE EXCEPTION!");
            break;
    }

#if (__riscv_xlen == 32)
#define __csr_fmt "%x"
#else
#define __csr_fmt "%lx"
#endif
    printf("\nmcause = 0x" __csr_fmt
            "\nmstatus = 0x" __csr_fmt
            "\nmepc = 0x" __csr_fmt
            "\nmtval = 0x" __csr_fmt,
            mcause, mstatus, mepc, mtval);


    exit(EXIT_FAILURE);
}

void trap_handler(trapframe_t *tf)
{
    uintptr_t mcause, mstatus, mepc, mtval;
    mcause = tf->cause;
    mstatus = tf->status;
    mepc = tf->epc;
    mtval = tf->tval;
    if (mcause & MCAUSE_INTERRUPT) {
        __irq_handler(mcause, mstatus, mepc);
        return;
    }

    __exc_handler(mcause, mstatus, mepc, mtval);
    tf->epc += 4;
    return;
}

