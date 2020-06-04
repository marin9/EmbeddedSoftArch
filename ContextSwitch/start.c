
void startup() {
    __asm("msr cpsr_c, 0xdf");
    __asm("ldr sp, =0x8000");
    __asm("bl setup");
    __asm("b .");
}

void context_switch(void *prev, void *next) {
    // suppress warnings
    prev = prev;
    next = next;

    // save current context
    __asm("mrs r12, cpsr");
	__asm("push {sp, lr}");
	__asm("push {r0-r12}");
	__asm("str sp, [r0]");

    // load new context
	__asm("ldr sp, [r1]");
	__asm("pop {r0-r12}");
	__asm("msr cpsr_c, r12");
	__asm("pop {lr, pc}");
}
