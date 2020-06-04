#pragma once

#define PERIPH_BASE 0x20000000
#define GPIO_BASE   (PERIPH_BASE + 0x200000)
#define UART_BASE   (PERIPH_BASE + 0x201000)

typedef unsigned int uint;

void uart_init();
void uart_print(char *s);
void context_switch(void *prev, void* next);
