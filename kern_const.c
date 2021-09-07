// to be linked with bootloader stage 2 as well as in the kernel.
// note if accessed from assembly, the labelled values are passed as pointers via extern. use [] to dereference value.
// bare bones to make sure bls2 fits in 8kb.
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

// when modifying these, must also modify makefile
const uint32_t _bls2_size = 8192; // (sectors*512)
const uint32_t _bls2_sectors = 16;
const uint32_t _kern32_size = 32768; // (sectors*512)
const uint32_t _kern32_sectors = 64;

const uint32_t _kern_start_mem = 0x00120000; // not the same as kernel entry
const uint32_t _stack_top = 0x00ff0000;

void* _global_idt = (void*) 0x00100000; // idt
void* _global_gdt = (void*) 0x00104000; // gdt
void* _global_pd = (void*) 0x0010c000; // paging directory

// unused memory - could be faulty however
char* _global_buf = (char*) 0x500;

const uint16_t __NULL_SEG = 0;
const uint16_t __KERNEL_CS = 0x08;
const uint16_t __KERNEL_DS = 0x10;
const uint16_t __USER_CS = 0x18;
const uint16_t __USER_DS = 0x20;