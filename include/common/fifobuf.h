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
 *  @file   common/include/fifobuf.h
 *  @author Cesar Fuguet
 *  @date   April, 2022
 *  @brief  Generic payload FIFO buffer
 */
#ifndef __FIFOBUF_H__
#define __FIFOBUF_H__

#include "common/list.h"
#include "common/ticket_mutex.h"

typedef struct fifobuf_s
{
    ticket_mutex_t lock;
    struct list_head list;
} fifobuf_t;

typedef struct fifobuf_node_s
{
    void *data;
    struct list_head member;
} fifobuf_node_t;

void fifobuf_init(fifobuf_t *q);

int fifobuf_is_empty(fifobuf_t *q);

void fifobuf_push(fifobuf_t *q, fifobuf_node_t *n);

fifobuf_node_t* fifobuf_pop(fifobuf_t *q);

#endif /* __FIFOBUF_H__ */
