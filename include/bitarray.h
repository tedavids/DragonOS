// These are functions to use for bit arrays

#ifndef _BITARRAY_H
#define _BITARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


// get the value of a bit (true or false)

// Parameters:  array -- The char array that has the bits
//              size -- The size of the bit array
//              index -- The index of the array you want to get
//              value -- The value of the bit

// Retuns:      True if the operation succeeds
//              value -- the value of the bit if the return code is true
//                          undefined if the return code is false
extern bool getBit(const unsigned char *array, size_t numbits, size_t index, bool *value);

// set the value for a bit (true or false) in a bit array/

// Parameters:  array -- The char array with the bits
//              size -- The size of the bit array
//              index -- The index of the array you want to get
//              oldValue -- The value of the bit before being changed

// Returns:     True if the operation succeeds
//              oldValue -- If return code is true, the value of the bit before it was chagned
//                              undefined otherwise

extern bool setBit(unsigned char *array, size_t numbits, size_t index, bool *oldValue);

// set the value fo a bit (true or false) in a bit array

// Parameters:  array -- The char array with the bits
//              numbits -- The size of the bit array
//              index -- The index of the array you want to get
//              oldValue -- The value of the bit before being changed

// Returns:     True if the operation succeeds
//              oldValue -- If return code is true, the value of the bit before it was chagned
//                              undefined otherwise

extern bool clearBit(unsigned char *array, size_t numbits, size_t index, bool *oldValue);

// set the value fo a bit (true or false) in a bit array

// Parameters:  array -- The char array with the bits
//              numbits -- The size of the bit array
//              index -- The index of the array you want to get
//              oldValue -- The value of the bit before being changed

// Returns:     True if the operation succeeds

extern bool toggleBit(unsigned char *array, size_t numbits, size_t index, bool *value);
#endif