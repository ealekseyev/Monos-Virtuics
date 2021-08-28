	.file	"kernel.c"
	.text
	.globl	memset
	.type	memset, @function
memset:
.LFB0:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	jmp	.L2
.L3:
	movl	-4(%ebp), %eax
	leal	1(%eax), %edx
	movl	%edx, -4(%ebp)
	movl	12(%ebp), %edx
	movb	%dl, (%eax)
.L2:
	movl	16(%ebp), %eax
	leal	-1(%eax), %edx
	movl	%edx, 16(%ebp)
	testl	%eax, %eax
	jg	.L3
	movl	8(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	memset, .-memset
	.globl	strlen
	.type	strlen, @function
strlen:
.LFB1:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	8(%ebp), %ebx
	jmp	.L6
.L7:
	addl	$1, %ebx
.L6:
	movzbl	(%ebx), %eax
	testb	%al, %al
	jne	.L7
	movl	%ebx, %eax
	subl	8(%ebp), %eax
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	strlen, .-strlen
	.data
	.align 4
	.type	vidmem, @object
	.size	vidmem, 4
vidmem:
	.long	753664
	.section	.rodata
	.type	Xlen, @object
	.size	Xlen, 1
Xlen:
	.byte	80
	.type	Ylen, @object
	.size	Ylen, 1
Ylen:
	.byte	25
	.data
	.type	global_color, @object
	.size	global_color, 1
global_color:
	.byte	7
	.text
	.globl	writebuf
	.type	writebuf, @function
writebuf:
.LFB2:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	vidmem, %edx
	movl	$25, %eax
	movzbl	%al, %eax
	imull	16(%ebp), %eax
	movl	%eax, %ecx
	movl	12(%ebp), %eax
	addl	%ecx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	movl	%eax, -8(%ebp)
	pushl	8(%ebp)
	call	strlen
	addl	$4, %esp
	movl	%eax, -12(%ebp)
	movl	$0, -4(%ebp)
	jmp	.L10
.L11:
	movl	8(%ebp), %edx
	movl	-4(%ebp), %eax
	addl	%edx, %eax
	movl	-4(%ebp), %edx
	leal	(%edx,%edx), %ecx
	movl	-8(%ebp), %edx
	addl	%ecx, %edx
	movzbl	(%eax), %eax
	movb	%al, (%edx)
	movzbl	global_color, %ecx
	movl	-4(%ebp), %eax
	addl	%eax, %eax
	leal	1(%eax), %edx
	movl	-8(%ebp), %eax
	addl	%edx, %eax
	movl	%ecx, %edx
	movb	%dl, (%eax)
	addl	$1, -4(%ebp)
.L10:
	movl	8(%ebp), %edx
	movl	-4(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L11
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	writebuf, .-writebuf
	.globl	dumpstr
	.type	dumpstr, @function
dumpstr:
.LFB3:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	$0
	pushl	$0
	pushl	8(%ebp)
	call	writebuf
	addl	$12, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	dumpstr, .-dumpstr
	.globl	clearscreen
	.type	clearscreen, @function
clearscreen:
.LFB4:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	$0, -4(%ebp)
	jmp	.L14
.L15:
	movl	vidmem, %edx
	movl	-4(%ebp), %eax
	addl	%edx, %eax
	movb	$32, (%eax)
	movzbl	global_color, %ecx
	movl	vidmem, %eax
	movl	-4(%ebp), %edx
	addl	$1, %edx
	addl	%edx, %eax
	movl	%ecx, %edx
	movb	%dl, (%eax)
	addl	$2, -4(%ebp)
.L14:
	movl	$80, %eax
	movzbl	%al, %edx
	movl	$25, %eax
	movzbl	%al, %eax
	imull	%edx, %eax
	addl	%eax, %eax
	cmpl	%eax, -4(%ebp)
	jb	.L15
	nop
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	clearscreen, .-clearscreen
	.globl	setcolor
	.type	setcolor, @function
setcolor:
.LFB5:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movb	%dl, -4(%ebp)
	movb	%al, -8(%ebp)
	movzbl	-4(%ebp), %edx
	movzbl	-8(%ebp), %eax
	addl	$4, %eax
	movl	%eax, %ecx
	sall	%cl, %edx
	movl	%edx, %eax
	movb	%al, global_color
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	setcolor, .-setcolor
	.globl	getcolor
	.type	getcolor, @function
getcolor:
.LFB6:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movb	%dl, -4(%ebp)
	movb	%al, -8(%ebp)
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	getcolor, .-getcolor
	.section	.rodata
.LC0:
	.string	"this is a test"
	.text
	.globl	kernel_entry
	.type	kernel_entry, @function
kernel_entry:
.LFB7:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	$10
	pushl	$0
	call	setcolor
	addl	$8, %esp
	pushl	$.LC0
	call	dumpstr
	addl	$4, %esp
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	kernel_entry, .-kernel_entry
	.globl	kernel_halt
	.type	kernel_halt, @function
kernel_halt:
.LFB8:
	.cfi_startproc
	endbr32
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	nop
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	kernel_halt, .-kernel_halt
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
