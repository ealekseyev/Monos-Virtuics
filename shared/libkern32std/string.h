#ifndef _STRING_H_
#define _STRING_H_
#include <stddef.h>
#include <memcpy.c>

//TODO: figure out register not usable shenanigans
size_t strlen(const char* str) {
	//register const char *s;
	const char *s;
	for (s = str; *s; ++s);
	return (s - str);
}
/* Copy SRC to DEST.  */
char* strcpy(char* dest, const char* src) {
  return memcpy(dest, src, strlen (src) + 1);
}

char* strcat(char* dest, const char* src) {
  strcpy(dest + strlen(dest), src);
  return dest;
}

void strrev(unsigned char *str) {
	int i;
	int j;
	unsigned char a;
	unsigned len = strlen((const char *)str);
	for (i = 0, j = len - 1; i < j; i++, j--) {
		a = str[i];
		str[i] = str[j];
		str[j] = a;
	}
}

#endif