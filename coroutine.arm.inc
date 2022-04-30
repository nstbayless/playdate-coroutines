asm(R"(
	.cpu arm7tdmi
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"coroutine.c"
	.text
	.align	2
	.arch armv4t
	.syntax unified
	.arm
	.fpu softvfp
	.type	stack_direction_helper, %function
stack_direction_helper:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #20
	str	r0, [fp, #-16]
	sub	r2, fp, #8
	sub	r3, fp, #16
	cmp	r2, r3
	bcs	.L2
	mvn	r3, #0
	b	.L4
.L2:
	mov	r3, #1
.L4:
	mov	r0, r3
	add	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	stack_direction_helper, .-stack_direction_helper
	.bss
	.align	2
pdco_first:
	.space	4
	.size	pdco_first, 4
	.align	2
pdco_active:
	.space	4
	.size	pdco_active, 4
	.data
	.align	2
	.type	next_coroutine_id, %object
	.size	next_coroutine_id, 4
next_coroutine_id:
	.word	1
	.text
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	delco, %function
delco:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	ldr	r3, .L10
	str	r3, [fp, #-8]
	b	.L6
.L9:
	ldr	r3, [fp, #-12]
	ldr	r3, [r3]
	ldr	r2, [fp, #-16]
	cmp	r2, r3
	bne	.L6
	ldr	r3, [fp, #-12]
	ldr	r2, [r3, #4]
	ldr	r3, [fp, #-8]
	str	r2, [r3]
	ldr	r3, [fp, #-12]
	ldr	r3, [r3, #212]
	cmp	r3, #0
	beq	.L7
	ldr	r3, [fp, #-12]
	ldr	r3, [r3, #212]
	mov	r0, r3
	bl	free
	ldr	r3, [fp, #-12]
	mov	r2, #0
	str	r2, [r3, #212]
.L7:
	ldr	r0, [fp, #-12]
	bl	free
	mov	r3, #1
	b	.L8
.L6:
	ldr	r3, [fp, #-8]
	ldr	r3, [r3]
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-12]
	cmp	r3, #0
	bne	.L9
	mov	r3, #0
.L8:
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L11:
	.align	2
.L10:
	.word	pdco_first
	.size	delco, .-delco
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	cleanup_if_complete, %function
cleanup_if_complete:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #12]
	cmp	r3, #0
	bne	.L13
	ldr	r3, [fp, #-8]
	ldr	r3, [r3]
	mov	r0, r3
	bl	delco
	mov	r3, #0
	b	.L14
.L13:
	mov	r3, #1
.L14:
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
	.size	cleanup_if_complete, .-cleanup_if_complete
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	getstackstart, %function
getstackstart:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r2, [r3, #212]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #208]
	lsr	r3, r3, #1
	add	r3, r2, r3
	mov	r0, r3
	add	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	getstackstart, .-getstackstart
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	getco, %function
getco:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #20
	str	r0, [fp, #-16]
	ldr	r3, .L22
	ldr	r3, [r3]
	str	r3, [fp, #-8]
	b	.L18
.L20:
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #4]
	str	r3, [fp, #-8]
.L18:
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	beq	.L19
	ldr	r3, [fp, #-8]
	ldr	r3, [r3]
	ldr	r2, [fp, #-16]
	cmp	r2, r3
	bne	.L20
.L19:
	ldr	r3, [fp, #-8]
	mov	r0, r3
	add	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
.L23:
	.align	2
.L22:
	.word	pdco_first
	.size	getco, .-getco
	.align	2
	.global	pdco_yield
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_yield, %function
pdco_yield:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	ldr	r3, .L28
	ldr	r3, [r3]
	add	r3, r3, #116
	mov	r0, r3
	bl	setjmp
	mov	r3, r0
	cmp	r3, #0
	bne	.L27
	ldr	r3, .L28
	ldr	r3, [r3]
	add	r3, r3, #24
	mov	r1, #1
	mov	r0, r3
	bl	longjmp
.L27:
	nop
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L29:
	.align	2
.L28:
	.word	pdco_active
	.size	pdco_yield, .-pdco_yield
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_guard, %function
pdco_guard:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	ldr	r3, .L31
	ldr	r3, [r3]
	ldr	r3, [r3, #20]
	mov	lr, pc
	bx	r3
	ldr	r3, .L31
	ldr	r3, [r3]
	mov	r2, #0
	str	r2, [r3, #12]
	bl	pdco_yield
	nop
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L32:
	.align	2
.L31:
	.word	pdco_active
	.size	pdco_guard, .-pdco_guard
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	resume_epilogue, %function
resume_epilogue:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	ldr	r3, .L37
	ldr	r3, [r3]
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #8]
	ldr	r2, .L37
	str	r3, [r2]
	ldr	r0, [fp, #-8]
	bl	cleanup_if_complete
	mov	r3, r0
	cmp	r3, #0
	beq	.L34
	ldr	r3, [fp, #-8]
	ldr	r3, [r3]
	b	.L36
.L34:
	mov	r3, #0
.L36:
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L38:
	.align	2
.L37:
	.word	pdco_active
	.size	resume_epilogue, .-resume_epilogue
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_resume_, %function
pdco_resume_:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	bne	.L40
	mvn	r3, #0
	b	.L41
.L40:
	ldr	r3, .L45
	ldr	r3, [r3]
	ldr	r2, [fp, #-8]
	cmp	r2, r3
	bne	.L42
	mvn	r3, #0
	b	.L41
.L42:
	ldr	r3, .L45
	ldr	r2, [r3]
	ldr	r3, [fp, #-8]
	str	r2, [r3, #8]
	ldr	r2, .L45
	ldr	r3, [fp, #-8]
	str	r3, [r2]
	ldr	r3, [fp, #-8]
	add	r3, r3, #24
	mov	r0, r3
	bl	setjmp
	mov	r3, r0
	cmp	r3, #0
	bne	.L44
	ldr	r3, [fp, #-8]
	add	r3, r3, #116
	mov	r1, #1
	mov	r0, r3
	bl	longjmp
.L44:
	bl	resume_epilogue
	mov	r3, r0
.L41:
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L46:
	.align	2
.L45:
	.word	pdco_active
	.size	pdco_resume_, .-pdco_resume_
	.align	2
	.global	pdco_run
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_run, %function
pdco_run:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	mov	r0, #220
	bl	malloc
	mov	r3, r0
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	bne	.L48
	mvn	r3, #0
	b	.L49
.L48:
	ldr	r3, [fp, #-8]
	mov	r2, #0
	str	r2, [r3, #212]
	ldr	r3, [fp, #-20]
	cmp	r3, #0
	bne	.L50
	mov	r3, #1024
	str	r3, [fp, #-20]
.L50:
	ldr	r0, [fp, #-20]
	bl	malloc
	mov	r3, r0
	mov	r2, r3
	ldr	r3, [fp, #-8]
	str	r2, [r3, #212]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #212]
	cmp	r3, #0
	bne	.L51
	ldr	r0, [fp, #-8]
	bl	free
	mvn	r3, #0
	b	.L49
.L51:
	ldr	r0, [fp, #-8]
	bl	getstackstart
	mov	r2, r0
	ldr	r3, [fp, #-8]
	str	r2, [r3, #216]
	ldr	r3, [fp, #-8]
	ldr	r2, [fp, #-20]
	str	r2, [r3, #208]
	ldr	r3, [fp, #-8]
	mov	r2, #1
	str	r2, [r3, #12]
	ldr	r3, .L54
	ldr	r2, [r3]
	ldr	r3, [fp, #-8]
	str	r2, [r3, #4]
	ldr	r2, .L54
	ldr	r3, [fp, #-8]
	str	r3, [r2]
	ldr	r3, .L54+4
	ldr	r3, [r3]
	add	r2, r3, #1
	ldr	r1, .L54+4
	str	r2, [r1]
	ldr	r2, [fp, #-8]
	str	r3, [r2]
	ldr	r3, [fp, #-8]
	ldr	r2, [fp, #-16]
	str	r2, [r3, #20]
	ldr	r3, [fp, #-8]
	add	r3, r3, #116
	mov	r0, r3
	bl	setjmp
	mov	r3, r0
	cmp	r3, #0
	beq	.L53
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #216]
	.syntax divided
@ 252 "coroutine.c" 1
	mov sp, r3
@ 0 "" 2
	.arm
	.syntax unified
	bl	pdco_guard
.L53:
	ldr	r0, [fp, #-8]
	bl	pdco_resume_
	mov	r3, r0
.L49:
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L55:
	.align	2
.L54:
	.word	pdco_first
	.word	next_coroutine_id
	.size	pdco_run, .-pdco_run
	.align	2
	.global	pdco_get_coroutine
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_get_coroutine, %function
pdco_get_coroutine:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	ldr	r3, .L59
	ldr	r3, [r3]
	cmp	r3, #0
	bne	.L57
	mov	r3, #0
	b	.L58
.L57:
	ldr	r3, .L59
	ldr	r3, [r3]
	ldr	r3, [r3]
.L58:
	mov	r0, r3
	add	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
.L60:
	.align	2
.L59:
	.word	pdco_active
	.size	pdco_get_coroutine, .-pdco_get_coroutine
	.align	2
	.global	pdco_ud
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_ud, %function
pdco_ud:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	ldr	r3, .L65
	ldr	r3, [r3]
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-16]
	cmp	r3, #0
	ble	.L62
	ldr	r0, [fp, #-16]
	bl	getco
	str	r0, [fp, #-8]
.L62:
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	beq	.L63
	ldr	r3, [fp, #-8]
	add	r3, r3, #16
	b	.L64
.L63:
	mov	r3, #0
.L64:
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L66:
	.align	2
.L65:
	.word	pdco_active
	.size	pdco_ud, .-pdco_ud
	.align	2
	.global	pdco_resume
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_resume, %function
pdco_resume:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #12
	str	r0, [fp, #-16]
	ldr	r0, [fp, #-16]
	bl	getco
	mov	r4, r0
	mov	r0, r4
	bl	pdco_resume_
	mov	r3, r0
	mov	r0, r3
	sub	sp, fp, #8
	@ sp needed
	pop	{r4, fp, lr}
	bx	lr
	.size	pdco_resume, .-pdco_resume
	.align	2
	.global	pdco_kill
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_kill, %function
pdco_kill:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, .L74
	ldr	r3, [r3]
	cmp	r3, #0
	beq	.L70
	ldr	r3, .L74
	ldr	r3, [r3]
	ldr	r3, [r3]
	ldr	r2, [fp, #-8]
	cmp	r2, r3
	bne	.L70
	mvn	r3, #0
	b	.L71
.L70:
	ldr	r0, [fp, #-8]
	bl	delco
	mov	r3, r0
	cmp	r3, #0
	bne	.L72
	ldr	r3, [fp, #-8]
	b	.L71
.L72:
	mov	r3, #0
.L71:
	mov	r0, r3
	sub	sp, fp, #4
	@ sp needed
	pop	{fp, lr}
	bx	lr
.L75:
	.align	2
.L74:
	.word	pdco_active
	.size	pdco_kill, .-pdco_kill
	.ident	"GCC: (15:9-2019-q4-0ubuntu1) 9.2.1 20191025 (release) [ARM/arm-9-branch revision 277599]"
)");
