#ifndef _PIC_C_
#define _PIC_C_
#include <stddef.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIC_EOI		0x20		/* End-of-interrupt command code */

static inline void pic_io_wait(void) {
    port_writeb(0x80, 0);
}

static void remap_pic(int offset1, int offset2) {
	uint8_t a1, a2;
	a1 = port_readb(PIC1_DATA);                        // save masks
	a2 = port_readb(PIC2_DATA);
 
	port_writeb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	pic_io_wait();
	port_writeb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	pic_io_wait();
	port_writeb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	pic_io_wait();
	port_writeb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	pic_io_wait();
	port_writeb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	pic_io_wait();
	port_writeb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	pic_io_wait();
 
	port_writeb(PIC1_DATA, ICW4_8086);
	pic_io_wait();
	port_writeb(PIC2_DATA, ICW4_8086);
	pic_io_wait();
 
	port_writeb(PIC1_DATA, a1);   // restore saved masks.
	port_writeb(PIC2_DATA, a2);
}

static inline bool are_interrupts_enabled() {
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

// enables irq
#define irq_disable irq_set_mask
void irq_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = port_readb(port) | (1 << IRQline);
    port_writeb(port, value);        
}

// disables irq
#define irq_enable irq_clear_mask
void irq_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = port_readb(port) & ~(1 << IRQline);
    port_writeb(port, value);        
}

// disable all irqs
void irq_nullify_mask() {
    port_writeb(PIC1_DATA, 0xff); // disable irq's 0 - 8
    pic_io_wait();
    port_writeb(PIC2_DATA, 0xff); // disable irq's 9 - 16
    pic_io_wait();
}

static inline void master_pic_irq_done() {
    port_writeb(PIC1, PIC_EOI);
}

static inline void slave_pic_irq_done() {
    port_writeb(PIC2, PIC_EOI);
}

// true to enable pic2, false to disable
void irq_set_slave_pic(bool enable) {
    if(enable) {
        irq_enable(2);
    } else {
        irq_disable(2);
    }
}

void init_pic() {
    remap_pic(0x20, 0x28);
    irq_nullify_mask();    // disable all irq's
    irq_set_slave_pic(true); // reenable slave pic, albeit with irq's masked
    __asm__("sti");
}
#endif