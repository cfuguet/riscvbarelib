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
 *  @file   shared/bsp_start.c
 *  @author Cesar Fuguet
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bsp/bsp_config.h"
#include "bsp/bsp_irq.h"
#include "common/mp.h"
#include "common/cache.h"
#include "common/compiler.h"
#include "common/cpu.h"
#include "common/cpu_defs.h"
#include "common/threads.h"

extern void __libc_init_array();
extern void bsp_init();
extern int main();
extern uintptr_t _sbss;
extern uintptr_t _ebss;

//
//  Wait for a software inter-processor interrupt
//
//  The processor loops until the Machine Software Interrupt Pending (MSIP)
//  bit in the MIP register is set
//
//  We use WFI hint instruction here. If the hardware implements this
//  instruction, the core enters a low-energy consumption mode waiting for
//  the interrupt.
//
static void bsp_start_wait_for_ipi()
{
    asm volatile (
        "1:                         \n"
        "wfi                        \n"
        "csrr  t0, mip              \n"
        "andi  t0, t0, %[mask]      \n"
        "beq   t0, x0, 1b           \n"
        :
        : [mask] "i"(MIP_MSIP)
        : "t0", "memory"
    );
}


//
//  Callback that can be overrided by a function (with the same name) defined
//  in the user application.
//
//  It is called after the main.
//
//  By default, it does nothing
//
void __WEAK__ cb_end_of_main()
{

}


//
//  Start routine for main/boot CPU
//
static void bsp_primary_start()
{
    //  Initialize the BSS data section
    uintptr_t bss_bytes = (uintptr_t)&_ebss - (uintptr_t)&_sbss;
    if (bss_bytes > 0) memset((void*)&_sbss, 0, bss_bytes);

    //  Call constructors
    __libc_init_array();

    //  Call BSP initialization routines
    bsp_init();

    //  Get logical CPU ID
    int      hid = cpu_id();
    uint16_t sid = cpu_hid2sid[hid];
    if (sid != 0) exit(EXIT_FAILURE);

    //  Initialize the per-cpu description structure
    cpu_t* this_cpu = (cpu_t*)&cpu_list[0];
    this_cpu->sid        = 0;
    this_cpu->hid        = hid;
    this_cpu->entry_func = (cpu_entry_func_t)main;
    this_cpu->args       = NULL;
    this_cpu->clint_drv  = bsp_get_clint_driver(hid);
    this_cpu->state      = CPU_RUNNING;

    //  Initialize the per-cpu description of secondary CPUs
    for (int i = 1; i < BSP_CONFIG_NCPUS; i++) {
        cpu_list[i].sid        = i;
        cpu_list[i].hid        = cpu_sid2hid[i];
        cpu_list[i].clint_drv  = bsp_get_clint_driver(cpu_sid2hid[i]);
        cpu_list[i].entry_func = NULL;
        cpu_list[i].args       = NULL;
        cpu_list[i].state      = CPU_IDLE;
    }

    //  Synchronize pending writes
    cpu_dfence();

    //  Save the per-cpu description into the thread pointer
    cpu_set_thread_pointer((uintptr_t)&cpu_list[0]);

    //  Initialize the thread information
    this_cpu->thread         = (thread_t*)malloc(sizeof(thread_t));
    if (this_cpu->thread == NULL) exit(EXIT_FAILURE);

    this_cpu->thread->id     = 0;
    this_cpu->thread->cpu_id = (void*)((uintptr_t)hid);
    this_cpu->thread->desc   = (cpu_t*)&cpu_list[0];
    this_cpu->thread->ret    = NULL;

    //  Initialize performance counters
    cpu_set_cycles(0);
    cpu_set_instructions(0);
    cpu_set_imiss(0);
    cpu_set_dmiss(0);

    //  Call the main function
    int status = main();

    //  End of main callback
    cb_end_of_main();

    //  call exit with the main routine's return status
    exit(status);
}

//
//  Start routine for secondary CPUs
//
static void bsp_secondary_start()
{
    //  If there is no hardware cache coherency, make sure that the L1 dcache
    //  is cold.
    cpu_dcache_invalidate();

    //  Get logical CPU ID
    int      hid = cpu_id();
    uint16_t sid = cpu_hid2sid[hid];

    //  Acknowledge the IPI interrupt
    int clint_status = clint_recv_ipi(cpu_list[sid].clint_drv, hid);
    if (clint_status == 0) {
        //  CPU woke up without IPI
        cpu_set_state(sid, CPU_ERROR);
        while(1);
    }

    //  Check the kind of inter-processor interrupt
    if (cpu_get_ipi_kind(sid) != CPU_IPI_CREATE) {
        //  Unexpected IPI
        cpu_set_state(sid, CPU_ERROR);
        while(1);
    }

    //  Save the per-cpu description into the thread pointer
    cpu_set_thread_pointer((uintptr_t)&cpu_list[sid]);

    //  Signal the director thread that this thread is now awake
    cpu_dfence();
    cpu_set_state(sid, CPU_AWAKE);

    //  Wait for the director thread to declare this thread as running
    while (cpu_get_state(sid) != CPU_RUNNING) cpu_delay(1000);

    //  Initialize performance counters
    cpu_set_cycles(0);
    cpu_set_instructions(0);
    cpu_set_imiss(0);
    cpu_set_dmiss(0);

    //  Call the entry function
    cpu_entry_func_t entry_func = cpu_get_entry_func(sid);
    int status = entry_func(cpu_get_args(sid));

    //  Thread finished
    if (status == THREAD_FAILURE) {
        cpu_dfence();
        cpu_set_state(sid, CPU_ERROR);
        while (1);
    }
    cpu_dfence();
    cpu_set_state(sid, CPU_IDLE);
}

//
//  Entry function for the start routine of all cores in the platform
//
void bsp_start(int hartid)
{
#ifndef BSP_CONFIG_NOXS
    // Enable the VP FPU (Xvpfloat extension)
    asm volatile (
        "li t0, %[clear_xs] \n"
        "csrc mstatus, t0   \n"
        "li t0, %[set_xs]   \n"
        "csrs mstatus, t0   \n"
        : /* no outputs */
        : [clear_xs] "i"(MSTATUS_XS),
          [set_xs]   "i"(MSTATUS_XS_CLEAN)
        : "t0", "memory"
    );
#endif

#ifdef BSP_CONFIG_FPU
    // Enable the STD FPU (RVF and RVD extensions)
    asm volatile (
        "li t0, %[clear_fs] \n"
        "csrc mstatus, t0   \n"
        "li t0, %[set_fs]   \n"
        "csrs mstatus, t0   \n"
        : /* no outputs */
        : [clear_fs] "i"(MSTATUS_FS),
          [set_fs]   "i"(MSTATUS_FS_CLEAN)
        : "t0", "memory"
    );
#endif

    if (hartid == BSP_CONFIG_HARTID_BOOT) {
        bsp_primary_start();
        return;
    }
    while (1) {
        bsp_start_wait_for_ipi();
        bsp_secondary_start();
    }
}
