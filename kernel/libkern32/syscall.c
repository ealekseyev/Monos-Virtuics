#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <intel/idt.c>
#include <proc/proc.c>

#define INT_SYSCALL 0x80

#define SYSCALL_SLEEP 0x0
#define SYSCALL_STDOUT 0x1
#define SYSCALL_STDIN 0x2
#define SYSCALL_NEWPROC 0x3
#define SYSCALL_ENDPROC 0x4

extern proc_t** _cur_proc_ptr;

// uses sysv abi instead of standard stack
void _syscall(irs_t* registers) {
    int syscall_code = registers->eax;
    // stdout
    switch(syscall_code) {
        case SYSCALL_SLEEP:
            // so far just suspend the process
            //_cur_proc_ptr->sleep = (int)registers->ebx;
            break;
        case SYSCALL_STDOUT:
            tty_writebuf(((char*)(registers->ebx)));
            break;
        case SYSCALL_ENDPROC:
            (*_cur_proc_ptr)->_active = false;
            // scheduler will wipe this task on next rotation.
            break;
    }
}

#endif