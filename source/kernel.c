// this will be my kernel

/* Copyright (C) <year> <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tty.h>
#include <ps2kbd.h>
#include <idt.h>
#include <heap.h>
#include <time.h>
#include <interupt.h>
#include <paging.h>
#include <multibootinfo.h>

#include <kernel.h>
void kernel_main() {

    // init display 
    if (!enableVGACursor(VGA_CURSOR_BLOCK)) {
        printf("Warning unable to create cursor\n\r");
    }

    // put cursor at 1
    struct cursor_pos_t cursorpos = {1,1};
    if (!set80x25cursorPos(&cursorpos)) {
        printf("Warning unable to set cursor positon\n\r");
    }

    if (!terminal_initialize()) {
        printf("Problem initializing terminal\r\n");
    }

    //  print name they can read while I do other stuff
    printf("%s Version: %s Date: %s\n\r",&OSNAME,&OSVERSION,&OSDATE);
    uint64_t counter = 0;


    printf("Boot Status: 0x%xl\r\n",BOOTSTATUS);


    // load the IDT, this MUST follow load APIC because it starts
    // interrupts again
    printf("Loading IDT...");

    if (!loadIDT()) {
        printf("FAILED!\n\r");
        abort();
    } 
    printf("Success\n\r");

        // APIC timer
    printf("Setting up APIC Timer...");
    if (!initAPICtimer()) {
        printf("Failed\n\r");
        abort();
    } 
    printf("Success\n\r");

    // turn on interrupts, everything is set up
    printf("Turning on interrupts...");
    EnableInterrupts();
    printf("Done\n\r");
/**/
    // Multiboot info
    printf("Loading multiboot info...");
    if (!loadMultibootInfo()) {
        printf("Failed\n\r");
        abort();
    }
    printf("Success\n\r");

    printf("Command line: %s\n\r", MULTIBOOT_CMDLINE);
    printf("Multiboot memory: %s\n\r",MULTIBOOT_MEMORY_AVAILABLE);
*/

    // this is the end of the kernel
    while (true) {
        counter++;
    }

}
