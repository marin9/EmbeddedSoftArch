#include "rpi.h"

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
	int i;
	*UART_CR = 0;

	gpio_mode(14, GPIO_FN0);
	gpio_mode(15, GPIO_FN0);

	*UART_ICR = 0x7FF;
    *UART_IBRD = 27;
    *UART_FBRD = 8;
    *UART_LCRH = LCRH_ENFIFO | LCRH_WLEN8;
    *UART_CR = CR_EN | CR_RXEN | CR_TXEN;

    for (i = 0; i < 32; ++i)
    	*UART_DR;
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

static void dec2str(char *str, int n) {
	int i = 0;
	int w = 0;
	int d = 1000000000;

	if (n < 0) {
		n = -n;
		str[i] = '-';
		++i;
	}

	while (d) {
		str[i] = n / d + '0';
		if (str[i] != '0')
			w = 1;

		if (w)
			++i;

		n = n % d;
		d /= 10;
	}

	if (i == 0) {
		str[0] = '0';
		str[1] = 0;
	} else {
		str[i] = 0;
	}
}

void uart_dec(uint n) {
	char buff[16];
	dec2str(buff, n);
	uart_print(buff);
}

void uart_flush() {
	while (!(*UART_FR & FR_TXFE));
}
