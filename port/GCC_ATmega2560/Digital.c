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
        &PORTE, &PORTE, &PORTE, &PORTE, &PORTG, &PORTE, &PORTH, &PORTH, &PORTH,
        &PORTH, &PORTB, &PORTB, &PORTB, &PORTB,
        /* 14 - 21 */
        &PORTJ, &PORTJ, &PORTH, &PORTH, &PORTD, &PORTD, &PORTD, &PORTD,
        /* 22 - 37 */
        &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTC,
        &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC,
        /* 38 - 49 */
        &PORTD, &PORTG, &PORTG, &PORTG, &PORTL, &PORTL, &PORTL, &PORTL, &PORTL,
        &PORTL, &PORTL, &PORTL,
        /* 50 - 53 */
        &PORTB, &PORTB, &PORTB, &PORTB,
        /* A0 - A15 */
        &PORTF, &PORTF, &PORTF, &PORTF, &PORTF, &PORTF, &PORTF, &PORTF, &PORTK,
        &PORTK, &PORTK, &PORTK, &PORTK, &PORTK, &PORTK, &PORTK
};

PROGMEM static volatile uint8_t * const pinIOLIST[] = {
        /* 0 - 13 */
        &PINE, &PINE, &PINE, &PINE, &PING, &PINE, &PINH, &PINH, &PINH, &PINH,
        &PINB, &PINB, &PINB, &PINB,
        /* 14 - 21 */
        &PINJ, &PINJ, &PINH, &PINH, &PIND, &PIND, &PIND, &PIND,
        /* 22 - 37 */
        &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINA, &PINC, &PINC,
        &PINC, &PINC, &PINC, &PINC, &PINC, &PINC,
        /* 38 - 49 */
        &PIND, &PING, &PING, &PING, &PINL, &PINL, &PINL, &PINL, &PINL, &PINL,
        &PINL, &PINL,
        /* 50 - 53 */
        &PINB, &PINB, &PINB, &PINB,
        /* A0 - A15 */
        &PINF, &PINF, &PINF, &PINF, &PINF, &PINF, &PINF, &PINF, &PINK, &PINK,
        &PINK, &PINK, &PINK, &PINK, &PINK, &PINK
};

PROGMEM static volatile uint8_t * const ddrIOLIST[] = {
        /* 0 - 13 */
        &DDRE, &DDRE, &DDRE, &DDRE, &DDRG, &DDRE, &DDRH, &DDRH, &DDRH, &DDRH,
        &DDRB, &DDRB, &DDRB, &DDRB,
        /* 14 - 21 */
        &DDRJ, &DDRJ, &DDRH, &DDRH, &DDRD, &DDRD, &DDRD, &DDRD,
        /* 22 - 37 */
        &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRA, &DDRC, &DDRC,
        &DDRC, &DDRC, &DDRC, &DDRC, &DDRC, &DDRC,
        /* 38 - 49 */
        &DDRD, &DDRG, &DDRG, &DDRG, &DDRL, &DDRL, &DDRL, &DDRL, &DDRL, &DDRL,
        &DDRL, &DDRL,
        /* 50 - 53 */
        &DDRB, &DDRB, &DDRB, &DDRB,
        /* A0 - A15 */
        &DDRF, &DDRF, &DDRF, &DDRF, &DDRF, &DDRF, &DDRF, &DDRF, &DDRK, &DDRK,
        &DDRK, &DDRK, &DDRK, &DDRK, &DDRK, &DDRK
};

PROGMEM static const uint8_t bitIOLIST[] = {
        /* 0 - 13 */
        0U, 1U, 4U, 5U, 5U, 3U, 3U, 4U, 5U, 6U, 4U, 5U, 6U, 7U,
        /* 14 - 21 */
        1U, 0U, 1U, 0U, 3U, 2U, 1U, 0U,
        /* 22 - 37 */
        0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 7U, 6U, 5U, 4U, 3U, 2U, 1U, 0U,
        /* 38 - 49 */
        7U, 2U, 1U, 0U, 7U, 6U, 5U, 4U, 3U, 2U, 1U, 0U,
        /* 50 - 53 */
        3U, 2U, 1U, 0U,
        /* A0 - A15 */
        0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U
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

struct InterruptRegBit_t {
    volatile uint8_t *reg;
    uint8_t bit;
};

#if (DIGITAL_EXTERNAL_INT_ENABLE != 0)

static struct InterruptRegBit_t convIoToInterruptRegister(uint8_t io)
{
    struct InterruptRegBit_t intrpt;
    void *port = (void *)pgm_read_word(&portIOLIST[io]);
    intrpt.bit = pgm_read_byte(&bitIOLIST[io]);

    ASSERT(io < MAXIO);

    if(port == &PORTB)
    {
        intrpt.reg = &PCMSK0;
    }
    else if(port == &PORTE)
    {
        intrpt.reg = &PCMSK1;
    }
    else if(port == &PORTJ)
    {
        intrpt.reg = &PCMSK1;
        intrpt.bit += 1U;
    }
    else if(port == &PORTK)
    {
        intrpt.reg = &PCMSK2;
    }
    else
    {
        ASSERT(0); /* Invalid port. */
    }

    return intrpt;
}

/** Enable external interrupt.

 The processor interrupts whenever any of the enabled pins state changes.
 The 3 available interrupts vectors are shared within each port. Therefore
 application must check by software which pin changed and to what state.

 Pins 53-50,10-13 (PORTB) use the interrupt vector PCINT0_vect.
 Pin 0(RX) (PORTE[0]) use the interrupt vector PCINT1_vect.
 Pins 15,14 (PORTJ[0-1]) use the interrupt vector PCINT1_vect.
 Unrouted pins PORTJ[2-6] use the interrupt vector PCINT1_vect.
 Pins A8-A15 (PORTK) use the interrupt vector PCINT2_vect.

 *  // SETUP
 *  digitalWrite(13, digitalRead(53));
 *  enableExternalInterrupt(53);
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
    struct InterruptRegBit_t intrpt = convIoToInterruptRegister(io);
    VAR_CRITICAL();

    ENTER_CRITICAL();
    {
        *intrpt.reg |= (1U << intrpt.bit);
    }
    EXIT_CRITICAL();
}

/** Disable external interrupt. */
void disableExternalInterrupt(uint8_t io)
{
    struct InterruptRegBit_t intrpt = convIoToInterruptRegister(io);
    VAR_CRITICAL();

    ENTER_CRITICAL();
    {
        *intrpt.reg &= ~(1U << intrpt.bit);
    }
    EXIT_CRITICAL();
}

#endif /* DIGITAL_EXTERNAL_INT_ENABLE */

#if (DIGITAL_ATTACH_INT_ENABLE != 0)

static void (*extIntVector[6U])(void) = {0};

/** Convert a digital pin to a interrupt number.

 In this implementation it does nothing other than avoid breaking people code.
 */
uint8_t digitalPinToInterrupt(uint8_t pin)
{
    return pin;
}

/** Enable external interrupt.

 Only pins 2, 3, 18, 19, 20 and 21 are able to use this function.
 isr is a pointer to the function to be called when the interrupt fires.
 mode is the interrupt mode. See enum DigitalInterruptModes. */
void attachInterrupt(uint8_t pin, void (*isr)(void), uint8_t mode)
{
    mode &= 0x03U;

    switch(pin) {
    case 2U:
    {
        EIMSK &= ~(1U << INT4);
        EIFR = (1U << INTF4);
        EICRB = (EICRA & ~(0x03U << ISC40)) | (mode << ISC40);
        extIntVector[4U] = isr;
        EIMSK |= (1U << INT4);
        break;
    }
    case 3U:
    {
        EIMSK &= ~(1U << INT5);
        EIFR = (1U << INTF5);
        EICRB = (EICRA & ~(0x03U << ISC50)) | (mode << ISC50);
        extIntVector[5U] = isr;
        EIMSK |= (1U << INT5);
        break;
    }
    case 18U:
    {
        EIMSK &= ~(1U << INT3);
        EIFR = (1U << INTF3);
        EICRA = (EICRA & ~(0x03U << ISC30)) | (mode << ISC30);
        extIntVector[3U] = isr;
        EIMSK |= (1U << INT3);
        break;
    }
    case 19U:
    {
        EIMSK &= ~(1U << INT2);
        EIFR = (1U << INTF2);
        EICRA = (EICRA & ~(0x03U << ISC20)) | (mode << ISC20);
        extIntVector[2U] = isr;
        EIMSK |= (1U << INT2);
        break;
    }
    case 20U:
    {
        EIMSK &= ~(1U << INT1);
        EIFR = (1U << INTF1);
        EICRA = (EICRA & ~(0x03U << ISC10)) | (mode << ISC10);
        extIntVector[1U] = isr;
        EIMSK |= (1U << INT1);
        break;
    }
    case 21U:
    {
        EIMSK &= ~(1U << INT0);
        EIFR = (1U << INTF0);
        EICRA = (EICRA & ~(0x03U << ISC00)) | (mode << ISC00);
        extIntVector[0U] = isr;
        EIMSK |= (1U << INT0);
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
    case 18U:
    {
        EIMSK &= ~(1U << INT2);
        break;
    }
    case 19U:
    {
        EIMSK &= ~(1U << INT3);
        break;
    }
    case 20U:
    {
        EIMSK &= ~(1U << INT4);
        break;
    }
    case 21U:
    {
        EIMSK &= ~(1U << INT5);
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

ISR(INT2_vect)
{
    extIntVector[2U]();
}

ISR(INT3_vect)
{
    extIntVector[3U]();
}

ISR(INT4_vect)
{
    extIntVector[4U]();
}

ISR(INT5_vect)
{
    extIntVector[5U]();
}

#endif /* DIGITAL_ATTACH_INT_ENABLE */
