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

#define SET_BITS(reg, bits, cast)   do{reg = (cast)( (reg) | (bits) );   }while(0U)
#define CLEAR_BITS(reg, bits, cast) do{reg = (cast)( (reg) & (~(bits)) );}while(0U)



#include <avr/io.h>



static volatile byte *conv_io_to_port(byte io)
{
    switch(io)
    {
        case 0U:    return &PORTE;
        case 1U:    return &PORTE;
        case 2U:    return &PORTE;
        case 3U:    return &PORTE;
        case 4U:    return &PORTG;
        case 5U:    return &PORTE;
        case 6U:    return &PORTH;
        case 7U:    return &PORTH;
        case 8U:    return &PORTH;
        case 9U:    return &PORTH;
        case 10U:   return &PORTB;
        case 11U:   return &PORTB;
        case 12U:   return &PORTB;
        case 13U:   return &PORTB;

        case 14U:   return &PORTJ;
        case 15U:   return &PORTJ;
        case 16U:   return &PORTH;
        case 17U:   return &PORTH;
        case 18U:   return &PORTD;
        case 19U:   return &PORTD;
        case 20U:   return &PORTD;
        case 21U:   return &PORTD;

        case 22U:   return &PORTA;
        case 23U:   return &PORTA;
        case 24U:   return &PORTA;
        case 25U:   return &PORTA;
        case 26U:   return &PORTA;
        case 27U:   return &PORTA;
        case 28U:   return &PORTA;
        case 29U:   return &PORTA;
        case 30U:   return &PORTC;
        case 31U:   return &PORTC;
        case 32U:   return &PORTC;
        case 33U:   return &PORTC;
        case 34U:   return &PORTC;
        case 35U:   return &PORTC;
        case 36U:   return &PORTC;
        case 37U:   return &PORTC;

        case 38U:   return &PORTD;
        case 39U:   return &PORTG;
        case 40U:   return &PORTG;
        case 41U:   return &PORTG;
        case 42U:   return &PORTL;
        case 43U:   return &PORTL;
        case 44U:   return &PORTL;
        case 45U:   return &PORTL;
        case 46U:   return &PORTL;
        case 47U:   return &PORTL;
        case 48U:   return &PORTL;
        case 49U:   return &PORTL;

        case 50U:   return &PORTB;
        case 51U:   return &PORTB;
        case 52U:   return &PORTB;
        case 53U:   return &PORTB;

        case 100U:  return &PORTF;
        case 101U:  return &PORTF;
        case 102U:  return &PORTF;
        case 103U:  return &PORTF;
        case 104U:  return &PORTF;
        case 105U:  return &PORTF;
        case 106U:  return &PORTF;
        case 107U:  return &PORTF;
        case 108U:  return &PORTK;
        case 109U:  return &PORTK;
        case 110U:  return &PORTK;
        case 111U:  return &PORTK;
        case 112U:  return &PORTK;
        case 113U:  return &PORTK;
        case 114U:  return &PORTK;
        case 115U:  return &PORTK;

        default:    ASSERT(0U);
                    return NULL;
    }
}

static byte conv_io_to_bit(byte io)
{
    switch(io)
    {
        case 0U:    return 0U;
        case 1U:    return 1U;
        case 2U:    return 4U;
        case 3U:    return 5U;
        case 4U:    return 5U;
        case 5U:    return 3U;
        case 6U:    return 3U;
        case 7U:    return 4U;
        case 8U:    return 5U;
        case 9U:    return 6U;
        case 10U:   return 4U;
        case 11U:   return 5U;
        case 12U:   return 6U;
        case 13U:   return 7U;

        case 14U:   return 1U;
        case 15U:   return 0U;
        case 16U:   return 1U;
        case 17U:   return 0U;
        case 18U:   return 3U;
        case 19U:   return 2U;
        case 20U:   return 1U;
        case 21U:   return 0U;

        case 22U:   return 0U;
        case 23U:   return 1U;
        case 24U:   return 2U;
        case 25U:   return 3U;
        case 26U:   return 4U;
        case 27U:   return 5U;
        case 28U:   return 6U;
        case 29U:   return 7U;
        case 30U:   return 7U;
        case 31U:   return 6U;
        case 32U:   return 5U;
        case 33U:   return 4U;
        case 34U:   return 3U;
        case 35U:   return 2U;
        case 36U:   return 1U;
        case 37U:   return 0U;

        case 38U:   return 7U;
        case 39U:   return 2U;
        case 40U:   return 1U;
        case 41U:   return 0U;
        case 42U:   return 7U;
        case 43U:   return 6U;
        case 44U:   return 5U;
        case 45U:   return 4U;
        case 46U:   return 3U;
        case 47U:   return 2U;
        case 48U:   return 1U;
        case 49U:   return 0U;

        case 50U:   return 3U;
        case 51U:   return 2U;
        case 52U:   return 1U;
        case 53U:   return 0U;

        case 100U:  return 0U;
        case 101U:  return 1U;
        case 102U:  return 2U;
        case 103U:  return 3U;
        case 104U:  return 4U;
        case 105U:  return 5U;
        case 106U:  return 6U;
        case 107U:  return 7U;
        case 108U:  return 0U;
        case 109U:  return 1U;
        case 110U:  return 2U;
        case 111U:  return 3U;
        case 112U:  return 4U;
        case 113U:  return 5U;
        case 114U:  return 6U;
        case 115U:  return 7U;

        default:    ASSERT(0U);
                    return 0U;
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
