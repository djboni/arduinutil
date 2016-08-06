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
#include <string.h>

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

 The call conv_ul2str(str, size, val, 10) produces the same string as
 snprintf(str, size, "%lu", val). However returns a pointer to the start of the
 converted string and the end of the string will be aligned with the last byte
 of str.

 Examples:
 hexadecimal conv_ul2str(str, size, val, 16)
 decimal     conv_ul2str(str, size, val, 10)
 octal       conv_ul2str(str, size, val, 8)
 binary      conv_ul2str(str, size, val, 2)

 As an example the following code will result in ptr pointing to str[6] and
 printing the string "3FF\0", with str beign equal to "??????3FF\0".

 char str[10], *ptr;
 ptr = conv_ul2str(str, 10, 0x3FF, 16);
 puts(ptr);

 @param str Buffer that will hold the number in printable format. Must be at
 least 2 bytes long to hold the string "0\0".
 @param size The size in bytes of the string str.
 @param value The value to be converted.
 @pram base Base for conversion. May be 2, 8, 10, 16 or any number among them.
 @return Pointer to the start of the converted string (in the rage &str[0]
 &str[size-2]).
 */
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

/** Fill bytes before a printable string with a character.

 As an example the following code will result in ptr pointing to str[5] and
 printing the string "03FF\0", with str beign equal to "?????03FF\0".

 char str[10], *ptr;
 ptr = conv_ul2str(str, 10, 0x3FF, 16);
 ptr = conv_fillstr(ptr, 4, '0');
 puts(ptr);

 @param str Null terminated string. Must point to the start of a printable
 string and must have at least num-strlen(str) bytes before where it points.
 This means that the string used with conv_ul2str() must be at least num+1 bytes
 long.
 @param num Maximum number of characters to fill with val.
 @param val Character to fill the string with.
 */
char *conv_fillstr(char *str, uint8_t num, char ch)
{
    uint8_t used = strlen(str);

    if(used >= num)
        return str;

    num -= used;
    while(num != 0U)
    {
        *(--str) = ch;
        --num;
    }

    return str;
}
