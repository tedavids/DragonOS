// our implementation of string length

#include <string.h>

size_t strlen(const char* str) {
    size_t len = 0;
    // go until we hit the null at the end of the string
    while (str[len])
        len++;

    return len;
} // size_t strlen(const char* str)