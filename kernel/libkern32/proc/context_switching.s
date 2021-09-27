global switch_context
extern printintln
switch_context:
    mov eax, [esp+4]

    cli ; critical phase. Do not allow other interrupts to fire

    push dword [eax]
    ; useresp
    push dword [eax+4]
    ; makes sure interrupt bit is set in buffered eflags
    ; eflags
    mov ebx, [eax+8]
    or ebx, 0x200
    push dword ebx
    ;push dword [eax+8]
    ;popf
    ;sti ; set interrupts enabled flag
    ;pushf
    ;cli ; disable interrupts again
    ; cs
    push dword [eax+12]
    ; eip
    push dword [eax+16]

    ; restore register states!
    mov ebx, dword [eax+24]
    mov ecx, dword [eax+28]
    mov edx, dword [eax+32]
    mov esi, dword [eax+36]
    mov edi, dword [eax+40]
    mov eax, dword [eax+20]

    ; perform jump
    iret           ; pops 5 things at once: EIP, CS, EFLAGS, ESP, and SS!
