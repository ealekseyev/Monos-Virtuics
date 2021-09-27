#ifndef _FATAL_ERROR_C
#define _FATAL_ERROR_C
#include <io/vga.h>
#include <intel/registers.h>

#define RS_UNKNOWN -1
#define RS_UNDEF_INT 0
#define RS_EOM       1
#define RS_DISK_ABSENT 2
#define RS_PAGE_FAULT 3

void kill_kern(irs_t* regs, int8_t reason) {
    tty_set_global_color(VGA_LRED, VGA_BWHITE);
    tty_clear_screen();
    tty_writebuf("Uh Oh! The Monos Kernel hit an error and was terminated.\n");
    tty_write_hex8_sandwich ("Kernel termination citation code: ", reason, "\n");
    tty_write_hex8_sandwich ("Interrupt?:                       ", regs->int_no, "\n");
    tty_write_hex32_sandwich("Interrupt Err Code:               ", regs->err_code, "\n\n");

    tty_write_hex32_sandwich("eax: ", regs->eax, " ");
	tty_write_hex32_sandwich("ebx: ", regs->ebx, " ");
	tty_write_hex32_sandwich("ecx: ", regs->ecx, " ");
	tty_write_hex32_sandwich("edx: ", regs->edx, "\n");
	tty_write_hex32_sandwich("esi: ", regs->esi, " ");
	tty_write_hex32_sandwich("edi: ", regs->edi, "\n");

	tty_write_hex32_sandwich("esp: ", regs->esp, " ");
	tty_write_hex32_sandwich("ebp: ", regs->ebp, " ");
	tty_write_hex32_sandwich("eip: ", regs->eip, " ");
	tty_write_hex32_sandwich("esp2?: ", regs->useresp, "\n");

	tty_write_hex16_sandwich("cs: ", regs->cs, " ");
	tty_write_hex16_sandwich("ds: ", regs->ds, " ");
	tty_write_hex16_sandwich("ss: ", regs->ss, " ");
	tty_write_hex16_sandwich("es: ", regs->es, " ");
	tty_write_hex16_sandwich("fs: ", regs->fs, " ");
	tty_write_hex16_sandwich("gs: ", regs->gs, "\n");

	tty_write_hex8_sandwich("int_no: ", regs->int_no, " ");
	tty_write_hex32_sandwich("eflags: ", regs->eflags, "\n");

    //dump_regs(registers);
    //tty_write_hex32_sandwich("Last EIP Value:                   ", registers->eip, "\n");
    // TODO: replace with cli and hlt or smth
    while(true);
}
#endif