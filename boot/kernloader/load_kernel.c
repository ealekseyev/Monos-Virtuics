#include <vga.h>
//#include <idt.c>
#include <gdt.c>
// todo setup gdt and idt
#include <stddef.h>
#include <stdlib.h>
#include <disk.c>

#define KERNEL_START 0x00120000
#define STACK_BOTTOM 0x00ff0000 // TODO: setup memory detection and set stack to bottom

char* str_boot = "Stage 2 Bootloader has successfully initialized\n";
char* str_init_gdt = "Setting up permanent GDT and updated segment registers\n";
char* str_load_code = "Loading kernel into memory\n";
char* str_init_idt = "Setting up permanent IDT\n";
char* str_init_paging = "Initializing paging and virtual memory\n";
char* str_set_stack = "Setting stack";
char* str_kern_jump = "Transfering control to main kernel\n";

//idt_t* global_idt = 0x00100000;
extern void* _global_gdt;// = (gdt_t*) 0x00104000;
//uint32_t* global_pd = (uint32_t*) 0x0010c000;
//uint32_t* global_pt = (uint32_t*) 0x00114000;

__attribute__((__noreturn__)) extern void transfer_control();

extern void set_stack(uint32_t addr);
extern void bootl_set_segments();

extern uint32_t _bls2_sectors;

void copy_kernel(uint32_t dst_addr) {
	read_sectors_ATA_PIO(KERNEL_START, _bls2_sectors+1, 60); // load 64 sectors of kernel from next sector after bootlaoder
}

void load_kern(void) {
	setcolor(VGA_RED, VGA_LGREEN);
	clearscreen();
	tty_writebuf(str_boot);

	copy_kernel(KERNEL_START);
	tty_writebuf(str_load_code);

	//init_gdt(global_gdt);
	//bootl_set_segments();
	tty_writebuf(str_init_gdt);

	//init_idt(global_idt);
	//tty_writebuf(str_init_idt);
	//tty_writebuf(str_init_paging);
	tty_writebuf(str_kern_jump);
	//tty_writebuf(itoa(*((uint32_t*)0x00120000), global_buf, 16));
	//tty_writebuf(itoa(*((uint8_t*)0x00120002), global_buf, 16));
	//tty_writebuf(itoa(*((uint8_t*)0x00120003), global_buf, 16));
	//transfer_control();
	while(true);
}

/*void arg_chk(uint32_t a, uint32_t b) {
	char strbuf[10];
	tty_writebuf(itoa(a, strbuf, 16));
}*/