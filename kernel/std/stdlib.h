#ifndef _STDLIB_H_
#define _STDLIB_H_
#include <string.h>
#include <stddef.h>

int abs(int i) {
  return i < 0 ? -i : i;
}

/*char* itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;
    // Handle 0 explicitely, otherwise empty string is printed for 0
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
    str[i] = '\0'; // Append string terminator
    // Reverse the string
    strrev((unsigned char*) str); // i);
    return str;
}*/
// Function to swap two numbers
/*static void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Function to reverse `buffer[i…j]`
static char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}
 
// Iterative function to implement `itoa()` function in C
//TODO: make it work
char* itoa(int value, char* buffer, int base) {
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i+1] = '\0'; // null terminate string
 
    // reverse the string and return it
	
    buffer = reverse(buffer, 0, i - 1);
	buffer[strlen(buffer)] = '\0';
	return buffer;
}*/

//works for 0-f
char digit_to_ascii(uint8_t digit) {
	return digit < 10 ? (48 + digit):(97 + (digit - 10));
}

char* itoa(uint32_t value, char* buffer, int base) {
	uint8_t digit = 0;
	int j = 0;
	do {
		digit = value % base;
		value -= digit;
		value /= base;
		buffer[j] = digit_to_ascii(digit); j++;
	} while(value > 0);
	buffer[j] = '\0';
	strrev((unsigned char*) buffer);
	return buffer;
}
#endif