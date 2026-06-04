# FlexLCD library

**FlexLCD** is a portable C driver for HD44780-compatible alphanumeric LCD displays featuring a clean **Hardware Abstraction Layer (HAL)** architecture.

The library is designed to run on a wide range of platforms including **Arduino, Raspberry Pi, Linux SBCs, and bare-metal microcontrollers**.

The driver core is written in **pure C**, allowing it to be reused across embedded and desktop environments while keeping platform-specific code isolated in HAL implementations.

## Features

- HD44780 compatible LCD driver
- 4-bit and 8-bit bus support
- Clean Hardware Abstraction Layer (HAL)
- Platform-agnostic core driver
- Works with microcontrollers and Linux systems
- Small footprint suitable for low-resource MCUs
- No dynamic memory allocation
- Reentrant driver design
- Easily portable to new platforms

## Supported platforms

FlexLCD is designed to be portable. The core driver does not depend on any specific hardware.

Current HAL implementations include:

| Platform | HAL source file(s) |
|--------|--------|
| Arduino | `arduino_hal` |
| ESP32 | `esp32_hal`|
| Raspberry Pi microcontrollers (RP2040) | `pico_hal` |
| Raspberry Pi computers (Linux) | `rpi_hal` (libgpiod) |
| PIC16 | `pic_hal_mini` `pic_hal` XC8 |
| AVR | `avr_hal` AVR GCC |

Additional HAL implementations can easily be created for new platforms.

## Basic library usage

The library is written in pure C, but can be used in platforms that support C++ like Arduino.

To use the library first we include the header files for:
- The main library
- The hardware abstraction layer we want to use
```c
#include "flexlcd.h"
#include "arduino_hal.h"
```

Then we declare the required structures:

- The **HAL context structure** contains all the required data to drive the IO pins, it's specific to the platform and implementation we're using. The type for this structure should be: `flexlcd_xxxxxx_context_t`; Where xxxxxxx is the name of the platform.
- The **HAL structure** contains a pointer to the context and the function pointers that are required to implement platform specific actions, for example writing to the bus or asserting control signals. The type for this structure is `flexlcd_hal_t`.
- Finally, the `flexlcd_t` is the wrapper that holds everything together and allows the API functions to access all the requried data and functions.

```c
flexlcd_arduino_context_t lcd_context;
flexlcd_hal_t lcd_hal;
flexlcd_t lcd;
````

We then call the function to initialize the HAL. **The parameters accepted in this fuction are specific to the HAL implementation**, but pointers to the **HAL structre** and **HAL context structure** are always required.
```c
flexlcd_arduino_init(&lcd_hal, &lcd_context, 7, 6, FLEXLCD_INVALID_PIN, 5, 4, 3, 2);
```

Then we can initialize the FlexLCD library by calling `flexlcd_init()`, which also initializes the HD44780 chip.

The function accepts 4 parameters:

- A pointer to a flexlcd_t structure
- A pointer to the HAL structure we'll employ
- The number of display columns
- Number of display rows

```c
flexlcd_init(&lcd, &lcd_hal, 16, 2);
```

After initialization the display is turned off, we can write data to it but it won't be visible. To turn it on we need ot call the `flexlcd_on()` function.
```c
flexlcd_on(&lcd);
```

After we initialized the library we can use all the API functions provided by the library:
```c
flexlcd_puts(&lcd, "FlexLCD Demo");
flexlcd_goto(&lcd, 0, 1);
flexlcd_puts(&lcd, "Geek Factory");
```

Note that all the API functions require a pointer to a flexlcd_t structure that represents the instance of the display we want to control, as the library can support the manipulation of multiple screens.

### Full example for Arduino

```c
#include <Arduino.h>
#include "flexlcd.h"
#include "arduino_hal.h"

flexlcd_arduino_context_t lcd_context;
flexlcd_hal_t lcd_hal;
flexlcd_t lcd;

void setup()
{
	// initialize the library
	flexlcd_arduino_init(&lcd_hal, &lcd_context, 7, 6, FLEXLCD_INVALID_PIN, 5, 4, 3, 2);
	flexlcd_init(&lcd, &lcd_hal, 16, 2);

	// turn on display
	flexlcd_on(&lcd);

	// print some text to the display
	flexlcd_puts(&lcd, "FlexLCD Demo");
	flexlcd_goto(&lcd, 0, 1);
	flexlcd_puts(&lcd, "Geek Factory");
}

void loop()
{
	// scroll the display right 4 times
	for (int i = 0; i < 4; i++)
	{
		flexlcd_scroll_right(&lcd);
		delay(1000);
	}
	// scroll the display left 4 times
	for (int i = 0; i < 4; i++)
	{
		flexlcd_scroll_left(&lcd);
		delay(1000);
	}
}
```
