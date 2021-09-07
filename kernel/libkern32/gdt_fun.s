[bits 32]

global load_gdt
global set_segments

load_gdt:
    mov edx, [esp+4]
    lgdt [edx]
    ret

set_segments:
    push ax
    mov ax, word [esp+4] ;contains gdt data seg ptr to use
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop ax
    ret


    