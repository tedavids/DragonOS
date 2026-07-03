// docommand from the shell

#ifndef _DOCMD_H
#define _DOCMD_H

#include <stdint.h>

// Process a particular command internal or external

// Parameters:  command - The command to execute

// Returns:     integer - 0 if it finishes correctly
//                        value - the return code if it doesn't
//                        value should be < 0 if failure, > 0 if completed with warnings

extern int doCommand(const char* command);

#endif