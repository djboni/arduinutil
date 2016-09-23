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
#include "Data/queue.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdarg.h>

#if (SERIAL3_ENABLE != 0)

static struct Queue_t RxBuff;
static uint8_t RxBuff_data[SERIAL3_RBUFSZ];

static struct Queue_t TxBuff;
static uint8_t TxBuff_data[SERIAL3_TBUFSZ];

void Serial3_begin(uint32_t speed, uint32_t config)
{
    uint8_t ucsra;
    uint32_t ubrr;

    ubrr = (F_CPU - 4U * speed) / (8U * speed);
    ucsra = config;

    if(ubrr > 0x0FFFU)
    {
        ubrr = (F_CPU - 8U * speed) / (16U * speed);
        ucsra &= ~(1U << U2X3);
    }
    else
    {
        ucsra |= (1U << U2X3);
    }

    PRR1 &= ~(1U << PRUSART3); /* Enable UART clock. */

    UCSR3B = 0; /* Disable TX and RX. */

    Queue_init(&RxBuff, &RxBuff_data[0], sizeof(RxBuff_data), sizeof(RxBuff_data[0]));
    Queue_init(&TxBuff, &TxBuff_data[0], sizeof(TxBuff_data), sizeof(TxBuff_data[0]));

    /* Set speed and other configurations. */
    UBRR3 = ubrr;
    UCSR3A = ucsra;
    UCSR3B = config >> 8U;
    UCSR3C = config >> 16U;
}

void Serial3_end(void)
{
    UCSR3B = 0U; /* Disable TX and RX. */
    PRR1 |= (1U << PRUSART3); /* Disable UART clock. */
}

Size_t Serial3_available(void)
{
    return Queue_used(&RxBuff);
}

void Serial3_flush(void)
{
    while(Queue_used(&TxBuff) != 0U)
    {
        WAIT();
    }
}

void Serial3_writeByte(uint8_t data)
{
    VAR_CRITICAL();

    ENTER_CRITICAL();

    if(     (UCSR3A & (1U << UDRE3)) &&
            Queue_empty(&TxBuff))
    {
        UDR3 = data;
    }
    else
    {
        UCSR3B |= (1U << UDRIE3);
        while(!Queue_pushback(&TxBuff, &data))
        {
            EXIT_CRITICAL();

            WAIT();

            ENTER_CRITICAL();
        }
    }

    EXIT_CRITICAL();
}

void Serial3_write(const void *str)
{
    const uint8_t *s = (const uint8_t *)str;
    while(*s != 0U)
        Serial3_writeByte(*s++);
}

void Serial3_writeBuff(const void *buff, uint16_t length)
{
    const uint8_t *b = (const uint8_t *)buff;
    while(length-- != 0)
        Serial3_writeByte(*b++);
}

void Serial3_print(const void *format, ...)
{
    char buf[SERIAL_PRINT_BUFSZ];
    {
        va_list vl;
        va_start(vl, format);
        vsnprintf(buf, SERIAL3_PRINT_BUFSZ, format, vl);
        va_end(vl);
    }
    Serial3_write(buf);
}

int16_t Serial3_read(void)
{
    uint8_t data;
    if(Queue_popfront(&RxBuff, &data))
        return data;
    else
        return -1;
}

ISR(USART3_RX_vect)
{
    uint8_t data = UDR3;
    Queue_pushback(&RxBuff, &data);
}

ISR(USART3_UDRE_vect)
{
    uint8_t data;
    if(Queue_popfront(&TxBuff, &data))
        UDR3 = data;
    else
        UCSR3B &= ~(1U << UDRIE3);
}

ISR(USART3_TX_vect)
{
}

#endif /* SERIAL3_ENABLE */
