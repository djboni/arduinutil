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

#ifndef __ARDUINUTIL_CONFIG_H__
#define __ARDUINUTIL_CONFIG_H__

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t Size_t;

#define ASSERT(expr) assert(expr)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_CONFIG_H__ */
