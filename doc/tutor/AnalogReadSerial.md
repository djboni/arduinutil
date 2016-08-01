# Arduinutil - AnalogReadSerial


Read analog input and print to serial.


```c
/* Config.h - Only changed lines */
#define SERIAL_ENABLE                1

#define TIMER_ENABLE                 1

#define ANALOG_ENABLE                1
```


```c
/* main.c */
#include "Arduinutil.h"

int main(void)
{
    /* init */
    init();
    timerBegin();
    adcBegin();

    /* setup */
    Serial_begin(9600, SERIAL_8N1);

    /* loop */
    for(;;)
    {
        uint16_t sensorValue = analogRead(A0);

        /* (0-1023)_ADC -> (0-5000)_mV */
        uint16_t sensorMilliVolts = (sensorValue * 5000UL) / 1023UL;

        Serial_print("%u_ADC ", sensorValue);
        Serial_print("%u_mV\n", sensorMilliVolts);

        delay(1000);
    }

    return 0;
}
```
