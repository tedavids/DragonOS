// ctlprotect.c

// this is an interupt handler for a control protection exception

#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>



void ctlprotexc(struct interupt_error_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Control protection exception", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);
 
    intPrintErrorCode(frame->errorcode);
    switch (frame->errorcode) {
        case 1: printf("Near return check failure\n\r");
                break;
        case 2: printf("Far return or IRET check failure\n\r");
                break;
        case 3: printf("Missing ENDBRACH instruction at the target of an indirect call or jmp\n\r");
                break;
        case 4: printf("RESTORSSP token check failure (shadow stack failure)\n\r");
                break;
        case 5: printf("SETSSBSY token check failure\n\r");
                break;
        default:
                printf("Reason unkown\n\r");
                break;
    }
    
    if (cpl==0x3) intPrintContextSwitch(frame->userstacksegment, frame->userestackptr);

    // handle the fault fallout
    kernelabort(IDT_CTRL_PROTECT_EXCEPT, frame->codesegment, frame->erroraddress, frame->errorflags, frame->errorcode);
    

} // virtexcp