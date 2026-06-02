// idt.h  structures used by the interrupt descriptor table

#ifndef _IDT_H
#define _IDT_H

#include <apic.h>
#include <stdint.h>
#include <stdio.h>




#define IDT_TSS_NOT_USED            0
#define IDT_PRESENT                 0b10000000
#define IDT_PRIVLEDGE_KERNEL        0b00000000
#define IDT_PRIVLEDGE_USER          0b01100000
#define IDT_GATE_TYPE_TASK          0b00000101
#define IDT_GATE_TYPE_INTERrUPT16   0b00000110
#define IDT_GATE_TYPE_TRAP16        0b00000111
#define IDT_GATE_TYPE_INTERRUPT32   0b00001110
#define IDT_GATE_TYPE_TRAP32        0b00001111
#define IDT_INTERRUPT_GATE32        (IDT_PRESENT & IDT_GATE_TYPE_INTERRUPT32)

#define IDT_DIVIDE_BY_ZERO          0
#define IDT_NON_MASKABLE_INTERRUPT  2
#define IDT_BREAKPOINT              3
#define IDT_INDEX_OUT_OF_RANGE      5
#define IDT_INVALID_OPCODE          6
#define IDT_DEVICE_NOT_AVAIL        7
#define IDT_DOUBLE_FAULT            8
#define IDT_COPROC_OVERRUN          9
#define IDT_INVALID_TSS             10
#define IDT_SEG_NOT_PRESENT         11
#define IDT_STACK_SEG_FAULT         12
#define IDT_GENERAL_PROTECTION_FLT  13
#define IDT_PAGE_FAULT              14
#define IDT_FPU_ERROR               16
#define IDT_ALIGNMENT_CHECK         17
#define IDT_MACHINE_CHECK           18
#define IDT_FLT_PT_EXCEPTION        19
#define IDT_VIRT_EXCEPTION          20
#define IDT_CTRL_PROTECT_EXCEPT     21

// apic timers here
#define IDT_APIC_TIMER          0x20
#define IDT_KEYBORD_INTERRUPT   0x21




struct __attribute__((packed)) idt_entry {
    uint16_t handlerlow; // offset bits 0..15                   1
    uint16_t selector; // a code segment selector in GDT        2 
    uint8_t reservedzero; // set to zero                        3
    uint8_t typeattr; // gate type, dpl, and p fields           4
    uint16_t handlerhigh; // offset bits 16..31                 5
};

struct __attribute__((packed)) idt_ptr {
    uint16_t limit;
    uint32_t base;
};

// define externals here
extern void int0_handler();
extern void int2_handler();
extern void int3_handler();
extern void int5_handler();
extern void int6_handler();
extern void int7_handler();
extern void int8_handler();
extern void int9_handler();
extern void int10_handler();
extern void int11_handler();
extern void int12_handler();
extern void int13_handler();
extern void int14_handler();
extern void int16_handler();
extern void int17_handler();
extern void int18_handler();
extern void int19_handler();
extern void int20_handler();
extern void int21_handler();
extern struct idt_entry         IDT[256];
extern struct idt_ptr           IDTP;

extern void EnableInterrupts();
extern void DisableInterrupts();

extern void setIDT();

// set the descriptor
// supress allows interrupts to be supressed when the gate is set up.  
// Normally this is true, except during kernel init
extern bool idt_set_gate(int idtentry, uint32_t handler, uint16_t selector, uint8_t privlegelevel, uint8_t type, bool supress);

// load initial handlers
extern bool loadIDT();

#endif
