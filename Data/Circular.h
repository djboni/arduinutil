/*
Arduinutil Circular - Circular list implementation in C++


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

#ifndef __ARDUINUTIL_CIRCULAR_H__
#define __ARDUINUTIL_CIRCULAR_H__

#include "Config.h"
#include "Vector.h"

#ifdef __cplusplus
namespace Arduinutil {

template<class T, Size_t Size, bool Assert=true, bool Concurrent=false>
class Circular
{
public:

    /** Default constructor. */
    Circular():
            Buff(), Head(0), Tail(0), NumUsed(0)
    {
    }

    /** Return true if the queue is full, false otherwise. */
    inline bool full() const
    {
        return numFree() == 0;
    }

    /** Return true if the queue is empty, false otherwise. */
    inline bool empty() const
    {
        return numUsed() == 0;
    }

    /** Return the capacity (total number of positions) of the list. */
    inline Size_t size() const
    {
        Size_t ret;

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            ret = Size;
        }
        EXIT_CRITICAL_IF_CONCURRENT();

        return ret;
    }

    /** Return the number of used positions of the list. */
    inline Size_t numUsed() const
    {
        Size_t ret;

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            ret = NumUsed;
        }
        EXIT_CRITICAL_IF_CONCURRENT();

        return ret;
    }

    /** Return the number of free positions of the list. */
    inline Size_t numFree() const
    {
        Size_t ret;

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            ret = size() - numUsed();
        }
        EXIT_CRITICAL_IF_CONCURRENT();

        return ret;
    }

    /** Insert val in the front of the list. */
    inline void push_front(const T& val)
    {
        if(Assert)
            ASSERT(numUsed() < Size);

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            ++NumUsed;
            Head = Head == 0 ? Size-1 : Head-1;
            Buff[Head] = val;
        }
        EXIT_CRITICAL_IF_CONCURRENT();
    }

    /** Insert val in the back of the list. */
    inline void push_back(const T& val)
    {
        if(Assert)
            ASSERT(numUsed() < Size);

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            ++NumUsed;
            Buff[Tail] = val;
            Tail = Tail == Size-1 ? 0 : Tail+1;
        }
        EXIT_CRITICAL_IF_CONCURRENT();
    }

    /** Remove the value in the front of the list. */
    inline void pop_front(T& val)
    {
        if(Assert)
            ASSERT(numUsed() > 0);

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            --NumUsed;
            val = Buff[Head];
            Head = Head == Size-1 ? 0 : Head+1;
        }
        EXIT_CRITICAL_IF_CONCURRENT();
    }

    /** Remove the value in the back of the list. */
    inline void pop_back(T& val)
    {
        if(Assert)
            ASSERT(numUsed() > 0);

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            --NumUsed;
            Tail = Tail == 0 ? Size-1 : Tail-1;
            val = Buff[Tail];
        }
        EXIT_CRITICAL_IF_CONCURRENT();
    }

    /** Get the value in the front of the list. */
    inline void front(T& val)
    {
        if(Assert)
            ASSERT(numUsed() > 0);

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            val = Buff[Head];
        }
        EXIT_CRITICAL_IF_CONCURRENT();
    }

    /** Get the value in the back of the list. */
    inline void back(T& val)
    {
        if(Assert)
            ASSERT(numUsed() > 0);

        ENTER_CRITICAL_IF_CONCURRENT();
        {
            val = Buff[Tail == 0 ? Size-1 : Tail-1];
        }
        EXIT_CRITICAL_IF_CONCURRENT();
    }

private:
    Vector<T, Size, false> Buff;
    Size_t Head, Tail, NumUsed;

    /* Should not use these */
    Circular(const Circular& o);
    void operator=(const Circular& o);
};

} /* namespace Arduinutil */
#endif /* __cplusplus */

#endif /* __ARDUINUTIL_CIRCULAR_H__ */
