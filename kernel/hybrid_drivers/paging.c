#ifndef _PAGING_C_
#define _PAGING_G_
#include <stddef.h>

#define PAGE_DIR_SIZE 1024

/*typedef struct __attribute__((__packed__)) {
    int flags: 12;        // p, r, u, w, c, a, d, 0, g, available (bits 9-11)
    int aligned_addr: 20; // 4 kilobyte aligned address
} page_dir_t;
*/
typedef struct __attribute__((__packed__)) {
    int present: 1;
    int rw: 1;
    int user: 1;
    int rsvd: 2;
    int accessed: 1;
    int dirty: 1;
    int internal: 2;
    int kern_flags: 3;
    int aligned_addr: 20; // 4 kilobyte aligned address
} page_table_t;

typedef uint32_t page_table_t; 
typedef uint32_t page_dir_t;

extern void load_page_dir(page_dir_t* pd);
extern void enable_paging();
extern void test_extern(char* str);

void init_page_dir(page_dir_t* page_dir) {
    for(int i = 0; i < PAGE_DIR_SIZE; i++) {
        page_dir[i] = 0x00000002; // Read/Write, Kernel-mode, Not Present.
    }
}
#endif