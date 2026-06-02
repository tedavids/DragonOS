// our standard io (stdio.h) implementation
#ifndef _STDIO_H
#define _STDIO_H

#include <stdint.h>
#include "sys/cdefs.h"
#include <tty.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

// print functions
int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);


#ifdef __cplusplus
}
#endif

#endif