#ifndef _PORTS_C_
#define _PORTS_C_
#include <stddef.h>

// reads one byte from specified port
uint8_t port_readb(uint16_t port) {
    uint8_t res;
    __asm__("in %%dx, %%al" : "=a" (res) : "d" (port));
    return res;
}
// reads two bytes from specified port
uint16_t port_readw(uint16_t port) {
    uint16_t res;
    __asm__("in %%dx, %%ax" : "=a" (res) : "d" (port));
    return res;
}
// reads four bytes from specified port
uint32_t port_readl(uint16_t port) {
    uint32_t res;
    __asm__("in %%dx, %%eax" : "=a" (res) : "d" (port));
    return res;
}

void port_read(uint16_t port, uint8_t* buf, size_t bytes) {
    for(int i = 0; i < bytes; i++) {
        buf[i] = port_readb(port);
    }
}

// writes a byte to specified port
void port_writeb(uint16_t port, uint8_t byte) {
    __asm__("out %%al, %%dx" : : "a" (byte), "d" (port));
}
// writes two bytes to specified port
void port_writew(uint16_t port, uint16_t byte) {
    __asm__("out %%ax, %%dx" : : "a" (byte), "d" (port));
}
// writes four bytes to specified port
void port_writel(uint16_t port, uint32_t byte) {
    __asm__("out %%eax, %%dx" : : "a" (byte), "d" (port));
}

void port_write(uint16_t port, uint8_t* buf, size_t bytes) {
    for(int i = 0; i < bytes; i++) {
        port_writeb(buf[i], port);
    }
}


#endif