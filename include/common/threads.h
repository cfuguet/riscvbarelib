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
 *  @file   include/common/threads.h
 *  @author Cesar Fuguet
 *  @brief  This file describes the routines and structures used to manage
 *          threads in multicore platforms
 */
#ifndef __THREADS_H__
#define __THREADS_H__

#include "common/mp.h"

#define THREAD_SUCCESS  0
#define THREAD_FAILURE -1

typedef struct thread_s
{
    /* On exit of the thread_create routine, it contains the ID of the thread */
    uint16_t id;

    /* If cpu_id != NULL, specifies the logical ID of the target CPU */
    void *cpu_id;

    /* On exit of the thread create routing, it contains the pointer to the CPU
     * description structure */
    cpu_t *desc;

    /* Can be used by the thread to return something to the director thread
     * when this last calls thread_join */
    void *ret;
} thread_t;

int thread_create(thread_t *t, cpu_entry_func_t func, void *args);
int thread_destroy(thread_t *t);
int thread_join(thread_t *t);
int thread_id();

#endif /* __THREADS_H__ */
