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

#define SET_BITS(reg, bits, cast)   do{reg=(cast)((reg)|(bits));}while(0U)
#define CLEAR_BITS(reg, bits, cast) do{reg=(cast)((reg)&(~(bits)));}while(0U)

#include <avr/io.h>
#include <avr/pgmspace.h>

PROGMEM static volatile byte *const portIOLIST[] = {
    /* 0 - 13 */
    &PORTE, &PORTE, &PORTE, &PORTE, &PORTG, &PORTE, &PORTH, &PORTH, &PORTH,
    &PORTH, &PORTB, &PORTB, &PORTB, &PORTB,
    /* 14 - 21 */
    &PORTJ, &PORTJ, &PORTH, &PORTH, &PORTD, &PORTD, &PORTD, &PORTD,
    /* 22 - 37 */
    &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTC,
    &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC,
    /* 38 - 49 */
    &PORTD, &PORTG, &PORTG, &PORTG, &PORTL, &PORTL, &PORTL, &PORTL, &PORTL,
    &PORTL, &PORTL, &PORTL,
    /* 50 - 53 */
    &PORTB, &PORTB, &PORTB, &PORTB,
    /* A0 - A15 */
    &PORTF, &PORTF, &PORTF, &PORTF, &PORTF, &PORTF, &PORTF, &PORTF, &PORTK,
    &PORTK, &PORTK, &PORTK, &PORTK, &PORTK, &PORTK, &PORTK
};

PROGMEM static volatile byte *const pinIOLIST[] = {
    /* 0 - 13 */
    &PINE, &PINE, &PINE, &PINE, &PING, &PINE, &PINH, &PINH, &PINH,
    &PINH, &PINB, &PINB, &PINB, &PINB,
    /* 14 - 21 */
    &PINJ, &PINJ, &PINH, &PINH, &PIND, &PIND, &PIND, &PIND,
    /* 22 - 37 */
    &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINC,
    &PINC, &PINC, &PINC, &PINC, &PINC, &PINC, &PINC,
    /* 38 - 49 */
    &PIND, &PING, &PING, &PING, &PINL, &PINL, &PINL, &PINL, &PINL,
    &PINL, &PINL, &PINL,
    /* 50 - 53 */
    &PINB, &PINB, &PINB, &PINB,
    /* A0 - A15 */
    &PINF, &PINF, &PINF, &PINF, &PINF, &PINF, &PINF, &PINF, &PINK,
    &PINK, &PINK, &PINK, &PINK, &PINK, &PINK, &PINK
};

PROGMEM static volatile byte *const ddrIOLIST[] = {
    /* 0 - 13 */
    &DDRE, &DDRE, &DDRE, &DDRE, &DDRG, &DDRE, &DDRH, &DDRH, &DDRH,
    &DDRH, &DDRB, &DDRB, &DDRB, &DDRB,
    /* 14 - 21 */
    &DDRJ, &DDRJ, &DDRH, &DDRH, &DDRD, &DDRD, &DDRD, &DDRD,
    /* 22 - 37 */
    &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRC,
    &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC,
    /* 38 - 49 */
    &DDRD, &DDRG, &DDRG, &DDRG, &DDRL, &DDRL, &DDRL, &DDRL, &DDRL,
    &DDRL, &DDRL, &DDRL,
    /* 50 - 53 */
    &DDRB, &DDRB, &DDRB, &DDRB,
    /* A0 - A15 */
    &DDRF, &DDRF, &DDRF, &DDRF, &DDRF, &DDRF, &DDRF, &DDRF, &DDRK,
    &DDRK, &DDRK, &DDRK, &DDRK, &DDRK, &DDRK, &DDRK
};

PROGMEM static const byte bitIOLIST[] = {
    /* 0 - 13 */
    0U, 1U, 4U, 5U, 5U, 3U, 3U, 4U, 5U, 6U, 4U, 5U, 6U, 7U,
    /* 14 - 21 */
    1U, 0U, 1U, 0U, 3U, 2U, 1U, 0U,
    /* 22 - 37 */
    0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 7U, 6U, 5U, 4U, 3U, 2U, 1U, 0U,
    /* 38 - 49 */
    7U, 2U, 1U, 0U, 7U, 6U, 5U, 4U, 3U, 2U, 1U, 0U,
    /* 50 - 53 */
    3U, 2U, 1U, 0U,
    /* A0 - A15 */
    0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U
};

/** Change pin configuration. The modes are INPUT, OUTPUT and INPUT_PULLUP.

Note: Use pin ANALOGIO+X for analog pin X. */
void pinMode(byte io, byte mode)
{
    volatile byte *port = pgm_read_word(&portIOLIST[io]);
    volatile byte *ddr = pgm_read_word(&ddrIOLIST[io]);
    byte bit = pgm_read_byte(&bitIOLIST[io]);

    ASSERT(io < MAXIO);

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
            /* Enable pull-up. */
            SET_BITS(*port, (1U << bit), byte);
            break;
    }
}

/** Change pin output value or input pull-up. The values are LOW and HIGH.

Note: Use pin ANALOGIO+X for analog pin X. */
void digitalWrite(byte io, byte value)
{
    volatile byte *port = pgm_read_word(&portIOLIST[io]);
    byte bit = pgm_read_byte(&bitIOLIST[io]);

    ASSERT(io < MAXIO);

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

/** Read pin input value. The values returned are LOW and HIGH.

Note: Use pin ANALOGIO+X for analog pin X. */
byte digitalRead(byte io)
{
    volatile byte *pin = pgm_read_word(&pinIOLIST[io]);
    byte bit = pgm_read_byte(&bitIOLIST[io]);

    ASSERT(io < MAXIO);

    return ( *pin & (1U << bit) ) != 0U;
}