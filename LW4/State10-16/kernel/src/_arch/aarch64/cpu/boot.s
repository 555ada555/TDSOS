
.macro ADR_REL register, symbol
	adrp	\register, \symbol
	add	\register, \register, #:lo12:\symbol
.endm

.macro ADR_ABS register, symbol
	movz	\register, #:abs_g3:\symbol
	movk	\register, #:abs_g2_nc:\symbol
	movk	\register, #:abs_g1_nc:\symbol
	movk	\register, #:abs_g0_nc:\symbol
.endm

.section .text._start

_start:
	mrs	x0, CurrentEL
	cmp	x0, {CONST_CURRENTEL_EL2}
	b.ne	.L_parking_loop

	mrs	x1, MPIDR_EL1
	and	x1, x1, {CONST_CORE_ID_MASK}
	ldr	x2, BOOT_CORE_ID      
	cmp	x1, x2
	b.ne	.L_parking_loop

	ADR_REL	x0, __bss_start
	ADR_REL x1, __bss_end_exclusive

.L_bss_init_loop:
	cmp	x0, x1
	b.eq	.L_prepare_rust
	stp	xzr, xzr, [x0], #16
	b	.L_bss_init_loop

	
.L_prepare_rust:
	
	ldr	x0, PHYS_KERNEL_TABLES_BASE_ADDR 

	ADR_ABS	x1, __boot_core_stack_end_exclusive
	ADR_ABS	x2, kernel_init

	ADR_REL	x3, __boot_core_stack_end_exclusive
	mov	sp, x3

	ADR_REL	x4, ARCH_TIMER_COUNTER_FREQUENCY 
	mrs	x5, CNTFRQ_EL0
	cmp	x5, xzr
	b.eq	.L_parking_loop
	str	w5, [x4]
	b	_start_rust
.L_parking_loop:
	wfe
	b	.L_parking_loop

.size	_start, . - _start
.type	_start, function
.global	_start
