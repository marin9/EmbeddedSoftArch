#include "api.h"
#include "rpi.h"
#include "core.h"

#define TASK_UNUSED		0
#define TASK_READY		1
#define TASK_SLEEP 		2
#define TASK_BLOCKED	3

#define SYS_ENTRY()	int ei = cpu_interrupts(1)
#define SYS_EXIT()	cpu_interrupts(ei)

static volatile uint sched_run;
static volatile uint sys_time;
static task_t *active_task;
static queue_t queue_ready[PRIO_COUNT];
static mutex_t mutex[MUTEX_COUNT];
static task_t task[TASK_COUNT];
static char stack[TASK_COUNT][STACK_SIZE] \
	__attribute__((aligned(8)));


static void task_yield() {
	int i;
	task_t *prev = active_task;

	if (prev && prev->status == TASK_READY)
		queue_push(&queue_ready[prev->prio], prev);

	for (i = 0; i < PRIO_COUNT; ++i){
		active_task = queue_pop(&queue_ready[i]);
		if (active_task)
			break;
	}

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
	int i;
	++sys_time;
	timer_set(TICK_TIME);

	if (!sched_run)
		return;

	for (i = 0; i < TASK_COUNT; ++i) {
		if (task[i].status == TASK_SLEEP) {
			if (sys_time >= task[i].sleep) {
				task[i].status = TASK_READY;
				queue_push(&queue_ready[task[i].prio], &task[i]);
			}
		}
	}
	task_yield();
}


void core_init() {
	int i;

	for (i = 0; i < PRIO_COUNT; ++i)
		queue_init(&queue_ready[i]);

	for (i = 0; i < TASK_COUNT; ++i)
		task[i].status = TASK_UNUSED;

	for (i = 0; i < MUTEX_COUNT; ++i) {
		mutex[i].owner = -1;
		queue_init(&mutex[i].wait_q);
	}

	active_task = 0;
	sys_time = 0;
	sched_run = 0;

	task_create(idle, 0, PRIO_COUNT - 1);
	pic_register(SYSTMR1_IRQ, task_tick);
	timer_set(TICK_TIME);
}

void sched_start() {
	sched_run = 1;
	timer_set(TICK_TIME);
	ENABLE_INTR();
}

void task_create(func fn, void *arg, uint pr) {
	int i;
	SYS_ENTRY();
	for (i = 0; i < TASK_COUNT; ++i)
		if (!task[i].status)
			break;

	if (i >= TASK_COUNT) {
		SYS_EXIT();
		return;
	}

	if (pr >= PRIO_COUNT) {
		SYS_EXIT();
		return;
	}

	task[i].id = i;
	task[i].prio = pr;
	task[i].sp = (uint*)(stack[i] + STACK_SIZE - 4);
	task[i].sp -= sizeof(context_t);
	task[i].status = TASK_READY;

	context_t *ctx = (context_t*)task[i].sp;
	ctx->cpsr = CPSR_MODE_SYS;
  	ctx->lr = (uint)task_exit;
  	ctx->pc = (uint)fn;
  	ctx->r0 = (uint)arg;

	queue_push(&queue_ready[pr], &task[i]);
	if(sched_run && !ei)
		task_yield();

	SYS_EXIT();
}

void task_sleep(uint ms) {
	SYS_ENTRY();
	if (ms) {
		active_task->sleep = sys_time + ms;
		active_task->status = TASK_SLEEP;
	}
	task_yield();
	SYS_EXIT();
}

void task_exit() {
	cpu_interrupts(1);
	active_task->status = TASK_UNUSED;
	task_yield();
}

int mutex_lock(uint id) {
	if (id >= MUTEX_COUNT)
		return -1;

	SYS_ENTRY();
	if (mutex[id].owner == (uint)-1) {
		mutex[id].owner = active_task->id;

	} else if (mutex[id].owner != active_task->id){
		active_task->status = TASK_BLOCKED;
		queue_push(&mutex[id].wait_q, active_task);
		task_yield();
		mutex[id].owner = active_task->id;
	}
	SYS_EXIT();
	return 0;
}

int mutex_unlock(uint id) {
	task_t *tmp;

	if (id >= MUTEX_COUNT)
		return -1;

	if (mutex[id].owner != active_task->id)
		return -2;

	SYS_ENTRY();
	mutex[id].owner = -1;
	tmp = queue_pop(&mutex[id].wait_q);
	if (tmp) {
		tmp->status = TASK_READY;
		queue_push(&queue_ready[tmp->prio], tmp);
	}

	task_yield();
	SYS_EXIT();
	return 0;
}

int ticks_get() {
	return sys_time;
}
