#include "rpi.h"

#define UART_DR		((volatile uint*)(UART_BASE + 0x00))
#define UART_FR		((volatile uint*)(UART_BASE + 0x18))
#define UART_IBRD	((volatile uint*)(UART_BASE + 0x24))
#define UART_FBRD	((volatile uint*)(UART_BASE + 0x28))
#define UART_LCRH	((volatile uint*)(UART_BASE + 0x2C))
#define UART_CR		((volatile uint*)(UART_BASE + 0x30))
#define UART_ICR	((volatile uint*)(UART_BASE + 0x44))

#define GPFSEL1     ((volatile uint*)(GPIO_BASE + 0x04))
#define GPPUD       ((volatile uint*)(GPIO_BASE + 0x94))
#define GPPUDCLK0   ((volatile uint*)(GPIO_BASE + 0x98))

#define CR_EN		(1 << 0)
#define CR_RX_EN	(1 << 9)
#define CR_TX_EN	(1 << 8)
#define LCRH_ENFIFO (1 << 4)
#define LCRH_WLEN8	(3 << 5)


static void delay(int n) {
	while (n--)
		asm volatile("nop");
}

void uart_init() {
	int i;
	// disable UART
	*UART_CR = 0;

	// set pin 14 & 15 to UART function
	i = *GPFSEL1;
	i &= ~(7 << 12);
	i |= (4 << 12);
	i &= ~(7 << 15);
	i |= (4 << 15);
	*GPFSEL1 = i;

	// disable pull up/down
	*GPPUD = 0;
    delay(200);
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    delay(200);
    *GPPUDCLK0 = 0;

	// clear interrupts
    *UART_ICR = 0x7FF;
    // baud rate 115200
    *UART_IBRD = 27;
    *UART_FBRD = 8;
	// enable fifo, 8-bit words
    *UART_LCRH = LCRH_ENFIFO | LCRH_WLEN8;
	// enable UART
    *UART_CR = CR_EN | CR_RX_EN | CR_TX_EN;
}

void uart_print(char *s) {
	while (*s) {
		while (*UART_FR & 0x20);
		*UART_DR = *s;
		++s;
	}
}
