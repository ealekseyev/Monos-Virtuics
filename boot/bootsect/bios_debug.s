[bits 16]
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