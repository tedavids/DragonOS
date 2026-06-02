// interrupt.h  basic stuff for interrupts
#ifndef _INTERUPT_H
#define _INTERUPT_H

#include <stdint.h>

#define INT_CARRY_FLAG      1
#define INT_PARITY_FLAG     2
#define INT_AUX_CARRY_FLAG  0x10
#define INT_ZERO_FLAG       0x40
#define INT_SIGN_FLAG       0x80
#define INT_TRAP_FLAG       0x100
#define INT_ENABLE_FLAG     0x200
#define INT_DIRECTION_FLAG  0x400
#define INT_OVERFLOW_FLAG   0x800
#define INT_IOPL_MASK       0x3000
#define INT_NESTED_TASK_FLAG 0x4000
#define INT_RESUME_FLAG     0x10000
#define INT_VIRTUAL_MODE    0x20000
#define INT_ALIGNMENT_CHK   0x40000
#define INT_VIRT_INTERUPT   0x80000
#define INT_VIRT_INT_PEND   0x100000
#define INT_HAS_CUPID       0x200000

// basic interrupt frame
struct interupt_frame_t {
    // we don't care about these from pusha in the caller
    uint32_t    edi;
    uint32_t    esi;
    uint32_t    ebp;
    uint32_t    esp;
    uint32_t    ebx;
    uint32_t    edx;
    uint32_t    ecx;
    uint32_t    eax;
    // the entries we care about
    uint32_t    erroraddress;
    uint32_t    codesegment;
    uint32_t    errorflags;
};

// basic interrupt frame
struct interupt_error_frame_t {
    // we don't care about these from pusha in the caller
    uint32_t    edi;
    uint32_t    esi;
    uint32_t    ebp;
    uint32_t    esp;
    uint32_t    ebx;
    uint32_t    edx;
    uint32_t    ecx;
    uint32_t    eax;
    // the entries we care about
    uint32_t    errorcode;
    uint32_t    erroraddress;
    uint32_t    codesegment;
    uint32_t    errorflags;


    
    // only if context switch
    uint32_t    userestackptr;
    uint32_t    userstacksegment;
};

// handle how a fault returns or not
extern void faulthandler(int interrupt, uint32_t segment, uint32_t address, uint32_t flags, uint32_t error);
// handle killing the system
extern void kernelabort(int interrupt, uint32_t segment, uint32_t address, uint32_t flags, uint32_t error);

// printing functions

// print general information 

// inputs:  char*   The name of the interrupt
//          segment The segment with the issue
//          address The address the error happend at
//          flags   The error flags
//          cpl     The priority of code

extern void intPrintGeneralInfo(const char* intName, uint32_t segment, uint32_t address, uint32_t flags, uint8_t cpl);

// print the error code line, note each function needs to take it apart for what it means

// inputs:  code    The error code to print

extern void intPrintErrorCode(uint32_t code);

// print the context switch information

// inputs:  stackset    The prior stack segment
//          stackptr    The prior stack pointer
extern void intPrintContextSwitch(uint32_t stackseg, uint32_t stackptr);


#endif