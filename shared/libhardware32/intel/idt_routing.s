; defines space between interrupt handler labels.
; this also is the amount of machine code each isr 
; can store - just jump to glbl_int_handler.
; idt.c is dependent on this!
%define INT_PADDING 16 
global _isr_padding
_isr_padding db INT_PADDING

; macros for err/noerr isr's.
%macro no_err_int_handler 1
global _idt_isr_%1
align INT_PADDING
_idt_isr_%1:
    push    byte 0                     ; push 0 as error code
    push    byte %1                    ; push the interrupt number
    jmp     glbl_int_handler    ; jump to the common handler
%endmacro
%macro err_int_handler 1
global _idt_isr_%1
align INT_PADDING
_idt_isr_%1:
    push    byte %1                    ; push the interrupt number
    jmp     glbl_int_handler    ; jump to the common handler
%endmacro
 
no_err_int_handler 0
no_err_int_handler 1
no_err_int_handler 2
no_err_int_handler 3
no_err_int_handler 4
no_err_int_handler 5
no_err_int_handler 6
no_err_int_handler 7
err_int_handler 8
no_err_int_handler 9
err_int_handler 10
err_int_handler 11
err_int_handler 12
err_int_handler 13
err_int_handler 14
no_err_int_handler 15
no_err_int_handler 16
no_err_int_handler 17
no_err_int_handler 18
no_err_int_handler 19
no_err_int_handler 20
no_err_int_handler 21
no_err_int_handler 22
no_err_int_handler 23
no_err_int_handler 24
no_err_int_handler 25
no_err_int_handler 26
no_err_int_handler 27
no_err_int_handler 28
no_err_int_handler 29
no_err_int_handler 30
no_err_int_handler 31
no_err_int_handler 32
no_err_int_handler 33
no_err_int_handler 34
no_err_int_handler 35
no_err_int_handler 36
no_err_int_handler 37
no_err_int_handler 38
no_err_int_handler 39
no_err_int_handler 40
no_err_int_handler 41
no_err_int_handler 42
no_err_int_handler 43
no_err_int_handler 44
no_err_int_handler 45
no_err_int_handler 46
no_err_int_handler 47
no_err_int_handler 48
no_err_int_handler 49
no_err_int_handler 50
no_err_int_handler 51
no_err_int_handler 52
no_err_int_handler 53
no_err_int_handler 54
no_err_int_handler 55
no_err_int_handler 56
no_err_int_handler 57
no_err_int_handler 58
no_err_int_handler 59
no_err_int_handler 60
no_err_int_handler 61
no_err_int_handler 62
no_err_int_handler 63
no_err_int_handler 64
no_err_int_handler 65
no_err_int_handler 66
no_err_int_handler 67
no_err_int_handler 68
no_err_int_handler 69
no_err_int_handler 70
no_err_int_handler 71
no_err_int_handler 72
no_err_int_handler 73
no_err_int_handler 74
no_err_int_handler 75
no_err_int_handler 76
no_err_int_handler 77
no_err_int_handler 78
no_err_int_handler 79

global load_idt
load_idt:
    mov edx, [esp+4]
    lidt [edx]
    ;sti
    ret

global clear_interrupts
clear_interrupts:
    cli
    ret

global glbl_int_handler
extern kern_interrupt_handler
glbl_int_handler:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, kern_interrupt_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!