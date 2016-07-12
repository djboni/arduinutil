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

#include "Data/mailbox.h"

#if (MAILBOX_ENABLE != 0)

/** Initialize mailbox struct.
 *
 * Note: Not thread-safe.
 *
 * @param o Pointer to mailbox.
 */
void Mailbox_init(struct Mailbox_t *o)
{
    o->Msg = NULL;
}

/** Send message to mailbox.
 *
 * @param o Pointer to mailbox.
 * @param msg Pointer to the message.
 * @return 1U upon success, 0U otherwise.
 */
uint8_t Mailbox_send(struct Mailbox_t *o, void *msg)
{
    uint8_t ret;
    ENTER_CRITICAL();
    {
        ret = o->Msg == NULL;
        if(ret)
        {
            o->Msg = msg;
        }
    }
    EXIT_CRITICAL();
    return ret;
}

/** Read message from mailbox.
 *
 * @param o Pointer to mailbox.
 * @return Pointer to the message upon success, NULL otherwise.
 */
void *Mailbox_receive(struct Mailbox_t *o)
{
    void *ret;
    ENTER_CRITICAL();
    {
        ret = o->Msg;
        o->Msg = NULL;
    }
    EXIT_CRITICAL();
    return ret;
}

#endif /* MAILBOX_ENABLE */
