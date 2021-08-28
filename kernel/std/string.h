#ifndef _STRING_H_
#define _STRING_H_
#include <stddef.h>

size_t strlen(const char* str) {
	register const char *s;
	for (s = str; *s; ++s);
	return (s - str);
}

#endif