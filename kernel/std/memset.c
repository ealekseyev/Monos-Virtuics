#ifndef _MEMSET_C_
#define _MEMSET_C_

#include <stddef.h>
void* memset (void *dest, int val, size_t len) {
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}

#endif