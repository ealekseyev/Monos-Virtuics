#ifndef __IDT_C_
#define __IDT_C_
#include <io/vga.h>
#include <intel/registers.h>
#include <monos/fatal_error.c>
//#define DEBUG_ON

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
// for passing global idt to lidt
typedef struct __attribute__((__packed__)) {
    uint16_t limit;
    uint32_t base;                // The address of the first element in our idt_entry_t array.
} idt_ptr_t;
// addresses of kernel 3rd layer interrupt handlers
typedef struct __attribute__((packed)){
    uint32_t addr;
} isr_handler_t;

extern void _idt_isr_0();
extern void load_idt(idt_ptr_t*);
extern void clear_interrupts(void);
extern isr_handler_t* _kern_isr_handlers;
extern uint8_t _isr_padding;

#define IDT_PRESENT_ENTRIES 80
#define IDT_SIZE 0xff

static void idt_format(idt_t* idt_entry, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entry->base_lo  = base & 0xFFFF;
    idt_entry->base_hi  = (base >> 16) & 0xFFFF;

    idt_entry->selector = sel;
    idt_entry->always0  = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entry->flags    = flags /* | 0x60 */;
}

uint32_t idt_get_base(idt_t* idt_entry) {
    return (idt_entry->base_hi << 16) + idt_entry->base_lo;
}

void init_idt(idt_t* _idt, isr_handler_t* _kern_isrs) {
    idt_ptr_t idt_ptr;
    idt_ptr.limit = sizeof(idt_t) * IDT_PRESENT_ENTRIES - 1;
    idt_ptr.base  = (uint32_t) _idt;
    // zero the idt and also kern isr handlers
    memset((idt_t*) _idt, 0, sizeof(idt_t)*IDT_SIZE);
    memset(_kern_isrs, 0, sizeof(isr_handler_t)*IDT_SIZE); //TODO: overwrites?
    // set present entries in idt
    for(int i = 0; i < IDT_PRESENT_ENTRIES; i++) {
        // all interrupt handlers are 32 byte aligned - so we can effectively 
        // pass the handlers as pointers instead of declaring each as extern.
        // note due to this you also must modify idt_routing.s whenever something changes.
        idt_format(&_idt[i], (uint32_t)(_idt_isr_0+(i*_isr_padding)), 0x08, 0x8E);
    }
    load_idt(&idt_ptr);
}
/*void kern_interrupt_handler(irs_t* registers) {
    tty_write_hex_sandwich("Interrupt ", registers->int_no, " triggered\n");

}*/

void kern_interrupt_handler(irs_t* registers) {
    //tty_write_hex_sandwich("int triggered: ", registers->int_no, "\n");
    // call function registered in irs_t to handle this interrupt
    // return if no registered handler
    if(((isr_handler_t*)_kern_isr_handlers)[registers->int_no].addr == 0) {
        kill_kern(0x0, RS_UNDEF_INT);

        /*tty_write_hex_sandwich("Faulty instruction: ", registers->eip, "\n");
        for(int i = 0; i < sizeof(irs_t); i++) {
            printint(*((uint8_t*)registers+i));
            tty_writebuf(" ");
        }*/
        while(true);
        return;
    }
    //tty_write_hex_sandwich("launching interrupt handler at ", (uint32_t) ((isr_handler_t*)_kern_isr_handlers)[registers->int_no].addr, "...\n");
    void (*isr_handler)(irs_t* registers);
    isr_handler = (void*) (((isr_handler_t*)_kern_isr_handlers)[registers->int_no].addr);
    (*isr_handler)(registers);
    //tty_write_hex_sandwich("Jumping to ", (uint32_t) isr_handler, "\n");
}

void isr_register_handler(uint8_t gate, void* handler) {
    _kern_isr_handlers[gate].addr = (uint32_t)handler;
}

/*
// TODO: set error codes or whatever
// this modifies the idt entry. should not be accessed. 
// everything should point to glbl_int_handler in idt_routing.s
int add_idt_int_handler(uint8_t gate_num, uint32_t* handler) {
    idt_set_gate(idt_location, gate_num, (uint32_t)handler, 0x08, 0x8E);
    return 0;
}
// tells kern_interrupt_handler to call function. "Soft" interrupt.
int register_kern_int_handler(uint8_t gate_num, void (*handler)(irs_t*)) {
    ((isr_handler_t*)_global_isr_l2_handlers)[gate_num].addr = (uint32_t) handler;
    return 0;
}
// zero isr handler
int del_kern_int_handler(uint8_t gate_num) {
    ((isr_handler_t*)_global_isr_l2_handlers)[gate_num].addr = 0;
    return 0;
}
bool is_idt_int_populated(uint8_t gate_num) {
    if((idt_location[gate_num]).base_lo == 0 && (idt_location[gate_num]).base_hi == 0)
        return false;
    return true;
}
bool is_kern_int_populated(uint8_t gate_num) {
    if(((isr_handler_t*)_global_isr_l2_handlers)[gate_num].addr == 0)
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
    if(((isr_handler_t*)_global_isr_l2_handlers)[registers->int_no].addr == 0) {
        tty_writebuf("No registered handler for interrupt 0x");
        printint(registers->int_no);
        tty_writebuf("\n");
        return;
    }
#ifdef DEBUG_ON
    tty_writebuf("launching interrupt handler at 0x");
    printint((uint32_t) ((isr_handler_t*)_global_isr_l2_handlers)[registers->int_no].addr);
    tty_writebuf("...\n");
#endif

    void (*_kern_isr_gate_handler)(irs_t* registers);
    _kern_isr_gate_handler = (void*) (((isr_handler_t*)_global_isr_l2_handlers)[registers->int_no].addr);
    (*_kern_isr_gate_handler)(registers);
}*/
#endif