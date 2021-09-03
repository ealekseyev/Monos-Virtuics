#include <vga.h>

void kernel_entry(void) {
	setcolor(VGA_BLACK, VGA_LGREEN);
	clearscreen();
	char* str = "Kernel successfully loaded!\n";
	tty_writebuf(str);	
}