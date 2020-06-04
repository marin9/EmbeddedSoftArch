.global start
.global cpu_interrupts

start:
	// System mode, no interrupts
 	msr cpsr_c, #0xDF

 	// Initialize stack
	ldr sp, =0x8000

	// Load irq vectors
  	ldr     r0, =_vectors
  	mov     r1, #0
  	ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	ldmia   r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
  	stmia   r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	// Start user program
	bl setup
lp: bl loop
	b lp


cpu_interrupts:
	mrs r1, cpsr
	mrs r2, cpsr
	and r2, r2, #0x80
	cmp r0, #0
	beq enable_irq
disable_irq:
	orr r1, r1, #0x80
	msr cpsr_c, r1
	mov r0, r2
	bx lr
enable_irq:
	bic r1, r1, #0x80
	msr cpsr_c, r1
	mov r0, r2
	bx lr


_interrupt:
	ldr	sp, =start
	sub sp, sp, #4096
	push {r0-r11}
	push {r14}
	bl irq_handler
	pop {r14}
	pop {r0-r11}
	subs pc, lr, #4


_vectors:
	ldr pc, _reset_h
    ldr pc, _undef_h
    ldr pc, _soft_h
    ldr pc, _pref_abort_h
    ldr pc, _data_abort_h
    ldr pc, _unused_h
    ldr pc, _irq_h
    ldr pc, _fast_h

_reset_h:		.word start
_undef_h:		.word start
_soft_h:		.word start
_pref_abort_h:	.word start
_data_abort_h:	.word start
_unused_h:		.word start
_irq_h:			.word _interrupt
_fast_h:		.word start
