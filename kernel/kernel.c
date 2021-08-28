#include "lib_debug/vga.h"

void kernel_entry(void) {
	setcolor(VGA_BLACK, VGA_LGREEN);
	clearscreen();
	char* str = "this is a test\n";
	dumpstr(str);	
}

void kernel_halt() {
    
}