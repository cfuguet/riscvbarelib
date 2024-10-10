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
 *  @file   common/mp.c
 *  @author Cesar Fuguet
 *  @brief  This file describes the routines and structures used to support
 *          multiple processor cores
 */
#include "common/mp.h"
#include "bsp/bsp_config.h"

#if (BSP_CONFIG_HARTID_BITS > 16)
#error "Number of HartID bits is greater than the supported one (16 bits max)"
#endif

#if (BSP_CONFIG_NCPUS > (1 << BSP_CONFIG_HARTID_BITS))
#error "Number of HartID bits is not enough"
#endif

volatile uint16_t cpu_hid2sid[1 << BSP_CONFIG_HARTID_BITS];
volatile uint16_t cpu_sid2hid[1 << BSP_CONFIG_HARTID_BITS];
volatile cpu_t    cpu_list[BSP_CONFIG_NCPUS];

cpu_t* mp_get_free_cpu()
{
    for (int i = 0; i < BSP_CONFIG_NCPUS; i++) {
        if (cpu_get_state(i) == CPU_IDLE) {
            return (cpu_t*)cpu_get_desc(i);
        }
    }
    return NULL;
}
