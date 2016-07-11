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

void Semaphore_initbinary(struct Semaphore_t *o)
{
	Semaphore_init(o, 1U);
}

void Semaphore_init(struct Semaphore_t *o, Size_t max)
{
	o->Count = 0U;
	o->Max = max;
}

uint8_t Semaphore_lock(struct Semaphore_t *o)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
    	ret = o->Count != 0U;
    	if(ret)
    	{
    		--(o->Count);
    	}
    }
    EXIT_CRITICAL();
    return ret;
}

uint8_t Semaphore_unlock(struct Semaphore_t *o)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
    	ret = o->Count < o->Max;
    	if(ret)
    	{
    		++(o->Count);
    	}
    }
    EXIT_CRITICAL();
    return ret;
}

Size_t Semaphore_getcount(const struct Semaphore_t *o)
{
    Size_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Count;
    }
    EXIT_CRITICAL();
    return ret;
}

Size_t Semaphore_getmax(const struct Semaphore_t *o)
{
    return o->Max;
}
