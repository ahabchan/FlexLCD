#include <xc.h>

#include "flexlcd.h"
#include "pic_hal_mini.h"

// Configuration bits (adjust as needed)
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

/**
 * This demo is intended to run at 20 MHz
 */
#define _XTAL_FREQ 20000000

/**
 * The main LCD structure which holds the state of the LCD and allows us to
 * control it through the provided API functions.
 */
flexlcd_t lcd;
/**
 * The HAL structure which holds function pointers for the LCD driver to call
 * platform specific operations such as setting control lines, writing to the
 * bus and delaying.
 */
flexlcd_hal_t hal;

void main()
{
	// Initialize the tiny HAL for PIC16
	flexlcd_pic_mini_init(&hal);
	// Initialize IO pins and the LCD controller itself
	flexlcd_init(&lcd, &hal, 16, 2);

	// Turn on the display (it is turned off by default after initialization)
	flexlcd_on(&lcd);

	// print some text to the display
	flexlcd_puts(&lcd, "FlexLCD Demo");
	flexlcd_goto(&lcd, 0, 1);
	flexlcd_puts(&lcd, "Geek Factory");

	// scroll the display forever
	for (;;) {
		for (uint8_t i = 0; i < 4; i++) {
			flexlcd_scroll_right(&lcd);
			__delay_us(1000000);
		}
		for (uint8_t i = 0; i < 4; i++) {
			flexlcd_scroll_left(&lcd);
			__delay_us(1000000);
		}
	}
}