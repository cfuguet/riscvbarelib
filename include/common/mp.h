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
 *  @file   include/common/mp.h
 *  @author Cesar Fuguet
 *  @brief  This file describes the routines and structures used to support
 *          multiple processor cores
 */
#ifndef __MP_H__
#define __MP_H__

#include <stdint.h>
#include <stdatomic.h>
#include <string.h>
#include "bsp/bsp_config.h"
#include "common/cache.h"
#include "drivers/clint.h"

struct thread_s;

typedef int (*cpu_entry_func_t)(void *args);

enum cpu_state_e {
    CPU_IDLE = 0,
    CPU_AWAKE,
    CPU_RUNNING,
    CPU_ERROR
};

enum cpu_ipi_e {
    CPU_IPI_NULL = 0,
    CPU_IPI_CREATE,
    CPU_IPI_KILL
};

typedef struct cpu_s {
    /*
     *  Logical (software) ID of the core
     */
    uint16_t sid;

    /*
     *  Physical (hardware) ID of the core
     */
    uint16_t hid;

    /*
     *  Pointer to currently executed thread
     */
    struct thread_s *thread;

    /*
     *  Function pointer to the worker function
     */
    cpu_entry_func_t entry_func;

    /*
     *  Pointer to the memory region containing worker's arguments
     */
    void *args;

    /*
     *  State of the CPU
     */
    enum cpu_state_e state;

    /*
     *  Pointer to the driver of the clint peripheral
     */
    clint_drv_t* clint_drv;

    /*
     *  IPI kind (clint inter-processor interrupt)
     */
    enum cpu_ipi_e ipi_kind;
} __cl_aligned__ cpu_t;

/*
 *  Mapping table between physical (Hardware) IDs and Logical (Software) IDs
 */
extern volatile uint16_t cpu_hid2sid[1 << BSP_CONFIG_HARTID_BITS];

/*
 *  Mapping table between Logical (Software) IDs and Physical (Hardware) IDs
 */
extern volatile uint16_t cpu_sid2hid[1 << BSP_CONFIG_HARTID_BITS];

/*
 *  Vector with a per-cpu structure
 */
extern volatile cpu_t cpu_list[BSP_CONFIG_NCPUS] ;

static inline volatile cpu_t* cpu_get_desc(int cpu_id)
{
    if (cpu_id > BSP_CONFIG_NCPUS) return NULL;
    return &cpu_list[cpu_id];
}


/*
 *  Getters and setters for the per-cpu structures
 *
 *  This get/set functions use atomic operations for reading and writing into
 *  the different fields. The idea is that AMOs are always handled as
 *  uncacheable with the CEA's dcache and therefore work even in systems with
 *  no hardware cache-coherency
 */
static inline void cpu_set_entry_func(int cpu_id, cpu_entry_func_t entry_func)
{
#if BSP_CONFIG_NCPUS > 1
    atomic_exchange((atomic_uintptr_t*)&cpu_list[cpu_id].entry_func,
            (uintptr_t)entry_func);
#else
    cpu_list[cpu_id].entry_func = entry_func;
#endif
}

static inline cpu_entry_func_t cpu_get_entry_func(int cpu_id)
{
#if BSP_CONFIG_NCPUS > 1
    return (cpu_entry_func_t)atomic_fetch_or(
            (atomic_uintptr_t*)&cpu_list[cpu_id].entry_func, 0);
#else
    return cpu_list[cpu_id].entry_func;
#endif
}

static inline void cpu_set_args(int cpu_id, void *args)
{
#if BSP_CONFIG_NCPUS > 1
    atomic_exchange((atomic_uintptr_t*)&cpu_list[cpu_id].args,
            (uintptr_t)args);
#else
    cpu_list[cpu_id].args = args;
#endif
}

static inline void* cpu_get_args(int cpu_id)
{
#if BSP_CONFIG_NCPUS > 1
    return (void*)atomic_fetch_or(
            (atomic_uintptr_t*)&cpu_list[cpu_id].args, 0);
#else
    return cpu_list[cpu_id].args;
#endif
}

static inline void cpu_set_state(int cpu_id, enum cpu_state_e state)
{
#if BSP_CONFIG_NCPUS > 1
    atomic_exchange((atomic_int*)&cpu_list[cpu_id].state, (int)state);
#else
    cpu_list[cpu_id].state = state;
#endif
}

static inline enum cpu_state_e cpu_get_state(int cpu_id)
{
#if BSP_CONFIG_NCPUS > 1
    return atomic_fetch_or((atomic_int*)&cpu_list[cpu_id].state, 0);
#else
    return cpu_list[cpu_id].state;
#endif
}

static inline void cpu_set_ipi_kind(int cpu_id, enum cpu_ipi_e kind)
{
#if BSP_CONFIG_NCPUS > 1
    atomic_exchange((atomic_int*)&cpu_list[cpu_id].ipi_kind, (int)kind);
#else
    cpu_list[cpu_id].ipi_kind = kind;
#endif
}

static inline enum cpu_ipi_e cpu_get_ipi_kind(int cpu_id)
{
#if BSP_CONFIG_NCPUS > 1
    return atomic_fetch_or((atomic_int*)&cpu_list[cpu_id].ipi_kind, 0);
#else
    return cpu_list[cpu_id].ipi_kind;
#endif
}

/**
 *  Returns the number of CPUs in the system
 */
static inline int mp_get_cpu_count()
{
    return BSP_CONFIG_NCPUS;
}

/**
 *  Returns the cpu description structure of the first free CPU in the CPU list
 *
 *  It returns NULL when all CPUs are busy.
 */
cpu_t* mp_get_free_cpu();

#endif /* __MP_H__ */
