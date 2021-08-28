#ifndef _VGA_H_
#define _VGA_H_

#include <stddef.h>
#include <memset.c>
#include <string.h>

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

void writebuf(char* str, int x, int y) {
    char* vidmem_offset = vidmem + ((y*Ylen) + x)*2;
    size_t buflen = strlen(str);
    unsigned int j = 0;
    while(str[j] != '\0') {
        *(vidmem_offset+(j*2)) = str[j];
        *(vidmem_offset+(j*2)+1) = global_color;
        j++;
    }
}

void dumpstr(char* str) {
    writebuf(str, 0, 0);
}
void clearscreen() {
    unsigned int j = 0;
    while(j < Xlen * Ylen * 2) {
		vidmem[j] = ' ';
		vidmem[j+1] = global_color; 		
		j += 2;
	}
}
void setcolor(unsigned char bg, unsigned char fg) {
    global_color = bg << 4 + fg;
}
void getcolor(unsigned char bg, unsigned char fg) {
    return bg << 4 + fg;
}
#endif