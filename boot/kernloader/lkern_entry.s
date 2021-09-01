[bits 32]
;[org 0x00001000]

global _start
extern load_kern
extern dumphex32
_start:
    call load_kern
    jmp $

set_stack:
    mov ebp, 0xffff     ; setup the stack
    mov esp, ebp

transfer_control:
    push ebp
    mov ebp, esp
    call dumphex32
    mov eax, [ebp+8]
    jmp eax
    pop ebp
    ret


; pad to size of two sectors
;times 2048 - ($-$$) db 0
