/**
 *  Copyright (c) - CEA List
 *  @file   ariane_testharness/bsp_perf.h
 *  @author Cesar Fuguet Tortolero
 */
#define __BSP_PERF_LOADS__
#define __BSP_PERF_STORES__

static inline void bsp_set_loads(uint64_t value)
{
    asm volatile (
            "csrw mhpmcounter7,  %[val]             \n"
            :
            : [val] "r"(value)
            : "memory");
}

static inline uint64_t bsp_loads()
{
    register uint64_t loads;

    asm volatile (
            "csrr %[loads], mhpmcounter7   \n"
            : [loads] "=r"(loads)
            :
            : "memory");

    return loads;
}

static inline void bsp_set_stores(uint64_t value)
{
    asm volatile (
            "csrw mhpmcounter8,  %[val]             \n"
            :
            : [val] "r"(value)
            : "memory");
}

static inline uint64_t bsp_stores()
{
    register uint64_t stores;

    asm volatile (
            "csrr %[stores], mhpmcounter8   \n"
            : [stores] "=r"(stores)
            :
            : "memory");

    return stores;
}
