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
	.align	2
	.syntax unified
	.arm
	.fpu softvfp
	.type	stack_direction, %function
stack_direction:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	str	lr, [sp, #-4]!
	sub	sp, sp, #12
	add	r0, sp, #4
	bl	stack_direction_helper
	add	sp, sp, #12
	@ sp needed
	ldr	lr, [sp], #4
	bx	lr
	.size	stack_direction, .-stack_direction
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
	mov	r2, r0
	ldr	r3, .L17
	ldr	r0, [r3]
.L8:
	cmp	r0, #0
	bxeq	lr
	ldr	r3, [r0]
	cmp	r3, r2
	bne	.L8
	push	{r4, lr}
	ldr	r2, [r0, #4]
	ldr	r3, .L17
	str	r2, [r3]
	bl	free
	mov	r0, #1
	pop	{r4, lr}
	bx	lr
.L18:
	.align	2
.L17:
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
	beq	.L23
.L19:
	mov	r0, r4
	pop	{r4, lr}
	bx	lr
.L23:
	ldr	r0, [r0]
	bl	delco
	b	.L19
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
	push	{r4, lr}
	mov	r4, r0
	bl	stack_direction
	cmp	r0, #1
	ldreq	r0, [r4, #208]
	addeq	r0, r0, #48
	ldrne	r0, [r4, #204]
	subne	r3, r0, #48
	ldrne	r0, [r4, #208]
	addne	r0, r0, r3
	pop	{r4, lr}
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
	ldr	r3, .L34
	ldr	r0, [r3]
	cmp	r0, #0
	bxeq	lr
.L29:
	ldr	r3, [r0]
	cmp	r3, r2
	bxeq	lr
	ldr	r0, [r0, #4]
	cmp	r0, #0
	bne	.L29
	bx	lr
.L35:
	.align	2
.L34:
	.word	pdco_first
	.size	getco, .-getco
	.align	2
	.global	pdco_run
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_run, %function
pdco_run:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, lr}
	sub	sp, sp, #20
	mov	r6, r0
	mov	r4, r1
	mov	r5, r2
	str	r3, [sp, #8]
	mov	r0, #220
	bl	malloc
	subs	r7, r0, #0
	str	r7, [sp, #4]
	beq	.L44
	cmp	r4, #0
	moveq	r4, #16384
	mov	r0, r4
	bl	malloc
	str	r0, [r7, #208]
	cmp	r0, #0
	beq	.L46
	ldr	r7, [sp, #4]
	mov	r0, r7
	bl	getstackstart
	str	r0, [r7, #212]
	mov	r3, #1
	str	r3, [r7, #12]
	ldr	r3, .L49
	ldr	r3, [r3]
	str	r3, [r7, #8]
	str	r4, [r7, #204]
	ldr	r3, .L49+4
	ldr	r3, [r3]
	str	r3, [r7, #4]
	ldr	r2, .L49+8
	ldr	r3, [r2]
	add	r1, r3, #1
	str	r1, [r2]
	str	r3, [r7]
	str	r6, [r7, #16]
	str	r5, [r7, #216]
	add	r3, r7, #112
	str	r3, [sp, #12]
	mov	r0, r3
	bl	setjmp
	cmp	r0, #0
	bne	.L47
	ldr	r1, [sp, #4]
	ldr	r2, [r1, #8]
	ldr	r3, .L49
	str	r2, [r3]
	ldr	r3, .L49+4
	str	r1, [r3]
	add	r0, r1, #20
	bl	setjmp
	cmp	r0, #0
	beq	.L48
	ldr	r1, [sp, #4]
	ldr	r2, [r1, #8]
	ldr	r3, .L49
	str	r2, [r3]
	ldr	r2, [sp, #8]
	cmp	r2, #0
	ldrne	r3, [r1, #216]
	strne	r3, [r2]
	ldr	r4, [sp, #4]
	mov	r0, r4
	bl	cleanup_if_complete
	cmp	r0, #0
	ldrne	r0, [r4]
.L36:
	add	sp, sp, #20
	@ sp needed
	pop	{r4, r5, r6, r7, lr}
	bx	lr
.L46:
	ldr	r0, [sp, #4]
	bl	free
	mvn	r0, #0
	b	.L36
.L47:
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #212]
	.syntax divided
@ 160 "coroutine.c" 1
	mov sp, r3
@ 0 "" 2
	.arm
	.syntax unified
	ldr	r4, .L49
	ldr	r3, [r4]
	ldr	r2, [r4]
	ldr	r5, [r4]
	ldr	r3, [r3, #16]
	ldr	r0, [r2, #216]
	mov	lr, pc
	bx	r3
	str	r0, [r5, #216]
	ldr	r3, [r4]
	mov	r2, #0
	str	r2, [r3, #12]
	ldr	r0, [r4]
	mov	r1, #1
	add	r0, r0, #20
	bl	longjmp
.L48:
	ldr	r3, .L49
	ldr	r2, [sp, #4]
	str	r2, [r3]
	mov	r1, #1
	ldr	r0, [sp, #12]
	bl	longjmp
.L44:
	mvn	r0, #0
	b	.L36
.L50:
	.align	2
.L49:
	.word	pdco_active
	.word	pdco_first
	.word	next_coroutine_id
	.size	pdco_run, .-pdco_run
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
	ldr	r3, .L56
	ldr	r2, [r3]
	str	r0, [r2, #216]
	ldr	r0, [r3]
	add	r0, r0, #112
	bl	setjmp
	cmp	r0, #0
	beq	.L55
	ldr	r3, .L56
	ldr	r3, [r3]
	ldr	r0, [r3, #216]
	pop	{r4, lr}
	bx	lr
.L55:
	ldr	r3, .L56
	ldr	r0, [r3]
	mov	r1, #1
	add	r0, r0, #20
	bl	longjmp
.L57:
	.align	2
.L56:
	.word	pdco_active
	.size	pdco_yield, .-pdco_yield
	.align	2
	.global	pdco_resume
	.syntax unified
	.arm
	.fpu softvfp
	.type	pdco_resume, %function
pdco_resume:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	sub	sp, sp, #8
	mov	r4, r1
	str	r2, [sp, #4]
	bl	getco
	subs	r2, r0, #0
	str	r2, [sp]
	beq	.L63
	ldr	r3, .L67
	ldr	r3, [r3]
	cmp	r3, r2
	beq	.L64
	ldr	r3, .L67
	ldr	r3, [r3]
	str	r3, [r2, #8]
	str	r4, [r2, #216]
	add	r0, r2, #20
	bl	setjmp
	cmp	r0, #0
	beq	.L66
	ldr	r1, [sp]
	ldr	r2, [r1, #8]
	ldr	r3, .L67
	str	r2, [r3]
	ldr	r2, [sp, #4]
	cmp	r2, #0
	ldrne	r3, [r1, #216]
	strne	r3, [r2]
	ldr	r4, [sp]
	mov	r0, r4
	bl	cleanup_if_complete
	cmp	r0, #0
	ldrne	r0, [r4]
.L58:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, lr}
	bx	lr
.L66:
	ldr	r3, .L67
	ldr	r2, [sp]
	str	r2, [r3]
	mov	r1, #1
	add	r0, r2, #112
	bl	longjmp
.L63:
	mvn	r0, #0
	b	.L58
.L64:
	mvn	r0, #0
	b	.L58
.L68:
	.align	2
.L67:
	.word	pdco_active
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
	ldr	r3, .L75
	ldr	r3, [r3]
	cmp	r3, #0
	beq	.L70
	ldr	r3, .L75
	ldr	r3, [r3]
	ldr	r3, [r3]
	cmp	r3, r0
	beq	.L72
.L70:
	mov	r0, r4
	bl	delco
	cmp	r0, #0
	moveq	r0, r4
	movne	r0, #0
.L69:
	pop	{r4, lr}
	bx	lr
.L72:
	mvn	r0, #0
	b	.L69
.L76:
	.align	2
.L75:
	.word	pdco_active
	.size	pdco_kill, .-pdco_kill
	.ident	"GCC: (15:9-2019-q4-0ubuntu1) 9.2.1 20191025 (release) [ARM/arm-9-branch revision 277599]"
)");
