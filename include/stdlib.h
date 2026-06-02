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

void abort(void);

// "print" to a string

int snprintf(char* restrict buff, size_t buffsize, const char* restrict format, ...);

int snprintfint(char* restrict buff, size_t buffsize, const char* restrict format, va_list* parameters);

#ifdef __cplusplus
}
#endif

#endif