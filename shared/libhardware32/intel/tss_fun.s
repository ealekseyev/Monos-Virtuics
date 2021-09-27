global load_tss
extern printword
load_tss:
    mov word ax, [esp+4]
    ltr ax
    ret