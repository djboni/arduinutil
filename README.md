# [Arduinutil](https://github.com/djboni/arduinutil)

by [Djones A. Boni](https://twitter.com/djonesboni)


Arduinutil is an Arduino-like library written in C. This library will help you
create your microcontroller project with any IDE and use the awesome Arduino
syntax. The Arduinutil library is intended to be easy to read, understand and
hack. Just drag and drop the library into your project, remove the unnecessary
ports and add the include paths.

You can use Arduinutil both for closed- and open-source projects. You are also
free to keep changes to this library to yourself. However we'll enjoy your
improvements and suggestions. `:-)`

You are free to copy, modify, and distribute Arduinutil with attribution under
the terms of the
[Apache License Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).
See the LICENSE file for details.


## Supported hardware

At this point Arduinutil runs in the following hardware:

* Arduino Uno - ATmega328P (GCC)
* Arduino Mega - ATmega2560 (GCC)


## How to use Arduinutil

To use Arduinutil the only thing you really need is a C compiler for the target
hardware. 

For this tutorial, however, you will need a clean AVR project on Eclipse CDT
for the microcontroller ATmega328P. For other IDEs, such as Atmel Studio, the
steps are pretty similar.

Enter the project directory and clone Arduinutil library into the project.

```
$ cd WORKSPACE/PROJECT/Source
$ git clone https://github.com/djboni/arduinutil.git Arduinutil
```

Remove unnecessary ports, such as `GCC_ATmega2560` and `GCC_Linux`.

```
$ cd Arduinutil/port
$ ls
GCC_ATmega2560  GCC_ATmega328P  GCC_Linux  OTHERS
$ rm -r GCC_ATmega2560 GCC_Linux OTHERS
```

Add `Source/Arduinutil` and `Source/Arduinutil/port/GCC_ATmega328P` to the
include paths. On Eclipse go to
`Project > Properties > C/C++ Build > Settings > AVR Compiler > Directories`
and add them.

Now you can create a file with the main function and compile. Below there is
an example as simple as it can be.

```c
#include <Arduinutil.h>

int main(void)
{
    /* Must init first! */
    init();
    
    /* setup() */
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    
    /* loop() */
    for(;;)
    {
        /* ... */
    }
    
    return 0;
}
```


## Tips and tricks

As rule of thumb, everything is by default disabled and not initialized.

Take a look in the `Arduinutil.h` file to figure out the initialization
functions for each module.

Also check the `port/SOME_PORT/Config.h` file to enable the modules.

Big executable? Try add `-ffunctions-sections -fdata-sections` to the compiler
options and `-Wl,--gc-sections` to the linker options. You can also link with
`-lm -lm` (twice) to get rid of the annoying `__clz_tab`.


## Contributing to Arduinutil

If you have suggestions for improving Arduinutil, please
[open an issue or pull request on GitHub](https://github.com/djboni/arduinutil).


## Important files

`README.md` Fast introduction (this file).

`LICENCE` Complete license text.
