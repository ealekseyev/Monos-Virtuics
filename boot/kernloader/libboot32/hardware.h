#ifndef _HARDWARE_H_
#define _HARDWARE_H_
#include <io/vga.h>

//#include <stddef.h>
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

// probe memory. cannot be run when paging is enabled.
// writes to one byte of memory and checks if it was written;
// if not, it means it is either a) a memory hole or 
// b) end of installed memory
uint32_t memory_probe(uint32_t start_addr, int gradient) {
	for(uint32_t i = start_addr; i < 0xffffffff; i+=gradient) {
		// buffer byte
		uint8_t buf = *((uint8_t*)i);
		*((uint8_t*)i) += 1;
		if(*((uint8_t*)i) == buf) {
			return i;
		}
		// restore previous value
		*((uint8_t*)i) = buf;
		//printint(i);
		//tty_writebuf("\n");
	}
	return 0xffffffff;
}

// scans in 4kb chunks, then once it sees end of memory
// scans last 4kb for exact end of phys addr spce.
uint32_t get_eom_addr(uint32_t start_addr) {
    uint32_t eof_4kb = memory_probe(start_addr, 0x10000);
    return memory_probe(eof_4kb-0x10000, 1);
}
#endif