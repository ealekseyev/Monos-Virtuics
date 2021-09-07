[bits 32]

KERNEL_STACK_ADDR equ 0x00ff0000
KERNEL_ENTRY equ 0x00120000

global _start
global transfer_control
global bootl_set_segments

extern load_kern

_start:
    call load_kern
    jmp $

; todo merge kern and gdt set_segments
bootl_set_segments:
    mov ax, 16        ; __KERNEL_DS
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ;mov ax, 8       ; __KERNEL_CS
    ;mov cs, ax

    mov ebp, KERNEL_STACK_ADDR        ; 6. setup stack
    mov esp, ebp
    ret

transfer_control:
    jmp KERNEL_ENTRY
; pad to size of two sectors 
;times 2048 - ($-$$) db 0
