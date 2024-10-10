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
 *  @file   plic.h
 *  @author Cesar Fuguet Tortolero
 */
#ifndef __PLIC_H__
#define __PLIC_H__

#include <stdint.h>

/*
 *  PLIC Address Map
 *  (source:https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc)
 *  ----------
 *  base + 0x000000: Reserved (interrupt source 0 does not exist)
 *  base + 0x000004: Interrupt source 1 priority
 *  base + 0x000008: Interrupt source 2 priority
 *  ...
 *  base + 0x000FFC: Interrupt source 1023 priority
 *  base + 0x001000: Interrupt Pending bit 0-31
 *  base + 0x00107C: Interrupt Pending bit 992-1023
 *  ...
 *  base + 0x002000: Enable bits for sources 0-31 on context 0
 *  base + 0x002004: Enable bits for sources 32-63 on context 0
 *  ...
 *  base + 0x00207F: Enable bits for sources 992-1023 on context 0
 *  base + 0x002080: Enable bits for sources 0-31 on context 1
 *  base + 0x002084: Enable bits for sources 32-63 on context 1
 *  ...
 *  base + 0x0020FF: Enable bits for sources 992-1023 on context 1
 *  base + 0x002100: Enable bits for sources 0-31 on context 2
 *  base + 0x002104: Enable bits for sources 32-63 on context 2
 *  ...
 *  base + 0x00217F: Enable bits for sources 992-1023 on context 2
 *  ...
 *  base + 0x1F1F80: Enable bits for sources 0-31 on context 15871
 *  base + 0x1F1F84: Enable bits for sources 32-63 on context 15871
 *  base + 0x1F1FFF: Enable bits for sources 992-1023 on context 15871
 *  ...
 *  base + 0x1FFFFC: Reserved
 *  base + 0x200000: Priority threshold for context 0
 *  base + 0x200004: Claim/complete for context 0
 *  base + 0x200008: Reserved
 *  ...
 *  base + 0x200FFC: Reserved
 *  base + 0x201000: Priority threshold for context 1
 *  base + 0x201004: Claim/complete for context 1
 *  ...
 *  base + 0x3FFE000: Priority threshold for context 15871
 *  base + 0x3FFE004: Claim/complete for context 15871
 *  base + 0x3FFE008: Reserved
 *  ...
 *  base + 0x3FFFFFC: Reserved
 */
#define PLIC_PRIO_OFFSET                    0x0000000
#define PLIC_PRIO_SPAN                      0x0001000 // up to 1024 4-bytes priority registers
#define PLIC_PENDING_OFFSET                 0x0001000
#define PLIC_PENDING_SPAN                   0x0000080 // up to 32 4-bytes pending registers
#define PLIC_ENABLE_OFFSET                  0x0002000
#define PLIC_ENABLE_SPAN                    0x0000080
#define PLIC_THRESHOLD_OFFSET               0x0200000
#define PLIC_CLAIM_OFFSET                   0x0200004
#define PLIC_THRESHOLD_CLAIM_SPAN           0x0001000
#define PLIC_SPAN                           0x4000000

#ifndef __ASSEMBLER__

#include "common/io.h"
#include "bsp/bsp_config.h"

typedef struct {
    uintptr_t base;
} plic_drv_t;

void plic_init(plic_drv_t *dev, uintptr_t base);

/*
 *  Utility functions
 */
static inline uint32_t plic_get_register(uintptr_t base, uintptr_t offset)
{
    return ioreadw(base + offset);
}

static inline void plic_set_register(uintptr_t base, uintptr_t offset, uint32_t value)
{
    iowritew(base + offset, value);
}

static inline uint32_t plic_get_prio(uintptr_t base, int source)
{
    return plic_get_register(base, PLIC_PRIO_OFFSET + 4*source);
}

static inline void plic_set_prio(uintptr_t base, int source, int prio)
{
    plic_set_register(base, PLIC_PRIO_OFFSET + 4*source, prio);
}

static inline int plic_get_enable(uintptr_t base, int hart, int source)
{
    uint32_t enable = plic_get_register(base, PLIC_ENABLE_OFFSET + hart*PLIC_ENABLE_SPAN + (source/32)*4);
    return (enable >> (source & 0x1f)) & 0x1;
}

static inline void plic_set_enable(uintptr_t base, int hart, int source, int value)
{
    uint32_t enable = plic_get_register(base, PLIC_ENABLE_OFFSET + hart*PLIC_ENABLE_SPAN + (source/32)*4);
    uint32_t mask = 1U << (source & 0x1f);
    if (value) enable = enable |  mask;
    else       enable = enable & ~mask;

    plic_set_register(base, PLIC_ENABLE_OFFSET + hart*PLIC_ENABLE_SPAN + (source/32)*4, enable);
}

static inline uint32_t plic_get_threshold(uintptr_t base, int hart)
{
    return plic_get_register(base, PLIC_THRESHOLD_OFFSET + hart*PLIC_THRESHOLD_CLAIM_SPAN);
}

static inline void plic_set_threshold(uintptr_t base, int hart, uint32_t threshold)
{
    plic_set_register(base, PLIC_THRESHOLD_OFFSET + hart*PLIC_THRESHOLD_CLAIM_SPAN, threshold);
}

static inline uint32_t plic_get_claim(uintptr_t base, int hart)
{
    return plic_get_register(base, PLIC_CLAIM_OFFSET + hart*PLIC_THRESHOLD_CLAIM_SPAN);
}

static inline void plic_set_claim(uintptr_t base, int hart, uint32_t id)
{
    plic_set_register(base, PLIC_CLAIM_OFFSET + hart*PLIC_THRESHOLD_CLAIM_SPAN, id);
}

static inline int plic_get_pending(uintptr_t base, int source)
{
    uint32_t pending = plic_get_register(base, PLIC_PENDING_OFFSET + (source/32)*4);
    return (pending >> (source & 0x1f)) & 0x1;
}

#endif /* __ASSEMBLER__ */

#endif /* __PLIC_H__ */
