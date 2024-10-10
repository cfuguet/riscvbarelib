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
 *  @file   common/syscall.c
 *  @author Cesar Fuguet
 */
#include "common/syscall.h"
#include "common/compiler.h"
#include "common/ticket_mutex.h"
#include "common/threads.h"
#include "common/cpu.h"
#include "bsp/bsp_config.h"
#include <sys/times.h>
#include <sys/time.h>
#include <sys/reent.h>

void (*_putchar)(char c)         = NULL;
int  (*_getchar)()               = NULL;
void (*_tohost_exit)(int status) = NULL;

void *_sbrk(int incr)
{
    extern int _end;
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    if (heap == NULL) heap = (unsigned char*)&_end;

    prev_heap = heap;
    heap += incr;
    return prev_heap;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    cpu_dcache_invalidate_range((uintptr_t)st, sizeof(void*));
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

//
//  Callback that can be overrided by a function (with the same name) defined
//  in the user application.
//
//  It is called on exit.
//
//  By default, it does nothing
//
void __WEAK__ cb_on_exit(int status)
{
    (void)status;
}


void _exit(int status)
{
    if (thread_id() == 0) {
        cb_on_exit(status);

        if (_tohost_exit != NULL) {
            _tohost_exit(status);
        }
    }
    while (1);
}

void _kill(int pid, int sig)
{
    return;
}

int _getpid(void)
{
    return -1;
}

int _write(int file, char * ptr, int len)
{
    int written = 0;

    if ((file != 1) && (file != 2)) return -1;
    if (_putchar == NULL) return -1;

    cpu_dcache_invalidate_range((uintptr_t)ptr, len);
    for (; len != 0; --len) {
        _putchar(*ptr++);
        ++written;
    }
    return written;
}

int _read(int file, char * ptr, int len)
{
    int read = 0;

    if (file != 0) return -1;
    if (_getchar == NULL) return -1;

    cpu_dcache_invalidate_range((uintptr_t)ptr, len);
    for (; len > 0; --len) {
        *ptr = _getchar();
        read++;
        ptr++;
    }
    return read;
}

/* Timing information for current process.
 *
 * From newlib/libc/include/sys/times.h the tms struct fields are as follows:
 *       - clock_t tms_utime  : user clock ticks
 *       - clock_t tms_stime  : system clock ticks
 *       - clock_t tms_cutime : children's user clock ticks
 *       - clock_t tms_cstime : children's system clock ticks
 */
clock_t _times(struct tms *buf)
{
    buf->tms_utime = cpu_cycles();
    buf->tms_stime = buf->tms_cstime = buf->tms_cutime = 0;
    return buf->tms_utime;
}
