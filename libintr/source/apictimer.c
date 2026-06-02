// apictimer.c -- only job is to count tics
#include <stdint.h>
#include <stdio.h>
#include <interupt.h>
#include <idt.h>
#include <gdt.h>
#include <kernel.h>

#include <apic.h>

volatile uint64_t ticks = 0;


void apictimer() {
    ticks++;

    putPortByte(APIC_PIC1_COMMAND_PORT, APIC_END_OF_INTERRUPT);
}

bool initAPICtimer() {
    // add idt entry to table

    if (!idt_set_gate(IDT_APIC_TIMER, (uint32_t) &int32_handler,  KERNEL_CODE_SELECTOR, 
                 IDT_PRIVLEDGE_KERNEL, IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to set APIC timer interrupt\r\n");
        return false;
    }

    // unmask the interrupt
    // PIC1 starts at interrupt 20, the keyboard is interrupt 21
    putPortByte(APIC_PIC1_DATA_PORT, 0xFE); // 0b1111 1110 the zero is the second port (21)

    return true;
}
