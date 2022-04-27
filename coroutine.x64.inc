asm(R"(
	.file	"coroutine.c"
	.text
	.type	stack_direction_helper, @function
stack_direction_helper:
.LFB6:
	.cfi_startproc
	endbr64
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, 8(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	leaq	20(%rsp), %rdx
	leaq	8(%rsp), %rax
	cmpq	%rax, %rdx
	sbbl	%eax, %eax
	orl	$1, %eax
	movq	24(%rsp), %rcx
	xorq	%fs:40, %rcx
	jne	.L6
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6:
	.size	stack_direction_helper, .-stack_direction_helper
	.type	stack_direction, @function
stack_direction:
.LFB7:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movl	$40, %ebx
	movq	%fs:(%rbx), %rax
	movq	%rax, 8(%rsp)
	xorl	%eax, %eax
	leaq	4(%rsp), %rdi
	call	stack_direction_helper
	movq	8(%rsp), %rdx
	xorq	%fs:(%rbx), %rdx
	jne	.L10
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L10:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE7:
	.size	stack_direction, .-stack_direction
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
	movl	%edi, %eax
	movq	pdco_first(%rip), %rdi
.L12:
	testq	%rdi, %rdi
	je	.L20
	cmpl	%eax, (%rdi)
	jne	.L12
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	8(%rdi), %rax
	movq	%rax, pdco_first(%rip)
	call	free@PLT
	movl	$1, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
.L20:
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE8:
	.size	delco, .-delco
	.type	cleanup_if_complete, @function
cleanup_if_complete:
.LFB9:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	24(%rdi), %ebx
	testl	%ebx, %ebx
	je	.L25
	movl	$1, %ebx
.L21:
	movl	%ebx, %eax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L25:
	.cfi_restore_state
	movl	(%rdi), %edi
	call	delco
	jmp	.L21
	.cfi_endproc
.LFE9:
	.size	cleanup_if_complete, .-cleanup_if_complete
	.type	getstackstart, @function
getstackstart:
.LFB10:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movl	$0, %eax
	call	stack_direction
	cmpl	$1, %eax
	je	.L30
	movq	440(%rbx), %rax
	movq	448(%rbx), %rdx
	leaq	-48(%rdx,%rax), %rax
.L26:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L30:
	.cfi_restore_state
	movq	448(%rbx), %rax
	addq	$48, %rax
	jmp	.L26
	.cfi_endproc
.LFE10:
	.size	getstackstart, .-getstackstart
	.type	getco, @function
getco:
.LFB11:
	.cfi_startproc
	endbr64
	movq	pdco_first(%rip), %rax
	testq	%rax, %rax
	je	.L36
.L32:
	cmpl	%edi, (%rax)
	je	.L31
	movq	8(%rax), %rax
	testq	%rax, %rax
	jne	.L32
.L31:
	ret
.L36:
	ret
	.cfi_endproc
.LFE11:
	.size	getco, .-getco
	.globl	pdco_run
	.type	pdco_run, @function
pdco_run:
.LFB12:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$40, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, %r12
	movq	%rsi, %rbx
	movq	%rdx, %rbp
	movq	%rcx, 16(%rsp)
	movl	$472, %edi
	call	malloc@PLT
	movq	%rax, 8(%rsp)
	testq	%rax, %rax
	je	.L45
	movq	%rax, %r14
	testq	%rbx, %rbx
	movl	$16384, %eax
	cmove	%rax, %rbx
	movq	%rbx, %rdi
	call	malloc@PLT
	movq	%rax, 448(%r14)
	testq	%rax, %rax
	je	.L47
	movq	8(%rsp), %r14
	movq	%r14, %rdi
	call	getstackstart
	movq	%rax, 456(%r14)
	movl	$1, 24(%r14)
	movq	pdco_active(%rip), %rax
	movq	%rax, 16(%r14)
	movq	%rbx, 440(%r14)
	movq	pdco_first(%rip), %rax
	movq	%rax, 8(%r14)
	movl	next_coroutine_id(%rip), %eax
	leal	1(%rax), %edx
	movl	%edx, next_coroutine_id(%rip)
	movl	%eax, (%r14)
	movq	%r12, 32(%r14)
	movq	%rbp, 464(%r14)
	leaq	240(%r14), %rax
	movq	%rax, 24(%rsp)
	movq	%rax, %rdi
	call	_setjmp@PLT
	endbr64
	testl	%eax, %eax
	jne	.L48
	movq	8(%rsp), %rsi
	movq	16(%rsi), %rax
	movq	%rax, pdco_active(%rip)
	movq	%rsi, pdco_first(%rip)
	leaq	40(%rsi), %rdi
	call	_setjmp@PLT
	endbr64
	testl	%eax, %eax
	je	.L49
	movq	8(%rsp), %rbx
	movq	16(%rbx), %rax
	movq	%rax, pdco_active(%rip)
	movq	16(%rsp), %rdx
	testq	%rdx, %rdx
	je	.L44
	movq	464(%rbx), %rax
	movq	%rax, (%rdx)
.L44:
	movq	8(%rsp), %rbx
	movq	%rbx, %rdi
	call	cleanup_if_complete
	testl	%eax, %eax
	je	.L37
	movl	(%rbx), %eax
.L37:
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L47:
	.cfi_restore_state
	movq	8(%rsp), %rdi
	call	free@PLT
	movl	$-1, %eax
	jmp	.L37
.L48:
	movq	8(%rsp), %rax
	movq	456(%rax), %rax
	movq %rax, %rsp 
 movq %rax, %rbp
	movq	pdco_active(%rip), %rax
	movq	pdco_active(%rip), %rdx
	movq	pdco_active(%rip), %rbx
	movq	464(%rdx), %rdi
	call	*32(%rax)
	movq	%rax, 464(%rbx)
	movq	pdco_active(%rip), %rax
	movl	$0, 24(%rax)
	movq	pdco_active(%rip), %rdi
	addq	$40, %rdi
	movl	$1, %esi
	call	longjmp@PLT
.L49:
	movq	8(%rsp), %rax
	movq	%rax, pdco_active(%rip)
	movl	$1, %esi
	movq	24(%rsp), %rdi
	call	longjmp@PLT
.L45:
	movl	$-1, %eax
	jmp	.L37
	.cfi_endproc
.LFE12:
	.size	pdco_run, .-pdco_run
	.globl	pdco_yield
	.type	pdco_yield, @function
pdco_yield:
.LFB13:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	pdco_active(%rip), %rax
	movq	%rdi, 464(%rax)
	movq	pdco_active(%rip), %rdi
	addq	$240, %rdi
	call	_setjmp@PLT
	endbr64
	testl	%eax, %eax
	je	.L54
	movq	pdco_active(%rip), %rax
	movq	464(%rax), %rax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L54:
	.cfi_restore_state
	movq	pdco_active(%rip), %rdi
	addq	$40, %rdi
	movl	$1, %esi
	call	longjmp@PLT
	.cfi_endproc
.LFE13:
	.size	pdco_yield, .-pdco_yield
	.globl	pdco_resume
	.type	pdco_resume, @function
pdco_resume:
.LFB14:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movq	%rsi, %rbx
	movq	%rdx, 8(%rsp)
	call	getco
	movq	%rax, (%rsp)
	testq	%rax, %rax
	je	.L60
	movq	%rax, %rcx
	movq	pdco_active(%rip), %rax
	cmpq	%rcx, %rax
	je	.L61
	movq	pdco_active(%rip), %rax
	movq	%rax, 16(%rcx)
	movq	%rbx, 464(%rcx)
	leaq	40(%rcx), %rdi
	call	_setjmp@PLT
	endbr64
	testl	%eax, %eax
	je	.L63
	movq	(%rsp), %rbx
	movq	16(%rbx), %rax
	movq	%rax, pdco_active(%rip)
	movq	8(%rsp), %rdx
	testq	%rdx, %rdx
	je	.L59
	movq	464(%rbx), %rax
	movq	%rax, (%rdx)
.L59:
	movq	(%rsp), %rbx
	movq	%rbx, %rdi
	call	cleanup_if_complete
	testl	%eax, %eax
	je	.L55
	movl	(%rbx), %eax
.L55:
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L63:
	.cfi_restore_state
	movq	(%rsp), %rax
	movq	%rax, pdco_active(%rip)
	leaq	240(%rax), %rdi
	movl	$1, %esi
	call	longjmp@PLT
.L60:
	movl	$-1, %eax
	jmp	.L55
.L61:
	movl	$-1, %eax
	jmp	.L55
	.cfi_endproc
.LFE14:
	.size	pdco_resume, .-pdco_resume
	.globl	pdco_kill
	.type	pdco_kill, @function
pdco_kill:
.LFB15:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%edi, %ebx
	movq	pdco_active(%rip), %rax
	testq	%rax, %rax
	je	.L65
	movq	pdco_active(%rip), %rax
	cmpl	%edi, (%rax)
	je	.L67
.L65:
	movl	%ebx, %edi
	call	delco
	testl	%eax, %eax
	movl	$0, %eax
	cmove	%ebx, %eax
.L64:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L67:
	.cfi_restore_state
	movl	$-1, %eax
	jmp	.L64
	.cfi_endproc
.LFE15:
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
