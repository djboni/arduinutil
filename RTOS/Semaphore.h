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

#ifndef __ARDUINUTIL_SEMAPHORE_H__
#define __ARDUINUTIL_SEMAPHORE_H__

#include "Config.h"

#ifdef __cplusplus
namespace Arduinutil {

class Semaphore
{
public:
    Semaphore();

    inline bool check();

    bool give();
    bool take();

protected:
    Size_t Count;

private:
    /* Should not use these */
    Semaphore(const Semaphore& o);
    void operator=(const Semaphore& o);
};

class SemaphoreCounter: protected Semaphore
{
public:
    SemaphoreCounter(Size_t max);

    inline Size_t check();

    bool give();
    bool take();

protected:
    const Size_t MaxCount;

private:
    /* Should not use these */
    SemaphoreCounter();
    SemaphoreCounter(const SemaphoreCounter& o);
    void operator=(const SemaphoreCounter& o);
};

class Mutex: protected Semaphore
{
public:
    inline Mutex();

    inline bool check();

    inline bool lock();
    inline bool unlock();

private:
    /* Should not use these */
    Mutex(const Mutex& o);
    void operator=(const Mutex& o);
};

} /* namespace Arduinutil */
#endif /* __cplusplus */

#endif /* __ARDUINUTIL_SEMAPHORE_H__ */
