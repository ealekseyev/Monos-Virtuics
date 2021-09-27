#include <intel/gdt.c>
#include <intel/idt.c> // keep this here - prevents linkage error 
#include <intel/paging.c>
#include <intel/tss.c>

#include <io/pic.c>
#include <io/vga.h>

#include <timer.c>
#include <mas.c>

#include <debugging/debug.h>
#include <monos/fatal_error.c>
#include <syscall.c>
#include <proc/proc.c>
#include <intel/registers.h>
#include <proc/scheduler.c>
#include "debugging/programs_bin.c"

// todo fix this
#define nullptr 0x00

extern uint32_t _kern_end_mem; // must be 4kb aligned

void* _keyboard_isr(irs_t*);

void _fatal_error() {
	__asm__("cli");
	tty_writebuf("fatal error occurred - nop sled activated");
	__asm__("lb1: jmp lb1");

}

static inline void enviro_init(void) {
	// essentially a line of defense in case the kernel 
	// goes haywire and starts executing non-executable 
	// instructions in direction of the stack
	// no op sled
	for(int i = 0; i < 0x10; i++) {
		*((uint8_t*) 0x00700000+i) = 0x90;
	}
	// jump
	*((uint8_t*) 0x00700010) = 0xcd;
	*((uint8_t*) 0x00700011) = 0x31;

	tty_set_global_color(VGA_GRAY, VGA_LGREEN);
	tty_clear_screen();
	tty_write("Kernel successfully loaded!\n");

	init_idt((idt_t*) _global_idt, (isr_handler_t*) _kern_isr_handlers);

	irq_enable(1);
	isr_register_handler(0x21, _keyboard_isr);
	isr_register_handler(0x31, _fatal_error);

	// heap allocator, page manager, etc.
	// always get/set pages through here, not paging.c!
	init_mas();
	mas_register_linear_mem(0x0, 0x00800000, PAGE_PRESENT | PAGE_RW | PAGE_USER_ACCESSIBLE);	
	//mas_kern_ready();

	tty_write("Initialized Memory Allocation Subsystem.\n");
	tty_write("Enabled Paging.\n");

	// register syscall interrupt
	isr_register_handler(INT_SYSCALL, _syscall);
	// init the scheduler
	init_scheduler();

	tty_write("Done booting.\n\n");
}

void prog_to_run1() {
	while(1) {
		for(int i = 0; i < 0xfffff; i++) {asm volatile("nop");}
		tty_write("proc1 running\n");

	}
}

void prog_to_run2() {
	while(1) {
		for(int i = 0; i < 0xfffff; i++) {asm volatile("nop");}
		tty_write("proc2 running\n");
	}
}

proc_t proc1;
proc_t proc2;

regs_state_t rstate;

int count = 0;

// mostly for debugging.
void* _keyboard_isr(irs_t* regs) {
	tty_write("\nKeyboard interrupt occurred:\n");
	dump_regs(regs);
	tty_write("\n");
	while(true);
	return 0;
}

void* _test_isr(irs_t* regs) {
	//tty_write("Launched\n");
	//tty_write_hex_sandwich("rstate.esp: ", rstate.esp, "\n");
	//tty_write_hex_sandwich("regs.esp: ", regs->esp, "\n");
	//tty_write_hex_sandwich("real esp: ", get_esp(), "\n");
	//regs->esp = rstate.esp;
	tty_write_hex_sandwich("returning to ", rstate.eip, "\n\n");
	//for(int i = 0; i < 0xffffffff; i++);
	switch_context(&rstate);
}

/* KERNEL ENTRY FUNCTION */
void kernel_entry(void) {
	enviro_init();
	//for(int i = 0; i < 0xfffffff; i++) { asm volatile("nop");}
	proc1.entry_mem = prog_to_run1;
	proc2.entry_mem = prog_to_run2;
	scheduler_register_task(&proc1);
	scheduler_register_task(&proc2);
	scheduler_start();

	rstate.eip = (uint32_t) _test_isr;
	rstate.esp = (uint32_t) mas_kalloc_page(PAGE_RW | PAGE_PRESENT) + 0xfff;
	rstate.ss = __KERNEL_DS;
	rstate.cs = __KERNEL_CS;
	rstate.eflags = 0x200;

	//isr_register_handler(0x69, _test_isr);
	//tty_write_hex_sandwich("Jumping to ", (uint32_t) _test_isr, "\n");
	//__asm__("int $0x69");
	//tty_write("returned to kernel.\n");

	while(true) {
		tty_write("kernel running\n");
		//for(int i = 0; i < 0x8ffffff; i++);
	}
}
