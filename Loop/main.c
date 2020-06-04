#include "defs.h"

#define BTN_1	17
#define BTN_2	18
#define BTN_3	23


void setup() {
	uart_init();

	gpio_mode(BTN_1, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_2, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_3, GPIO_IN | GPIO_PULLUP);
}

void loop() {
	if (!gpio_read(BTN_1)) {
		uart_print("Button 1 pressed.\r\n");
	}

	if (!gpio_read(BTN_2)) {
		uart_print("Button  2 pressed.\r\n");
	}

	if (!gpio_read(BTN_3)) {
		uart_print("Button   3 pressed.\r\n");
	}

	time_delay(100);
}
