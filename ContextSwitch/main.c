#include "rpi.h"
#include "task.h"


static void t0() {
	while (1) {
		uart_print("Task\t0\r\n");
		task_yield();
	}
}

static void t1() {
	while (1) {
		uart_print("Task\t\t1\r\n");
		task_yield();
	}
}

static void t2(){
	while (1) {
		uart_print("Task\t\t\t2\r\n");
		task_yield();
	}
}

void setup(){
	uart_init();
	task_init();

	task_create(t0);
	task_create(t1);
	task_create(t2);

	task_yield();
}
