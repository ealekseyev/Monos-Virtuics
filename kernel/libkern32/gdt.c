#ifndef _GDT_H_
#define _GDT_H_
#include <stddef.h>

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
typedef struct __attribute__((packed)) {
    uint16_t limit_low;           // The lower 16 bits of the limit.
    uint16_t base_low;            // The lower 16 bits of the base.
    uint8_t  base_middle;         // The next 8 bits of the base.
    uint8_t  access;              // Access flags, determine what ring this segment can be used in.
    uint8_t  granularity;         // low 4 bits are high 4 bits of limit
    uint8_t  base_high;           // The last 8 bits of the base.
} gdt_t;

typedef struct __attribute__((packed)) {
    uint16_t limit;               // The upper 16 bits of all selector limits.
    uint32_t base;                // The address of the first gdt_entry_t struct.
} gdt_ptr_t;


// Lets us access our ASM functions from our C code.
//extern void load_gdt(uint32_t);
void load_gdt(gdt_ptr_t* gdt_ptr) {
    __asm__("lgdt (%%edx)": : "d" (gdt_ptr));
}

static void gdt_set_gate(gdt_t* gdt_location, int32_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_location[entry].base_low = (base & 0xFFFF);
    gdt_location[entry].base_middle = (base >> 16) & 0xFF;
    gdt_location[entry].base_high = (base >> 24) & 0xFF;

    gdt_location[entry].limit_low = (limit & 0xFFFF);
    gdt_location[entry].granularity = (limit >> 16) & 0x0F;

    gdt_location[entry].granularity |= gran & 0xF0;
    gdt_location[entry].access = access;
}

#define __NULL_SEG  0
#define __KERNEL_CS 1
#define __KERNEL_DS 2
#define __USER_CS   3
#define __USER_DS   4

void init_gdt(gdt_t* gdt_location) {
    gdt_ptr_t gdt_ptr;
    gdt_ptr.limit = (sizeof(gdt_t)*5) - 1;
    gdt_ptr.base = (uint32_t) gdt_location;

    /*
      Pr  Priv  1   Ex  DC   RW   Ac
      0x9A == 1001 1010  == 1   00    1   1   0    1    0
      0x92 == 1001 0010  == 1   00    1   0   0    1    0
      0xFA == 1111 1010  == 1   11    1   1   0    1    0
      0xF2 == 1111 0010  == 1   11    1   0   0    1    0
      We have page-granularity and 32-bit mode
      G   D   0   Av
      0xCF == 1100 1111  == 1   1   0   0  ~
    */

    gdt_set_gate(gdt_location, 0,0,0,0,0);                    //Null segment
    gdt_set_gate(gdt_location, 1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //Kernel mode code segment
    gdt_set_gate(gdt_location, 2, 0, 0xFFFFFFFF, 0x92, 0xCF); //Kernel mode data segment
    gdt_set_gate(gdt_location, 3, 0, 0xFFFFFFFF, 0xFA, 0xCF); //User mode code segment
    gdt_set_gate(gdt_location, 4, 0, 0xFFFFFFFF, 0xF2, 0xCF); //User mode data segment

    load_gdt(&gdt_ptr);
}
#endif