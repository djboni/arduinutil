# Arduinutil - Blink


Blink a LED.


```c
/* Config.h - Only changed lines */
#define TIMER_ENABLE                 1
```


```c
/* main.c */
#include "Arduinutil.h"

#define LED 13

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
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
        delay(1000);
    }

    return 0;
}
```