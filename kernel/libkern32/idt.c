#ifndef _IDT_H_
#define _IDT_H_
#include <stddef.h>
#include <memset.c>
// comment to disable prints
//#define DEBUG_ON
#define IDT_SIZE 256 // 256 idt gates

#define ISR_DIV_BY_ZERO 0
#define ISR_DEBUG_EXCEPTION 1
#define ISR_NON_MASKABLE_INT_EXCEPTION 2
#define ISR_INT_3_EXCEPTION 3
#define ISR_INTO_EXCEPTION 4
#define ISR_OUT_OF_BOUNDS 5
#define ISR_INVALID_OPCODE 6
#define ISR_COPROCESSOR_ABSENT 7
#define ISR_DOUBLE_FAULT 8
#define ISR_COPROCESSOR_SEG_OVERRUN 9
#define ISR_BAD_TSS 10
#define ISR_SEG_ABSENT 11
#define ISR_STACK_FAULT 12
#define ISR_GENERAL_PROT_FAULT 13
#define ISR_PAGE_FAULT 14
#define ISR_RESERVED_15 15
#define ISR_FLOAT_EXCEPTION 16
#define ISR_ALIGNMENT_CHK_EXCEPTION 17
#define ISR_MACHINE_CHK_EXCEPTION 18
#define ISR_RESERVED_19 19
#define ISR_RESERVED_20 20
#define ISR_RESERVED_21 21
#define ISR_RESERVED_22 22
#define ISR_RESERVED_23 23
#define ISR_RESERVED_24 24
#define ISR_RESERVED_25 25

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

typedef struct {
    uint32_t addr;
} handler_t;

typedef struct {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} irs_t; // interrupt register state type

static handler_t isr_l2_handlers[255];

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void _idt_isr_0 ();
extern void _idt_isr_1 ();
extern void _idt_isr_2 ();
extern void _idt_isr_3 ();
extern void _idt_isr_4 ();
extern void _idt_isr_5 ();
extern void _idt_isr_6 ();
extern void _idt_isr_7 ();
extern void _idt_isr_8 ();
extern void _idt_isr_9 ();
extern void _idt_isr_10();
extern void _idt_isr_11();
extern void _idt_isr_12();
extern void _idt_isr_13();
extern void _idt_isr_14();
extern void _idt_isr_15();
extern void _idt_isr_16();
extern void _idt_isr_17();
extern void _idt_isr_18();
extern void _idt_isr_19();
extern void _idt_isr_20();
extern void _idt_isr_21();
extern void _idt_isr_22();
extern void _idt_isr_23();
extern void _idt_isr_24();
extern void _idt_isr_25();
extern void _idt_isr_26();
extern void _idt_isr_27();
extern void _idt_isr_28();
extern void _idt_isr_29();
extern void _idt_isr_30();
extern void _idt_isr_31();
// IRQ Handlers
extern void _idt_isr_32();
extern void _idt_isr_33();
extern void _idt_isr_34();
extern void _idt_isr_35();
extern void _idt_isr_36();
extern void _idt_isr_37();
extern void _idt_isr_38();
extern void _idt_isr_39();
extern void _idt_isr_40();
extern void _idt_isr_41();
extern void _idt_isr_42();
extern void _idt_isr_43();
extern void _idt_isr_44();
extern void _idt_isr_45();
extern void _idt_isr_46();
extern void _idt_isr_47();
// cont
extern void _idt_isr_48();
extern void _idt_isr_49();

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

static idt_t* idt_location;
void init_idt(idt_t* kern_idt) {
    idt_location = kern_idt;
    idt_ptr_t idt_ptr;
    idt_ptr.limit = sizeof(idt_t) * 256 - 1;
    idt_ptr.base  = (uint32_t) idt_location;

    memset(idt_location, 0, sizeof(idt_t)*IDT_SIZE);

    // Generic intel error code interrupts
    idt_set_gate(idt_location,  0, (uint32_t)_idt_isr_0,  0x08, 0x8E);
    idt_set_gate(idt_location,  1, (uint32_t)_idt_isr_1,  0x08, 0x8E);
    idt_set_gate(idt_location,  2, (uint32_t)_idt_isr_2,  0x08, 0x8E);
    idt_set_gate(idt_location,  3, (uint32_t)_idt_isr_3,  0x08, 0x8E);
    idt_set_gate(idt_location,  4, (uint32_t)_idt_isr_4,  0x08, 0x8E);
    idt_set_gate(idt_location,  5, (uint32_t)_idt_isr_5,  0x08, 0x8E);
    idt_set_gate(idt_location,  6, (uint32_t)_idt_isr_6,  0x08, 0x8E);
    idt_set_gate(idt_location,  7, (uint32_t)_idt_isr_7,  0x08, 0x8E);
    idt_set_gate(idt_location,  8, (uint32_t)_idt_isr_8,  0x08, 0x8E);
    idt_set_gate(idt_location,  9, (uint32_t)_idt_isr_9,  0x08, 0x8E);
    idt_set_gate(idt_location, 10, (uint32_t)_idt_isr_10, 0x08, 0x8E);
    idt_set_gate(idt_location, 11, (uint32_t)_idt_isr_11, 0x08, 0x8E);
    idt_set_gate(idt_location, 12, (uint32_t)_idt_isr_12, 0x08, 0x8E);
    idt_set_gate(idt_location, 13, (uint32_t)_idt_isr_13, 0x08, 0x8E);
    idt_set_gate(idt_location, 14, (uint32_t)_idt_isr_14, 0x08, 0x8E);
    idt_set_gate(idt_location, 15, (uint32_t)_idt_isr_15, 0x08, 0x8E);
    idt_set_gate(idt_location, 16, (uint32_t)_idt_isr_16, 0x08, 0x8E);
    idt_set_gate(idt_location, 17, (uint32_t)_idt_isr_17, 0x08, 0x8E);
    idt_set_gate(idt_location, 18, (uint32_t)_idt_isr_18, 0x08, 0x8E);
    idt_set_gate(idt_location, 19, (uint32_t)_idt_isr_19, 0x08, 0x8E);
    idt_set_gate(idt_location, 20, (uint32_t)_idt_isr_20, 0x08, 0x8E);
    idt_set_gate(idt_location, 21, (uint32_t)_idt_isr_21, 0x08, 0x8E);
    idt_set_gate(idt_location, 22, (uint32_t)_idt_isr_22, 0x08, 0x8E);
    idt_set_gate(idt_location, 23, (uint32_t)_idt_isr_23, 0x08, 0x8E);
    idt_set_gate(idt_location, 24, (uint32_t)_idt_isr_24, 0x08, 0x8E);
    idt_set_gate(idt_location, 25, (uint32_t)_idt_isr_25, 0x08, 0x8E);
    idt_set_gate(idt_location, 26, (uint32_t)_idt_isr_26, 0x08, 0x8E);
    idt_set_gate(idt_location, 27, (uint32_t)_idt_isr_27, 0x08, 0x8E);
    idt_set_gate(idt_location, 28, (uint32_t)_idt_isr_28, 0x08, 0x8E);
    idt_set_gate(idt_location, 29, (uint32_t)_idt_isr_29, 0x08, 0x8E);
    idt_set_gate(idt_location, 30, (uint32_t)_idt_isr_30, 0x08, 0x8E);
    idt_set_gate(idt_location, 31, (uint32_t)_idt_isr_31, 0x08, 0x8E);

    // IRQ entries
    idt_set_gate(idt_location, 32, (uint32_t)_idt_isr_32, 0x08, 0x8E);
    idt_set_gate(idt_location, 33, (uint32_t)_idt_isr_33, 0x08, 0x8E);
    idt_set_gate(idt_location, 34, (uint32_t)_idt_isr_34, 0x08, 0x8E);
    idt_set_gate(idt_location, 35, (uint32_t)_idt_isr_35, 0x08, 0x8E);
    idt_set_gate(idt_location, 36, (uint32_t)_idt_isr_36, 0x08, 0x8E);
    idt_set_gate(idt_location, 37, (uint32_t)_idt_isr_37, 0x08, 0x8E);
    idt_set_gate(idt_location, 38, (uint32_t)_idt_isr_38, 0x08, 0x8E);
    idt_set_gate(idt_location, 39, (uint32_t)_idt_isr_39, 0x08, 0x8E);
    idt_set_gate(idt_location, 40, (uint32_t)_idt_isr_40, 0x08, 0x8E);
    idt_set_gate(idt_location, 41, (uint32_t)_idt_isr_41, 0x08, 0x8E);
    idt_set_gate(idt_location, 42, (uint32_t)_idt_isr_42, 0x08, 0x8E);
    idt_set_gate(idt_location, 43, (uint32_t)_idt_isr_43, 0x08, 0x8E);
    idt_set_gate(idt_location, 44, (uint32_t)_idt_isr_44, 0x08, 0x8E);
    idt_set_gate(idt_location, 45, (uint32_t)_idt_isr_45, 0x08, 0x8E);
    idt_set_gate(idt_location, 46, (uint32_t)_idt_isr_46, 0x08, 0x8E);
    idt_set_gate(idt_location, 47, (uint32_t)_idt_isr_47, 0x08, 0x8E);

    idt_set_gate(idt_location, 48, (uint32_t)_idt_isr_48, 0x08, 0x8E);
    idt_set_gate(idt_location, 49, (uint32_t)_idt_isr_49, 0x08, 0x8E);

    memset(isr_l2_handlers, 0, sizeof(handler_t)*255);

    load_idt(&idt_ptr);
}

// TODO: set error codes or whatever
// this modifies the idt entry. should not be accessed. 
// everything should point to glbl_int_handler in idt_routing.s
int add_idt_int_handler(uint8_t gate_num, uint32_t* handler) {
    idt_set_gate(idt_location, gate_num, (uint32_t)handler, 0x08, 0x8E);
    return 0;
}
// tells kern_interrupt_handler to call function. "Soft" interrupt.
int register_kern_int_handler(uint8_t gate_num, void (*handler)(irs_t*)) {
    isr_l2_handlers[gate_num].addr = (uint32_t) handler;
    return 0;
}
// zero isr handler
int del_kern_int_handler(uint8_t gate_num) {
    isr_l2_handlers[gate_num].addr = 0;
    return 0;
}
bool is_idt_int_populated(uint8_t gate_num) {
    if((idt_location[gate_num]).base_lo == 0 && (idt_location[gate_num]).base_hi == 0)
        return false;
    return true;
}
bool is_kern_int_populated(uint8_t gate_num) {
    if(isr_l2_handlers[gate_num].addr == 0)
        return false;
    return true;
}

// all interrupts go here
void kern_interrupt_handler(irs_t* registers) {
#ifdef DEBUG_ON
    tty_writebuf("int triggered: 0x");
    printint(registers->int_no);
    tty_writebuf("\n");
#endif
    // call function registered in irs_t to handle this interrupt
    // return if no registered handler
    if(isr_l2_handlers[registers->int_no].addr == 0) {
        tty_writebuf("No registered handler for interrupt 0x");
        printint(registers->int_no);
        tty_writebuf("\n");
        return;
    }
#ifdef DEBUG_ON
    tty_writebuf("launching interrupt handler at 0x");
    printint((uint32_t) isr_l2_handlers[registers->int_no].addr);
    tty_writebuf("...\n");
#endif

    void (*_kern_isr_gate_handler)(irs_t* registers);
    _kern_isr_gate_handler = (void*) (isr_l2_handlers[registers->int_no].addr);
    (*_kern_isr_gate_handler)(registers);
}
#endif