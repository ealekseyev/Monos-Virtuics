[bits 16]
BLS2_TMP_STACK equ 0x90000

switch_to_32bit:
    cli                     ; 1. disable interrupts
    lgdt [gdt_descriptor]   ; 2. load GDT descriptor

    mov si, BIT_SWITCH
    call bios_print
    
    mov eax, cr0            ; 3. enable protected mode
    or eax, 0x1             
    mov cr0, eax
    jmp CODE_SEG:init_32bit ; 4. far jump

[bits 32]
init_32bit:
    mov ax, DATA_SEG        ; 5. update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ;mov ax, CODE_SEG
    ;mov cs, ax

    mov ebp, BLS2_TMP_STACK   ; 6. setup temporary stack for bl
    mov esp, ebp

    jmp BEGIN_32BIT
    ;call BEGIN_32BIT        ; 7. move back to mbr.asm