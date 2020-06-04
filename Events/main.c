#include "defs.h"

#define BTN_1	17
#define BTN_2	18
#define BTN_3	23

static int button[3];


void bt1() {
	button[0] = 1;
}

void bt2() {
	button[1] = 1;
}

void bt3() {
	button[2] = 1;
}

int check_btn(int n) {
	int ret = 0;
	DISABLE_INTR();
	if (button[n]) {
		ret = 1;
		button[n] = 0;
	}
	ENABLE_INTR();
	return ret;
}


void setup() {
	int i;
	uart_init();
	pic_init();

	for (i = 0; i < 3; ++i)
		button[i] = 0;

	gpio_mode(BTN_1, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_2, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_3, GPIO_IN | GPIO_PULLUP);

	gpio_enint(BTN_1, bt1, GPIO_FALLING_EDGE);
	gpio_enint(BTN_2, bt2, GPIO_FALLING_EDGE);
	gpio_enint(BTN_3, bt3, GPIO_FALLING_EDGE);

	pic_register(GPIO_IRQ_0, gpio_irq_handler);
	ENABLE_INTR();
}

void loop() {
	if (check_btn(0)) {
		uart_print("Button 1.\r\n");
	}
	if (check_btn(1)) {
		uart_print("Button  2.\r\n");
	}
	if (check_btn(2)) {
		uart_print("Button   3.\r\n");
	}
	timer_delay(100*1000);
}
