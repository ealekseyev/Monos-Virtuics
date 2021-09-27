#ifndef _REGISTERS_H_
#define _REGISTERS_H_
// struct with 32 bit registers
typedef struct __attribute__((packed)){
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} irs_t; // interrupt register state type

extern void set_eax(uint32_t);
extern void set_ebx(uint32_t);
extern void set_ecx(uint32_t);
extern void set_edx(uint32_t);

void dump_regs(irs_t* regs) {
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
}
#endif