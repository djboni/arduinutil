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

#if (PWM_ENABLE != 0)

void pwmMode(uint8_t pin, enum PwmModes mode)
{
    ENTER_CRITICAL();

    switch(pin) {

    case 5U:
        /* Timer0 -
           WGM02:0=0b011 - Fast PWM - BOTTOM=0 to TOP=0xFF
           COM0B1:0=0b10 - mode */
        TCCR0A = (TCCR0A & ~(3U << COM0B0)) | (mode << COM0B0);
        break;

    case 6U:
        /* Timer0
           WGM02:0=0b011 - Fast PWM - BOTTOM=0 to TOP=0xFF
           COM0A1:0=0b10 - mode */
        TCCR0A = (TCCR0A & ~(3U << COM0A0)) | (mode << COM0A0);
        break;

    default:
        ASSERT(0); /* Invalid PWM pin. */
    }

    EXIT_CRITICAL();
}

void analogWrite(uint8_t pin, uint16_t value)
{
    switch(pin) {

    case 5U:
        OCR0B = value;
        break;

    case 6U:
        OCR0A = value;
        break;

    default:
        ASSERT(0);
    }
}

#endif /* PWM_ENABLE */
