#include <intel/gdt.c>
#include <intel/idt.c> // keep this here - prevents linkage error 
#include <intel/paging.c>

#include <io/pic.c>
#include <io/vga.h>

#include <timer.c>
#include <mas.c>

#include <debugging/debug.h>
#include <monos/fatal_error.c>

// todo fix this
#define nullptr 0x00

extern uint32_t _kern_end_mem; // must be 4kb aligned

// do NOT deallocate/mangle
page_table_t* kern_mem_0_4m = (page_table_t*) 0x100000;
page_table_t* kern_mem_4_8m = (page_table_t*) 0x101000;

static inline void enviro_init(void) {
	tty_set_global_color(VGA_GRAY, VGA_LGREEN);
	tty_clear_screen();
	tty_writebuf("Kernel successfully loaded!\n");

	// init timer
	init_pit();
	tty_writebuf("Initialized System Timer.\n");

	// init tss
	//gdt_append();

	// init paging
	init_paging();
	paging_clear_table(kern_mem_0_4m); // kernel table! 4 megabytes.
	paging_fill_table(kern_mem_0_4m, 0x0, 0x00400000, PAGE_PRESENT | PAGE_RW);
	paging_clear_table(kern_mem_4_8m);
	paging_fill_table(kern_mem_4_8m, 0x00400000, 0x00800000, PAGE_PRESENT | PAGE_RW);

	paging_append_pd_entry(kern_mem_0_4m, PAGE_PRESENT | PAGE_RW);
	paging_append_pd_entry(kern_mem_4_8m, PAGE_PRESENT | PAGE_RW);
	enable_paging();
	tty_writebuf("Enabled Paging.\n");

	// heap allocator, etc.
	//init_mas();
	//tty_writebuf("Initialized Memory Allocation Subsystem.\n");
	
	tty_writebuf("Done booting.\n\n");
}
/* KERNEL ENTRY FUNCTION */
void kernel_entry(void) {
	enviro_init();
	dump_kern_const();
}