// process commands header

#ifndef _DOCMD_H
#define _DOCMD_H

#include <stdbool.h>

// execute a command
// returns true if valid command, and it completes normally
extern int doCommand(const char* command);

#endif