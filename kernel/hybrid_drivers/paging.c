#ifndef _PAGING_C_
#define _PAGING_G_
#include <stddef.h>
#include <idt.c>
#include <pit_timer.c>

#define PAGE_DIR_SIZE 1024

typedef struct __attribute__((__packed__)) {
    int present: 1;
    int rw: 1;
    int user: 1;
    //int rsvd: 2;
    int accessed: 1;
    int dirty: 1;
    int internal: 2;
    int kern_flags: 3;
    int aligned_addr: 20; // 4 kilobyte aligned address
} page_t;

typedef uint32_t page_t; 
typedef uint32_t page_dir_t;

typedef struct __attribute__((__packed__)) {
    page_t pages[PAGE_DIR_SIZE];
} page_table_t;

extern void load_page_dir(page_dir_t* pd);
extern void enable_paging();
extern void test_extern(char* str);
extern void* _global_pd;

void _page_fault(irs_t registers);
void init_paging() {
    // zero the table - set all entrie to 'unmapped'
    for(int i = 0; i < PAGE_DIR_SIZE; i++) {
        ((page_dir_t*) _global_pd)[i] = 0x00000002; // Read/Write, Kernel-mode, Not Present.
    }
    load_page_dir((page_dir_t*) _global_pd);
    enable_paging();
    register_kern_int_handler(ISR_PAGE_FAULT, _page_fault);
}

// kernel/process must NOT free memory associated with the page table.
// pt must be 4kb aligned.
void paging_set_pd_entry(uint16_t entry, page_table_t* pt) {
    ((page_dir_t*) _global_pd)[entry] = pt & 0xfffffff000 + 0b00000001111;
}

// overwrites old paging directory and initializes it (prolly not necessary)
void paging_copy_new_pd(page_dir_t* pd) {
    memcpy(_global_pd, (void*) pd, sizeof(page_dir_t)*PAGE_DIR_SIZE);
    load_page_dir((page_dir_t*) _global_pd);
}

page_t* paging_get_page(void* address) {
    return (page_t*) 0;
}
// page fault handler
void _page_fault(irs_t registers) {
    
}
extern void* _malloc_start;
static void* alloc_ptr;
// memory management unit
void init_mms() {
    alloc_ptr = _malloc_start;
}

void* kmalloc(uint32_t bytes) {
    void* tmp_ptr = alloc_ptr;
    alloc_ptr+=bytes;
    return tmp_ptr;
}
#endif