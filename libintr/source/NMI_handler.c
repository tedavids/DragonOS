// NMI_handler.c

// this is an interupt handler for divide by zero
#include <stdlib.h>
#include <stdio.h>
#include <idt.h>
#include <kernel.h>
#include <interupt.h>

#define PARITY_CHECK  0b10000000
#define CHANNEL_CHECK 0b01000000


void NMI_handler(struct interupt_frame_t *frame) {
    
    uint8_t cpl = frame->codesegment & 0x3; // lower 3 bits of the segment are the CPL
    intPrintGeneralInfo("x87 FPU Error", frame->codesegment, frame->erroraddress, frame->errorflags, cpl);

    unsigned char port92data = getPortByte(0x92);
    unsigned char port61data = getPortByte(0x61);

    printf("Port 92: %Xl, Port 61: %xl\n\4", port92data, port61data);

    if (port61data & PARITY_CHECK) {
        printf("Memory Error, aborting\r\n");
        abort();
    }

    if (port61data & CHANNEL_CHECK) {
        printf("Bus Failure, aborting\n\r");
        abort();
    }
    // handle the fault fallout
    kernelabort(IDT_NON_MASKABLE_INTERRUPT, frame->codesegment, frame->erroraddress, frame->errorflags, 0);

} // dividebyzero