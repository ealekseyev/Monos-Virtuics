#ifndef _MAS_C_
#define _MAS_C_
#include <stddef.h>
#include <memcpy.c>
#include <intel/paging.c>

extern uint32_t get_esp();
extern uint32_t get_ebp();
extern page_table_t* _global_pd; // pointer to the page directory - 1024 pageentries
extern page_t* _global_pt_matrix; // pointer to 1024 tables with 1024 page entries each

// ODO: have pd dynamically change entries from present to non-present
// to improve search speed when kalloc'ing

void init_mas() {
    init_paging();
    // set the entrire matrix to not present and link pd to this matrix.
    for(int i = 0; i < PAGE_DIR_SIZE; i++) {
        paging_set_pd_entry(i, (page_table_t*) &_global_pt_matrix[i*PAGE_DIR_SIZE], PAGE_PRESENT | PAGE_RW); // temporary parameters
        paging_clear_table((page_table_t*)&_global_pt_matrix[i*PAGE_DIR_SIZE]);
    }
}

// only do this once you have kalloc'ed pages for the kernel.
// will cause a page fault and red screen if you don't.
void mas_kern_ready() {
    enable_paging();
}

// inputs/outputs linear pt index or linear addr
int _mas_get_ind_from_addr(uint32_t addr) {
    return addr / PAGE_SIZE;
}

uint32_t _mas_get_addr_from_ind(int ind) {
    return ind * PAGE_SIZE;
}

// kalloc - does not remember the pages allocated. 
// Kernel should remember them, if at all necessary
// this is not an especially speedy subroutine. 
// try not to use it all the time
void* mas_kalloc_nonsequential_pages(uint32_t start, uint32_t end, int flags) {
    uint32_t next_addr = start;
    for(int i = 0; i < PAGE_TABLE_SIZE * PAGE_DIR_SIZE; i++) {
        if(next_addr >= end)
            return (void*) start;
        if(!paging_is_page_present(_global_pt_matrix[i])) {
            paging_set_page(&_global_pt_matrix[i], next_addr, flags);
            next_addr += PAGE_SIZE;
        }
    }
    return (void*)0x0; // could not alloc.
}

// allocated pages at selected memory. page table is treated sequentially
void* mas_register_linear_mem(uint32_t start, uint32_t end, int flags) {
    uint32_t start_ind = start / PAGE_SIZE;
    uint32_t end_ind = end / PAGE_SIZE;
    uint32_t next_addr = start;
    for(int i = start_ind; i < end_ind; i++) {
        paging_set_page(&_global_pt_matrix[i], next_addr, flags);
        next_addr += PAGE_SIZE;
    }
    return (void*)start;
}

void* mas_kalloc_page(int flags) {
    for(int i = 0; i < PAGE_TABLE_SIZE * PAGE_DIR_SIZE; i++) {
        if(!paging_is_page_present(_global_pt_matrix[i])) {
            paging_set_page(&_global_pt_matrix[i], _mas_get_addr_from_ind(i), flags);
            return (void*) (_global_pt_matrix[i] & 0xfffff000);
        }
    }
    return (void*) 0x0; // failed to alloc
}

// allocates adjacent memory.
void* mas_kalloc_adjacent_pages(int count, int flags) {
    int free_page_count = 0;
    for(int i = 0; i < PAGE_TABLE_SIZE * PAGE_DIR_SIZE; i++) {
        if(free_page_count == count) {
            for(int j = i-count; j < i; j++) {
                paging_set_page(&_global_pt_matrix[j], _mas_get_addr_from_ind(j), flags);
            }
            return (void*) _mas_get_addr_from_ind(i-count);
        }
        // free_page_count - number of adjecent, free pages
        if(!paging_is_page_present(_global_pt_matrix[i])) {
            free_page_count++;
        } else {
            free_page_count = 0;
        }
    }
    return (void*) 0x0; // failed to alloc
}
#endif