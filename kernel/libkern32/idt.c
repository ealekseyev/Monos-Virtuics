#ifndef _IDT_H_
#define _IDT_H_
#include <stddef.h>
#include <memset.c>

#define IDT_SIZE 256 // 256 idt gates

// A struct describing an interrupt gate.
typedef struct __attribute__((__packed__)) {
    uint16_t base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
    uint16_t selector;                 // Kernel segment selector.
    uint8_t  always0;             // This must always be zero.
    uint8_t  flags;               // More flags. See documentation.
    uint16_t base_hi;             // The upper 16 bits of the address to jump to.
} idt_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
typedef struct __attribute__((__packed__)) {
    uint16_t limit;
    uint32_t base;                // The address of the first element in our idt_entry_t array.
} idt_ptr_t;

typedef struct __attribute__((__packed__)) {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // eax pushed first, edi last.
} reg_t;
typedef struct __attribute__((__packed__)) {
    uint16_t ip, cs, flags, sp, ss;
} int_frame_t;

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
// IRQ Handlers
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void load_idt(idt_ptr_t* idt_ptr);

static void idt_set_gate(idt_t* idt_location, uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_location[num].base_lo  = base & 0xFFFF;
    idt_location[num].base_hi  = (base >> 16) & 0xFFFF;

    idt_location[num].selector = sel;
    idt_location[num].always0  = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_location[num].flags    = flags /* | 0x60 */;
}

static void idt_format(idt_t* idt_entry, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entry->base_lo  = base & 0xFFFF;
    idt_entry->base_hi  = (base >> 16) & 0xFFFF;

    idt_entry->selector = sel;
    idt_entry->always0  = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entry->flags    = flags /* | 0x60 */;
}

idt_t* idt_location;
void init_idt(idt_t* kern_idt) {
    idt_location = kern_idt;
    idt_ptr_t idt_ptr;
    idt_ptr.limit = sizeof(idt_t) * 256 - 1;
    idt_ptr.base  = (uint32_t) idt_location;

    memset(idt_location, 0, sizeof(idt_t)*IDT_SIZE);

    // Generic intel error code interrupts
    idt_set_gate(idt_location,  0, (uint32_t)isr0,  0x08, 0x8E);
    idt_set_gate(idt_location,  1, (uint32_t)isr1,  0x08, 0x8E);
    idt_set_gate(idt_location,  2, (uint32_t)isr2,  0x08, 0x8E);
    idt_set_gate(idt_location,  3, (uint32_t)isr3,  0x08, 0x8E);
    idt_set_gate(idt_location,  4, (uint32_t)isr4,  0x08, 0x8E);
    idt_set_gate(idt_location,  5, (uint32_t)isr5,  0x08, 0x8E);
    idt_set_gate(idt_location,  6, (uint32_t)isr6,  0x08, 0x8E);
    idt_set_gate(idt_location,  7, (uint32_t)isr7,  0x08, 0x8E);
    idt_set_gate(idt_location,  8, (uint32_t)isr8,  0x08, 0x8E);
    idt_set_gate(idt_location,  9, (uint32_t)isr9,  0x08, 0x8E);
    idt_set_gate(idt_location, 10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(idt_location, 11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(idt_location, 12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(idt_location, 13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(idt_location, 14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(idt_location, 15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(idt_location, 16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(idt_location, 17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(idt_location, 18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(idt_location, 19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(idt_location, 20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(idt_location, 21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(idt_location, 22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(idt_location, 23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(idt_location, 24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(idt_location, 25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(idt_location, 26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(idt_location, 27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(idt_location, 28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(idt_location, 29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(idt_location, 30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(idt_location, 31, (uint32_t)isr31, 0x08, 0x8E);

    // IRQ entries
    /*idt_set_gate(idt_location, 32, (uint32_t)irq0,  0x08, 0x8E);
    idt_set_gate(idt_location, 33, (uint32_t)irq1,  0x08, 0x8E);
    idt_set_gate(idt_location, 34, (uint32_t)irq2,  0x08, 0x8E);
    idt_set_gate(idt_location, 35, (uint32_t)irq3,  0x08, 0x8E);
    idt_set_gate(idt_location, 36, (uint32_t)irq4,  0x08, 0x8E);
    idt_set_gate(idt_location, 37, (uint32_t)irq5,  0x08, 0x8E);
    idt_set_gate(idt_location, 38, (uint32_t)irq6,  0x08, 0x8E);
    idt_set_gate(idt_location, 39, (uint32_t)irq7,  0x08, 0x8E);
    idt_set_gate(idt_location, 40, (uint32_t)irq8,  0x08, 0x8E);
    idt_set_gate(idt_location, 41, (uint32_t)irq9,  0x08, 0x8E);
    idt_set_gate(idt_location, 42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(idt_location, 43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(idt_location, 44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(idt_location, 45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(idt_location, 46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(idt_location, 47, (uint32_t)irq15, 0x08, 0x8E);*/

    load_idt(&idt_ptr);
}

#define INT_START asm volatile("pusha")
#define INT_END asm volatile("popa"); asm volatile("iret")
// handlers must implement IRQ_START and IRQ_END. (or int attr)
// TODO: set error codes or whatever
int add_int_handler(uint8_t gate_num, uint32_t* handler) {
    idt_set_gate(idt_location, gate_num, (uint32_t)handler, 0x08, 0x8E);
    return 0;
}
bool is_int_populated(uint8_t gate_num) {
    if((idt_location[gate_num]).base_lo == 0 && (idt_location[gate_num]).base_hi == 0)
        return false;
    return true;
}

uint32_t* int_routines[255];

void isr_handler() {
return;
}
#endif