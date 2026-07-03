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

// the ticks of the clock
extern volatile uint64_t ticks;

// handler for interrut 32 (0x20)  APIC timer
extern void int32_handler(); 

/// Initialize the APIC processor
/// Returns:    true if initialization is successfull
extern bool initAPIC();
// get the number of ticks 
// returns: The number of tics that have happend since the timer was set up
extern uint64_t getTicks();
// Initialize the timer
// returns:     true if initialization is ok
extern bool initAPICtimer();
// clear all APIC interrupts, this sends end of interrupt to everything
extern void clearAPICInterrupts();

#endif