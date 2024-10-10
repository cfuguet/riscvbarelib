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
 *  @file   include/common/ticket_mutex.h
 *  @date   March, 2022
 *  @author Cesar Fuguet
 *  @brief  This file describes the routines and structures used to manage
 *          mutexes (mutual exclusion locks)
 */
#ifndef __TICKET_MUTEX_H__
#define __TICKET_MUTEX_H__

#ifdef __cplusplus
  #include <atomic>
  using std::atomic_int;

  extern "C" {
#else /* not __cplusplus */
  #include <stdatomic.h>
#endif /* __cplusplus */
#include "common/cache.h"

typedef struct {
    atomic_int curr __cl_aligned__;
    atomic_int next __cl_aligned__;
} ticket_mutex_t;

void ticket_mutex_init(ticket_mutex_t *m);
void ticket_mutex_destroy(ticket_mutex_t *m);
void ticket_mutex_lock(ticket_mutex_t *m);
int ticket_mutex_trylock(ticket_mutex_t *m);
void ticket_mutex_unlock(ticket_mutex_t *m);


#ifdef __cplusplus
}
#endif

#endif /* __TICKET_MUTEX_H__ */
