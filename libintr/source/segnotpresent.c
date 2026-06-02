// segnotpresent.c

// this is an interupt handler for a segment not present
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>


void segnotpresent(struct interupt_error_frame_t *frame) {
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Segment not present", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);
 
    intPrintErrorCode(frame->errorcode);
    printf("Error code is the segment selector index\n\r");
    
    if (cpl==0x3) intPrintContextSwitch(frame->userstacksegment, frame->userestackptr);

    // handle the fault fallout
    kernelabort(IDT_SEG_NOT_PRESENT, frame->codesegment, frame->erroraddress, frame->errorflags, frame->errorcode);

}