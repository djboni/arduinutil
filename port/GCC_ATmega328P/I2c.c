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

/* These are values written to the TWCR control register. */
enum I2c_twcr_values_t {
    I2C_TWCR_ENABLE = (1U << TWEN), /* Enable I2C. */
    I2C_TWCR_DISABLE = 0U, /* Disable I2C. */

    I2C_TWCR_CLEAR_INT = (1U << TWINT) | (1U << TWIE) | I2C_TWCR_ENABLE, /* Enable interrupt. Clear interrupt flag. */
    I2C_TWCR_DIS_INT = I2C_TWCR_ENABLE, /* Disable interrupt. Do not clear interrupt. */

    I2C_TWCR_SEND_STA = (1U << TWSTA) | I2C_TWCR_CLEAR_INT, /* Send STA. */
    I2C_TWCR_CLEAR_STA = I2C_TWCR_CLEAR_INT, /* Clear STA bit. */
    I2C_TWCR_SEND_STO = (1U << TWINT) | (1U << TWSTO) | I2C_TWCR_DIS_INT, /* Send STO. Clear and disable interrupt. */

    I2C_TWCR_SEND_ACK = (1U << TWEA) | I2C_TWCR_CLEAR_INT, /* Send ACK. */
    I2C_TWCR_SEND_NACK = I2C_TWCR_CLEAR_INT /* Send NACK */
};

/* These are the used status from the TWSR status register. */
enum I2c_twsr_status_t {
    I2C_OK             = 0x00U,
    I2C_START          = 0x08U,
    I2C_RESTART        = 0x10U,
    I2C_SLAW_ACK       = 0x18U,
    I2C_SLAW_NACK      = 0x20U,
    I2C_WDATA_ACK      = 0x28U,
    I2C_WDATA_NACK     = 0x30U,
    I2C_ARBITRATION    = 0x38U,
    I2C_SLAR_ACK       = 0x40U,
    I2C_SLAR_NACK      = 0x48U,
    I2C_RDATA_ACK      = 0x50U,
    I2C_RDATA_NACK     = 0x58U
};

/* These are the status returned by I2c_getStatus() function. Zero means the I2C
 peripheral is IDLE and non-zero means it is busy. */
enum I2c_interrupt_status_t {
    IDLE = 0U,
    STA_SLAW = 1U,
    SLAW = 2U,
    WDATA = 3U,
    STA_SLAR = 4U,
    SLAR = 5U,
    RDATA = 6U
};

/* Data structure which holds I2C peripheral status and communication data. */
struct I2c_buffer_t {
    uint8_t status;
    uint8_t addr;
    uint8_t length;
    uint8_t *num;
    const uint8_t *txbuff;
    uint8_t *rxbuff;
};

static struct I2c_buffer_t RxTxBuffer;

/** Initialize peripheral for communication.

 @param speed Maximum speed the interface is allowed to use.
 */
void I2c_begin(uint32_t speed)
{
    uint8_t twsr_twps_bits;
    uint32_t twbr_val;
    uint32_t twbr_divider;

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
        ASSERT(0); /* Invalid prescaler value. */
    }

    /* Ceiling of twbr_val. Avoids higher speeds than the 'speed' parameter. */
    twbr_divider = 2 * I2C_PRESCALER * speed;
    twbr_val = (F_CPU - 16UL * speed + twbr_divider - 1UL) / twbr_divider;
    ASSERT(twbr_val <= 0xFFU); /* Prescaler and/or speed too low. */

    PRR &= ~(1U << PRTWI); /* Enable I2C clock. */

    TWSR = (twsr_twps_bits << TWPS0); /* Prescaler. */
    TWBR = (uint8_t)twbr_val; /* Bit rate. */

    TWCR = I2C_TWCR_ENABLE;

    RxTxBuffer.status = IDLE;
}

/** Disable peripheral. */
void I2c_end(void)
{
    TWCR = I2C_TWCR_DISABLE;
    PRR |= (1U << PRTWI); /* Disable I2C clock. */
}

/** Return peripheral status. Zero means IDLE, busy otherwise. */
uint8_t I2c_getStatus(void)
{
    return RxTxBuffer.status;
}

/** Send stop condition in the bus. */
void I2c_stop(void)
{
    TWCR = I2C_TWCR_SEND_STO; /* Send STO. STO bit auto-clears. */
}

/** Send start condition in the bus and send (write) up to 'length' bytes from
 'buff' to the device of address 'addr'. The value pointed by 'numsent' holds
 the actual number of bytes sent. The function returns immediately after
 starting the communication. To test if the communication has finished use the
 function I2c_getStatus(). A stop condition is not sent by this function. */
void I2c_write(uint8_t addr, const uint8_t *buff, uint8_t length, uint8_t *numsent)
{
    CRITICAL_VAL();
    CRITICAL_ENTER();
    {
        if(RxTxBuffer.status == IDLE)
        {
            RxTxBuffer.status = STA_SLAW;
            CRITICAL_EXIT();

            *numsent = 0U;

            RxTxBuffer.addr = (addr << 1U);
            RxTxBuffer.txbuff = buff;
            RxTxBuffer.length = length;
            RxTxBuffer.num = numsent;

            TWCR = I2C_TWCR_SEND_STA; /* Send STA. */
        }
        else
        {
            CRITICAL_EXIT();
        }
    }
}

/** Send start condition in the bus and receive (read) up to 'length' bytes in
 'buff' from the device of address 'addr'. The value pointed by 'numread' holds
 the actual number of bytes received. The function returns immediately after
 starting the communication. To test if the communication has finished use the
 function I2c_getStatus(). A stop condition is not sent by this function. */
void I2c_read(uint8_t addr, uint8_t *buff, uint8_t length, uint8_t *numread)
{
    CRITICAL_VAL();
    CRITICAL_ENTER();
    {
        if(RxTxBuffer.status == IDLE)
        {
            RxTxBuffer.status = STA_SLAR;
            CRITICAL_EXIT();

            *numread = 0U;

            RxTxBuffer.addr = (addr << 1U) | 1U;
            RxTxBuffer.rxbuff = buff;
            RxTxBuffer.length = length;
            RxTxBuffer.num = numread;

            TWCR = I2C_TWCR_SEND_STA; /* Send STA. */
        }
        else
        {
            CRITICAL_EXIT();
        }
    }
}

/* Interrupt vector for I2C peripheral. */
ISR(TWI_vect)
{
    uint8_t twsr = TWSR & 0xF8U;

    switch(RxTxBuffer.status)
    {
        case STA_SLAW:
        {
            if(twsr == I2C_START || twsr == I2C_RESTART)
            {
                RxTxBuffer.status = SLAW;
                TWDR = RxTxBuffer.addr; /* Send SLA+W. */
                TWCR = I2C_TWCR_CLEAR_STA; /* Clear STA bit. */
            }
            else
            {
                goto err;
            }
            break;
        }
        case SLAW:
        {
            if(twsr == I2C_SLAW_ACK)
            {
                RxTxBuffer.status = WDATA;
                twsr = I2C_WDATA_ACK;
            }
            else
            {
                goto err;
            }
            /* No break! */
        }
        case WDATA:
        {
            if(twsr == I2C_WDATA_ACK)
            {
                if(RxTxBuffer.length != 0U)
                {
                    TWDR = *(RxTxBuffer.txbuff); /* Send data. */
                    TWCR = I2C_TWCR_CLEAR_INT; /* Clear TWINT. */

                    ++(*RxTxBuffer.num);
                    ++(RxTxBuffer.txbuff);
                    --(RxTxBuffer.length);
                }
                else
                {
                    goto clrint;
                }
            }
            else if(twsr == I2C_WDATA_NACK)
            {
                goto clrint;
            }
            else
            {
                goto err;
            }
            break;
        }

        case STA_SLAR:
        {
            if(twsr == I2C_START || twsr == I2C_RESTART)
            {
                RxTxBuffer.status = SLAR;
                TWDR = RxTxBuffer.addr; /* Send SLA+R. */
                TWCR = I2C_TWCR_CLEAR_STA; /* Clear STA bit. */
            }
            else
            {
                goto err;
            }
            break;
        }
        case SLAR:
        {
            if(twsr == I2C_SLAR_ACK)
            {
                RxTxBuffer.status = RDATA;
                if(RxTxBuffer.length > 1U)
                {
                    TWCR = I2C_TWCR_SEND_ACK; /* Start receiving. Send ACK. */
                }
                else
                {
                    TWCR = I2C_TWCR_SEND_NACK; /* Start receiving. Send NACK. */
                }
            }
            else
            {
                goto err;
            }
            break;
        }
        case RDATA:
        {
            if(twsr == I2C_RDATA_ACK || twsr == I2C_RDATA_NACK)
            {
                *RxTxBuffer.rxbuff = TWDR; /* Get received data. */
                ++*RxTxBuffer.num;
                ++RxTxBuffer.rxbuff;
                --RxTxBuffer.length;

                if(RxTxBuffer.length > 1U)
                {
                    TWCR = I2C_TWCR_SEND_ACK; /* Start receiving. Send ACK. */
                }
                else if(RxTxBuffer.length == 1U)
                {
                    TWCR = I2C_TWCR_SEND_NACK; /* Start receiving. Send NACK. */
                }
                else
                {
                    goto clrint; /* Nothing to receive. */
                }
            }
            else
            {
                goto err;
            }
            break;
        }
        default:
        {
            goto clrint;
        }
    }

    return;

clrint:
err:
    RxTxBuffer.status = IDLE;
    TWCR = I2C_TWCR_DIS_INT;
}

#endif /* I2C_ENABLE */
