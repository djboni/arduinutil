/*
 Arduinutil - Arduino-like library written in C

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

static volatile uint8_t *pin2sel(uint8_t pin);
static volatile uint8_t *pin2dir(uint8_t pin);
static volatile uint8_t *pin2ren(uint8_t pin);
static volatile uint8_t *pin2out(uint8_t pin);
static const volatile uint8_t *pin2in(uint8_t pin);
static uint8_t pin2bit(uint8_t pin);

static volatile uint8_t *pin2sel(uint8_t pin)
{
    volatile uint8_t *ret;

    switch(pin) {
    case 0U: ret = &P1SEL; break;
    case 1U: ret = &P1SEL; break;
    case 2U: ret = &P2SEL; break;
    case 3U: ret = &P2SEL; break;
    case 4U: ret = &P2SEL; break;
    case 5U: ret = &P2SEL; break;
    case 6U: ret = &P2SEL; break;
    case 7U: ret = &P2SEL; break;
    case 8U: ret = &P2SEL; break;
    case 9U: ret = &P2SEL; break;
    case 10U: ret = &P1SEL; break;
    case 11U: ret = &P1SEL; break;
    case 12U: ret = &P1SEL; break;
    case 13U: ret = &P1SEL; break;
    case 14U: ret = &P1SEL; break;
    case 15U: ret = &P1SEL; break;
    }

    return ret;
}

static volatile uint8_t *pin2dir(uint8_t pin)
{
    volatile uint8_t *ret;

    switch(pin) {
    case 0U: ret = &P1DIR; break;
    case 1U: ret = &P1DIR; break;
    case 2U: ret = &P2DIR; break;
    case 3U: ret = &P2DIR; break;
    case 4U: ret = &P2DIR; break;
    case 5U: ret = &P2DIR; break;
    case 6U: ret = &P2DIR; break;
    case 7U: ret = &P2DIR; break;
    case 8U: ret = &P2DIR; break;
    case 9U: ret = &P2DIR; break;
    case 10U: ret = &P1DIR; break;
    case 11U: ret = &P1DIR; break;
    case 12U: ret = &P1DIR; break;
    case 13U: ret = &P1DIR; break;
    case 14U: ret = &P1DIR; break;
    case 15U: ret = &P1DIR; break;
    }

    return ret;
}

static volatile uint8_t *pin2ren(uint8_t pin)
{
    volatile uint8_t *ret;

    switch(pin) {
    case 0U: ret = &P1REN; break;
    case 1U: ret = &P1REN; break;
    case 2U: ret = &P2REN; break;
    case 3U: ret = &P2REN; break;
    case 4U: ret = &P2REN; break;
    case 5U: ret = &P2REN; break;
    case 6U: ret = &P2REN; break;
    case 7U: ret = &P2REN; break;
    case 8U: ret = &P2REN; break;
    case 9U: ret = &P2REN; break;
    case 10U: ret = &P1REN; break;
    case 11U: ret = &P1REN; break;
    case 12U: ret = &P1REN; break;
    case 13U: ret = &P1REN; break;
    case 14U: ret = &P1REN; break;
    case 15U: ret = &P1REN; break;
    }

    return ret;
}

static volatile uint8_t *pin2out(uint8_t pin)
{
    volatile uint8_t *ret;

    switch(pin) {
    case 0U: ret = &P1OUT; break;
    case 1U: ret = &P1OUT; break;
    case 2U: ret = &P2OUT; break;
    case 3U: ret = &P2OUT; break;
    case 4U: ret = &P2OUT; break;
    case 5U: ret = &P2OUT; break;
    case 6U: ret = &P2OUT; break;
    case 7U: ret = &P2OUT; break;
    case 8U: ret = &P2OUT; break;
    case 9U: ret = &P2OUT; break;
    case 10U: ret = &P1OUT; break;
    case 11U: ret = &P1OUT; break;
    case 12U: ret = &P1OUT; break;
    case 13U: ret = &P1OUT; break;
    case 14U: ret = &P1OUT; break;
    case 15U: ret = &P1OUT; break;
    }

    return ret;
}

static const volatile uint8_t *pin2in(uint8_t pin)
{
    const volatile uint8_t *ret;

    switch(pin) {
    case 0U: ret = &P1IN; break;
    case 1U: ret = &P1IN; break;
    case 2U: ret = &P2IN; break;
    case 3U: ret = &P2IN; break;
    case 4U: ret = &P2IN; break;
    case 5U: ret = &P2IN; break;
    case 6U: ret = &P2IN; break;
    case 7U: ret = &P2IN; break;
    case 8U: ret = &P2IN; break;
    case 9U: ret = &P2IN; break;
    case 10U: ret = &P1IN; break;
    case 11U: ret = &P1IN; break;
    case 12U: ret = &P1IN; break;
    case 13U: ret = &P1IN; break;
    case 14U: ret = &P1IN; break;
    case 15U: ret = &P1IN; break;
    }

    return ret;
}

static uint8_t pin2bit(uint8_t pin)
{
    volatile uint8_t ret;

    switch(pin) {
    case 0U: ret = BIT1; break;
    case 1U: ret = BIT2; break;
    case 2U: ret = BIT2; break;
    case 3U: ret = BIT1; break;
    case 4U: ret = BIT0; break;
    case 5U: ret = BIT3; break;
    case 6U: ret = BIT4; break;
    case 7U: ret = BIT5; break;
    case 8U: ret = BIT7; break;
    case 9U: ret = BIT6; break;
    case 10U: ret = BIT4; break;
    case 11U: ret = BIT7; break;
    case 12U: ret = BIT6; break;
    case 13U: ret = BIT5; break;
    case 14U: ret = BIT0; break;
    case 15U: ret = BIT3; break;
    }

    return ret;
}

/** Change pin configuration. The modes are INPUT, OUTPUT and INPUT_PULLUP. */
void pinMode(uint8_t io, uint8_t mode)
{
    volatile uint8_t *sel = pin2sel(io);
    volatile uint8_t *dir = pin2dir(io);
    volatile uint8_t *ren = pin2ren(io);
    uint8_t bit = pin2bit(io);

    ASSERT(io < MAXIO);

    *sel &= ~bit;

    switch(mode)
    {
    case INPUT:
        *dir &= ~bit;
        *ren &= ~bit;
        break;
    case OUTPUT:
        *dir |= bit;
        *ren &= ~bit;
        break;
    case INPUT_PULLUP:
        *dir &= ~bit;
        *ren |= bit;
        *pin2out(io) |= bit;
        break;
    case INPUT_PULLDOWN:
        *dir &= ~bit;
        *ren |= bit;
        *pin2out(io) &= ~bit;
        break;
    default:
        ASSERT(0); /* Invalid pin mode. */
        break;
    }
}

/** Change pin output value or input pull-up. The values are LOW and HIGH. */
void digitalWrite(uint8_t io, uint8_t value)
{
    volatile uint8_t *out = pin2out(io);
    uint8_t bit = pin2bit(io);

    ASSERT(io < MAXIO);

    switch(value)
    {
    case LOW:
        *out &= ~bit;
        break;
    default:
        *out |= bit;
        break;
    }
}

/** Read pin input value. The values returned are LOW and HIGH. */
uint8_t digitalRead(uint8_t io)
{
    const volatile uint8_t *in = pin2in(io);
    uint8_t bit = pin2bit(io);

    ASSERT(io < MAXIO);

    return (*in & bit) != 0U;
}
