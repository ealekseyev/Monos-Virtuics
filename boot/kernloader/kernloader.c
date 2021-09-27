#include <io/vga.h>
#include <io/disk.c>
#include <io/pic.c>

#include <hardware.h>

#include <intel/gdt.c>
#include <intel/idt.c>

#include <stddef.h>
#include <stdlib.h>

extern void* _global_gdt;
extern void* _global_idt;

extern uint32_t _bls2_sectors;
extern uint32_t _kern_start_mem;
extern uint32_t _bls2_start_mem;
extern uint32_t _bls2_end_mem;
extern uint32_t* _installed_mem_loc;
extern uint32_t _kern32_sectors;
extern void* _global_idt;
//extern void* _kern_isr_handlers;
extern void* _kern_stack_start;

__attribute__((__noreturn__)) extern void transfer_control();

void copy_kernel(uint32_t dst_addr) {
	read_sectors_ATA_PIO(dst_addr-1, _bls2_sectors+1, _kern32_sectors); // load 128 sectors of kernel from next sector after bootlaoder
}

void load_kern(void) {
	tty_set_global_color(VGA_BLACK, VGA_LGREEN);
	tty_clear_screen();
	tty_write_hex_sandwich("Stage 2 Bootloader successfully loaded at ", _bls2_start_mem, "\n");

	copy_kernel(_kern_start_mem);
	tty_writebuf("Loading kernel into memory\n");

	init_gdt((gdt_t*) _global_gdt);
	tty_writebuf("Setting up permanent GDT and updated segment registers\n");

	init_idt((idt_t*) _global_idt, (isr_handler_t*) _kern_isr_handlers);
	tty_writebuf("Setting up permanent IDT and interrupt routing\n");

	init_pic();
	//irq_enable(0);
	//irq_enable(1);
	tty_writebuf("Configuring PIC controller and IRQ servicers\n");

	tty_writebuf("Probing memory...");
	*_installed_mem_loc = get_eom_addr(_kern_start_mem); // beginning of unused memory
	tty_write_hex_sandwich(" ", *_installed_mem_loc, " bytes installed\n");

	tty_write_hex_sandwich("Setting stack pointer to ", (int)_kern_stack_start, "\n");
	tty_writebuf("Transferring control to main kernel\n");

	//tty_write_hex8_sandwich("Val @ kernel start: ", *((uint8_t*)0x500002), "\n");

	transfer_control();
}

#define UNEXPECTED_RETURN 1

void lkern_err(uint8_t err_code) {
	switch(err_code) {
		case UNEXPECTED_RETURN:
		tty_set_global_color(VGA_RED, VGA_BWHITE);
		tty_clear_screen();
		tty_writebuf("Kernelloader unexpectedly terminated");
		break;
	}
}