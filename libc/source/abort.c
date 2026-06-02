// our abort command
#include <stdlib.h>
#include <stdio.h>

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
    // TODO: add proper kernel panic
    printf("Kernel: panic: abort()\n");
    asm volatile("hlt");
#else
    //todo: abnormally terminate the process as if by SIGABRT
    printf("abort()\n");
#endif

    // hang the machine
    while (true) 
        __builtin_unreachable();

} // void abort(void)