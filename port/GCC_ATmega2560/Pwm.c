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

#if (PWM_ENABLE != 0)

#define PIN_D13_nTIMER0_TIMER1 0

void timer1Begin(void)
{
    uint8_t prescaler;

    PRR0 &= ~(1U << PRTIM1); /* Enable timer clock. */

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
            (0x00U << COM1C0) | /* Normal port operation, OC1C disconnected. */
            (0x01U << WGM10); /* Mode: Fast PWM 8bit (WGM13:0=0b0101). */

    TCCR1B =
            (0x00U << ICNC1) |
            (0x00U << ICES1) |
            (0x01U << WGM12) | /* Mode: Fast PWM 8bit (WGM13:0=0b0101). */
            (prescaler << CS10); /* Clock source. */

    TCCR1C =
            (0x00U << FOC1A) |
            (0x00U << FOC1B) |
            (0x00U << FOC1C);

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
    PRR0 |= (1U << PRTIM1); /* Disable timer clock. */
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

    PRR0 &= ~(1U << PRTIM2); /* Enable timer clock. */

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
    PRR0 |= (1U << PRTIM2); /* Disable timer clock. */
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

ISR(TIMER2_OVF_vect)
{
    Timer2IntCount += 1U;
}

#endif /* TIMER2_OVERFLOW_INTERRUPT */

void timer3Begin(void)
{
    uint8_t prescaler;

    PRR1 &= ~(1U << PRTIM3); /* Enable timer clock. */

    TIMSK3 = 0U; /* Disable timer interrupts. */

    switch(TIMER3_PRESCALER) {
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

    TCCR3A =
            (0x00U << COM3A0) | /* Normal port operation, OC3A disconnected. */
            (0x00U << COM3B0) | /* Normal port operation, OC3B disconnected. */
            (0x00U << COM3C0) | /* Normal port operation, OC3C disconnected. */
            (0x01U << WGM30); /* Mode: Fast PWM 8bit (WGM33:0=0b0101). */

    TCCR3B =
            (0x00U << ICNC3) |
            (0x00U << ICES3) |
            (0x01U << WGM32) | /* Mode: Fast PWM 8bit (WGM33:0=0b0101). */
            (prescaler << CS30); /* Clock source. */

    TCCR3C =
            (0x00U << FOC3A) |
            (0x00U << FOC3B) |
            (0x00U << FOC3C);

    TCNT3 = 0U; /* Clear counter. */
    TIFR3 = 0xFFU; /* Clear interrupt flags. */

    #if (TIMER3_OVERFLOW_INTERRUPT != 0)
    TIMSK3 = (1U << TOIE3); /* Enable timer overflow interrupt. */
    #endif
}

void timer3End()
{
    TIMSK3 = 0U; /* Disable timer interrupts. */
    TCCR3B = 0U; /* Disable clock source. */
    PRR1 |= (1U << PRTIM3); /* Disable timer clock. */
}

#if (TIMER3_OVERFLOW_INTERRUPT != 0)

static uint32_t Timer3IntCount = 0U;

uint32_t timer3Counts(void)
{
    uint32_t timerIntCount;
    uint8_t timerCount;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        timerIntCount = Timer3IntCount;
        timerCount = TCNT3;
        if(TIFR3 & (1U << TOV3))
            timerCount = 255U;
    }
    CRITICAL_EXIT();
    return (timerIntCount * 256UL + timerCount);
}

ISR(TIMER3_OVF_vect)
{
    Timer3IntCount += 1U;
}

#endif /* TIMER2_OVERFLOW_INTERRUPT */

void timer4Begin(void)
{
    uint8_t prescaler;

    PRR1 &= ~(1U << PRTIM4); /* Enable timer clock. */

    TIMSK4 = 0U; /* Disable timer interrupts. */

    switch(TIMER4_PRESCALER) {
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

    TCCR4A =
            (0x00U << COM4A0) | /* Normal port operation, OC4A disconnected. */
            (0x00U << COM4B0) | /* Normal port operation, OC4B disconnected. */
            (0x00U << COM4C0) | /* Normal port operation, OC4C disconnected. */
            (0x01U << WGM40); /* Mode: Fast PWM 8bit (WGM43:0=0b0101). */

    TCCR4B =
            (0x00U << ICNC4) |
            (0x00U << ICES4) |
            (0x01U << WGM42) | /* Mode: Fast PWM 8bit (WGM43:0=0b0101). */
            (prescaler << CS40); /* Clock source. */

    TCCR4C =
            (0x00U << FOC4A) |
            (0x00U << FOC4B) |
            (0x00U << FOC4C);

    TCNT4 = 0U; /* Clear counter. */
    TIFR4 = 0xFFU; /* Clear interrupt flags. */

    #if (TIMER4_OVERFLOW_INTERRUPT != 0)
    TIMSK4 = (1U << TOIE4); /* Enable timer overflow interrupt. */
    #endif
}

void timer4End()
{
    TIMSK4 = 0U; /* Disable timer interrupts. */
    TCCR4B = 0U; /* Disable clock source. */
    PRR1 |= (1U << PRTIM4); /* Disable timer clock. */
}

#if (TIMER4_OVERFLOW_INTERRUPT != 0)

static uint32_t Timer4IntCount = 0U;

uint32_t timer4Counts(void)
{
    uint32_t timerIntCount;
    uint8_t timerCount;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        timerIntCount = Timer4IntCount;
        timerCount = TCNT4L;
        if(TIFR4 & (1U << TOV4))
            timerCount = 255U;
    }
    CRITICAL_EXIT();
    return (timerIntCount * 256UL + timerCount);
}

ISR(TIMER4_OVF_vect)
{
    Timer4IntCount += 1U;
}

#endif /* TIMER4_OVERFLOW_INTERRUPT */

void timer5Begin(void)
{
    uint8_t prescaler;

    PRR1 &= ~(1U << PRTIM5); /* Enable timer clock. */

    TIMSK5 = 0U; /* Disable timer interrupts. */

    switch(TIMER5_PRESCALER) {
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

    TCCR5A =
            (0x00U << COM5A0) | /* Normal port operation, OC5A disconnected. */
            (0x00U << COM5B0) | /* Normal port operation, OC5B disconnected. */
            (0x00U << COM5C0) | /* Normal port operation, OC5C disconnected. */
            (0x01U << WGM50); /* Mode: Fast PWM 8bit (WGM53:0=0b0101). */

    TCCR5B =
            (0x00U << ICNC5) |
            (0x00U << ICES5) |
            (0x01U << WGM52) | /* Mode: Fast PWM 8bit (WGM53:0=0b0101). */
            (prescaler << CS50); /* Clock source. */

    TCCR5C =
            (0x00U << FOC5A) |
            (0x00U << FOC5B) |
            (0x00U << FOC5C);

    TCNT5 = 0U; /* Clear counter. */
    TIFR5 = 0xFFU; /* Clear interrupt flags. */

    #if (TIMER5_OVERFLOW_INTERRUPT != 0)
    TIMSK5 = (1U << TOIE5); /* Enable timer overflow interrupt. */
    #endif
}

void timer5End()
{
    TIMSK5 = 0U; /* Disable timer interrupts. */
    TCCR5B = 0U; /* Disable clock source. */
    PRR1 |= (1U << PRTIM5); /* Disable timer clock. */
}

#if (TIMER5_OVERFLOW_INTERRUPT != 0)

static uint32_t Timer5IntCount = 0U;

uint32_t timer5Counts(void)
{
    uint32_t timerIntCount;
    uint8_t timerCount;
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        timerIntCount = Timer5IntCount;
        timerCount = TCNT5L;
        if(TIFR5 & (1U << TOV5))
            timerCount = 255U;
    }
    CRITICAL_EXIT();
    return (timerIntCount * 256UL + timerCount);
}

ISR(TIMER5_OVF_vect)
{
    Timer5IntCount += 1U;
}

#endif /* TIMER5_OVERFLOW_INTERRUPT */

void pwmMode(uint8_t pin, enum PwmModes mode)
{
    CRITICAL_VAL();
    CRITICAL_ENTER();

    switch(pin) {

    case 2U:
        /* Timer3 - B */
        TCCR3A = (TCCR3A & ~(3U << COM3B0)) | (mode << COM3B0);
        break;

    case 3U:
        /* Timer3 - C */
        TCCR3A = (TCCR3A & ~(3U << COM3C0)) | (mode << COM3C0);
        break;

    case 4U:
        /* Timer0 - B */
        TCCR0A = (TCCR0A & ~(3U << COM0B0)) | (mode << COM0B0);
        break;

    case 5U:
        /* Timer3 - A */
        TCCR3A = (TCCR3A & ~(3U << COM3A0)) | (mode << COM3A0);
        break;

    case 6U:
        /* Timer4 - A */
        TCCR4A = (TCCR4A & ~(3U << COM4A0)) | (mode << COM4A0);
        break;

    case 7U:
        /* Timer4 - B */
        TCCR4A = (TCCR4A & ~(3U << COM4B0)) | (mode << COM4B0);
        break;

    case 8U:
        /* Timer4 - C */
        TCCR4A = (TCCR4A & ~(3U << COM4C0)) | (mode << COM4C0);
        break;

    case 9U:
        /* Timer2 - B */
        TCCR2A = (TCCR2A & ~(3U << COM2B0)) | (mode << COM2B0);
        break;

    case 10U:
        /* Timer2 - A */
        TCCR2A = (TCCR2A & ~(3U << COM2A0)) | (mode << COM2A0);
        break;

    case 11U:
        /* Timer1 - A */
        TCCR1A= (TCCR1A & ~(3U << COM1A0)) | (mode << COM1A0);
        break;

    case 12U:
        /* Timer1 - B */
        TCCR1A= (TCCR1A & ~(3U << COM1B0)) | (mode << COM1B0);
        break;

    case 13U:
        #if (PIN_D13_nTIMER0_TIMER1 == 0)
            /* Timer0 - A */
            TCCR0A = (TCCR0A & ~(3U << COM0A0)) | (mode << COM0A0);
        #else
            /* Timer1 - C */
            TCCR1A= (TCCR1A & ~(3U << COM1C0)) | (mode << COM1C0);
        #endif
        break;

    case 44U:
        /* Timer5 - C */
        TCCR5A= (TCCR5A & ~(3U << COM5C0)) | (mode << COM5C0);
        break;

    case 45U:
        /* Timer5 - B */
        TCCR5A= (TCCR5A & ~(3U << COM5B0)) | (mode << COM5B0);
        break;

    case 46U:
        /* Timer5 - A */
        TCCR5A= (TCCR5A & ~(3U << COM5A0)) | (mode << COM5A0);
        break;

    default:
        ASSERT(0); /* Invalid PWM pin. */
    }

    CRITICAL_EXIT();
}

void analogWrite(uint8_t pin, uint8_t value)
{
    switch(pin) {

    case 2U:
        OCR3B = value;
        break;

    case 3U:
        OCR3C = value;
        break;

    case 4U:
        OCR0B = value;
        break;

    case 5U:
        OCR3A = value;
        break;

    case 6U:
        OCR4AL = value;
        break;

    case 7U:
        OCR4BL = value;
        break;

    case 8U:
        OCR4CL = value;
        break;

    case 9U:
        OCR2B = value;
        break;

    case 10U:
        OCR2A = value;
        break;

    case 11U:
        OCR1AL = value;
        break;

    case 12U:
        OCR1BL = value;
        break;

    case 13U:
        #if (PIN_D13_nTIMER0_TIMER1 == 0)
            OCR0A = value;
        #else
            OCR1CL = value;
        #endif
        break;

    case 44U:
        OCR5CL = value;
        break;

    case 45U:
        OCR5BL = value;
        break;

    case 46U:
        OCR5AL = value;
        break;

    default:
        ASSERT(0);
    }
}

#endif /* PWM_ENABLE */
