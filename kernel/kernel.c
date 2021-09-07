#include <vga.h>
#include <idt.c> // keep this here - prevents linkage error 
//#include "hybrid_drivers/paging.c"
extern uint32_t _global_idt;

void isrhandler_37(void);
void kernel_entry(void) {
	setcolor(VGA_GRAY, VGA_LGREEN);
	clearscreen();
	tty_writebuf("Kernel successfully loaded!\n");
	init_idt((idt_t*) _global_idt);
	tty_writebuf("Initialized IDT.\n");

	//register_kern_int_handler(37, isrhandler_37);
	asm volatile("int $37");
}

//TODO: pass irs_t to isr handlers
void isrhandler_37(void) {
	tty_writebuf("Successfully launched isrhandler_37!\n");
}