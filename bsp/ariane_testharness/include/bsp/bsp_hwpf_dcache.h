/**
 *  Copyright (c) 2022 - CEA/DRT/LIST/DSCIN/LSTA
 */
/**
 *  @file   bsp/bsp_hwpf_dcache.h
 *  @author Cesar Fuguet
 */
#ifndef __BSP_HWPF_H__
#define __BSP_HWPF_H__

#include <stdint.h>

#define __HWPF_GET_BITFIELD(val, mask, offset) (((val) >> (offset)) & (mask))
#define __HWPF_SET_BITFIELD(val, mask, offset) (((val) & (mask)) << (offset))

#define HWPF_BASE_ENABLE_MASK         0x1ULL
#define HWPF_BASE_ENABLE_OFFSET       0
#define HWPF_BASE_REARM_MASK          0x1ULL
#define HWPF_BASE_REARM_OFFSET        1
#define HWPF_BASE_CYCLE_MASK          0x1ULL
#define HWPF_BASE_CYCLE_OFFSET        2
#define HWPF_BASE_ADDR_MASK           0xFFFFFFFFFFFFFFC0ULL
#define HWPF_BASE_ADDR_OFFSET         0

#define HWPF_PARAM_STRIDE_MASK        0xFFFFFFFFULL
#define HWPF_PARAM_STRIDE_OFFSET      0
#define HWPF_PARAM_NLINES_MASK        0xFFFFULL
#define HWPF_PARAM_NLINES_OFFSET      32
#define HWPF_PARAM_NBLOCKS_MASK       0xFFFFULL
#define HWPF_PARAM_NBLOCKS_OFFSET     48

#define HWPF_THROTTLE_NWAIT_MASK       0xFFFFULL
#define HWPF_THROTTLE_NWAIT_OFFSET     0
#define HWPF_THROTTLE_NINFLIGHT_MASK   0xFFFFULL
#define HWPF_THROTTLE_NINFLIGHT_OFFSET 16

#define HWPF_STATUS_BUSY_MASK         0xFULL
#define HWPF_STATUS_BUSY_OFFSET       32
#define HWPF_STATUS_FREE_MASK         0x1ULL
#define HWPF_STATUS_FREE_OFFSET       31
#define HWPF_STATUS_FREE_INDEX_MASK   0xFULL
#define HWPF_STATUS_FREE_INDEX_OFFSET 16
#define HWPF_STATUS_ENABLE_MASK       0xFULL
#define HWPF_STATUS_ENABLE_OFFSET     0

#define CSR_CMO_DCACHE_HWPF_BASE0     0x8e0
#define CSR_CMO_DCACHE_HWPF_PARAM0    0x8e1
#define CSR_CMO_DCACHE_HWPF_THROTTLE0 0x8e2
#define CSR_CMO_DCACHE_HWPF_BASE1     0x8e3
#define CSR_CMO_DCACHE_HWPF_PARAM1    0x8e4
#define CSR_CMO_DCACHE_HWPF_THROTTLE1 0x8e5
#define CSR_CMO_DCACHE_HWPF_BASE2     0x8e6
#define CSR_CMO_DCACHE_HWPF_PARAM2    0x8e7
#define CSR_CMO_DCACHE_HWPF_THROTTLE2 0x8e8
#define CSR_CMO_DCACHE_HWPF_BASE3     0x8e9
#define CSR_CMO_DCACHE_HWPF_PARAM3    0x8ea
#define CSR_CMO_DCACHE_HWPF_THROTTLE3 0x8eb
#define CSR_CMO_DCACHE_HWPF_STATUS    0x8f0

#define HWPF_ENGINE_CSR_BASE(x)       ((CSR_CMO_DCACHE_HWPF_BASE0)  + (x)*3)
#define HWPF_ENGINE_CSR_PARAM(x)      ((CSR_CMO_DCACHE_HWPF_PARAM0) + (x)*3)
#define HWPF_ENGINE_CSR_THROTTLE(x)   ((CSR_CMO_DCACHE_HWPF_THROTTLE0) + (x)*3)


__ALWAYS_INLINE__
static inline void __hwpf_set_base(const int engine, const uint64_t raw)
{
    switch(engine) {
        case 0: write_csr(HWPF_ENGINE_CSR_BASE(0), raw); break;
        case 1: write_csr(HWPF_ENGINE_CSR_BASE(1), raw); break;
        case 2: write_csr(HWPF_ENGINE_CSR_BASE(2), raw); break;
        case 3: write_csr(HWPF_ENGINE_CSR_BASE(3), raw); break;
    }
}


__ALWAYS_INLINE__
static inline uint64_t __hwpf_get_base(int engine)
{
    switch(engine) {
        case 0: return read_csr(HWPF_ENGINE_CSR_BASE(0));
        case 1: return read_csr(HWPF_ENGINE_CSR_BASE(1));
        case 2: return read_csr(HWPF_ENGINE_CSR_BASE(2));
        case 3: return read_csr(HWPF_ENGINE_CSR_BASE(3));
    }
    return -1;
}


__ALWAYS_INLINE__
static inline void __hwpf_set_param(int engine, uint64_t raw)
{
    switch(engine) {
        case 0: write_csr(HWPF_ENGINE_CSR_PARAM(0), raw); break;
        case 1: write_csr(HWPF_ENGINE_CSR_PARAM(1), raw); break;
        case 2: write_csr(HWPF_ENGINE_CSR_PARAM(2), raw); break;
        case 3: write_csr(HWPF_ENGINE_CSR_PARAM(3), raw); break;
    }
}


__ALWAYS_INLINE__
static inline uint64_t __hwpf_get_param(int engine)
{
    switch(engine) {
        case 0: return read_csr(HWPF_ENGINE_CSR_PARAM(0)); break;
        case 1: return read_csr(HWPF_ENGINE_CSR_PARAM(1)); break;
        case 2: return read_csr(HWPF_ENGINE_CSR_PARAM(2)); break;
        case 3: return read_csr(HWPF_ENGINE_CSR_PARAM(3)); break;
    }
    return -1;
}


__ALWAYS_INLINE__
static inline void __hwpf_set_throttle(int engine, uint64_t raw)
{
    switch(engine) {
        case 0: write_csr(HWPF_ENGINE_CSR_THROTTLE(0), raw); break;
        case 1: write_csr(HWPF_ENGINE_CSR_THROTTLE(1), raw); break;
        case 2: write_csr(HWPF_ENGINE_CSR_THROTTLE(2), raw); break;
        case 3: write_csr(HWPF_ENGINE_CSR_THROTTLE(3), raw); break;
    }
}


__ALWAYS_INLINE__
static inline uint64_t __hwpf_get_throttle(int engine)
{
    switch(engine) {
        case 0: return read_csr(HWPF_ENGINE_CSR_THROTTLE(0));
        case 1: return read_csr(HWPF_ENGINE_CSR_THROTTLE(1));
        case 2: return read_csr(HWPF_ENGINE_CSR_THROTTLE(2));
        case 3: return read_csr(HWPF_ENGINE_CSR_THROTTLE(3));
    }
    return -1;
}


__ALWAYS_INLINE__
static inline void bsp_hwpf_set_addr(int engine, uint64_t addr, int params)
{
    const uint64_t __aligned_addr = addr & HWPF_BASE_ADDR_MASK;
    const uint64_t __base = __aligned_addr | params;
    __hwpf_set_base(engine, __base);
}


__ALWAYS_INLINE__
static inline uint64_t bsp_hwpf_get_addr(int engine)
{
    return __hwpf_get_base(engine) & HWPF_BASE_ADDR_MASK;
}


__ALWAYS_INLINE__
static inline void bsp_hwpf_set_params(int engine, const hwpf_engine_params_t *p)
{
    const uint64_t __stride = __HWPF_SET_BITFIELD(p->stride-1,
            HWPF_PARAM_STRIDE_MASK,
            HWPF_PARAM_STRIDE_OFFSET);
    const uint64_t __nblocks = __HWPF_SET_BITFIELD(p->nblocks-1,
            HWPF_PARAM_NBLOCKS_MASK,
            HWPF_PARAM_NBLOCKS_OFFSET);
    const uint64_t __nlines = __HWPF_SET_BITFIELD(p->nlines-1,
            HWPF_PARAM_NLINES_MASK,
            HWPF_PARAM_NLINES_OFFSET);

    __hwpf_set_param(engine, __stride | __nblocks | __nlines);
}


__ALWAYS_INLINE__
static inline void bsp_hwpf_get_params(int engine, hwpf_engine_params_t *p)
{
    const uint64_t __param  = __hwpf_get_param(engine);

    p->stride = __HWPF_GET_BITFIELD(__param,
            HWPF_PARAM_STRIDE_MASK,
            HWPF_PARAM_STRIDE_OFFSET) + 1;
    p->nblocks = __HWPF_GET_BITFIELD(__param,
            HWPF_PARAM_NBLOCKS_MASK,
            HWPF_PARAM_NBLOCKS_OFFSET) + 1;
    p->nlines = __HWPF_GET_BITFIELD(__param,
            HWPF_PARAM_NLINES_MASK,
            HWPF_PARAM_NLINES_OFFSET) + 1;
}


__ALWAYS_INLINE__
static inline void bsp_hwpf_set_throttle(
        int engine,
        const hwpf_engine_throttle_t *t)
{
    const uint64_t __nwait =
        __HWPF_SET_BITFIELD(t->nwait,
                HWPF_THROTTLE_NWAIT_MASK,
                HWPF_THROTTLE_NWAIT_OFFSET);
    const uint64_t __ninfligt =
        __HWPF_SET_BITFIELD(t->ninflight,
                HWPF_THROTTLE_NINFLIGHT_MASK,
                HWPF_THROTTLE_NINFLIGHT_OFFSET);

    __hwpf_set_throttle(engine, __ninfligt | __nwait);
}


__ALWAYS_INLINE__
static inline void bsp_hwpf_get_throttle(
        int engine,
        hwpf_engine_throttle_t *t)
{
    const uint64_t __throttle  = __hwpf_get_throttle(engine);

    t->nwait = __HWPF_GET_BITFIELD(__throttle,
            HWPF_THROTTLE_NWAIT_MASK,
            HWPF_THROTTLE_NWAIT_OFFSET);
    t->ninflight = __HWPF_GET_BITFIELD(__throttle,
            HWPF_THROTTLE_NINFLIGHT_MASK,
            HWPF_THROTTLE_NINFLIGHT_OFFSET);
}


__ALWAYS_INLINE__ static inline int bsp_hwpf_get_free()
{
    const uint64_t status = read_csr(CSR_CMO_DCACHE_HWPF_STATUS);

    int is_free = __HWPF_GET_BITFIELD(status,
            HWPF_STATUS_FREE_MASK,
            HWPF_STATUS_FREE_OFFSET);

    if (!is_free) return -1;

    int free_index = __HWPF_GET_BITFIELD(
            status,
            HWPF_STATUS_FREE_INDEX_MASK,
            HWPF_STATUS_FREE_INDEX_OFFSET);

    return free_index;
}


__ALWAYS_INLINE__ static inline int bsp_hwpf_is_busy(int engine)
{
    const uint64_t status = read_csr(CSR_CMO_DCACHE_HWPF_STATUS);

    uint64_t busy_mask = __HWPF_GET_BITFIELD(status,
            HWPF_STATUS_BUSY_MASK,
            HWPF_STATUS_BUSY_OFFSET);

    return busy_mask & (0x1ULL << engine);
}


__ALWAYS_INLINE__ static inline int bsp_hwpf_is_enabled(int engine)
{
    const uint64_t status = read_csr(CSR_CMO_DCACHE_HWPF_STATUS);

    uint64_t enable_mask = __HWPF_GET_BITFIELD(status,
            HWPF_STATUS_ENABLE_MASK,
            HWPF_STATUS_ENABLE_OFFSET);

    return enable_mask & (0x1ULL << engine);
}


__ALWAYS_INLINE__ static inline void bsp_hwpf_abort(int engine)
{
    __hwpf_set_base(engine, 0);
}


#endif /* __BSP_HWPF_H__ */
