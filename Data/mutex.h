/*
 Arduinutil Mutex - Mutex implementation in C


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

#ifndef __ARDUINUTIL_MUTEX_H__
#define __ARDUINUTIL_MUTEX_H__

#include "Arduinutil.h"
#include "Data/semphr.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (MUTEX_ENABLE != 0)

#define MUTEX_INIT {{1U, 1U}}

struct Mutex_t {
    struct Semaphore_t sem;
};

void Mutex_init(struct Mutex_t *o);
uint8_t Mutex_lock(struct Mutex_t *o);
uint8_t Mutex_unlock(struct Mutex_t *o);

#endif /* MUTEX_ENABLE */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_MUTEX_H__ */
