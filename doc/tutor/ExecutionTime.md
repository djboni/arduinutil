# Arduinutil - Execution time measurement



## Execution time



One simple way to test execution time is call `timerCounts()` before and after
the code you want to test and subtract the values. It will give you the number
of counts the hardware timer hat while the code has executed plus the time of
execution of one `timerCounts()` call. To obtain the number of CPU clocks
instead of timer counts, just multiply the number of timer counts by
`TIMER_PRESCALER`. That is exactly what the macro below does.



```c
#define EXEC_TIME(time, code) \
do{ \
    uint32_t __start = timerCounts(); \
    { code; } \
    time = (timerCounts() - __start) * TIMER_PRESCALER; \
}while(0)


void test_once(void)
{
    uint32_t time;
    EXEC_TIME(time, /*code*/; );

    Serial_write("timerCounts() took ");
    Serial_print("%lu ", time);
    Serial_write("clock cycles\n");
}
```



## Execution time statistics



However many times we want to run the same test several times because
statistics. And that is what the macro below does. It takes the number of times
the test must run, the variables where to put the minimum, average and the
maximum times of execution, the preamble (code to execute before), the code to
test the execution time and the postamble (code to execute after). This macro
makes use of the previous one.



```c
#define EXEC_TIME_MULTIPLE(num, min, avg, max, pre, code, pos) \
do{ \
    uint32_t __max = 0, __min = -1, __avg = 0, __i; \
    for(__i = 0; __i < num; ++__i) \
    { \
        uint32_t __time; \
        { pre; } \
        EXEC_TIME(__time, code); \
        { pos; } \
        if(__time > __max) \
            __max = __time; \
        if(__time < __min) \
            __min = __time; \
        __avg += __time; \
    } \
    max = __max; \
    min = __min; \
    avg = __avg / num; \
}while(0)


void test_multiple(void)
{
    const uint32_t num = 1000;
    uint32_t min, max, avg;
    EXEC_TIME_MULTIPLE(num, min, avg, max, /*pre*/;, /*code*/;, /*pos*/; );

    Serial_write("timerCounts() takes ");
    Serial_print("min=%lu ", min);
    Serial_print("avg=%lu ", avg);
    Serial_print("max=%lu ", max);
    Serial_write("clock cycles\n");
}
```



## Complete example



For this example we enabled the serial and the timer modules by changing the
definitions of `SERIAL_ENABLE` and `TIMER_ENABLE` from 0 to 1. For a better
time resolution we also changed the `TIMER_PRESCALER` from 1024 to 8.



```c
/* Config.h - Only changed lines */
#define SERIAL_ENABLE                1

#define TIMER_ENABLE                 1
#define TIMER_PRESCALER              8U
```



The main function starts initializing the modules we use, then calls the
functions that run the execution time tests and print the results and then
waits in a for loop.



```c
/* main.c */
#include "Arduinutil.h"


#define EXEC_TIME(time, code) \
do{ \
    uint32_t __start = timerCounts(); \
    { code; } \
    time = (timerCounts() - __start) * TIMER_PRESCALER; \
}while(0)


#define EXEC_TIME_MULTIPLE(num, min, avg, max, pre, code, pos) \
do{ \
    uint32_t __max = 0, __min = -1, __avg = 0, __i; \
    for(__i = 0; __i < num; ++__i) \
    { \
        uint32_t __time; \
        { pre; } \
        EXEC_TIME(__time, code); \
        { pos; } \
        if(__time > __max) \
            __max = __time; \
        if(__time < __min) \
            __min = __time; \
        __avg += __time; \
    } \
    max = __max; \
    min = __min; \
    avg = __avg / num; \
}while(0)


void test_once(void)
{
    uint32_t time;
    EXEC_TIME(time, /*code*/; );

    Serial_write("timerCounts() took ");
    Serial_print("%lu ", time);
    Serial_write("clock cycles\n");
}


void test_multiple(void)
{
    const uint32_t num = 1000;
    uint32_t min, max, avg;
    EXEC_TIME_MULTIPLE(num, min, avg, max, /*pre*/;, /*code*/;, /*pos*/; );

    Serial_write("timerCounts() takes ");
    Serial_print("min=%lu ", min);
    Serial_print("avg=%lu ", avg);
    Serial_print("max=%lu ", max);
    Serial_write("clock cycles\n");
}


int main(void)
{
    init();
    timerBegin();
    Serial_begin(9600, SERIAL_8N1);

    test_once();
    test_multiple();

    /* Wait forever */
    for(;;) {}
    return 0;
}
```
