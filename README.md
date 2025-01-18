# RISC-V software Baremetal environment (riscvbarelib)

## Description

This repository contains a baremetal runtime for systems integrating RISC-V cores.
This riscvbarelib supports mono- or multi-core RISC-V systems.

In order to enable the support on different "platforms", there is a BSP (Board Support Package) layer that provides a hardware abstraction layer (HAL) to the operating system.

The common/ and include/ directores contains the procedures and header files of the runtime, respectively.


## Features

- Small footprint OS (less than 100 KiBytes).
- Support of interrupts.
- Provide configurable callbacks for handling interrupts and exceptions.
- Kernel and applications are run on the same privilege level. By default the privilege level is "machine". However, user applications may change it if needed.
- No support is currently provided for virtual memory.
- Uses the newlib C library


## Layers

    -------------------------
    |          APP          | ->  USER APPLICATIONS
    -------------------------
    |         COMMON        | ->  RISCVBARELIB KERNEL
    -------------------------
    |          BSP          | ->  RISCVBARELIB HAL
    =========================
    |       HARDWARE        |
    -------------------------


### Compilation

To compile the library, use the following command:

```sh
    make BSP=<path to the target BSP> \
         O=<path to the output installation path of the BSP>
```

Some example BSPs are provided in the bsp/ subdirectory.

The output path (O) shall contain after the installation the following files:

- Static library file (librvb.a)
- Linker script (linkcmds.include) for the target BSP
- Makefile (makefile.include) for user applications (APP)
- Object files (\*.o)

The makefile.include shall be included from the Makefile of user's applications to compile the applications with the riscvbarelib runtime and the target BSP.
