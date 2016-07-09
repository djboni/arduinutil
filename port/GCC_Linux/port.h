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

#define DISABLE_INTERRUPTS() do{}while(0U)
#define ENABLE_INTERRUPTS()  do{}while(0U)

#define ENTER_CRITICAL() do{}while(0U)
#define EXIT_CRITICAL()  do{}while(0U)

#define ENTER_CRITICAL_IF_CONCURRENT() if(Concurrent) ENTER_CRITICAL()
#define EXIT_CRITICAL_IF_CONCURRENT()  if(Concurrent) EXIT_CRITICAL()

#define WAIT() do{}while(0U)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_PORT_H__ */
