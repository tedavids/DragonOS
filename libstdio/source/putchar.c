// our putchar function

#include "stdio.h" 

#if defined (__is_libk) 
#include "../../include/kernel/tty.h"
#endif

int putchar(int ic) {
#if defined(__is_libstdio) 
    char c = (char) ic;
    terminal_write(&c, sizeof(c));
#else
    // todo implement stdio and write system call
#endif

    return ic;
} // int putchar(int ic)