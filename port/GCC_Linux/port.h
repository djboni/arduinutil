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

#define INTERRUPTS_DISABLE() do{}while(0U)
#define INTERRUPTS_ENABLE()  do{}while(0U)

#define CRITICAL_VAL()
#define CRITICAL_ENTER() do{}while(0U)
#define CRITICAL_EXIT()  do{}while(0U)

#define CRITICAL_ENTER_IF_CONCURRENT() if(Concurrent) CRITICAL_ENTER()
#define CRITICAL_EXIT_IF_CONCURRENT()  if(Concurrent) CRITICAL_EXIT()

#define WAIT_INT() do{}while(0U)
#define WAIT_BUSY() do{}while(0U)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_PORT_H__ */
