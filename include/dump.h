// dump.h dump command for kernel command line

#ifndef _DUMP_H
#define _DUMP_H

#include <stdint.h>

// dump an object

// Parameters:  parms -- What to dump

// Return:      0 if successful, < 0 if failure, > 0 if complete with warnings

extern int dump(const char * const restrict parms);

#endif
