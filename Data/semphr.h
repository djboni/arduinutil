/*
 Arduinutil Semaphore - Semaphore implementation in C


 Copyright 2016 Djones A. Boni

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef __ARDUINUTIL_SEMPHR_H__
#define __ARDUINUTIL_SEMPHR_H__

#include "Arduinutil.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (SEMAPHORE_ENABLE != 0)

#define SEMAPHORE_INIT_BINARY       {0U, 1U}
#define SEMAPHORE_INIT_COUNTER(max) {0U, (max)}

struct Semaphore_t {
    volatile Size_t Count;
    const Size_t Max;
};

void Semaphore_initbinary(struct Semaphore_t *o);
void Semaphore_init(struct Semaphore_t *o, Size_t max);
uint8_t Semaphore_lock(struct Semaphore_t *o);
uint8_t Semaphore_unlock(struct Semaphore_t *o);
Size_t Semaphore_getcount(const struct Semaphore_t *o);
Size_t Semaphore_getmax(const struct Semaphore_t *o);

#endif /* SEMAPHORE_ENABLE */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_SEMPHR_H__ */
