# Arduinutil - BlinkNBW


Blink a LED with NBW (No Busy Wait / without using delay).


```c
/* Config.h - Only changed lines */
#define TIMER_ENABLE                 1
```


```c
/* main.c */
#include "Arduinutil.h"

#define LED 13

void blink(void);

int main(void)
{
    /* init */
    init();
    timerBegin();

    /* setup */
    pinMode(LED, OUTPUT);

    /* loop */
    for(;;)
    {
        blink();
        /* Do other stuff */
    }

    return 0;
}

void blink(void)
{
    /* Declare last_blink_ms as static so it will not lose its value. */
    static uint32_t last_blink_ms = 0;
    const uint32_t delay_time_ms = 1000;

    if((millis() - last_blink_ms) >= delay_time_ms)
    {
        digitalWrite(LED, !digitalRead(LED));
        last_blink_ms += delay_time_ms;
    }
}
```
