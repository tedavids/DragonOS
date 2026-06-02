// interrupt descriptor table (IDT) functions

#include <stdlib.h>
#include <string.h>     // for memory functions
#include <gdt.h>
#include <idt.h>

// enable and disable interrupts
inline void EnableInterrupts() {
    asm volatile("sti\n\t");
}

inline void DisableInterrupts() {
    asm volatile("cli\n\t");
}

// set the descriptor

// supress is do we supress interrupts, it is used when the interrupts are first set up (loadIDT)
bool idt_set_gate(int idtentry, uint32_t handler, uint16_t selector, uint8_t privlegelevel, uint8_t type, bool supress) {

    // supress interrupts while we change this
    if (supress) asm volatile("cli\n\t");

    IDT[idtentry].handlerlow = (uint16_t) handler & 0xFFFF;
    IDT[idtentry].handlerhigh = (uint16_t) (handler >> 16) & 0xFFFF;
    IDT[idtentry].selector = selector;
    IDT[idtentry].reservedzero = 0;
    IDT[idtentry].typeattr = IDT_PRESENT | privlegelevel | type;
    // turn interrupts back on
    if (supress) asm volatile("sti\n\t");

    return true;
} // bool idt_set_gate(int n, uint32_t handler, uint16_t selector, uint8_t attr)

// load the IDT
bool loadIDT() {
    
    bool rtncde = true;

    // clear the IDT 
    memset(&IDT,sizeof(IDT),0);

    // divide by zero

    if (!idt_set_gate(IDT_DIVIDE_BY_ZERO, (uint32_t) &int0_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load divide by zero handler");
        rtncde = false;
    }

    // non maskable interrupt
    if (!idt_set_gate(IDT_NON_MASKABLE_INTERRUPT, (uint32_t) &int2_handler, USER_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load divide by zero handler");
        rtncde = false;
    }

    // non maskable interrupt
    if (!idt_set_gate(IDT_BREAKPOINT, (uint32_t) &int3_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_USER, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load divide by zero handler");
        rtncde = false;
    }

    // index out of range

    if (!idt_set_gate(IDT_INDEX_OUT_OF_RANGE, (uint32_t) &int5_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load index out of range handler");
        rtncde = false;
    }

    // invalid opcode

    if (!idt_set_gate(IDT_INVALID_OPCODE, (uint32_t) &int6_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load Invalid opcode handler");
        rtncde = false;
    }

        // device not available

    if (!idt_set_gate(IDT_DEVICE_NOT_AVAIL, (uint32_t) &int7_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load Device not available handler");
        rtncde = false;
    }
    // double fault

    if (!idt_set_gate(IDT_DOUBLE_FAULT, (uint32_t) &int8_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load Double fault Fault");
        rtncde = false;
    }

        // coprocessor segment overrun

    if (!idt_set_gate(IDT_COPROC_OVERRUN, (uint32_t) &int9_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load Coprocessor overrun Fault");
        rtncde = false;
    }

     // invalidTSS fault
    
    if (!idt_set_gate(IDT_INVALID_TSS, (uint32_t) &int10_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load Invalid TSS Fault");
        rtncde = false;
    }

        // segment not present fault
    
    if (!idt_set_gate(IDT_SEG_NOT_PRESENT, (uint32_t) &int11_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load Segment not present Fault");
        rtncde = false;
    }

        // stack segment fault
    
    if (!idt_set_gate(IDT_STACK_SEG_FAULT, (uint32_t) &int12_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load Stack Segment Fault");
        rtncde = false;
    }
    // general protection fault

    if (!idt_set_gate(IDT_GENERAL_PROTECTION_FLT, (uint32_t) &int13_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load General Protection Fault");
        rtncde = false;
    }

       // Page fault

    if (!idt_set_gate(IDT_PAGE_FAULT, (uint32_t) &int14_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load General Protection Fault");
        rtncde = false;
    }

    // FPUError

    if (!idt_set_gate(IDT_FPU_ERROR, (uint32_t) &int16_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load FPU Error");
        rtncde = false;
    }

    // Alignment check

    if (!idt_set_gate(IDT_ALIGNMENT_CHECK, (uint32_t) &int17_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load FPU Error");
        rtncde = false;
    }

    // machine check

    if (!idt_set_gate(IDT_MACHINE_CHECK, (uint32_t) &int18_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to load Machine Check");
        rtncde = false;
    }

    // SIMD Floating Point exceptinon

    if (!idt_set_gate(IDT_FLT_PT_EXCEPTION, (uint32_t) &int19_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to Floating point exception");
        rtncde = false;
    }

    // Virtualization Exception

    if (!idt_set_gate(IDT_VIRT_EXCEPTION, (uint32_t) &int20_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to Virtualization exception");
        rtncde = false;
    }

    // Control point exception 

    if (!idt_set_gate(IDT_CTRL_PROTECT_EXCEPT, (uint32_t) &int21_handler, KERNEL_CODE_SELECTOR, IDT_PRIVLEDGE_KERNEL, 
                      IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to Virtualization exception");
        rtncde = false;
    }

    initAPIC();

    setIDT();

    
    return rtncde;
}