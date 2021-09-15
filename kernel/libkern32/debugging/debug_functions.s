global get_esp
global get_ebp
get_ebp:
    mov eax, ebp
    ret
get_esp:
    mov eax, esp
    ret