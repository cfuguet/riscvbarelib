# RISC-V software Baremetal environment (riscvbarelib)

## Description

This repository contains a baremetal runtime for systems integrating RISC-V cores.
This riscvbarelib supports mono- or multi-core RISC-V systems.

In order to enable the support on different "platforms", there is a BSP (Board Support Package) layer that provides a hardware abstraction layer (HAL) to the operating system.

The common/ and include/ directories contains the procedures and header files of the runtime, respectively.


## Features

- Small footprint OS (less than 100 KiBytes).
- Support of interrupts.
- Provide configurable callbacks for handling interrupts and exceptions.
- Kernel and applications are ran on the same privilege level. By default the privilege level is "machine". However, user applications may change it.
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


## Compilation

### Dependencies

You need a working RISC-V cross-toolchain compiled together with the newlib library.

The following repository [crosstool_scripts](https://github.com/cfuguet/crosstool_scripts) provides a Python script to generate a compatible RISC-V cross-toolchain.
In particular, the script compiles the newlib library with some specific flags to make it compatible with this riscvbarelib library.

### Procedure

To compile the library, use the following command:

```sh
make BSP=<path to the target BSP> \
     XLEN=<32/64/128: default is 64> \
     O=<path to the output installation path of the BSP: default is ./build>
```

Some example BSPs are provided in the bsp/ subdirectory.

The output path (O) shall contain after the installation the following files:

- Static library file (librvb.a)
- Linker script (linkcmds.include) for the target BSP
- Makefile (makefile.include) for user applications (APP)
- Object files (\*.o)

The makefile.include shall be included from the Makefile of user's applications to compile the applications with the riscvbarelib runtime and the target BSP.

## Board Support Packages (BSP)

### bsp/ariane_testharness

This BSP implements the platform-specific functions for the [CVA6](https://github.com/openhwgroup/cva6) ariane-testharness simulation platform.
This platform is, for the essential, compatible to the Hifive board address map. Therefore, you can use this with other simulators such as QEMU and its "virt" platform, or gem5 and its RISC-V reference configuration.

When compiling the riscvbarelib for this BSP, you can add additional parameters:

```sh
make BSP=bsp/ariane_testharness \
     BSP_FLOAT=<support of single/double floating point: default is 1> \
     BSP_COMPRESSED=<support of compressed instructions: default is 1> \
     BSP_ATOMIC=<support of atomic memory operations: default is 1> \
     BSP_NCPUS=<number of cores in the platform: default is 1> \
     M5=<Use m5ops library for Gem5 simulation: default is 0> \
     O=<path to the output installation path of the BSP>
```
