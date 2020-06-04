#pragma once

#define PERIPH_BASE		0x20000000
#define SYSTMR_BASE		(PERIPH_BASE + 0x003000)
#define IRQ_BASE		(PERIPH_BASE + 0x00B000)
#define GPIO_BASE		(PERIPH_BASE + 0x200000)
#define UART_BASE		(PERIPH_BASE + 0x201000)

// GPIO
#define GPIO_IN             0
#define GPIO_OUT			1
#define GPIO_FN0			4
#define GPIO_FN1			5
#define GPIO_FN2			6
#define GPIO_FN3			7
#define GPIO_FN4			3
#define GPIO_FN5			2
#define GPIO_PULLOFF		(0 << 4)
#define GPIO_PULLDOWN		(1 << 4)
#define GPIO_PULLUP         (2 << 4)
#define GPIO_NO_EDGE        0
#define GPIO_RISING_EDGE	1
#define GPIO_FALLING_EDGE	2
#define GPIO_HIGH_EDGE	    3
#define GPIO_LOW_EDGE		4

// Interrupt numbers
#define SYSTMR1_IRQ		1
#define SYSTMR3_IRQ 	3
#define GPIO_IRQ_0  	49
#define GPIO_IRQ_1  	50
#define GPIO_IRQ_2  	51
#define GPIO_IRQ_3  	52
#define IRQ_COUNT		72

#define ENABLE_INTR()	cpu_interrupts(0)
#define DISABLE_INTR()	cpu_interrupts(1)

typedef unsigned int uint;

uint cpu_interrupts(uint eif);

void pic_init();
void pic_register(uint irqn, void (*handler)());
void pic_unregister(uint irqn);

void timer_init();
uint timer_get();
void timer_set(uint us);
void timer_delay(uint us);

void gpio_irq_handler();
void gpio_mode(uint pin, uint mode);
void gpio_write(uint pin, uint val);
uint gpio_read(uint pin);
void gpio_enint(uint pin, void (*h)(), uint edge);

void uart_init();
uint uart_getc();
void uart_print(char *s);
void uart_hex(uint n);
void uart_flush();
