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
 *  @file   common/ticket_mutex.c
 *  @author Cesar Fuguet
 *  @date   March, 2022
 *  @brief  This file describes the routines and structures used to manage
 *          mutexes (mutual exclusion locks)
 */
#include "common/ticket_mutex.h"
#include "common/cpu.h"

#define MUTEX_WAIT_DELAY 500

void ticket_mutex_init(ticket_mutex_t *m)
{
    atomic_exchange(&m->curr, 0);
    atomic_exchange(&m->next, 0);
}

void ticket_mutex_destroy(ticket_mutex_t *m)
{
}

void ticket_mutex_lock(ticket_mutex_t *m)
{
    cpu_dfence();
    int ticket = atomic_fetch_add(&m->next, 1);
    while(ticket != atomic_fetch_or(&m->curr, 0)) {
        cpu_delay(MUTEX_WAIT_DELAY);
    }
}

int ticket_mutex_trylock(ticket_mutex_t *m)
{
    ticket_mutex_lock(m);
    return 1;
}

void ticket_mutex_unlock(ticket_mutex_t *m)
{
    cpu_dfence();
    atomic_fetch_add(&m->curr, 1);
}
