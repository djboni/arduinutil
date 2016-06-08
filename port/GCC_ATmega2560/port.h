/*
Arduinutil - Arduino compatible library written in C/C++

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
namespace Arduinutil {
extern "C" {
#else
/* Avoid C90 complaining about inline in some AVR headers. */
#define inline __inline
#endif

#define ANALOGIO 54U
#define MAXIO    (ANALOGIO + 16U)

#define DISABLE_INTERRUPTS() __asm volatile("cli" ::)
#define ENABLE_INTERRUPTS()  __asm volatile("sei" ::)

#define ENTER_CRITICAL()                              \
    __asm volatile("in    __tmp_reg__,__SREG__" ::);  \
    __asm volatile("cli" ::);                         \
    __asm volatile("push  __tmp_reg__" ::)
#define EXIT_CRITICAL()                               \
    __asm volatile("pop   __tmp_reg__" ::);           \
    __asm volatile("out   __SREG__,__tmp_reg__" ::)

#define ENTER_CRITICAL_IF_CONCURRENT() if(Concurrent) ENTER_CRITICAL()
#define EXIT_CRITICAL_IF_CONCURRENT()  if(Concurrent) EXIT_CRITICAL()

#define WAIT() __asm volatile("sleep" ::)

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

#ifdef __cplusplus
} /* extern "C" */
} /* namespace Arduinutil */
#endif

#endif /* __ARDUINUTIL_PORT_H__ */
