// string.h -- our string functions
#ifndef _STRING_H
#define _STRING_H

#include "sys/cdefs.h"
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

// memory functions
// compare memory

// Parameters:  aptr - pointer to the first memory block
//              bptr - pointer to the second memory block
//              size - Number of bytes to compare

// returns:     if a < b returns -1, if a = b returns 0, if a > b returns 1
//              if aptr is null, return -999, if bptr is null return 999
extern int memcmp(const void* aptr, const void* bptr, size_t size);

// copy memory
// Parameters:  dstptr -- a pointer to the destination of the copy
//              srcptr -- a pointer to the source of the copy
//              size -- the number of bytes to copy

//  Returns:    a pointer to the destination, or null ptr on error
extern void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size);

// move memory
// Parameters:  dstptr -- A pointer to the destination
//              srcptr -- a pointer to the source
//              size -- number of bytes to move

// Returns:     A pointer to the destination, or nullptr if an error occurs
extern void* memmove(void* dstptr, const void* srcptr, size_t size);

// set memory to a value
// Parameters:  buffptr -- a pointer to the memory bufffer
//              value -- The value to set memory to
//              size -- The number of bytes to set

// returns:     a pointer to the destination, or nullptr if an error occurrs
extern void* memset(void* buffptr, int value, size_t size);


// string functions
// get the length of a string
// Parameters:  str -- the string to check

// returns:     the length of the string, or 0 if str is null
extern size_t strlen(const char* str);

// copy a string
// Parameters:  dest -- a pointer to the destination
//              src -- a pointer to the source string
//              bufsize -- the length of the destination buffer, including the null at the end

// returns:     a pointer to the destination, or nullptr on error
extern char* strncpy(char* dest, const char * const src, size_t bufsize);

// compare two strings
// Parameters:  len -- maximum number of bytes to compare
//              s1 -- a pointer to the first string
//              s2 -- a pointer to the second string

// returns:     if s1 < s2 return -1, if s1 = s2 return 0, if s1 > s2 return 1
//              if both s1 and s2 are null return 0, if s1 is null return 1, if s2 is null return -1
extern int strncmp(int len, const char* const s1, const char* const s2);

// Get a series of substrings
// as long as the string remains the same gets the next token
// Parameters:  str -- The string to scan
//              delim -- a list of characters that are delimiterssssssss
//              buffer -- a pointer to the destination
//              buflen -- the length of the buffer, including the trailing null
// Returns:     a pointer to the buffer, or null ptr if end of string or an error occurs
extern char* strntok(const char* const str, const char * const delim, char *buffer, int buflen);

// trim leading characters
// Parameters:  c - the character to trim
//              s - the string to trim

// Returns:     a pointer to the new string, or nullptr on error
extern char* trimc(char c, char* restrict s);

// trim trailing characters from a string

// Parameters:  c - The char to trim
//              s - a pionter to the string to trim

// Return:      a pointer to the new string, or nullptr on error
extern char* rtrimc(char c, char* restrict s);

// trim leading characters
// Parameters:  delims - the character(s) to trim
//              s - the string to trim

// Returns:     a pointer to the new string, or nullptr on error
extern char* trimd(const char* const delims, char* restrict s);

// trim trailing characters from a string

// Parameters:  delims - The char(s) to trim
//              s - a pionter to the string to trim

// Return:      a pointer to the new string, or nullptr on error
extern char* rtrimd(const char * const delims, char* restrict s);

// check if a character is in a list of delimiters

// Parameters:  c -- The character to check
//              delims -- a list of delimiters

// return:      true if c is in the list, false otherwise
extern bool isdelim(char c, const char* const restrict delims);

// convert to hex functions

// get the hex value of a nibble (4 bits) in lower case

// Parameters:  nibble - The nibble you want the characer for

// Returns:     A character (lower case) representing the value of the 4 bits

extern char nibbleToHexCharlower(unsigned char nibble);

// get the hex value of a byte

// parameters:  byte -- The byte you want the value for
//              buffer -- Returns the value in hex
//              size -- The length of the buffer

// Returns:     A pointer to the buffer, or nullptr if the buffer is too small
extern char* toHex8lower(unsigned char byte, char* buffer, size_t size);

// get the hex value of a 16 bit number in lower case 

// Parameters:  val -- the 16 bit value
//              buffer -- The return buffer
//              size -- The size of the buffer

// Returns:     A pointer to the buffer, or nullptr if the buffer is too short
extern char* toHex16lower(uint16_t val, char* buffer, size_t size);

// get the hex value of a 32 bit number in lower case 

// Parameters:  val -- the 32 bit value
//              buffer -- The return buffer
//              size -- The size of the buffer

// Returns:     A pointer to the buffer, or nullptr if the buffer is too short
extern char* toHex32lower(uint32_t val, char * buffer, size_t size);

// get the hex value of a 64 bit number in lower case 

// Parameters:  val -- the 64 bit value
//              buffer -- The return buffer
//              size -- The size of the buffer

// Returns:     A pointer to the buffer, or nullptr if the buffer is too short
extern char* toHex64lower(uint64_t val, char * buffer, size_t size);

// get the hex value of a nibble (4 bits) in upper case

// Parameters:  nibble - The nibble you want the characer for

// Returns:     A character (upper case) representing the value of the 4 bits
extern char nibbleToHexCharupper(unsigned char nibble);

// get the hex value of a byte

// parameters:  byte -- The byte you want the value for
//              buffer -- Returns the value in hex
//              size -- The length of the buffer

// Returns:     A pointer to the buffer, or nullptr if the buffer is too small
extern char* toHex8upper(unsigned char byte, char* buffer, size_t size);

// get the hex value of a 16 bit number in upper case 

// Parameters:  val -- the 16 bit value
//              buffer -- The return buffer
//              size -- The size of the buffer

// Returns:     A pointer to the buffer, or nullptr if the buffer is too short
extern char* toHex16upper(uint16_t val, char* buffer, size_t size);

// get the hex value of a 32 bit number in upper case 

// Parameters:  val -- the 32 bit value
//              buffer -- The return buffer
//              size -- The size of the buffer

// Returns:     A pointer to the buffer, or nullptr if the buffer is too short
extern char* toHex32upper(uint32_t val, char * buffer, size_t size);

// get the hex value of a 64 bit number in upper case 

// Parameters:  val -- the 64 bit value
//              buffer -- The return buffer
//              size -- The size of the buffer

// Returns:     A pointer to the buffer, or nullptr if the buffer is too short
extern char* toHex64upper(uint64_t val, char * buffer, size_t size);

// convert to base 10 functions

// converts a byte to a base 10 character string
// Parameters:  val - The value to convert
//              buffer -- The buffer for the string
//              size -- The size of the buffer

// Returns:     a pointer to the buffer, or nullptr on an error
extern char* toBase10ib(int8_t val, char* buffer, size_t size);

// converts a 16 bit integer to a base 10 character string
// Parameters:  val - The value to convert
//              buffer -- The buffer for the string
//              size -- The size of the buffer

// Returns:     a pointer to the buffer, or nullptr on an error
extern char* toBase10iw(int16_t val, char* buffer, size_t size);

// converts a 32 bit integer to a base 10 character string
// Parameters:  val - The value to convert
//              buffer -- The buffer for the string
//              size -- The size of the buffer

// Returns:     a pointer to the buffer, or nullptr on an error
extern char* toBase10il(int32_t val, char* buffer, size_t size);

// converts a 64 bit integer to a base 10 character string
// Parameters:  val - The value to convert
//              buffer -- The buffer for the string
//              size -- The size of the buffer

// Returns:     a pointer to the buffer, or nullptr on an error
extern char* toBase10iq(int64_t val, char* buffer, size_t size);

// converts an unsigned byte to a base 10 character string
// Parameters:  val - The value to convert
//              buffer -- The buffer for the string
//              size -- The size of the buffer

// Returns:     a pointer to the buffer, or nullptr on an error
extern char* toBase10ub(uint8_t val, char* buffer, size_t size);

// converts a 16 bit unsigned integer to a base 10 character string
// Parameters:  val - The value to convert
//              buffer -- The buffer for the string
//              size -- The size of the buffer

// Returns:     a pointer to the buffer, or nullptr on an error
extern char* toBase10uw(uint16_t val, char* buffer, size_t size);

// converts a 32 bit unsigned integer to a base 10 character string
// Parameters:  val - The value to convert
//              buffer -- The buffer for the string
//              size -- The size of the buffer

// Returns:     a pointer to the buffer, or nullptr on an error
extern char* toBase10ul(uint32_t val, char* buffer, size_t size);

// converts a 64 bit unsigned integer to a base 10 character string
// Parameters:  val - The value to convert
//              buffer -- The buffer for the string
//              size -- The size of the buffer

// Returns:     a pointer to the buffer, or nullptr on an error
extern char* toBase10uq(uint64_t val, char* buffer, size_t size);




#endif