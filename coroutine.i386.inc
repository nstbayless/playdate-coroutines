asm(R"(
	.file	"coroutine.c"
	.text
	.type	stack_direction_helper, @function
stack_direction_helper:
.LFB6:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	leal	-16(%ebp), %edx
	leal	-28(%ebp), %eax
	cmpl	%eax, %edx
	jnb	.L2
	movl	$-1, %eax
	jmp	.L4
.L2:
	movl	$1, %eax
.L4:
	movl	-12(%ebp), %ecx
	xorl	%gs:20, %ecx
	je	.L5
	call	__stack_chk_fail_local
.L5:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	stack_direction_helper, .-stack_direction_helper
	.local	pdco_first
	.comm	pdco_first,4,4
	.local	pdco_active
	.comm	pdco_active,4,4
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
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$20, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	leal	pdco_first@GOTOFF(%ebx), %eax
	movl	%eax, -16(%ebp)
	jmp	.L7
.L10:
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	cmpl	%eax, 8(%ebp)
	jne	.L7
	movl	-12(%ebp), %eax
	movl	4(%eax), %edx
	movl	-16(%ebp), %eax
	movl	%edx, (%eax)
	movl	-12(%ebp), %eax
	movl	756(%eax), %eax
	testl	%eax, %eax
	je	.L8
	movl	-12(%ebp), %eax
	movl	756(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	free@PLT
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movl	$0, 756(%eax)
.L8:
	subl	$12, %esp
	pushl	-12(%ebp)
	call	free@PLT
	addl	$16, %esp
	movl	$1, %eax
	jmp	.L9
.L7:
	movl	-16(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	.L10
	movl	$0, %eax
.L9:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	delco, .-delco
	.type	cleanup_if_complete, @function
cleanup_if_complete:
.LFB9:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
	movl	12(%eax), %eax
	testl	%eax, %eax
	jne	.L12
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	subl	$12, %esp
	pushl	%eax
	call	delco
	addl	$16, %esp
	movl	$0, %eax
	jmp	.L13
.L12:
	movl	$1, %eax
.L13:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	cleanup_if_complete, .-cleanup_if_complete
	.type	getstackstart, @function
getstackstart:
.LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
	movl	756(%eax), %edx
	movl	8(%ebp), %eax
	movl	752(%eax), %eax
	shrl	%eax
	addl	%edx, %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	getstackstart, .-getstackstart
	.type	getco, @function
getco:
.LFB11:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	pdco_first@GOTOFF(%eax), %eax
	movl	%eax, -4(%ebp)
	jmp	.L17
.L19:
	movl	-4(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, -4(%ebp)
.L17:
	cmpl	$0, -4(%ebp)
	je	.L18
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	cmpl	%eax, 8(%ebp)
	jne	.L19
.L18:
	movl	-4(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	getco, .-getco
	.globl	pdco_yield
	.type	pdco_yield, @function
pdco_yield:
.LFB12:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$4, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	pdco_active@GOTOFF(%eax), %edx
	leal	24(%edx), %ecx
	movl	pdco_active@GOTOFF(%eax), %edx
	addl	$388, %edx
	subl	$8, %esp
	pushl	%ecx
	pushl	%edx
	movl	%eax, %ebx
	call	swapcontext@PLT
	endbr32
	addl	$16, %esp
	nop
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE12:
	.size	pdco_yield, .-pdco_yield
	.type	pdco_guard, @function
pdco_guard:
.LFB13:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$4, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	pdco_active@GOTOFF(%ebx), %eax
	movl	20(%eax), %eax
	call	*%eax
	movl	pdco_active@GOTOFF(%ebx), %eax
	movl	$0, 12(%eax)
	call	pdco_yield
	nop
	addl	$4, %esp
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE13:
	.size	pdco_guard, .-pdco_guard
	.type	resume_epilogue, @function
resume_epilogue:
.LFB14:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	pdco_active@GOTOFF(%eax), %edx
	movl	%edx, -12(%ebp)
	movl	-12(%ebp), %edx
	movl	8(%edx), %edx
	movl	%edx, pdco_active@GOTOFF(%eax)
	subl	$12, %esp
	pushl	-12(%ebp)
	call	cleanup_if_complete
	addl	$16, %esp
	testl	%eax, %eax
	je	.L24
	movl	-12(%ebp), %eax
	movl	(%eax), %eax
	jmp	.L26
.L24:
	movl	$0, %eax
.L26:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE14:
	.size	resume_epilogue, .-resume_epilogue
	.type	pdco_resume_, @function
pdco_resume_:
.LFB15:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$4, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	cmpl	$0, 8(%ebp)
	jne	.L28
	movl	$-1, %eax
	jmp	.L29
.L28:
	movl	pdco_active@GOTOFF(%eax), %edx
	cmpl	%edx, 8(%ebp)
	jne	.L30
	movl	$-1, %eax
	jmp	.L29
.L30:
	movl	pdco_active@GOTOFF(%eax), %ecx
	movl	8(%ebp), %edx
	movl	%ecx, 8(%edx)
	movl	8(%ebp), %edx
	movl	%edx, pdco_active@GOTOFF(%eax)
	movl	8(%ebp), %edx
	leal	388(%edx), %ecx
	movl	8(%ebp), %edx
	addl	$24, %edx
	subl	$8, %esp
	pushl	%ecx
	pushl	%edx
	movl	%eax, %ebx
	call	swapcontext@PLT
	endbr32
	addl	$16, %esp
	call	resume_epilogue
.L29:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE15:
	.size	pdco_resume_, .-pdco_resume_
	.globl	pdco_run
	.type	pdco_run, @function
pdco_run:
.LFB16:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$36, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	%eax, -28(%ebp)
	subl	$12, %esp
	pushl	$764
	movl	%eax, %ebx
	call	malloc@PLT
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	cmpl	$0, -12(%ebp)
	jne	.L32
	movl	$-1, %eax
	jmp	.L33
.L32:
	movl	-12(%ebp), %eax
	addl	$24, %eax
	subl	$12, %esp
	pushl	%eax
	movl	-28(%ebp), %ebx
	call	getcontext@PLT
	endbr32
	addl	$16, %esp
	testl	%eax, %eax
	jns	.L35
	subl	$12, %esp
	pushl	-12(%ebp)
	movl	-28(%ebp), %ebx
	call	free@PLT
	addl	$16, %esp
	movl	$-1, %eax
	jmp	.L33
.L35:
	movl	-12(%ebp), %eax
	movl	$0, 756(%eax)
	cmpl	$0, 12(%ebp)
	jne	.L36
	movl	$1024, 12(%ebp)
.L36:
	subl	$12, %esp
	pushl	12(%ebp)
	movl	-28(%ebp), %ebx
	call	malloc@PLT
	addl	$16, %esp
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	movl	%edx, 756(%eax)
	movl	-12(%ebp), %eax
	movl	756(%eax), %eax
	testl	%eax, %eax
	jne	.L37
	subl	$12, %esp
	pushl	-12(%ebp)
	call	free@PLT
	addl	$16, %esp
	movl	$-1, %eax
	jmp	.L33
.L37:
	subl	$12, %esp
	pushl	-12(%ebp)
	call	getstackstart
	addl	$16, %esp
	movl	-12(%ebp), %edx
	movl	%eax, 760(%edx)
	movl	-12(%ebp), %eax
	movl	12(%ebp), %edx
	movl	%edx, 752(%eax)
	movl	-12(%ebp), %eax
	movl	$1, 12(%eax)
	movl	-28(%ebp), %ecx
	movl	pdco_first@GOTOFF(%ecx), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 4(%eax)
	movl	-12(%ebp), %eax
	movl	%eax, pdco_first@GOTOFF(%ecx)
	movl	next_coroutine_id@GOTOFF(%ecx), %eax
	leal	1(%eax), %edx
	movl	%edx, next_coroutine_id@GOTOFF(%ecx)
	movl	-12(%ebp), %edx
	movl	%eax, (%edx)
	movl	-12(%ebp), %eax
	movl	8(%ebp), %edx
	movl	%edx, 20(%eax)
	movl	-12(%ebp), %eax
	movl	760(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 396(%eax)
	movl	-12(%ebp), %eax
	movl	752(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 404(%eax)
	movl	-12(%ebp), %eax
	movl	$0, 392(%eax)
	movl	-12(%ebp), %eax
	leal	388(%eax), %edx
	subl	$4, %esp
	pushl	$0
	leal	pdco_guard@GOTOFF(%ecx), %eax
	pushl	%eax
	pushl	%edx
	movl	%ecx, %ebx
	call	makecontext@PLT
	addl	$16, %esp
	movl	-12(%ebp), %eax
	movl	484(%eax), %eax
	testl	%eax, %eax
	jne	.L38
	movl	-12(%ebp), %eax
	leal	624(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 484(%eax)
.L38:
	subl	$12, %esp
	pushl	-12(%ebp)
	call	pdco_resume_
	addl	$16, %esp
.L33:
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE16:
	.size	pdco_run, .-pdco_run
	.globl	pdco_get_coroutine
	.type	pdco_get_coroutine, @function
pdco_get_coroutine:
.LFB17:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	pdco_active@GOTOFF(%eax), %edx
	testl	%edx, %edx
	jne	.L40
	movl	$0, %eax
	jmp	.L41
.L40:
	movl	pdco_active@GOTOFF(%eax), %eax
	movl	(%eax), %eax
.L41:
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE17:
	.size	pdco_get_coroutine, .-pdco_get_coroutine
	.globl	pdco_ud
	.type	pdco_ud, @function
pdco_ud:
.LFB18:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	pdco_active@GOTOFF(%eax), %eax
	movl	%eax, -4(%ebp)
	cmpl	$0, 8(%ebp)
	jle	.L43
	pushl	8(%ebp)
	call	getco
	addl	$4, %esp
	movl	%eax, -4(%ebp)
.L43:
	cmpl	$0, -4(%ebp)
	je	.L44
	movl	-4(%ebp), %eax
	addl	$16, %eax
	jmp	.L45
.L44:
	movl	$0, %eax
.L45:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE18:
	.size	pdco_ud, .-pdco_ud
	.globl	pdco_resume
	.type	pdco_resume, @function
pdco_resume:
.LFB19:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	subl	$4, %esp
	.cfi_offset 3, -12
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	pushl	8(%ebp)
	call	getco
	addl	$4, %esp
	movl	%eax, %ebx
	subl	$12, %esp
	pushl	%ebx
	call	pdco_resume_
	addl	$16, %esp
	movl	-4(%ebp), %ebx
	leave
	.cfi_restore 5
	.cfi_restore 3
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE19:
	.size	pdco_resume, .-pdco_resume
	.globl	pdco_kill
	.type	pdco_kill, @function
pdco_kill:
.LFB20:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	pdco_active@GOTOFF(%eax), %edx
	testl	%edx, %edx
	je	.L49
	movl	pdco_active@GOTOFF(%eax), %eax
	movl	(%eax), %eax
	cmpl	%eax, 8(%ebp)
	jne	.L49
	movl	$-1, %eax
	jmp	.L50
.L49:
	subl	$12, %esp
	pushl	8(%ebp)
	call	delco
	addl	$16, %esp
	testl	%eax, %eax
	jne	.L51
	movl	8(%ebp), %eax
	jmp	.L50
.L51:
	movl	$0, %eax
.L50:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE20:
	.size	pdco_kill, .-pdco_kill
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB21:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE21:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB22:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE22:
	.hidden	__stack_chk_fail_local
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 4
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 4
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 4
4:
)");
