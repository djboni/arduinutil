/*
 Arduinutil Queue - Queue implementation in C

 Supported microcontrollers:
 See Arduinutil.h


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

void Queue_init(struct Queue_t *o, uint8_t *buff, Size_t size)
{
    o->Buff = buff;
    o->Size = size;
    o->Used = 0U;
    o->Head = 0U;
    o->Tail = 0U;
}

uint8_t Queue_pushfront(struct Queue_t *o, uint8_t val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = Queue_free(o) != 0U;
        if(ret != 0U)
        {
            ++(o->Used);
            if(--(o->Head) >= o->Size)
                o->Head = o->Size - 1U;
            o->Buff[o->Head] = val;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

uint8_t Queue_pushback(struct Queue_t *o, uint8_t val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = Queue_free(o) != 0U;
        if(ret != 0U)
        {
            ++(o->Used);
            o->Buff[o->Tail] = val;
            if(++(o->Tail) >= o->Size)
                o->Tail = 0U;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

uint8_t Queue_popfront(struct Queue_t *o, uint8_t *val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = Queue_used(o) != 0U;
        if(ret != 0U)
        {
            --(o->Used);
            *val = o->Buff[o->Head];
            if(++(o->Head) >= o->Size)
                o->Head = 0U;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

uint8_t Queue_popback(struct Queue_t *o, uint8_t *val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = Queue_used(o) != 0U;
        if(ret != 0U)
        {
            --(o->Used);
            if(--(o->Tail) >= o->Size)
                o->Tail = o->Size - 1U;
            *val = o->Buff[o->Tail];
        }
    }
    EXIT_CRITICAL();
    return ret;
}

Size_t Queue_size(const struct Queue_t *o)
{
    return o->Size;
}

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

Size_t Queue_free(const struct Queue_t *o)
{
    return Queue_size(o) - Queue_used(o);
}
