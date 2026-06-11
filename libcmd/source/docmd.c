// do a command in the command shell

#include <string.h>
#include <stdio.h>
#include <multiboot.h>

#include <docmd.h>

// print the memory map

int printmmap() {
    printf("\n\r***Memory Map***\n\r");
    uint32_t megs = multiboot_info.meminfo.upper / 0x100000;
    printf("Total memory: 0x%xl (%ulMB)\n\r\n\r", multiboot_info.meminfo.upper, megs);
    for (uint32_t i = 0; i < multiboot_info.mmap.count; i++) {
        printf("%il:    Type: 0x%xl    Begin: 0x%Xq   End: 0x%Xq\n\r",i, 
            multiboot_info.mmap.region[i].type, multiboot_info.mmap.region[i].baseaddr,
            multiboot_info.mmap.region[i].endaddr);
    }
    printf("\n\r");

    return 0;
}

int doCommand(const char* command) {
    // check we have a command
    if (!command) return 1;

    int cmdLen = (int) strlen(command);

    // blank command
    if (!cmdLen) return false;

    // start compare
    if (strncmp(cmdLen, command, "mmap")) {
        return printmmap();
    }

    // we don't have a command
    return 1;
}