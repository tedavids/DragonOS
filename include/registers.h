// these are functions to get the various registers

#ifndef _REGISTERS_H
#define _REGISTERS_H

#include <stdint.h>

// define register flags
#define EFLAGS_CARRY_FLAG           0x1
#define EFLAGS_PARITY_FLAG          0x2
#define EFLAGS_AUX_FLAG             (1<<4)
#define EFLAGS_ZERO_FLAG            (1<<6)
#define EFLAGS_SIGN_FLAG            (1<<7)
#define EFLAGS_TRAP_FLAG            (1<<8)
#define EFLAGS_INT_ENABLE_FLAG      (1<<9)
#define EFLAGS_DIRECTION_FLAG       (1<<10)
#define EFLAGS_OVERFLOW_FLAG        (1<<11)
#define EFLAGS_NESTED_TASK_FLAG     (1<<14)
#define EFLAGS_RESUME_FLAG          (1<<16)
#define EFLAGS_VIRT_8086_MODE_FLAG  (1<<17)
#define EFLAGS_ALIGN_CHECK_FLAG     (1<<18)
#define EFLAGS_VIRT_INT_FLAG        (1<<19)
#define EFLAGS_VIRT_INT_PEND_FLAG   (1<<20)
#define EFLAGS_CUPID_FLAG           (1<<21)

#define CR0_PROTECTED_MODE_FLAG     0x1
#define CR0_MONITOR_COPROC_FLAG     0x2
#define CR0_X87_FPU_EMUL_FLAG       0x4
#define CR0_TASK_SWITCHED_FLAG      0x8
#define CR0_EXTENSION_TYPE_FLAG     0x10
#define CR0_NUMERIC_ERROR_FLAG      0x20
#define CR0_WRITE_PROTECT_FLAG      0x10000
#define CR0_ALIGNMENT_MASK_FLAG     0x40000
#define CR0_NOT_WRITE_THROUGH_FLAG  0x20000000
#define CR0_CACHE_DISABLE_FLAG      0x40000000
#define CR0_PAGING_FLAG             0x80000000

#define CR2_PG_LVL_WRT_THRU_FLAG    0x4
#define CR2_PG_LVL_CACHE_DIS_FLAG   0x8

#define CR4_VIRT_86_EXTENSIONS_FLAG 0x1
#define CR4_PROT_MOD_VIRT_INT_FLAG  0x2
#define CR4_TIME_STAMP_DISABLE_FLAG 0x4
#define CR4_DEBUG_EXTEN_FLAG        0x8
#define CR4_PG_SIZE_EXTEN_FLAG      0x10
#define CR4_PHYS_ADR_EXTEN_FLAG     0x20
#define CR4_MCH_CHK_EXCEPT_FLAG     0x40
#define CR4_PG_GLBL_ENABLE_FLAG     0x80
#define CR4_PERF_MON_ENABLE_FLAG    0x100
#define CR4_FXSAV_RST_SUPPORT_FLAG  0x200
#define CR4_OS_UNMSK_SIMD_FP_FLAG   0x400
#define CR4_USR_MOD_PREV_FLAG       0x800
#define CR4_5_LVL_PAGING_FLAG       0x1000
#define CR4_VRT_MCH_EXT_ENBL_FLAG   0x2000
#define CR4_SAFR_MODE_EXT_ENBL_FLAG 0x4000
#define CR4_ENBL_BASE_INST_FLAG     0x8000
#define CR4_PCID_ENBL_FLAG          0x10000
#define CR4_OS_XSAVE_ENBL_FLAG      0x20000
#define CR4_SUPR_MODE_EXC_PROT_FLAG 0x40000
#define CR4_SUPR_MODE_ACC_PREV_FLAG 0x80000
#define CR4_PROT_KEY_ENBL_FLAG      0x100000
#define CR4_CTL_FLW_ENFORC_FLAG     0x200000
#define CR4_ENBL_PRO_KEY_FLAG       0x400000
#define CR4_FLEX_RTN_ENBL_FLAG      0x100000000

// get routines for each register
extern uint32_t    getRegisterESI();
extern uint32_t    getRegisterEDI();
extern uint32_t    getRegisterESP();
extern uint32_t    getRegisterEBP();
extern uint32_t    getRegisterEFLAGS();
extern uint32_t    getRegisterCR0();
extern uint32_t    getRegisterCR2();
extern uint32_t    getRegisterCR3();
extern uint32_t    getRegisterCR4();
extern uint32_t    getRegisterCR8();


// print the registers

extern void    printRegisterESI();
extern void    printRegisterEDI();
extern void    printRegisterESP();
extern void    printRegisterEBP();
extern void    printRegisterEFLAGS();
extern void    printRegisterCR0();
extern void    printRegisterCR2();
extern void    printRegisterCR3();
extern void    printRegisterCR4();

// composit printing
extern void    printControlRegisters();
extern void    printStackRegisters();
extern void    printIndexRegisters();
extern void    printAllRegisters();
#endif