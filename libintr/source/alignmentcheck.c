// alignmentcheck.c

// this is an interupt handler for a general protection fault
#include <stdint.h>
#include <stdio.h>
#include <kernel.h>
#include <idt.h>
#include <interupt.h>


void alignmentcheck(struct interupt_error_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Alignment check", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);
 
    intPrintErrorCode(frame->errorcode);
    printf("No further information on error\n\r");

    if (cpl==0x3) intPrintContextSwitch(frame->userstacksegment, frame->userestackptr);

    // handle the fault fallout
    kernelabort(IDT_ALIGNMENT_CHECK, frame->codesegment, frame->erroraddress, frame->errorflags, frame->errorcode);

}