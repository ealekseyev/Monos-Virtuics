[bits 32]
global _reboot
_reboot:
    cli
    mov eax, cr0
    and eax, 0x80000000
    mov cr0, eax

    xor eax, eax
    mov cr3, eax

    mov eax, cr0
    and eax, 0xfffffffe
    mov cr0, eax
    
    jmp 0x7c00
