/*
Arduinutil Vector - Vector implementation in C++


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

#ifndef __ARDUINUTIL_VECTOR_H__
#define __ARDUINUTIL_VECTOR_H__

#include "Config.h"

#ifdef __cplusplus
namespace Arduinutil {

template<class T, Size_t Size>
class Vector
{
public:
    Vector();

    inline bool full() const;
    inline bool empty() const;
    inline Size_t numUsed() const;
    inline Size_t size() const;

    bool front(T& val);
    bool back(T& val);

    bool push_back(const T& val);
    bool pop_back(T& val);

    T& operator[](Size_t pos);

private:
    Size_t NumUsed; /* Number of used positions */
    T Buff[Size]; /* Data storage */



    /* Should not use these */
    Vector(const Vector& o);
    void operator=(const Vector& o);
};

/** Default constructor. */
template<class T, Size_t Size>
Vector<T, Size>::Vector() :
        NumUsed(0U), Buff()
{
}

/** Return true if the vector is full, false otherwise. */
template<class T, Size_t Size>
bool Vector<T, Size>::full() const
{
    return NumUsed == Size;
}

/** Return true if the vector is empty, false otherwise. */
template<class T, Size_t Size>
bool Vector<T, Size>::empty() const
{
    return NumUsed == 0;
}

/** Return the number of used positions of the vector. */
template<class T, Size_t Size>
Size_t Vector<T, Size>::numUsed() const
{
    return NumUsed;
}

/** Return the capacity (total number of positions) of the vector. */
template<class T, Size_t Size>
Size_t Vector<T, Size>::size() const
{
    return Size;
}

/** Copy the front of the vector to val, without removing it. On success returns
true, false otherwise (vector empty). */
template<class T, Size_t Size>
bool Vector<T, Size>::front(T& val)
{
    if(NumUsed != 0)
    {
        val = Buff[0U];
        return true;
    }
    else
    {
        return false;
    }
}

/** Copy the back of the vector to val, without removing it. On success returns
true, false otherwise (vector empty). */
template<class T, Size_t Size>
bool Vector<T, Size>::back(T& val)
{
    if(NumUsed != 0)
    {
        val = Buff[NumUsed - 1U];
        return true;
    }
    else
    {
        return false;
    }
}

/** Insert val in the back of the vector. On success returns true, false
otherwise (vector at full capacity). */
template<class T, Size_t Size>
bool Vector<T, Size>::push_back(const T& val)
{
    if(NumUsed != Size)
    {
        Buff[NumUsed++] = val;
        return true;
    }
    else
    {
        return false;
    }
}

/** Remove the back of the vector and copy it to val. On success returns true,
false otherwise (vector empty). */
template<class T, Size_t Size>
bool Vector<T, Size>::pop_back(T& val)
{
    if(NumUsed != 0)
    {
        val = Buff[--NumUsed];
        return true;
    }
    else
    {
        return false;
    }
}

/** Return a reference to the position pos of the vector (pos must be in the
range 0 to Size-1). */
template<class T, Size_t Size>
T& Vector<T, Size>::operator[](Size_t pos)
{
    ASSERT(pos < Size);
    if(pos >= NumUsed)
        NumUsed = pos + 1U;
    return Buff[pos];
}

} /* namespace Arduinutil */
#endif /* __cplusplus */

#endif /* __ARDUINUTIL_VECTOR_H__ */
