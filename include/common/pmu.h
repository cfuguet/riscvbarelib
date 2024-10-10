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
 *  @file   pmu.h
 *  @author Eric Guthmuller
 */
#ifndef __PMU_H__
#define __PMU_H__

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "bsp/bsp_pmu.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pmu_object_t
{
    int (*start)(bsp_pmu_object_t *self);
    int (*sample_and_stop)(bsp_pmu_object_t *self);
    int (*accumulate)(bsp_pmu_object_t *self);
    int (*display)(bsp_pmu_object_t *self);
    int (*reset)(bsp_pmu_object_t *self);
    int (*destroy)(bsp_pmu_object_t *self);
    bsp_pmu_object_t obj;
} pmu_object_t;

// Default API in case PMU is disabled
int __pmu_disabled_start(bsp_pmu_object_t *self);
int __pmu_disabled_sample_and_stop(bsp_pmu_object_t *self);
int __pmu_disabled_accumulate(bsp_pmu_object_t *self);
int __pmu_disabled_display(bsp_pmu_object_t *self);
int __pmu_disabled_reset(bsp_pmu_object_t *self);
int __pmu_disabled_destroy(bsp_pmu_object_t *self);

int pmu_init(pmu_object_t *self, const char *ident, bool disabled, const char *type, ...);

static inline int pmu_start(pmu_object_t *self){
    return self->start(&(self->obj));
};

static inline int pmu_sample_and_stop(pmu_object_t *self){
    return self->sample_and_stop(&(self->obj));
};

static inline int pmu_accumulate(pmu_object_t *self){
    return self->accumulate(&(self->obj));
};

static inline int pmu_display(pmu_object_t *self){
    return self->display(&(self->obj));
};

static inline int pmu_reset(pmu_object_t *self){
    return self->reset(&(self->obj));
};

static inline int pmu_destroy(pmu_object_t *self){
    return self->destroy(&(self->obj));
};

#ifdef __cplusplus
}
#endif

#endif /* __PMU_H__ */
