// faulthandler.c  handle faults needs to properly handle user faults
//                 for non user faults the proper thing to do is die
#include <gdt.h>
#include <kernel.h>

void faulthandler(int interrupt, uint32_t segment, uint32_t address, uint32_t flags, uint32_t error) {

    if (segment == USER_CODE_SELECTOR) {

        // TODO:  handler user divide by zero
        interrupt += 0;
        address += 0;
        flags += 0;
        error += 0;
    }

    // if we got here we need to halt the kernel
    // especially if it is not in the KERNEL_CODE_SEGMENT

    // halt code
    asm("loop:\n"
        "cli\n"
        "hlt\n"
        "jmp loop\n"
    );
    
}