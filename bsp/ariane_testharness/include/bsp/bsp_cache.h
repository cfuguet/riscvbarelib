/**
 *  Copyright (c) - CEA List
 *  @file   bsp/bsp_cache.h
 *  @author Cesar Fuguet Tortolero
 *  @brief  This file describes the routines for cache maintenance operations
 */
#include <stdlib.h>
#include <stdint.h>
#include "bsp/bsp_config.h"
#include "bsp/bsp_cmo.h"

#ifndef BSP_CONFIG_ICACHE_NWAYS
#define BSP_CONFIG_ICACHE_NWAYS                        4
#endif
#ifndef BSP_CONFIG_ICACHE_NSETS
#define BSP_CONFIG_ICACHE_NSETS                        64
#endif
#ifndef BSP_CONFIG_ICACHE_LINE_OFFSET
#define BSP_CONFIG_ICACHE_LINE_OFFSET                  6
#endif
#define BSP_CONFIG_ICACHE_LINE_BYTES                   (1 << BSP_CONFIG_ICACHE_LINE_OFFSET)
#define BSP_CONFIG_ICACHE_INVALIDATE_LINE_IS_SUPPORTED 0
#define BSP_CONFIG_ICACHE_PREFETCH_LINE_IS_SUPPORTED   0

#ifndef BSP_CONFIG_DCACHE_NWAYS
#define BSP_CONFIG_DCACHE_NWAYS                        8
#endif
#ifndef BSP_CONFIG_DCACHE_NSETS
#define BSP_CONFIG_DCACHE_NSETS                        64
#endif
#ifndef BSP_CONFIG_DCACHE_LINE_OFFSET
#define BSP_CONFIG_DCACHE_LINE_OFFSET                  6
#endif
#define BSP_CONFIG_DCACHE_LINE_BYTES                   (1 << BSP_CONFIG_DCACHE_LINE_OFFSET)
#define BSP_CONFIG_DCACHE_INVALIDATE_LINE_IS_SUPPORTED 1
#define BSP_CONFIG_DCACHE_PREFETCH_LINE_IS_SUPPORTED   1

static inline void bsp_icache_enable()
{
}

static inline void bsp_icache_disable()
{
}

static inline void bsp_dcache_enable()
{
}

static inline void bsp_dcache_disable()
{
}

static inline void bsp_icache_invalidate()
{
    asm volatile ("fence.i" ::: "memory");
}

static inline void bsp_dcache_invalidate()
{
#ifndef BSP_CMO_DISABLE
    cmo_inval_all();
#endif
}

static inline void bsp_icache_invalidate_address(uintptr_t addr)
{
    bsp_icache_invalidate();
}

static inline void bsp_dcache_invalidate_address(uintptr_t addr)
{
#ifndef BSP_CMO_DISABLE
    cmo_inval(addr);
#endif
}

static inline void bsp_icache_prefetch_address(uintptr_t addr)
{
}

static inline void bsp_dcache_prefetch_address(uintptr_t addr)
{
#ifndef BSP_CMO_DISABLE
    cmo_prefetch_r(addr);
#endif
}
