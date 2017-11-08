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
#include "Config.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#if (PWM_ENABLE != 0)

void timer1Begin(void)
{
    uint8_t prescaler;

    PRR &= ~(1U << PRTIM1); /* Enable timer clock. */

    TIMSK1 = 0U; /* Disable timer interrupts. */

    switch(TIMER1_PRESCALER) {
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

    TCCR1A =
            (0x00U << COM1A0) | /* Normal port operation, OC1A disconnected. */
            (0x00U << COM1B0) | /* Normal port operation, OC1B disconnected. */
            (0x01U << WGM10); /* Mode: Fast PWM 8bit (WGM13:0=0b0101). */

    TCCR1B =
            (0x00U << ICNC1) |
            (0x00U << ICES1) |
            (0x01U << WGM12) | /* Mode: Fast PWM 8bit (WGM13:0=0b0101). */
            (prescaler << CS10); /* Clock source. */

    TCCR1C =
            (0x00U << FOC1A) |
            (0x00U << FOC1B);

    TCNT1 = 0U; /* Clear counter. */
    TIFR1 = 0xFFU; /* Clear interrupt flags. */

    #if (TIMER1_OVERFLOW_INTERRUPT != 0)
    TIMSK1 = (1U << TOIE1); /* Enable timer overflow interrupt. */
    #endif
}

void timer1End()
{
    TIMSK1 = 0U; /* Disable timer interrupts. */
    TCCR1B = 0U; /* Disable clock source. */
    PRR |= (1U << PRTIM1); /* Disable timer clock. */
}

#if (TIMER1_OVERFLOW_INTERRUPT != 0)

static uint32_t Timer1IntCount = 0U;

uint32_t timer1Counts(void)
{
    uint32_t timerIntCount;
    uint8_t timerCount;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        timerIntCount = Timer1IntCount;
        timerCount = TCNT1L;
        if(TIFR1 & (1U << TOV1))
            timerCount = 255U;
    }
    CRITICAL_EXIT();
    return (timerIntCount * 256UL + timerCount);
}

ISR(TIMER1_OVF_vect)
{
    Timer1IntCount += 1U;
}

#endif /* TIMER1_OVERFLOW_INTERRUPT */

void timer2Begin(void)
{
    uint8_t prescaler;

    PRR &= ~(1U << PRTIM2); /* Enable timer clock. */

    TIMSK2 = 0U; /* Disable timer interrupts. */

    switch(TIMER2_PRESCALER) {
        case 1U:
            prescaler = 0x01U;
            break;
        case 8U:
            prescaler = 0x02U;
            break;
        case 32U:
            prescaler = 0x03U;
            break;
        case 64U:
            prescaler = 0x04U;
            break;
        case 128U:
            prescaler = 0x05U;
            break;
        case 256U:
            prescaler = 0x06U;
            break;
        case 1024U:
            prescaler = 0x07U;
            break;
        default:
            prescaler = 0x07U;
            ASSERT(0); /* Invalid prescaler value */
    }

    TCCR2A =
            (0x00U << COM2A0) | /* Normal port operation, OC2A disconnected. */
            (0x00U << COM2B0) | /* Normal port operation, OC2B disconnected. */
            (0x03U << WGM20); /* Mode: Fast PWM (WGM22:0=0b011). */

    TCCR2B =
            (0x00U << FOC2A) |
            (0x00U << FOC2B) |
            (0x00U << WGM22) | /* Mode: Fast PWM (WGM22:0=0b011). */
            (prescaler << CS20); /* Clock source. */

    TCNT2 = 0U; /* Clear counter. */
    TIFR2 = 0xFFU; /* Clear interrupt flags. */

    #if (TIMER2_OVERFLOW_INTERRUPT != 0)
    TIMSK2 = (1U << TOIE2); /* Enable timer overflow interrupt. */
    #endif
}

void timer2End()
{
    TIMSK2 = 0U; /* Disable timer interrupts. */
    TCCR2B = 0U; /* Disable clock source. */
    PRR |= (1U << PRTIM2); /* Disable timer clock. */
}

#if (TIMER2_OVERFLOW_INTERRUPT != 0)

static uint32_t Timer2IntCount = 0U;

uint32_t timer2Counts(void)
{
    uint32_t timerIntCount;
    uint8_t timerCount;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        timerIntCount = Timer2IntCount;
        timerCount = TCNT2;
        if(TIFR2 & (1U << TOV2))
            timerCount = 255U;
    }
    CRITICAL_EXIT();
    return (timerIntCount * 256UL + timerCount);
}

ISR(TIMER2OVF_vect)
{
    Timer2IntCount += 1U;
}

#endif /* TIMER2_OVERFLOW_INTERRUPT */

void pwmMode(uint8_t pin, enum PwmModes mode)
{
    CRITICAL_VAL();
    CRITICAL_ENTER();

    switch(pin) {

    case 3U:
        /* Timer2 - B */
        TCCR2A = (TCCR2A & ~(3U << COM2B0)) | (mode << COM2B0);
        break;

    case 5U:
        /* Timer0 - B */
        TCCR0A = (TCCR0A & ~(3U << COM0B0)) | (mode << COM0B0);
        break;

    case 6U:
        /* Timer0 - A */
        TCCR0A = (TCCR0A & ~(3U << COM0A0)) | (mode << COM0A0);
        break;

    case 9U:
        /* Timer1 - A */
        TCCR1A = (TCCR1A & ~(3U << COM1A0)) | (mode << COM1A0);
        break;

    case 10U:
        /* Timer1 - B */
        TCCR1A = (TCCR1A & ~(3U << COM1B0)) | (mode << COM1B0);
        break;

    case 11U:
        /* Timer2 - A */
        TCCR2A = (TCCR2A & ~(3U << COM2A0)) | (mode << COM2A0);
        break;

    default:
        ASSERT(0); /* Invalid PWM pin. */
    }

    CRITICAL_EXIT();
}

void analogWrite(uint8_t pin, uint8_t value)
{
    switch(pin) {

    case 3U:
        OCR2B = value;
        break;

    case 5U:
        OCR0B = value;
        break;

    case 6U:
        OCR0A = value;
        break;

    case 9U:
        OCR1AL = value;
        break;

    case 10U:
        OCR1BL = value;
        break;

    case 11U:
        OCR2A = value;
        break;

    default:
        ASSERT(0);
    }
}

#endif /* PWM_ENABLE */
