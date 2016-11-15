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

/** Microcontroller initialization. */
void init(void)
{
    /* Configure all pins of the microcontroller as input with pull-up.
     Avoid power consumption of floating input pins.

     You can change pin direction and pull-up with pinMode() or make changes
     directly here. */
    P1SEL = 0U;
    P1SEL2 = 0U;
    P1DIR = 0U;
    P1REN = 0xFFU;
    P1OUT = 0xFFU;
    P2SEL = 0U;
    P2SEL2 = 0U;
    P2DIR = 0U;
    P2REN = 0xFFU;
    P2OUT = 0xFFU;

    /* Configure external interrupts registers. */
    P1IE = 0U;
    P2IE = 0U;

    INTERRUPTS_ENABLE();
}
