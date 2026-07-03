// register retrieval and printing

#include <stdio.h>

#include <registers.h>

// get registers

uint32_t    getRegisterESI() {

    uint32_t value;
    __asm__ volatile ("movl %%ESI, %0" : "=r" (value) : :);

    return value;
}

uint32_t    getRegisterEDI() {
    uint32_t value;
    __asm__ volatile ("movl %%EDI, %0" : "=r" (value) : :);

    return value;
}

uint32_t    getRegisterESP() {
    uint32_t value;
    __asm__ volatile ("movl %%ESP, %0" : "=r" (value) : :);

    return value;
}

uint32_t    getRegisterEBP() {
    uint32_t value;
    __asm__ volatile ("movl %%EBP, %0" : "=r" (value) : :);

    return value;
}

uint32_t    getRegisterEFLAGS() {
    uint32_t value;
    __asm__ volatile ("pushfl\n\t"
                      "popl %0" : "=g" (value) : : "memory"
                    );

    return value;
}

uint32_t    getRegisterCR0() {
   uint32_t value;
    __asm__ volatile ("movl %%CR0, %0" : "=r" (value) : :);

    return value;    
}

uint32_t    getRegisterCR2() {
   uint32_t value;
    __asm__ volatile ("movl %%CR2, %0" : "=r" (value) : :);

    return value;    
}

uint32_t    getRegisterCR3() {
   uint32_t value;
    __asm__ volatile ("movl %%CR3, %0" : "=r" (value) : :);

    return value;    
}

uint32_t    getRegisterCR4() {
   uint32_t value;
    __asm__ volatile ("movl %%CR4, %0" : "=r" (value) : :);

    return value;    
}


// print register functions
void printRegisterESI() {
    uint32_t esi = getRegisterESI();
    printf("ESI: 0x%xl", esi);
}

void printRegisterEDI() {
    uint32_t edi = getRegisterEDI();
    printf("EDI: 0x%xl",edi);
}

void printRegisterESP() {
    uint32_t esp = getRegisterEDI();
    printf("ESP: 0x%xl",esp);
}

void printRegisterEBP() {
    uint32_t ebp = getRegisterEBP();
    printf("EBP: 0x%xl",ebp);
}

void printRegisterEFLAGS() {
    uint32_t eflags = getRegisterEFLAGS();
    printf("EFLAGS: 0x%xl ",eflags);
    // print flags
    if (eflags & EFLAGS_CARRY_FLAG) printf(" CF");
    if (eflags & EFLAGS_PARITY_FLAG) printf(" PF");
    if (eflags & EFLAGS_AUX_FLAG) printf(" AF");
    if (eflags & EFLAGS_ZERO_FLAG) printf(" ZF");
    if (eflags & EFLAGS_SIGN_FLAG) printf(" SF");
    if (eflags & EFLAGS_TRAP_FLAG) printf(" TF");
    if (eflags & EFLAGS_INT_ENABLE_FLAG) printf(" IF");
    if (eflags & EFLAGS_DIRECTION_FLAG) {
        printf(" DU");
    } else {
        printf(" DD");
    }
    if (eflags & EFLAGS_OVERFLOW_FLAG) printf(" OF");
    if (eflags & EFLAGS_NESTED_TASK_FLAG) printf(" NT");
    if (eflags & EFLAGS_RESUME_FLAG) printf(" RF");
    if (eflags & EFLAGS_VIRT_8086_MODE_FLAG) printf(" VM");
    if (eflags & EFLAGS_ALIGN_CHECK_FLAG) printf(" AC");
    if (eflags & EFLAGS_VIRT_INT_FLAG) printf(" VIF");
    if (eflags & EFLAGS_VIRT_INT_PEND_FLAG) printf(" VIP");
    if (eflags & EFLAGS_CUPID_FLAG) printf(" ID");
}

void    printRegisterCR0() {
    uint32_t cr0 = getRegisterCR0();
    printf("CR0: 0x%xl ", cr0);

    if (cr0 & CR0_PROTECTED_MODE_FLAG) printf(" PE");
    if (cr0 & CR0_MONITOR_COPROC_FLAG) printf(" MP");
    if (cr0 & CR0_X87_FPU_EMUL_FLAG) printf(" EM");
    if (cr0 & CR0_TASK_SWITCHED_FLAG) printf(" TS");
    if (cr0 & CR0_EXTENSION_TYPE_FLAG) printf(" ET");
    if (cr0 & CR0_NUMERIC_ERROR_FLAG) printf(" NE");
    if (cr0 & CR0_WRITE_PROTECT_FLAG) printf(" WP");
    if (cr0 & CR0_ALIGNMENT_MASK_FLAG) printf(" AM");
    if (cr0 & CR0_NOT_WRITE_THROUGH_FLAG) printf(" NW");
    if (cr0 & CR0_CACHE_DISABLE_FLAG) printf(" CD");
    if (cr0 & CR0_PAGING_FLAG) printf(" PG");
}

void    printRegisterCR2() {
    uint32_t cr2 = getRegisterCR2();
    printf("CR2: 0x%Xl ",cr2);
}

void    printRegisterCR3() {
    uint32_t cr3 = getRegisterCR3();
    printf("CR3: 0x%xl ",cr3);
}

void    printRegisterCR4() {
    uint32_t cr4 = getRegisterCR4();
    printf("CR4: 0x%xl ",cr4);

    if (cr4 & CR4_VIRT_86_EXTENSIONS_FLAG) printf(" VME");
    if (cr4 & CR4_PROT_MOD_VIRT_INT_FLAG) printf(" PVI");
    if (cr4 & CR4_TIME_STAMP_DISABLE_FLAG) printf(" TSD");
    if (cr4 & CR4_DEBUG_EXTEN_FLAG) printf(" DE");
    if (cr4 & CR4_PG_SIZE_EXTEN_FLAG) printf(" PSE");
    if (cr4 & CR4_PHYS_ADR_EXTEN_FLAG) printf(" PAE");
    if (cr4 & CR4_MCH_CHK_EXCEPT_FLAG) printf(" MCE");
    if (cr4 & CR4_PG_GLBL_ENABLE_FLAG) printf(" PGE");
    if (cr4 & CR4_OS_XSAVE_ENBL_FLAG) printf(" OSFXSR");
    if (cr4 & CR4_OS_UNMSK_SIMD_FP_FLAG) printf(" OSXMMEXCPT");
    if (cr4 & CR4_USR_MOD_PREV_FLAG) printf(" UMIP");
    if (cr4 & CR4_5_LVL_PAGING_FLAG) printf(" LA57");
    if (cr4 & CR4_VRT_MCH_EXT_ENBL_FLAG) printf(" VMXE");
    if (cr4 & CR4_SAFR_MODE_EXT_ENBL_FLAG) printf(" SMXE");
    if (cr4 & CR4_ENBL_BASE_INST_FLAG) printf(" FSGSBASE");
    if (cr4 & CR4_PCID_ENBL_FLAG) printf(" PCIDE");
    if (cr4 & CR4_OS_XSAVE_ENBL_FLAG) printf(" OSXSAVE");
    if (cr4 & CR4_SUPR_MODE_EXC_PROT_FLAG) printf(" SMEP");
    if (cr4 & CR4_SUPR_MODE_ACC_PREV_FLAG) printf(" SMAP");
    if (cr4 & CR4_PROT_KEY_ENBL_FLAG) printf(" PKE");
    if (cr4 & CR4_CTL_FLW_ENFORC_FLAG) printf(" CET");
    if (cr4 & CR4_ENBL_PRO_KEY_FLAG) printf(" PKS");
    if (cr4 & CR4_FLEX_RTN_ENBL_FLAG) printf(" FRED");
}

void printControlRegisters() {
    printf("\n\rControl Registers\n\r");

    printRegisterCR0(); printf("\n\r");
    printRegisterCR2(); printf("  ");
    printRegisterCR3(); printf("\n\r");
    printRegisterCR4(); printf("\n\r");
}

void printIndexRegisters() {
    printf("Index Registers: ");
    printRegisterESI();
    printf(" ");
    printRegisterEDI();
    printf("\n\r");
}

void printStackRegisters() {
    printf("Stack Registers: ");
    printRegisterEBP();
    printf(" ");
    printRegisterESP();
    printf("\n\r");
} 

void printAllRegisters() {
    
    printIndexRegisters();
    printStackRegisters();
    printRegisterEFLAGS();
    printControlRegisters();
    printf("\n\r");
}