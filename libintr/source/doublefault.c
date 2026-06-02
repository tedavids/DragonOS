// doublefault.c

// this is an interupt handler for a doublefault
// (a fault withing an interupt)
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>


void doublefault(struct interupt_error_frame_t *frame) {
  
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Double Fault", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);
 
    intPrintErrorCode(frame->errorcode);
    
    if (cpl==0x3) intPrintContextSwitch(frame->userstacksegment, frame->userestackptr);

    printf("Halting\r\n");

    // handle the fault fallout
    kernelabort(IDT_DOUBLE_FAULT, frame->codesegment, frame->erroraddress, frame->errorflags, frame->errorcode);

}