/*
 Arduinutil Queue - Queue implementation in C


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

#ifndef __ARDUINUTIL_QUEUE_H__
#define __ARDUINUTIL_QUEUE_H__

#include "Arduinutil.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Queue_t {
    Size_t ItemSize;
    Size_t Free;
    Size_t Used;
    Size_t WLock;
    Size_t RLock;
    uint8_t *Head;
    uint8_t *Tail;
    uint8_t *Buff;
    uint8_t *BufEnd;
};

void Queue_init(struct Queue_t *o, void *buff, Size_t length, Size_t item_size);
uint8_t Queue_pushfront(struct Queue_t *o, const void *val);
uint8_t Queue_pushback(struct Queue_t *o, const void *val);
uint8_t Queue_popfront(struct Queue_t *o, void *val);
uint8_t Queue_popback(struct Queue_t *o, void *val);
Size_t Queue_length(const struct Queue_t *o);
Size_t Queue_used(const struct Queue_t *o);
Size_t Queue_free(const struct Queue_t *o);

#define Queue_send(o, val)     Queue_pushback(o, val)
#define Queue_receive(o, val)  Queue_popfront(o, val)
#define Queue_empty(o)         (Queue_used(o) == 0U)
#define Queue_full(o)          (Queue_free(o) == 0U)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_QUEUE_H__ */
