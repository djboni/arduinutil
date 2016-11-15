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

#include "port.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QUEUE_ENABLE                 1
#define MAILBOX_ENABLE               1
#define SEMAPHORE_ENABLE             1
#define MUTEX_ENABLE                 1

#define SERIAL_ENABLE                0
#define SERIAL_RBUFSZ                16U
#define SERIAL_TBUFSZ                16U

#define TIMER_ENABLE                 0
#define TIMER_PRESCALER              8U /* 1, 2, 4, 8 */

#define ANALOG_ENABLE                0

#define DIGITAL_ATTACH_INT_ENABLE    0

typedef uint16_t Size_t;

#define ASSERT(expr)                                   \
  do {                                                 \
    if((expr) == 0)                                    \
    {                                                  \
      INTERRUPTS_DISABLE();                            \
      /*assert_function(__FILE__, __LINE__, #expr);*/  \
      for(;;) {}                                       \
    }                                                  \
  } while(0)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_CONFIG_H__ */
