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

/** Enable ADC. */
void adcBegin(void)
{
    PRR &= ~(1U << PRADC); /* Enable ADC clock. */

    /* Enable ADC, clear interrupt flag and set baud rate to minimum. */
    ADCSRA = (1U << ADEN) | (1U << ADIF) | (0x07U << ADPS0);

    analogReference(INTERNALVCC);
}

/** Disable ADC. */
void adcEnd(void)
{
    ADCSRA = 0U; /* Disable ADC. */
    PRR |= (1U << PRADC); /* Disable ADC clock. */
}

/** Read and return an analog value. */
uint16_t analogRead(uint8_t analog)
{
    analog -= A0;
    ADMUX = (ADMUX & ~(0x0FU << MUX0)) | (analog << MUX0);
    ADCSRA |= (1U << ADSC);
    while(!(ADCSRA & (1U << ADIF))) {}
    return ADC;
}

/** Set analog reference.

See enum AnalogReferences for the available references. */
void analogReference(uint8_t reference)
{
    ADMUX = (ADMUX & ~(0x03U << REFS0)) | (reference << REFS0);
}
