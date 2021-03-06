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
#include "Arduinutil_Timer.h"

#if (TIMER_ENABLE != 0)

static uint32_t TimerIntCount = 0U;

/** Enable Timer. */
void timerBegin(void)
{
    uint8_t prescaler;

    (void)prescaler;

    TACTL = 0U; /* Disable timer. */

    switch(TIMER_PRESCALER) {
    case 1U:
        prescaler = 0x00U;
        break;
    case 2U:
        prescaler = 0x40U;
        break;
    case 3U:
        prescaler = 0x80U;
        break;
    case 8U:
        prescaler = 0xC0U;
        break;
    default:
        prescaler = 0xC0U;
        ASSERT(0); /* Invalid prescaler value */
    }

    TACTL |=
            (0x0200U) | /* TASSELx=0b10 SMCLK */
            prescaler | /* IDx=prescaler */
            (0x20U) | /* MCx=0b10 Continuous mode */
            TACLR | /* Clear timer */
            TAIE; /* Enable interrupt */
}

/** Disable Timer. */
void timerEnd(void)
{
    TACTL = 0U;
}

__attribute__((interrupt(TIMER0_A1_VECTOR)))
void timer0_a1_isr(void)
{
    if(TAIV == 0x0AU /* TAIFG */)
    {
        TimerIntCount += 1UL;
    }
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
    /* There is no reliable way to know if the timer has overflowed.
     The register TAIV shows only the highest priority pending interrupt, so if
     there is an overflow and a capture interrupt pending we may get an
     inconsistent TimerIntCount. */

    uint32_t timerIntCount;
    uint16_t timerCount;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        timerIntCount = TimerIntCount;

        #if 1
        {
            /* Capture disabled */
            timerCount = TAR;
            if(TACTL & TAIFG)
                timerCount = 65535U;
        }
        #else
        {
            /* Capture enabled - lose precision for consistency */
            timerCount = 0U;
        }
        #endif
    }
    CRITICAL_EXIT();

    return (timerIntCount * 65536U + timerCount);
}

/** Add counts to timer counter variable.

 This function allows to update the timer count for the time the microcontroller
 has been sleeping. This way the application will know time has passed if it
 should do something in the meantime.

  void enter_sleep_mode_and_update_time(void) {
    hw_sleep();
    after_wakeup();
    timerAddSleepedCounts(TIMER_MS_TO_COUNT(1000));
  }
 */
void timerAddSleepedCounts(uint32_t counts)
{
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        TimerIntCount += counts / 65536U;
    }
    CRITICAL_EXIT();
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
