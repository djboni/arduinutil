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
#else
/* Avoid C90 complaining about inline in some AVR headers. */
#define inline __inline
#endif

/*******************************************************************************
 Timer.c
 ******************************************************************************/

#define TIMER_COUNT_TO_MS(x) ((x) * TIMER_PRESCALER / (F_CPU / 1000UL) )
#define TIMER_MS_TO_COUNT(x) ((x) * (F_CPU / 1000UL) / TIMER_PRESCALER )

#define TIMER_COUNT_TO_US(x) ((x) * (TIMER_PRESCALER * 125UL) / (F_CPU / 8000UL) )
#define TIMER_US_TO_COUNT(x) ((x) * (F_CPU / 8000UL) / (TIMER_PRESCALER * 125UL) )

/*******************************************************************************
 Digital.c
 ******************************************************************************/

#define ANALOGIO 14U
#define MAXIO    (ANALOGIO + 6U)

enum DigitalPinModes {
    LOW = 0U,
    HIGH = 1U,

    INPUT = 0U,
    OUTPUT = 1U,
    INPUT_PULLUP = 2U
};

enum DigitalInterruptModes {
    /* LOW = 0x00U, */
    CHANGE = 0x01U,
    FALLING = 0x02U,
    RISING = 0x03U
};

/*******************************************************************************
 Analog.c
 ******************************************************************************/

/* Analog/Digital pins */
#define A0       (ANALOGIO + 0U)
#define A1       (ANALOGIO + 1U)
#define A2       (ANALOGIO + 2U)
#define A3       (ANALOGIO + 3U)
#define A4       (ANALOGIO + 4U)
#define A5       (ANALOGIO + 5U)
/* Analog only pins */
#define A6       (ANALOGIO + 6U)
#define A7       (ANALOGIO + 7U)
/* Analog only internal */
#define ATEMP    (ANALOGIO + 8U)
#define A1V1     (ANALOGIO + 14U)

enum AnalogReferences {
    EXTERNAL = 0x00U, /* External voltage on AREF. */
    INTERNALVCC = 0x01U, /* Internal voltage VCC. */
    INTERNAL1V1 = 0x03U, /* Internal voltage reference 1.1V. */
    /* Arduino IDE compatibility. */
    DEFAULT = INTERNALVCC,
    INTERNAL = INTERNAL1V1
};

/*******************************************************************************
 Others
 ******************************************************************************/

#define DISABLE_INTERRUPTS() do{ __asm volatile("cli" ::); }while(0U)
#define ENABLE_INTERRUPTS()  do{ __asm volatile("sei" ::); }while(0U)

#define VAR_CRITICAL()
#define ENTER_CRITICAL() do{                          \
    __asm volatile("in    __tmp_reg__,__SREG__" ::);  \
    __asm volatile("cli" ::);                         \
    __asm volatile("push  __tmp_reg__" ::);           }while(0U)
#define EXIT_CRITICAL() do{                           \
    __asm volatile("pop   __tmp_reg__" ::);           \
    __asm volatile("out   __SREG__,__tmp_reg__" ::);  }while(0U)

#define WAIT() __asm volatile("sleep" ::)

/*******************************************************************************
 Serial.c
 ******************************************************************************/

#define SERIAL_CONF(A,B,C) ((A)|((B)<<8UL)|((C)<<16UL))
#define SERIAL_5N1 SERIAL_CONF(0x00UL, 0x98UL, 0x00UL)
#define SERIAL_6N1 SERIAL_CONF(0x00UL, 0x98UL, 0x02UL)
#define SERIAL_7N1 SERIAL_CONF(0x00UL, 0x98UL, 0x04UL)
#define SERIAL_8N1 SERIAL_CONF(0x00UL, 0x98UL, 0x06UL)
#define SERIAL_5N2 SERIAL_CONF(0x00UL, 0x98UL, 0x08UL)
#define SERIAL_6N2 SERIAL_CONF(0x00UL, 0x98UL, 0x0AUL)
#define SERIAL_7N2 SERIAL_CONF(0x00UL, 0x98UL, 0x0CUL)
#define SERIAL_8N2 SERIAL_CONF(0x00UL, 0x98UL, 0x0EUL)
#define SERIAL_5E1 SERIAL_CONF(0x00UL, 0x98UL, 0x20UL)
#define SERIAL_6E1 SERIAL_CONF(0x00UL, 0x98UL, 0x22UL)
#define SERIAL_7E1 SERIAL_CONF(0x00UL, 0x98UL, 0x24UL)
#define SERIAL_8E1 SERIAL_CONF(0x00UL, 0x98UL, 0x26UL)
#define SERIAL_5E2 SERIAL_CONF(0x00UL, 0x98UL, 0x28UL)
#define SERIAL_6E2 SERIAL_CONF(0x00UL, 0x98UL, 0x2AUL)
#define SERIAL_7E2 SERIAL_CONF(0x00UL, 0x98UL, 0x2CUL)
#define SERIAL_8E2 SERIAL_CONF(0x00UL, 0x98UL, 0x2EUL)
#define SERIAL_5O1 SERIAL_CONF(0x00UL, 0x98UL, 0x30UL)
#define SERIAL_6O1 SERIAL_CONF(0x00UL, 0x98UL, 0x32UL)
#define SERIAL_7O1 SERIAL_CONF(0x00UL, 0x98UL, 0x34UL)
#define SERIAL_8O1 SERIAL_CONF(0x00UL, 0x98UL, 0x36UL)
#define SERIAL_5O2 SERIAL_CONF(0x00UL, 0x98UL, 0x38UL)
#define SERIAL_6O2 SERIAL_CONF(0x00UL, 0x98UL, 0x3AUL)
#define SERIAL_7O2 SERIAL_CONF(0x00UL, 0x98UL, 0x3CUL)
#define SERIAL_8O2 SERIAL_CONF(0x00UL, 0x98UL, 0x3EUL)

/*******************************************************************************
 I2c.c
 ******************************************************************************/

typedef enum I2C_STATUS_t {
    I2C_OK             = 0x00U,
    I2C_START          = 0x08U,
    I2C_RSTART         = 0x10U,
    I2C_SLAW_ACK       = 0x18U,
    I2C_SLAW_NACK      = 0x20U,
    I2C_WDATA_ACK      = 0x28U,
    I2C_WDATA_NACK     = 0x30U,
    I2C_ARBITRATION    = 0x38U,
    I2C_SLAR_ACK       = 0x40U,
    I2C_SLAR_NACK      = 0x48U,
    I2C_RDATA_ACK      = 0x50U,
    I2C_RDATA_NACK     = 0x58U
} I2C_STATUS;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_PORT_H__ */
