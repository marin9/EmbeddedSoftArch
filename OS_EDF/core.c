#include "api.h"
#include "rpi.h"
#include "core.h"

#define TASK_UNUSED		0
#define TASK_READY		1
#define TASK_EDL_R		2
#define TASK_EDL_W		3

#define SYS_ENTRY()	int ei = cpu_interrupts(1)
#define SYS_EXIT()	cpu_interrupts(ei)

static volatile uint sched_run;
static volatile uint sys_time;
static task_t *active_task;
static queue_t queue_ready;
static queue_t q_edl_wait;
static queue_t q_edl_ready;
static task_t task[TASK_COUNT];
static char stack[TASK_COUNT][STACK_SIZE] \
	__attribute__((aligned(8)));


static void task_yield() {
	task_t *prev = active_task;
	if (prev) {
		switch (prev->status) {
		case TASK_READY:
			queue_push(&queue_ready, prev);
			break;
		case TASK_EDL_W:
			squeue_put(&q_edl_wait, prev);
			break;
		case TASK_EDL_R:
			squeue_put(&q_edl_ready, prev);
			break;
		}
	}

	active_task = squeue_pop(&q_edl_ready);
	if (!active_task)
		active_task = queue_pop(&queue_ready);

	if (active_task)
		cpu_switch(prev, active_task);
}

static void idle() {
	while (1) {
		SYS_ENTRY();
		task_yield();
		SYS_EXIT();
	}
}

static void task_tick() {
	int sched = 0;
	task_t *tmp;

	timer_set(TICK_TIME);
	sys_time += 10;

	if (!sched_run)
		return;

	while (1) {
		tmp = squeue_peek(&q_edl_wait);
		if (!tmp || (sys_time < tmp->time_p))
			break;

		sched = 1;
		tmp = squeue_pop(&q_edl_wait);
		tmp->status = TASK_EDL_R;
		tmp->time_p += tmp->period;
		squeue_put(&q_edl_ready, tmp);
	}

	if (sched || !squeue_peek(&q_edl_ready))
		task_yield();
}


void core_init() {
	int i;
	for (i = 0; i < TASK_COUNT; ++i)
		task[i].status = TASK_UNUSED;

	sys_time = 0;
	sched_run = 0;
	active_task = 0;

	queue_init(&queue_ready);
	squeue_init(&q_edl_ready);
	squeue_init(&q_edl_wait);

	task_create(idle, 0);
	pic_register(SYSTMR1_IRQ, task_tick);
	timer_set(TICK_TIME);
}

void sched_start() {
	sched_run = 1;
	timer_set(TICK_TIME);
	ENABLE_INTR();
}

void task_create(func fn, void *arg) {
	int i;
	SYS_ENTRY();

	for (i = 0; i < TASK_COUNT; ++i)
		if (!task[i].status)
			break;

	if (i >= TASK_COUNT) {
		SYS_EXIT();
		return;
	}

	task[i].id = i;
	task[i].sp = (uint*)(stack[i] + STACK_SIZE - 4);
	task[i].sp -= sizeof(context_t);
	task[i].status = TASK_READY;

	context_t *ctx = (context_t*)task[i].sp;
	ctx->cpsr = CPSR_MODE_SYS;
  	ctx->lr = (uint)task_exit;
  	ctx->pc = (uint)fn;
  	ctx->r0 = (uint)arg;

	queue_push(&queue_ready, &task[i]);
	SYS_EXIT();
}

void task_setdl(uint period) {
	SYS_ENTRY();
	active_task->period = period;
	active_task->time_p = sys_time + period;
	active_task->status = TASK_EDL_R;
	task_yield();
	SYS_EXIT();
}

uint task_wait() {
	uint ret = 0;
	SYS_ENTRY();
	active_task->status = TASK_EDL_W;
	if (sys_time > active_task->time_p) {
		ret = 1;
	}
	task_yield();
	SYS_EXIT();

	if (ret)
		uart_print("ERROR: Deadline fail.\r\n");

	return ret;
}

void task_exit() {
	cpu_interrupts(1);
	active_task->status = TASK_UNUSED;
	task_yield();
}

int ticks_get() {
	return sys_time;
}
