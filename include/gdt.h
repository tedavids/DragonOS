// gdt.h Global Descriptor Table header

#ifndef __GDT_H
#define __GDT_H

#include <stdint.h>

#define GDT_NULL_SEG                    0
#define GDT_KERNEL_CODE_SEG             1
#define GDT_KERNEL_DATA_SEG             2
#define GDT_USER_CODE_SEG               3
#define GDT_USER_DATA_SEG               4
#define GDT_TASK_STATE_SCHEDULER_SEG    5

#define GDT_PRESENT                     0b10000000
#define GDT_DPL_KERNEL                  0b00000000
#define GDT_DPL_USER                    0b01100000
#define GDT_NON_SYSSEG                  0b00010000
#define GDT_SYSTEM_SEG                  0b00000000
#define GDT_EXECUTABLE                  0b00001000
#define GDT_DATA                        0b00000000
#define GDT_DATA_GROW_UP                0b00000000
#define GDT_DATA_GROW_DOWN              0b00000100
#define GDT_READ_ONLY                   0b00000000
#define GDT_READ_WRITE                  0b00000010
#define GDT_ACCESSED                    0b00000001                

#define GDT_KERNEL_CODE                 0x9A
#define GDT_KERNEL_DATA                 0x92
#define GDT_USER_CODE                   0xFA
#define GDT_USER_DATA                   0xF2
#define GDT_TASK_STATE                  0xF2

// structures
struct __attribute__((packed)) gdt_entry {
    uint16_t    limit;          // size of memory for this segment (low 2 bytes)
    uint16_t    membaselow;     // Low bytes of where this segment starts
    uint8_t     membasemiddle;  // middle bytes of where this segment start
    uint8_t     access;         // baccess bytes
    uint8_t     flagslimit;     // flags high nibble of this segmement size
    uint8_t     membasehigh;    // High bytes of where this segment start
} ; // struct gdt_entry

// GDT register pointer (for assembler command LGDT)
struct __attribute__((packed)) gdt_ptr {
    uint16_t limit;
    uint32_t gdtaddr;
} ; // struct gdt_ptr

extern struct gdt_ptr           GDTP;
extern const uint16_t           KERNEL_CODE_SELECTOR;
extern const uint16_t           KERNEL_DATA_SELECTOR;
extern const uint16_t           USER_CODE_SELECTOR;
extern const uint16_t           USER_DATA_SELECTOR;
extern const uint16_t           TSS_SELECTOR;           // not implemented
extern const uint32_t           GDTLOADED;
extern const uint32_t           IDTLOADED;
extern const uint32_t           TSSLOADED;

#endif

