// commands for the kernel command line

#include <string.h>

#include <dump.h>

#define SUCCESS     0
#define FAILURE     -1

int dump(const char * const restrict parms) {

    if (!parms) return FAILURE;

    return SUCCESS;
}