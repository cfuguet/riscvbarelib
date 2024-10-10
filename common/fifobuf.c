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
 *  @file   common/fifobuf.c
 *  @author Cesar Fuguet
 *  @date   April, 2022
 */
#include "common/fifobuf.h"

void fifobuf_init(fifobuf_t *q)
{
    ticket_mutex_init(&q->lock);
    list_head_init(&q->list);
}

int fifobuf_is_empty(fifobuf_t *q)
{
    ticket_mutex_lock(&q->lock);
    int ret = list_is_empty(&q->list);
    ticket_mutex_unlock(&q->lock);
    return ret;
}

void fifobuf_push(fifobuf_t *q, fifobuf_node_t *n)
{
    ticket_mutex_lock(&q->lock);
    list_add_last(&q->list, &n->member);
    ticket_mutex_unlock(&q->lock);
}

fifobuf_node_t* fifobuf_pop(fifobuf_t *q)
{
    fifobuf_node_t* ret;
    ticket_mutex_lock(&q->lock);
    ret = list_first_entry_or_null(&q->list, fifobuf_node_t, member);
    if (ret != NULL) {
        cpu_dcache_invalidate_range((uintptr_t)ret, sizeof(fifobuf_node_t));
        list_del(&ret->member);
    }
    ticket_mutex_unlock(&q->lock);
    return ret;
}

