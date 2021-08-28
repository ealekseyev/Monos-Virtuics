#ifndef _PORTS_C_
#define _PORTS_C_
#include <stddef.h>

// reads one byte from specified port
inline uint8_t port_readb(uint8_t port) {
    uint8_t res;
    __asm__("in %%dx, %%al" : "=a" (res) : "d" (port));
    return res;
}

void port_read(uint8_t* buf, size_t bytes, uint8_t port) {
    for(int i = 0; i < bytes; i++) {
        buf[i] = port_readb(port);
    }
}

inline void port_writeb(uint8_t byte, uint8_t port) {
    __asm__("out %%al, %%dx" : : "a" (byte), "d" (port));
}

void port_write(uint8_t* buf, size_t bytes, uint8_t port) {
    for(int i = 0; i < bytes; i++) {
        port_writeb(buf[i], port);
    }
}


#endif