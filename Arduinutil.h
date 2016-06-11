/*
 Arduinutil - Arduino compatible library written in C/C++

 Supported microcontrollers:
 ATmega328P
 ATmega2560


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

#include <stdint.h>
#include <stddef.h>

#include "Config.h"

#ifdef __cplusplus
namespace Arduinutil {
extern "C" {
#endif

typedef uint8_t byte;
typedef uint16_t word;

enum {
    LOW = 0U,
    HIGH = 1U,

    INPUT = 0U,
    OUTPUT = 1U,
    INPUT_PULLUP = 2U
};

void init(void);

void disablePeripheralsClocks(void);
void enablePeripheralsClocks(void);

void disableDigitalInputsOfAnalogPins(void);
void enableDigitalInputsOfAnalogPins(void);

void pinMode(byte pin, byte mode);
void digitalWrite(byte pin, byte value);
byte digitalRead(byte pin);

void adcBegin(void);
void adcEnd(void);
void analogConvertStart(uint8_t analog);
uint8_t analogConvertReady(void);
uint16_t analogConvertGetValue(void);
uint16_t analogRead(uint8_t analog);
void analogReference(uint8_t reference);

void timerBegin(void);
void timerEnd(void);
uint32_t timerConvCountToUs(uint32_t count);
uint32_t timerConvUsToCount(uint32_t us);
uint32_t timerGetCounts(void);
uint32_t millis(void);
uint32_t micros(void);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);

struct USART0_Serial {
    void (* const begin)(uint32_t speed, uint32_t config);
    void (* const end)(void);
    void (* const writeByte)(uint8_t data);
    void (* const write)(const void *str);
    void (* const writeBuff)(const void *buff, uint16_t length);
    int16_t (* const read)(void);
    Size_t (* const available)(void);
    void (* const flush)(void);
};

extern struct USART0_Serial Serial;

#ifdef __cplusplus
} /* extern "C" */
} /* namespace Arduinutil */
#endif

#endif /* __ARDUINUTIL_H__ */
