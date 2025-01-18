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
 *  @file   common/mem.c
 *  @author Cesar Fuguet
 *  @brief  This file describes the optimized routines for memory manipulation
 */
#include <stdint.h>
#include <string.h>

static inline void* __memset_byte(
        unsigned char *s,
        unsigned char c,
        size_t nbytes)
{
    for (; nbytes > 0; nbytes--)
        *s++ = c;

    return (void*)s;
}

static inline void* __memset_word(
        unsigned long *s,
        unsigned char c,
        size_t nwords)
{
    unsigned long word = (c << 24) | (c << 16) | (c << 8) | c;

#if __SIZEOF_LONG__ == 8
    word = (word << 32) | word;
#endif

    for (; nwords > 0; nwords--)
        *s++ = word;

    return (void*)s;
}

void *memset(void *s, int c, size_t n)
{
    const void *_s = s;
    const size_t wsize = sizeof(long);

    if ((((uintptr_t)s % wsize) != 0) && (n >= wsize)) {
        size_t nbytes = wsize - ((uintptr_t)s % wsize);
        s = __memset_byte((unsigned char*)s, (unsigned char)c,
                nbytes);
        n -= nbytes;
    }

    if (n >= wsize) {
        s = __memset_word((unsigned long*)s, (unsigned char)c,
                n / wsize);
        n %= wsize;
    }

    if (n > 0)
        __memset_byte((unsigned char*)s, (unsigned char)c, n);

    return (void*)_s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    size_t _n = n;
    uintptr_t *_dest = (uintptr_t*)dest;
    uintptr_t *_src = (uintptr_t*)src;

    uintptr_t s0, s1, s2, s3;
    if (_n >= (4*sizeof(uintptr_t))) {
        s0 = _src[0];
        s1 = _src[1];
        s2 = _src[2];
        s3 = _src[3];
        while (_n >= (4*sizeof(uintptr_t))) {
            _src += 4;
            _n -= 4*sizeof(uintptr_t);
            _dest[0] = s0;
            s0 = _src[0];
            _dest[1] = s1;
            s1 = _src[1];
            _dest[2] = s2;
            s2 = _src[2];
            _dest[3] = s3;
            s3 = _src[3];
            _dest += 4;
        }
    }

    while (_n--) {
        *_dest = *_src;
        _dest++;
        _src++;
    }

    return dest;
}
