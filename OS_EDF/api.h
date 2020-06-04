#pragma once
#include "core.h"

void task_create(func fn, void *arg);
void task_setdl(uint period);
uint task_wait();
void task_exit();

int ticks_get();
