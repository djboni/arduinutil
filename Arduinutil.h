/*
 Arduinutil - Arduino-like library written in C

 Supported microcontrollers:

 ATmega328P
 ATmega2560

 MSP430G2553


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

#ifndef __ARDUINUTIL_H__
#define __ARDUINUTIL_H__

#include "Config.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void init(void);

void disablePeripheralsClocks(void);
void enablePeripheralsClocks(void);

void disableDigitalInputsOfAnalogPins(void);
void enableDigitalInputsOfAnalogPins(void);

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
uint8_t digitalRead(uint8_t pin);
uint8_t digitalPinToInterrupt(uint8_t pin);
void attachInterrupt(uint8_t pin, void (*isr)(void), uint8_t mode);
void detachInterrupt(uint8_t pin);
void enableExternalInterrupt(uint8_t pin);
void disableExternalInterrupt(uint8_t pin);

void adcBegin(void);
void adcEnd(void);
void analogConvertStart(uint8_t analog);
uint8_t analogConvertReady(void);
uint16_t analogConvertGetValue(void);
uint16_t analogRead(uint8_t analog);
void analogReference(uint8_t reference);

void timer1Begin(void);
void timer1End(void);
uint32_t timer1Counts(void);
void timer2Begin(void);
void timer2End(void);
uint32_t timer2Counts(void);
void timer3Begin(void);
void timer3End(void);
uint32_t timer3Counts(void);
void timer4Begin(void);
void timer4End(void);
uint32_t timer4Counts(void);
void timer5Begin(void);
void timer5End(void);
uint32_t timer5Counts(void);
void pwmMode(uint8_t pin, enum PwmModes mode);
void analogWrite(uint8_t pin, uint8_t value);

void timerBegin(void);
void timerEnd(void);
uint32_t timerCounts(void);
void delayCounts(uint32_t counts);
uint32_t millis(void);
uint32_t micros(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);

void Serial_begin(uint32_t speed, uint32_t config);
void Serial_end(void);
Size_t Serial_available(void);
void Serial_flush(void);
void Serial_writeByte(uint8_t data);
void Serial_write(const void *str);
void Serial_writeBuff(const void *buff, uint16_t length);
int Serial_print(const char *format, ...);
int16_t Serial_read(void);

void Serial1_begin(uint32_t speed, uint32_t config);
void Serial1_end(void);
Size_t Serial1_available(void);
void Serial1_flush(void);
void Serial1_writeByte(uint8_t data);
void Serial1_write(const void *str);
void Serial1_writeBuff(const void *buff, uint16_t length);
int Serial1_print(const void *format, ...);
int16_t Serial1_read(void);

void Serial2_begin(uint32_t speed, uint32_t config);
void Serial2_end(void);
Size_t Serial2_available(void);
void Serial2_flush(void);
void Serial2_writeByte(uint8_t data);
void Serial2_write(const void *str);
void Serial2_writeBuff(const void *buff, uint16_t length);
int Serial2_print(const void *format, ...);
int16_t Serial2_read(void);

void Serial3_begin(uint32_t speed, uint32_t config);
void Serial3_end(void);
Size_t Serial3_available(void);
void Serial3_flush(void);
void Serial3_writeByte(uint8_t data);
void Serial3_write(const void *str);
void Serial3_writeBuff(const void *buff, uint16_t length);
int Serial3_print(const void *format, ...);
int16_t Serial3_read(void);

void I2c_begin(uint32_t speed);
void I2c_end(void);
uint8_t I2c_getStatus(void);
void I2c_write(uint8_t addr, const uint8_t *buff, uint8_t length, uint8_t *numsent);
void I2c_read(uint8_t addr, uint8_t *buff, uint8_t length, uint8_t *numread);
void I2c_stop(void);

void Wdt_enable(uint16_t timeout_ms);
void Wdt_disable(void);
void Wdt_reset(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_H__ */
