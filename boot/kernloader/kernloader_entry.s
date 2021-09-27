[bits 32]

global _start
global transfer_control

extern load_kern
extern lkern_err
_start:
    call load_kern
    ; if load_kern returns:
    push byte 0x01
    call lkern_err
    pop eax ; dummy register
    jmp $ ; hang

; todo merge kern and gdt set_segments
extern _kern_start_mem
extern _stack_top
;extern _installed_mem_loc
extern _kern_stack_start

; very last operation!
; must set stack right before far jump - any subroutines ('ret' opcodes) will not work.
;_____________ignore! stack is set to the end of memory - same as _installed_mem_loc 
; stack is set to end of first pd entry - 4mb exactly. grows downward towards the kernel.
; otherwise due to absence of a location to return to (usually stored in the stack)

transfer_control:
    mov eax, [_kern_start_mem]
    mov ebx, [_kern_stack_start] ;[_installed_mem_loc]
    mov ebp, ebx        ; setup stack | note ebx may have to be in brackets if kernel not booting
    mov esp, ebp
    jmp eax

