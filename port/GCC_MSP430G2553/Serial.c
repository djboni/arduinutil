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
#include <stdio.h>
#include <stdarg.h>

#if (SERIAL_ENABLE != 0)

static uint8_t RxBuff_data[SERIAL_RBUFSZ];
static uint8_t TxBuff_data[SERIAL_TBUFSZ];
static struct Queue_t RxBuff;
static struct Queue_t TxBuff;

void Serial_begin(uint32_t speed, uint32_t config)
{
    uint8_t mctl;
    uint32_t br3;
    uint16_t br;

    br3 = (F_CPU * 8UL + speed / 2U) / speed;
    br = (br3 >> 3U);
    mctl = (br3 & 0x07U) << 1U;

    Queue_init(&RxBuff, &RxBuff_data, sizeof(RxBuff_data), 1);
    Queue_init(&TxBuff, &TxBuff_data, sizeof(TxBuff_data), 1);

    /* Configure TX and RX pins. */
    P1REN &= ~(BIT1 | BIT2);
    P1DIR &= ~(BIT1 | BIT2);
    P1OUT |= (BIT1 | BIT2);

    P1SEL |= (BIT1 | BIT2);
    P1SEL2 |= (BIT1 | BIT2);

    IE2 &= ~(UCA0TXIE | UCA0RXIE);  /* Disable RX and TX interrupts. */

    UCA0CTL1 = UCSWRST; /* Put USI in reset mode. */

    /* Set speed and other configurations. */
    UCA0CTL1 |= config >> 8U;
    UCA0CTL0 = config;

    UCA0BR0 = br & 0xFFU;
    UCA0BR1 = br >> 8U;
    UCA0MCTL = mctl;

    UCA0CTL1 &= ~UCSWRST; /* Take USI out of reset. */

    IE2 |= UCA0RXIE; /* Enable RX interrupt. */
}

void Serial_end(void)
{
    IE2 &= ~(UCA0TXIE | UCA0RXIE);  /* Disable RX and TX interrupts. */

    UCA0CTL1 = UCSWRST; /* Put USI in reset mode. */
}

Size_t Serial_available(void)
{
    return Queue_used(&RxBuff);
}

void Serial_flush(void)
{
    while(Queue_used(&TxBuff) != 0U)
    {
        YIELD();
    }
}

void Serial_writeByte(uint8_t data)
{
    CRITICAL_VAL();

    CRITICAL_ENTER();

    if(     (IFG2 & UCA0TXIFG) &&
            Queue_empty(&TxBuff))
    {
        UCA0TXBUF = data;
    }
    else
    {
        IE2 |= UCA0TXIE; /* Enable TX interrupt */
        while(!Queue_pushback(&TxBuff, &data))
        {
            CRITICAL_EXIT();

            YIELD();

            CRITICAL_ENTER();
        }
    }

    CRITICAL_EXIT();
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

int16_t Serial_read(void)
{
    uint8_t data;
    if(Queue_popfront(&RxBuff, &data))
        return data;
    else
        return -1;
}

__attribute__((interrupt(USCIAB0RX_VECTOR)))
void usci0rx_isr(void)
{
    uint8_t data = UCA0RXBUF;
    Queue_pushback(&RxBuff, &data);
}

__attribute__((interrupt(USCIAB0TX_VECTOR)))
void usci0tx_isr(void)
{
    uint8_t data;
    if(Queue_popfront(&TxBuff, &data))
        UCA0TXBUF = data;
    else
        IE2 &= ~UCA0TXIE; /* Disable TX interrupt. */
}

#endif /* SERIAL_ENABLE */

