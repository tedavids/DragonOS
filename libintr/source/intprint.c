// print interrupt information

#include <stdio.h>
#include <interupt.h>

extern uint32_t stack_bottom;
void intPrintGeneralInfo(const char* intName, uint32_t segment, uint32_t address, uint32_t flags, uint8_t cpl) {
    printf("\r\n%s\n\r", intName);
    printf("\r\nAddress: 0x%Xl\r\n", address);
    printf("Code Segment: %xl, Privledge level: %ib\n\r", segment, cpl);

    // check for stack overflow
    if (stack_bottom) printf("****Stack Overflow Detected****\n\r");

    // print flags and meaning
    printf("Flags: 0x%xl\n\r", flags);
    
    if ((flags & INT_CARRY_FLAG)) printf("CF");
    if ((flags & INT_PARITY_FLAG)) printf("PF ");
    if ((flags & INT_AUX_CARRY_FLAG)) printf("(AF) ");
    if ((flags & INT_ZERO_FLAG)) printf("ZF ");
    if ((flags & INT_SIGN_FLAG)) printf("SF ");
    if ((flags & INT_TRAP_FLAG)) printf("TF ");
    if ((flags & INT_ENABLE_FLAG)) printf("IF ");
    if ((flags & INT_DIRECTION_FLAG)) printf("DF ");
    if ((flags & INT_OVERFLOW_FLAG)) printf("OF ");
    if ((flags & INT_NESTED_TASK_FLAG)) printf("NT ");
    if ((flags & INT_RESUME_FLAG)) printf("RF ");
    if ((flags & INT_VIRTUAL_MODE)) printf("VM ");
    if ((flags & INT_ALIGNMENT_CHK)) printf("AC ");
    if ((flags & INT_VIRT_INTERUPT)) printf("VIF");
    if ((flags & INT_VIRT_INT_PEND)) printf("VIP");
    if ((flags & INT_HAS_CUPID)) printf("ID");
    printf("\r\n");

    uint8_t iopl = (flags & INT_IOPL_MASK) >> 12;
    printf("I/O Privledge Level 0x%xb\n\r",iopl);
    
    return;

}

void intPrintContextSwitch(uint32_t stackseg, uint32_t stackptr) {
    printf("\n\rContext switch detected.\n\r");
    printf("Old Stack Segment: %xl, Old Stack Pointer: %xl\n\r", stackseg, stackptr);
}

void intPrintErrorCode(uint32_t code) {
    printf("\n\rError Code: 0x%xl\n\r", code);
}