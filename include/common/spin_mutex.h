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
 *  @file   common/spin_mutex.h
 *  @author Cesar Fuguet
 *  @date   March, 2022
 *  @brief  This file describes the routines and structures used to manage
 *          mutexes (mutual exclusion locks)
 */
#ifndef __SPIN_MUTEX_H__
#define __SPIN_MUTEX_H__

#include <stdatomic.h>
#include "common/cache.h"

typedef struct {
    atomic_int lock __cl_aligned__;
} spin_mutex_t;

void spin_mutex_init(spin_mutex_t *m);
void spin_mutex_destroy(spin_mutex_t *m);
void spin_mutex_lock(spin_mutex_t *m);
int spin_mutex_trylock(spin_mutex_t *m);
void spin_mutex_unlock(spin_mutex_t *m);

#endif /* __SPIN_MUTEX_H__ */
