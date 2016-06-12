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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduinutil.h"
#include "Data/cCircular.h"
#include <stdio.h>
#include <stdarg.h>

PROGMEM static const char msg_overflow[] = "Overflow!\n";

static struct cCircular RxBuff;
uint8_t RxBuff_data[SERIAL_RBUFSZ];

static struct cCircular TxBuff;
uint8_t TxBuff_data[SERIAL_TBUFSZ];

static void USART0_begin(uint32_t speed, uint32_t config)
{
    uint8_t ucsra;
    uint32_t ubrr;

    ubrr = (F_CPU - 4U * speed) / (8U * speed);
    ucsra = config;

    if(ubrr > 0x0FFFU)
    {
        ubrr = (F_CPU - 8U * speed) / (16U * speed);
        ucsra &= ~(1U << U2X0);
    }
    else
    {
        ucsra |= (1U << U2X0);
    }

    PRR0 &= ~(1U << PRUSART0); /* Enable UART clock. */

    UCSR0B = 0; /* Disable TX and RX. */

    cCircular_init(&RxBuff, &RxBuff_data[0], sizeof(RxBuff_data));
    cCircular_init(&TxBuff, &TxBuff_data[0], sizeof(TxBuff_data));

    /* Set speed and other configurations. */
    UBRR0 = ubrr;
    UCSR0A = ucsra;
    UCSR0B = config >> 8U;
    UCSR0C = config >> 16U;
}

static void USART0_end(void)
{
    UCSR0B = 0; /* Disable TX and RX. */
    PRR0 |= (1 << PRUSART0); /* Disable UART clock. */
}

static Size_t USART0_available(void)
{
    return cCircular_used(&RxBuff);
}

static void USART0_flush(void)
{
    while(cCircular_used(&TxBuff) != 0U)
    {
        WAIT();
    }
}

static void USART0_writeByte(uint8_t data)
{
    if((UCSR0A & (1U << UDRE0)))
    {
        UDR0 = data;
    }
    else
    {
        UCSR0B |= (1U << UDRIE0);
        while(!cCircular_pushback(&TxBuff, data))
        {
            WAIT();
        }
    }
}

static void USART0_write(const void *str)
{
    const uint8_t *s = (const uint8_t *)str;
    while(*s != 0U)
        USART0_writeByte(*s++);
}

static void USART0_writeBuff(const void *buff, uint16_t length)
{
    const uint8_t *b = (const uint8_t *)buff;
    while(length-- != 0)
        USART0_writeByte(*b++);
}

static void USART0_print(const void *format, ...)
{
    char buf[SERIAL_PRINT_BUFSZ];
    {
        va_list vl;
        va_start(vl, format);
        if(vsnprintf(buf, SERIAL_PRINT_BUFSZ, format, vl) >=
                (int)SERIAL_PRINT_BUFSZ)
            strncpy_P(buf, msg_overflow, SERIAL_PRINT_BUFSZ);
        va_end(vl);
    }
    USART0_write(buf);
}

static int16_t USART0_read(void)
{
    uint8_t data;
    if(cCircular_popfront(&RxBuff, &data))
        return data;
    else
        return -1;
}

ISR(USART0_RX_vect)
{
    cCircular_pushback(&RxBuff, UDR0);
}

ISR(USART0_UDRE_vect)
{
    uint8_t data;
    if(cCircular_popfront(&TxBuff, &data))
    UDR0 = data;
    else
    UCSR0B &= ~(1U << UDRIE0);
}

ISR(USART0_TX_vect)
{
}

const struct USART0_Serial Serial =
{
    USART0_begin,
    USART0_end,
    USART0_writeByte,
    USART0_write,
    USART0_writeBuff,
    USART0_read,
    USART0_available,
    USART0_flush,
    USART0_print
};
