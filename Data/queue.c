/*
 Arduinutil Queue - Queue implementation in C


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

#include "Data/queue.h"
#include <string.h>

#if (QUEUE_ENABLE != 0)

/** Initialize queue struct.
 *
 * Note: Not thread-safe.
 *
 * @param o Pointer to queue.
 * @param buff Pointer to data buffer (must be length*item_size bytes long).
 * @param length Number of items the queue can hold.
 * @param item_size Number of bytes per item.
 */
void Queue_init(struct Queue_t *o, void *buff, Size_t length, Size_t item_size)
{
    uint8_t *buff8 = (uint8_t*)buff;

    o->ItemSize = item_size;
    o->Free = length;
    o->Used = 0U;
    o->WLock = 0U;
    o->RLock = 0U;
    o->Head = buff8;
    o->Tail = buff8;
    o->Buff = buff8;
    o->BufEnd = &buff8[(length - 1U) * item_size];
}

/** Insert item in the front of the queue.
 *
 * @param o Pointer to queue.
 * @param val Pointer to item.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Queue_pushfront(struct Queue_t *o, const void *val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Free != 0U;
        if(ret != 0U)
        {
            Size_t lock;
            uint8_t *pos;

            lock = (o->WLock)++;
            --(o->Free);

            if((o->Head -= o->ItemSize) < o->Buff)
                o->Head = o->BufEnd;
            pos = o->Head;

            EXIT_CRITICAL();
            {
                memcpy(pos, val, o->ItemSize);
            }
            ENTER_CRITICAL();

            if(lock == 0U)
            {
                o->Used += o->WLock;
                o->WLock = 0;
            }
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Insert item in the back of the queue.
 *
 * @param o Pointer to queue.
 * @param val Pointer to item.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Queue_pushback(struct Queue_t *o, const void *val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Free != 0U;
        if(ret != 0U)
        {
            Size_t lock;
            uint8_t *pos;

            lock = (o->WLock)++;
            --(o->Free);

            pos = o->Tail;
            if((o->Tail += o->ItemSize) > o->BufEnd)
                o->Tail = o->Buff;

            EXIT_CRITICAL();
            {
                memcpy(pos, val, o->ItemSize);
            }
            ENTER_CRITICAL();

            if(lock == 0U)
            {
                o->Used += o->WLock;
                o->WLock = 0;
            }
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Remove item in the front of the queue.
 *
 * @param o Pointer to queue.
 * @param val Pointer to item.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Queue_popfront(struct Queue_t *o, void *val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Used != 0U;
        if(ret != 0U)
        {
            Size_t lock;
            uint8_t *pos;

            lock = (o->RLock)++;
            --(o->Used);

            pos = o->Head;
            if((o->Head += o->ItemSize) > o->BufEnd)
                o->Head = o->Buff;

            EXIT_CRITICAL();
            {
                memcpy(val, pos, o->ItemSize);
            }
            ENTER_CRITICAL();

            if(lock == 0U)
            {
                o->Free += o->RLock;
                o->RLock = 0;
            }
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Remove item in the back of the queue.
 *
 * @param o Pointer to queue.
 * @param val Pointer to item.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Queue_popback(struct Queue_t *o, void *val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Used != 0U;
        if(ret != 0U)
        {
            Size_t lock;
            uint8_t *pos;

            lock = (o->RLock)++;
            --(o->Used);

            if((o->Tail -= o->ItemSize) < o->Buff)
                o->Tail = o->BufEnd;
            pos = o->Tail;

            EXIT_CRITICAL();
            {
                memcpy(val, pos, o->ItemSize);
            }
            ENTER_CRITICAL();

            if(lock == 0U)
            {
                o->Free += o->RLock;
                o->RLock = 0;
            }
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Get the queue length.
 *
 * @param o Pointer to queue.
 * @return Length of the queue.
 */
Size_t Queue_length(const struct Queue_t *o)
{
    Size_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Used + o->Free + o->RLock + o->WLock;
    }
    EXIT_CRITICAL();
    return ret;
}

/** Get the number of used positions of the queue.
 *
 * @param o Pointer to queue.
 * @return Number of used positions of the queue.
 */
Size_t Queue_used(const struct Queue_t *o)
{
    Size_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Used;
    }
    EXIT_CRITICAL();
    return ret;
}

/** Get the number of free positions of the queue.
 *
 * @param o Pointer to queue.
 * @return Number of free positions of the queue.
 */
Size_t Queue_free(const struct Queue_t *o)
{
    Size_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Free;
    }
    EXIT_CRITICAL();
    return ret;
}

/** Clear the queue, freeing all positions.
 *
 * @param o Pointer to queue.
 */
void Queue_clear(struct Queue_t *o)
{
    ENTER_CRITICAL();
    {
        o->Free = Queue_length(o);
        o->Used = 0U;
        o->WLock = 0U;
        o->RLock = 0U;
        o->Head = o->Buff;
        o->Tail = o->Buff;
    }
    EXIT_CRITICAL();
}

#endif /* QUEUE_ENABLE */
