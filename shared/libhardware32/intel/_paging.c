#ifndef _PAGING_C_
#define _PAGING_C_
#include <stddef.h>
#include <intel/idt.c>

#define PAGE_DIR_SIZE 1024
#define PAGE_TABLE_SIZE 1024
#define PAGE_SIZE 0x1000

#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER_ACCESSIBLE 0x4
#define PAGE_WRITE_THROUGH 0x8
#define PAGE_CACHE_DISABLED 0x10
#define PAGE_ACCESSED 0x20
#define PAGE_DIRTY 0x40
#define PAGE_GLOBAL 0x100 // skip a bit

typedef uint32_t page_t; 
typedef uint32_t page_entry_t; 
typedef uint32_t page_dir_t;

extern void load_page_dir(page_dir_t* pd);
extern void enable_paging();
extern uint32_t get_pd_src(); // returns addr of currently loaded page directory

extern void* _global_pd;
#define PAGE_DIR ((page_dir_t*) _global_pd)

typedef struct {
    uint16_t pd_index; // just a number. pd can move around
    page_entry_t* page_entry; // addr of entry
} page_ptr_t;

// page fault handler TODO
void _page_fault(irs_t* registers) {
    uint8_t color_buf = tty_get_global_color();
    tty_set_global_color(color_buf >> 4, VGA_LRED);
    tty_write_hex_sandwich("Page Fault occurred: ", registers->err_code, "\n");
    tty_set_global_color(color_buf >> 4, color_buf & 0xf);
}

void paging_reset_pd() {
    // zero the table - set all entrie to 'unmapped'
    for(int i = 0; i < PAGE_DIR_SIZE; i++) {
        ((page_dir_t*) _global_pd)[i] = 0x00000002; // Read/Write, Kernel-mode, Not Present.
    }
}
void init_paging() {
    paging_reset_pd();
    load_page_dir((page_dir_t*) _global_pd);
    //enable_paging();
    isr_register_handler(ISR_PAGE_FAULT, &_page_fault);
}

// kernel/process must NOT free memory associated with the page table.
// pt is 4kb aligned first entry of 1024 entry sequential table.
#define DEFAULT_FLAGS 0b1111
void paging_set_pd_entry(uint16_t entry, page_entry_t* pt, int flags) {
    ((page_dir_t*) _global_pd)[entry] = (page_dir_t)((((uint32_t) pt) & 0xfffff000) + flags); //0b00000001111);
    //tty_writebuf("global page dir: ");
    //printint(((page_dir_t*) _global_pd)[entry]);
    //tty_writebuf("\n");
}

void paging_clear_pd_entry(uint16_t entry) {
    ((page_dir_t*) _global_pd)[entry] = 0x2;
    //tty_writebuf("global page dir: ");
    //printint(((page_dir_t*) _global_pd)[entry]);
    //tty_writebuf("\n");
}

void paging_clear_pt_entry(page_entry_t* table, uint16_t entry) {
    table[entry] = 0x2;
}

void paging_set_pt_entry(page_entry_t* entry, page_entry_t* page, int flags) {
    *entry = ((uint32_t)page & 0xfffff000) + flags;
}


// overwrites old paging directory and initializes it (prolly not necessary)
void paging_copy_new_pd(page_dir_t* pd) {
    memcpy(_global_pd, (void*) pd, sizeof(page_dir_t)*PAGE_DIR_SIZE);
    load_page_dir((page_dir_t*) _global_pd);
}


void paging_copy_page(page_ptr_t dst_page, page_ptr_t src_page) {
    /// 1: rewrite page entry
    /// 2: copy page contents

    page_entry_t* dst_pt = paging_get_pt_from_gpd(dst_page.pd_index);    // get page table in question
    page_t* dst_page = paging_get_page(dst_pt, dst_page.pt_index);    // pointer to dst page in memory

    page_entry_t* src_pt = paging_get_pt_from_gpd(src_page.pd_index);
    page_t* src_page = paging_get_page(dst_pt, src_page.pt_index);    // pointer to src page in memory

    memcpy(dst_page, src_page, PAGE_SIZE);

    paging_clear_pt_entry();

    return;
}

// fills one page table - 4M usable memory per table
bool paging_fill_pt(page_entry_t* table, uint32_t phys_addr, int flags) {
    if(phys_addr % 0x1000 != 0) return false;
    if((uint32_t)table % 0x1000 != 0) return false;

    for(int i = 0; i < 1024; i++) {
        table[i] = (phys_addr + (i * 0x1000)) | flags;
    }
    return true;
}
/*
void* get_page_entry(uint32_t entry_num) {

}

// returns index in 1d page dir/table matrix
int paging_does_entry_exist(void* addr) {
    void* addr_aligned = addr & 0x1000;
    for(int i = 0; i < PAGE_DIR_SIZE; i++) {
        if(paging_is_gpd_entry_present(i)) {
            
        }
    }
}

// check if page present bit is set.
// gpd - global page dir
// TODO: check
bool paging_is_gpd_entry_present(uint16_t entry) {
    return ((page_dir_t*)_global_pd)[entry] & PAGE_PRESENT;
}
// TODO: check
bool paging_is_page_present(page_entry_t page) {
    return page & PAGE_PRESENT;
}*/

// convert 4kb aligned to regular addr
page_entry_t* paging_get_pt_from_gpd(uint16_t entry) {
    return (page_entry_t*)(((page_dir_t*)_global_pd)[entry] & 0xfffff000);
}

// returns mem_addr of page entry
page_entry_t* paging_get_page(page_entry_t* table, uint16_t entry) {
    return (page_entry_t*)(table[entry] & 0xfffff000);
}
#endif