/*
Arduinutil - Arduino compatible library written in C/C++

Supported microcontrollers:
    See Arduinutil.h


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

#include "Arduinutil.h"

#define SET_BITS(reg, bits, cast)   do{reg = (cast)( (reg) | (bits) );   }while(0)
#define CLEAR_BITS(reg, bits, cast) do{reg = (cast)( (reg) & (~(bits)) );}while(0)



#include <avr/io.h>



static volatile byte *conv_io_to_port(byte io)
{
    switch(io)
    {
        case 0:    return &PORTE;
        case 1:    return &PORTE;
        case 2:    return &PORTE;
        case 3:    return &PORTE;
        case 4:    return &PORTG;
        case 5:    return &PORTE;
        case 6:    return &PORTH;
        case 7:    return &PORTH;
        case 8:    return &PORTH;
        case 9:    return &PORTH;
        case 10:   return &PORTB;
        case 11:   return &PORTB;
        case 12:   return &PORTB;
        case 13:   return &PORTB;

        case 14:   return &PORTJ;
        case 15:   return &PORTJ;
        case 16:   return &PORTH;
        case 17:   return &PORTH;
        case 18:   return &PORTD;
        case 19:   return &PORTD;
        case 20:   return &PORTD;
        case 21:   return &PORTD;

        case 22:   return &PORTA;
        case 23:   return &PORTA;
        case 24:   return &PORTA;
        case 25:   return &PORTA;
        case 26:   return &PORTA;
        case 27:   return &PORTA;
        case 28:   return &PORTA;
        case 29:   return &PORTA;
        case 30:   return &PORTC;
        case 31:   return &PORTC;
        case 32:   return &PORTC;
        case 33:   return &PORTC;
        case 34:   return &PORTC;
        case 35:   return &PORTC;
        case 36:   return &PORTC;
        case 37:   return &PORTC;

        case 38:   return &PORTD;
        case 39:   return &PORTG;
        case 40:   return &PORTG;
        case 41:   return &PORTG;
        case 42:   return &PORTL;
        case 43:   return &PORTL;
        case 44:   return &PORTL;
        case 45:   return &PORTL;
        case 46:   return &PORTL;
        case 47:   return &PORTL;
        case 48:   return &PORTL;
        case 49:   return &PORTL;

        case 50:   return &PORTB;
        case 51:   return &PORTB;
        case 52:   return &PORTB;
        case 53:   return &PORTB;

        case 100:  return &PORTF;
        case 101:  return &PORTF;
        case 102:  return &PORTF;
        case 103:  return &PORTF;
        case 104:  return &PORTF;
        case 105:  return &PORTF;
        case 106:  return &PORTF;
        case 107:  return &PORTF;
        case 108:  return &PORTK;
        case 109:  return &PORTK;
        case 110:  return &PORTK;
        case 111:  return &PORTK;
        case 112:  return &PORTK;
        case 113:  return &PORTK;
        case 114:  return &PORTK;
        case 115:  return &PORTK;

        default:   ASSERT(0);
                   return NULL;
    }
}

static byte conv_io_to_bit(byte io)
{
    switch(io)
    {
        case 0:    return 0;
        case 1:    return 1;
        case 2:    return 4;
        case 3:    return 5;
        case 4:    return 5;
        case 5:    return 3;
        case 6:    return 3;
        case 7:    return 4;
        case 8:    return 5;
        case 9:    return 6;
        case 10:   return 4;
        case 11:   return 5;
        case 12:   return 6;
        case 13:   return 7;

        case 14:   return 1;
        case 15:   return 0;
        case 16:   return 1;
        case 17:   return 0;
        case 18:   return 3;
        case 19:   return 2;
        case 20:   return 1;
        case 21:   return 0;

        case 22:   return 0;
        case 23:   return 1;
        case 24:   return 2;
        case 25:   return 3;
        case 26:   return 4;
        case 27:   return 5;
        case 28:   return 6;
        case 29:   return 7;
        case 30:   return 7;
        case 31:   return 6;
        case 32:   return 5;
        case 33:   return 4;
        case 34:   return 3;
        case 35:   return 2;
        case 36:   return 1;
        case 37:   return 0;

        case 38:   return 7;
        case 39:   return 2;
        case 40:   return 1;
        case 41:   return 0;
        case 42:   return 7;
        case 43:   return 6;
        case 44:   return 5;
        case 45:   return 4;
        case 46:   return 3;
        case 47:   return 2;
        case 48:   return 1;
        case 49:   return 0;

        case 50:   return 3;
        case 51:   return 2;
        case 52:   return 1;
        case 53:   return 0;

        case 100:  return 0;
        case 101:  return 1;
        case 102:  return 2;
        case 103:  return 3;
        case 104:  return 4;
        case 105:  return 5;
        case 106:  return 6;
        case 107:  return 7;
        case 108:  return 0;
        case 109:  return 1;
        case 110:  return 2;
        case 111:  return 3;
        case 112:  return 4;
        case 113:  return 5;
        case 114:  return 6;
        case 115:  return 7;

        default:   ASSERT(0);
                   return 0;
    }
}

static volatile byte *conv_port_to_pin(volatile byte *port)
{
    return port - 2U;
}

static volatile byte *conv_port_to_ddr(volatile byte *port)
{
    return port - 1U;
}



void pinMode(byte io, byte mode)
{
    volatile byte *port, *ddr;
    byte bit;

    port = conv_io_to_port(io);
    ddr = conv_port_to_ddr(port);
    bit = conv_io_to_bit(io);

    switch(mode)
    {
        case INPUT:
            /* Input. */
            CLEAR_BITS(*ddr, (1U << bit), byte);
            /* Disable pull-up. */
            CLEAR_BITS(*port, (1U << bit), byte);
            break;
        case OUTPUT:
            /* Output. */
            SET_BITS(*ddr, (1U << bit), byte);
            break;
        case INPUT_PULLUP:
            /* Input. */
            CLEAR_BITS(*ddr, (1U << bit), byte);
            /* Disable pull-up. */
            SET_BITS(*port, (1U << bit), byte);
            break;
    }
}

void digitalWrite(byte io, byte value)
{
    volatile byte *port;
    byte bit;

    port = conv_io_to_port(io);
    bit = conv_io_to_bit(io);

    switch(value)
    {
        case LOW:
            CLEAR_BITS(*port, (1U << bit), byte);
            break;
        case HIGH:
            SET_BITS(*port, (1U << bit), byte);
            break;
    }
}

byte digitalRead(byte io)
{
    volatile byte *port;
    byte bit;

    port = conv_io_to_port(io);
    bit = conv_io_to_bit(io);

    return ( *conv_port_to_pin(port) & (1U << bit) ) != 0U;
}
