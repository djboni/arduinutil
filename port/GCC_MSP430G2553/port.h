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

#ifndef __ARDUINUTIL_PORT_H__
#define __ARDUINUTIL_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <msp430.h>

/*******************************************************************************
 Timer.c
 ******************************************************************************/

#define TIMER_COUNT_TO_MS(x) ((x) / (F_CPU / 1000UL) * TIMER_PRESCALER  )
#define TIMER_MS_TO_COUNT(x) ((x) * (F_CPU / 1000UL) / TIMER_PRESCALER )

#define TIMER_COUNT_TO_US(x) ((x) / (F_CPU / 8000UL) * (TIMER_PRESCALER * 125UL) )
#define TIMER_US_TO_COUNT(x) ((x) * (F_CPU / 8000UL) / (TIMER_PRESCALER * 125UL) )

/*******************************************************************************
 Digital.c
 ******************************************************************************/

#define MAXIO    16U

enum PinDefinition {
    P10 = 14U,
    P11 = 0U,
    P12 = 1U,
    P13 = 15U,
    P14 = 10U,
    P15 = 13U,
    P16 = 12U,
    P17 = 11U,
    P20 = 4U,
    P21 = 3U,
    P22 = 2U,
    P23 = 5U,
    P24 = 6U,
    P25 = 7U,
    P26 = 9U,
    P27 = 8U
};

enum DigitalPinModes {
    LOW = 0U,
    HIGH = 1U,

    INPUT = 0U,
    OUTPUT = 1U,
    INPUT_PULLUP = 2U,
    INPUT_PULLDOWN = 3U,

    /* Available only if ANALOG_ENABLE=1. */
    ANALOG_INPUT = 4U, /* Configure analog capable pin as analog input. */
    NOT_ANALOG_INPUT = 5U /* Undo ANALOG_INPUT. */
};

enum DigitalInterruptModes {
    RISING = 0x00U,
    FALLING = 0x01U
};

/*******************************************************************************
 Analog.c
 ******************************************************************************/

enum AnalogDefinition {
    A0 = P10,
    A1 = P11,
    A2 = P12,
    A3 = P13,
    A4 = P14,
    A5 = P15,
    A6 = P16,
    A7 = P17,
    AREFP = 16U,
    AREFM = 17U,
    ATEMP = 18U
};

enum AnalogReferences {
    INTERNALVCC = 0U, /* Internal voltage VCC. */
    EXTERNAL = 1U, /* External voltage on VREF+. */

    INTERNAL1V5 = 2U, /* Internal voltage reference 1.5V. */
    INTERNAL2V5 = 3U, /* Internal voltage reference 2.5V. */

    /* Arduino IDE compatibility. */
    DEFAULT = INTERNALVCC,
    INTERNAL = INTERNAL1V5
};

/*******************************************************************************
 Others
 ******************************************************************************/

#define DISABLE_INTERRUPTS() __asm__ __volatile__("DINT \n\t" "NOP")
#define ENABLE_INTERRUPTS()  __asm__ __volatile__("NOP \n\t" "EINT \n\t" "NOP")

#define VAR_CRITICAL()       __istate_t __istate_val

#define ENTER_CRITICAL() \
    __asm__ __volatile__( \
        "MOV R2, %0 \n\t" \
        "DINT \n\t" \
        "NOP" \
        :"=r" (__istate_val) \
        : \
    )

#define EXIT_CRITICAL() \
    __asm__ __volatile__( \
        "MOV %0, R2 \n\t" \
        "NOP" \
        : \
        : "r" (__istate_val) \
    )

#define YIELD()              __asm__ __volatile__("NOP")

/*******************************************************************************
 Serial.c
 ******************************************************************************/

#define SERIAL_CONF(A,B) ((A)|((B)<<8UL))
#define SERIAL_8N1 SERIAL_CONF(0U, UCSSEL1 | UCSWRST)
#define SERIAL_8E1 SERIAL_CONF(UCPEN | UCPAR, UCSSEL1 | UCSWRST)
#define SERIAL_8O1 SERIAL_CONF(UCPEN, UCSSEL1 | UCSWRST)
#define SERIAL_8N2 SERIAL_CONF(UCSPB, UCSSEL1 | UCSWRST)
#define SERIAL_8E2 SERIAL_CONF(UCSPB | UCPEN | UCPAR, UCSSEL1 | UCSWRST)
#define SERIAL_8O2 SERIAL_CONF(UCSPB | UCPEN, UCSSEL1 | UCSWRST)
#define SERIAL_7N1 SERIAL_CONF(UC7BIT, UCSSEL1 | UCSWRST)
#define SERIAL_7E1 SERIAL_CONF(UC7BIT | UCPEN | UCPAR, UCSSEL1 | UCSWRST)
#define SERIAL_7O1 SERIAL_CONF(UC7BIT | UCPEN, UCSSEL1 | UCSWRST)
#define SERIAL_7N2 SERIAL_CONF(UCSPB | UC7BIT, UCSSEL1 | UCSWRST)
#define SERIAL_7E2 SERIAL_CONF(UCSPB | UC7BIT | UCPEN | UCPAR, UCSSEL1 | UCSWRST)
#define SERIAL_7O2 SERIAL_CONF(UCSPB | UC7BIT | UCPEN, UCSSEL1 | UCSWRST)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_PORT_H__ */
