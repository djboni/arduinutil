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
    inline Size_t size() const;
    inline T& operator[](Size_t pos);

private:
    T Buff[Size]; /* Data storage */

    /* Should not use these */
    Vector(const Vector& o);
    void operator=(const Vector& o);
};

/** Default constructor. */
template<class T, Size_t Size>
Vector<T, Size>::Vector() :
        Buff()
{
}

/** Return the capacity (total number of positions) of the vector. */
template<class T, Size_t Size>
Size_t Vector<T, Size>::size() const
{
    return Size;
}

/** Return a reference to the position pos of the vector (pos must be in the
range 0 to Size-1). */
template<class T, Size_t Size>
T& Vector<T, Size>::operator[](Size_t pos)
{
    ASSERT(pos < Size);
    return Buff[pos];
}

} /* namespace Arduinutil */
#endif /* __cplusplus */

#endif /* __ARDUINUTIL_VECTOR_H__ */
