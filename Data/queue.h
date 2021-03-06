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

#if (QUEUE_ENABLE != 0)

struct Queue_t {
    Size_t ItemSize;
    volatile Size_t Free;
    volatile Size_t Used;
    volatile Size_t WLock;
    volatile Size_t RLock;
    uint8_t *volatile Head;
    uint8_t *volatile Tail;
    uint8_t* Buff;
    uint8_t* BufEnd;
};

void Queue_init(struct Queue_t *o, void *buff, Size_t length, Size_t item_size);
uint8_t Queue_pushfront(struct Queue_t *o, const void *val);
uint8_t Queue_pushback(struct Queue_t *o, const void *val);
uint8_t Queue_popfront(struct Queue_t *o, void *val);
uint8_t Queue_popback(struct Queue_t *o, void *val);
Size_t Queue_length(const struct Queue_t *o);
Size_t Queue_used(const struct Queue_t *o);
Size_t Queue_free(const struct Queue_t *o);
void Queue_clear(struct Queue_t *o);

#define Queue_write(o, val)    Queue_pushback(o, val)
#define Queue_read(o, val)     Queue_popfront(o, val)
#define Queue_empty(o)         (Queue_used(o) == 0U)
#define Queue_full(o)          (Queue_free(o) == 0U)

#endif /* QUEUE_ENABLE */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_QUEUE_H__ */
