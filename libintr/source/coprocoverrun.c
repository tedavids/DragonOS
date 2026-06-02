// coprocoverrun.c

// this is an interupt handler for divice not available
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>



void coprocoverrun(struct interupt_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Coprocessor segment overrun", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);

    printf("Halting\r\n");

    // handle the fault fallout
    kernelabort(IDT_COPROC_OVERRUN, frame->codesegment, frame->erroraddress, frame->errorflags, 0);

} // dividebyzero