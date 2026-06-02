// multibootmemorymap.h
#ifndef _MULTIBOOTINFO_H
#define _MULTIBOOTINFO_H

#include <stdint.h>

struct multiboot_info_t {
    uint32_t flags;            // Required: Indicates which fields are valid
    uint32_t mem_lower;        // Valid if flags[0] is set
    uint32_t mem_upper;        // Valid if flags[0] is set
    uint32_t boot_device;      // Valid if flags[1] is set
    uint32_t cmdline;          // Valid if flags[2] is set
    uint32_t mods_count;       // Valid if flags[3] is set
    uint32_t mods_addr;        // Valid if flags[3] is set
    uint32_t syms[4];          // Symbol table info (flags[4] or [5] set)
    uint32_t mmap_length;      // Valid if flags[6] is set
    uint32_t mmap_addr;        // Valid if flags[6] is set
    uint32_t drives_length;    // Valid if flags[7] is set
    uint32_t drives_addr;      // Valid if flags[7] is set
    uint32_t config_table;     // ROM configuration table
    uint32_t boot_loader_name; // Bootloader name string
    uint32_t apm_table;        // APM table
    uint32_t vbe_control_info; // VBE control info
    uint32_t vbe_mode_info;    // VBE mode info
    uint32_t vbe_mode;         // VBE mode
    uint32_t vbe_interface_seg;// VBE interface seg
    uint32_t vbe_interface_off;// VBE interface off
    uint32_t vbe_interface_len;// VBE interface len
};

#define MULTIBOOT_MEMORY_AVAILABLE		        1
#define MULTIBOOT_MEMORY_RESERVED		        2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5

struct multiboot_mmap_entry {
    uint32_t size;              // Size of this entry (usually 20 or 24)
    //uint64_t addr;              // Starting physical address
    uint32_t addrlow;
    uint32_t addrhigh;
    //uint64_t len;               // Length of the region
    uint32_t lenlow;
    uint32_t lenhigh;
    uint32_t type;              // Type of region (see below)
} __attribute__((packed));

// 32 bit available memory map entry
struct avail32_mmap_entry {
    uint32_t addr;              // 32 bitt address where memory starts
    uint32_t len;               // length of available memory
};

struct multiboot_video_t {
    uint32_t    mode;
    uint32_t    width;
    uint32_t    height;
    uint32_t    color_depth;
}; 

extern const uint32_t            MMAP_LOADED; 
extern struct multiboot_info_t*  MULTIBOOT_INFO_ADDRESS;
extern uint32_t                  MMAP_AVAIL_COUNT;
extern struct avail32_mmap_entry MMAP_AVAIL_MEM[10];
extern char*                     MULTIBOOT_CMDLINE;
extern uint32_t                  MMAP_AVAIL_LOWMEM;
extern uint32_t                  MMAP_AVAIL_HIMEM;
extern uint32_t                  MULTIBOOT_BOOTDEV;
extern char*                     MULTIBOOT_LOADERNAME;
extern struct multiboot_video_t  MULTIBOOT_VIDEO;
// load multiboot info

// inputs:  None

// outputs: true if there is multiboot info
//          false if there is no multiboot info

extern bool loadMultibootInfo();
#endif