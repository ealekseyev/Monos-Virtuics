/// To be used by stage 2 bootloader and main kernel VGA/TTY driver - NOT the kernel directly.

#ifndef _VGA_H_
#define _VGA_H_

#include <stddef.h>
#include <stdlib.h>
#include <memset.c>
#include <string.h>
#include <io/ports.c>

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

static uint8_t global_color = 0x07;

static uint8_t cursorX = 0;
static uint8_t cursorY = 0;

typedef struct {
    int fg:4;
    int bg:4;
} vga_color_t;

// writing to ports part
#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

void _tty_set_cursor(uint16_t offset) {
    //offset /= 2;
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_writeb(VGA_DATA_REGISTER, (uint8_t) (offset >> 8));
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_writeb(VGA_DATA_REGISTER, (uint8_t) (offset & 0xff));
}

void tty_set_cursor(uint8_t x, uint8_t y) {
    _tty_set_cursor((y*Xlen)+x);
    cursorX = x;
    cursorY = y;
}

void tty_get_cursor(uint8_t* x, uint8_t* y) {
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_readb(VGA_DATA_REGISTER) << 8;
    port_writeb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_readb(VGA_DATA_REGISTER);
    //offset/=2;
    *x = offset%Xlen;
    *y = (offset-(*x))/Xlen;
}
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

void tty_scroll(uint8_t lines) {
    tty_get_cursor(&cursorX, &cursorY);
    char* vidmem_offset = (char*) get_vram_offset(0, lines);
    memcpy(vidmem, vidmem_offset, ((Xlen-lines)*Ylen*2));
    for(int i = 0; i < lines*Xlen*2; i+=2) {
        // clear last lines
        vidmem_offset = (char*) get_vram_offset(0, Ylen - lines);
        vidmem_offset[i] = ' ';
        vidmem_offset[i+1] = global_color;
    }
    tty_set_cursor(cursorX, cursorY-lines);
}
#define tty_write tty_writebuf
void tty_writebuf(char* str) {
    tty_get_cursor(&cursorX, &cursorY);
    char* vidmem_offset;
    unsigned int j = 0;
    // do we scroll?
    if(cursorY == Ylen) {
        tty_scroll(1);
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
    tty_set_cursor(cursorX, cursorY);
}

void tty_clear_screen() {
    unsigned int j = 0;
    while(j < Xlen * Ylen * 2) {
		vidmem[j] = ' ';
		vidmem[j+1] = global_color; 		
		j += 2;
	}
    tty_set_cursor(0, 0);
}

void tty_set_global_color(uint8_t bg, uint8_t fg) {
    global_color = (bg << 4) + fg;
}
uint8_t tty_get_global_color() {
    return global_color;
}
void tty_get_global_color_ptr(uint8_t* bg, uint8_t* fg) {
    *bg = global_color >> 4;
    *fg = global_color & 0x0f;
}
uint8_t tty_calculate_color(uint8_t bg, uint8_t fg) {
    return (bg << 4) + fg;
}

extern char* _global_buf;
void printword(uint16_t word) {
	tty_writebuf(itoa(word, _global_buf, 16));
}
void printint(uint32_t hex) {
	tty_writebuf(itoa(hex, _global_buf, 16));
}

void printintln(uint32_t hex) {
	tty_writebuf(itoa(hex, _global_buf, 16));
    tty_write("\n");
}

void printint_dec(uint32_t dec) {
	tty_writebuf(itoa(dec, _global_buf, 10));
}

void tty_write_hex_sandwich(char* s1, int num, char* s2) {
	tty_writebuf(s1);
    tty_writebuf("0x");
    printint(num);
    tty_writebuf(s2);
}

void tty_write_hex32_sandwich(char* s1, int num, char* s2) {
	tty_writebuf(s1);
    tty_writebuf("0x");

    itoa(num, _global_buf, 16);
    size_t fhexlen = strlen(_global_buf);
    char hex[9];

    for(int i = 0; i < 8; i++) {
        if(i < (8-fhexlen)) {
            hex[i] = '0';
        } else {
            hex[i] = _global_buf[i-(8-fhexlen)];
        }
    }
    hex[8] = '\0';
    tty_writebuf(hex);
    tty_writebuf(s2);
}

void tty_write_hex16_sandwich(char* s1, uint16_t num, char* s2) {
	tty_writebuf(s1);
    tty_writebuf("0x");

    itoa(num, _global_buf, 16);
    size_t fhexlen = strlen(_global_buf);
    char hex[5];

    for(int i = 0; i < 4; i++) {
        if(i < (4-fhexlen)) {
            hex[i] = '0';
        } else {
            hex[i] = _global_buf[i-(4-fhexlen)];
        }
    }
    hex[4] = '\0';
    tty_writebuf(hex);
    tty_writebuf(s2);
}

void tty_write_hex8_sandwich(char* s1, uint8_t num, char* s2) {
	tty_writebuf(s1);
    tty_writebuf("0x");

    itoa(num, _global_buf, 16);
    size_t fhexlen = strlen(_global_buf);
    char hex[3];

    for(int i = 0; i < 2; i++) {
        if(i < (2-fhexlen)) {
            hex[i] = '0';
        } else {
            hex[i] = _global_buf[i-(2-fhexlen)];
        }
    }
    hex[2] = '\0';
    tty_writebuf(hex);
    tty_writebuf(s2);
}

#endif