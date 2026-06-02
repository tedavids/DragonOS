// gdt functions
#include <stddef.h>
#include <stdio.h>

#include <gdt.h>

// usefull defines
#define GDT_FLAG_1_BYTE         0
#define GDT_FLAG_32BIT_1BYTE    0xCF

struct gdt_entry gdt[6];    // NULL, kernel code, kernel data, user code, user data, task state

struct gdt_ptr gdtp;        // our pointer to the GDT for LGDT instruction

__attribute__((noinline))
bool setGDTEntry(struct gdt_entry *target, uint32_t limit, uint32_t beginaddr, uint8_t access, uint8_t flags) {
    // largest possible limit
    if (limit > 0xFFFFF) return false;

    printf("Parms: limit: %x addr: %X access: %x flags: %x\r\n",limit, beginaddr, access, flags);
    // TODO: rearrange to make shifting the beginaddress more efficient?
    
    target->membasehigh = (uint8_t)(beginaddr >> 24) & 0x00FF; // high byte
    target->flagslimit = (uint8_t)(limit >> 16) & 0x000F;           // top 4 bits of limit
    target->flagslimit |= flags & 0xF0; 
    target->access = access;
    target->membasemiddle = (beginaddr >> 16) & 0x00FF; // middle byte
    target->membaselow = beginaddr & 0x0000FFFF; // low 2 bytes
    target->limit = (uint16_t)(limit & 0xFFFF);
  
    // debug code
    printf("Entry: ");
    printf("limit: %x ", target->limit);
    printf("low: %x ", target->membaselow);
    printf("middle: %x ",target->membasemiddle);
    printf("access: %x ", target->access);
    printf("flagslimit: %x", target->flagslimit);
    printf("high: %x\r\n", target->membasehigh);


    return true;
} // bool setGDTEntry(uint8_t *target, struct gdt_entry source)

struct gdt_ptr *initGDT(void) {

    // null entry
     if (!setGDTEntry(&gdt[GDT_NULL_SEG], 0, 0, 0, 0)) {
        printf("Error loading GDT NULL entry\n\r");
        return nullptr;
     }
    
    // load GDT Kernel code segment
    if (!setGDTEntry(&gdt[GDT_KERNEL_CODE_SEG],0xFFFFF, 0, GDT_KERNEL_CODE, GDT_FLAG_32BIT_1BYTE)) {
        printf("Error loading GDT Kernel Code Segment");
        return nullptr;
    }

    // load GDT Kernel data segment
    if (!setGDTEntry(&gdt[GDT_KERNEL_DATA_SEG],0xFFFFF, 0, GDT_KERNEL_DATA, GDT_FLAG_32BIT_1BYTE)) {
        printf("Error loading GDT Kernel Data Segment");
        return nullptr;
    }

      // load GDT User code segment
    if (!setGDTEntry(&gdt[GDT_USER_CODE_SEG],0xFFFFF,0,GDT_USER_CODE, GDT_FLAG_32BIT_1BYTE)) {
        printf("Error loading GDT User Code Segment");
        return nullptr;
    }

    // load GDT User data segment
    if (!setGDTEntry(&gdt[GDT_USER_DATA_SEG],0xFFFFF, 0, GDT_USER_DATA, GDT_FLAG_32BIT_1BYTE)) {
        printf("Error loading GDT User Data Segment");
        return nullptr;
    }

    // load GDT Task State Scheduler segment

    if (!setGDTEntry(&gdt[GDT_TASK_STATE_SCHEDULER_SEG],0xFFFFF, 0, GDT_TASK_STATE, GDT_FLAG_32BIT_1BYTE)) {
        printf("Error loading GDT Task State Scheduler Segment");
        return nullptr;
    }

    // load GDT
    gdtp.limit = sizeof(gdt)-1;
    gdtp.gdtaddr = (uint32_t) &gdt;

    // load the gdt
    asm volatile("cli"); // turn off interrrupts
    asm volatile("lgdt %0"::"m" (gdtp));
    asm volatile("sti"); // turn on interupts

    // reload segment registers? todo: remove?
    asm volatile(
        "mov $0x10, %ax\n"
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        "mov %ax, %ss\n"
        "ljmp $0x08, $.reload_CS\n"
        ".reload_CS:\n"
        );
    return &gdtp;

} // loadGDT(void)