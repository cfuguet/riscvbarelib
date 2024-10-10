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
 *  @file   clint.h
 *  @author Cesar Fuguet Tortolero
 */
#ifndef __CLINT_H__
#define __CLINT_H__

#include <stdint.h>
#include "bsp/bsp_config.h"

typedef struct {
    uintptr_t base;
    uintptr_t ncores;
    uintptr_t timer_period[BSP_CONFIG_CLINT_NTARGETS];
} clint_drv_t;

#define CLINT_MSIP_OFFSET     0x0000
#define CLINT_MTIMECMP_OFFSET 0x4000
#define CLINT_MTIME_OFFSET    0xbff8

void clint_init(clint_drv_t *dev, uintptr_t base, int ncores);
void clint_send_ipi(clint_drv_t *dev, int target_core);
int clint_recv_ipi(clint_drv_t *dev, int target_core);
void clint_clear_ipi(clint_drv_t *dev, int target_core);
void clint_set_mtimecmp(clint_drv_t *dev, int target_core, uintptr_t mtimecmp);
void clint_set_mtime(clint_drv_t *dev, uintptr_t mtime);
void clint_set_timer_period(clint_drv_t *dev, int target_core, uintptr_t period);
uintptr_t clint_get_mtimecmp(clint_drv_t *dev, int target_core);
uintptr_t clint_get_mtime(clint_drv_t *dev);
uintptr_t clint_get_timer_period(clint_drv_t *dev, int target_core);

#endif /* __CLINT_H__ */
