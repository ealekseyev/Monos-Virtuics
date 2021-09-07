#include <vga.h>
#include <idt.c> // keep this here - prevents linkage error 
//#include "hybrid_drivers/paging.c"

void kernel_entry(void) {
	setcolor(VGA_BLACK, VGA_LGREEN);
	clearscreen();
	char* str = "Kernel successfully loaded!\n";
	tty_writebuf(str);
	//test_extern("test extern successful");
}