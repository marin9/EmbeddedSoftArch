#include "api.h"
#include "rpi.h"

#define BTN_1	17
#define BTN_2	18
#define BTN_3	23
#define BTN_4	24

static volatile uint last[4];

static void work(char *msg) {
	int i, w;
	for (i = 0; i < 7; ++i) {
		uart_print(msg);
		for (w = 0; w < 6000000; ++w)
			asm volatile("nop");
	}
}


void btn1_handler() {
	if (timer_get() > last[0] + 200000) {
		last[0] = timer_get();
		task_create(work, " Doing\t\t1\r\n");
		uart_print("Created 1\r\n");
	}
}

void btn2_handler() {
	if (timer_get() > last[1] + 200000) {
		last[1] = timer_get();
		task_create(work, " Doing\t\t\t2\r\n");
		uart_print("Created 2\r\n");
	}
}

void btn3_handler() {
	if (timer_get() > last[2] + 200000) {
		last[2] = timer_get();
		task_create(work, " Doing\t\t\t\t3\r\n");
		uart_print("Created 3\r\n");
	}
}

void btn4_handler() {
	if (timer_get() > last[3] + 200000) {
		last[3] = timer_get();
		task_create(work, " Doing\t\t\t\t\t4\r\n");
		uart_print("Created 4\r\n");
	}
}

void setup() {
	uart_init();
	timer_init();
	pic_init();
	core_init();

	last[0] = last[1] = last[2] = last[3] = 0;

	gpio_mode(BTN_1, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_2, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_3, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_4, GPIO_IN | GPIO_PULLUP);

	gpio_enint(BTN_1, btn1_handler, GPIO_FALLING_EDGE);
	gpio_enint(BTN_2, btn2_handler, GPIO_FALLING_EDGE);
	gpio_enint(BTN_3, btn3_handler, GPIO_FALLING_EDGE);
	gpio_enint(BTN_4, btn4_handler, GPIO_FALLING_EDGE);

	pic_register(GPIO_IRQ_0, gpio_irq_handler);
	ENABLE_INTR();
	task_yield();
}
