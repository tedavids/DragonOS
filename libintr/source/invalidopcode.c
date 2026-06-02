// invalidopcode.c

// this is an interupt handler for divide by zero
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>



void invalidopcode(struct interupt_frame_t *frame) {
    
uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Invalid opcode", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);

    printf("Halting\r\n");

    // handle the fault fallout
    kernelabort(IDT_INVALID_OPCODE, frame->codesegment, frame->erroraddress, frame->errorflags, 0); 

} // dividebyzero