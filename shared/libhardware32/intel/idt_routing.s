; defines space between interrupt handler labels.
; this also is the amount of machine code each isr 
; can store - just jump to glbl_int_handler.
; idt.c is dependent on this!
%define INT_PADDING 32 
global _isr_padding
_isr_padding db INT_PADDING

; macros for err/noerr isr's.
%macro no_err_int_handler 1
global _idt_isr_%1
align INT_PADDING
_idt_isr_%1:
    push    dword 0                     ; push 0 as error code
    push    dword %1                    ; push the interrupt number
    jmp     glbl_int_handler    ; jump to the common handler
%endmacro
%macro err_int_handler 1
global _idt_isr_%1
align INT_PADDING
_idt_isr_%1:
    push    dword %1                    ; push the interrupt number
    jmp     glbl_int_handler    ; jump to the common handler
%endmacro
 
no_err_int_handler 0
no_err_int_handler 1
no_err_int_handler 2
no_err_int_handler 3
no_err_int_handler 4
no_err_int_handler 5
no_err_int_handler 6
no_err_int_handler 7
err_int_handler 8
no_err_int_handler 9
err_int_handler 10
err_int_handler 11
err_int_handler 12
err_int_handler 13
err_int_handler 14
no_err_int_handler 15
no_err_int_handler 16
no_err_int_handler 17
no_err_int_handler 18
no_err_int_handler 19
no_err_int_handler 20
no_err_int_handler 21
no_err_int_handler 22
no_err_int_handler 23
no_err_int_handler 24
no_err_int_handler 25
no_err_int_handler 26
no_err_int_handler 27
no_err_int_handler 28
no_err_int_handler 29
no_err_int_handler 30
no_err_int_handler 31
no_err_int_handler 32
no_err_int_handler 33
no_err_int_handler 34
no_err_int_handler 35
no_err_int_handler 36
no_err_int_handler 37
no_err_int_handler 38
no_err_int_handler 39
no_err_int_handler 40
no_err_int_handler 41
no_err_int_handler 42
no_err_int_handler 43
no_err_int_handler 44
no_err_int_handler 45
no_err_int_handler 46
no_err_int_handler 47
no_err_int_handler 48
no_err_int_handler 49
no_err_int_handler 50
no_err_int_handler 51
no_err_int_handler 52
no_err_int_handler 53
no_err_int_handler 54
no_err_int_handler 55
no_err_int_handler 56
no_err_int_handler 57
no_err_int_handler 58
no_err_int_handler 59
no_err_int_handler 60
no_err_int_handler 61
no_err_int_handler 62
no_err_int_handler 63
no_err_int_handler 64
no_err_int_handler 65
no_err_int_handler 66
no_err_int_handler 67
no_err_int_handler 68
no_err_int_handler 69
no_err_int_handler 70
no_err_int_handler 71
no_err_int_handler 72
no_err_int_handler 73
no_err_int_handler 74
no_err_int_handler 75
no_err_int_handler 76
no_err_int_handler 77
no_err_int_handler 78
no_err_int_handler 79
no_err_int_handler 80
no_err_int_handler 81
no_err_int_handler 82
no_err_int_handler 83
no_err_int_handler 84
no_err_int_handler 85
no_err_int_handler 86
no_err_int_handler 87
no_err_int_handler 88
no_err_int_handler 89
no_err_int_handler 90
no_err_int_handler 91
no_err_int_handler 92
no_err_int_handler 93
no_err_int_handler 94
no_err_int_handler 95
no_err_int_handler 96
no_err_int_handler 97
no_err_int_handler 98
no_err_int_handler 99

no_err_int_handler 100
no_err_int_handler 101
no_err_int_handler 102
no_err_int_handler 103
no_err_int_handler 104
no_err_int_handler 105
no_err_int_handler 106
no_err_int_handler 107
no_err_int_handler 108
no_err_int_handler 109
no_err_int_handler 110
no_err_int_handler 111
no_err_int_handler 112
no_err_int_handler 113
no_err_int_handler 114
no_err_int_handler 115
no_err_int_handler 116
no_err_int_handler 117
no_err_int_handler 118
no_err_int_handler 119
no_err_int_handler 120
no_err_int_handler 121
no_err_int_handler 122
no_err_int_handler 123
no_err_int_handler 124
no_err_int_handler 125
no_err_int_handler 126
no_err_int_handler 127
no_err_int_handler 128
no_err_int_handler 129
no_err_int_handler 130
no_err_int_handler 131
no_err_int_handler 132
no_err_int_handler 133
no_err_int_handler 134
no_err_int_handler 135
no_err_int_handler 136
no_err_int_handler 137
no_err_int_handler 138
no_err_int_handler 139
no_err_int_handler 140
no_err_int_handler 141
no_err_int_handler 142
no_err_int_handler 143
no_err_int_handler 144
no_err_int_handler 145
no_err_int_handler 146
no_err_int_handler 147
no_err_int_handler 148
no_err_int_handler 149
no_err_int_handler 150
no_err_int_handler 151
no_err_int_handler 152
no_err_int_handler 153
no_err_int_handler 154
no_err_int_handler 155
no_err_int_handler 156
no_err_int_handler 157
no_err_int_handler 158
no_err_int_handler 159
no_err_int_handler 160
no_err_int_handler 161
no_err_int_handler 162
no_err_int_handler 163
no_err_int_handler 164
no_err_int_handler 165
no_err_int_handler 166
no_err_int_handler 167
no_err_int_handler 168
no_err_int_handler 169
no_err_int_handler 170
no_err_int_handler 171
no_err_int_handler 172
no_err_int_handler 173
no_err_int_handler 174
no_err_int_handler 175
no_err_int_handler 176
no_err_int_handler 177
no_err_int_handler 178
no_err_int_handler 179
no_err_int_handler 180
no_err_int_handler 181
no_err_int_handler 182
no_err_int_handler 183
no_err_int_handler 184
no_err_int_handler 185
no_err_int_handler 186
no_err_int_handler 187
no_err_int_handler 188
no_err_int_handler 189
no_err_int_handler 190
no_err_int_handler 191
no_err_int_handler 192
no_err_int_handler 193
no_err_int_handler 194
no_err_int_handler 195
no_err_int_handler 196
no_err_int_handler 197
no_err_int_handler 198
no_err_int_handler 199

no_err_int_handler 200
no_err_int_handler 201
no_err_int_handler 202
no_err_int_handler 203
no_err_int_handler 204
no_err_int_handler 205
no_err_int_handler 206
no_err_int_handler 207
no_err_int_handler 208
no_err_int_handler 209
no_err_int_handler 210
no_err_int_handler 211
no_err_int_handler 212
no_err_int_handler 213
no_err_int_handler 214
no_err_int_handler 215
no_err_int_handler 216
no_err_int_handler 217
no_err_int_handler 218
no_err_int_handler 219
no_err_int_handler 220
no_err_int_handler 221
no_err_int_handler 222
no_err_int_handler 223
no_err_int_handler 224
no_err_int_handler 225
no_err_int_handler 226
no_err_int_handler 227
no_err_int_handler 228
no_err_int_handler 229
no_err_int_handler 230
no_err_int_handler 231
no_err_int_handler 232
no_err_int_handler 233
no_err_int_handler 234
no_err_int_handler 235
no_err_int_handler 236
no_err_int_handler 237
no_err_int_handler 238
no_err_int_handler 239
no_err_int_handler 240
no_err_int_handler 241
no_err_int_handler 242
no_err_int_handler 243
no_err_int_handler 244
no_err_int_handler 245
no_err_int_handler 246
no_err_int_handler 247
no_err_int_handler 248
no_err_int_handler 249
no_err_int_handler 250
no_err_int_handler 251
no_err_int_handler 252
no_err_int_handler 253
no_err_int_handler 254
no_err_int_handler 255

global load_idt
load_idt:
    mov edx, [esp+4]
    lidt [edx]
    ;sti
    ret

global clear_interrupts
clear_interrupts:
    cli
    ret

global glbl_int_handler
extern kern_interrupt_handler
extern printintln
glbl_int_handler:
    cli
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax ; ????

    mov eax, esp   ; Push us the stack
    push eax
    mov eax, kern_interrupt_handler
    call eax       ; A special call, preserves the 'eip' register

    ; if it's not zero, this is a return addr:
    ;cmp eax, 0
    ;jne .custom_ret
    
    .reg_ret:
    add esp, 4
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

    ;.custom_ret:
    ;cli ; critical phase. Do not allow other interrupts to fire
    ;mov dword [_register_buf], eax ; buffers ptr to iret_args_t from eax
    ;add esp, 4
    ;pop gs
    ;pop fs
    ;pop es
    ;pop ds
    ;popa
    ;add esp, 8     ; Cleans up the pushed error code and pushed ISR number;

    ; next, set iret destination and attrs:
    ; pushed last to first:  eip, then cs, eflags, esp, ss

    ;add esp, 20    ; pop previous iret attr's!
    ;mov eax, dword [_register_buf]

    ; ss
    ;push dword [eax]
    ; useresp
    ;push dword [eax+4]
    ; makes sure interrupt bit is set in buffered eflags
    ; eflags
    ;mov ebx, [eax+8]
    ;or ebx, 0x200
    ;push dword ebx
    ;push dword [eax+8]
    ;popf
    ;sti ; set interrupts enabled flag
    ;pushf
    ;cli ; disable interrupts again
    ; cs
    ;push dword [eax+12]
    ; eip
    ;push dword [eax+16]

    ; restore register states!
    ;mov ebx, dword [eax+24]
    ;mov ecx, dword [eax+28]
    ;mov edx, dword [eax+32]
    ;mov esi, dword [eax+36]
    ;mov edi, dword [eax+40]
    ;mov eax, dword [eax+20]

    ;mov dword [_register_buf], eax
    ;call printintln
    ;mov eax, dword [_register_buf]
    ;jmp $

    ; perform jump
    ;iret           ; pops 5 things at once: EIP, CS, EFLAGS, ESP, and SS!


_register_buf dd 0x0