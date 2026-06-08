#ifndef __BSP_CMO_H__
#define __BSP_CMO_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "common/compiler.h"

__ALWAYS_INLINE__
static inline void cmo_inval(uintptr_t address)
{
    register uint64_t _rs1 = (uint64_t)address;
    asm volatile(
        "cbo.inval (%[base])           \n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_inval_all()
{
    puts("[ PANIC ] Unsupported CMO operation (inval_all)\n");
    exit(EXIT_FAILURE);
}

__ALWAYS_INLINE__
static inline void cmo_clean(uintptr_t address)
{
    register uint64_t _rs1 = (uint64_t)address;
    asm volatile(
        "cbo.clean (%[base])           \n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_clean_all()
{
    puts("[ PANIC ] Unsupported CMO operation (clean_all)\n");
    exit(EXIT_FAILURE);
}

__ALWAYS_INLINE__
static inline void cmo_flush(uintptr_t address)
{
    register uint64_t _rs1 = (uint64_t)address;
    asm volatile(
        "cbo.flush (%[base])           \n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_flush_all()
{
    puts("[ PANIC ] Unsupported CMO operation (flush_all)\n");
    exit(EXIT_FAILURE);
}

__ALWAYS_INLINE__
static inline void cmo_zero(uintptr_t address)
{
    puts("[ PANIC ] Unsupported CMO operation (zero)\n");
    exit(EXIT_FAILURE);
}

__ALWAYS_INLINE__
static inline void cmo_prefetch_i(uintptr_t address)
{
}

__ALWAYS_INLINE__
static inline void cmo_prefetch_r(uintptr_t address)
{
}

__ALWAYS_INLINE__
static inline void cmo_prefetch_w(uintptr_t address)
{
}

#endif /* __BSP_CMO_H__ */
