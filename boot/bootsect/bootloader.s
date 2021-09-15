[bits 16]
[org 0x7c00]

; also update /kern_const.c and /makefile for BLS2_SIZE
KERNEL_OFFSET equ 0x8000 ; The same one we used when linking the kernel
BLS2_SIZE equ 32
BLS2_STACK_START equ 0x90000


mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
; setup stack
mov bp, BLS2_STACK_START
mov sp, bp

; set vga mode
;mov ah, 0
;mov al, 03h
;int 10h

call load_kernel ; read the kernel from disk
call switch_to_32bit ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
jmp $ ; Never executed

;%include "disk.asm"
;%include "gdt.asm"
;%include "switch-to-32bit.asm"
%include "boot/bootsect/bios_debug.s"
%include "boot/bootsect/disk.s"
%include "boot/bootsect/gdt.s"
%include "boot/bootsect/activate32.s"

[bits 16]
load_kernel:
    mov si, BOOTING
    call bios_print
    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
    mov dh, BLS2_SIZE
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_32BIT:
    mov eax, 0xb8000
    mov [eax], byte 'X'
    call KERNEL_OFFSET ; Give control to the kernel
    mov eax, 0xb8000
    mov [eax], byte 'Y'
    jmp $ ; Stay here when the kernel returns control to us (if ever)

BOOT_DRIVE db 0
BOOTING db 'Loading kernel into memory ...', 0xa, 0xd, 0
BIT_SWITCH db 'Entering Protected Mode and handing control to kernel...', 0xa, 0xd, 0
EXIT_ERR db 'E: unexpected kernel exit', 0xa, 0xd, 0

; padding
times 510 - ($-$$) db 0
dw 0xaa55
