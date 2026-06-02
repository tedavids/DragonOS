// stacksegflt.c

// this is an interupt handler for a general protection fault
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>


void stacksegflt(struct interupt_error_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Stack segment fault", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);
 
    intPrintErrorCode(frame->errorcode);
    if (frame->errorcode) {
        printf("Error code is the stack segment selector\n\r");
    } else {
        printf("Stack overflow\n\r");
    }
    if (cpl==0x3) intPrintContextSwitch(frame->userstacksegment, frame->userestackptr);

    // handle the fault fallout
    kernelabort(IDT_STACK_SEG_FAULT, frame->codesegment, frame->erroraddress, frame->errorflags, frame->errorcode);

}