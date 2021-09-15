#ifndef _REGISTERS_H_
#define _REGISTERS_H_
// struct with 32 bit registers
typedef struct __attribute__((packed)){
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} irs_t; // interrupt register state type
#endif