/// To be used by stage 2 bootloader and main kernel VGA/TTY driver - NOT the kernel directly.

#ifndef _VGA_H_
#define _VGA_H_

#include <stddef.h>
#include <stdlib.h>
#include <memset.c>
#include <string.h>
#include <ports.c>

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

// vram actually starts at 0xa0000
static char* vidmem = (char*)0xb8000; 	// video memory begins here
static char* vidmem_end = (char*)0xc0000; // end of vram
static const unsigned char Xlen = 80;
static const unsigned char Ylen = 25;

static unsigned char global_color = 0x07;

static uint8_t cursorX = 0;
static uint8_t cursorY = 0;

void set_cursor(uint8_t, uint8_t);

static inline uint32_t* get_vram_offset(uint8_t x, uint8_t y) {
    return (uint32_t*)(vidmem + (x + y*Xlen)*2);
}

void writebufto(char* str, int x, int y) {
    char* vidmem_offset = (char*) get_vram_offset(x, y);
    unsigned int j = 0;
    while(str[j] != '\0') {
        *(vidmem_offset+(j*2)) = str[j];
        *(vidmem_offset+(j*2)+1) = global_color;
        j++;
    }
}

void vga_scroll(uint8_t lines) {
    //uint8_t prev_x, prev_y;
    //prev_x = cursorX; prev_y = cursorY;
    char* vidmem_offset = (char*) get_vram_offset(0, lines);
    memcpy(vidmem, vidmem_offset, ((Xlen-lines)*Ylen*2));
    for(int i = 0; i < lines*Xlen*2; i+=2) {
        // clear last lines
        vidmem_offset = (char*) get_vram_offset(0, Ylen - lines);
        vidmem_offset[i] = ' ';
        vidmem_offset[i+1] = global_color;
    }
    set_cursor(cursorX, cursorY-lines);
}

void tty_writebuf(char* str) {
    char* vidmem_offset;
    unsigned int j = 0;
    // do we scroll?
    if(cursorY == Ylen) {
        vga_scroll(1);
    }
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
void clear_screen() {
    unsigned int j = 0;
    while(j < Xlen * Ylen * 2) {
		vidmem[j] = ' ';
		vidmem[j+1] = global_color; 		
		j += 2;
	}
    set_cursor(0, 0);
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
uint8_t getcolor(uint8_t bg, uint8_t fg) {
    return (bg << 4) + fg;
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

//TODO: test
void get_cursor(uint8_t* x, uint8_t* y) {
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_readb(VGA_DATA_REGISTER) << 8;
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_readb(VGA_DATA_REGISTER);
    *x = offset%Xlen;
    *y = (offset-(*x))/Ylen;
    //return offset * 2;
}

extern char* _global_buf;
void printword(uint16_t word) {
	tty_writebuf(itoa(word, _global_buf, 16));
}
void printint(uint32_t hex) {
	tty_writebuf(itoa(hex, _global_buf, 16));
}

void printint_dec(uint32_t dec) {
	tty_writebuf(itoa(dec, _global_buf, 10));
}

#endif