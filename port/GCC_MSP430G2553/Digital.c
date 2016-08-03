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
static volatile uint8_t *pin2sel2(uint8_t pin);
static volatile uint8_t *pin2dir(uint8_t pin);
static volatile uint8_t *pin2ren(uint8_t pin);
static volatile uint8_t *pin2out(uint8_t pin);
static const volatile uint8_t *pin2in(uint8_t pin);
static uint8_t pin2bit(uint8_t pin);

static volatile uint8_t *pin2sel(uint8_t pin)
{
    if(pin < 2 || pin >= 8)
        return &P1SEL;
    else
        return &P2SEL;
}

static volatile uint8_t *pin2sel2(uint8_t pin)
{
    if(pin < 2 || pin >= 8)
        return &P1SEL2;
    else
        return &P2SEL2;
}

static volatile uint8_t *pin2dir(uint8_t pin)
{
    if(pin < 2 || pin >= 8)
        return &P1DIR;
    else
        return &P2DIR;
}

static volatile uint8_t *pin2ren(uint8_t pin)
{
    if(pin < 2 || pin >= 8)
        return &P1REN;
    else
        return &P2REN;
}

static volatile uint8_t *pin2out(uint8_t pin)
{
    if(pin < 2 || pin >= 8)
        return &P1OUT;
    else
        return &P2OUT;
}

static const volatile uint8_t *pin2in(uint8_t pin)
{
    if(pin < 2 || pin >= 8)
        return &P1IN;
    else
        return &P2IN;
}

static uint8_t pin2bit(uint8_t pin)
{
    switch(pin) {
    case 0U: return BIT1;
    case 1U: return BIT2;
    case 2U: return BIT2;
    case 3U: return BIT1;
    case 4U: return BIT0;
    case 5U: return BIT3;
    case 6U: return BIT4;
    case 7U: return BIT5;
    case 8U: return BIT7;
    case 9U: return BIT6;
    case 10U: return BIT4;
    case 11U: return BIT7;
    case 12U: return BIT6;
    case 13U: return BIT5;
    case 14U: return BIT0;
    case 15U: return BIT3;
    default: return 0U;
    }
}

/** Change pin configuration. The modes are INPUT, OUTPUT and INPUT_PULLUP. */
void pinMode(uint8_t io, uint8_t mode)
{
    volatile uint8_t *sel = pin2sel(io);
    volatile uint8_t *sel2 = pin2sel2(io);
    volatile uint8_t *dir = pin2dir(io);
    volatile uint8_t *ren = pin2ren(io);
    uint8_t bit = pin2bit(io);

    ASSERT(io < MAXIO);

    *sel &= ~bit;
    *sel2 &= ~bit;

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
