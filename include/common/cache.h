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
 *  @file   include/common/cache.h
 *  @author Cesar Fuguet
 *  @brief  This file describes the routines for cache maintenance operations
 */
#ifndef __CACHE_H__
#define __CACHE_H__

#include <stdint.h>
#include <string.h>
#include "bsp/bsp_config.h"
#include "bsp/bsp_cache.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __cacheblock_aligned__ __attribute__((aligned(BSP_CONFIG_DCACHE_LINE_BYTES)))
#define __cacheline_aligned__  __cacheblock_aligned__
#define __cl_aligned__         __cacheblock_aligned__

static inline size_t cpu_icache_get_size()
{
    return BSP_CONFIG_ICACHE_NWAYS*BSP_CONFIG_ICACHE_NSETS*BSP_CONFIG_ICACHE_LINE_BYTES;
}

static inline size_t cpu_dcache_get_size()
{
    return BSP_CONFIG_DCACHE_NWAYS*BSP_CONFIG_DCACHE_NSETS*BSP_CONFIG_DCACHE_LINE_BYTES;
}

static inline void cpu_icache_enable()
{
    bsp_icache_enable();
}

static inline void cpu_icache_disable()
{
    bsp_icache_disable();
}

static inline void cpu_icache_invalidate()
{
    bsp_icache_invalidate();
}

static inline void cpu_dcache_invalidate()
{
    bsp_dcache_invalidate();
}

static inline void cpu_icache_invalidate_address(uintptr_t addr)
{
#if BSP_CONFIG_ICACHE_INVALIDATE_LINE_IS_SUPPORTED
    bsp_icache_invalidate_address(addr);
#else
    bsp_icache_invalidate();
#endif
}

static inline void cpu_dcache_invalidate_address(uintptr_t addr)
{
#if BSP_CONFIG_DCACHE_INVALIDATE_LINE_IS_SUPPORTED
    bsp_dcache_invalidate_address(addr);
#else
    bsp_dcache_invalidate();
#endif
}

static inline void cpu_icache_prefetch_address(uintptr_t addr)
{
#if BSP_CONFIG_ICACHE_PREFETCH_LINE_IS_SUPPORTED
    bsp_icache_prefetch_address(addr);
#endif
}

static inline void cpu_dcache_prefetch_address(uintptr_t addr)
{
#if BSP_CONFIG_DCACHE_PREFETCH_LINE_IS_SUPPORTED
    bsp_dcache_prefetch_address(addr);
#endif
}

static inline void cpu_dcache_enable()
{
    bsp_dcache_enable();
}

static inline void cpu_dcache_disable()
{
    bsp_dcache_disable();
}

static inline void cpu_icache_invalidate_range(uintptr_t addr, size_t bytes)
{
    if (bytes == 0) return;

#if BSP_CONFIG_ICACHE_INVALIDATE_LINE_IS_SUPPORTED
    uintptr_t nline_base = addr               >> BSP_CONFIG_ICACHE_LINE_OFFSET;
    uintptr_t nline_end  = (addr + bytes - 1) >> BSP_CONFIG_ICACHE_LINE_OFFSET;
    for (uintptr_t n = nline_base; n <= nline_end; nline++) {
        cpu_icache_invalidate_address(n << BSP_CONFIG_ICACHE_LINE_OFFSET);
    }
#else
    cpu_icache_invalidate();
#endif
}

static inline void cpu_dcache_invalidate_range(uintptr_t addr, size_t bytes)
{
    if (bytes == 0) return;

#if BSP_CONFIG_DCACHE_INVALIDATE_LINE_IS_SUPPORTED
    uintptr_t nline_base = addr               >> BSP_CONFIG_DCACHE_LINE_OFFSET;
    uintptr_t nline_end  = (addr + bytes - 1) >> BSP_CONFIG_DCACHE_LINE_OFFSET;
    for (uintptr_t n = nline_base; n <= nline_end; n++) {
        cpu_dcache_invalidate_address(n << BSP_CONFIG_DCACHE_LINE_OFFSET);
    }
#else
    cpu_dcache_invalidate();
#endif
}

static inline void cpu_icache_prefetch_range(uintptr_t addr, size_t bytes)
{
    if (bytes == 0) return;

    uintptr_t nline_base = addr               >> BSP_CONFIG_ICACHE_LINE_OFFSET;
    uintptr_t nline_end  = (addr + bytes - 1) >> BSP_CONFIG_ICACHE_LINE_OFFSET;
    for (uintptr_t n = nline_base; n <= nline_end; n++) {
        cpu_icache_prefetch_address(n << BSP_CONFIG_DCACHE_LINE_OFFSET);
    }
}

static inline void cpu_dcache_prefetch_range(uintptr_t addr, size_t bytes)
{
    if (bytes == 0) return;

    uintptr_t nline_base = addr               >> BSP_CONFIG_DCACHE_LINE_OFFSET;
    uintptr_t nline_end  = (addr + bytes - 1) >> BSP_CONFIG_DCACHE_LINE_OFFSET;
    for (uintptr_t n = nline_base; n <= nline_end; n++) {
        cpu_dcache_prefetch_address(n << BSP_CONFIG_DCACHE_LINE_OFFSET);
    }
}

#ifdef __cplusplus
}
#endif

#endif /*  __CACHE_H__ */
