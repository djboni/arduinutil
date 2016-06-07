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
    LOW          = 0U,
    HIGH         = 1U,

    INPUT        = 0U,
    OUTPUT       = 1U,
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

struct USART0_Serial {
    void (*const begin)(uint32_t speed, uint32_t config);
    void (*const end)(void);
    void (*const write)(uint8_t data);
    void (*const writeStr)(const void *str);
    void (*const writeBuff)(const void *buff, uint16_t length);
    int16_t (*const read)(void);
};

extern struct USART0_Serial Serial;

#ifdef __cplusplus
} /* extern "C" */
} /* namespace Arduinutil */
#endif

#endif /* __ARDUINUTIL_H__ */
