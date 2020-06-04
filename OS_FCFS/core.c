#include "api.h"
#include "rpi.h"
#include "core.h"

#define TASK_UNUSED		0
#define TASK_READY		1

#define SYS_ENTRY()		int ei = cpu_interrupts(1)
#define SYS_EXIT()		cpu_interrupts(ei)


static task_t *active_task;
static queue_t queue_ready;
static task_t task[TASK_COUNT];
static char stack[TASK_COUNT][STACK_SIZE] \
	__attribute__((aligned(8)));


static void idle() {
	while (1)
		task_yield();
}

void core_init() {
	int i;
	for (i = 0; i < TASK_COUNT; ++i)
		task[i].status = TASK_UNUSED;

	active_task = 0;
	queue_init(&queue_ready);
	task_create(idle, 0);
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

void task_yield() {
	SYS_ENTRY();
	task_t *prev = active_task;

	if (prev && prev->status == TASK_READY)
		queue_push(&queue_ready, prev);

	active_task = queue_pop(&queue_ready);
	if (active_task)
		cpu_switch(prev, active_task);
	SYS_EXIT();
}

void task_exit() {
	cpu_interrupts(1);
	active_task->status = TASK_UNUSED;
	task_yield();
}
