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
 *  @file   ariane_testharness/bsp_tohost.c
 *  @author Cesar Fuguet
 */
#include <stdio.h>

#include <stdlib.h>
#include "common/tohost.h"
#include "common/io.h"

__attribute__ ((section(".tohost")))
#if __riscv_xlen == 128 
__uint128_t tohost;
#elif __risc
uint64_t tohost;
#endif

void bsp_tohost_exit(int status)
{
    static const uint64_t VERIF_SUCCESS_CODE = 0x00000001ULL;
    static const uint64_t VERIF_FAILURE_CODE = 0xbad0bad1ULL;

    #if __riscv_xlen == 128 
    iowritell((uintptr_t)&tohost, status == EXIT_SUCCESS ?
            VERIF_SUCCESS_CODE : VERIF_FAILURE_CODE);
    #else
    iowritel((uintptr_t)&tohost, status == EXIT_SUCCESS ?
            VERIF_SUCCESS_CODE : VERIF_FAILURE_CODE);
    #endif
    while(1);
}
