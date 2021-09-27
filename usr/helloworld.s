global _start
_start:
mov eax, 0x1
mov ebx, printstr
int 0x30
jmp $

printstr db "kernel prog launch successful!", 0, 0xa
