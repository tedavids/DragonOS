// multiboot.c Get information from multiboot

#include <stdint.h>
#include <kernel.h>
#include <string.h>
#include <stdio.h>
#include <multiboot.h>

#define END_TAG             0
#define CMDLINE_TAG         1
#define BOOT_LOADER_TAG     2
#define MEMORY_SIZES_TAG    4
#define BIOS_BOOT_DEV_TAG   5
#define MMAP_TAG            6
#define MMAP_NO_RAM         99

// this structure holds the multiboot informatino

struct multiboot_info_t  multiboot_info;

// local structures
struct tag_t {
    uint32_t type;
    uint32_t size;
};

struct tag_mem_info_t {
    uint32_t    type;
    uint32_t    size;
    uint32_t    lower;
    uint32_t    upper;
};

struct tag_boot_dev_t {
    uint32_t    type;
    uint32_t    size;
    uint32_t    biosdev;
    uint32_t    partition;
    uint32_t    subpartition;
};

struct tag_string_t {
    uint32_t    type;
    uint32_t    size;
    char        string[];
};

struct tag_mmap_entry_t {
    uint64_t    baseaddr;
    uint64_t    len;
    uint32_t    type;
    uint32_t    reserved;
};

struct tag_mmap_t {
    uint32_t            type;
    uint32_t            size;
    uint32_t            entrysize;
    uint32_t            version;
    struct tag_mmap_entry_t    entry[10];
};

struct multiboot_info_header_t {
    uint32_t    size;       // total_size
    uint32_t    reserved;   // reserved
};


// helper function
inline static uint32_t align8(uint32_t x) {
    return  ((x + 7) & ~(uint32_t)7);
}

// build the memory map

void buildMMAP(const struct tag_mmap_t* const mmap) {
    multiboot_info.mmap.version = mmap->version;
    multiboot_info.mmap.count = (mmap->size - (3 * sizeof(uint32_t))) / 
                                sizeof(struct tag_mmap_entry_t);

    // check that we don't overrun our entries
    if (multiboot_info.mmap.count > 10) {
        printf("WARNING:  More map segments than available space: %ul\n\r",
            multiboot_info.mmap.count);
        multiboot_info.mmap.count = 10;
    }

    // copy segments
    for (int i = 0; i < (int) multiboot_info.mmap.count; i++) {
        multiboot_info.mmap.region[i].baseaddr = mmap->entry[i].baseaddr;
        // memory is 0 based, length is 1 based so subtract 1
        multiboot_info.mmap.region[i].endaddr = mmap->entry[i].baseaddr + 
            mmap->entry[i].len - 1;
        multiboot_info.mmap.region[i].type = mmap->entry[i].type;
    }
}

// load multiboot information

bool loadMultibootInfo() {
    if (MULTIBOOT_INFO_ADDRESS == nullptr) return false;

    // we know our address is good
    // get the info header
    struct multiboot_info_header_t *mbi = (struct multiboot_info_header_t*) MULTIBOOT_INFO_ADDRESS;
    struct tag_t *tag = (struct tag_t*) ((uint8_t*)mbi +8);

    // load the tags
    while (tag->type != END_TAG) {
        switch (tag->type) {
            case CMDLINE_TAG: {
                struct tag_string_t *s = (struct tag_string_t *) tag;
                strncpy(multiboot_info.cmdline,s->string, sizeof(multiboot_info.cmdline));
                break;
                }
            case BOOT_LOADER_TAG: {
                struct tag_string_t *s = (struct tag_string_t*) tag;
                strncpy(multiboot_info.bootloadername, s->string, sizeof(multiboot_info.bootloadername));
                break;
                }
            case MEMORY_SIZES_TAG: {
                struct tag_mem_info_t * meminfo = (struct tag_mem_info_t*) tag;
                multiboot_info.meminfo.lower = meminfo->lower * 1024;
                multiboot_info.meminfo.upper = meminfo->upper * 1024;
                break;
                }
            case BIOS_BOOT_DEV_TAG: {
                struct tag_boot_dev_t *dev = (struct tag_boot_dev_t *) tag;
                multiboot_info.bootdev.device = dev->biosdev;
                multiboot_info.bootdev.partition = dev->partition;
                multiboot_info.bootdev.subpartition = dev->subpartition;
                break;
                }
            case MMAP_TAG: {
                buildMMAP((struct tag_mmap_t*) tag);
                break;
                }
            default:
                break;
        }

        tag = (struct tag_t *) ((uint8_t *)tag + align8(tag->size));
    }

    // fix memory map entry that is at the end of physical memory
    for (int i = 0; i < (int) multiboot_info.mmap.count; i++) {
        if (multiboot_info.mmap.region[i].baseaddr < multiboot_info.meminfo.upper) {
            if (multiboot_info.mmap.region[i].endaddr > multiboot_info.meminfo.upper) {
                // remember addesses are 0 based, lengths are 1 based
                multiboot_info.mmap.region[i].endaddr = multiboot_info.meminfo.upper - 1;
            }
        }
    }

    return true;
}