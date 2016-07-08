/*
 Arduinutil Queue - Queue implementation in C

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

#ifndef __ARDUINUTIL_QUEUE_H__
#define __ARDUINUTIL_QUEUE_H__

#include "Arduinutil.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct cCircular {
    uint8_t *Buff;
    Size_t Size;
    Size_t Used;
    Size_t Head;
    Size_t Tail;
};

void cCircular_init(struct cCircular *o, uint8_t *buff, Size_t size);
uint8_t cCircular_pushfront(struct cCircular *o, uint8_t val);
uint8_t cCircular_pushback(struct cCircular *o, uint8_t val);
uint8_t cCircular_popfront(struct cCircular *o, uint8_t *val);
uint8_t cCircular_popback(struct cCircular *o, uint8_t *val);
Size_t cCircular_size(const struct cCircular *o);
Size_t cCircular_used(const struct cCircular *o);
Size_t cCircular_free(const struct cCircular *o);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ARDUINUTIL_QUEUE_H__ */
