/*
Arduinutil - Arduino compatible library written in C/C++

Supported microcontrollers:
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

#include <stddef.h>

#ifdef __cplusplus
namespace Arduinutil {
extern "C" {
#endif

#include "Config.h"

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

#ifdef __cplusplus
} /* extern "C" */
} /* namespace Arduinutil */
#endif

#endif /* __ARDUINUTIL_H__ */
