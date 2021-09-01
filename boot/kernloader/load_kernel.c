#include "../../kernel/lib_debug/vga.h"
#include <idt.c>
#include <gdt.c>
#include <stddef.h>

#define KERNEL_START 0x00120000
#define STACK_BOTTOM 0x00ff0000 // TODO: setup memory detection and set stack to bottom

char* str_boot = "Stage 2 Bootloader has successfully initialized\n";
char* str_init_gdt = "Setting up permanent GDT\n";
char* str_load_code = "Loading kernel into memory\n";
char* str_init_idt = "Setting up permanent IDT\n";
char* str_init_paging = "Initializing paging and virtual memory\n";
char* str_set_stack = "Setting stack";
char* str_kern_jump = "Transfering control to main kernel\n";

idt_t* global_idt = 0x00100000;
uint32_t* global_gdt = 0x00104000;
uint32_t* global_pd = 0x0010c000;
uint32_t* global_pt = 0x00114000;

void copy_kernel() {

}

extern void transfer_control(uint32_t addr) __attribute__(( __noreturn__));
extern void set_stack(uint32_t addr);
void load_kern(void) {
	clearscreen();
	setcolor(VGA_BLACK, VGA_LGREEN);
	tty_writebuf(str_boot);
	tty_writebuf(str_init_gdt);
	tty_writebuf(str_load_code);
	tty_writebuf(str_init_idt);
	tty_writebuf(str_init_paging);
	tty_writebuf(str_kern_jump);
	transfer_control(KERNEL_START);
}
