// multiboot.c Get information from multiboot

//#include <globals.h>
#include <kernel.h>
#include <string.h>
#include <stdio.h>
#include <multibootinfo.h>

// defines for what might be available
#define MULTIBOOT_INFO_MEMORY			0x00000001
#define MULTIBOOT_INFO_BOOTDEV			0x00000002
#define MULTIBOOT_INFO_CMDLINE			0x00000004
#define MULTIBOOT_INFO_MODS			    0x00000008
#define MULTIBOOT_INFO_AOUT_SYMS		0x00000010
#define MULTIBOOT_INFO_MEM_MAP			0x00000040
#define MULTIBOOT_INFO_DRIVE_INFO		0x00000080
#define MULTIBOOT_INFO_CONFIG_TABLE		0x00000100
#define MULTIBOOT_INFO_BOOT_LOADER_NAME	0x00000200
#define MULTIBOOT_INFO_APM_TABLE		0x00000400
#define MULTIBOOT_INFO_VBE_INFO		    0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO	0x00001000


// these are avaiable globally
uint32_t                  MMAP_AVAIL_COUNT = 0;
struct avail32_mmap_entry MMAP_AVAIL_MEM[10];
char*                     MULTIBOOT_CMDLINE = nullptr;
uint32_t                  MMAP_AVAIL_LOWMEM = 0;
uint32_t                  MMAP_AVAIL_HIMEM = 0;
uint32_t                  MULTIBOOT_BOOTDEV = 0;
char*                     MULTIBOOT_LOADERNAME;
struct multiboot_video_t  MULTIBOOT_VIDEO;

// load drive info

bool loadDriveInfo() {
    if (MULTIBOOT_INFO_ADDRESS->drives_length == 0)  return false;

    return true;
}
 
// load available memory map
bool loadAvailMMAP() {
    // check basic stuff
    if (MULTIBOOT_INFO_ADDRESS == 0x0) return false;
    if (MULTIBOOT_INFO_ADDRESS->mmap_length == 0) return false;
    if (MULTIBOOT_INFO_ADDRESS->mmap_addr == 0x0) return false;

    // we have data to process
    // get the count
    struct multiboot_mmap_entry * entry = (struct multiboot_mmap_entry *) MULTIBOOT_INFO_ADDRESS->mmap_addr;
    // make sure size if valid
    if (entry->size) {
        MMAP_AVAIL_COUNT = (MULTIBOOT_INFO_ADDRESS->mmap_length / entry[0].size);
        // transfer it to my structure
        int availcount = 0;
        for (int i = 0; i < (int) MMAP_AVAIL_COUNT; i++) {
            // its valid
            if (entry[i].size > 0) {
                // and its available memory
                if (entry[i].type == MULTIBOOT_MEMORY_AVAILABLE) {
                    MMAP_AVAIL_MEM[availcount].addr = entry[i].addrlow;
                    MMAP_AVAIL_MEM[availcount++].len = entry[i].lenlow;
                }
                //entry += entry->size; // next entry
            } // if (entry->size > 0)
        } // for (int i = 0; i < MMAP_AVAIL_COUNT; i++)
        MMAP_AVAIL_COUNT = (uint32_t) availcount;
    }

    return (MMAP_AVAIL_COUNT > 0);
}
// load basic memory information
bool loadBasicMemory() {
    MMAP_AVAIL_LOWMEM = MULTIBOOT_INFO_ADDRESS->mem_lower;
    MMAP_AVAIL_HIMEM = MULTIBOOT_INFO_ADDRESS->mem_upper;
    return true;
}

bool loadMultibootInfo() {
    if (MULTIBOOT_INFO_ADDRESS == nullptr) return false;

    // if this is null, we have nothing
    if (MULTIBOOT_INFO_ADDRESS)  {
        // load command line
        if (MULTIBOOT_INFO_ADDRESS->cmdline) MULTIBOOT_CMDLINE = rtrimc(' ', (char*) MULTIBOOT_INFO_ADDRESS->cmdline);
        // load name of loader
        if (MULTIBOOT_INFO_ADDRESS->boot_loader_name) 
            MULTIBOOT_LOADERNAME = rtrimc(' ',(char*)MULTIBOOT_INFO_ADDRESS->boot_loader_name); 
        // boot device
        if (MULTIBOOT_INFO_ADDRESS->boot_device) MULTIBOOT_BOOTDEV =  MULTIBOOT_INFO_ADDRESS->boot_device;

        // set to everything went well
        bool rtncde = true;

        // basic memory info
        if (MULTIBOOT_INFO_MEMORY && MULTIBOOT_INFO_ADDRESS->flags) {
            rtncde &= loadBasicMemory();
        }

        // load available memory map
        if (MULTIBOOT_INFO_MEM_MAP & MULTIBOOT_INFO_ADDRESS->flags) {
            rtncde &= loadAvailMMAP();
        }

        // load available drives
        if (MULTIBOOT_INFO_DRIVE_INFO & MULTIBOOT_INFO_ADDRESS->flags) {
            printf("TODO load drive info\n\r");
            rtncde &= loadDriveInfo();
        }

        return rtncde;
    } // (MULTIBOOT_INFO_ADDRESS)

    return true;
}