// pagefault.c

// this is an interupt handler for a general protection fault
#include <stdint.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>

#define PAGE_FAULT_PRESESNT 0b1
#define PAGE_FAULT_WRITE    0b10
#define PAGE_FAULT_RESERVED 0b100
#define PAGE_FAULT_INSTRUCT 0b10000
#define PAGE_FAULT_SHADOW_S 0b1000000


void pagefault(struct interupt_error_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("Page fault",frame->codesegment, frame->erroraddress, frame->errorflags, cpl);

    intPrintErrorCode(frame->errorcode);
    if (frame->errorflags & PAGE_FAULT_PRESESNT) {
        printf("Page-Protection Violation (write to read only page)\n\r");
    } else {
        printf("Page not present\n\r");
    }
    if (frame->errorflags & PAGE_FAULT_WRITE) {
        printf("Write violation\n\r");
    } else{
        printf("Read violation\n\r");
    }
    if (frame->errorflags & PAGE_FAULT_RESERVED) {
        printf("Problem with PDE or PTE\n\r");
    }
    if (frame->errorflags & PAGE_FAULT_INSTRUCT) {
        printf("Instruction fetch\n\r");
    }
    if (frame->errorflags & PAGE_FAULT_SHADOW_S) {
        printf("Shadow Stack issue\n\r");
    }
    if (frame->errorflags) printf("Page is reserved\n\r");

    printf("Not all reasons may be listed, check error code. Halting\r\n");

    if (cpl==0x3) intPrintContextSwitch(frame->userstacksegment, frame->userestackptr);

    printf("Halting\r\n");

    // handle the fault fallout
    kernelabort(IDT_GENERAL_PROTECTION_FLT, frame->codesegment, frame->erroraddress, frame->errorflags, frame->errorcode);

}