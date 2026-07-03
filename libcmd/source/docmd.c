// do the command entered, this is effectively the shell

#include <string.h>
#include <mmap.h>
#include <registers.h>

#include <docmd.h>

int doCommand(const char* command) {

    char    cmd[256];
    strncpy(cmd,command, sizeof(cmd));

    rtrimd(" \n\r",cmd);
    trimc(' ', cmd);

    if (!strncmp(10, "mmap", cmd)) {
        return mmap();
    } 

    if (!strncmp(11, "registers", cmd)) {
        printAllRegisters();
    }

    return 0;
}