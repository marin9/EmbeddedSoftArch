#include "api.h"
#include "rpi.h"

static void work(){
	// 80 ms
	//uint tm = timer_get();
	int i;
	for (i = 0; i < 1000000; ++i)
		asm volatile("nop");
	/*tm = timer_get() - tm;
	uart_print("---");
	uart_dec(tm);
	uart_print("\r\n");*/
}

static void task_edf1() {
	task_setdl(300);
	while (1) {
		uart_print("\x1B[33m");
		uart_dec(ticks_get());
		uart_print("\tA\r\n");
		work();
		task_wait();
	}
}

static void task_edf2() {
	task_setdl(400);
	while (1) {
		uart_print("\x1B[31m");
		uart_dec(ticks_get());
		uart_print("\t\tB\r\n");
		work();
		task_wait();
	}
}

static void task_edf3() {
	task_setdl(500);
	while (1) {
		uart_print("\x1B[36m");
		uart_dec(ticks_get());
		uart_print("\t\t\tC\r\n");
		work();
		task_wait();
	}
}

static void task_edf4() {
	task_setdl(500);
	while (1) {
		uart_print("\x1B[34m");
		uart_dec(ticks_get());
		uart_print("\t\t\t\tD\r\n");
		work();
		task_wait();
	}
}

void setup() {
	uart_init();
	timer_init();
	pic_init();
	core_init();

	task_create(task_edf1, 0);
	task_create(task_edf2, 0);
	task_create(task_edf3, 0);
	task_create(task_edf4, 0);

	sched_start();
}
