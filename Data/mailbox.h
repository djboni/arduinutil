/*
Arduinutil Mailbox - Mailbox implementation in C


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

#ifndef __ARDUINUTIL_MAILBOX_H__
#define __ARDUINUTIL_MAILBOX_H__

#include <stdint.h>
#include "Arduinutil.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Mailbox_t {
    void *volatile Msg;
};

void Mailbox_init(struct Mailbox_t *o);
uint8_t Mailbox_send(struct Mailbox_t *o, void *msg);
void *Mailbox_receive(struct Mailbox_t *o);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_MAILBOX_H__ */
