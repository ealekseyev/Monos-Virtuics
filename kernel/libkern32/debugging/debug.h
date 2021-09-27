#ifndef _DEBUG_H_
#define _DEBUG_H_
#include <stddef.h>
#include <io/vga.h>
#include <mas.c>

extern uint32_t* _installed_mem_loc;
extern uint8_t* _installed_cores;
extern uint32_t _bls2_start_mem;
extern uint32_t _bls2_end_mem;
extern uint32_t _kern_start_mem;
extern uint32_t _kern_end_mem;
//extern isr_handler_t* _kern_isr_handlers;

extern void* _global_idt;
extern void* _global_gdt;
//extern void* _global_pd;


void dump_kern_const() {
	tty_write_hex_sandwich("Installed memory:      ", *_installed_mem_loc, " bytes\n");
	tty_write_hex_sandwich("Installed cores?:      ", (uint32_t) *_installed_cores, " cores\n");

	tty_write_hex_sandwich("IDT addr:              ", (uint32_t) _global_idt, "\n");
	tty_write_hex_sandwich("GDT addr:              ", (uint32_t) _global_gdt, "\n");
	tty_write_hex_sandwich("Global PD addr:        ", (uint32_t) _global_pd, "\n");

    tty_write_hex_sandwich("Bootloader start addr: ", _bls2_start_mem, "\n");
	tty_write_hex_sandwich("Bootloader end addr:   ", _bls2_end_mem, "\n");
	tty_write_hex_sandwich("Kernel start addr:     ", _kern_start_mem, "\n");
	tty_write_hex_sandwich("Kernel end addr:       ", _kern_end_mem, "\n");

    tty_write_hex_sandwich("EBP register value:    ", get_ebp(), "\n");
	tty_write_hex_sandwich("ESP register value:    ", get_esp(), "\n");
}

void dump_isr() {
	for(int i = 0; i < 50; i+=3) {
		tty_write_hex8_sandwich("ISR ", i,   ": ");
		tty_write_hex8_sandwich("", _kern_isr_handlers[i].addr,   "      ");
		tty_write_hex8_sandwich("ISR ", i+1, ": ");
		tty_write_hex8_sandwich("", _kern_isr_handlers[i+1].addr, "      ");
		tty_write_hex8_sandwich("ISR ", i+2, ": ");
		tty_write_hex8_sandwich("", _kern_isr_handlers[i+2].addr, "\n");
	}
}

#endif