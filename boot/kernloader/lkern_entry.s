[bits 32]

global _start
global transfer_control
global bootl_set_stack

extern load_kern
_start:
    call load_kern
    jmp $

; todo merge kern and gdt set_segments
extern _kern_start_mem
extern _stack_top
; very last operation!
; must set stack right before far jump - any subroutines will crash 
; otherwise due to absence of a location to return to (usually stored in the stack)
transfer_control:
    mov ebp, [_stack_top]        ; 6. setup stack
    mov esp, ebp
    mov eax, [_kern_start_mem]
    jmp eax
    ;jmp KERNEL_ENTRY
; pad to size of two sectors 
;times 2048 - ($-$$) db 0
