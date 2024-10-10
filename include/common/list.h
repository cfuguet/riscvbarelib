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
 *  @file   list.h
 *  @author Cesar Fuguet
 *  @brief  Generic doubled linked list implementation strongly inspired in
 *          Linux's list.h
 */
#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>
#include "common/cpu.h"
#include "common/cache.h"

struct list_head
{
    struct list_head *prev, *next;
};

static inline struct list_head* __list_read(struct list_head* h)
{
    cpu_dcache_invalidate_range((uintptr_t)h, sizeof(struct list_head));
    return h;
}

#define LIST_HEAD_INIT(name) { &(name), &(name) }

static inline void list_head_init(struct list_head *head)
{
    head = __list_read(head);
    head->prev = head->next = head;
    cpu_mfence();
}

static inline int list_is_empty(struct list_head *head)
{
    return (__list_read(head)->next == head);
}

static inline void list_add_first(struct list_head *head, struct list_head *n)
{
    struct list_head *first;

    head = __list_read(head);
    n = __list_read(n);
    first = __list_read(head->next);

    n->prev = head;
    n->next = first;
    first->prev = n;
    head->next = n;
    cpu_mfence();
}

static inline void list_add_last(struct list_head *head, struct list_head *n)
{
    struct list_head *last;

    head = __list_read(head);
    n = __list_read(n);
    last = __list_read(head->prev);

    n->prev = last;
    n->next = head;
    last->next = n;
    head->prev = n;
    cpu_mfence();
}

static inline void list_del_first(struct list_head *head)
{
    struct list_head *h, *hnext;

    head = __list_read(head);
    h = __list_read(head->next);
    hnext = __list_read(h->next);

    hnext->prev = head;
    head->next  = h->next;
    h->prev = h->next = h;
    cpu_mfence();
}

static inline void list_del_last(struct list_head *head)
{
    struct list_head *t, *tprev;

    head = __list_read(head);
    t = __list_read(head->prev);
    tprev = __list_read(t->prev);

    tprev->next = head;
    head->prev = t->prev;
    t->prev = t->next = t;
    cpu_mfence();
}

static inline void list_del(struct list_head *entry)
{
    struct list_head *p, *n;

    entry = __list_read(entry);
    p = __list_read(entry->prev);
    n = __list_read(entry->next);

    p->next = n;
    n->prev = p;
    entry->prev = entry->next = entry;
    cpu_mfence();
}

static inline int list_is_first(struct list_head *head, struct list_head* n)
{
    return (__list_read(head)->next == n);
}

static inline int list_is_last(struct list_head *head, struct list_head* n)
{
    return (__list_read(head)->prev == n);
}

static inline int list_is_head(struct list_head *head, struct list_head* n)
{
    return (head == n);
}

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({          \
    void *__mptr = (void *)(ptr);                   \
    ((type *)(__mptr - offsetof(type, member))); })

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((__list_read(ptr))->next, type, member)

/**
 * list_last_entry - get the last element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_last_entry(ptr, type, member) \
    list_entry((__list_read(ptr))->prev, type, member)


/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define list_first_entry_or_null(ptr, type, member) ({        \
    struct list_head *head__ = __list_read(ptr);              \
    struct list_head *pos__ = head__->next;                   \
    pos__ != head__ ? list_entry(pos__, type, member) : NULL; \
})

/**
 * list_next_entry - get the next element in list
 * @pos:    the type * to cursor
 * @member: the name of the list_head within the struct.
 */
#define list_next_entry(pos, member) ({                                  \
    cpu_dcache_invalidate_range((uintptr_t)pos, sizeof(typeof(*(pos)))); \
    list_entry((pos)->member.next, typeof(*(pos)), member)               \
    })

/**
 * list_prev_entry - get the prev element in list
 * @pos:    the type * to cursor
 * @member: the name of the list_head within the struct.
 */
#define list_prev_entry(pos, member) ({                                  \
    cpu_dcache_invalidate_range((uintptr_t)pos, sizeof(typeof(*(pos)))); \
    list_entry((pos)->member.prev, typeof(*(pos)), member)               \
    })

/**
 * list_for_each    -   iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */
#define list_for_each(pos, head)          \
    for (pos = (__list_read(head))->next; \
         !list_is_head(pos, (head));      \
         pos = (__list_read(pos))->next)

/**
 * list_for_each_continue - continue iteration over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 *
 * Continue to iterate over a list, continuing after the current position.
 */
#define list_for_each_continue(pos, head) \
    for (pos = (__list_read(pos))->next;  \
         !list_is_head(pos, (head));      \
         pos = (__list_read(pos))->next)

/**
 * list_for_each_prev   -   iterate over a list backwards
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */
#define list_for_each_prev(pos, head)     \
    for (pos = (__list_read(head))->prev; \
         !list_is_head(pos, (head));      \
         pos = (__list_read(pos))->prev)

/**
 * list_entry_is_head - test if the entry points to the head of the list
 * @pos:    the type * to cursor
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_entry_is_head(pos, head, member) ({                         \
    cpu_dcache_invalidate_range((uintptr_t)pos, sizeof(typeof(*(pos)))); \
    (&pos->member == (head))                                             \
    )}

/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_for_each_entry(pos, head, member)                  \
    for (pos = list_first_entry(head, typeof(*pos), member);    \
         !list_entry_is_head(pos, head, member);                \
         pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_for_each_entry_reverse(pos, head, member)          \
    for (pos = list_last_entry(head, typeof(*pos), member);     \
         !list_entry_is_head(pos, head, member);                \
         pos = list_prev_entry(pos, member))

/**
 * list_for_each_entry_continue - continue iteration over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define list_for_each_entry_continue(pos, head, member)         \
    for (pos = list_next_entry(pos, member);                    \
         !list_entry_is_head(pos, head, member);                \
         pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_continue_reverse - iterate backwards from the given point
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after
 * the current position.
 */
#define list_for_each_entry_continue_reverse(pos, head, member) \
    for (pos = list_prev_entry(pos, member);                    \
         !list_entry_is_head(pos, head, member);                \
         pos = list_prev_entry(pos, member))

/**
 * list_for_each_entry_from - iterate over list of given type from the current point
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define list_for_each_entry_from(pos, head, member)             \
    for (; !list_entry_is_head(pos, head, member);              \
         pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_from_reverse - iterate backwards over list of given type
 *                                    from the current point
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Iterate backwards over list of given type, continuing from current position.
 */
#define list_for_each_entry_from_reverse(pos, head, member)     \
    for (; !list_entry_is_head(pos, head, member);              \
         pos = list_prev_entry(pos, member))


#endif /* __LIST_H__ */
