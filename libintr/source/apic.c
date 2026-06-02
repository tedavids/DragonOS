// apic.c PIC code

#include <kernel.h>

#include <apic.h>


#define APIC_END_OF_INTERRUPT_OCW2  0x30
#define APIC_READ_IRR               0x0A
#define APIC_READ_ISR               0x0B

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

void disable8259PIC() {
    // Mask all interrupts on both PIC chips
    putPortByte(PIC1_DATA, 0xFF); 
    putPortByte(PIC2_DATA, 0xFF);
}

// initialize the APIC
bool initAPIC() {
    putPortByte(APIC_PIC1_COMMAND_PORT, APIC_INIT);
    putPortByte(APIC_PIC2_COMMAND_PORT, APIC_INIT); 
    
 
    // ICW2
    // set base vector this is where the pic interrupts start
    // 0x20 is the first unused IDT entry
    putPortByte(APIC_PIC1_DATA_PORT, 0x20);
    // 0x28 is the first ports after the ones APIC_PIC1 uses
    putPortByte(APIC_PIC2_DATA_PORT, 0x28);

    // ICW3
    // mask all interrupts -- the drivers will enable them
    putPortByte(APIC_PIC1_DATA_PORT, APIC_MASK_ALL_PORTS);

    // ICW4 set to 8086/8088 compatibility mode
    putPortByte(APIC_PIC1_DATA_PORT, 0x1);
    putPortByte(APIC_PIC2_DATA_PORT, 0x1);

    //disable8259PIC();
    return true;
}



void clearAPICInterrupts() {
    putPortByte(APIC_PIC1_COMMAND_PORT, APIC_END_OF_INTERRUPT);
    putPortByte(APIC_PIC2_COMMAND_PORT, APIC_END_OF_INTERRUPT); 
}

uint64_t getTicks() {
    return ticks;
}