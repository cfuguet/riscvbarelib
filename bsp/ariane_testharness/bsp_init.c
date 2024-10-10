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
 *  @file   ariane_testharness/bsp_init.c
 *  @author Cesar Fuguet
 */
#include <string.h>
#include <stdio.h>
#include "common/cpu.h"
#include "common/cpu_defs.h"
#include "common/tohost.h"
#include "common/mp.h"
#include "common/io.h"

extern uintptr_t UART_BASE;
void bsp_mp_init();
void bsp_irq_init();

#ifndef NSTDOUT
#  ifndef TTY_UART16550
void simple_putchar(char c)
{
    iowriteb((uintptr_t)&UART_BASE, c);
}
int simple_getchar()
{
    return 0;
}
#  else
#    include "drivers/uart16550.h"
#  endif
#else
void dummy_putchar(char c)
{
    (void)c;
}
int dummy_getchar()
{
    return 0;
}
#endif

void bsp_init()
{
    extern void (*_putchar)(char c);
    extern int  (*_getchar)();
    extern void (*_tohost_exit)(int status);

#ifndef NSTDOUT
#  ifndef TTY_UART16550
    _putchar = simple_putchar;
    _getchar = simple_getchar;
#  else
    uart16550_init((uintptr_t)&UART_BASE);
    _putchar = uart16550_putchar;
    _getchar = uart16550_getchar;
#  endif
#else
    _putchar = dummy_putchar;
    _getchar = dummy_getchar;
#endif

    _tohost_exit = bsp_tohost_exit;

    printf("Executing the bare cea riscv environment (compiled: %s | %s)\n",
            __DATE__, __TIME__);

    write_csr(CSR_MHPMEVENT3, 1); // select Icache Miss Event
    write_csr(CSR_MHPMEVENT4, 2); // select Dcache Miss Event

    bsp_mp_init();
    bsp_irq_init();
}

void bsp_mp_init()
{
    memset((void*)cpu_hid2sid, 0xff, sizeof(cpu_hid2sid));
    memset((void*)cpu_sid2hid, 0xff, sizeof(cpu_sid2hid));

    for (int i = 0; i < BSP_CONFIG_NCPUS; i++) {
        //  In this platform. the hart and logical IDs are the same.
        cpu_hid2sid[i] = i;

        //  In this platform. the hart and logical IDs are the same.
        cpu_sid2hid[i] = i;

        //  By default consider that all CPUs are in the IDLE state
        cpu_set_state(i, CPU_IDLE);
    }

    cpu_dfence();
}
