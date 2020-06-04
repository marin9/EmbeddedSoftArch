#pragma once
#include "core.h"

void task_create(func fn, void *arg);
void task_yield();
void task_exit();
