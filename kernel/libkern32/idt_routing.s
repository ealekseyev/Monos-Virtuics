global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global isr_common_stub
extern isr_handler
isr_common_stub:
    pushad
    call isr_handler
    popad
    iret
    
_null:
    mov ax,ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    iret

# load idt table

global load_idt
load_idt:
    mov edx, [esp+4]
    lidt [edx]
    ;sti
    ret

; div by 0
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub
; debug exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub
; non maskable int exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub
; int 3 exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub
; into exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub
; out of bounds exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub
; invalid opcode exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub
; coprocessor not available exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub
; double fault exception (w error)
isr8:
    cli
    ;push byte 0
    push byte 8
    jmp isr_common_stub
; coprocessor segment overrun exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub
; bad tss exception (w error)
isr10:
    cli
    ;push byte 0
    push byte 10
    jmp isr_common_stub
; segment not present exception (w error)
isr11:
    cli
    ;push byte 0
    push byte 11
    jmp isr_common_stub
; stack fault exception (w error)
isr12:
    cli
    ;push byte 0
    push byte 12
    jmp isr_common_stub
; general protection fault exception (w error)
isr13:
    cli
    ;push byte 0
    push byte 13
    jmp isr_common_stub
; page fault exception (w error)
isr14:
    cli
    ;push byte 0
    push byte 14
    jmp isr_common_stub
; reserved exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub
; floating point exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub
; alignment check exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub
; machine check exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub
; reserved onward
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

; to be set by drivers and such
irq0: iret
irq1: iret
irq2: iret
irq3: iret
irq4: iret
irq5: iret
irq6: iret
irq7: iret
irq8: iret
irq9: iret
irq10: iret
irq11: iret
irq12: iret
irq13: iret
irq14: iret
irq15: iret



