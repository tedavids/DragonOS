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
#include <multiboot.h>

#include <kernel.h>

char *getCommand(char* restrict buffer, size_t size) {
    // check parameters
    if (!buffer) return nullptr;
    if (!size) return nullptr;

    uint16_t key = 0;
    size_t cmdidx = 0;
    char ascii = 0;

    // wait for a key (if necessary)
    while (kbdBufferEmpty());

    // do until we get to the end of the command (enter) or ESC is pressed
    // minus 2 because we need space for eh null
    while ((((char)key != '\n') && ((char)key != KBD_KEY_ESC)) && (cmdidx < (size -2))) {
        key = kbdGetKey();
        ascii = (char) key;
        // if its valid ascii (ignore control chars kinda)) TODO: make more bulletproof
        if ((ascii < 127) && (ascii > 0)) {
            printf("%c",ascii);
            buffer[cmdidx++] = ascii;
        }

        // do we wait for the next key
        if ((ascii != '\n') && (ascii != KBD_KEY_ESC) && (cmdidx < (size -2))) {
            while (kbdBufferEmpty());
        }
    }
    // terminate
    if (ascii != KBD_KEY_ESC) {
        buffer[cmdidx] = '\0';
    } else {
        buffer[0] = (char) key;
        buffer[1] = '\0';
    }

    return buffer;
}

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

   // system clock
    rtime_t time = read_system_clock();
    printf("Time: %uw/%ub/%ub %ub:%ub:%ub\n\r",
        time.year, time.month, time.day, time.hour, time.minute, time.second);

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

    // Multiboot info
    printf("Loading multiboot info...");
    if (!loadMultibootInfo()) {
        printf("Failed\n\r");
        abort();
    }
    printf("Success\n\r");

    printf("Parameters: %s\n\r", multiboot_info.cmdline);

    printf("Total Memory: 0x%xl\n\r",multiboot_info.meminfo.upper);

    // initialize paging
    printf("Initializing paging...");
    if (!initPaging()) {
        printf("Failed\n\r");
        abort();
    }
    printf("Success\n\r");


    // Initializing keyboard
    printf("Initializing keyboard...");
    if (!initKbd()) {
        printf("Failed\n\r");
        abort();
    }
    printf("Success\n\r");


    // command loop
    char command[256];

    int rtncde = 0;
    while (command[0] != KBD_KEY_ESC) {
        printf("Command> ");
        if (getCommand(command, sizeof(command))) {
            printf("\r");
            // TODO: process command
            printf("Return code: %iw\n\r", rtncde);
        }

    }
    printf("Kernel end");

    // this is the end of the kernel

    while (true);

}
