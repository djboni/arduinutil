/*
Arduinutil ConvIntStr - Conversions from integer to printable strings


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

#include "convintstr.h"

/** Convert a digit 0-15 to a char 0-9,A-F. May be used for binary, octal,
 decimal, hexadecimal. */
char conv_digit2char(uint8_t digit)
{
    if(digit < 10U)
        return digit +  '0';
    else if(digit < 16U)
        return digit + ('A' - 10U);
    return '?';
}

/** Convert a number to a printable string.

 str must be at least 2 bytes long.
 size is the size of str.
 value is the number to be converted.
 base can be any base from 2 to 16. */
char *conv_ul2str(char *str, uint8_t size, uint32_t val, uint8_t base)
{
    str[size - 1U] = '\0';
    str[size - 2U] = conv_digit2char(0);

    str += (size - 2U);
    size -= 1U;
    if(val == 0U)
        return str;

    while(val != 0U && size != 0U)
    {
        uint8_t digit = val % base;
        val /= base;

        *str = conv_digit2char(digit);

        --str;
        --size;
    }

    return str + 1U;
}
