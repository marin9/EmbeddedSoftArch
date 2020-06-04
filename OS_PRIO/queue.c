#include "core.h"

void queue_init(queue_t *q) {
	q->first = 0;
	q->last = 0;
}

void queue_push(queue_t *q, task_t *t) {
	if (q->first == 0) {
		t->next = 0;
		q->first = t;
		q->last = t;
	} else {
		t->next = 0;
		q->last->next = t;
		q->last = t;
	}
}

task_t* queue_pop(queue_t *q) {
	task_t *tmp = q->first;
	if (!tmp)
		return 0;

	q->first = tmp->next;
	if (!q->first)
		q->last = 0;

	return tmp;
}
