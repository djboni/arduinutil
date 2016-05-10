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
#define MAXIO    (ANALOGIO+16U)

#define DISABLE_INTERRUPTS() __asm volatile("cli" ::)
#define ENABLE_INTERRUPTS()  __asm volatile("sei" ::)

#define ENTER_CRITICAL()                              \
    __asm volatile("in    __tmp_reg__,__SREG__" ::);  \
    __asm volatile("cli" ::);                         \
    __asm volatile("push  __tmp_reg__" ::)
#define EXIT_CRITICAL()                               \
    __asm volatile("pop   __tmp_reg__" ::);           \
    __asm volatile("out   __SREG__,__tmp_reg__" ::)

#ifdef __cplusplus
} /* extern "C" */
} /* namespace Arduinutil */
#endif

#endif /* __ARDUINUTIL_PORT_H__ */
