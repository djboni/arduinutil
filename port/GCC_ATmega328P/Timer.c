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
#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMER_US_COUNT_SUM (TIMER_PRESCALER * 1000UL / (F_CPU / 1000UL))
#define TIMER_US_SUM (256UL * TIMER_US_COUNT_SUM)

static uint32_t TimerUs = 0U;

/** Enable Timer. */
void timerBegin(void)
{
    uint8_t prescaler;

    PRR &= ~(1U << PRTIM0); /* Enable timer clock. */

    TIMSK0 = 0U; /* Disable timer interrupts. */

    switch(TIMER_PRESCALER) {
        case 1U:
            prescaler = 0x01U;
            break;
        case 8U:
            prescaler = 0x02U;
            break;
        case 64U:
            prescaler = 0x03U;
            break;
        case 256U:
            prescaler = 0x04U;
            break;
        case 1024U:
            prescaler = 0x05U;
            break;
        default:
            prescaler = 0x05U;
            ASSERT(0); /* Invalid prescaler value */
    }

    TCCR0A =
        (0x00U << COM0A0) | /* Normal port operation, OC0A disconnected. */
        (0x00U << COM0B0) | /* Normal port operation, OC0B disconnected. */
        (0x00U << WGM00); /* Mode: Normal (WGM00:2=0b000). */

    TCCR0B =
        (0x00U << FOC0A) |
        (0x00U << FOC0B) |
        (0x00U << WGM02) | /* Mode: Normal (WGM00:2=0b000). */
        (prescaler << CS00); /* Clock source. */

    TCNT0 = 0U; /* Clear counter. */
    TIFR0 = 0xFFU; /* Clear interrupt flags. */
    TIMSK0 = (1U << TOIE0); /* Enable timer overflow interrupt. */
}

/** Disable Timer. */
void timerEnd(void)
{
    TIMSK0 = 0U; /* Disable timer interrupts. */
    PRR |= (1U << PRTIM0); /* Disable timer clock. */
}

ISR(TIMER0_OVF_vect)
{
    TimerUs += TIMER_US_SUM;
}

/** Return the number of milliseconds the system is running since last call to
timerBegin().

Note: This function may return an outdated value if interrupts are disabled. */
uint32_t millis(void)
{
    return micros() / 1000UL;
}

/** Return the number of microseconds the system is running since last call to
timerBegin().

Note: This function may return an outdated value if interrupts are disabled. */
uint32_t micros(void)
{
    uint32_t us;
    uint8_t count;
    ENTER_CRITICAL();
    {
        count = TCNT0;
        us = TimerUs;
        if(TIFR0 & (1U << TOV0))
            count = 255U;
    }
    EXIT_CRITICAL();
    return (us + count * TIMER_US_COUNT_SUM);
}

/** Stop execution for a given time in milliseconds.

Note: This function requires interrupts to be enabled. */
void delay(uint32_t ms)
{
    delayMicroseconds(ms * 1000UL);
}

/** Stop execution for a given time in microseconds.
Note: This function requires interrupts to be enabled. */
void delayMicroseconds(uint32_t us)
{
    uint32_t call_time = micros();
    while( (int32_t)(micros() - call_time) < (int32_t)us ) {}
}
