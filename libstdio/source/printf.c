// our printf command

#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) 
{
    const unsigned char* bytes = (const unsigned char*) data;
    
    for (size_t i = 0; i < length; i++) 
        if (putchar(bytes[i]) == EOF)
            return false;

    return true;
} // static bool print(const char* data, size_t length)

int printf(const char* restrict format, ...) 
{
    va_list parameters;
    va_start(parameters, format);

    char buffer[256] = "";

    snprintfint(buffer, sizeof(buffer), format, &parameters);
    size_t written =  strlen(buffer);
    print(buffer, written);

    return (int) written;

} // int printf(const char* restrict format, ...)