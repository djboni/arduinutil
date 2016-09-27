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
#include <avr/interrupt.h>

#if (I2C_ENABLE != 0)

#define I2C_RETURN_IF_NOTOK(expr) do{  \
    uint8_t x;                         \
    x = expr;                          \
    if(x != I2C_OK)                    \
        return x;                      }while(0U)

static uint8_t waitIntStatus(uint8_t a)
{
    uint8_t twsr;
    while(!(TWCR & (1U << TWINT))) {}
    twsr = TWSR & 0xF8U;
    return (twsr == a) ? I2C_OK : twsr;
}

static uint8_t waitIntStatus2(uint8_t a, uint8_t b)
{
    uint8_t twsr;
    while(!(TWCR & (1U << TWINT))) {}
    twsr = TWSR & 0xF8U;
    return (twsr == a || twsr == b) ? I2C_OK : twsr;
}

void I2c_begin(uint32_t speed)
{
    uint8_t twsr_twps_bits;
    uint8_t twbr_val;

    switch(I2C_PRESCALER) {
    case 1U:
        twsr_twps_bits = 0U;
        break;
    case 4U:
        twsr_twps_bits = 1U;
        break;
    case 16U:
        twsr_twps_bits = 2U;
        break;
    case 64U:
        twsr_twps_bits = 3U;
        break;
    default:
        twsr_twps_bits = 3U;
        ASSERT(0); /* Invalid prescaler value */
    }

    twbr_val = (F_CPU - 16UL * speed) / (2 * I2C_PRESCALER * speed);

    PRR &= ~(1U << PRTWI); /* Enable I2C clock. */

    TWSR = (twsr_twps_bits << TWPS0); /* Prescaler. */
    TWBR = twbr_val; /* Bit rate. */

    TWCR =  (1U << TWINT) | /* Clear interrupt flag. */
            (1U << TWEN); /* Enable I2C. */
}

void I2c_end(void)
{
    TWCR = 0U; /* Disable I2C. */
    PRR |= (1U << PRTWI); /* Disable I2C clock. */
}

uint8_t I2c_sendStart(uint8_t addr, uint8_t r1w0)
{
    r1w0 = ((r1w0 == 0U) ? 0U : 1U); /* To bool 0 or 1. */
    addr = (addr << 1U) | r1w0;

    /* Send STA. */
    TWCR |= (1U << TWSTA);

    I2C_RETURN_IF_NOTOK(waitIntStatus2(I2C_START, I2C_RSTART));

    /* Send SLA+W or SLA+R. */
    TWDR = addr;

    /* Clear STA and TWINT bits. */
    TWCR = (TWCR & ~(1U << TWSTA)) | (1U << TWINT);

    if(r1w0)
    {
        I2C_RETURN_IF_NOTOK(waitIntStatus(I2C_SLAW_ACK));
    }
    else
    {
        I2C_RETURN_IF_NOTOK(waitIntStatus(I2C_SLAR_ACK));
    }

    return I2C_OK;
}

void I2c_sendStop(void)
{
    TWCR |= (1U << TWSTO); /* Send STO. Auto-clears. */
}

uint8_t I2c_writeByte(uint8_t data)
{
    /* Send data. */
    TWDR = data;

    /* Clear TWINT. */
    TWCR |= (1U << TWINT);

    I2C_RETURN_IF_NOTOK(waitIntStatus(I2C_WDATA_ACK));

    return I2C_OK;
}

uint8_t I2c_readByte(uint8_t *data, uint8_t ack1nack0)
{
    if(ack1nack0 != 0U)
    {
        /* Send ACK. Clear TWINT. */
        TWCR |= (1U << TWEA) | (1U << TWINT);
    }
    else
    {
        /* Send NACK. Clear TWINT. */
        TWCR = (TWCR & ~(1U << TWEA)) | (1U << TWINT);
    }
    I2C_RETURN_IF_NOTOK(waitIntStatus2(I2C_RDATA_NACK, I2C_RDATA_ACK));

    *data = TWDR;

    return I2C_OK;
}

ISR(TWI_vect)
{
}

#endif /* I2C_ENABLE */
