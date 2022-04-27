asm(R"(
	.file	"coroutine.c"
	.text
	.type	stack_direction_helper, @function
stack_direction_helper:
.LFB6:
	.cfi_startproc
	endbr32
	subl	$44, %esp
	.cfi_def_cfa_offset 48
	movl	48(%esp), %eax
	movl	%eax, 12(%esp)
	movl	%gs:20, %eax
	movl	%eax, 28(%esp)
	xorl	%eax, %eax
	leal	24(%esp), %edx
	leal	12(%esp), %eax
	cmpl	%eax, %edx
	sbbl	%eax, %eax
	orl	$1, %eax
	movl	28(%esp), %ecx
	xorl	%gs:20, %ecx
	jne	.L6
	addl	$44, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 4
	ret
.L6:
	.cfi_restore_state
	call	__stack_chk_fail_local
	.cfi_endproc
.LFE6:
	.size	stack_direction_helper, .-stack_direction_helper
	.type	stack_direction, @function
stack_direction:
.LFB7:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$36, %esp
	.cfi_def_cfa_offset 44
	movl	$20, %ebx
	movl	%gs:(%ebx), %eax
	movl	%eax, 24(%esp)
	xorl	%eax, %eax
	leal	20(%esp), %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	stack_direction_helper
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	12(%esp), %edx
	xorl	%gs:(%ebx), %edx
	jne	.L10
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L10:
	.cfi_restore_state
	call	__stack_chk_fail_local
	.cfi_endproc
.LFE7:
	.size	stack_direction, .-stack_direction
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
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	16(%esp), %edx
	movl	pdco_first@GOTOFF(%ebx), %eax
.L12:
	testl	%eax, %eax
	je	.L11
	cmpl	%edx, (%eax)
	jne	.L12
	movl	4(%eax), %edx
	movl	%edx, pdco_first@GOTOFF(%ebx)
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%eax
	.cfi_def_cfa_offset 32
	call	free@PLT
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	movl	$1, %eax
.L11:
	addl	$8, %esp
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE8:
	.size	delco, .-delco
	.type	cleanup_if_complete, @function
cleanup_if_complete:
.LFB9:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	movl	12(%eax), %ebx
	testl	%ebx, %ebx
	je	.L21
	movl	$1, %ebx
.L17:
	movl	%ebx, %eax
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L21:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	(%eax)
	.cfi_def_cfa_offset 32
	call	delco
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	jmp	.L17
	.cfi_endproc
.LFE9:
	.size	cleanup_if_complete, .-cleanup_if_complete
	.type	getstackstart, @function
getstackstart:
.LFB10:
	.cfi_startproc
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	movl	%eax, %ebx
	call	stack_direction
	cmpl	$1, %eax
	je	.L26
	movl	332(%ebx), %eax
	movl	336(%ebx), %edx
	leal	-48(%edx,%eax), %eax
.L22:
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L26:
	.cfi_restore_state
	movl	336(%ebx), %eax
	addl	$48, %eax
	jmp	.L22
	.cfi_endproc
.LFE10:
	.size	getstackstart, .-getstackstart
	.type	getco, @function
getco:
.LFB11:
	.cfi_startproc
	endbr32
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	4(%esp), %edx
	movl	pdco_first@GOTOFF(%eax), %eax
	testl	%eax, %eax
	je	.L32
.L28:
	cmpl	%edx, (%eax)
	je	.L27
	movl	4(%eax), %eax
	testl	%eax, %eax
	jne	.L28
.L27:
	ret
.L32:
	ret
	.cfi_endproc
.LFE11:
	.size	getco, .-getco
	.globl	pdco_run
	.type	pdco_run, @function
pdco_run:
.LFB12:
	.cfi_startproc
	endbr32
	pushl	%edi
	.cfi_def_cfa_offset 8
	.cfi_offset 7, -8
	pushl	%esi
	.cfi_def_cfa_offset 12
	.cfi_offset 6, -12
	pushl	%ebx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subl	$28, %esp
	.cfi_def_cfa_offset 44
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	%ebx, 20(%esp)
	movl	48(%esp), %esi
	pushl	$348
	.cfi_def_cfa_offset 48
	call	malloc@PLT
	movl	%eax, 20(%esp)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L41
	movl	%eax, %edi
	testl	%esi, %esi
	movl	$16384, %eax
	cmove	%eax, %esi
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%esi
	.cfi_def_cfa_offset 48
	call	malloc@PLT
	movl	%eax, 336(%edi)
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L43
	movl	4(%esp), %ebx
	movl	%ebx, %eax
	call	getstackstart
	movl	%ebx, %ecx
	movl	%eax, 340(%ebx)
	movl	$1, 12(%ebx)
	movl	8(%esp), %ebx
	movl	pdco_active@GOTOFF(%ebx), %eax
	movl	%eax, 8(%ecx)
	movl	%esi, 332(%ecx)
	movl	pdco_first@GOTOFF(%ebx), %eax
	movl	%eax, 4(%ecx)
	movl	next_coroutine_id@GOTOFF(%ebx), %eax
	leal	1(%eax), %edx
	movl	%edx, next_coroutine_id@GOTOFF(%ebx)
	movl	%eax, (%ecx)
	movl	32(%esp), %eax
	movl	%eax, 16(%ecx)
	movl	40(%esp), %eax
	movl	%eax, 344(%ecx)
	leal	176(%ecx), %eax
	movl	%eax, 12(%esp)
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	_setjmp@PLT
	endbr32
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	jne	.L44
	movl	4(%esp), %edx
	movl	8(%edx), %eax
	movl	8(%esp), %ebx
	movl	%eax, pdco_active@GOTOFF(%ebx)
	movl	%edx, pdco_first@GOTOFF(%ebx)
	leal	20(%edx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	_setjmp@PLT
	endbr32
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L45
	movl	4(%esp), %esi
	movl	8(%esi), %eax
	movl	8(%esp), %edi
	movl	%eax, pdco_active@GOTOFF(%edi)
	cmpl	$0, 44(%esp)
	je	.L40
	movl	344(%esi), %eax
	movl	44(%esp), %esi
	movl	%eax, (%esi)
.L40:
	movl	4(%esp), %edi
	movl	%edi, %eax
	call	cleanup_if_complete
	testl	%eax, %eax
	je	.L33
	movl	(%edi), %eax
.L33:
	addl	$16, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 12
	popl	%esi
	.cfi_restore 6
	.cfi_def_cfa_offset 8
	popl	%edi
	.cfi_restore 7
	.cfi_def_cfa_offset 4
	ret
.L43:
	.cfi_restore_state
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	16(%esp)
	.cfi_def_cfa_offset 48
	movl	24(%esp), %ebx
	call	free@PLT
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	movl	$-1, %eax
	jmp	.L33
.L44:
	movl	4(%esp), %eax
	movl	340(%eax), %eax
	movl %eax, %esp 
 movl %eax, %ebp
	movl	8(%esp), %ebx
	movl	pdco_active@GOTOFF(%ebx), %eax
	movl	pdco_active@GOTOFF(%ebx), %edx
	movl	pdco_active@GOTOFF(%ebx), %esi
	subl	$12, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 44
	pushl	344(%edx)
	.cfi_def_cfa_offset 48
	call	*16(%eax)
	addl	$8, %esp
	.cfi_def_cfa_offset 40
	movl	%eax, 344(%esi)
	movl	pdco_active@GOTOFF(%ebx), %eax
	movl	$0, 12(%eax)
	movl	pdco_active@GOTOFF(%ebx), %eax
	pushl	$1
	.cfi_def_cfa_offset 44
	addl	$20, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	longjmp@PLT
.L45:
	.cfi_restore_state
	movl	8(%esp), %eax
	movl	4(%esp), %esi
	movl	%esi, pdco_active@GOTOFF(%eax)
	subl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pushl	$1
	.cfi_def_cfa_offset 44
	pushl	24(%esp)
	.cfi_def_cfa_offset 48
	movl	%eax, %ebx
	call	longjmp@PLT
.L41:
	.cfi_restore_state
	movl	$-1, %eax
	jmp	.L33
	.cfi_endproc
.LFE12:
	.size	pdco_run, .-pdco_run
	.globl	pdco_yield
	.type	pdco_yield, @function
pdco_yield:
.LFB13:
	.cfi_startproc
	endbr32
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$36, %esp
	.cfi_def_cfa_offset 44
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	%ebx, 24(%esp)
	movl	pdco_active@GOTOFF(%ebx), %eax
	movl	44(%esp), %edx
	movl	%edx, 344(%eax)
	movl	pdco_active@GOTOFF(%ebx), %eax
	addl	$176, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	_setjmp@PLT
	endbr32
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L50
	movl	12(%esp), %eax
	movl	pdco_active@GOTOFF(%eax), %eax
	movl	344(%eax), %eax
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L50:
	.cfi_restore_state
	movl	12(%esp), %ebx
	movl	pdco_active@GOTOFF(%ebx), %eax
	subl	$8, %esp
	.cfi_def_cfa_offset 40
	pushl	$1
	.cfi_def_cfa_offset 44
	addl	$20, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	longjmp@PLT
	.cfi_endproc
.LFE13:
	.size	pdco_yield, .-pdco_yield
	.globl	pdco_resume
	.type	pdco_resume, @function
pdco_resume:
.LFB14:
	.cfi_startproc
	endbr32
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$24, %esp
	.cfi_def_cfa_offset 32
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	%ebx, 12(%esp)
	pushl	32(%esp)
	.cfi_def_cfa_offset 36
	call	getco
	addl	$4, %esp
	.cfi_def_cfa_offset 32
	movl	%eax, 8(%esp)
	testl	%eax, %eax
	je	.L56
	movl	%eax, %edx
	movl	pdco_active@GOTOFF(%ebx), %eax
	cmpl	%edx, %eax
	je	.L57
	movl	pdco_active@GOTOFF(%ebx), %eax
	movl	%eax, 8(%edx)
	movl	36(%esp), %eax
	movl	%eax, 344(%edx)
	leal	20(%edx), %eax
	subl	$12, %esp
	.cfi_def_cfa_offset 44
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	_setjmp@PLT
	endbr32
	addl	$16, %esp
	.cfi_def_cfa_offset 32
	testl	%eax, %eax
	je	.L59
	movl	8(%esp), %ecx
	movl	8(%ecx), %eax
	movl	12(%esp), %ebx
	movl	%eax, pdco_active@GOTOFF(%ebx)
	cmpl	$0, 40(%esp)
	je	.L55
	movl	344(%ecx), %eax
	movl	40(%esp), %ecx
	movl	%eax, (%ecx)
.L55:
	movl	8(%esp), %ebx
	movl	%ebx, %eax
	call	cleanup_if_complete
	testl	%eax, %eax
	je	.L51
	movl	(%ebx), %eax
.L51:
	addl	$24, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L59:
	.cfi_restore_state
	movl	12(%esp), %ebx
	movl	8(%esp), %eax
	movl	%eax, pdco_active@GOTOFF(%ebx)
	subl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	pushl	$1
	.cfi_def_cfa_offset 44
	addl	$176, %eax
	pushl	%eax
	.cfi_def_cfa_offset 48
	call	longjmp@PLT
.L56:
	.cfi_restore_state
	movl	$-1, %eax
	jmp	.L51
.L57:
	movl	$-1, %eax
	jmp	.L51
	.cfi_endproc
.LFE14:
	.size	pdco_resume, .-pdco_resume
	.globl	pdco_kill
	.type	pdco_kill, @function
pdco_kill:
.LFB15:
	.cfi_startproc
	endbr32
	pushl	%ebx
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	subl	$8, %esp
	.cfi_def_cfa_offset 16
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	16(%esp), %ebx
	movl	pdco_active@GOTOFF(%eax), %edx
	testl	%edx, %edx
	je	.L61
	movl	pdco_active@GOTOFF(%eax), %eax
	cmpl	%ebx, (%eax)
	je	.L63
.L61:
	subl	$12, %esp
	.cfi_def_cfa_offset 28
	pushl	%ebx
	.cfi_def_cfa_offset 32
	call	delco
	addl	$16, %esp
	.cfi_def_cfa_offset 16
	testl	%eax, %eax
	movl	$0, %eax
	cmove	%ebx, %eax
.L60:
	addl	$8, %esp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	popl	%ebx
	.cfi_restore 3
	.cfi_def_cfa_offset 4
	ret
.L63:
	.cfi_restore_state
	movl	$-1, %eax
	jmp	.L60
	.cfi_endproc
.LFE15:
	.size	pdco_kill, .-pdco_kill
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB16:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE16:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB17:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE17:
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
