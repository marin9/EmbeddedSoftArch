#include "defs.h"

#define QUEUE_SIZE	4

static volatile int in, out, count;
static volatile task_t task[QUEUE_SIZE];


void queue_init() {
	DISABLE_INTR();
	in = 0;
	out = 0;
	count = 0;
	RESTORE_INTR();
}

int queue_push(task_t ts) {
	if (count == QUEUE_SIZE)
		return 0;

	DISABLE_INTR();
	task[in].fn = ts.fn;
	task[in].arg = ts.arg;
	in = (in + 1) % QUEUE_SIZE;
	++count;
	RESTORE_INTR();
	return 1;
}

int queue_pop(task_t *ts) {
	if (count == 0)
		return 0;

	DISABLE_INTR();
	ts->fn = task[out].fn;
	ts->arg = task[out].arg;
	out = (out + 1) % QUEUE_SIZE;
	--count;
	RESTORE_INTR();
	return 1;
}
