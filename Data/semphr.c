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

#include "Data/semphr.h"
#include <string.h>

#if (SEMAPHORE_ENABLE != 0)

#define UNCONST(type, var) *((type*)&(var))

/** Initialize semaphore struct as a binary semaphore.
 *
 * Note: Not thread-safe.
 *
 * @param o Pointer to semaphore.
 */
void Semaphore_initbinary(struct Semaphore_t *o)
{
    Semaphore_init(o, 1U);
}

/** Initialize semaphore struct as a counting semaphore.
 *
 * Note: Not thread-safe.
 *
 * @param o Pointer to semaphore.
 * @param max Maximum number of counts of the semaphore.
 */
void Semaphore_init(struct Semaphore_t *o, Size_t max)
{
    o->Count = 0U;
    UNCONST(Size_t, o->Max) = max;
}

/** Lock semaphore.
 *
 * @param o Pointer to semaphore.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Semaphore_lock(struct Semaphore_t *o)
{
    uint8_t ret;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        ret = o->Count != 0U;
        if(ret)
        {
            --(o->Count);
        }
    }
    CRITICAL_EXIT();
    return ret;
}

/** Unlock semaphore.
 *
 * @param o Pointer to semaphore.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Semaphore_unlock(struct Semaphore_t *o)
{
    uint8_t ret;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        ret = o->Count < o->Max;
        if(ret)
        {
            ++(o->Count);
        }
    }
    CRITICAL_EXIT();
    return ret;
}

/** Get semaphore count value.
 *
 * @param o Pointer to semaphore.
 * @return Counter value of the semaphore.
 */
Size_t Semaphore_getcount(const struct Semaphore_t *o)
{
    Size_t ret;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        ret = o->Count;
    }
    CRITICAL_EXIT();
    return ret;
}

/** Get semaphore maximum count value.
 *
 * Note: Not thread-safe.
 * Max should not be changed after initialization.
 *
 * @param o Pointer to semaphore.
 * @return Maximum counter value of the semaphore.
 */
Size_t Semaphore_getmax(const struct Semaphore_t *o)
{
    return o->Max;
}

#endif /* SEMAPHORE_ENABLE */
