[bits 32]

global _start
extern kernel_entry
_start:
    call kernel_entry
    jmp $
