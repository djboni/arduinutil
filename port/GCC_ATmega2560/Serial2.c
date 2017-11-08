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
#include "Config.h"
#include "Data/queue.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdarg.h>

#if (SERIAL2_ENABLE != 0)

static uint8_t RxBuff_data[SERIAL2_RBUFSZ];
static uint8_t TxBuff_data[SERIAL2_TBUFSZ];
static struct Queue_t RxBuff;
static struct Queue_t TxBuff;

void Serial2_begin(uint32_t speed, uint32_t config)
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

    PRR1 &= ~(1U << PRUSART2); /* Enable UART clock. */

    UCSR2B = 0; /* Disable TX and RX. */

    Queue_init(&RxBuff, &RxBuff_data, sizeof(RxBuff_data), 1);
    Queue_init(&TxBuff, &TxBuff_data, sizeof(TxBuff_data), 1);

    /* Set speed and other configurations. */
    UBRR2 = ubrr;
    UCSR2A = ucsra;
    UCSR2B = config >> 8U;
    UCSR2C = config >> 16U;
}

void Serial2_end(void)
{
    UCSR2B = 0U; /* Disable TX and RX. */
    PRR1 |= (1U << PRUSART2); /* Disable UART clock. */
}

Size_t Serial2_available(void)
{
    return Queue_used(&RxBuff);
}

void Serial2_flush(void)
{
    while(Queue_used(&TxBuff) != 0U)
    {
        WAIT_INT();
    }
}

void Serial2_writeByte(uint8_t data)
{
    CRITICAL_VAL();

    CRITICAL_ENTER();

    if(     (UCSR2A & (1U << UDRE2)) &&
            Queue_empty(&TxBuff))
    {
        UDR2 = data;
    }
    else
    {
        UCSR2B |= (1U << UDRIE2);
        while(!Queue_write(&TxBuff, &data))
        {
            CRITICAL_EXIT();

            WAIT_INT();

            CRITICAL_ENTER();
        }
    }

    CRITICAL_EXIT();
}

void Serial2_write(const void *str)
{
    const uint8_t *s = (const uint8_t *)str;
    while(*s != 0U)
        Serial2_writeByte(*s++);
}

void Serial2_writeBuff(const void *buff, uint16_t length)
{
    const uint8_t *b = (const uint8_t *)buff;
    while(length-- != 0)
        Serial2_writeByte(*b++);
}

int Serial2_print(const void *format, ...)
{
	int used_length;
    char buf[SERIAL_PRINT_BUFSZ];
    {
        va_list vl;
        va_start(vl, format);
        used_length = vsnprintf(buf, SERIAL2_PRINT_BUFSZ, format, vl);
        va_end(vl);
    }
    Serial2_write(buf);
    return used_length;
}

int16_t Serial2_read(void)
{
    uint8_t data;
    if(Queue_read(&RxBuff, &data))
        return data;
    else
        return -1;
}

ISR(USART2_RX_vect)
{
    uint8_t data = UDR2;
    Queue_write(&RxBuff, &data);
}

ISR(USART2_UDRE_vect)
{
    uint8_t data;
    if(Queue_read(&TxBuff, &data))
        UDR2 = data;
    else
        UCSR2B &= ~(1U << UDRIE2);
}

#endif /* SERIAL2_ENABLE */
