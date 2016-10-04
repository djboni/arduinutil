/*
Arduinutil Hexdump - Dumps data in hexadecimal format.


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

#include "hexdump.h"

#ifndef hexdump_write
#define hexdump_write Serial_write
#endif

#ifndef hexdump_print
#define hexdump_print Serial_print
#endif

extern int hexdump_write(const void *str);
extern int hexdump_print(const char *format, ...);

void hexdump(const void *vdata, uint16_t len, uint8_t bytes_per_line,
        uint8_t print_line_number)
{
    const uint8_t *data = (const uint8_t*)vdata;
    uint8_t j = 0U;
    uint16_t i;
    for (i = 0U; i < len; ++i)
    {
        if(print_line_number != 0U && j == 0U)
        {
            hexdump_print("%04X", i);
            hexdump_write(" ");
        }

        hexdump_print("%02X", data[i]);

        if (++j < bytes_per_line)
        {
            hexdump_write(" ");
        }
        else
        {
            hexdump_write("\n");
            j = 0U;
        }
    }
    if (j != 0U)
    {
        hexdump_write("\n");
    }
}
