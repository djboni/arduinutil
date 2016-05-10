/*
Arduinutil Queue - Queue implementation in C++


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

#ifndef __ARDUINUTIL_QUEUE_H__
#define __ARDUINUTIL_QUEUE_H__

#include "Config.h"

#ifdef __cplusplus
namespace Arduinutil {

template<class T, Size_t Size>
class Queue
{
public:
    Queue();

    inline bool full() const;
    inline bool empty() const;
    inline Size_t numFree() const;
    inline Size_t numUsed() const;
    inline Size_t size() const;

    bool front(T& val);
    bool back(T& val);

    bool push_back(const T& val);
    bool push_front(const T& val);

    bool pop_back(T& val);
    bool pop_front(T& val);

private:
    T Buff[Size]; /* Data storage */
    Size_t PosFront; /* First used position */
    Size_t PosBack; /* First unused position */
    Size_t NumUsed; /* Number of used positions */

    /* Should not use these */
    Queue(const Queue& o);
    void operator=(const Queue& o);
};

/** Default constructor. */
template<class T, Size_t Size>
Queue<T, Size>::Queue() :
        Buff(), PosFront(0), PosBack(0), NumUsed(0)
{
}

/** Return true if the queue is full, false otherwise. */
template<class T, Size_t Size>
bool Queue<T, Size>::full() const
{
    return NumUsed == Size;
}

/** Return true if the queue is empty, false otherwise. */
template<class T, Size_t Size>
bool Queue<T, Size>::empty() const
{
    return NumUsed == 0;
}

/** Return the number of free positions in the queue. */
template<class T, Size_t Size>
Size_t Queue<T, Size>::numFree() const
{
    return Size - NumUsed;
}

/** Return the number of used positions in the queue. */
template<class T, Size_t Size>
Size_t Queue<T, Size>::numUsed() const
{
    return NumUsed;
}

/** Return the total number of positions in the queue. */
template<class T, Size_t Size>
Size_t Queue<T, Size>::size() const
{
    return Size;
}

/** Copy the front of the queue to val, without removing it from the queue. On
success returns true, false otherwise (queue empty). */
template<class T, Size_t Size>
bool Queue<T, Size>::front(T& val)
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = !empty();
        if (ret)
        {
            val = Buff[PosFront];
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Copy the back of the queue to val, without removing it from the queue. On
success returns true, false otherwise (queue empty). */
template<class T, Size_t Size>
bool Queue<T, Size>::back(T& val)
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = !empty();
        if (ret)
        {
            val = Buff[PosBack == 0 ? Size - 1 : PosBack - 1];
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Insert val in the back of the queue. On success returns true, false
otherwise (queue full). */
template<class T, Size_t Size>
bool Queue<T, Size>::push_back(const T& val)
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = !full();
        if (ret)
        {
            Buff[PosBack++] = val;
            if (PosBack >= Size)
                PosBack = 0;
            ++NumUsed;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Insert val in the front of the queue. On success returns true, false
otherwise (queue full). */
template<class T, Size_t Size>
bool Queue<T, Size>::push_front(const T& val)
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = !full();
        if (ret)
        {
            if (PosFront <= 0)
                PosFront = Size;
            Buff[--PosFront] = val;
            ++NumUsed;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Remove the back of the queue and copy it to val. On success returns true,
false otherwise (queue empty). */
template<class T, Size_t Size>
bool Queue<T, Size>::pop_back(T& val)
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = !empty();
        if (ret)
        {
            if (PosBack <= 0)
                PosBack = Size;
            val = Buff[--PosBack];
            --NumUsed;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Remove the front of the queue and copy it to val. On success returns true,
false otherwise (queue empty). */
template<class T, Size_t Size>
bool Queue<T, Size>::pop_front(T& val)
{
    bool ret;
    ENTER_CRITICAL();
    {
        ret = !empty();
        if (ret)
        {
            val = Buff[PosFront++];
            if (PosFront >= Size)
                PosFront = 0;
            --NumUsed;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

} /* namespace Arduinutil */
#endif /* __cplusplus */

#endif /* __ARDUINUTIL_QUEUE_H__ */
