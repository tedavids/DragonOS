// multibootmemorymap.h
#ifndef _MULTIBOOTINFO_H
#define _MULTIBOOTINFO_H

#include <stdint.h>

struct multiboot_memory_info_t {
    uint32_t    lower;      // first MULTIBOOT_INFO_CONFIG_TABLE
    uint32_t    upper;      // actual memory size
};

struct multiboot_bios_dev_info_t {
    uint32_t    device;         // device ID
    uint32_t    partition;      // partition
    uint32_t    subpartition;   // sub-partison 
};

struct multiboot_mem_map_entry_t {
    uint64_t    baseaddr;       // base address
    uint64_t    end;            // end of the block
    uint32_t    type;           // type of memory 1 = available, 3 = ACPI, 5 is defective
                                // type 1 is read/write, type 5 should be set to unavailable
                                // all others should be read only
    uint32_t    reserved;
};

struct multiboot_mem_map_info_t {
    uint32_t                            count;          // number of entries
    uint32_t                            version;        // version for entry
    struct multiboot_mem_map_entry_t    region[10];     // if we have more than 10 decide what TODO
};

struct multiboot_info_t {
    char                                cmdline[1028];
    char                                bootloadername[256];
    struct multiboot_memory_info_t      meminfo;
    struct multiboot_bios_dev_info_t    bootdev;
    struct multiboot_mem_map_info_t     mmap;
};


extern struct multiboot_info_header_t *MULTIBOOT_INFO_ADDRESS;
extern struct multiboot_info_t  multiboot_info;

extern bool loadMultibootInfo();
#endif