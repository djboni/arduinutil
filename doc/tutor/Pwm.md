# Arduinutil - PWM


Using PWM.


```c
/* Config.h - Only changed lines */
#define TIMER_ENABLE                 1
#define TIMER_PRESCALER              64U

#define PWM_ENABLE                   1
#define TIMER1_PRESCALER             64U
#define TIMER2_PRESCALER             64U
```



```c
/* main.c */
#include "Arduinutil.h"

#define PWM  5

int main(void)
{
    init();

    timerBegin(); /* Delay and PWM (pins 5 and 6). */
    timer1Begin(); /* PWM (pins 9 and 10). */
    timer2Begin(); /* PWM (pins 3 and 11). */

    pinMode(PWM, OUTPUT); /* Configure pin as output. */
    pwmMode(PWM, PWM_NOINVERT); /* Configure pin PWM mode. */

    for(;;)
    {
        static uint8_t i = 0;
        analogWrite(PWM, i++); /* Write PWM value to pin. */
        delay(8);
    }
}
```
