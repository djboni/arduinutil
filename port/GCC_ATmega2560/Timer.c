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
#include <avr/io.h>
#include <avr/interrupt.h>

#if (TIMER_ENABLE != 0)

static uint32_t TimerIntCount = 0U;

/** Enable Timer. */
void timerBegin(void)
{
    uint8_t prescaler;

    PRR0 &= ~(1U << PRTIM0); /* Enable timer clock. */

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
    TCCR0B = 0U; /* Disable clock source. */
    PRR0 |= (1U << PRTIM0); /* Disable timer clock. */
}

ISR(TIMER0_OVF_vect)
{
    TimerIntCount += 1U;
}

/** Return the number of milliseconds the timer is running.

 Note: This function may return an outdated value if interrupts are disabled. */
uint32_t millis(void)
{
    return TIMER_COUNT_TO_MS(timerCounts());
}

/** Return the number of microseconds the timer is running.

 Note: This function may return an outdated value if interrupts are disabled. */
uint32_t micros(void)
{
    return TIMER_COUNT_TO_US(timerCounts());
}

/** Return the number of counts the timer had.

 Note: This function may return an outdated value if interrupts are disabled. */
uint32_t timerCounts(void)
{
    uint32_t timerIntCount;
    uint8_t timerCount;
    VAR_CRITICAL();

    ENTER_CRITICAL();
    {
        timerIntCount = TimerIntCount;
        timerCount = TCNT0;
        if(TIFR0 & (1U << TOV0))
            timerCount = 255U;
    }
    EXIT_CRITICAL();
    return (timerIntCount * 256UL + timerCount);
}

/** Stop execution for a given time in milliseconds.

 Note: This function requires interrupts to be enabled. */
void delay(uint32_t ms)
{
    delayCounts(TIMER_MS_TO_COUNT(ms));
}

/** Stop execution for a given time in microseconds.

 Note: This function requires interrupts to be enabled. */
void delayMicroseconds(uint32_t us)
{
    delayCounts(TIMER_US_TO_COUNT(us));
}

/** Stop execution for a given number of timer counts.

 Note: This function requires interrupts to be enabled. */
void delayCounts(uint32_t counts)
{
    uint32_t call_time = timerCounts();
    while((timerCounts() - call_time) < counts)
    {
    }
}

#endif /* TIMER_ENABLE */
