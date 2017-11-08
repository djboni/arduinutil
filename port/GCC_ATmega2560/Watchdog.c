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

#include "Arduinutil.h"
#include "Config.h"
#include <avr/wdt.h>

#if (WATCHDOG_ENABLE != 0)

void Wdt_enable(uint16_t timeout_ms)
{
    uint8_t config;

    switch(timeout_ms) {
        case 15U:
            config = WDTO_15MS;
            break;
        case 30U:
            config = WDTO_30MS;
            break;
        case 60U:
            config = WDTO_60MS;
            break;
        case 120U:
            config = WDTO_120MS;
            break;
        case 250U:
            config = WDTO_250MS;
            break;
        case 500U:
            config = WDTO_500MS;
            break;
        case 1000U:
            config = WDTO_1S;
            break;
        case 2000U:
            config = WDTO_2S;
            break;
        case 4000U:
            config = WDTO_4S;
            break;
        case 8000U:
            config = WDTO_8S;
            break;
        default:
            ASSERT(0); /* Invalid watchdog timeout. */
            config = WDTO_8S;
            break;
    }

    wdt_enable(config);
}

void Wdt_disable(void)
{
    wdt_disable();
}

void Wdt_reset(void)
{
    wdt_reset();
}

#if (WATCHDOG_AUTOINIT != 0)

__attribute__((naked)) __attribute__((section(".init3")))
void wdt_init(void)
{
    #if (WATCHDOG_AUTOINIT_TIMEOUT != 0U)
    {
        Wdt_enable(WATCHDOG_AUTOINIT_TIMEOUT);
    }
    #else
    {
        Wdt_disable();
    }
    #endif
}

#endif /* WATCHDOG_AUTOINIT */

#endif /* WATCHDOG_ENABLE */
