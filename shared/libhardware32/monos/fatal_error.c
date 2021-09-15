#ifndef _FATAL_ERROR_C
#define _FATAL_ERROR_C
#include <io/vga.h>
#include <intel/registers.h>

#define RS_UNDEF_INT 0
#define RS_EOM       1
#define RS_DISK_ABSENT 2
#define RS_UNKNOWN 0

void kill_kern(irs_t* registers, uint8_t reason) {
    tty_set_global_color(VGA_LRED, VGA_BWHITE);
    tty_clear_screen();
    tty_writebuf("Uh Oh! The Monos Kernel has hit an error and was terminated.\n");
    tty_write_hex8_sandwich("Error Code: ", reason, "\n");
    tty_write_hex8_sandwich("Faulty Instruction location: ", registers->eip, "\n");
    while(true);
}
#endif