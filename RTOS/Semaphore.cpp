/*
 Arduinutil Semaphores and Mutex - Semaphore, Counting Semaphore and Mutex
 implementation in C++


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

#include "Semaphore.h"

namespace Arduinutil
{

/** Default constructor. The semaphore is initialized taken. */
Semaphore::Semaphore() :
        Count(0)
{
}

/** Check if a semaphore is unlocked/given without modifying it. Return true if
 the semaphore is unlocked, false otherwise. */
inline bool Semaphore::check()
{
    return Count;
}

/** Unlock/give a semaphore. Return true if the semaphore could be unlocked,
 false otherwise. */
bool Semaphore::give()
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = Count == 0;
        if(ret)
        {
            Count = 1;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Lock/take a semaphore. Return true if the semaphore could be locked, false
 otherwise. */
bool Semaphore::take()
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = Count != 0;
        if(ret)
        {
            Count = 0;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Maximum count value constructor. The semaphore is initialized taken.
 max == 0  The semaphore will always be taken and cannot be given.
 max == 1  Equivalent to a binary semaphore (use Semaphore class instead).
 max >= 2  Counting semaphore. Can be taken/given successfully up to max
 consecutive times. */
SemaphoreCounter::SemaphoreCounter(Size_t max) :
        Semaphore(), MaxCount(max)
{
}

/** Check if a semaphore is unlocked/given without modifying it. Return the
 number of counts that the semaphore can be locked, 0 if cannot be locked. */
inline Size_t SemaphoreCounter::check()
{
    return Count;
}

/** Unlock/give a semaphore. Return true if the semaphore could be unlocked,
 false otherwise. */
bool SemaphoreCounter::give()
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = Count < MaxCount;
        if(ret)
        {
            ++Count;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Lock/take a semaphore. Return true if the semaphore could be locked, false
 otherwise. */
bool SemaphoreCounter::take()
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = Count > 0;
        if(ret)
        {
            --Count;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Default constructor. Wrapper for Semaphore class, for meaningful naming. The
 mutex is initialized unlocked (given). */
inline Mutex::Mutex() :
        Semaphore()
{
    Semaphore::give();
}

/** Check if a mutex is unlocked/given without modifying it. Return true if the
 mutex is unlocked, false otherwise. */
inline bool Mutex::check()
{
    return Semaphore::check();
}

/** Unlock/give a mutex. Return true if the mutex could be unlocked, false
 otherwise. */
inline bool Mutex::unlock()
{
    return Semaphore::give();
}

/** Lock/take a mutex. Return true if the mutex could be locked, false
 otherwise. */
inline bool Mutex::lock()
{
    return Semaphore::take();
}

} /* namespace Arduinutil */
