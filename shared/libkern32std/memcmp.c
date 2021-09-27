#ifndef __MEMCMP_C_
#define __MEMCMP_C_
#include <stddef.h>

int memcmp(void* str1, void* str2, size_t count) {
  register const uint8_t* s1 = (const uint8_t*)str1;
  register const uint8_t* s2 = (const uint8_t*)str2;
  while (count-- > 0)
    {
      if (*s1++ != *s2++)
          return s1[-1] < s2[-1] ? -1 : 1;
    }
  return 0;
}
#endif