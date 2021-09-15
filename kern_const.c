// to be linked with bootloader stage 2 as well as in the kernel.
// note if accessed from assembly, the labelled values are passed as pointers via extern. use [] to dereference value.
// bare bones to make sure bls2 fits in 8kb.
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

// editing any of these file's values without proper consideration can
// fuck up the stage 2 bootloader, the kernel, or god knows what else

// must modify corresponding linker load points -
#define BL_START_MEM    0x00008000   // boot/kernloader/link.ld
#define KERN_START_MEM  0x00500000   // kernel/link.ld

// when modifying these, must also modify makefile
#define BL_SIZE     0x4000    // also must modify bootsector 'BLS2_SIZE' to match _bls2_sectors
#define KERN_SIZE   0x8000

const uint32_t _bls2_size = BL_SIZE; // (sectors*512)
const uint32_t _bls2_sectors = BL_SIZE/512; // 16
const uint32_t _kern32_size = KERN_SIZE; // (sectors*512)
const uint32_t _kern32_sectors = KERN_SIZE/512; //64

// must also modify linkage, bootsector constants
const uint32_t _bls2_start_mem = BL_START_MEM; // not the same as bootloader entry
const uint32_t _bls2_end_mem = BL_START_MEM+BL_SIZE;
// when editing kernel start mem, also modify linkage
const uint32_t _kern_start_mem = KERN_START_MEM; // not the same as kernel entry
const uint32_t _kern_end_mem = KERN_START_MEM+KERN_SIZE;

// intel data structures
void* _global_idt = (void*) 0x10000; // idt + 0x200 bytes
void* _global_gdt = (void*) 0x11000; // gdt
void* _global_pd = (void*) 0x13000; // paging directory
void* _kern_isr_handlers = (void*) 0x14000; // kernel-level isr handler array

//const uint32_t _stack_top = 0x90000; //TODO: fix paging so the ptr is at the end of memory again

// both the kernel get equal COPIES of this obj file.
// to pass data between the two 'modes', modify a preset memory location.
// Monos uses 0x9e000 - 0x9fc00 for global nonconst & nonarray data values
uint32_t* _installed_mem_loc = (uint32_t*) 0x9e000; // 0x100000; // at least 1 megabyte on all modern x86 systems since the i8086
uint8_t* _installed_cores =  (uint8_t*) 0x9e004;

void* _kern_stack_start = (void*) 0x00800000; // unfortunately, moving the stack whie the kernel is running causes all sorts of problems. we must account for this.

// unused memory - could be faulty however. c
// can be used as nullptr and such.
char* _global_buf = (char*) 0x500;

// where malloc can start alloc-ing
//void* _malloc_start = (void*) 0x50000; // paging directory

// gdt entry offsets
const uint16_t __NULL_SEG = 0;
const uint16_t __KERNEL_CS = 0x08;
const uint16_t __KERNEL_DS = 0x10;
const uint16_t __USER_CS = 0x18;
const uint16_t __USER_DS = 0x20;