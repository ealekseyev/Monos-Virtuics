// this file contains raw tss manipulation functions.
// no kernel-level fancy shmancy process management.
// that's in scheduler.c
#ifndef __TSS_C_
#define __TSS_C_
#include <stddef.h>
#include <intel/gdt.c>

typedef struct __attribute__((packed)){
    // 4
    uint16_t link;
    uint16_t _reserved_1;
    // 8
    uint32_t esp0;
    uint16_t ss0;
    uint16_t _reserved_2;
    // 8
    uint32_t esp1;
    uint16_t ss1;
    uint16_t _reserved_3;
    // 8
    uint32_t esp2;
    uint16_t ss2;
    uint16_t _reserved_3_5;
    // 44
    uint32_t cr3, eip, eflags;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    // 8
    uint16_t es;
    uint16_t _reserved_4;
    uint16_t cs;
    uint16_t _reserved_5;
    // 8
    uint16_t ss;
    uint16_t _reserved_6;
    uint16_t ds;
    uint16_t _reserved_7;
    // 8
    uint16_t fs;
    uint16_t _reserved_8;
    uint16_t gs;
    uint16_t _reserved_9;
    // 8
    uint16_t ldtr;
    uint32_t _reserved_10; 
    uint16_t iopb;
} tss_t;

extern void load_tss(uint16_t gdt_index);

// tss gdt index
uint16_t __TSS_SEG;

// add gdt entry for tss and tell the cpu it is present
void init_tss(tss_t* tss) {
    __TSS_SEG = gdt_append(0x0, 0xffffffff, 0x0, 0x0);
    load_tss(__TSS_SEG);
}
#endif