#include <ports.c>

uint16_t get_installed_mem_cmos() {
    uint16_t total;
    uint8_t lowmem, highmem;
 
    port_writeb(0x70, 0x30);
    lowmem = port_readb(0x71);
    port_writeb(0x70, 0x31);
    highmem = port_readb(0x71);
 
    total = lowmem | highmem << 8;
    return total;
}