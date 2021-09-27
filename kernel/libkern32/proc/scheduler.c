#ifndef _SCHEDULER_C_
#define _SCHEDULER_C_
#include <proc/proc.c>
#include <timer.c>
#include <intel/registers.h>
#include <io/pic.c>
#include <memcmp.c>
#include <mas.c>

#define MAX_PROCS 20
#define SWITCHES_PER_SEC 100

// registers
__attribute__((noreturn))
extern void switch_context(regs_state_t*);

extern proc_t** _cur_proc_ptr;
extern proc_t* _global_proc_tbl;
//proc_t* proc_ptr; // current process
proc_t* first_proc;
proc_t* last_proc;
proc_t null_proc;
proc_t kernel;

size_t proc_count;

int __ticks = 0;

// TODO: I think the kernel is fucking up esp
// thru the first proc. Fix that!
// TODO: determine why regs are getting overwritten across tasks.
void* _schedule_task(irs_t* registers) {
    // store registers for next proc cycle
    // critical phase - do not allow interrupts to fire
    //asm volatile("cli");
    //tty_write_hex_sandwich("(SCHDLR: STORE) proc: ", (*_cur_proc_ptr)->_id, " ");
    //tty_write_hex_sandwich("esp: ", registers->esp, " ");
    //tty_write_hex_sandwich("eip: ", registers->eip, " ");
    //tty_write_hex_sandwich("ef_: ", registers->eflags, " ");
    //tty_write_hex_sandwich("ss: ", registers->ss, "\n\n");
    (*_cur_proc_ptr)->_iret.eax = registers->eax;
    (*_cur_proc_ptr)->_iret.ebx = registers->ebx;
    (*_cur_proc_ptr)->_iret.ecx = registers->ecx;
    (*_cur_proc_ptr)->_iret.edx = registers->edx;
    (*_cur_proc_ptr)->_iret.esi = registers->esi;
    (*_cur_proc_ptr)->_iret.edi = registers->edi;

    (*_cur_proc_ptr)->_iret.eflags = registers->eflags;
    (*_cur_proc_ptr)->_iret.eip = registers->eip;
    (*_cur_proc_ptr)->_iret.esp = registers->esp;
    (*_cur_proc_ptr)->_iret.ss = __KERNEL_DS;
    (*_cur_proc_ptr)->_iret.ds = __KERNEL_CS;

    // move to next process
    (*_cur_proc_ptr) = (proc_t*) (*_cur_proc_ptr)->_next_proc;
    if((*_cur_proc_ptr)->_id == 0) {
        (*_cur_proc_ptr) = (proc_t*) (*_cur_proc_ptr)->_next_proc;
    }
    //tty_write_hex_sandwich("(SCHDLR: RESTORE) proc: ", (*_cur_proc_ptr)->_id, " ");
    //tty_write_hex_sandwich("esp: ", (*_cur_proc_ptr)->_iret.esp, " ");
    //tty_write_hex_sandwich("eip: ", (*_cur_proc_ptr)->_iret.eip, " ");
    //tty_write_hex_sandwich("ef_: ", (*_cur_proc_ptr)->_iret.eflags, " ");
    //tty_write_hex_sandwich("ss: ", (*_cur_proc_ptr)->_iret.ss, "\n");
    //asm volatile("lb1: jmp lb1");

    __ticks++;
    master_pic_irq_done();
    switch_context(&((*_cur_proc_ptr)->_iret));
    // _iret already has all saved values from last process cycle
    //return (void*) &((*_cur_proc_ptr)->_iret); //(void*) (*_cur_proc_ptr)->_regs.eip; // this will initiate a jump
}

void scheduler_start() {
    //tty_write_hex32_sandwich("Scheduler handler: ", (uint32_t)_schedule_task, "\n");
    init_pit(SWITCHES_PER_SEC, _schedule_task); // set proc switching handler
}

int scheduler_locate_slot() {
    int count = 0;
    while(memcmp((void*) &_global_proc_tbl[count], (void*) &null_proc, sizeof(proc_t)) != 0) count++;
    return count;
}

//TODO: does not work. must use linked list
bool scheduler_proc_exists(int id) {
    return memcmp((void*) &_global_proc_tbl[id], (void*) &null_proc, sizeof(proc_t)) != 0;
}

void scheduler_register_kernel() {
    last_proc = &_global_proc_tbl[0];
    first_proc = &_global_proc_tbl[0];
    _global_proc_tbl[0]._id = 0;
    _global_proc_tbl[0]._active = true;
    _global_proc_tbl[0]._sleep = 0;
    _global_proc_tbl[0]._next_proc = first_proc; // set next proc to be the first
    last_proc->_next_proc = &_global_proc_tbl[0];  // set the previous proc to link to this one instead of first_proc
    // don't set eip, esp, other registers etc. will be overwritten on first timer fire anyways.
    proc_count++;
}

void scheduler_register_task(proc_t* process) {
    int id = scheduler_locate_slot();
    process->_id = id;
    process->_active = true;
    process->_sleep = 0;
    process->_next_proc = first_proc; // set next proc to be the first
    last_proc->_next_proc = process;  // set the previous proc to link to this one instead of first_proc
    last_proc = process;              // declare that this is the new last proc, which links to the first
    // copy changes
    memcpy((void*) &_global_proc_tbl[id], (void*) process, sizeof(proc_t));
    //tty_write_hex_sandwich("Registered process ", id, " @ ");
    //tty_write_hex32_sandwich("", (uint32_t)&_global_proc_tbl[id], "\n");

    process->_page = mas_kalloc_page(PAGE_PRESENT | PAGE_RW | PAGE_USER_ACCESSIBLE);
    //process->_iret.ebp = (uint32_t) process->_page+PAGE_SIZE-1;
    process->_iret.ss = (uint32_t) __KERNEL_DS;
    process->_iret.ds = (uint32_t) __KERNEL_DS;
    process->_iret.cs = (uint32_t) __KERNEL_CS;
    process->_iret.esp = (uint32_t) process->_page+PAGE_SIZE-1;
    process->_iret.eip = (uint32_t) process->entry_mem; //instruction pointer starts at beginning of subroutine
    process->_iret.eflags = 0x200; //TODO: figure out default eflags for user mode
    proc_count++;

    tty_write_hex_sandwich("(SCHDLR: RGSTR) proc: ", process->_id, "  ");
    //tty_write_hex_sandwich("ebp: ", process->_regs.ebp, "  ");
    tty_write_hex_sandwich("esp: ", process->_iret.esp, "  ");
    tty_write_hex_sandwich("eip: ", process->_iret.eip, "  ");
    tty_write_hex_sandwich("entry: ", (uint32_t) process->entry_mem, "\n");
    tty_write_hex_sandwich("next proc: ", (uint32_t) process->_next_proc, "  ");
    tty_write_hex_sandwich("page: ", (uint32_t) process->_page, "\n\n");
}

void scheduler_kill_task(int id) {
    memcpy((void*) &_global_proc_tbl[id], (void*) &null_proc, sizeof(proc_t));
}

void init_scheduler() {
    memset((void*) _global_proc_tbl, 0, sizeof(proc_t)*MAX_PROCS); // zero proc table
    memset((void*) &null_proc, 0, sizeof(proc_t)); // set null proc for reference
    // register task 0 in kernel's name
    // doesnt matter what the default proc_t val is - 
    // will be overwritten on first timer interrupt
    scheduler_register_kernel();
    (*_cur_proc_ptr) = &_global_proc_tbl[0];
}
#endif