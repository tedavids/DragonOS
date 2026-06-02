// APIC interrupt chip

#ifndef _APIC_H
#define _APIC_H

#define APIC_PIC1_COMMAND_PORT      0x20
#define APIC_PIC1_DATA_PORT         0x21
#define APIC_PIC2_COMMAND_PORT      0xA0
#define APIC_PIC2_DATA_PORT         0xA1

#define APIC_INIT                   0x11
#define APIC_MASK_ALL_PORTS         0xFF
#define APIC_END_OF_INTERRUPT       0x20

extern volatile uint64_t ticks;

extern void int32_handler(); 

extern bool initAPIC();
extern uint64_t getTicks();
extern bool initAPICtimer();
extern void clearAPICInterrupts();

#endif