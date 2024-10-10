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
 *  @file   include/common/trap_handler.h
 *  @author Cesar Fuguet
 */
#ifndef __TRAP_HANDLER_H__
#define __TRAP_HANDLER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uintptr_t gpr[30];
    uintptr_t cause;
    uintptr_t status;
    uintptr_t epc;
    uintptr_t tval;
} trapframe_t;

typedef void (*irq_handler_t)(
        uintptr_t mcause,
        uintptr_t mstatus,
        uintptr_t mepc);

typedef void (*exc_handler_t)(
        uintptr_t mcause,
        uintptr_t mstatus,
        uintptr_t mepc,
        uintptr_t mtval);

void set_irq_ipi_handler(int core, irq_handler_t handler);
void set_irq_tim_handler(int core, irq_handler_t handler);
void set_irq_ext_handler(int core, irq_handler_t handler);
void set_exc_ld_flt_handler(int core, exc_handler_t handler);
void set_exc_st_flt_handler(int core, exc_handler_t handler);
void set_exc_flt_handler(int core, exc_handler_t handler);
void trap_handler(trapframe_t *tf);

#ifdef __cplusplus
}
#endif

#endif /* __TRAP_HANDLER_H__ */

