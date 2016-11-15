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
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define SET_BITS(reg, bits, cast)   do{reg=(cast)((reg)|(bits));}while(0U)
#define CLEAR_BITS(reg, bits, cast) do{reg=(cast)((reg)&(~(bits)));}while(0U)

PROGMEM static volatile uint8_t * const portIOLIST[] = {
        /* 0 - 13 */
        &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTD, &PORTB,
        &PORTB, &PORTB, &PORTB, &PORTB, &PORTB,
        /* A0 - A5 */
        &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC
};

PROGMEM static volatile uint8_t * const pinIOLIST[] = {
        /* 0 - 13 */
        &PIND, &PIND, &PIND, &PIND, &PIND, &PIND, &PIND, &PIND, &PINB, &PINB,
        &PINB, &PINB, &PINB, &PINB,
        /* A0 - A5 */
        &PINC, &PINC, &PINC, &PINC, &PINC, &PINC
};

PROGMEM static volatile uint8_t * const ddrIOLIST[] = {
        /* 0 - 13 */
        &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRD, &DDRB, &DDRB,
        &DDRB, &DDRB, &DDRB, &DDRB,
        /* A0 - A5 */
        &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC
};

PROGMEM static const uint8_t bitIOLIST[] = {
        /* 0 - 13 */
        0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 0U, 1U, 2U, 3U, 4U, 5U,
        /* A0 - A5 */
        0U, 1U, 2U, 3U, 4U, 5U
};

/** Change pin configuration. The modes are INPUT, OUTPUT and INPUT_PULLUP.

 Note: Use pin ANALOGIO+X for analog pin X. */
void pinMode(uint8_t io, uint8_t mode)
{
    volatile uint8_t *port = (volatile uint8_t *)pgm_read_word(&portIOLIST[io]);
    volatile uint8_t *ddr = (volatile uint8_t *)pgm_read_word(&ddrIOLIST[io]);
    uint8_t bit = pgm_read_byte(&bitIOLIST[io]);

    ASSERT(io < MAXIO);

    switch(mode)
    {
    case INPUT:
        /* Input. */
        CLEAR_BITS(*ddr, (1U << bit), uint8_t);
        /* Disable pull-up. */
        CLEAR_BITS(*port, (1U << bit), uint8_t);
        break;
    case OUTPUT:
        /* Output. */
        SET_BITS(*ddr, (1U << bit), uint8_t);
        break;
    case INPUT_PULLUP:
        /* Input. */
        CLEAR_BITS(*ddr, (1U << bit), uint8_t);
        /* Enable pull-up. */
        SET_BITS(*port, (1U << bit), uint8_t);
        break;
    default:
        ASSERT(0); /* Invalid pin mode. */
        break;
    }
}

/** Change pin output value or input pull-up. The values are LOW and HIGH.

 Note: Use pin ANALOGIO+X for analog pin X. */
void digitalWrite(uint8_t io, uint8_t value)
{
    volatile uint8_t *port = (volatile uint8_t *)pgm_read_word(&portIOLIST[io]);
    uint8_t bit = pgm_read_byte(&bitIOLIST[io]);

    ASSERT(io < MAXIO);

    switch(value)
    {
    case LOW:
        CLEAR_BITS(*port, (1U << bit), uint8_t);
        break;
    default:
        SET_BITS(*port, (1U << bit), uint8_t);
        break;
    }
}

/** Read pin input value. The values returned are LOW and HIGH.

 Note: Use pin ANALOGIO+X for analog pin X. */
uint8_t digitalRead(uint8_t io)
{
    volatile uint8_t *pin = (volatile uint8_t *)pgm_read_word(&pinIOLIST[io]);
    uint8_t bit = pgm_read_byte(&bitIOLIST[io]);

    ASSERT(io < MAXIO);

    return (*pin & (1U << bit)) != 0U;
}

#if (DIGITAL_EXTERNAL_INT_ENABLE != 0)

static volatile uint8_t * convIoToInterruptRegister(uint8_t io)
{
    volatile uint8_t *reg;
    void *port = (void *)pgm_read_word(&portIOLIST[io]);

    ASSERT(io < MAXIO);

    if(port == &PORTB)
    {
        reg = &PCMSK0;
    }
    else if(port == &PORTC)
    {
        reg = &PCMSK1;
    }
    else if(port == &PORTD)
    {
        reg = &PCMSK2;
    }
    else
    {
        ASSERT(0); /* Invalid port. */
    }

    return reg;
}

/** Enable external interrupt.

 The processor interrupts whenever any of the enabled pins state changes.
 The 3 available interrupts vectors are shared within each port. Therefore
 application must check by software which pin changed and to what state.

 Pins 0-7 (PORTD) use the interrupt vector PCINT2_vect.
 Pins 8-13 (PORTB) use the interrupt vector PCINT0_vect.
 Pins A0-A5 (PORTC) use the interrupt vector PCINT1_vect.

 *  // SETUP
 *  digitalWrite(13, digitalRead(8));
 *  enableExternalInterrupt(8);
 *
 *  // PORTB ISR
 *  ISR(PCINT0_vect) {
 *      static uint8_t port_last;
 *
 *      uint8_t port_now = PINB;
 *      uint8_t port_changed = port_last ^ port_now;
 *      port_last = port_now;
 *
 *      if(port_changed & (1 << PINB0)) {
 *          if(port_now & (1 << PINB0)) {
 *              // Rising edge
 *          }
 *          else {
 *              // Falling edge
 *          }
 *      }
 *
 *      // Check other pins
 *  }
 */
void enableExternalInterrupt(uint8_t io)
{
    volatile uint8_t *reg = convIoToInterruptRegister(io);
    uint8_t bit = pgm_read_byte(&bitIOLIST[io]);
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        *reg |= (1U << bit);
    }
    CRITICAL_EXIT();
}

/** Disable external interrupt. */
void disableExternalInterrupt(uint8_t io)
{
    volatile uint8_t *reg = convIoToInterruptRegister(io);
    uint8_t bit = pgm_read_byte(&bitIOLIST[io]);
    CRITICAL_VAL();

    CRITICAL_ENTER();
    {
        *reg &= ~(1U << bit);
    }
    CRITICAL_EXIT();
}

#endif /* DIGITAL_EXTERNAL_INT_ENABLE */

#if (DIGITAL_ATTACH_INT_ENABLE != 0)

static void (*extIntVector[2U])(void) = {0};

/** Convert a digital pin to a interrupt number.

 In this implementation it does nothing other than avoid breaking people code.
 */
uint8_t digitalPinToInterrupt(uint8_t pin)
{
    return pin;
}

/** Enable external interrupt.

 Only pins 2 and 3 are able to use this function.
 isr is a pointer to the function to be called when the interrupt fires.
 mode is the interrupt mode. See enum DigitalInterruptModes. */
void attachInterrupt(uint8_t pin, void (*isr)(void), uint8_t mode)
{
    mode &= 0x03U;

    switch(pin) {
    case 2U:
    {
        EIMSK &= ~(1U << INT0);
        EIFR = (1U << INTF0);
        EICRA = (EICRA & ~(0x03U << ISC00)) | (mode << ISC00);
        extIntVector[0U] = isr;
        EIMSK |= (1U << INT0);
        break;
    }
    case 3U:
    {
        EIMSK &= ~(1U << INT1);
        EIFR = (1U << INTF1);
        EICRA = (EICRA & ~(0x03U << ISC10)) | (mode << ISC10);
        extIntVector[1U] = isr;
        EIMSK |= (1U << INT1);
        break;
    }
    default:
        ASSERT(0); /* Invalid interrupt pin. */
    }
}

/** Disable external interrupt. */
void detachInterrupt(uint8_t pin)
{
    switch(pin) {
    case 2U:
    {
        EIMSK &= ~(1U << INT0);
        break;
    }
    case 3U:
    {
        EIMSK &= ~(1U << INT1);
        break;
    }
    default:
        ASSERT(0); /* Invalid interrupt pin. */
    }
}

ISR(INT0_vect)
{
    extIntVector[0U]();
}

ISR(INT1_vect)
{
    extIntVector[1U]();
}

#endif /* DIGITAL_ATTACH_INT_ENABLE */
