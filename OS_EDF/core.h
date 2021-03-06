#pragma once

#define TICK_TIME		10000
#define TASK_COUNT		8
#define STACK_SIZE		(4 * 1024)

typedef void (*func)();
typedef unsigned int uint;

typedef struct {
	uint r0, r1, r2, r3, r4, r5, r6, r7;
	uint r8, r9, r10, r11, cpsr, lr, pc;
} context_t;

typedef struct task_t {
	uint *sp;
	uint id;
	uint status;
	uint period;
	uint time_p;
	struct task_t *next;
} task_t;

typedef struct {
	task_t *first;
	task_t *last;
} queue_t;


void core_init();
void sched_start();

void queue_init(queue_t *q);
void queue_push(queue_t *q, task_t *t);
task_t* queue_pop(queue_t *q);

void squeue_init(queue_t *q);
void squeue_put(queue_t *q, task_t *t);
task_t* squeue_pop(queue_t *q);
task_t* squeue_peek(queue_t *q);
