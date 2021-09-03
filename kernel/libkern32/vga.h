/// To be used by stage 2 bootloader and main kernel VGA/TTY driver - NOT the kernel directly.

#ifndef _VGA_H_
#define _VGA_H_

#include <stddef.h>
#include <memset.c>
#include <string.h>
#include "../libkern32/ports.c"

#define VGA_BLACK 0
#define VGA_BLUE 1
#define VGA_GREEN 2
#define VGA_CYAN 3
#define VGA_RED 4
#define VGA_MAGENTA 5
#define VGA_BROWN 6
#define VGA_WHITE 7
#define VGA_GRAY 8
#define VGA_LBLUE 9
#define VGA_LGREEN 10
#define VGA_LCYAN 11
#define VGA_LRED 12
#define VGA_LMAGENTA 13
#define VGA_YELLOW 14
#define VGA_BWHITE 15

static char* vidmem = (char*)0xb8000; 	// video memory begins here
static const unsigned char Xlen = 80;
static const unsigned char Ylen = 25;

static unsigned char global_color = 0x07;

static uint8_t cursorX = 0;
static uint8_t cursorY = 0;

static inline uint32_t get_vram_offset(uint8_t x, uint8_t y) {
    return vidmem + (x + y*Xlen)*2;
}

void writebufto(char* str, int x, int y) {
    char* vidmem_offset = get_vram_offset(x, y);
    unsigned int j = 0;
    while(str[j] != '\0') {
        *(vidmem_offset+(j*2)) = str[j];
        *(vidmem_offset+(j*2)+1) = global_color;
        j++;
    }
}

void tty_writebuf(char* str) {
    char* vidmem_offset;
    unsigned int j = 0;
    while(str[j] != '\0') {
        // handle newline and return characters
        if(str[j] == '\n') {
            cursorX = 0; cursorY += 1;
            j++;
            continue;
        } else if(str[j] == '\r') {
            cursorX = 0;
            j++;
            continue;
        }
        vidmem_offset = (char*) get_vram_offset(cursorX, cursorY);
        *(vidmem_offset) = str[j];
        *(vidmem_offset+1) = global_color;
        // increment cursor
        if(cursorX < Xlen) {
            cursorX++;
        } else if(cursorX == Xlen) {
            cursorX = 0;
            cursorY += 1;
        }
        j++;
    }
    set_cursor(cursorX, cursorY);
}

void dumpstr(char* str) {
    writebufto(str, 0, 0);
}
void clearscreen() {
    unsigned int j = 0;
    while(j < Xlen * Ylen * 2) {
		vidmem[j] = ' ';
		vidmem[j+1] = global_color; 		
		j += 2;
	}
}

void dumphex32(uint32_t hex) {
	uint8_t bit_offset = 28;
	//while(bit_offset >= 0) {
	for(int dignum = 0; dignum < 8; dignum++) {
		uint8_t hex_dig = (hex >> bit_offset) & 0xf;
		char hex_char;
		if(hex_dig < 10) {
			hex_char = 48 + hex_dig;
		} else {
			hex_char = 97 + (hex_dig - 10);
		}
		*(vidmem+(dignum*2)) = hex_char;
		bit_offset -= 4;
	}
}

void setcolor(uint8_t bg, uint8_t fg) {
    global_color = (bg << 4) + fg;
}
void getcolor(uint8_t bg, uint8_t fg) {
    return bg << 4 + fg;
}

// writing to ports part
#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

void _set_cursor(uint16_t offset) {
    //offset /= 2;
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_writeb(VGA_DATA_REGISTER, (uint8_t) (offset >> 8));
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_writeb(VGA_DATA_REGISTER, (uint8_t) (offset & 0xff));
}

void set_cursor(uint8_t x, uint8_t y) {
    _set_cursor((y*Xlen)+x);
    cursorX = x;
    cursorY = y;
}

int get_cursor() {
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_readb(VGA_DATA_REGISTER) << 8;
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_readb(VGA_DATA_REGISTER);
    return offset * 2;
}

#endif