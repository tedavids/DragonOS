// invalidTSS.c

// this is an interupt handler for a general protection fault
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>


void invalidTSS(struct interupt_error_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Invalid TSS", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);
 
    intPrintErrorCode(frame->errorcode);
    uint8_t rpl = (uint8_t)(frame->errorcode & 0xFFFC);
    printf("Requestor privledge levle: %ub\n\r", rpl);
    if ((frame->errorcode)) {
        printf("LDT selector %ul\r\n", (frame->errorcode >> 3));
    } else {
        printf("GDT selector %ul\r\n", (frame->errorcode >> 3));
    }
    if (cpl==0x3) intPrintContextSwitch(frame->userstacksegment, frame->userestackptr);

    // handle the fault fallout
    kernelabort(IDT_INVALID_TSS, frame->codesegment, frame->erroraddress, frame->errorflags, frame->errorcode);

}