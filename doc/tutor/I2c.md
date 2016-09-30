# Arduinutil - I2C


I2C example (EEPROM 24C32).


```c
/* Config.h - Only changed lines */
#define SERIAL_ENABLE                1

#define I2C_ENABLE                   1

#define TIMER_ENABLE                 1
```


```c
/* main.c */
#include "Arduinutil.h"

#define DEVICE_ADDR 0x57

void tutor_eeprom_24c32_write(void);
void tutor_eeprom_24c32_read(void);

int main(void)
{
    init();
    timerBegin();
    Serial_begin(9600, SERIAL_8N1);
    I2c_begin(100000);

    tutor_eeprom_24c32_write();
    tutor_eeprom_24c32_read();

    for(;;) {}

    return 0;
}

void tutor_eeprom_24c32_write(void)
{
    uint8_t buff[10];
    uint8_t num;

    /* Data address (2 bytes). */
    buff[0] = 0;
    buff[1] = 0;
    /* Data to write. */
    buff[2] = 0xFF;
    buff[3] = 0xFF;
    buff[4] = 0xFF;
    buff[5] = 0xFF;

    /* Write and wait finish. */
    I2c_write(DEVICE_ADDR, buff, 6, &num);
    while(I2c_getStatus() != 0) {}

    /* Send stop. */
    I2c_stop();

    Serial_print("W data: %u\n", num);

    delay(100);
}

void tutor_eeprom_24c32_read(void)
{
    uint8_t buff[10];
    uint8_t num;

    /* Data address (2 bytes). */
    buff[0] = 0;
    buff[1] = 0;

    /* Write and wait finish. */
    I2c_write(DEVICE_ADDR, buff, 2, &num);
    while(I2c_getStatus() != 0) {}

    Serial_print("W addr: %u\n", num);

    /* Read (sending repeated start) and wait finish. */
    I2c_read(DEVICE_ADDR, buff, 4, &num);
    while(I2c_getStatus() != 0) {}

    /* Send stop. */
    I2c_stop();

    Serial_print("R data: %u\n", num);
    Serial_print("%02X %02X %02X %02X\n", buff[0], buff[1], buff[2], buff[3]);

    delay(100);
}
```
