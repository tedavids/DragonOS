// print the memory map, this is part of the shell

#include <stdio.h>
#include <multiboot.h>

#include <mmap.h>

int mmap() {

    uint64_t meminmegs = multiboot_info.meminfo.upper / 0x100000;

    printf("Memory Map.  Available memory: 0x%xl (%uqM)\n\r", 
        multiboot_info.meminfo.upper, meminmegs);

    for (uint32_t i = 0; i < multiboot_info.mmap.count; i++) {
        printf("%ul: Type: %ul Begin: 0x%xq    End: 0x%xq\n\r",
            i, multiboot_info.mmap.region[i].type, 
            multiboot_info.mmap.region[i].baseaddr,
            multiboot_info.mmap.region[i].endaddr);
    }

    return 0;
}