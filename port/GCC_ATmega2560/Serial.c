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

#if (SERIAL_ENABLE != 0)

static uint8_t RxBuff_data[SERIAL_RBUFSZ];
static uint8_t TxBuff_data[SERIAL_TBUFSZ];
static struct Queue_t RxBuff = QUEUE_INIT(&RxBuff_data[0], sizeof(RxBuff_data), 1U);
static struct Queue_t TxBuff = QUEUE_INIT(&TxBuff_data[0], sizeof(TxBuff_data), 1U);

void Serial_begin(uint32_t speed, uint32_t config)
{
    uint8_t ucsra;
    uint32_t ubrr;

    ucsra = config;

    if((speed & SERIAL_CUSTOM_UBRR_MASK) == 0UL)
    {
        ubrr = (F_CPU - 4U * speed) / (8U * speed);

        if(ubrr > 0x0FFFU)
        {
            ubrr = (F_CPU - 8U * speed) / (16U * speed);
            ucsra &= ~(1U << U2X0);
        }
        else
        {
            ucsra |= (1U << U2X0);
        }
    }
    else
    {
        ubrr = speed & ~SERIAL_CUSTOM_UBRR_MASK;

        if((speed & SERIAL_CUSTOM_UBRR_DIV16) == 0UL)
        {
            ucsra |= (1U << U2X0);
        }
        else if((speed & SERIAL_CUSTOM_UBRR_DIV8) == 0UL)
        {
            ucsra &= ~(1U << U2X0);
        }
        else
        {
            ASSERT(0); /* Invalid custom speed option. */
        }
    }

    PRR0 &= ~(1U << PRUSART0); /* Enable UART clock. */

    UCSR0B = 0; /* Disable TX and RX. */

    Queue_clear(&RxBuff);
    Queue_clear(&TxBuff);

    /* Set speed and other configurations. */
    UBRR0 = ubrr;
    UCSR0A = ucsra;
    UCSR0B = config >> 8U;
    UCSR0C = config >> 16U;
}

void Serial_end(void)
{
    UCSR0B = 0U; /* Disable TX and RX. */
    PRR0 |= (1U << PRUSART0); /* Disable UART clock. */
}

Size_t Serial_available(void)
{
    return Queue_used(&RxBuff);
}

void Serial_flush(void)
{
    while(Queue_used(&TxBuff) != 0U)
    {
        WAIT_INT();
    }
}

void Serial_writeByte(uint8_t data)
{
    VAR_CRITICAL();

    ENTER_CRITICAL();

    if(     (UCSR0A & (1U << UDRE0)) &&
            Queue_empty(&TxBuff))
    {
        UDR0 = data;
    }
    else
    {
        UCSR0B |= (1U << UDRIE0);
        while(!Queue_pushback(&TxBuff, &data))
        {
            EXIT_CRITICAL();

            WAIT_INT();

            ENTER_CRITICAL();
        }
    }

    EXIT_CRITICAL();
}

void Serial_write(const void *str)
{
    const uint8_t *s = (const uint8_t *)str;
    while(*s != 0U)
        Serial_writeByte(*s++);
}

void Serial_writeBuff(const void *buff, uint16_t length)
{
    const uint8_t *b = (const uint8_t *)buff;
    while(length-- != 0)
        Serial_writeByte(*b++);
}

int Serial_print(const char *format, ...)
{
    int used_length;
    char buf[SERIAL_PRINT_BUFSZ];
    {
        va_list vl;
        va_start(vl, format);
        used_length = vsnprintf(buf, SERIAL_PRINT_BUFSZ, format, vl);
        va_end(vl);
    }
    Serial_write(buf);
    return used_length;
}

int16_t Serial_read(void)
{
    uint8_t data;
    if(Queue_popfront(&RxBuff, &data))
        return data;
    else
        return -1;
}

ISR(USART0_RX_vect)
{
    uint8_t data = UDR0;
    Queue_pushback(&RxBuff, &data);
}

ISR(USART0_UDRE_vect)
{
    uint8_t data;
    if(Queue_popfront(&TxBuff, &data))
        UDR0 = data;
    else
        UCSR0B &= ~(1U << UDRIE0);
}

#endif /* SERIAL_ENABLE */
