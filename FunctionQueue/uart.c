#include "defs.h"

#define UART_DR		((volatile uint*)(UART_BASE + 0x00))
#define UART_FR		((volatile uint*)(UART_BASE + 0x18))
#define UART_IBRD	((volatile uint*)(UART_BASE + 0x24))
#define UART_FBRD	((volatile uint*)(UART_BASE + 0x28))
#define UART_LCRH	((volatile uint*)(UART_BASE + 0x2C))
#define UART_CR		((volatile uint*)(UART_BASE + 0x30))
#define UART_ICR	((volatile uint*)(UART_BASE + 0x44))

#define CR_EN		(1 << 0)
#define CR_RXEN		(1 << 9)
#define CR_TXEN		(1 << 8)
#define FR_RXFE		(1 << 4)
#define FR_TXFF		(1 << 5)
#define FR_TXFE		(1 << 7)
#define LCRH_ENFIFO (1 << 4)
#define LCRH_WLEN8	(3 << 5)


void uart_init() {
	// disable UART
	*UART_CR = 0;

	// set pin 14 & 15 to use UART function
	gpio_mode(14, GPIO_FN0);
	gpio_mode(15, GPIO_FN0);

	// clear interrupts
	*UART_ICR = 0x7FF;
    // set baud rate to 115200
    *UART_IBRD = 27;
    *UART_FBRD = 8;
    // enable fifo, 8-bit word
    *UART_LCRH = LCRH_ENFIFO | LCRH_WLEN8;
    // enable UART, rx, tx
    *UART_CR = CR_EN | CR_RXEN | CR_TXEN;
}

uint uart_getc() {
	while (*UART_FR & FR_RXFE);
	return *UART_DR;
}

void uart_print(char *s) {
	while (*s) {
		while (*UART_FR & FR_TXFF);
		*UART_DR = *s;
		++s;
	}
}

void uart_hex(uint n) {
	int i;
	char buff[9];

	for (i = 7; i >= 0; --i) {
		buff[i] = n & 0x0f;
		n = n >> 4;

		if (buff[i] < 10)
			buff[i] += '0';
		else
			buff[i] += ('A'-10);
	}
	buff[8] = 0;
	uart_print("0x");
	uart_print(buff);
}

void uart_flush() {
	while (!(*UART_FR & FR_TXFE));
}
