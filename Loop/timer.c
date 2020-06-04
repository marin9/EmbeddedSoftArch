#include "defs.h"

#define SYS_TIMER_CS	((volatile uint*)(SYSTMR_BASE + 0x00))
#define SYS_TIMER_CLO	((volatile uint*)(SYSTMR_BASE + 0x04))
#define SYS_TIMER_CHI	((volatile uint*)(SYSTMR_BASE + 0x08))
#define SYS_TIMER_C1	((volatile uint*)(SYSTMR_BASE + 0x10))
#define SYS_TIMER_C3	((volatile uint*)(SYSTMR_BASE + 0x18))


uint time_get() {
	return *SYS_TIMER_CLO;
}

void time_delay(uint ms) {
	uint us = 1000 * ms + time_get();
	while (us > time_get());
}
