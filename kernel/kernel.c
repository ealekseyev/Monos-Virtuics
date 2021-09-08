#include <vga.h>
#include <idt.c> // keep this here - prevents linkage error 
#include <pic.c>
#include <pit_timer.c>
extern uint32_t _global_idt;

void test_proc_func(void);
void test_proc_func2(void);

static inline void enviro_init(void) {
	setcolor(VGA_GRAY, VGA_LGREEN);
	clearscreen();
	tty_writebuf("Kernel successfully loaded!\n");

	init_idt((idt_t*) _global_idt);
	tty_writebuf("Initialized IDT.\n");
	init_pic();
	init_pit();
	wait_for_timer_init();
	tty_writebuf("Initialized PIC and PIT Timer.\n");
	tty_writebuf("Done booting.\n");
}
/* KERNEL ENTRY FUNCTION */
void kernel_entry(void) {
	enviro_init();
	timer_proc_t test_proc;
	test_proc.freq = 2000;
	test_proc.handler = &test_proc_func;
	register_timed_process(&test_proc);
	test_proc.freq = 4000;
	test_proc.handler = &test_proc_func2;
	register_timed_process(&test_proc);
}

void test_proc_func(void) {
	tty_writebuf("hi\n");
}
void test_proc_func2(void) {
	tty_writebuf("bye\n");
}