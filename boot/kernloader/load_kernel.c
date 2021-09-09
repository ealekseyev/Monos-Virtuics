#include <vga.h>
//#include <idt.c>
#include <gdt.c>
// todo setup gdt and idt
#include <stddef.h>
#include <stdlib.h>
#include <disk.c>

char* str_boot = "Stage 2 Bootloader has successfully initialized\n";
char* str_init_gdt = "Setting up permanent GDT and updated segment registers\n";
char* str_load_code = "Loading kernel into memory\n";
char* str_init_idt = "Setting up permanent IDT\n";
char* str_init_paging = "Initializing paging and virtual memory\n";
char* str_set_stack = "Setting stack";
char* str_kern_jump = "Transfering control to main kernel\n";

extern void* _global_gdt;// = (gdt_t*) 0x00104000;
extern uint32_t _bls2_sectors;
extern uint32_t _kern_start_mem;

//extern void set_stack(uint32_t addr);
__attribute__((__noreturn__)) extern void transfer_control();


void copy_kernel(uint32_t dst_addr) {
	read_sectors_ATA_PIO(_kern_start_mem, _bls2_sectors+1, 60); // load 64 sectors of kernel from next sector after bootlaoder
}

void load_kern(void) {
	setcolor(VGA_RED, VGA_LGREEN);
	clear_screen();
	tty_writebuf(str_boot);

	copy_kernel(_kern_start_mem);
	tty_writebuf(str_load_code);

	init_gdt((gdt_t*) _global_gdt);
	tty_writebuf(str_init_gdt);

	//init_idt(global_idt);
	//tty_writebuf(str_init_idt);
	//tty_writebuf(str_init_paging);
	tty_writebuf(str_set_stack);
	tty_writebuf(str_kern_jump);
	transfer_control();
	while(true);
}

/*void arg_chk(uint32_t a, uint32_t b) {
	char strbuf[10];
	tty_writebuf(itoa(a, strbuf, 16));
}*/