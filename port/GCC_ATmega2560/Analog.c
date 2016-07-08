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
#include <avr/pgmspace.h>

#if (ANALOG_ENABLE != 0)

PROGMEM static const byte muxADC[] = {
        /* A0 - A7 */
        0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U,
        /* A8 - A15 */
        32U, 33U, 34U, 35U, 36U, 37U, 38U, 39U,
        /* A1V1 */
        30U
};

/** Enable ADC. */
void adcBegin(void)
{
    PRR0 &= ~(1U << PRADC); /* Enable ADC clock. */

    /* Enable ADC, clear interrupt flag and set baud rate to minimum. */
    ADCSRA = (1U << ADEN) | (1U << ADIF) | (0x07U << ADPS0);

    analogReference (INTERNALVCC);
}

/** Disable ADC. */
void adcEnd(void)
{
    ADCSRA = 0U; /* Disable ADC. */
    PRR0 |= (1U << PRADC); /* Disable ADC clock. */
}

/** Start an analog to digital conversion. */
void analogConvertStart(uint8_t analog)
{
    analog = pgm_read_byte(&muxADC[analog - A0]);
    ADMUX = (ADMUX & ~(0x1FU << MUX0)) | ((analog & 0x1FU) << MUX0);
    ADCSRB = (ADCSRB & ~(1U << MUX5)) | (((analog & 0x20U) >> 5U) << MUX5);
    ADCSRA |= (1U << ADSC);
}

/** Check if an analog to digital conversion has finished.

 Return 1U if the conversion has finished, 0U otherwise. */
uint8_t analogConvertReady(void)
{
    return (ADCSRA & (1U << ADIF)) != 0U;
}

/** Get the value from the analog to digital conversion. */
uint16_t analogConvertGetValue(void)
{
    return ADC;
}

/** Read and return an analog value. */
uint16_t analogRead(uint8_t analog)
{
    analogConvertStart(analog);
    while(!analogConvertReady())
    {
    }
    return analogConvertGetValue();
}

/** Set analog reference.

 See enum AnalogReferences for the available references. */
void analogReference(uint8_t reference)
{
    ADMUX = (ADMUX & ~(0x03U << REFS0)) | (reference << REFS0);

}

#endif /* ANALOG_ENABLE */
