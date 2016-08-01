# Arduinutil - AnalogReadSerialNBW


Read analog input and print to serial with NBW (No Busy Wait / without wait for
conversion).


```c
/* Config.h - Only changed lines */
#define SERIAL_ENABLE                1

#define TIMER_ENABLE                 1

#define ANALOG_ENABLE                1
```


```c
/* main.c */
#include "Arduinutil.h"

void analog_read_serial(void);

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
        analog_read_serial();
        /* Do other stuff */
    }

    return 0;
}

void analog_read_serial(void)
{
    /* Declare converting and last_conversion_ms as static so they will not lose
     * their values. */
    static uint8_t converting = 0; /* false */
    static uint32_t last_conversion_ms = 0;
    const uint32_t delay_time_ms = 1000;

    /* Start conversion every 1 second */
    if((millis() - last_conversion_ms) >= delay_time_ms)
    {
        analogConvertStart(A0);
        converting = 1; /* true */
        last_conversion_ms += delay_time_ms;
    }

    /* Write value to serial if conversion is ready */
    else if(converting && analogConvertReady())
    {
        uint16_t sensorValue = analogConvertGetValue();

        /* (0-1023)_ADC -> (0-5000)_mV */
        uint16_t sensorMilliVolts = (sensorValue * 5000UL) / 1023UL;

        Serial_print("%u_ADC ", sensorValue);
        Serial_print("%u_mV\n", sensorMilliVolts);

        converting = 0; /* false */
    }
}
```
