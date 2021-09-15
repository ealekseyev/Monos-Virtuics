#ifndef _MAS_C_
#define _MAS_C_
#include <stddef.h>
#include <memcpy.c>
#include <intel/paging.c>

extern uint32_t get_esp();
extern uint32_t get_ebp();

void init_mas() {

}
/*
void* kalloc_page(int flags) {
    for(int i = 0; i < PAGE_DIR_SIZE; i++) {
        if(paging_is_page_present(paging_get_pd_entry(i))) {
            page_table_t* table = paging_get_pd_table(i);
            for(int j = 0; j < PAGE_TABLE_SIZE; j++) {
                if(!paging_is_page_present(&table[j]) {
                    //paging_set_page(&table[j], , );
                    return;
                }
            }
        }
    }
}*/

#endif