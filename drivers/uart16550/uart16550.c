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
 *  @file   drivers/uart16550/uart16550.c
 *  @author Cesar Fuguet
 */
#include <drivers/uart16550.h>
#include <common/io.h>

#define TX_FILL_MAX          16

#if   (UART_IO_WIDTH == 1)
#define uart_write iowriteb
#define uart_read  ioreadb
#elif (UART_IO_WIDTH == 2)
#define uart_write iowriteh
#define uart_read  ioreadh
#elif (UART_IO_WIDTH == 4)
#define uart_write iowritew
#define uart_read  ioreadw
#elif (UART_IO_WIDTH == 8)
#define uart_write iowritel
#define uart_read  ioreadl
#endif

static uintptr_t uart16550_base;
static int       uart16550_fillstate;

void uart16550_init(uintptr_t base)
{
    /* Initialize base address and fillstate of the UART */
    uart16550_base = base;
    uart16550_fillstate = 0;

    /* Enable access to the divisor latch */
    uart_write(base | UART16550_LCR, 0x80);

    /* Configure divisor latch */
    const unsigned int target_freq = UART_BAUDRATE*16;
    const uint16_t divisor = (UART_REFCLK/target_freq);
    uart_write(base | UART16550_DLL, (divisor >> 0) & 0xff);
    uart_write(base | UART16550_DLM, (divisor >> 8) & 0xff);

    /* Enable access to data FIFOs and select word length of 8 bits */
    uart_write(base | UART16550_LCR, 0x03);
}

static void uart16550_wait_tx_fifo_empty() {
    uint8_t lsr;
    do {
        lsr = uart_read(uart16550_base | UART16550_LSR);
    } while (((lsr >> 6) & 0x1) == 0x0);
    uart16550_fillstate = 0;
}

void uart16550_putchar(char c)
{
    if (c == '\n') uart16550_putchar('\r');

    uart_write(uart16550_base | UART16550_THR, (uint8_t)c);
    uart16550_fillstate++;
    uart16550_wait_tx_fifo_empty();
}

int uart16550_getchar()
{
    return (int)uart_read(uart16550_base | UART16550_RBR);
}
