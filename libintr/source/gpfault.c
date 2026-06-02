// gpfault.c

// this is an interupt handler for a general protection fault
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>

#define EXTERNAL_EVENT      0x1
#define IDT_ERROR           0x2
#define LDT_REFERENCE       0x4

void gpfault(struct interupt_error_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("General protection fault",frame->codesegment, frame->erroraddress, frame->errorflags, cpl);

    intPrintErrorCode(frame->errorcode);
    uint32_t index = (frame->errorcode >> 3);

    if ((frame->errorcode & EXTERNAL_EVENT)) printf("Caused by an external event\n\r");
    if ((frame->errorcode & IDT_ERROR)) 
        printf("IDT Error index: %ul\r\n",index);
    else {
        // ldt/gdt error
        if ((frame->errorcode & LDT_REFERENCE))
            printf("LDT error index: %ul\n\r",index);
        else 
            printf("GDT error index: %ul", index);
    }

    if (cpl==0x3) intPrintContextSwitch(frame->userstacksegment, frame->userestackptr);

    printf("Halting\r\n");

    // handle the fault fallout
    kernelabort(IDT_GENERAL_PROTECTION_FLT, frame->codesegment, frame->erroraddress, frame->errorflags, frame->errorcode);

}