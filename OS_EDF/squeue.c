#include "core.h"

void squeue_init(queue_t *q) {
	q->first = 0;
}

void squeue_put(queue_t *q, task_t *t) {
	task_t *p;

	if (q->first) {
		if (q->first->time_p > t->time_p) {
			t->next = q->first;
			q->first = t;
		} else {
			p = q->first;
			while (p->next && (p->next->time_p <= t->time_p)) {
				p = p->next;
			}

			t->next = p->next;
			p->next = t;
		}
	} else {
		q->first = t;
		t->next = 0;
	}
}

task_t* squeue_pop(queue_t *q) {
	task_t *tmp = q->first;
	if (!tmp)
		return 0;

	q->first = tmp->next;
	tmp->next = 0;
	return tmp;
}

task_t* squeue_peek(queue_t *q) {
	return q->first;
}
