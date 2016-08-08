/*
 Arduinutil - Arduino-like library written in C

 Supported microcontrollers:

 ATmega328P
 ATmega328PB
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

void timerBegin(void);
void timerEnd(void);
uint32_t timerCounts(void);
void delayCounts(uint32_t counts);
uint32_t millis(void);
uint32_t micros(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
uint32_t timerCountToMs(uint32_t count);
uint32_t timerMsToCount(uint32_t ms);
uint32_t timerCountToUs(uint32_t count);
uint32_t timerUsToCount(uint32_t us);

void Serial_begin(uint32_t speed, uint32_t config);
void Serial_end(void);
Size_t Serial_available(void);
void Serial_flush(void);
void Serial_writeByte(uint8_t data);
void Serial_write(const void *str);
void Serial_writeBuff(const void *buff, uint16_t length);
void Serial_print(const void *format, ...);
int16_t Serial_read(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_H__ */
