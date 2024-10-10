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
 *  @file   drivers/uart16550/include/drivers/uart16550.h
 *  @author Cesar Fuguet
 */
#ifndef __UART16550_H__
#define __UART16550_H__

#include <stdint.h>

#ifndef UART_REGSIZE
#define UART_REGSIZE    3
#endif

#ifndef UART_IO_WIDTH
#define UART_IO_WIDTH   1
#endif

#ifndef UART_BAUDRATE
#define UART_BAUDRATE   1
#endif

#ifndef UART_REFCLK
#define UART_REFCLK     ((UART_BAUDRATE)*16*2)
#endif

#define UART16550_RBR   (0 << (UART_REGSIZE)) /* Receiver FIFO; R */
#define UART16550_THR   (0 << (UART_REGSIZE)) /* Transmitter FIFO; W */
#define UART16550_IER   (1 << (UART_REGSIZE)) /* Interrupt Enable Register; R/W */
#define UART16550_IIR   (2 << (UART_REGSIZE)) /* Interrupt Identification Register; R */
#define UART16550_FCR   (2 << (UART_REGSIZE)) /* FIFO Control Register; W */
#define UART16550_LCR   (3 << (UART_REGSIZE)) /* Line Control Register; R/W */
#define UART16550_MCR   (4 << (UART_REGSIZE)) /* Model Control Register; R/W */
#define UART16550_LSR   (5 << (UART_REGSIZE)) /* Line Status Register; R */
#define UART16550_MSR   (6 << (UART_REGSIZE)) /* Modem Status Register; R */
#define UART16550_SCR   (7 << (UART_REGSIZE)) /* Scratch Register; R/W */
#define UART16550_DLL   (0 << (UART_REGSIZE)) /* Divisor Latch (LSB); LSR(7) == 1; R/W */
#define UART16550_DLM   (1 << (UART_REGSIZE)) /* Divisor Latch (MSB); LSR(7) == 1; R/W */

void uart16550_init(uintptr_t base);
void uart16550_putchar(char c);
int uart16550_getchar();

#endif /* __UART16550_H__ */
