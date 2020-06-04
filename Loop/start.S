.global start
start:
	// System mode, no interrupts
 	msr cpsr_c, #0xDF

 	// Initialize stack
	ldr sp, =0x8000

	// Start user program
	bl setup
n:	bl loop
	b n
