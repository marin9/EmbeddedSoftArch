#pragma once
#include "core.h"

void task_create(func fn, void *arg, uint pr);
void task_sleep(uint ms);
void task_exit();

int mutex_lock(uint id);
int mutex_unlock(uint id);

int ticks_get();
