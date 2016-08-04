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

#if (ANALOG_ENABLE != 0)

/** Enable ADC. */
void adcBegin(void)
{
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 = ADC10ON;
    ADC10CTL1 = 0U;

    analogReference(INTERNALVCC);
}

/** Disable ADC. */
void adcEnd(void)
{
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 &= ~ADC10ON;
}

/* Port specific function. Not declared as static because it is used in
 Digital.c file. */
uint8_t pin2analog(uint8_t pin)
{
    switch(pin) {
    case A0: return 0U;
    case A1: return 1U;
    case A2: return 2U;
    case A3: return 3U;
    case A4: return 4U;
    case A5: return 5U;
    case A6: return 6U;
    case A7: return 7U;
    case AREFP: return 8U;
    case AREFM: return 9U;
    case ATEMP: return 10U;
    default:
        return 8U;
    }
}

/** Start an analog to digital conversion. */
void analogConvertStart(uint8_t pin)
{
    uint8_t analog = pin2analog(pin);
    ADC10CTL0 &= ~ENC;
    ADC10CTL1 = (ADC10CTL1 & ~0xF000U) | (analog << 12U);
    ADC10CTL0 |= ENC | ADC10SC;
}

/** Check if an analog to digital conversion has finished.

 Return 1U if the conversion has finished, 0U otherwise. */
uint8_t analogConvertReady(void)
{
    return (ADC10CTL0 & ADC10IFG) != 0U;
}

/** Get the value from the analog to digital conversion. */
uint16_t analogConvertGetValue(void)
{
    return ADC10MEM;
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
    uint16_t ref;
    switch(reference) {
    case INTERNALVCC: ref = 0x0000U; break;
    case EXTERNAL: ref = 0x2000U; break;
    case INTERNAL1V5: ref = 0x2020U; break;
    case INTERNAL2V5: ref = 0x2060U; break;
    default: ref = 0x0000U; break;
    }
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 = (ADC10CTL0 & ~0xE060U) | ref;
}

#endif /* ANALOG_ENABLE */
