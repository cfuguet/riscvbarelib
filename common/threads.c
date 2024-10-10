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
 *  @file   common/threads.c
 *  @author Cesar Fuguet
 *  @brief  This file describes the routines and structures used to manage
 *          threads in multicore platforms
 */
#include "common/cpu.h"
#include "common/mp.h"
#include "common/cache.h"
#include "common/threads.h"
#include "drivers/clint.h"

int thread_create(thread_t *t, cpu_entry_func_t func, void *args)
{
    //  Get description of the target CPU
    volatile cpu_t* cpu;
    if (t->cpu_id != NULL) cpu = cpu_get_desc((uintptr_t)t->cpu_id);
    else                   cpu = mp_get_free_cpu();

    //  No valid CPU
    if (cpu == NULL) return -1;

    //  If there is no hardware cache coherency, make sure that the CPU
    //  description structure is not cached. This way the following accesses
    //  will fetch up to date data.
    cpu_dcache_invalidate_range((uintptr_t)cpu, sizeof(cpu_t));

    //  Check that the target core is actually IDLE
    if (cpu_get_state(cpu->sid) != CPU_IDLE) return -1;

    //  Write arguments for the new thread
    cpu->entry_func = func;
    cpu->args       = args;
    cpu->ipi_kind   = CPU_IPI_CREATE;
    cpu->thread     = t;

    //  Make sure that arguments are visible
    cpu_dfence();

    //  Send an IPI to the target core
    clint_send_ipi(cpu->clint_drv, cpu->hid);

    //  Wait for the target core to be AWAKE
    int timeout = 1;
    for (int i = 0; (i < 10000) && timeout; i++) {
        cpu_delay(1000);

        enum cpu_state_e state = cpu_get_state(cpu->sid);
        if (state == CPU_AWAKE) timeout = 0;
        if (state == CPU_ERROR) return -1;
    }

    //  The target thread did not wake up (in reasonable time)
    if (timeout) return -2;

    //  Fill the thread structure on successfull creation
    t->id     = cpu->sid;
    t->cpu_id = (void*)((uintptr_t)cpu->sid);
    t->desc   = (cpu_t*)cpu;
    t->ret    = NULL;

    //  Declare the target thread as running
    cpu_set_state(cpu->sid, CPU_RUNNING);
    return 0;
}

int thread_destroy(thread_t *t)
{
    //  If there is no hardware cache coherency, make sure that the CPU
    //  description structure is not cached. This way the following accesses
    //  will fetch up to date data.
    cpu_dcache_invalidate_range((uintptr_t)t->desc, sizeof(cpu_t));

    //  Check if the target core is already IDLE
    if (cpu_get_state(t->desc->sid) == CPU_IDLE) return 0;

    //  Check if the target core is in ERROR
    if (cpu_get_state(t->desc->sid) == CPU_ERROR) return -1;

    //  Send KILL signal to the target thread
    t->desc->ipi_kind = CPU_IPI_KILL;

    //  Make sure that arguments are visible
    cpu_dfence();

    //  Send an IPI to the target core
    clint_send_ipi(t->desc->clint_drv, t->desc->hid);

    //  Wait for the target core to be IDLE
    return thread_join(t);
}

int thread_join(thread_t *t)
{
    //  Wait for the target core to be IDLE (or ERROR)
    for (int timeout = 0; timeout < 10000; timeout++) {
        enum cpu_state_e state = cpu_get_state(t->desc->sid);
        if (state == CPU_IDLE)  return  0;
        if (state == CPU_ERROR) return -1;
        cpu_delay(1000);
    }
    return -2;
}

int thread_id()
{
    //  Get thread description structure
    cpu_t *cpu = (cpu_t*)cpu_get_thread_pointer();
    if (cpu == NULL) return -1;

    //  If there is no hardware cache coherency, make sure that the CPU
    //  description structure is not cached. This way the following accesses
    //  will fetch up to date data.
    cpu_dcache_invalidate_range((uintptr_t)cpu, sizeof(cpu_t));
    cpu_dcache_invalidate_range((uintptr_t)cpu->thread, sizeof(thread_t));

    return cpu->thread->id;
}
