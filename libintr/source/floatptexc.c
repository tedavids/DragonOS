// floatptexc.c

// this is an interupt handler FPU error
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>



void floatptexc(struct interupt_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("SIMD Floating-point exception", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);

    printf("Halting\r\n");

    // handle the fault fallout
    kernelabort(IDT_FLT_PT_EXCEPTION, frame->codesegment, frame->erroraddress, frame->errorflags, 0);

} // floatptexc