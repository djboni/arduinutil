/*
Arduinutil Circular - Byte circular list implementation in C

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

#include "cCircular.h"

void cCircular_init(struct cCircular *o, uint8_t *buff, Size_t size)
{
    o->Buff = buff;
    o->Size = size;
    o->Used = 0U;
    o->Head = 0U;
    o->Tail = 0U;
}

uint8_t cCircular_pushfront(struct cCircular *o, uint8_t val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = cCircular_free(o) != 0U;
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

uint8_t cCircular_pushback(struct cCircular *o, uint8_t val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = cCircular_free(o) != 0U;
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

uint8_t cCircular_popfront(struct cCircular *o, uint8_t *val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = cCircular_used(o) != 0U;
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

uint8_t cCircular_popback(struct cCircular *o, uint8_t *val)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = cCircular_used(o) != 0U;
        if(ret != 0U)
        {
            --(o->Used);
            if(--(o->Tail) >= o->Size)
                o->Tail = o->Size-1U;
            *val = o->Buff[o->Tail];
        }
    }
    EXIT_CRITICAL();
    return ret;
}

Size_t cCircular_size(const struct cCircular *o)
{
    return o->Size;
}

Size_t cCircular_used(const struct cCircular *o)
{
    Size_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Used;
    }
    EXIT_CRITICAL();
    return ret;
}

Size_t cCircular_free(const struct cCircular *o)
{
    return cCircular_size(o) - cCircular_used(o);
}
