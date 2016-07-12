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

#include "Data/mutex.h"
#include <string.h>

#if (MUTEX_ENABLE != 0)

/** Initialize mutex struct.
 *
 * Note: Not thread-safe.
 *
 * @param o Pointer to mutex.
 */
void Mutex_init(struct Mutex_t *o)
{
    Semaphore_init(&o->sem, 1U);
    Semaphore_unlock(&o->sem);
}

/** Lock mutex.
 *
 * @param o Pointer to mutex.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Mutex_lock(struct Mutex_t *o)
{
    return Semaphore_lock(&o->sem);
}

/** Unlock mutex.
 *
 * @param o Pointer to mutex.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Mutex_unlock(struct Mutex_t *o)
{
    return Semaphore_unlock(&o->sem);
}

#endif /* MUTEX_ENABLE */
