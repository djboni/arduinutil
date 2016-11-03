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
#include <avr/sleep.h>

#define SET_BITS(reg, bits, cast)   do{reg=(cast)((reg)|(bits));}while(0U)
#define CLEAR_BITS(reg, bits, cast) do{reg=(cast)((reg)&(~(bits)));}while(0U)

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
    DDRA = 0U;
    PORTA = 0xFFU;
    DDRB = 0U;
    PORTB = 0xFFU;
    DDRC = 0U;
    PORTC = 0xFFU;
    DDRD = 0U;
    PORTD = 0xFFU;
    DDRE = 0U;
    PORTE = 0xFFU;
    DDRF = 0U;
    PORTF = 0x00U;
    DDRG = 0U;
    PORTG = 0xFFU;
    DDRH = 0U;
    PORTH = 0xFFU;
    DDRJ = 0U;
    PORTJ = 0xFFU;
    DDRK = 0U;
    PORTK = 0x00U;
    DDRL = 0U;
    PORTL = 0xFFU;

    disableDigitalInputsOfAnalogPins();
    disablePeripheralsClocks();

    /* Configure external interrupts registers. */
    EIMSK = 0U;
    EIFR = 0xFFU;
    PCICR = 0U;
    PCIFR = 0x07U;
    PCMSK0 = 0U;
    PCMSK1 = 0U;
    PCMSK2 = 0U;
    PCICR = 0x07U;

    ENABLE_INTERRUPTS();

    set_sleep_mode(0); /* IDLE */
    sleep_enable();
}

/** Disable all peripherals clocks for lower power consumption.

 Note: Changes on peripheral registers will not have effect after disabling its
 clock. As an example, you should disable ADC (ADCSRA[ADEN]=0) before disabling
 its clock (PRR[PRADC]=1), otherwise it will still be enabled and consuming
 power. */
void disablePeripheralsClocks(void)
{
    PRR0 = 0xFFU;
    PRR1 = 0xFFU;
}

/** Enable peripherals clocks. */
void enablePeripheralsClocks(void)
{
    PRR0 = 0U;
    PRR1 = 0U;
}

/** Disable digital inputs of analog pins for lower power consumption. */
void disableDigitalInputsOfAnalogPins(void)
{
    VAR_CRITICAL();

    ENTER_CRITICAL();
    {
        uint8_t prr_save = PRR0;
        PRR0 &= ~(1U << PRADC);

        /* PRR0[PRADC] must be 0. */
        DIDR0 = 0xFFU;
        DIDR1 = 0xFFU;
        DIDR2 = 0xFFU;

        PRR0 = prr_save;
    }
    EXIT_CRITICAL();
}

/** Enable digital inputs of analog pins. */
void enableDigitalInputsOfAnalogPins(void)
{
    VAR_CRITICAL();

    ENTER_CRITICAL();
    {
        uint8_t prr_save = PRR0;
        PRR0 &= ~(1U << PRADC);

        /* PRR0[PRADC] must be 0. */
        DIDR0 = 0U;
        DIDR1 = 0U;
        DIDR2 = 0U;

        PRR0 = prr_save;
    }
    EXIT_CRITICAL();
}
