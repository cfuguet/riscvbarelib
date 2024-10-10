# RISC-V software Baremetal environment (riscvbarelib)

## Description

This repository contains a baremetal runtime for systems integrating RISC-V cores.
This riscvbarelib supports mono- or multi-core RISC-V systems.

In order to enable the support on different "platforms", there is a BSP (Board Support Package) layer that provides a hardware abstraction layer (HAL) to the operating system.

The common/ and include/ directores contains the procedures and header files of the OS kernel, respectively.


## Features

- Small footprint OS (less than 100 KiBytes).
- Support of interrupts.
- Provide configurable callbacks for handling interrupts and exceptions.
- Kernel and applications are run on the same privilege level. By default the privilege level is "machine". However, user applications may change it if needed.
- No support is provided for virtual memory. However, user applications may add the support if needed.


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

In order to compile the user applications, consider copying one of the existing Makefile in example user applications, and then modify it as needed.

User makefiles for applications include some global makefiles that are provided with this repository (riscvbarelib). To find them, user makefiles may use some environment variables. To set the environment, users are invited to "source" the setup.csh script at the root of this repository.

Once the environment is set, makefiles require the user to pass the target BSP through the makefile. The procedure is as follows:

    cd <riscvbarelib_directory>
    cd <user_app_directory>
    make BSP=<target_BSP>

As a result of the compilation, you will find a build/ directory. This directory contains at least:

- Object files (\*.o)
- ELF Executable file (\*.x)
- Executable objdump file (\*.x.dump)
