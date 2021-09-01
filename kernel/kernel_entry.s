[bits 32]
;[org 0x00100000]

global _start
extern kernel_entry
extern kernel_halt
_start:
    call kernel_entry
    call kernel_halt
    jmp $

; pad to size of two sectors
;times 1024 - ($-$$) db 0
