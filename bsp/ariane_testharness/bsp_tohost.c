/**
 * Copyright 2023,2024 CEA*
 * Commissariat a l'Energie Atomique et aux Energies Alternatives
 * Copyright 2025 Inria, Univ. Grenoble Alpes, TIMA
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
 *  @file   ariane_testharness/bsp_tohost.c
 *  @author Cesar Fuguet
 */
#include <stdlib.h>
#include "common/tohost.h"
#include "common/io.h"

#ifdef M5
#include "m5ops.h"
#else
__attribute__ ((section(".tohost")))
uint64_t tohost;
#endif

void bsp_tohost_exit(int status)
{
#ifdef M5
    if (status == EXIT_SUCCESS) {
        m5_exit(10);
    } else {
        m5_fail(10, status);
    }
#elif QEMU
    static const uint32_t EXIT_SUCCESS_CODE = 0x00005555U;
    static const uint32_t EXIT_FAILURE_CODE = 0x00003333U;
    iowritew((uintptr_t)0x100000, status == EXIT_SUCCESS ?
            EXIT_SUCCESS_CODE : EXIT_FAILURE_CODE);
#else
    static const uint64_t EXIT_SUCCESS_CODE = 0x00000001ULL;
    static const uint64_t EXIT_FAILURE_CODE = 0xbad0bad1ULL;
    iowritel((uintptr_t)&tohost, status == EXIT_SUCCESS ?
            EXIT_SUCCESS_CODE : EXIT_FAILURE_CODE);
#endif
    while(1) cpu_wait_for_interrupt();
}
