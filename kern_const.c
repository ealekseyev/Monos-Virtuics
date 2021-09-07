// to be linked with bootloader stage 2 as well as in the kernel.
typedef unsigned int uint32_t;

// when modifying these, must also modify makefile
const uint32_t _bls2_size = 8192; // (sectors*512)
const uint32_t _bls2_sectors = 16;
const uint32_t _kern32_size = 32768; // (sectors*512)
const uint32_t _kern32_sectors = 64;

const uint32_t stack_top = 0x00ff0000;

void* _global_idt = (void*) 0x00100000; // idt
void* _global_gdt = (void*) 0x00104000; // gdt
void* _global_pd = (void*) 0x0010c000; // paging directory

// unused memory - could be faulty however
char* _global_buf = (char*) 0x500;

const uint32_t __NULL_SEG = 0;
const uint32_t __KERNEL_CS = 8;
const uint32_t __KERNEL_DS = 16;
const uint32_t __USER_CS = 24;
const uint32_t __USER_DS = 32;