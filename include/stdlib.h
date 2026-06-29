// our standard lib
#ifndef _STDLIB_H
#define _STDLIB_H

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// abort everything
__attribute__((__noreturn__))

// abort the kernel

extern void abort(void);

// "print" to a string

// return a formatted string 

// Parameters:  buff -- the output buffer
//              buffsize -- The length of the output buffer
//              format -- The format string
//              ... -- A variable list of items to fill the format 

// returns:     The lenght of the string
extern int snprintf(char* restrict buff, size_t buffsize, const char* restrict format, ...);

// return a formatted string 

// Parameters:  buff -- the output buffer
//              buffsize -- The length of the output buffer
//              format -- The format string
//              ... -- A variable list of items to fill the format 

// returns:     The lenght of the string
extern int snprintfint(char* restrict buff, size_t buffsize, const char* restrict format, va_list* parameters);

#ifdef __cplusplus
}
#endif

#endif