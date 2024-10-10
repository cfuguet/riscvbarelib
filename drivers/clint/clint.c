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
 *  @file   clint.c
 *  @author Cesar Fuguet Tortolero
 */
#include "drivers/clint.h"
#include "common/io.h"

#if (__INTPTR_WIDTH__ == 64)
#define CLINT_SET(addr,val) iowritel(addr, val);
#define CLINT_GET(addr)     ioreadl(addr);
#else
#define CLINT_SET(addr,val) iowritew(addr, val);
#define CLINT_GET(addr)     ioreadw(addr);
#endif

void clint_init(clint_drv_t *dev, uintptr_t base, int ncores)
{
    int i;

    dev->base   = base;
    dev->ncores = ncores;

    /* reset the time counter in the clint */
    clint_set_mtime(dev, 0);

    for (i = 0; i < ncores; ++i) {
        /* set the timer due dates at their maximum value (virtually
         * disable them) */
        clint_set_mtimecmp(dev, i, (uintptr_t)-1ULL);

        /* clear any pending IPI in the clint */
        clint_clear_ipi(dev, i);
    }
}

void clint_send_ipi(clint_drv_t *dev, int target_core)
{
    iowritew(dev->base + (CLINT_MSIP_OFFSET + 4*target_core), 0x1);
}

int clint_recv_ipi(clint_drv_t *dev, int target_core)
{
    const uintptr_t offset = CLINT_MSIP_OFFSET + 4*target_core;
    int active;

    active = ioreadw(dev->base + offset);
    if (active) iowritew(dev->base + offset, 0x0);
    return active;
}

void clint_clear_ipi(clint_drv_t *dev, int target_core)
{
    const uintptr_t offset = CLINT_MSIP_OFFSET + 4*target_core;
    iowritew(dev->base + offset, 0x0);
}

void clint_set_mtimecmp(clint_drv_t *dev, int target_core, uintptr_t mtimecmp)
{
    const uintptr_t offset = CLINT_MTIMECMP_OFFSET + 8*target_core;
    CLINT_SET(dev->base + offset, mtimecmp);
}

void clint_set_mtime(clint_drv_t *dev, uintptr_t mtime)
{
    CLINT_SET(dev->base + CLINT_MTIME_OFFSET, mtime);
}

void clint_set_timer_period(clint_drv_t *dev, int target_core, uintptr_t period)
{
    dev->timer_period[target_core] = period;
    clint_set_mtimecmp(dev, target_core, clint_get_mtime(dev) + period);
}

uintptr_t clint_get_mtimecmp(clint_drv_t *dev, int target_core)
{
    const uintptr_t offset = CLINT_MTIMECMP_OFFSET + 8*target_core;
    return CLINT_GET(dev->base + offset);
}

uintptr_t clint_get_mtime(clint_drv_t *dev)
{
    return CLINT_GET(dev->base + CLINT_MTIME_OFFSET);
}

uintptr_t clint_get_timer_period(clint_drv_t *dev, int target_core)
{
    return dev->timer_period[target_core];
}
