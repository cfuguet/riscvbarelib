#ifndef __BSP_CMO_H__
#define __BSP_CMO_H__

#include <stdint.h>
#include "common/compiler.h"

/*
    I-type instruction:
    ===================

        +-------------+-----+-------+----+-------------+
        |      simm12 | rs1 | func3 | rd |      opcode |
        +-------------+-----+-------+----+-------------+
        31            20    15      12   7             0

        .insn i opcode, func3, rd, rs1, simm12

    S-type instruction:
    ===================

        +--------------+-----+-----+-------+-------------+-------------+
        | simm12[11:5] | rs2 | rs1 | func3 | simm12[4:0] |      opcode |
        +--------------+-----+-----+-------+-------------+-------------+
        31             25    20    15      12            7             0

        .insn s opcode, func3, rd, rs1, simm12
*/

#define INSN_CMO_CBO(rs1, imm)     \
    /* opcode */((0x0f    <<  0) | \
    /* rd     */ (0x00    <<  7) | \
    /* func3  */ (0x2     << 12) | \
    /* rs1    */ ((rs1)   << 15) | \
    /* simm12 */ ((imm)   << 20))

#define INSN_CMO_PREFETCH(rs1, rs2, imm12)                \
    /* opcode       */( (0x13                    <<  0) | \
    /* simm12[4:0]  */  ((((imm12) >> 0) & 0x1f) <<  7) | \
    /* func3        */  (0x6                     << 12) | \
    /* rs1          */ ((rs1)                    << 15) | \
    /* rs2          */ ((rs2)                    << 20) | \
    /* simm12[11:5] */  ((((imm12) >> 5) & 0x7f) << 25))

__ALWAYS_INLINE__
static inline void cmo_inval(uintptr_t address)
{
    register uint64_t _rs1 asm ("x5") = (uint64_t)address;
    asm volatile(
        ".word " STR(INSN_CMO_CBO(5, 0x000)) "\n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_inval_all()
{
    asm volatile(
        ".word " STR(INSN_CMO_CBO(0, 0x800)) "\n\t"
        : /* no outputs */
        : /* no inputs */
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_clean(uintptr_t address)
{
    register uint64_t _rs1 asm ("x5") = (uint64_t)address;
    asm volatile(
        ".word " STR(INSN_CMO_CBO(5, 0x001)) "\n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_clean_all()
{
    asm volatile(
        ".word " STR(INSN_CMO_CBO(0, 0x801)) "\n\t"
        : /* no outputs */
        : /* no inputs */
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_flush(uintptr_t address)
{
    register uint64_t _rs1 asm ("x5") = (uint64_t)address;
    asm volatile(
        ".word " STR(INSN_CMO_CBO(5, 0x002)) "\n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_flush_all()
{
    asm volatile(
        ".word " STR(INSN_CMO_CBO(0, 0x802)) "\n\t"
        : /* no outputs */
        : /* no inputs */
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_zero(uintptr_t address)
{
    register uint64_t _rs1 asm ("x5") = (uint64_t)address;
    asm volatile(
        ".word " STR(INSN_CMO_CBO(5, 0x004)) "\n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_prefetch_i(uintptr_t address)
{
    register uint64_t _rs1 asm ("x5") = (uint64_t)address;
    asm volatile(
        ".word " STR(INSN_CMO_PREFETCH(5, 0x0, 0)) "\n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_prefetch_r(uintptr_t address)
{
    register uint64_t _rs1 asm ("x5") = (uint64_t)address;
    asm volatile(
        ".word " STR(INSN_CMO_PREFETCH(5, 0x1, 0)) "\n\t"
        //".word " STR(0xcafecafe) "\n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

__ALWAYS_INLINE__
static inline void cmo_prefetch_w(uintptr_t address)
{
    register uint64_t _rs1 asm ("x5") = (uint64_t)address;
    asm volatile(
        ".word " STR(INSN_CMO_PREFETCH(5, 0x3, 0)) "\n\t"
        : /* no outputs */
        : [base]"r"(_rs1)
        : "memory"
    );
}

#endif /* __BSP_CMO_H__ */
