// kernelabort.c  handle when the kernel has to die
#include <gdt.h>
#include <kernel.h>

void kernelabort(int interrupt, uint32_t segment, uint32_t address, uint32_t flags, uint32_t error) {

    // possibly handler user errors here
    if (segment == USER_CODE_SELECTOR) {
        interrupt += 0;
        address += 0;
        flags += 0;
        error += 0;
    }
    // halt code
    asm("loop:\n"
        "cli\n"
        "hlt\n"
        "jmp loop\n"
    );
    
}