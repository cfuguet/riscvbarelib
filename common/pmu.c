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
 *  @file   common/pmu.c
 *  @author Eric Guthmuller
 */
#include "common/pmu.h"
#include <string.h>

// Default API in case PMU is disabled
int __pmu_disabled_start(bsp_pmu_object_t *self){return 0;};
int __pmu_disabled_sample_and_stop(bsp_pmu_object_t *self){return 0;};
int __pmu_disabled_accumulate(bsp_pmu_object_t *self){return 0;};
int __pmu_disabled_display(bsp_pmu_object_t *self){return 0;};
int __pmu_disabled_reset(bsp_pmu_object_t *self){return 0;};
int __pmu_disabled_destroy(bsp_pmu_object_t *self){return 0;};

int pmu_init(pmu_object_t *self, const char *ident, bool disabled, const char *type, ...) {
    int err = 0;
    if(disabled) {
            self->start           = &__pmu_disabled_start;
            self->sample_and_stop = &__pmu_disabled_sample_and_stop;
            self->accumulate      = &__pmu_disabled_accumulate;
            self->display         = &__pmu_disabled_display;
            self->reset           = &__pmu_disabled_reset;
            self->destroy         = &__pmu_disabled_destroy;
    } else {
        va_list args;
        va_start(args, type);
        err = bsp_pmu_init(&(self->obj),
                           ident,
                           type,
                           &(self->start),
                           &(self->sample_and_stop),
                           &(self->accumulate),
                           &(self->display), 
                           &(self->reset),
                           &(self->destroy),
                           args);
        va_end(args);
    }
    if (err < 0) {
        printf("ERROR %d (%s) while initializing PMU %s / Disabling PMU\n",-err,strerror(-err),ident);
        self->start           = &__pmu_disabled_start;
        self->sample_and_stop = &__pmu_disabled_sample_and_stop;
        self->accumulate      = &__pmu_disabled_accumulate;
        self->display         = &__pmu_disabled_display;
        self->reset           = &__pmu_disabled_reset;
        self->destroy         = &__pmu_disabled_destroy; 
    }
    return err;
}
