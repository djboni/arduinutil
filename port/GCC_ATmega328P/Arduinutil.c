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

#define SET_BITS(reg, bits, cast)   do{reg=(cast)((reg)|(bits));}while(0U)
#define CLEAR_BITS(reg, bits, cast) do{reg=(cast)((reg)&(~(bits)));}while(0U)

#include <avr/io.h>

/** Microcontroller initialization. */
void init(void)
{
    /* Configure all pins of the microcontroller as input with pull-up
     (DDR=0,PORT=FF), even those not used by Arduino. Avoid power consumption of
     floating input pins.

     Analog pins have the pull-ups disabled, but the digital inputs for these
     pins are also disabled.

     You can change pin direction and pull-up with pinMode() or make changes
     directly here. */
    DDRB = 0U;
    PORTB = 0xFFU;
    DDRC = 0U;
    PORTC = 0xC0U;
    DDRD = 0U;
    PORTD = 0xFFU;

    disableDigitalInputsOfAnalogPins();
    disablePeripheralsClocks();

    /* Configure external interrupts registers. */
    EIMSK = 0U;
    EIFR = 0x03U;
    PCICR = 0U;
    PCIFR = 0x07U;
    PCMSK0 = 0U;
    PCMSK1 = 0U;
    PCMSK2 = 0U;
    PCICR = 0x07U;

    ENABLE_INTERRUPTS();
}

/** Disable all peripherals clocks for lower power consumption.

 Note: Changes on peripheral registers will not have effect after disabling its
 clock. As an example, you should disable ADC (ADCSRA[ADEN]=0) before disabling
 its clock (PRR[PRADC]=1), otherwise it will still be enabled and consuming
 power. */
void disablePeripheralsClocks(void)
{
    PRR = 0xFFU;
}

/** Enable peripherals clocks. */
void enablePeripheralsClocks(void)
{
    PRR = 0U;
}

/** Disable digital inputs of analog pins for lower power consumption.

 Note: PRR0[PRADC] must be 1. */
void disableDigitalInputsOfAnalogPins(void)
{
    /*  must be 1 */
    DIDR0 = 0xFFU;
    DIDR1 = 0xFFU;
}

/** Enable digital inputs of analog pins.

 Note: PRR0[PRADC] must be 1. */
void enableDigitalInputsOfAnalogPins(void)
{
    /* PRR0[PRADC] must be 1 */
    DIDR0 = 0U;
    DIDR1 = 0U;
}
