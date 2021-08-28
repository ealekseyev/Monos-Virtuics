[bits 16]
[org 0x7c00]
; where to load the kernel to
KERNEL_OFFSET equ 0x1000 ; 0x8c00 - 0x9fc00 available memory for kernel

; BIOS sets boot drive in 'dl'; store for later use
mov [BOOT_DRIVE], dl

; set vga mode
mov ah, 0
mov al, 03h
int 10h
; setup stack
mov bp, 0x9000
mov sp, bp

call load_kernel
call switch_to_32bit
jmp $

;%include "boot/bios_debug.s"
%include "boot/disk.s"
%include "boot/gdt.s"
%include "boot/activate32.s"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET ; bx -> destination
    mov dh, 2             ; dh -> num sectors
    mov dl, [BOOT_DRIVE]  ; dl -> disk
    call disk_load
    ret

[bits 32]
BEGIN_32BIT:
    call KERNEL_OFFSET ; give control to the kernel
    jmp $ ; loop in case kernel returns

[bits 16]
; boot drive variable
BOOT_DRIVE db 0

; padding & magic number
times 510 - ($-$$) db 0
dw 0xaa55
