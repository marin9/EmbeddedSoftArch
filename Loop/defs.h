#pragma once

#define PERIPH_BASE		0x20000000
#define SYSTMR_BASE		(PERIPH_BASE + 0x003000)
#define GPIO_BASE		(PERIPH_BASE + 0x200000)
#define UART_BASE		(PERIPH_BASE + 0x201000)

// GPIO
#define GPIO_IN			0
#define GPIO_OUT		1
#define GPIO_FN0		4
#define GPIO_FN1		5
#define GPIO_FN2		6
#define GPIO_FN3		7
#define GPIO_FN4		3
#define GPIO_FN5		2
#define GPIO_PULLOFF	(0 << 4)
#define GPIO_PULLDOWN	(1 << 4)
#define GPIO_PULLUP		(2 << 4)

typedef unsigned int uint;

uint time_get();
void time_delay(uint ms);

void gpio_mode(uint pin, uint mode);
void gpio_write(uint pin, uint val);
uint gpio_read(uint pin);

void uart_init();
uint uart_getc();
void uart_print(char *s);
void uart_hex(uint n);
void uart_flush();
