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
 *  @file        bitset.h
 *  @author      Valentin Isaac--Chassande
 */

#ifndef _BITSET_H
#define _BITSET_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Structure containing the table of bits.
 * (64 bits are stored in a double precision word)
 */
typedef volatile struct {
    uint64_t * tab;
    int n;
    int tab_length;
} bitset_t;

/*
 * Allocate and initialize the bitset structure.
 * All bits are reset (set to 0).
 */
void bitset_init(bitset_t * bitset, int n);

/*
 * Set a bit (to 1).
 */
void bitset_set(bitset_t * bitset, int i);

/*
 * Reset a bit (set to 0).
 */
void bitset_reset(bitset_t * bitset, int i);

/*
 * Flip a bit (from 1 to 0 or 0 to 1).
 */
void bitset_flip(bitset_t * bitset, int i);

/*
 * Test if a bit is set.
 * Return the bit value.
 */
int bitset_test(bitset_t * bitset, int i);

/*
 * Test if all bits are set.
 * Return 1 if it is true, 0 else.
 */
int bitset_all(bitset_t * bitset);

/*
 * Test if at least 1 bit is set.
 * Return 1 if it is true, 0 else.
 */
int bitset_any(bitset_t * bitset);

/*
 * Test if none of the bits are set (all are reset).
 * Return 1 if it is true, 0 else.
 */
int bitset_none(bitset_t * bitset);

/*
 * Return the number of bit set.
 */
int bitset_count(bitset_t * bitset);

/*
 * Return the number of bit.
 */
int bitset_size(bitset_t * bitset);

/*
 * Destroy the bitset structure.
 */
void bitset_destroy(bitset_t * bitset);

#ifdef __cplusplus
}
#endif

#endif /* _BITSET_H */
