/**
 *  Copyright (c) - CEA List
 *  @file   erac/bsp/bsp_irq.h
 *  @author Cesar Fuguet Tortolero
 *  @date   March, 2022
 *  @brief  Description of routines regarding the IRQ handlers and IPIs
 */
#ifndef __BSP_IRQ_H__
#define __BSP_IRQ_H__

#include <stdint.h>
#include "drivers/clint.h"

clint_drv_t* bsp_get_clint_driver(int hartid);
void bsp_irq_init();

#endif // __BSP_IRQ_H__
