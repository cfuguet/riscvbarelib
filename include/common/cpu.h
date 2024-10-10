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
 *  @file   cpu.h
 *  @author Cesar Fuguet
 */
#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>
#include "common/cpu_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define write_csr(csr, val) ({                             \
        asm volatile ("csrw %[rd], %[rs]\n"                \
                : /* no output */                     \
                : [rd]"i"(csr), [rs]"r"(val)          \
                : "memory");                          \
        })

#define read_csr(csr) ({                                   \
        uint64_t ret;                                      \
        asm volatile ("csrr %[rd], %[rs]\n"                \
                : [rd]"=r"(ret)                       \
                : [rs]"i"(csr)                        \
                : "memory");                          \
        ret;                                               \
        })

#define set_csr(csr, mask) ({                              \
        uint64_t ret;                                      \
        asm volatile ("csrrs %[rd], %[rs1], %[rs2]\n"      \
                : [rd]"=r"(ret)                       \
                : [rs1]"i"(csr), [rs2]"r"(mask)       \
                : "memory");                          \
        ret;                                               \
        })

#define clear_csr(csr, mask) ({                            \
        uint64_t ret;                                      \
        asm volatile ("csrrc %[rd], %[rs1], %[rs2]\n"      \
                : [rd]"=r"(ret)                       \
                : [rs1]"i"(csr), [rs2]"r"(mask)       \
                : "memory");                          \
        ret;                                               \
        })

/* memory barrier operations */
static inline void cpu_dfence();
static inline void cpu_ifence();
static inline void cpu_mfence();

/* miscellaneous */
static inline int cpu_id();
static inline void cpu_delay(int ncycles);
static inline uint64_t cpu_cycles();
static inline uint64_t cpu_instructions();
uint64_t cpu_frequency();

/* interrupts */
static inline void cpu_enable_machine_all_irq();
static inline void cpu_enable_machine_external_irq();
static inline void cpu_disable_machine_external_irq();
static inline void cpu_enable_machine_software_irq();
static inline void cpu_disable_machine_software_irq();
static inline void cpu_enable_machine_timer_irq();
static inline void cpu_disable_machine_timer_irq();
static inline void cpu_enable_interrupts();
static inline void cpu_disable_interrupts();

static inline void cpu_nop()
{
    asm volatile ("nop\n" ::: "memory");
}

static inline void cpu_dfence()
{
    asm volatile ("fence  rw, rw\n" ::: "memory");
}

static inline void cpu_ifence()
{
    asm volatile ("fence.i\n" ::: "memory");
}

static inline void cpu_mfence()
{
    asm volatile ("" ::: "memory");
}

static inline int cpu_id()
{
    register int ret;
    asm volatile ("csrr %0, mhartid\n" : "=r"(ret));
    return ret;
}

static inline void cpu_delay(int ncycles)
{
    register int niters = ncycles / 3;

    asm volatile (
            "1:                                 \n"
            "beq    %[n], zero, 2f              \n"
            "addi   %[n], %[n], -1              \n"
            "j      1b                          \n"
            "2:                                 \n"
            : [n] "+r"(niters)
            : /* no inputs */
            : "memory");
}

static inline void cpu_set_thread_pointer(uintptr_t p)
{
    asm volatile (
            "add    tp, %[ptr], x0              \n"
            : /* no outputs */
            : [ptr] "r"(p)
            : "memory"
    );
}

static inline uintptr_t cpu_get_thread_pointer()
{
    register uintptr_t ptr;

    asm volatile (
            "add    %[ret], tp, x0              \n"
            : [ret] "=r" (ptr)
            : /* no inputs */
            : "memory"
    );

    return ptr;
}

static inline void cpu_set_cycles(uint64_t value)
{
#if (__SIZEOF_LONG__ == 4)
    asm volatile (
            "csrw mcycle,  %[lo]              \n"
            "csrw mcycleh, %[hi]              \n"
            :
            : [lo] "r"(value         & ((1ULL << 32) - 1)),
              [hi] "r"((value >> 32) & ((1ULL << 32) - 1))
            : "memory");
#else
    asm volatile (
            "csrw mcycle,  %[val]             \n"
            :
            : [val] "r"(value)
            : "memory");
#endif
}

static inline uint64_t cpu_cycles()
{
#if (__SIZEOF_LONG__ == 4)
    register uint32_t lo;
    register uint32_t hi;

    asm volatile (
            "csrr %[lo], mcycle                  \n"
            "csrr %[hi], mcycleh                 \n"
            : [lo] "=r"(lo), [hi] "=r"(hi)
            :
            : "memory");

    /* as there is one cycle between the reading of the lower bits and
     * higher bits, decrement hi if lo was about to wrap */
    if (lo == 0xffffffffUL)
        hi--;

    return ((uint64_t)hi << 32) | lo;
#else
    register uint64_t cycles;

    asm volatile (
            "csrr %[cycles], mcycle            \n"
            : [cycles] "=r"(cycles)
            :
            : "memory");

    return cycles;
#endif
}

static inline void cpu_set_instructions(uint64_t value)
{
#if (__SIZEOF_LONG__ == 4)
    asm volatile (
            "csrw minstret,  %[lo]              \n"
            "csrw minstreth, %[hi]              \n"
            :
            : [lo] "r"(value         & ((1ULL << 32) - 1)),
              [hi] "r"((value >> 32) & ((1ULL << 32) - 1))
            : "memory");
#else
    asm volatile (
            "csrw minstret,  %[val]             \n"
            :
            : [val] "r"(value)
            : "memory");
#endif
}

static inline uint64_t cpu_instructions()
{
#if (__SIZEOF_LONG__ == 4)
    register uint32_t lo;
    register uint32_t hi;

    asm volatile (
            "csrr %[lo], minstret                \n"
            "csrr %[hi], minstreth               \n"
            : [lo] "=r"(lo), [hi] "=r"(hi)
            :
            : "memory");

    /* as there is one cycle between the reading of the lower bits and
     * higher bits, decrement hi if lo was about to wrap */
    if (lo == 0xffffffffUL)
        hi--;

    return ((uint64_t)hi << 32) | lo;
#else
    register uint64_t instrs;

    asm volatile (
            "csrr %[instrs], minstret          \n"
            : [instrs] "=r"(instrs)
            :
            : "memory");

    return instrs;
#endif
}

static inline void cpu_set_imiss(uint64_t value)
{
#if (__SIZEOF_LONG__ == 4)
    asm volatile (
            "csrw mhpmcounter3,  %[lo]              \n"
            "csrw mhpmcounter3h, %[hi]              \n"
            :
            : [lo] "r"(value         & ((1ULL << 32) - 1)),
              [hi] "r"((value >> 32) & ((1ULL << 32) - 1))
            : "memory");
#else
    asm volatile (
            "csrw mhpmcounter3,  %[val]             \n"
            :
            : [val] "r"(value)
            : "memory");
#endif
}

static inline uint64_t cpu_imiss()
{
#if (__SIZEOF_LONG__ == 4)
    register uint32_t lo;
    register uint32_t hi;

    asm volatile (
            "csrr %[lo], mhpmcounter3       \n"
            "csrr %[hi], mhpmcounter3h      \n"
            : [lo] "=r"(lo), [hi] "=r"(hi)
            :
            : "memory");

    /* as there is one cycle between the reading of the lower bits and
     * higher bits, decrement hi if lo was about to wrap */
    if (lo == 0xffffffffUL)
        hi--;

    return ((uint64_t)hi << 32) | lo;
#else
    register uint64_t imiss;

    asm volatile (
            "csrr %[imiss], mhpmcounter3   \n"
            : [imiss] "=r"(imiss)
            :
            : "memory");

    return imiss;
#endif
}

static inline void cpu_set_dmiss(uint64_t value)
{
#if (__SIZEOF_LONG__ == 4)
    asm volatile (
            "csrw mhpmcounter4,  %[lo]              \n"
            "csrw mhpmcounter4h, %[hi]              \n"
            :
            : [lo] "r"(value         & ((1ULL << 32) - 1)),
              [hi] "r"((value >> 32) & ((1ULL << 32) - 1))
            : "memory");
#else
    asm volatile (
            "csrw mhpmcounter4,  %[val]             \n"
            :
            : [val] "r"(value)
            : "memory");
#endif
}

static inline uint64_t cpu_dmiss()
{
#if (__SIZEOF_LONG__ == 4)
    register uint32_t lo;
    register uint32_t hi;

    asm volatile (
            "csrr %[lo], mhpmcounter4       \n"
            "csrr %[hi], mhpmcounter4h      \n"
            : [lo] "=r"(lo), [hi] "=r"(hi)
            :
            : "memory");

    /* as there is one cycle between the reading of the lower bits and
     * higher bits, decrement hi if lo was about to wrap */
    if (lo == 0xffffffffUL)
        hi--;

    return ((uint64_t)hi << 32) | lo;
#else
    register uint64_t dmiss;

    asm volatile (
            "csrr %[dmiss], mhpmcounter4   \n"
            : [dmiss] "=r"(dmiss)
            :
            : "memory");

    return dmiss;
#endif
}

static inline void cpu_enable_machine_all_irq()
{
    register uintptr_t temp = MIE_MSIE | MIE_MTIE | MIE_MEIE;
    asm volatile (
            "csrrs    zero, mie, %0\n"
            :
            : "r"(temp)
            : "memory");
}

static inline void cpu_enable_machine_external_irq()
{
    register uintptr_t temp = MIE_MEIE;
    asm volatile (
            "csrrs    zero, mie, %0\n"
            :
            : "r"(temp)
            : "memory");
}

static inline void cpu_enable_machine_software_irq()
{
    register uintptr_t temp = MIE_MSIE;
    asm volatile (
            "csrrs    zero, mie, %0\n"
            :
            : "r"(temp)
            : "memory");
}

static inline void cpu_enable_machine_timer_irq()
{
    register uintptr_t temp = MIE_MTIE;
    asm volatile (
            "csrrs    zero, mie, %0\n"
            :
            : "r"(temp)
            : "memory");
}

static inline void cpu_disable_machine_external_irq()
{
    register uintptr_t temp = MIE_MEIE;
    asm volatile (
            "csrrc    zero, mie, %0\n"
            :
            : "r"(temp)
            : "memory");
}

static inline void cpu_disable_machine_software_irq()
{
    register uintptr_t temp = MIE_MSIE;
    asm volatile (
            "csrrc    zero, mie, %0\n"
            :
            : "r"(temp)
            : "memory");
}

static inline void cpu_disable_machine_timer_irq()
{
    register uintptr_t temp = MIE_MTIE;
    asm volatile (
            "csrrc    zero, mie, %0\n"
            :
            : "r"(temp)
            : "memory");
}

static inline void cpu_enable_interrupts()
{
    asm volatile (
            "csrrsi   zero, mstatus, %0"
            :
            : "i"(MSTATUS_MIE)
            : "memory");
}

static inline void cpu_disable_interrupts()
{
    asm volatile (
            "csrrci   zero, mstatus, %0"
            :
            : "i"(MSTATUS_MIE)
            : "memory");
}

static inline void cpu_wait_for_interrupt()
{
    asm volatile (
            "wfi            \n"
            "nop            \n"
            ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* __CPU_H__ */
