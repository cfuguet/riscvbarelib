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
 *  @file        bitset.c
 *  @author      Valentin Isaac--Chassande
 */
#include "common/bitset.h"

void bitset_init(bitset_t * bitset, int n) {
    bitset->n = n;
    bitset->tab_length = n/64+1;
    bitset->tab = (uint64_t *)malloc(bitset->tab_length*sizeof(uint64_t));
    for (int i = 0; i < bitset->tab_length; i++)
        bitset->tab[i] = 0;
}

void bitset_set(bitset_t * bitset, int i) {
    if (i >= bitset->n)
        return;
    int idx = i%64;
    int block_idx = i/64;
    uint64_t block = bitset->tab[block_idx];
    block |= ((uint64_t)1<<(63-idx));
    bitset->tab[block_idx] = block;
}

void bitset_reset(bitset_t * bitset, int i) {
    if (i >= bitset->n)
        return;
    int idx = i%64;
    int block_idx = i/64;
    uint64_t block = bitset->tab[block_idx];
    block &= ~((uint64_t)1<<(63-idx));
    bitset->tab[block_idx] = block;
}

void bitset_flip(bitset_t * bitset, int i) {
    if (i >= bitset->n)
        return;
    int idx = i%64;
    int block_idx = i/64;
    uint64_t block = bitset->tab[block_idx];
    block ^= ((uint64_t)1<<(63-idx));
    bitset->tab[block_idx] = block;
}

int bitset_test(bitset_t * bitset, int i) {
    if (i >= bitset->n)
        return 0;
    int idx = i%64;
    int block_idx = i/64;
    uint64_t block = bitset->tab[block_idx];
    return (int)((block>>(63-idx))&(uint64_t)1);
}

int bitset_all(bitset_t * bitset) {
    for (int i = 0; i < bitset->tab_length-1; i++)
        if (bitset->tab[i] != 0xffffffffffffffff)
            return 0;
    uint64_t last = 0xffffffffffffffff<<(64-bitset->n%64);
    if (bitset->tab[bitset->tab_length-1] != last)
        return 0;
    return 1;
}

int bitset_any(bitset_t * bitset) {
    for (int i = 0; i < bitset->tab_length; i++)
        if (bitset->tab[i] != 0x0)
            return 1;
    return 0;
}

int bitset_none(bitset_t * bitset) {
    for (int i = 0; i < bitset->tab_length; i++)
        if (bitset->tab[i] != 0x0)
            return 0;
    return 1;
}

int bitset_count(bitset_t * bitset) {
    int cnt = 0;
    for (int i = 0; i < bitset->tab_length; i++)
        for (int j = 0; j < 64; j++)
            if (bitset->tab[i] & ((uint64_t)1<<j))
                cnt += 1;
    return cnt;
}

int bitset_size(bitset_t * bitset) {
    return bitset->n;
}

void bitset_destroy(bitset_t * bitset) {
    free(bitset->tab);
}
