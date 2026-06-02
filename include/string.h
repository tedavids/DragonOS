// string.h -- our string functions
#ifndef _STRING_H
#define _STRING_H

#include "sys/cdefs.h"
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

// memory functions
extern int memcmp(const void*, const void*, size_t);
extern void* memcpy(void* __restrict, const void* __restrict, size_t);
extern void* memmove(void*, const void* , size_t);
extern void* memset(void*, int, size_t);

// string functions
extern size_t strlen(const char*);
extern char* strncpy(char* dest, const char * const src, size_t bufsize);
extern int strncmp(int len, const char* const s1, const char* const s2);
extern char* strntok(const char* const str, const char * const delim, char buffer[], int buflen);
extern char* trimc(char c, char* restrict s);
extern char* rtrimc(char c, char* restrict s);
extern char* trimd(const char* const delims, char* restrict s);
extern char* rtrimd(const char * const delims, char* restrict s);
extern bool isdelim(char c, const char* const restrict delims);

// convert to hex functions

extern char nibbleToHexCharlower(unsigned char nibble);
extern char* toHex8lower(unsigned char byte, char* buffer, size_t size);
extern char* toHex16lower(uint16_t val, char* buffer, size_t size);
extern char* toHex32lower(uint32_t val, char * buffer, size_t size);
extern char* toHex64lower(uint64_t val, char * buffer, size_t size);
extern char nibbleToHexCharupper(unsigned char nibble);
extern char* toHex8upper(unsigned char byte, char* buffer, size_t size);
extern char* toHex16upper(uint16_t val, char* buffer, size_t size);
extern char* toHex32upper(uint32_t val, char * buffer, size_t size);
extern char* toHex64upper(uint64_t val, char * buffer, size_t size);

// convert to base 10 functions

extern char* toBase10ib(int8_t val, char* buffer, size_t size);
extern char* toBase10iw(int16_t val, char* buffer, size_t size);
extern char* toBase10il(int32_t val, char* buffer, size_t size);
extern char* toBase10iq(int64_t val, char* buffer, size_t size);
extern char* toBase10ub(uint8_t val, char* buffer, size_t size);
extern char* toBase10uw(uint16_t val, char* buffer, size_t size);
extern char* toBase10ul(uint32_t val, char* buffer, size_t size);
extern char* toBase10uq(uint64_t val, char* buffer, size_t size);




#endif