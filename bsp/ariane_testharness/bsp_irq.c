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
 *  @file   ariane_testharness/bsp_irq.c
 *  @author Cesar Fuguet
 */
#include <stdint.h>
#include <stddef.h>
#include <drivers/clint.h>

clint_drv_t* bsp_get_clint_driver(int hartid)
{
    return NULL;
}

void bsp_irq_init()
{
    /* do nothing */
}
