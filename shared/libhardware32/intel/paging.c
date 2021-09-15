#ifndef _PAGING_2_C_
#define _PAGING_2_C_
#include <stddef.h>
#include <intel/idt.c>
#include <io/vga.h>

#define PAGE_DIR_SIZE 1024
#define PAGE_TABLE_SIZE 1024
#define PAGE_SIZE 0x1000

#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_USER_ACCESSIBLE 0x4
#define PAGE_WRITE_THROUGH 0x8
#define PAGE_CACHE_DISABLED 0x10
#define PAGE_ACCESSED 0x20

#define PD_PAGE_4M 0x80

#define PT_DIRTY 0x40
#define PT_GLOBAL 0x100

typedef uint32_t page_t;

typedef struct __attribute__((packed)) {
    page_t table[PAGE_TABLE_SIZE] __attribute__((aligned(0x1000)));
} page_table_t;

typedef page_table_t page_dir_t;

extern void load_page_dir(page_dir_t* pd);
extern void enable_paging();
extern uint32_t get_pd_src(); // returns addr of currently loaded page directory
extern page_dir_t* _global_pd;

// page fault handler TODO
void _page_fault(irs_t* registers) {
    //uint8_t color_buf = tty_get_global_color();
    //tty_set_global_color(color_buf >> 4, VGA_LRED);
    tty_write_hex_sandwich("Page Fault occurred: ", registers->err_code, "\n");
    //tty_set_global_color(color_buf >> 4, color_buf & 0xf);
    //while(true);
}

/// internal functions:
// could just use memcpy
/*static void _page_struct_set(page_t* page, uint32_t val) {
    page->addr = val >> 12;
    page->attrs = val & 0xfff;
}*/

static void _page_set(page_t* page, uint32_t addr, uint32_t flags) {
    *page = (addr & 0xfffff000) | (flags & 0xfff);
}


// Page Entry to Int
uint32_t paging_petoi(page_t page) {
    //return (page.addr << 12) + page.attrs;
    return (uint32_t) page;
}

void paging_clear_pd() {
    // zero the table - set all entrie to 'unmapped'
    for(int i = 0; i < PAGE_DIR_SIZE; i++) {
        _page_set(&(_global_pd->table[i]), 0x0, 0x2); // Read/Write, Kernel-mode, Not Present.
    }
}

void paging_clear_pd_entry(int entry) {
    _page_set(&(_global_pd->table[entry]), 0x0, 0x2); // Read/Write, Kernel-mode, Not Present.
}

uint32_t paging_format_page_entry(int addr, int flags) {
    return (addr & 0xfffff000) + (flags & 0xfff);
}

void paging_load_new_pd(page_dir_t* pd) {
    memcpy((void*) _global_pd, (void*) pd, sizeof(page_dir_t));
}

bool paging_is_page_present(page_t page) {
    //return page.attrs & PAGE_PRESENT;
    return page & PAGE_PRESENT;
}

void paging_clear_table(page_table_t* table) {
    for(int i = 0; i < PAGE_TABLE_SIZE; i++) {
        _page_set(&(table->table[i]), 0x0, 0x2); // Read/Write, Kernel-mode, Not Present.
    }
}

#define PAGE_KERN_DEFAULT 0x3
void paging_set_pd_entry(int entry, page_table_t* table, int flags) {
    _page_set(&(_global_pd->table[entry]), (uint32_t) table, flags);
}

page_table_t* paging_get_pd_table(int entry) {
    return (page_table_t*) (_global_pd->table[entry] & 0xfffff000);
}

page_dir_t* paging_get_pd() {
    return _global_pd;
}

page_table_t* paging_get_pd_entry(int entry) {
    return &_global_pd[entry];
}

// returns address of pd entry
page_t* paging_append_pd_entry(page_table_t* table, int flags) {
    // skip used pages
    int i = 0;
    while(paging_is_page_present(_global_pd->table[i])) {i++;}
    _page_set(&(_global_pd->table[i]), (uint32_t) table, flags);
    return &(_global_pd->table[i]);
}

// fills in table with page entries, skipping over already present pages
/*void paging_fill_table(page_table_t* tbl, uint32_t start_addr, uint32_t end_addr, int flags) {
    uint32_t cur_addr = start_addr;
    int index = 0;
    while(cur_addr < end_addr) {
        if(paging_is_page_present(tbl->table[index])) {
            index++;
            continue;
        }
        //tbl->table[index].addr = (cur_addr >> 12);
        //tbl->table[index].attrs = flags;
        
        _page_struct_set(&(tbl->table[index]), 0x100000 | 0x20);
        //tty_write_hex_sandwich("", (cur_addr), "\n");


        //((page_t*)(table+(sizeof(page_t)*index)))->addr = cur_addr >> 12;
        //((page_t*)(table+(sizeof(page_t)*index)))->attrs = flags;
        cur_addr += PAGE_SIZE;
        index++;
    }
}*/

void paging_fill_table(page_table_t* table, uint32_t start_addr, uint32_t end_addr, int flags) {
    uint32_t cur_addr = start_addr;
    int index = 0;
    while(cur_addr < end_addr) {
        if(paging_is_page_present(table->table[index])) {
            index++;
            continue;
        }
        //tbl->table[index].addr = (cur_addr >> 12);
        //tbl->table[index].attrs = flags;
        
        _page_set(&table->table[index], cur_addr,  flags);
        //tty_write_hex_sandwich("", (cur_addr), "\n");


        //((page_t*)(table+(sizeof(page_t)*index)))->addr = cur_addr >> 12;
        //((page_t*)(table+(sizeof(page_t)*index)))->attrs = flags;
        cur_addr += PAGE_SIZE;
        index++;
    }
}

void paging_set_page(page_t* page, uint32_t mem, int flags) {
    *page = (mem & 0xfffff000) + (flags & 0xfff);
}

/*page_t* paging_register_page(page_t* page) {
    for(int i = 0; i < PAGE_DIR_SIZE; i++) {
        if(paging_is_page_present(paging_get_pd_entry(i))) {
            page_table_t* table = paging_get_pd_table(i);
            for(int j = 0; j < PAGE_TABLE_SIZE; j++) {
                if(!paging_is_page_present(&table[j]) {
                    table[j] = page;
                    return;
                }
            }
        }
    }
    // if it could not register in existing table.
}*/
/*
typedef struct __attribute__((packed)) {
    unsigned int addr: 20;
    unsigned int attrs: 12;
} page_t;

typedef struct __attribute__((packed)) {
    page_t table[PAGE_TABLE_SIZE] __attribute__((aligned(0x1000)));
} page_table_t;*/

// remember to call enable_paging once evreything is in place.
void init_paging() {
    paging_clear_pd();
    load_page_dir((page_dir_t*) _global_pd);
    isr_register_handler(ISR_PAGE_FAULT, &_page_fault);
}

#endif