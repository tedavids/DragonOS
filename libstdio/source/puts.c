// our putc function for stdio

#include <stdio.h>

int puts(const char* string) {
    return printf("%s\n",string);
} // int puts(const char* string)