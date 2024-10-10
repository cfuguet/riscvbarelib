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
 *  @file   io.h
 *  @author Cesar Fuguet
 */
#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>
#include "cpu.h"

static inline void iowriteb(uintptr_t addr, uint8_t val)
{
	*((volatile uint8_t*)addr) = val;
	cpu_dfence();
}

static inline void iowriteh(uintptr_t addr, uint16_t val)
{
	*((volatile uint16_t*)addr) = val;
	cpu_dfence();
}

static inline void iowritew(uintptr_t addr, uint32_t val)
{
	*((volatile uint32_t*)addr) = val;
	cpu_dfence();
}

static inline void iowritel(uintptr_t addr, uint64_t val)
{
	*((volatile uint64_t*)addr) = val;
	cpu_dfence();
}

static inline uint8_t ioreadb(uintptr_t addr)
{
	return *((volatile uint8_t*)addr);
}

static inline uint16_t ioreadh(uintptr_t addr)
{
	return *((volatile uint16_t*)addr);
}

static inline uint32_t ioreadw(uintptr_t addr)
{
	return *((volatile uint32_t*)addr);
}

static inline uint64_t ioreadl(uintptr_t addr)
{
	return *((volatile uint64_t*)addr);
}

static inline double ioreadd(uintptr_t addr)
{
	return *((volatile double*)addr);
}

#define writeb iowriteb
#define writeh iowriteh
#define writew iowritew
#define writel iowritel
#define readb  ioreadb
#define readh  ioreadh
#define readw  ioreadw
#define readl  ioreadl
#define readd  ioreadd

#endif /* __IO_H__ */
