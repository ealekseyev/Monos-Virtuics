[bits 16]
[org 0x7c00]

_start:
    mov si, ERROR
    call bios_print
    jmp $

bios_print:
    pusha
    ;mov  bx, 0007h   ; BH is DisplayPage, BL is GraphicsColor
  .start:
    mov  al, [si]    ; <<<<<<<<<
    cmp  al, 0 
    je   .done
    mov  ah, 0x0E    ; BIOS.Teletype
    int  0x10
    inc  si          ; <<<<<<<<<
    jmp  .start
  .done:
    popa
    ret
ERROR: db 'error occurred', 0

; padding & magic number
times 510 - ($-$$) db 0
dw 0xaa55