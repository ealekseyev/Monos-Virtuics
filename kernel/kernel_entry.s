[bits 32]
global _start
extern kernel_entry
extern kernel_halt
_start:
    call kernel_entry
    call kernel_halt
    jmp $