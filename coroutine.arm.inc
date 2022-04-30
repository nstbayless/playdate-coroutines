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
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
	add	r2, sp, #12
	add	r3, sp, #4
	cmp	r2, r3
	mvncc	r0, #0
	movcs	r0, #1
	add	sp, sp, #16
	@ sp needed
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L13
	ldr	r4, [r3]
.L6:
	cmp	r4, #0
	beq	.L12
	ldr	r3, [r4]
	cmp	r3, r0
	bne	.L6
	ldr	r2, [r4, #4]
	ldr	r3, .L13
	str	r2, [r3]
	ldr	r0, [r4, #212]
	cmp	r0, #0
	blne	free
.L7:
	mov	r0, r4
	bl	free
	mov	r0, #1
	b	.L5
.L12:
	mov	r0, #0
.L5:
	pop	{r4, lr}
	bx	lr
.L14:
	.align	2
.L13:
	.word	pdco_first
	.size	delco, .-delco
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	cleanup_if_complete, %function
cleanup_if_complete:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r4, [r0, #12]
	cmp	r4, #0
	movne	r4, #1
	beq	.L19
.L15:
	mov	r0, r4
	pop	{r4, lr}
	bx	lr
.L19:
	ldr	r0, [r0]
	bl	delco
	b	.L15
	.size	cleanup_if_complete, .-cleanup_if_complete
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	getstackstart, %function
getstackstart:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r2, [r0, #208]
	ldr	r0, [r0, #212]
	add	r0, r0, r2, lsr #1
	bx	lr
	.size	getstackstart, .-getstackstart
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	getco, %function
getco:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, r0
	ldr	r3, .L27
	ldr	r0, [r3]
	cmp	r0, #0
	bxeq	lr
.L22:
	ldr	r3, [r0]
	cmp	r3, r2
	bxeq	lr
	ldr	r0, [r0, #4]
	cmp	r0, #0
	bne	.L22
	bx	lr
.L28:
	.align	2
.L27:
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
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L34
	ldr	r0, [r3]
	add	r0, r0, #116
	bl	setjmp
	cmp	r0, #0
	beq	.L33
	pop	{r4, lr}
	bx	lr
.L33:
	ldr	r3, .L34
	ldr	r0, [r3]
	mov	r1, #1
	add	r0, r0, #24
	bl	longjmp
.L35:
	.align	2
.L34:
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
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r4, .L38
	ldr	r3, [r4]
	ldr	r3, [r3, #20]
	mov	lr, pc
	bx	r3
	ldr	r3, [r4]
	mov	r2, #0
	str	r2, [r3, #12]
	bl	pdco_yield
	pop	{r4, lr}
	bx	lr
.L39:
	.align	2
.L38:
	.word	pdco_active
	.size	pdco_guard, .-pdco_guard
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	resume_epilogue, %function
resume_epilogue:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r3, .L43
	ldr	r4, [r3]
	ldr	r2, [r4, #8]
	str	r2, [r3]
	mov	r0, r4
	bl	cleanup_if_complete
	cmp	r0, #0
	ldrne	r0, [r4]
	pop	{r4, lr}
	bx	lr
.L44:
	.align	2
.L43:
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
	@ frame_needed = 0, uses_anonymous_args = 0
	str	lr, [sp, #-4]!
	sub	sp, sp, #12
	subs	r1, r0, #0
	str	r1, [sp, #4]
	beq	.L49
	ldr	r3, .L53
	ldr	r3, [r3]
	cmp	r3, r1
	beq	.L50
	ldr	r3, .L53
	ldr	r2, [r3]
	str	r2, [r1, #8]
	str	r1, [r3]
	add	r0, r1, #24
	bl	setjmp
	cmp	r0, #0
	beq	.L52
	bl	resume_epilogue
.L45:
	add	sp, sp, #12
	@ sp needed
	ldr	lr, [sp], #4
	bx	lr
.L52:
	mov	r1, #1
	ldr	r3, [sp, #4]
	add	r0, r3, #116
	bl	longjmp
.L49:
	mvn	r0, #0
	b	.L45
.L50:
	mvn	r0, #0
	b	.L45
.L54:
	.align	2
.L53:
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
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	sub	sp, sp, #8
	mov	r5, r0
	mov	r4, r1
	mov	r0, #220
	bl	malloc
	subs	r6, r0, #0
	str	r6, [sp, #4]
	beq	.L61
	cmp	r4, #0
	moveq	r4, #1024
	mov	r0, r4
	bl	malloc
	str	r0, [r6, #212]
	cmp	r0, #0
	beq	.L63
	ldr	r6, [sp, #4]
	mov	r0, r6
	bl	getstackstart
	str	r0, [r6, #216]
	str	r4, [r6, #208]
	mov	r3, #1
	str	r3, [r6, #12]
	ldr	r3, .L65
	ldr	r2, [r3]
	str	r2, [r6, #4]
	str	r6, [r3]
	ldr	r2, .L65+4
	ldr	r3, [r2]
	add	r1, r3, #1
	str	r1, [r2]
	str	r3, [r6]
	str	r5, [r6, #20]
	add	r0, r6, #116
	bl	setjmp
	cmp	r0, #0
	bne	.L64
.L60:
	ldr	r0, [sp, #4]
	bl	pdco_resume_
.L55:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, r5, r6, lr}
	bx	lr
.L63:
	ldr	r0, [sp, #4]
	bl	free
	mvn	r0, #0
	b	.L55
.L64:
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #216]
	.syntax divided
@ 250 "coroutine.c" 1
	mov sp, r3
@ 0 "" 2
	.arm
	.syntax unified
	bl	pdco_guard
	b	.L60
.L61:
	mvn	r0, #0
	b	.L55
.L66:
	.align	2
.L65:
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
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L70
	ldr	r3, [r3]
	cmp	r3, #0
	ldrne	r3, .L70
	ldrne	r3, [r3]
	ldrne	r0, [r3]
	moveq	r0, #0
	bx	lr
.L71:
	.align	2
.L70:
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L82
	ldr	r3, [r3]
	cmp	r0, #0
	bgt	.L81
	cmp	r3, #0
	addne	r3, r3, #16
	mov	r0, r3
	bx	lr
.L81:
	push	{r4, lr}
	bl	getco
	mov	r3, r0
	cmp	r0, #0
	addne	r3, r0, #16
	mov	r0, r3
	pop	{r4, lr}
	bx	lr
.L83:
	.align	2
.L82:
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	bl	getco
	bl	pdco_resume_
	pop	{r4, lr}
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
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	mov	r4, r0
	ldr	r3, .L92
	ldr	r3, [r3]
	cmp	r3, #0
	beq	.L87
	ldr	r3, .L92
	ldr	r3, [r3]
	ldr	r3, [r3]
	cmp	r3, r0
	beq	.L89
.L87:
	mov	r0, r4
	bl	delco
	cmp	r0, #0
	moveq	r0, r4
	movne	r0, #0
.L86:
	pop	{r4, lr}
	bx	lr
.L89:
	mvn	r0, #0
	b	.L86
.L93:
	.align	2
.L92:
	.word	pdco_active
	.size	pdco_kill, .-pdco_kill
	.ident	"GCC: (15:9-2019-q4-0ubuntu1) 9.2.1 20191025 (release) [ARM/arm-9-branch revision 277599]"
)");
