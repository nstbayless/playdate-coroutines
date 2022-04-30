asm(R"(
	.file	"coroutine.c"
	.text
	.type	stack_direction_helper, @function
stack_direction_helper:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	-12(%rbp), %rdx
	leaq	-24(%rbp), %rax
	cmpq	%rax, %rdx
	jnb	.L2
	movl	$-1, %eax
	jmp	.L4
.L2:
	movl	$1, %eax
.L4:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	stack_direction_helper, .-stack_direction_helper
	.local	pdco_first
	.comm	pdco_first,8,8
	.local	pdco_active
	.comm	pdco_active,8,8
	.data
	.align 4
	.type	next_coroutine_id, @object
	.size	next_coroutine_id, 4
next_coroutine_id:
	.long	1
	.text
	.type	delco, @function
delco:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	leaq	pdco_first(%rip), %rax
	movq	%rax, -16(%rbp)
	jmp	.L7
.L10:
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%eax, -20(%rbp)
	jne	.L7
	movq	-8(%rbp), %rax
	movq	8(%rax), %rdx
	movq	-16(%rbp), %rax
	movq	%rdx, (%rax)
	movq	-8(%rbp), %rax
	movq	1992(%rax), %rax
	testq	%rax, %rax
	je	.L8
	movq	-8(%rbp), %rax
	movq	1992(%rax), %rax
	movq	%rax, %rdi
	call	free@PLT
	movq	-8(%rbp), %rax
	movq	$0, 1992(%rax)
.L8:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movl	$1, %eax
	jmp	.L9
.L7:
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L10
	movl	$0, %eax
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	delco, .-delco
	.type	cleanup_if_complete, @function
cleanup_if_complete:
.LFB9:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	24(%rax), %eax
	testl	%eax, %eax
	jne	.L12
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	delco
	movl	$0, %eax
	jmp	.L13
.L12:
	movl	$1, %eax
.L13:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	cleanup_if_complete, .-cleanup_if_complete
	.type	getstackstart, @function
getstackstart:
.LFB10:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	1992(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	1984(%rax), %rax
	shrq	%rax
	addq	%rdx, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	getstackstart, .-getstackstart
	.type	getco, @function
getco:
.LFB11:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movq	pdco_first(%rip), %rax
	movq	%rax, -8(%rbp)
	jmp	.L17
.L19:
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -8(%rbp)
.L17:
	cmpq	$0, -8(%rbp)
	je	.L18
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	cmpl	%eax, -20(%rbp)
	jne	.L19
.L18:
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE11:
	.size	getco, .-getco
	.globl	pdco_yield
	.type	pdco_yield, @function
pdco_yield:
.LFB12:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	pdco_active(%rip), %rax
	leaq	48(%rax), %rdx
	movq	pdco_active(%rip), %rax
	addq	$1016, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	swapcontext@PLT
	endbr64
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE12:
	.size	pdco_yield, .-pdco_yield
	.type	pdco_guard, @function
pdco_guard:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	pdco_active(%rip), %rax
	movq	40(%rax), %rax
	call	*%rax
	movq	pdco_active(%rip), %rax
	movl	$0, 24(%rax)
	call	pdco_yield
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE13:
	.size	pdco_guard, .-pdco_guard
	.type	resume_epilogue, @function
resume_epilogue:
.LFB14:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	pdco_active(%rip), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, pdco_active(%rip)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	cleanup_if_complete
	testl	%eax, %eax
	je	.L24
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	jmp	.L26
.L24:
	movl	$0, %eax
.L26:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE14:
	.size	resume_epilogue, .-resume_epilogue
	.type	pdco_resume_, @function
pdco_resume_:
.LFB15:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L28
	movl	$-1, %eax
	jmp	.L29
.L28:
	movq	pdco_active(%rip), %rax
	cmpq	%rax, -8(%rbp)
	jne	.L30
	movl	$-1, %eax
	jmp	.L29
.L30:
	movq	pdco_active(%rip), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 16(%rax)
	movq	-8(%rbp), %rax
	movq	%rax, pdco_active(%rip)
	movq	-8(%rbp), %rax
	leaq	1016(%rax), %rdx
	movq	-8(%rbp), %rax
	addq	$48, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	swapcontext@PLT
	endbr64
	movl	$0, %eax
	call	resume_epilogue
.L29:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE15:
	.size	pdco_resume_, .-pdco_resume_
	.globl	pdco_run
	.type	pdco_run, @function
pdco_run:
.LFB16:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$2008, %edi
	call	malloc@PLT
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L32
	movl	$-1, %eax
	jmp	.L33
.L32:
	movq	-8(%rbp), %rax
	addq	$48, %rax
	movq	%rax, %rdi
	call	getcontext@PLT
	endbr64
	testl	%eax, %eax
	jns	.L35
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movl	$-1, %eax
	jmp	.L33
.L35:
	movq	-8(%rbp), %rax
	movq	$0, 1992(%rax)
	cmpq	$0, -32(%rbp)
	jne	.L36
	movq	$1024, -32(%rbp)
.L36:
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 1992(%rax)
	movq	-8(%rbp), %rax
	movq	1992(%rax), %rax
	testq	%rax, %rax
	jne	.L37
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free@PLT
	movl	$-1, %eax
	jmp	.L33
.L37:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	getstackstart
	movq	-8(%rbp), %rdx
	movq	%rax, 2000(%rdx)
	movq	-8(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, 1984(%rax)
	movq	-8(%rbp), %rax
	movl	$1, 24(%rax)
	movq	pdco_first(%rip), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 8(%rax)
	movq	-8(%rbp), %rax
	movq	%rax, pdco_first(%rip)
	movl	next_coroutine_id(%rip), %eax
	leal	1(%rax), %edx
	movl	%edx, next_coroutine_id(%rip)
	movq	-8(%rbp), %rdx
	movl	%eax, (%rdx)
	movq	-8(%rbp), %rax
	movq	-24(%rbp), %rdx
	movq	%rdx, 40(%rax)
	movq	-8(%rbp), %rax
	movq	2000(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 1032(%rax)
	movq	-8(%rbp), %rax
	movq	1984(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 1048(%rax)
	movq	-8(%rbp), %rax
	movq	$0, 1024(%rax)
	movq	-8(%rbp), %rax
	addq	$1016, %rax
	movl	$0, %edx
	leaq	pdco_guard(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	makecontext@PLT
	movq	-8(%rbp), %rax
	movq	1240(%rax), %rax
	testq	%rax, %rax
	jne	.L38
	movq	-8(%rbp), %rax
	leaq	1440(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 1240(%rax)
.L38:
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	pdco_resume_
.L33:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE16:
	.size	pdco_run, .-pdco_run
	.globl	pdco_get_coroutine
	.type	pdco_get_coroutine, @function
pdco_get_coroutine:
.LFB17:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	pdco_active(%rip), %rax
	testq	%rax, %rax
	jne	.L40
	movl	$0, %eax
	jmp	.L41
.L40:
	movq	pdco_active(%rip), %rax
	movl	(%rax), %eax
.L41:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE17:
	.size	pdco_get_coroutine, .-pdco_get_coroutine
	.globl	pdco_ud
	.type	pdco_ud, @function
pdco_ud:
.LFB18:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$24, %rsp
	movl	%edi, -20(%rbp)
	movq	pdco_active(%rip), %rax
	movq	%rax, -8(%rbp)
	cmpl	$0, -20(%rbp)
	jle	.L43
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	call	getco
	movq	%rax, -8(%rbp)
.L43:
	cmpq	$0, -8(%rbp)
	je	.L44
	movq	-8(%rbp), %rax
	addq	$32, %rax
	jmp	.L45
.L44:
	movl	$0, %eax
.L45:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE18:
	.size	pdco_ud, .-pdco_ud
	.globl	pdco_resume
	.type	pdco_resume, @function
pdco_resume:
.LFB19:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	call	getco
	movq	%rax, %rbx
	movq	%rbx, %rdi
	call	pdco_resume_
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE19:
	.size	pdco_resume, .-pdco_resume
	.globl	pdco_kill
	.type	pdco_kill, @function
pdco_kill:
.LFB20:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	pdco_active(%rip), %rax
	testq	%rax, %rax
	je	.L49
	movq	pdco_active(%rip), %rax
	movl	(%rax), %eax
	cmpl	%eax, -4(%rbp)
	jne	.L49
	movl	$-1, %eax
	jmp	.L50
.L49:
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	delco
	testl	%eax, %eax
	jne	.L51
	movl	-4(%rbp), %eax
	jmp	.L50
.L51:
	movl	$0, %eax
.L50:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE20:
	.size	pdco_kill, .-pdco_kill
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
)");
