#ifndef _PROC_C_
#define _PROC_C_
#include <intel/paging.c>
#include <intel/ldt.c>

// TODO: remove compatability with regs_state_t
typedef struct __attribute__((packed)){
    uint32_t ds;
    uint32_t esp;
    uint32_t eflags;
    uint32_t cs;
    uint32_t eip;
} iret_args_t;

typedef struct {
    // for pushing to iret
    uint32_t ss;
    uint32_t esp;
    uint32_t eflags;
    uint32_t cs;
    uint32_t eip;

    uint32_t eax, ebx, ecx, edx, esi, edi;
    // optional, at the end
    uint32_t ds, es, fs, gs;
} regs_state_t;

// underscored values - for scheduler reference
typedef struct {
    int _id;            // process id (for os)
    char name[32];      // string name of process
    uint8_t cpl;        // current privelege level
    regs_state_t _iret; // first section of this struct is compatible with iret_args_t
    void* entry_mem;    // jump location
    void* _next_proc;    // proc_t
    uint32_t _sleep;     // time in system ticks to relaunch. If it's 0 ignore.
    bool _active;        // should always be true; once it is false scheduler will dealloc it.
    void* _page;
} proc_t;


#endif