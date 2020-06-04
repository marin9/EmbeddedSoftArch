#include "defs.h"

#define BTN_1	17
#define BTN_2	18
#define BTN_3	23


void bt1() {
	task_t t;
	t.fn = (void*)uart_print;
	t.arg = "Button 1\r\n";
	queue_push(t);
}

void bt2() {
	task_t t;
	t.fn = (void*)uart_print;
	t.arg = "Button  2\r\n";
	queue_push(t);
}

void bt3() {
	task_t t;
	t.fn = (void*)uart_print;
	t.arg = "Button   3\r\n";
	queue_push(t);
}


void setup() {
	uart_init();
	pic_init();
	timer_init();
	queue_init();
	
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
	task_t t;
	// execute tasks
	while (queue_pop(&t))
		t.fn(t.arg);
}
